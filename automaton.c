#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATE_NAME_SIZE 10

typedef struct State State;

#pragma region StringUtils
    void remove_all_newlines(char *str) {
        int i = 0; // Reads through the original string
        int j = 0; // Writes the cleaned string
        
        while (str[i] != '\0') {
            if (str[i] != '\n') {
                str[j] = str[i];
                j++;
            }
            i++;
        }
        str[j] = '\0'; // Properly terminate the shortened string
    }
#pragma endregion

int sizeOf_alphabet = 0;
char *alphabet = NULL;
char * fita;
int tamanhoFita;
int n_of_states = 0;
State ** states;

#pragma region Structs

    #pragma region Stack
    typedef struct Node Node;
    struct Node{
        char value;
        Node * next;
    };

    typedef struct Stack
    {
        Node * top;
        int length;
    } Stack;

    int isEmpty(Stack * stack){
        return stack->length < 1;
    }

    char * getStringFromStack(Stack * stack){
        if (isEmpty(stack) || stack == NULL)
            return NULL;
        char * newChar = malloc(sizeof(char) * (stack->length + 1)); //Needs extra space for '\0'
        if (newChar == NULL) 
            return NULL;
        Node * currentNode = stack->top;
        for (int i = 0; i < stack->length; i++){
            newChar[i] = currentNode->value;
            currentNode = currentNode->next;
        }
        newChar[stack->length] = '\0';
        return newChar;
    }

    void add(Stack * stack, char value){
        Node * n = malloc(sizeof(Node));

        n->value = value;
        if (stack->top != NULL)
            n->next = stack->top;
        else
            n->next = NULL;

        stack->top = n;
        stack->length++;
    }

    char pop(Stack * stack){
        if (stack->top == NULL)
            return '\0';
        //Guardar o nó que vai sair e seu valor
        Node * temp = stack->top;
        char r = temp->value;

        //O topo vira o suscessor do nó que saiu 
        stack->top = temp->next;
        stack->length--;

        free(temp);
        return r;
    }

    Stack * stack(){
        Stack * s = malloc(sizeof(Stack));
        s->length = 0;
        s->top = NULL;
        return s;
    }

    int DestroyStack(Stack * stack){
        if (stack == NULL) return 0;
        //Acessa o proximo
        Node * current = stack->top;
        while (current != NULL) {
            Node * next_node = current->next;
            free(current);
            current = next_node;
        }
        free(stack);
        return 1;
    }
    #pragma endregion

    #pragma region Transition
    typedef struct Transition
    {
        int to;
        char condition;
        char read;
        char write;
    } Transition;

    Transition * transition(int to, char condition, char read, char write){
        Transition * t = malloc(sizeof(Transition));
        t->to = to;
        t->condition = condition;
        t->read = read;
        t->write = write;
        return t;
    }    // Função auxiliar recursiva que copia os nós mantendo a ordem correta
    Node * cloneNodes(Node * originalNode) {
        if (originalNode == NULL) {
            return NULL;
        }

        // Aloca o novo nó correspondente ao nó atual
        Node * newNode = malloc(sizeof(Node));
        if (newNode == NULL) return NULL; // Proteção contra falta de memória

        newNode->value = originalNode->value;
        
        // Avança até o fim e conecta o próximo nó na volta da recursão
        newNode->next = cloneNodes(originalNode->next);

        return newNode;
    }

    // Função principal que gera a nova pilha alocada
    Stack * cloneStack(Stack * originalStack) {
        if (originalStack == NULL) {
            return NULL;
        }

        // Aloca o cabeçalho da nova pilha
        Stack * newStack = malloc(sizeof(Stack));
        if (newStack == NULL) {
            return NULL;
        }

        // Copia os metadados
        newStack->length = originalStack->length;
        
        // Chama a função auxiliar para clonar a cadeia de nós
        newStack->top = cloneNodes(originalStack->top);

        return newStack;
    }
    #pragma endregion

    #pragma region State

    struct State
    {
        char name[MAX_STATE_NAME_SIZE]; 
        int numbOfTransition;
        Transition *transition;
        int isFinal;
    };

    State * create_state(char name[MAX_STATE_NAME_SIZE]){
        State * s = malloc(sizeof(State));
        strcpy(s->name, name);
        s->numbOfTransition = 0;
        s->transition = NULL;
        s->isFinal = 0;
        return s;
    }
    #pragma endregion

#pragma endregion

#pragma region Prints

    void printTransition(Transition transition){
        printf("->%s", states[transition.to]->name);
        printf("(%c,%c,%c) ", transition.condition, transition.read, transition.write);
    }

    void printState(State * state){
        printf("Name: %s", state->name);
        if (state->isFinal)
            printf(" : estado final\n");
        else 
            printf("\n");

        for (int i = 0; i < state->numbOfTransition; i++){
            printTransition(state->transition[i]);
        }
        printf("\n");
    }

    void printStates(){
    for (int i = 0; i < n_of_states; i++){
        printState(states[i]);
    }
}

#pragma endregion

void lerLista(char *lista, int tamanho){
    for (int i = 0; i < tamanho; i++)
        scanf("%c", &lista[i]);
}

int GetStateIDByName(char * nameKey){
    for (int i = 0; i < n_of_states; i++){
        if (strcmp(states[i]->name, nameKey) == 0){
            return i;
        }
    }
    printf("There is no state named '%s'", nameKey);
    return -1;
}

int _simularAutomato(int posicaoFita, int estadoAtual, Stack * pilha){
    State * currentState = states[estadoAtual];
    if (currentState == NULL) {
        printf("ERRO ESTADO NULLO");
        return 0;
    }
    printf(
        "Fita: %s \t Estado atual: %s \t Pilha: %s\n", ((fita[posicaoFita] == '\0')? "\t" : (&fita[posicaoFita])), currentState->name, getStringFromStack(pilha)
    );
    
    if (posicaoFita == tamanhoFita){
        if (currentState->isFinal)
            return 1; //Palavra aceita
    }
    for (int i = 0; i < currentState->numbOfTransition; i++){
        // printf("Estado: %s i: %i \n", currentState->name, i);
        char currentChar = fita[posicaoFita];
        char condition = currentState->transition[i].condition;
        char read = currentState->transition[i].read;
        char write = currentState->transition[i].write;
        
        int hasCondition = condition != '-';
        //Nada a ler
        if (read == '-'){
            if (condition == currentChar || !hasCondition){
                Stack * newStack = cloneStack(pilha);
                if (write != '-') add(newStack, write);
                if (_simularAutomato(posicaoFita + hasCondition, currentState->transition[i].to, newStack) 
                    & DestroyStack(newStack))
                    return 1;
            }
        }
        else if (read == '?'){
            if (isEmpty(pilha) && (condition == currentChar || !hasCondition)){
                Stack * newStack = cloneStack(pilha);
                if (write != '-') add(newStack, write);
                if (_simularAutomato(posicaoFita + hasCondition, currentState->transition[i].to, newStack) 
                    & DestroyStack(newStack))
                    return 1;
            }
        }
        //Algo a ler
        else{
            //A palavra é igual
            if (isEmpty(pilha)) continue;
            int isEqual = read == pilha->top->value;
            if (isEqual){
                Stack * newStack = cloneStack(pilha);
                pop(newStack);
                if (write != '-') add(newStack, write);
                if (_simularAutomato(posicaoFita + hasCondition, currentState->transition[i].to, newStack) 
                    & DestroyStack(newStack))
                    return 1;
            }
        }
    }
    return 0;
}

void simularAutomato(){
    printf("o====================================================o\n");
    printf("|                Simulando  automato                 |\n");
    printf("o====================================================o\n");
    Stack * pilha = stack();
    int resultado = _simularAutomato(0,0, pilha);
    DestroyStack(pilha);
    if (resultado == 0)
        printf("Palavra nao reconhecida.\n");
    else
        printf("Palavra reconhecida.\n");
}

int main() {
    alphabet = NULL;
    FILE *arquivo = fopen("linha.txt", "r");
    char linha[256]; // Define o tamanho máximo de cada linha

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int nLinha = 0;
    int flag = 1;
    while (flag && fgets(linha, sizeof(linha), arquivo)) {
        remove_all_newlines(linha);
        printf("Processando linha %i: %s\n", nLinha, linha);
        switch (nLinha)
        {
        case 0:{
            char *token = strtok(linha, " ");
            while (token != NULL) {
                printf("Estado definido: %s\n", token);
                n_of_states++;

                if (states == NULL)
                    states = malloc(sizeof(State) * n_of_states);
                else
                    states = realloc(states, sizeof(State) * n_of_states);
                
                states[n_of_states - 1] = create_state(token);

                printf("Nome definido?: %s\n", states[n_of_states - 1]->name);
                token = strtok(NULL, " ");
            }
        }
        break;
        case 1:{
            char *token = strtok(linha, " ");
            while (token != NULL) {
                printf("Estado definido como final: %s\n", token);
                states[GetStateIDByName(token)]->isFinal = 1;
                token = strtok(NULL, " ");
            }
        }break;
        case 2: {
            char *token = strtok(linha, " ");
            while (token != NULL) {
                char letra = token[0];
                printf("Definindo letra: %c\n", letra);

                // 1. Validação de caracteres reservados
                if (letra == '?' || letra == '-') {
                    printf("Erro, letra reservada nao pode fazer parte do alfabeto, nao use '?' ou '-'\n");
                    return 0;
                }

                // 2. Validação de letra repetida (Busca linear no alfabeto já existente)
                for (int i = 0; i < sizeOf_alphabet; i++) {
                    if (alphabet[i] == letra) {
                        printf("Erro, letra repetida: %c\n", letra);
                        return 0;
                    }
                }

                // 3. Alocação de memória segura
                sizeOf_alphabet++;
                char *temp = realloc(alphabet, sizeof(char) * sizeOf_alphabet);
                if (temp == NULL) {
                    printf("Erro: Falha na alocacao de memoria.\n");
                    return 0;
                }
                alphabet = temp;

                // 4. Inserção do novo caractere
                alphabet[sizeOf_alphabet - 1] = letra;

                token = strtok(NULL, " ");
            } 
        } break;
        case 3: {
            char *token = strtok(linha, " ");
            tamanhoFita = strlen(token);
            fita = malloc(tamanhoFita * sizeof(char));
            strcpy(fita, token);
            printf("Palavra definida: %s \n", fita);
        } break;
        default: {
            char *token = strtok(linha, " ");
            if (token == NULL) flag = 0;
            while (token != NULL) {
                char fromStateName[50];
                char toStateName[50];
                
                char readFromTape;
                char readFromStack;
                char writeOnStack;

                int lidos = sscanf(token, "%[^>]>%[^:]:(%c,%c,%c)", 
                                   fromStateName, toStateName, &readFromTape, &readFromStack, &writeOnStack);

                if (lidos == 5) {
                    printf("Estado '%s' ", fromStateName);
                    printf("Vai para: '%s' \n", toStateName);
                    printf("Assinatura (%c,%c,%c)\n", readFromTape, readFromStack, writeOnStack);
                    
                    State * stateOrigin = states[GetStateIDByName(fromStateName)];
                    int toState = GetStateIDByName(toStateName);

                    stateOrigin->numbOfTransition++;
                    int newSize = sizeof(Transition) * stateOrigin->numbOfTransition;
                    
                    if (stateOrigin->transition == NULL)
                        stateOrigin->transition = malloc(newSize);
                    else
                        stateOrigin->transition = realloc(stateOrigin->transition, newSize);

                    Transition * currentTransition = &(stateOrigin->transition[stateOrigin->numbOfTransition - 1]);
                    currentTransition->to = toState;
                    currentTransition->condition = readFromTape;
                    currentTransition->read = readFromStack;
                    currentTransition->write = writeOnStack;
                }
                else {
                    printf("Erro ao processar a transicao: %s\n", token);
                }
                token = strtok(NULL, " ");
            } 
        }
        break;
        }
        nLinha++;
    }
    fclose(arquivo);
    printStates();
    simularAutomato();
    return 0;
}
