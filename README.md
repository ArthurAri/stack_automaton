Um simulador de automatos com pilha, altere quintopla.txt dentro do projeto e execute o codigo para simular o automato.
Cada linha separando termos por espaço seguindo a sequencia:
```
1.Estados [O primeiro estado da lista é o estado inicial]
2.Estados finais
3.Alfabeto
+4.Transições seguindo a notação q1>q2:(A,B,C) onde A é a leitura da fita, B a leitura da pilha e C a gravação da pilha
```
Exemplo:
```txt
q0 q1 q2 q3 qf<br>
qf<br>
a b c d<br>
aaabbbbccccddd<br>
q0>q0:(a,-,A)<br>
q0>q1:(b,-,B)<br>
q1>q1:(b,-,B)<br>
q1>q2:(c,B,-)<br>
q2>q2:(c,B,-)<br>
q2>q3:(d,A,-)<br>
q3>q3:(d,A,-)<br>
q3>qf:(-,-,-)<br>
```
