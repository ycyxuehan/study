package main

import (
	"flag"
	"fmt"
	"log"
	"net"
	"net/rpc"
	"net/rpc/jsonrpc"
)

const (
ServiceName = "study.bing89.com/go/rpc/hellojson"
NetWork = "tcp"
Address = "localhost:1234"
)

type Hello interface {
	Hello(string, *string)error
}

type HelloJSON struct {}

func (hs *HelloJSON)Hello(req string, rep *string)error{
	*rep = "hello: " + req
	return nil
}

func RegisterHelloService(svc Hello)error{
	return rpc.RegisterName(ServiceName, svc)
}

type HelloClient struct{
	*rpc.Client
}

func DialHelloService(network, address string)(*HelloClient, error){
	c, err := net.Dial(network, address)
	if err != nil {
		return nil, err
	}
	cjson := rpc.NewClientWithCodec(jsonrpc.NewClientCodec(c))
	return &HelloClient{Client: cjson}, nil
}

func (hc *HelloClient)Hello(req string, rep *string)error{
	return hc.Call(ServiceName + ".Hello", req, rep)
}



func runClient(network, address string)error{
	client, err := DialHelloService(network, address)
	if err != nil {return err}
	var rep string
	err =  client.Hello("client", &rep)
	if err != nil {return err}
	fmt.Println(rep)
	return nil
}

func runServer(network, address string)error{
	err := RegisterHelloService(new(HelloJSON))
	if err != nil {
		return err
	}
	listener, err := net.Listen(network, address)
	if err != nil {return err}
	for {
		conn, err := listener.Accept()
		if err != nil {return err}
		go rpc.ServeCodec(jsonrpc.NewServerCodec(conn))
	}
}

func main(){
	var c, s bool
	flag.BoolVar(&c, "c", false, "run as client")
	flag.BoolVar(&s, "s", false, "run as server")
	flag.Parse()
	switch{
	case s:
		err := runServer(NetWork, Address)
		if err != nil {
			log.Fatal(err)
		}
		return
	case c:
		err := runClient(NetWork, Address)
		if err != nil {
			log.Fatal(err)
		}
		return
	default:
		fmt.Printf("must specified running mode: server or client: %v, %v\n", c, s)
	}
}