Todo código fonte está disponível no arquivo server.cpp

Para compilação do projeto demanda a realização da seguinte linha de comando:
g++ server.cpp -o server -lboost_system -std=c++11

Após, basta rodas o servidor:
./server

Os testes realizados foram feitos seguindo o seguinte exemplo:
*Cliente
 netcat 127.0.0.1 3337
 <frases aleatorias> Hello world!
 <frases aleatorias> Olá meu nome é Tasso Barbosa
 <frases aleatorias> teste
 <frases aleatorias> teste curto
 <frases aleatorias> 1 2 3

 Para a conclusão do projeto foram utilizadas as seguintes bibliotecas:
 *BOOST: Esta biblioteca foi responsável por todo o protótipo do Servidor,
 bem como toda a sua comunicação assíncrona e gerenciamento de múltiplas sessões.
 Assim como foi pedido consegui através dela utilizar o protocolo TCP e tornar
 o projeto robusto. Vale ressaltar que o módulo ASIO foi o mais requisitado
 de toda biblioteca.

 *FSTREAM: Esta biblioteca foi extremamente útil e responsável por todas as ações
 que necessitavam a realização de acessos à arquivos externos. Foi com ela que
 consegui extrair os dados do config.txt para reutiliza-los no código e também
 para criar os arquivos locais dos dados recebidos do cliente.

 **Outras bibliotecas inseridas nos "include" foram de segunda importancia
 servindo apenas como auxiliares em concatenações e movimentações de dados temporários.

 Tempo gasto:
 Podendo ser checado através dos logs que utilizei no git, acredito
 que gastei cerca 5:30 para codificar tudo, lembrando que boa parte deste tempo
 também foi gasto pensando na realização de todas as tarefas, mesmo as que
 eu não realizei. Ainda tive um tempo médio de 2:30 para estudo e compreensão
 do que realmente era necessário e traria maior impacto ao projeto, com isso
 quero dizer que as práticas de trazer maiores funcionalidades em menor tempo
 foram utilizadas, já que haviam apenas 3 dias para conclusão de todo projeto.

 Dificuldades:
 Encontrei reais dificuldades na tentativa de utilização do steady_timer, uma vez
 que de maneira alguma eu conseguia compilar nem os exemplos básicos encontrados
 nos tutorias. Penso eu que havia alguma incompatibilidade de versão com o sistema
 no qual estou usando e portanto tive que deixar de lado essa funcionalidade. Pensei
 em utilizar outras bibliotecas, como "time" e função "clock", mas durante a codificação
 tive que optar definitivamente por deixa-la de lado visando o bom progresso do projeto.
 Também tive dificuldade na conclusão de outra funcionalidade, a que possibilita dividir
 os arquivos caso o tamanho tenha excedido, e mais uma vez tive que optar por deixar
 essa funcionalidade em branco.
 Essas foram as únicas duas funcionalidades que não consegui concluir, e de uma
 maneira geral acredito que me saí muito bem na conclusão do projeto como um todo.
 De 10 requisitos pedidos consegui concluir 8 deles, lembrando que nunca havia
 utilizado a biblioteca BOOST, porém isso não foi uma dificuldade dado que houve
 tempo suficiente para estudar e compreender seu uso.
