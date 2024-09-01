#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void limpiarBuffer();

int main() {
    printf("Iniciamos");
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in serverAddress;
    char *serverIp = "127.0.0.1";
    int serverPort = 5000;
    char message[] = "";
    int result;
    int salir = 0;
    int opcion = 0;
    int einva = 0;
    int longitud;
    int sfDevolvio = 0;
    char serverResponse[1024];
    int responseSize;

    
    // Inicializa Winsock
    printf("SALIENDO...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Falla al inicializar Winsock. Err: %d\n", WSAGetLastError());
        return 1;
    }

    // Crea un socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear el socket. Err: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Configura la dirección del servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = inet_addr(serverIp);

    // Conecta al servidor
    if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Falla al conectar. Err: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    
    
    while(salir==0){
        system("cls");
        printf("1)Generar Usuario\n2)Generar Passwd\n3)Salir\nSelecciona una opcion: ");

        sfDevolvio = scanf("%d", &opcion);
        if(sfDevolvio != 0){
            printf("\nDevolvio %d\n", sfDevolvio);
            limpiarBuffer();
            switch (opcion)
            {
            case 1:
                strcpy(message, "Generar Usuario");
                break;
            case 2:
                strcpy(message, "Generar Pass");
                break;
            case 3:
                salir = 1;
                strcpy(message, "Salir");
                printf("\nPreparando Para Salir\n");
                break;
            default:
                einva = 1;
                break;
            }
            
            if(salir == 0 && einva == 0){
                einva = 0;
                
                printf("\nEnviando al server: %s\n", message);
                // Envía un mensaje al servidor
                if (send(sock, message, strlen(message), 0) == SOCKET_ERROR) {
                    printf("Fallo el envío. Err: %d\n", WSAGetLastError());
                } else {
                    printf("Mensaje enviado al servidor.\n");
                }
                
                //recibo mensaje
                memset(serverResponse, 0, sizeof(serverResponse));
                responseSize = recv(sock, serverResponse, sizeof(serverResponse), 0);
                if (responseSize > 0 && strcmp(serverResponse, "ERR") != 0) {
                    serverResponse[responseSize] = '\0';
                    printf("Respuesta del servidor: %s\n", serverResponse);
                    sfDevolvio = scanf("%d", &longitud);
                    if (sfDevolvio != 0){
                        printf("Longitud que le mando al srvr: %d\n", longitud);
                        sprintf(message, "%d", longitud);
                        printf(message);
                        if (send(sock, message, strlen(message), 0) == SOCKET_ERROR) {
                            printf("Fallo el envío. Err: %d\n", WSAGetLastError());
                        } else {
                            printf("Mensaje enviado al servidor.\n");
                        }
                    } else {
                        printf("Hay un error en la lectura de longtud");
                    }
                    memset(serverResponse, 0, sizeof(serverResponse));
                    responseSize = recv(sock, serverResponse, sizeof(serverResponse), 0);
                    if (responseSize > 0 && strcmp(serverResponse, "ERR1") != 0 && strcmp(serverResponse, "ERR2") != 0) {
                        printf("\n%s, fue generado por el server\n", serverResponse);
                        
                    } else {
                        printf("ERROR: %s\n", serverResponse);
                    }
                    
                    system("pause");
                } else {
                    printf("Falla al recibir la respuesta del servidor. Err: %d\n", WSAGetLastError());
                }


                    
                    
                
            }
        } else {
           
            limpiarBuffer();
        }
    }
    
    printf("Enviando salir");
    strcpy(message, "Salir");
    if (send(sock, message, strlen(message), 0) == SOCKET_ERROR) {
        printf("Fallo el envío. Err: %d\n", WSAGetLastError());
    } else {
        printf("Mensaje enviado al servidor.\n");
    }
    
    // Cierra el socket y limpia Winsock
    closesocket(sock);
    WSACleanup();

    return 0;
}

void limpiarBuffer(){
    fflush(stdin);
}