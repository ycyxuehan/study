package main

import (
	"context"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"time"

	"google.golang.org/grpc"
	"study.bing89.com/rpc/grpc-stream/proto"
)
const (
	ServiceName = "study.bing89.com/go/rpc/hellojson"
	NetWork = "tcp"
	Address = "localhost:1234"
	)

type HelloServiceImpl struct{}

func (p *HelloServiceImpl)Hello(ctx context.Context, args *proto.String)(*proto.String, error){
	reply := &proto.String{Value: "hello:" + args.GetValue()}
	return reply, nil
}

func (p *HelloServiceImpl)Channel(stream proto.HelloService_ChannelServer)error{
	for {
		args, err := stream.Recv()
		if err != nil {
			if err == io.EOF{
				return nil
			}
			return err
		}
		reply := &proto.String{Value: "hello:" + args.GetValue()}
		err = stream.Send(reply)
		if err != nil {
			return err
		}
	}
}

func runServer(network string, address string)error{
	grpcSvc := grpc.NewServer()
	proto.RegisterHelloServiceServer(grpcSvc, new(HelloServiceImpl))
	lis, err := net.Listen("tcp", ":1234")
	if err != nil {
		return err
	}
	return grpcSvc.Serve(lis)
}

func runClient(network string, address string)error{
	conn, err := grpc.Dial(fmt.Sprintf("%s", address), grpc.WithInsecure())
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()
	client := proto.NewHelloServiceClient(conn)
	stream, err := client.Channel(context.Background())
	if err != nil {return err}
	go func(){
		for {
			if err := stream.Send(&proto.String{Value: "hi"}); err != nil {
				log.Fatal(err)
			}
			time.Sleep(time.Second)
		}
	}()
	for {
		reply, err := stream.Recv()
		if err != nil {
			if err == io.EOF{
				break
			}
			return err
		}
		fmt.Println(reply.GetValue())
	}
	return nil
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