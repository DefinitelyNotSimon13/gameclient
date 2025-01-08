package server

import (
	"encoding/binary"
	"math"
)

const (
	PacketVersion0 = 0
)

// parseSenderID extracts the sender ID from bytes 1..4 (little-endian).
func parseSenderID(data []byte) uint32 {
	return binary.LittleEndian.Uint32(data[1:5])
}

// parseCoordinates extracts x, y, z from the data (bytes 5..8, 9..12, 13..16).
func parseCoordinates(data []byte) (float32, float32, float32) {
	xBits := binary.LittleEndian.Uint32(data[5:9])
	yBits := binary.LittleEndian.Uint32(data[9:13])
	zBits := binary.LittleEndian.Uint32(data[13:17])
	return math.Float32frombits(xBits),
		math.Float32frombits(yBits),
		math.Float32frombits(zBits)
}
