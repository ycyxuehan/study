package main

import (
	"context"
	"flag"
	"fmt"
	"log"
	"net"

	"google.golang.org/grpc"
	"study.bing89.com/rpc/chat/chat"
)


func runServer()error{
	svc := grpc.NewServer()
	ctx := context.Background()
	cctx, cancel := context.WithCancel(ctx)
	defer cancel()
	server := chat.NewServer(cctx)
	chat.RegisterChatServiceServer(svc, server)
	lis, err := net.Listen("tcp", ":1234")
	if err != nil {
		return err
	}
	svc.Serve(lis)
	return nil
}

func runClient(name string, to string)error{
	conn, err := grpc.Dial(":1234", grpc.WithInsecure())
	if err != nil {
		return err
	}
	defer conn.Close()
	client := chat.NewChatServiceClient(conn)
	ctx := context.Background()
	vctx := context.WithValue(ctx, "from", name)
	stream, err := client.Channel(vctx)
	if err != nil {
		return err
	}
	for {
		str := ""
		fmt.Scanln(&str)
		if str == "quit" || str == "q" {
			return nil
		}
		err := stream.Send(&chat.Message{From: name, To: to, Content: str})
		if err != nil {
			fmt.Println(err)
		}
	}
}

func main(){
	var s, c  bool
	var t, f string
	flag.BoolVar(&s, "s", false, "run server")
	flag.BoolVar(&c, "c", false, "run client")
	flag.StringVar(&t, "t", "", "message send to")
	flag.StringVar(&f, "f", "", "message from")
	flag.Parse()
	switch {
	case s:
		err := runServer()
		if err != nil {
			log.Fatal(err)
		}
	case c:
		if t == "" || f == "" {
			log.Fatal("name error")
		}
		err := runClient(f, t)
		if err != nil {
			log.Fatal(err)
		}
	}
}