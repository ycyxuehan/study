package main

import (
	"context"
	"flag"
	"fmt"
	"io"
	"log"
	"net"

	"google.golang.org/grpc"
	"study.bing89.com/rpc/pubsub/demo/proto"
)
func runServer() {
	p := proto.NewPubsubService()
	grpcSvc := grpc.NewServer()
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
	conn, err := grpc.Dial("localhost:1234", grpc.WithInsecure())
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
	conn, err := grpc.Dial("localhost:1234", grpc.WithInsecure())
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