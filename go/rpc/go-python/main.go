package main

import (
	"context"
	"fmt"
	"log"
	"net"
	"net/http"
	"time"

	"github.com/gin-gonic/gin"
	"google.golang.org/grpc"
	"study.bing89.com/rpc/go-python/gorpc"
)

type GatewayRPCServer struct {
	users map[string]*gorpc.User
}

func NewGatewayRPCServer()*GatewayRPCServer{
	return &GatewayRPCServer{
		users: make(map[string]*gorpc.User),
	}
}

func (s *GatewayRPCServer)Register(ctx context.Context, user *gorpc.User)(*gorpc.Result, error){
	//用户已存在
	if _, ok := s.users[user.Name]; ok {
		return &gorpc.Result{Res: false,Error: "user exists", CreatedAt: uint64(time.Now().Unix())}, fmt.Errorf("user exists")
	}
	s.users[user.Name] = user
	log.Println("user", user.Name, "registed")
	return &gorpc.Result{Res: true,Error: "", CreatedAt: uint64(time.Now().Unix())}, nil
}

func (s *GatewayRPCServer)User(name string)(*gorpc.User, error){
	if user, ok := s.users[name]; ok {
		return user, nil
	}
	return nil, fmt.Errorf("user not found")
}

func (s *GatewayRPCServer)Run(){
	grpcSvc := grpc.NewServer()
	gorpc.RegisterGatewayServiceServer(grpcSvc, s)
	lis, err := net.Listen("tcp", ":8081")
	if err != nil {
		log.Fatal(err)
	}
	grpcSvc.Serve(lis)
}

type GatewayAPIServer struct {
	rpcServer *GatewayRPCServer
}

func NewGatewayAPIServer(rpcServer *GatewayRPCServer)(*GatewayAPIServer, error){
	if rpcServer == nil {
		return nil, fmt.Errorf("rpc server is nil")
	}
	g:= &GatewayAPIServer{rpcServer: rpcServer}
	return g, nil
}

func (g *GatewayAPIServer)Hello(c *gin.Context){
	message := gorpc.Message{}
	err := c.Bind(&message)
	if err != nil {
		log.Println("read post data failed", err)
		c.String(http.StatusOK, "read post data failed %v", err)
		return
	}
	user, err := g.rpcServer.User("hello")
	if err != nil {
		log.Println(err)
		c.String(http.StatusOK, "%v", err)
		return
	}
	conn, err := grpc.Dial(user.Ip, grpc.WithInsecure())
	if err != nil {
		log.Println(err)
		c.String(http.StatusOK, "create connection to hello server failed: %v", err)
		return
	}
	defer conn.Close()
	client := gorpc.NewHelloServiceClient(conn)
	res, err := client.Hello(context.Background(), &message)
	if err != nil {
		log.Println(err)
		c.String(http.StatusOK, "do rpc failed: %v", err)
		return
	}
	c.JSON(http.StatusOK, &res)
}

func (g *GatewayAPIServer)initRoute(engine *gin.Engine){
	apiV1 := engine.Group("/api/v1")
	apiV1.POST("/hello", g.Hello)
}

func (g *GatewayAPIServer)Run()error{
	engine := gin.Default()
	g.initRoute(engine)
	go g.rpcServer.Run()
	return engine.Run("localhost:8080")
}

func main(){
	rpcServer := NewGatewayRPCServer()
	apiServer, _ := NewGatewayAPIServer(rpcServer)
	err := apiServer.Run()
	if err != nil {
		log.Fatal(err)
	}
}