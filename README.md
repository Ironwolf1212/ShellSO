Documentación Shell

Este proyecto utiliza dos clases para realizar la tarea interprete de comandos :

*Tenemos tenemos nuestra clase "leercadena.h" que define las funciones que están proveemos después a leercadena.c.

*Después "leercadena.c" que nos provee de la funcion leer_de_teclado(int size, char* cadena), gracias a esta funcion es que podemos generar un vector que nos separa los comandos y los parametros de este.

Nuestro método principal se encuentra en mishell.c, esta clase hace uso de algunas funciones declaradas en leercadena.h, tales como leer_de_teclado() y de_cadena_a_vector().

Al inicio del método se declaran las variables a utilizar:

char command[256]; // Búfer
char **comando; // vector de string
int i; // contador
int background; // flag
int tareas[5]; // vector de int
int colaTareas = 0; // contador

Posteriormente entramos a un ciclo que se repite de manera indefinida, esto para que nuestro intérprete de comandos siempre esté pidiendo la siguiente instrucción

while(1){
... //instrucciones a repetir en el ciclo
}

Dentro de el ciclo, pedimos una entrada de teclado, ésta debería ser nuestro comando

printf("> ");
leer_de_teclado(256, command);

Una vez el usuario digita su comando, verificamos si es uno de los comandos especiales propios de nuestro intérprete, para ejecutar las funciones acordes

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
		    if(strcmp(tareas[w],comando[1])==0){
	          tareas[w] = '\0';
		        break;
		    }
		}
}

Usamos este ciclo para contar los elementos del comando ingresado, el cual ya se encuentra separado por palabras gracias a la función de_cadena_a_vector()

while (comando[i]){
    i++;
}

Después comparamos si la última palabra de nuestro comando es un "&", ya que en este caso, debemos remover esa palabra antes de ejecutar el comando, ya que de lo contrario, el intérprete pasaría el símbolo como parámetro. (Lo cual no se busca, ya que el "&" sólo está allí para indicar que ese proceso se requiere ejecutar en background)

if(strcmp(comando[i-1],"&")==0){
		background = 1;
		comando[i-1] = '\0';
}

Una vez hechas todas las rectificaciones y correcciones necesarias para el comando, podemos crear un proceso hijo que lo ejecute. También podemos hacer que el proceso hijo y el proceso padre ejecuten funciones distintas al utilizar un if que verifique el número de proceso, si el número es 0, el proceso es el hijo y entra en el primer condicional, de lo contrario, si el número es mayor que 0, entonces el proceso es el padre, y el número es el identificador de proceso del hijo

int rc = fork();
assert(rc >= 0);
if (rc == 0)
    execvp(comando[0], comando);
else{
... //código que ejecuta el padre
}

El padre puede verificar si su hijo requiere estar en background o no gracias a una bandera que cambia a 1 si el comando contenía un "&" en la última palabra, de ser así, el identificador de proceso del hijo entra al vector de tareas, y sumamos 1 al contador de tareas para saber que hay 1 tarea más en la lista; si el proceso hijo no necesita ejecutarse en background, entonces el padre espera a que el estado de su hijo se actualice antes de terminar el while indefinido y repetir todo el proceso de pedir un input al usuario

if(background == 1){
    tareas[colaTareas]=rc;
		colaTareas++;
}
else
  wait(NULL);