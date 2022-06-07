#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "leercadena.h"

int main(int argc, char* argv[]) {
        char command[256];
        char **comando;
	int i;
	int background;
	int tareas[5];
	int colaTareas = 0;
        while (1) {
                printf("> ");
                leer_de_teclado(256, command);
                if (strcmp(command,"salir") == 0) break;
		if (strcmp(command,"tareas")==0){
		for(int c = 0 ; c<colaTareas;c++){
                        printf("%d \n",tareas[c]);
                        }continue;
		}
        comando = de_cadena_a_vector(command);
		i = 0;
		background = 0;
		if (strcmp(comando[0],"detener")==0){
		comando[0] = "kill";
		for(int w = 0; w<colaTareas;w++){
		if(tareas[w]==(int)comando[1])
		{
			tareas[w] = '\0';
			break;
		}
		}
		}
		while (comando[i]){
		i++;
		}
		if(strcmp(comando[i-1],"&")==0)
		{
		background = 1;
		comando[i-1] = '\0';
		}
                int rc = fork();
                assert(rc >= 0);
                if (rc == 0)
                        execvp(comando[0], comando);
                else{
			if(background == 1){
			tareas[colaTareas]=rc;
			colaTareas++;
			}
			else
                        	wait(NULL);
        	}
	}
        return 0;
}
