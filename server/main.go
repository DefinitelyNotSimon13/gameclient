package main

import (
	"fmt"
	"log"
	"net"
	"os"
)

func main() {
	// Set the address to listen on
	address := "localhost:9000"
	listener, err := net.Listen("tcp", address)
	if err != nil {
		log.Fatal("Error starting server:", err)
		os.Exit(1)
	}
	defer listener.Close()

	fmt.Println("Server is listening on", address)

	for {
		// Accept incoming connections
		conn, err := listener.Accept()
		if err != nil {
			log.Println("Error accepting connection:", err)
			continue
		}

		// Handle the connection in a new goroutine
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	// Ensure the connection is closed when we're done
	defer conn.Close()

	// Send a greeting message
	message := "Hello you!"
	_, err := conn.Write([]byte(message))
	if err != nil {
		log.Println("Error sending message:", err)
		return
	}

	fmt.Println("Sent greeting to", conn.RemoteAddr())
}
