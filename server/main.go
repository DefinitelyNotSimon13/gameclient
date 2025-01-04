package main

import (
    "fmt"
    "log"
    "net"
    "os"
)

func main() {
    address := "localhost:9000"

    udpAddr, err := net.ResolveUDPAddr("udp", address)
    if err != nil {
        log.Fatal("Error resolving UDP address:", err)
        os.Exit(1)
    }

    udpConn, err := net.ListenUDP("udp", udpAddr)
    if err != nil {
        log.Fatal("Error starting UDP server:", err)
        os.Exit(1)
    }
    defer udpConn.Close()

    fmt.Println("UDP server is listening on", address)

    buf := make([]byte, 1024)
    for {
        n, remoteAddr, err := udpConn.ReadFromUDP(buf)
        if err != nil {
            log.Println("Error reading UDP data:", err)
            continue
        }
        fmt.Printf("Received UDP data from %v: %s\n", remoteAddr, string(buf[:n]))

        // response := "General Kenobi!"
        // _, err = udpConn.WriteToUDP([]byte(response), remoteAddr)
        // if err != nil {
        //     log.Println("Error sending UDP response:", err)
        // }
    }
}
