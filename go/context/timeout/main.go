package main

import (
	"context"
	"log"
	"os"
	"sync"
	"time"
)

var funcList []func()
var mux = sync.RWMutex{}
var count int = 3

func todo(){
	mux.Lock()
	log.Println("i am working now")
	count = count - 1
	mux.Unlock()
	log.Println("work success")
}

func shutdown(ctx context.Context)error{
	for _, f := range funcList{
		go f()
	}
	// time.Sleep(5 * time.Second)
	ticker := time.NewTicker(1 * time.Second)
	for {
		select{
		case <- ticker.C:
			if count == 0 {
				log.Println("i have done all works, exit")
			}
			return nil
		case <- ctx.Done():
			log.Println("time out, but my work do not complete, exit")
			return ctx.Err()
		}
	}
}

func main(){
	funcList = append(funcList, todo, todo, todo)
	ctx, cancel := context.WithTimeout(context.Background(), 3 * time.Second)
	defer cancel()
	err := shutdown(ctx)
	if err != nil {
		log.Println(err)
		os.Exit(1)
	}
}