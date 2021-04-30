package auth

import (
	"context"

	"study.bing89.com/rpc/grpc/proto"
)

type GRPCServer struct {
	auth *Authentication
}

func (g *GRPCServer)SomeMethod(ctx context.Context, in *proto.String)(*proto.String, error){
	if err := g.auth.Auth(ctx); err != nil {
		return nil, err
	}
	return &proto.String{Value: "Hello " + in.GetValue()}, nil
}