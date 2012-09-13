<<<<<<< HEAD
/*
Parte 1 de la tarea programada
generar un proceso P1 que logre
enviar un string a un proceso P2
y que P2 Responda*/

#include <stdio.h>

//Proceso P2
void P2(char linea[])
{
	printf("%s\n",linea);
	printf("Mensage recibido.\n");
}

//Proceso P1
void P1()
{
	char linea[50];
	printf("Por favor ingrese el comentario:\n");
	gets(linea);
	P2(linea);
}


//Main del sistema
void main()
{
	P1();
}
=======
/*
Parte 1 de la tarea programada
generar un proceso P1 que logre
enviar un string a un proceso P2
y que P2 Responda*/

#include <stdio.h>

//Proceso P2
void P2(char linea[])
{
	printf("%s\n",linea);
	printf("Mensage recibido.\n");
}

//Proceso P1
void P1()
{
	char linea[50];
	printf("Por favor ingrese el comentario:\n");
	gets(linea);
	P2(linea);
}


//Main del sistema
void main()
{
	P1();
}
>>>>>>> Documentción de la tarea programada y manual.
