package main

import (
	"log"

	"study.bing89.com/rpc/protobuf/proto"
)

type HelloService struct{}

func (p *HelloService) Hello(req *proto.String, rep *proto.String)error {
	rep.Value = "hello: " + req.GetValue()
	return nil
}

func main() {
	log.Println("test")
}
