Um simulador de automatos com pilha, altere quintopla.txt dentro do projeto e execute o codigo para simular o automato.
Cada linha separando termos por espaço seguindo a sequencia:

1.Estados [O primeiro estado da lista é o estado inicial]
2.Estados finais
3.Alfabeto
+4.Transições seguindo a notação q1>q2:(A,B,C) onde A é a leitura da fita, B a leitura da pilha e C a gravação da pilha

Exemplo:
q0 q1 q2 q3 qf
qf
a b c d
aaabbbbccccddd
q0>q0:(a,-,A)
q0>q1:(b,-,B)
q1>q1:(b,-,B)
q1>q2:(c,B,-)
q2>q2:(c,B,-)
q2>q3:(d,A,-)
q3>q3:(d,A,-)
q3>qf:(-,-,-)
