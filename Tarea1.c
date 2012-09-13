#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*funcion de recepcion y envio de errores
  recive un puntero a char y envia el 
  contenido de este a pantalla.*/
void error(const char *msg)
{
	perror(msg);
	exit(0);
}

/*Programa pricipal*/
int main(int argc, char *argv[])
{
	/*Revisa que entren minimo 3 argumentos
	  1- el puerto del servidor
	  2- el puerto del cliente
	  3- la direccion ip del cliente*/
	if (argc < 4)
	{
		fprintf(stderr,"El puerto host %s está en uso\n", argv[0]);
		exit(0);
	}
	else
	{
		/*Inicia el proceso de fork con el cliente del socket*/ 
		int  proceso = fork();
		if(proceso == 0)
		{
			int sockec_cliente, portno, n;
			struct sockaddr_in serv_addr;
			struct hostent *server;
			char buffer[256], linea;
			char salida[] = "Adios";
			
			portno = atoi(argv[2]);
			sockec_cliente = socket(AF_INET, SOCK_STREAM, 0);
			
			if (sockec_cliente < 0)/*Apertura del socket de cliente*/
			{error("ERROR de apertura de socket");}
			else
			{
				server = gethostbyname(argv[2]);

				if (server == NULL)/*Busqueda del servidor*/
				{
					fprintf(stderr,"ERROR, no hay host \n");
					exit(0);
				}
				else
				{
					/*Asignaciones de conexion*/
					bzero((char *) &serv_addr, sizeof(serv_addr));
					serv_addr.sin_family = AF_INET;
					inet_pton(AF_INET, argv[3],&(serv_addr.sin_addr.s_addr));
					serv_addr.sin_port = htons(portno);
					int intentos = 0;
					int conexion = -1;
					
					/*Intentos de conexion
					 el cliente intena conectarse al servidor
					 el intento se realiza durante un minuto*/	
					printf("Intentando generar conexion\n");
					printf("Por favor espere\n");
						
					while (conexion < 0)
					{
						conexion = connect(sockec_cliente,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
						
						intentos++;
						if (intentos  >  60)
						{
							error("Limite de tiempo para conectarse exedido");
							close(sockec_cliente);
							exit(0);
							return 0;
						}
						else
						{
							sleep(1);
						}	
					}
					
					printf("Conexion establecida.\n");
					printf("Bienvenido\n");
					int estado = 0;
					
					/*Revisa el estado de la escritura
					  para saver si esta habilitada o si
					  se deben cerrar los sockets*/		
					while(estado == 0)
					{	
						printf("Por favor, introduzca el mensaje:\n");
						bzero(buffer,256);
						fgets(buffer,255,stdin);
						linea = *buffer;
							
						n = write(sockec_cliente,buffer,strlen(buffer));
							
						if(linea == *salida)/*Revisa si la palabra "Adios para cerrar los sockets"*/
						{
							close(sockec_cliente);/*Cierre de ls sockets*/
							estado = 1;
							return 0;
						}
						else/*En caso de no ser la palabra envie el mensaje normalmente*/
						{
							if (n < 0)
							{
								error("ERROR al escribir en socket");
							}
									
							else
							{
								bzero(buffer,256);
								n = read(sockec_cliente,buffer,255);

								if (n < 0)
								{
									error("ERROR al leer del socket");
								}
							
								else
								{
									printf("Mensaje recivido: %s\n",buffer);
								}
							}
						}
					}/*Sale del estado de bucle para cerrar el cliente*/
					n = write(sockec_cliente,buffer,strlen(buffer));

					if (n < 0)
					{
						error("ERROR al escribir en socket");
					}
					else
					{
						close(sockec_cliente);/*Cierre de ls sockets*/
						return 0;
					}
				}
			}
		}
		
		/*Sistema de servidor del chat*/
		else
		{
			int socked_server, newsocked_server, portno;
			socklen_t clilen;
			char buffer[256], linea;
			struct sockaddr_in serv_addr, cli_addr;
			int n;
			char salida[] = "Adios";

			socked_server = socket(AF_INET, SOCK_STREAM, 0);//Genera el socket del servidor

			if (socked_server < 0)
			{
				error("ERROR de apertura de socket");
			}
			else
			{
				/*Argumentos de conexión y asignación de puertos 
				  de entrada de información y envio de respuestas*/
				bzero((char *) &serv_addr, sizeof(serv_addr));
				portno = atoi(argv[1]);
				serv_addr.sin_family = AF_INET;
				serv_addr.sin_addr.s_addr = INADDR_ANY;
				serv_addr.sin_port = htons(portno);
				
				/*Establecimiento del la conexión del servidor*/			
				bind(socked_server, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr));
					
				/*Recepción de:
				  Numero total de clientes permitidos
				  Generar conexión con el cliente
				  Aceptación de cliente y generacion del socket*/	
				listen(socked_server,5);
				clilen = sizeof(cli_addr);
				newsocked_server = accept(socked_server,(struct sockaddr *) &cli_addr,&clilen);

				if (newsocked_server < 0)
				{
					error("ERROR en aceptar cliente");
				}
				else
				{
					int estado = 0;
					/*Revision del estado de conversación*/
					while(estado == 0)
					{
						bzero(buffer,256);
						n = read(newsocked_server,buffer,255);
						linea = *buffer;							
						if(linea == *salida)//Revisa si se cumple la palabra de salida
						{
							estado = 1;
						}
						else/*En caso de no cumplirse recive la información
							  y envia una respuesta de forma normal*/
						{
							if (n < 0)
							{
								error("ERROR al leer del socket");
							}
							else
							{
								printf("Aquí está el mensaje: %s\n",buffer);
								n = write(newsocked_server,"Tengo el mensaje",18);
								if (n < 0)
								{
									error("ERROR al escribir en el socket");
								}
								else
								{
									bzero(buffer,256);
								}
							}
						}
					}/*Recepcion del mensaje == "Adios"
					   lee el mensaje del cliente
					   envia un recivido y cierra los sockets
					   del servidor*/
					if (n < 0)
					{	
						error("ERROR al leer del socket");
					}
					else
					{
						printf("Aquí está el mensaje: %s\n",buffer);
						n = write(newsocked_server,"Tengo el mensaje",18);

						if (n < 0)
						{
							error("ERROR al escribir en el socket");
						}
						else
						{
							close(newsocked_server);
							close(socked_server);
							return 0;
						}
					}
				}
			}
		}
	}
}
