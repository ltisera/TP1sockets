#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

//Considero 
/*El sistema numérico en base 36 se llama sistema alfanumérico y utiliza para su representación los símbolos 
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z 
(las letras pueden ser mayúsculas o minúsculas).*/


char numerico[] = "0123456789";
char letras[] = "bcdfghjklmnpqrstvwxyzaeiouBCDFGHJKLMNPQRSTVWXYZAEIOU";
char mVocales[] = "AEIOU";
char mConsonantes[] = "BCDFGHJKLMNPQRSTVWXYZ";
char vocales[] = "aeiou";
char consonantes[] = "bcdfghjklmnpqrstvwxyz";


char *generarUsuario(int longitud);
char *generarPassword(int longitud);
int generarLetra();
int generarNumero();
int generarVocal();
int generarConsonante();
int generarNumeroAleatorio(int min, int max);
int main() {
    srand(time(NULL));
    WSADATA wsa;
    SOCKET serverSock, clientSock;
    struct sockaddr_in serverAddress, clientAddress;
    int clientAddressSize = sizeof(clientAddress);
    char buffer[1024];
    char response[1024];
    int recvSize;
    int salirLoopMensajes = 0;
    //Para manejo de longitudes
    long numLongitud = 0;
    char *finptr;

    // Inicializa Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Falla al inicializar Winsock. Err: %d\n", WSAGetLastError());
        return 1;
    }

    // Crea el socket del servidor
    if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear el socket del servidor. Err: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Configura la dirección del servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Escucha en todas las interfaces
    serverAddress.sin_port = htons(5000);

    // Asocia el socket con la dirección
    if (bind(serverSock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Falla al asociar el socket. Err: %d\n", WSAGetLastError());
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    // Escucha conexiones entrantes
    if (listen(serverSock, 3) == SOCKET_ERROR) {
        printf("Falla al escuchar en el puerto. Err: %d\n", WSAGetLastError());
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    printf("Esperando conexiones...\n");

    // Acepta una conexión entrante
    if ((clientSock = accept(serverSock, (struct sockaddr *)&clientAddress, &clientAddressSize)) == INVALID_SOCKET) {
        printf("Falla al aceptar la conexión. Err: %d\n", WSAGetLastError());
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    printf("Cliente conectado.\n");

    // Recibe un mensaje del cliente
    while (salirLoopMensajes == 0)
    {
        
    
        if ((recvSize = recv(clientSock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR) {
            printf("Falla al recibir datos. Err: %d\n", WSAGetLastError());
        } else {
            buffer[recvSize] = '\0'; // Asegúrate de que el buffer sea una cadena C
            printf("Mensaje recibido: %s\n", buffer);

            if(strcmp(buffer, "Generar Usuario") == 0){
                strcpy(response, "Indicar longitud de Usuario\n");
                if (send(clientSock, response, strlen(response), 0) == SOCKET_ERROR) {
                    printf("Falla al enviar respuesta. Err: %d\n", WSAGetLastError());
                } else {
                    printf("Respuesta enviada al cliente.\n");
                }
                if ((recvSize = recv(clientSock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR) {
                    printf("Falla al recibir datos. Err: %d\n", WSAGetLastError());
                } else {
                    buffer[recvSize] = '\0'; // Asegúrate de que el buffer sea una cadena C
                    printf("\nMensaje recibido: %s\n", buffer);
                    numLongitud = strtol(buffer, &finptr, 10);
                    if(*finptr == '\0'){
                        printf("Validando longitud: %d\n", numLongitud);
                        if(numLongitud >= 5 && numLongitud <=15){
                            printf("\nGENERA USUARIO\n");
                            char *usuario = generarUsuario(numLongitud);
                            strcpy(response, usuario);
                            free(usuario);
                            if (send(clientSock, response, strlen(response), 0) == SOCKET_ERROR) {
                                printf("Falla al enviar respuesta. Err: %d\n", WSAGetLastError());
                            } else {
                                printf("Respuesta enviada al cliente. %s\n", response);
                            }
                        } else {
                            printf("\nError Longitud no valida\n");
                            strcpy(response, "ERR1\n");
                            if (send(clientSock, response, strlen(response), 0) == SOCKET_ERROR) {
                                printf("Falla al enviar respuesta. Err: %d\n", WSAGetLastError());
                            } else {
                                printf("Respuesta enviada al cliente.\n");
                            }

                        }
                    } else {
                        printf("Tirame ERRCONV");
                        strcpy(response, "ERRCONV");
                        if (send(clientSock, response, strlen(response), 0) == SOCKET_ERROR) {
                            printf("Falla al enviar respuesta. Err: %d\n", WSAGetLastError());
                        } else {
                            printf("Respuesta enviada al cliente.\n");
                        }
                    }
                }

            } else if (strcmp(buffer, "Generar Pass") == 0){
                strcpy(response, "Indicar longitud de Password\n");
                if (send(clientSock, response, strlen(response), 0) == SOCKET_ERROR) {
                    printf("Falla al enviar respuesta. Err: %d\n", WSAGetLastError());
                } else {
                    printf("Respuesta enviada al cliente.\n");
                }
                if ((recvSize = recv(clientSock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR) {
                    printf("Falla al recibir datos. Err: %d\n", WSAGetLastError());
                } else {
                    
                    buffer[recvSize] = '\0'; // Asegúrate de que el buffer sea una cadena C
                    printf("\nMensaje recibido: %s\n", buffer);
                    numLongitud = strtol(buffer, &finptr, 10);
                    if(*finptr == '\0'){
                        printf("Validando longitud: %d\n", numLongitud);
                        if(numLongitud >= 8 && numLongitud <=50){
                            char *password = generarPassword(numLongitud);
                            strcpy(response, password);
                            free(password);
                            if (send(clientSock, response, strlen(response), 0) == SOCKET_ERROR) {
                                printf("Falla al enviar respuesta. Err: %d\n", WSAGetLastError());
                            } else {
                                printf("Respuesta enviada al cliente. %s\n", response);
                            }
                        } else {
                            printf("\nError Longitud no valida\n");
                            strcpy(response, "ERR2\n");
                            if (send(clientSock, response, strlen(response), 0) == SOCKET_ERROR) {
                                printf("Falla al enviar respuesta. Err: %d\n", WSAGetLastError());
                            } else {
                                printf("Respuesta enviada al cliente.\n");
                            }

                        }
                    } else {
                        printf("Tirame ERRCONV");
                        strcpy(response, "ERRCONV\n");
                        if (send(clientSock, response, strlen(response), 0) == SOCKET_ERROR) {
                            printf("Falla al enviar respuesta. Err: %d\n", WSAGetLastError());
                        } else {
                            printf("Respuesta enviada al cliente.\n");
                        }
                    }

                }


            } else if (strcmp(buffer, "Salir") == 0) {
                printf("Saliendo");
                salirLoopMensajes = 1;
            } else {
                printf("Notificar mensaje desconocido");
                strcpy(response, "ERRCOMANDO\n");
                if (send(clientSock, response, strlen(response), 0) == SOCKET_ERROR) {
                    printf("Falla al enviar respuesta. Err: %d\n", WSAGetLastError());
                } else {
                    printf("Respuesta enviada al cliente.\n");
                }
            }


        }
        
    }
    // Cierra los sockets y limpia Winsock
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();

    return 0;
}

char *generarPassword(int cant){
    char *sRetorno = NULL;
    char letra = '\0';
    sRetorno = (char *)malloc(cant * sizeof(char) + 1);
    if(sRetorno != NULL){
        for(int i = 0; i < cant; i++){
            if(rand()%2 == 0){
                letra = numerico[generarNumero()];
            } else {
                letra = letras[generarLetra()];
            }
            sRetorno[i] = letra;
        }
        sRetorno[cant] = '\n';
    }
    return sRetorno;
}

char *generarUsuario (int cant){
    char *sRetorno = NULL;
    int cualVa = 0;
    int cumple = 0;
    int ultima = 0;
    int esVocal= 0;
    char letra = '\0';
    
    sRetorno = (char *)malloc(cant * sizeof(char) + 1);
    if(sRetorno != NULL){
        for(int i = 0;  i < cant; i++){
            if(esVocal){
                letra = vocales[generarVocal()];
            } else {
                letra = consonantes[generarConsonante()];
            }
            esVocal = !esVocal;
            sRetorno[i] = letra;
        }
        sRetorno[cant] = '\n';
    }
    

    return sRetorno;
}

int generarNumero() {
    return rand() % (strlen(numerico));
}

int generarLetra() {
    return rand() % (strlen(letras));
}

int generarVocal() {
    return rand() % (strlen(vocales));
}

int generarConsonante() {
    return rand() % (strlen(consonantes));
}