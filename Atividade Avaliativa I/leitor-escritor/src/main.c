#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int num_leituras, num_escritas, quant_leitores, quant_escritores;
int var_global;
FILE * log_file;
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;
int escr_fila = 0, leit_fila = 0, lendo = 0, escrevendo = 0;

int vez = 0; // escr = 0 | leit = 1

void EntraLeitura(int id){

	pthread_mutex_lock(&mutex);
	while (escrevendo > 0 && !vez) {
		leit_fila++;
		pthread_cond_wait(&cond_leit, &mutex);
		leit_fila--;
	}
	lendo++;
	fprintf(log_file, "Leit %d entrou\n", id);
	fprintf(log_file, "Leit %d leu %d\n", id, var_global);
	pthread_mutex_unlock(&mutex);
	

	FILE *leit_file;
	char *leit_filename;

	leit_file = calloc(sizeof(FILE*),1);
	leit_filename = calloc(sizeof(char),20);

	sprintf(leit_filename,"%d.txt",id);
	leit_file = fopen(leit_filename, "a+");
	fprintf(leit_file,"%d\n",var_global);
	fclose(leit_file);

	free(leit_filename);
}

//escr, ler
void SaiLeitura(int id){
	pthread_mutex_lock(&mutex);
	lendo--;
	fprintf(log_file, "Leit %d saiu\n", id);
	if (lendo == 0) {
		vez = 0;
		pthread_cond_signal(&cond_escr);
	}	
	pthread_mutex_unlock(&mutex);
}

void EntraEscrita(int id){
	while (lendo > 0 && vez) {
		escr_fila++;
		pthread_cond_wait(&cond_escr, &mutex);
		escr_fila--;
	}
	escrevendo++;
	fprintf(log_file, "Esc %d entrou\n", id);
	fprintf(log_file, "Esc %d leu %d\n", id, var_global);
	var_global = id;
	fprintf(log_file, "Esc %d escreveu %d\n", id, var_global);	
	
}

void SaiEscrita(int id){
	escrevendo--;
	if(escrevendo == 0) {
		vez = 1;
		//pthread_cond_signal(&cond_leit);
	}
	fprintf(log_file, "Esc %d saiu\n", id);
	//pthread_cond_broadcast(&cond_leit);
}	

void * Escritora ( void * arg ){
	int * id = (int *) arg;
	int escritas = num_escritas;
	while (escritas > 0) {
		escritas--;
		/*if (escritas < 0){
			pthread_cond_broadcast(&cond_leit);
			break;
		} else {*/
			pthread_mutex_lock(&mutex);
			EntraEscrita(*id);
			SaiEscrita(*id);
			pthread_mutex_unlock(&mutex);
		//}
	}
	pthread_exit(NULL);
}

void * Leitora( void * arg ){
	int * id = (int *) arg;
	int leituras = num_leituras;
	while (leituras > 0) {
		leituras--;
        EntraLeitura(*id);
        SaiLeitura(*id);
	}
	pthread_exit(NULL);
}

int main(){
	quant_leitores = 5;
	quant_escritores = 5;
	num_leituras = 3;
	num_escritas = 2;
	log_file = fopen("log","w+");

	pthread_cond_init(&cond_escr, NULL);
	pthread_cond_init(&cond_leit, NULL);

	pthread_mutex_init(&mutex, NULL);

	int i, * t;
	pthread_t threads[quant_leitores + quant_escritores];

	for (i = 0; i < (quant_leitores + quant_escritores); i++){
		t = malloc(sizeof(int));
		*t = i;
		
		if ( i < quant_leitores){
			printf("Leitor : %d", *t);         
			pthread_create( &threads[i], NULL, Leitora, (void *) t);
		}else 
			pthread_create( &threads[i], NULL, Escritora, (void *) t);
	}

	for (i = 0; i < quant_leitores + quant_escritores; i++)
		pthread_join(threads[i], NULL);

	return 0;
}