package main

import (
	"flag"
	"fmt"
	"log"
	"net"
	"net/rpc"
)

type HelloService struct {}

func (hs *HelloService)Hello(request string, reply *string)error{
	*reply = "hello: " + request
	return nil
}

func runServer(){
	rpc.RegisterName("HelloService", new(HelloService))

	listener, err := net.Listen("tcp", ":1234")
	if err != nil {
		log.Fatal("Listen TCP error:", err)
	}
	conn, err := listener.Accept()
	if err != nil {
		log.Fatal("Accept error:", err)
	}
	rpc.ServeConn(conn)
}

func runClient(){
	client, err := rpc.Dial("tcp", "localhost:1234")
	if err != nil {
		log.Fatal("dialing:", err)
	}
	var reply string
	err = client.Call("HelloService.Hello", "hello", &reply)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(reply)
}

const (
	ServerMode = "server"
	ClientMode = "client"
)

func main(){
	var runMode string
	flag.StringVar(&runMode, "mode", "", "run mode: server or client")
	flag.Parse()
	switch runMode{
	case ServerMode:
		runServer()
		return
	case ClientMode:
		runClient()
		return
	default:
		fmt.Println("must specified running mode: server or client")
	}
}