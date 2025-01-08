package server

import (
	"log"
	"net"
)

// handlePacketV0 is a method for handling version-0 UDP packets.
func (s *Server) handlePacketV0(data []byte, length int, remoteAddr *net.UDPAddr) {
	// Safe to assume we've already checked length >= 1
	if length < 17 {
		log.Println("Received too few bytes for PacketVersion0 (need >= 17)")
		return
	}

	senderID := parseSenderID(data)
	x, y, z := parseCoordinates(data)

	log.Printf(
		"V0 packet from %v with ID %d => x=%.2f, y=%.2f, z=%.2f\n",
		remoteAddr, senderID, x, y, z,
	)

	// Register the new client if it doesn't exist
	s.mu.Lock()
	if _, ok := s.udpClients[senderID]; !ok {
		s.udpClients[senderID] = remoteAddr
		log.Printf("Registered new UDP client with ID %d at %v\n", senderID, remoteAddr)
	}
	// Broadcast to all other clients
	for clientID, clientAddr := range s.udpClients {
		if clientID == senderID {
			continue
		}
		if _, err := s.udpConn.WriteToUDP(data[:length], clientAddr); err != nil {
			log.Printf("Error broadcasting to client %d at %v: %v\n", clientID, clientAddr, err)
		}
	}
	s.mu.Unlock()
}
