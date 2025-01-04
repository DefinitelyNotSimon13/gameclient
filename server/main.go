package main

// Things that will need to be saved during runtime:
// - connected players
// - active sessions
// - sessions -> players
// Possible future persitant:
// - "stats"

import (
	"fmt"
	"log"
	"net"
	"os"
)

func main() {
	addr := "localhost:9000"

	// --- TCP ---
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatal("Error starting TCP server:", err)
		os.Exit(1)
	}
	defer listener.Close()
	fmt.Println("TCP server is listening on", addr)

	go func() {
		for {
			tcpConn, err := listener.Accept()
			fmt.Println("Received TCP connection request...")
			if err != nil {
				log.Println("Error accepting TCP connection:", err)
				continue
			}
			go handleTCPConnection(tcpConn)
		}
	}()

	// --- UDP ---
	udpAddr, err := net.ResolveUDPAddr("udp", addr)
	if err != nil {
		log.Fatal("Error resolving UDP addr:", err)
		os.Exit(1)
	}

	udpConn, err := net.ListenUDP("udp", udpAddr)
	if err != nil {
		log.Fatal("Error starting UDP server:", err)
		os.Exit(1)
	}
	defer udpConn.Close()

	fmt.Println("UDP server is listening on", addr)

	handleUDPConnection(udpConn)
}

func handleTCPConnection(conn net.Conn) {
	defer conn.Close()

	message := "Hello from TCP server!"
	_, err := conn.Write([]byte(message))
	if err != nil {
		log.Println("Error sending TCP message:", err)
		return
	}

	fmt.Println("Sent greeting to", conn.RemoteAddr())
}

func handleUDPConnection(conn *net.UDPConn) {
	buf := make([]byte, 1024)

	for {
		n, remoteAddr, err := conn.ReadFromUDP(buf)
		if err != nil {
			log.Println("Error reading UDP data:", err)
			continue
		}
		fmt.Printf("Received UDP data from %v: %s\n", remoteAddr, string(buf[:n]))

		response := "General Kenobi!"
		_, err = conn.WriteToUDP([]byte(response), remoteAddr)
		if err != nil {
			log.Println("Error sending UDP response:", err)
		}
	}
}
