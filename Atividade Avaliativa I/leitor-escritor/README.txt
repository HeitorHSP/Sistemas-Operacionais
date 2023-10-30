Esse código é uma implementação em C do problema clássico dos leitores e escritores com controle de concorrência. O problema envolve múltiplos threads (leitores e escritores) acessando um recurso compartilhado (uma variável global) ao mesmo tempo, enquanto garantem a consistência dos dados. O código utiliza primitivas de sincronização, como mutexes e variáveis de condição, para garantir que os threads sigam as regras apropriadas. Vou explicar as partes principais do código:

Variáveis Globais: O código possui várias variáveis globais para controlar o comportamento dos threads, como num_leituras, num_escritas, quant_leitoras, quant_escritoras, var_global, log_file, mutex, cond_leit, cond_escr, escr_fila, leit_fila, lendo, escrevendo e vez.

Função EntraLeitura: Esta função é chamada por um thread leitor quando deseja ler o recurso compartilhado. Ela adquire o mutex, verifica se há escritores ativos ou na fila de espera, e se sim, aguarda. Em seguida, registra que está lendo, escreve mensagens de log e libera o mutex. Além disso, cria e escreve em um arquivo com o nome correspondente ao ID do leitor.

Função SaiLeitura: Esta função é chamada por um thread leitor quando termina de ler o recurso compartilhado. Ela adquire o mutex, registra que o leitor saiu e, se nenhum leitor estiver lendo, permite que escritores sejam executados (definindo vez para 0) e sinaliza um escritor que estava aguardando.

Função EntraEscrita: Semelhante à função EntraLeitura, mas para escritores. Verifica se há leitores ou escritores ativos ou na fila de espera, e se sim, aguarda. Registra que um escritor entrou, lê o valor atual da variável global, atualiza o valor da variável global e escreve mensagens de log.

Função SaiEscrita: Chamada por um escritor quando termina a escrita. Registra que o escritor saiu e, se não houver mais escritores ativos, permite que os leitores sejam executados (definindo vez para 1) e sinaliza todos os leitores que estavam aguardando.

Funções Escritora e Leitora: Essas funções são os pontos de entrada dos threads escritores e leitores. Eles repetem o número de leituras ou escritas especificado, chamando as funções EntraEscrita, SaiEscrita, EntraLeitura e SaiLeitura.

Função main: No main, as variáveis globais são inicializadas, incluindo mutexes e variáveis de condição. Em seguida, ele cria threads para leitores e escritores e aguarda que todos eles terminem.

O código é uma implementação clara do problema dos leitores e escritores, mas tenha em mente que a sincronização é feita em nível de thread em vez de processos. Além disso, é importante observar que a alocação de memória dinâmica com malloc não é desalocada (falta free) no código, o que pode causar vazamento de memória. Certifique-se de desalocar a memória alocada dinamicamente apropriadamente em uma aplicação em produção.

comando para rodar:
gcc main.c -o main -Wall -lpthread