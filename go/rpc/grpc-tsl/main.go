package main

import (
	"context"
	"crypto/tls"
	"crypto/x509"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials"
	"study.bing89.com/rpc/pubsub/demo/proto"
)

func getCredentials(crt , key, caFile, server string)(credentials.TransportCredentials, error){
	cert, err := tls.LoadX509KeyPair(crt, key)
	if err != nil {
		return nil, err
	}
	certPool := x509.NewCertPool()
	ca, err := ioutil.ReadFile(caFile)
	if err != nil {
		return nil, err
	}
	if ok := certPool.AppendCertsFromPEM(ca); !ok {
		return nil, fmt.Errorf("failed to append certs")
	}
	conf := tls.Config{
		Certificates: []tls.Certificate{cert},
		ClientAuth: tls.RequireAndVerifyClientCert,
		ClientCAs: certPool,
	}
	if server != "" {
		conf = tls.Config{
			Certificates: []tls.Certificate{cert},
			ServerName: server,
			RootCAs: certPool,
		}
	}
	creds := credentials.NewTLS(&conf)
	return creds, nil
}

func runServer() {
	creds, err := getCredentials("keys/server.crt", "keys/server.key", "keys/ca.crt", "")
	if err != nil {
		log.Fatal(err)
	}
	p := proto.NewPubsubService()
	grpcSvc := grpc.NewServer(grpc.Creds(creds))
	proto.RegisterPubsubServiceServer(grpcSvc, p)
	lis, err := net.Listen("tcp", ":1234")
	if err != nil {
		log.Fatal(err)
	}
	err = grpcSvc.Serve(lis)
	if err != nil {
		log.Fatal(err)
	}
}

func runSender(){
	creds, err := getCredentials("keys/client.crt", "keys/client.key", "keys/ca.crt", "server.io")
	if err != nil {
		log.Fatal(err)
	}
	conn, err := grpc.Dial("localhost:1234", grpc.WithTransportCredentials(creds))
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()
	client := proto.NewPubsubServiceClient(conn)
	_, err = client.Publish(context.Background(), &proto.String{Value: "golang: hello Go"})
	if err != nil {
		log.Fatal(err)
	}
	_, err = client.Publish(context.Background(), &proto.String{Value: "docker: hello Docker"})
	if err != nil {
		log.Fatal(err)
	}
}

func runReciever(){
	creds, err := getCredentials("keys/client.crt", "keys/client.key", "keys/ca.crt", "server.io")
	if err != nil {
		log.Fatal(err)
	}
	conn, err := grpc.Dial("localhost:1234", grpc.WithTransportCredentials(creds))
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()
	client := proto.NewPubsubServiceClient(conn)
	stream, err := client.Subscribe(context.Background(), &proto.String{Value:"golang:"})
	if err != nil {
		log.Fatal(err)
	}
	for {
		reply, err := stream.Recv()
		if err != nil {
			if err == io.EOF{
				break
			}
			log.Fatal(err)
		}
		fmt.Println(reply.GetValue())
	}

}

func main(){
	var c, s, r bool
	flag.BoolVar(&c, "c", false, "run as sender")
	flag.BoolVar(&s, "s", false, "run as server")
	flag.BoolVar(&r, "r", false, "run as reciever")
	flag.Parse()
	switch{
	case s:
		runServer()
		return
	case c:
		runSender()
		return
	case r:
		runReciever()
		return
	default:
		fmt.Printf("must specified running mode: server or client: %v, %v\n", c, s)
	}

}