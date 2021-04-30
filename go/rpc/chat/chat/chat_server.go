package chat

import (
	"context"
	"fmt"
	"log"
)


type Server struct {
	users map[string]*User
	chanMap map[string]ChatService_ChannelServer
	messageChan chan Message
}

func NewServer(ctx context.Context)*Server{
	s := &Server{
		users: make(map[string]*User),
		chanMap: make(map[string]ChatService_ChannelServer),
		messageChan: make(chan Message),
	}
	go s.run(ctx)
	return s
}

func (s *Server)clearChannel(name string){
	delete(s.chanMap, name)
}

func (s *Server)addChannel(from string, stream ChatService_ChannelServer){
	// from, ok := stream.Context().Value("from").(string)
	// fmt.Println(stream.Context().Value("from"), from, ok)
	// if !ok || from == "" {
	// 	return
	// }
	s.chanMap[from] = stream
}

func (s *Server)getChannel(name string)(ChatService_ChannelServer, error){
	if ch, ok := s.chanMap[name]; ok {
		return ch, nil
	}
	return nil, fmt.Errorf("not found")
}

func (s *Server)send(msg *Message)error{
	return nil
}

func (s *Server)run(ctx context.Context){
	for {
		select {
		case msg := <- s.messageChan :
			fmt.Println("got message", msg.GetFrom(), msg.GetTo(), msg.GetContent())
			toChannel, err := s.getChannel(msg.GetTo())
			if err == nil && toChannel != nil{
				err = toChannel.Send(&msg)
				if err != nil {
					log.Println(err)
				}
			}else {
				log.Println(err)
			}
		case <- ctx.Done():
			return
		}
	}
}

func (s *Server)Channel(stream ChatService_ChannelServer)(error){
	// from, ok := stream.Context().Value("from").(string)
	msg, err := stream.Recv()
	s.addChannel(msg.From, stream)
	s.messageChan <- *msg
	return err
}

func (s *Server)Register(ctx context.Context, user *User)(*Result, error){
	if _, ok := s.users[user.GetName()]; ok {
		return &Result{Res: false}, fmt.Errorf("user exists")
	}
	s.users[user.GetName()] = user
	return &Result{Res: true}, nil
}