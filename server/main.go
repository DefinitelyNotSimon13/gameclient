package main

// Things that will need to be saved during runtime:
// - connected players
// - active sessions
// - sessions -> players
// Possible future persitant:
// - "stats"

import (
	"encoding/binary"
	"fmt"
	"log"
	"math"
	"net"
	"os"
	"sync"
)

const (
	PacketVersion0 = 0
)

var (
	udpClients = make(map[uint32]*net.UDPAddr)
	mu         sync.Mutex
)

type Connection struct {
	Id      int
	UdpAddr net.UDPAddr
}

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

	id := uint32(0)
	go func() {
		for {
			tcpConn, err := listener.Accept()
			fmt.Println("Received TCP connection request...")
			if err != nil {
				log.Println("Error accepting TCP connection:", err)
				continue
			}
			id++
			go handleTCPConnection(tcpConn, id)
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

func handleTCPConnection(conn net.Conn, id uint32) {
	defer conn.Close()

	bs := make([]byte, 4)
	binary.LittleEndian.PutUint32(bs, id)

	fmt.Println("Writing %s", bs)
	_, err := conn.Write(bs)
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

		if n < 1 {
			log.Println("Received less then 1 byte, can't read version")
		}

		// first 4 bytes from
		version := buf[0] & 0x0F
		switch version {
		case PacketVersion0:
			handlePacketV0(conn, buf, n, remoteAddr)
		default:
			fmt.Printf("Unknown version (%d) from %v\n", version, remoteAddr)
		}
	}
}

func handlePacketV0(conn *net.UDPConn, data []byte, length int, remoteAddr *net.UDPAddr) {
	if length < 17 {
		log.Fatal("Received to few bytes for PacketVersion 0")
		return
	}

	senderId := binary.LittleEndian.Uint32(data[1 : 1+4])

	xBits := binary.LittleEndian.Uint32(data[5 : 5+4])
	yBits := binary.LittleEndian.Uint32(data[9 : 9+4])
	zBits := binary.LittleEndian.Uint32(data[13 : 13+4])

	x := math.Float32frombits(xBits)
	y := math.Float32frombits(yBits)
	z := math.Float32frombits(zBits)

	fmt.Printf("V0 packet from %v with ID %d => x=%f, y=%f, z=%f\n", remoteAddr, senderId, x, y, z)

	mu.Lock()
	if _, ok := udpClients[senderId]; !ok {
		udpClients[senderId] = remoteAddr
		fmt.Printf("Registered new UDP client with ID %d at %v\n", senderId, remoteAddr)
	}
	mu.Unlock()

	mu.Lock()
	defer mu.Unlock()

	for clientId, clientAddr := range udpClients {
		if clientId == senderId {
			continue
		}

		_, err := conn.WriteToUDP(data[:length], clientAddr)
		if err != nil {
			log.Printf("Error broadcasting to client %d at %v: %v\n", clientId, clientAddr, err)
		}
	}
}
