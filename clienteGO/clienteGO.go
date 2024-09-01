package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
)

func main() {
	// Declaración e inicialización de variables usando var
	var serverIp string = "127.0.0.1"
	var serverPort string = "5000"
	var conn net.Conn
	var err error
	var scanner *bufio.Scanner = bufio.NewScanner(os.Stdin)
	var salir bool = false
	var opcion string
	var message string
	var response string
	var longitudStr string
	var finalResponse string

	// Conectarse al servidor
	conn, err = net.Dial("tcp", serverIp+":"+serverPort)
	if err != nil {
		fmt.Printf("Falla al conectar al servidor: %s\n", err)
		return
	}
	defer conn.Close()

	fmt.Println("Conectado al servidor.")

	for !salir {
		fmt.Println("1) Generar Usuario")
		fmt.Println("2) Generar Passwd")
		fmt.Println("3) Salir")
		fmt.Print("Selecciona una opcion: ")

		scanner.Scan()
		opcion = scanner.Text()

		switch opcion {
		case "1":
			message = "Generar Usuario"
		case "2":
			message = "Generar Pass"
		case "3":
			message = "Salir"
			salir = true
			fmt.Println("Preparando para salir...")
		default:
			fmt.Println("Opción inválida.")
			continue
		}

		// Enviar el mensaje al servidor
		_, err = conn.Write([]byte(message))
		if err != nil {
			fmt.Printf("Falla al enviar el mensaje: %s\n", err)
			continue
		}

		// Recibir la respuesta del servidor
		response, err = bufio.NewReader(conn).ReadString('\n')
		if err != nil {
			fmt.Printf("Falla al recibir la respuesta del servidor: %s\n", err)
			continue
		}
		response = strings.TrimSpace(response)
		fmt.Printf("Respuesta del servidor: %s\n", response)

		// Si el servidor espera un número, pedir la longitud
		if response != "ERR" && (opcion == "1" || opcion == "2") {
			fmt.Print("Introduce la longitud: ")
			scanner.Scan()
			longitudStr = scanner.Text()
			_, err = strconv.Atoi(longitudStr)
			if err != nil {
				fmt.Println("Entrada inválida para la longitud.")
				continue
			}

			// Enviar la longitud al servidor
			_, err = conn.Write([]byte(longitudStr))
			if err != nil {
				fmt.Printf("Falla al enviar la longitud: %s\n", err)
				continue
			}

			// Recibir la respuesta final del servidor
			finalResponse, err = bufio.NewReader(conn).ReadString('\n')
			if err != nil {
				fmt.Printf("Falla al recibir la respuesta del servidor: %s\n", err)
				continue
			}
			finalResponse = strings.TrimSpace(finalResponse)
			fmt.Printf("Respuesta final del servidor: %s\n", finalResponse)
		}

		fmt.Println()
		if !salir {
			fmt.Println("Presiona Enter para continuar...")
			scanner.Scan()
		}
	}

	fmt.Println("Cerrando la conexión...")
}
