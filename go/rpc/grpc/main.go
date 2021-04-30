package main

import (
	"context"
	"log"
	"net"

	"google.golang.org/grpc"
	"study.bing89.com/rpc/grpc/proto"
)

type HelloServiceImpl struct{}

func (p *HelloServiceImpl)Hello(ctx context.Context, args *proto.String)(*proto.String, error){
	reply := &proto.String{Value: "hello:" + args.GetValue()}
	return reply, nil
}

func main(){
	grpcSvc := grpc.NewServer()
	proto.RegisterHelloServiceServer(grpcSvc, new(HelloServiceImpl))
	lis, err := net.Listen("tcp", ":1234")
	if err != nil {
		log.Fatal(err)
	}
	grpcSvc.Serve(lis)
}