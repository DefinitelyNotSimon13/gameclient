package client

import (
	"net"
)

type Client struct {
	id         uint32
	udpAddress *net.UDPAddr
	// Pointer or not?
	tcpConn *net.Conn
	// Status?
}

func NewClient(id uint32, tcpConn *net.Conn) *Client {
	return &Client {
		id: id,
		tcpConn: tcpConn,
	}
}
