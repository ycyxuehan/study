package main

import (
	"context"
	"fmt"
	"log"
	"time"
)


func todo(ch chan<- struct{}, ctx context.Context){
	log.Println("start working")
	i := 1
	for {
		select{
			case <- ctx.Done():
				log.Println("boss have no time to read my report")
				return
			default:
				time.Sleep(1*time.Second)
				log.Printf("i have finished step %d\n", i)
				i = i + 1
				if i == 4 {
					log.Println("boss say i am good stuff")
					ch <- struct{}{}
					return
				}
		}
	}
}

func main(){
	ch := make(chan struct{}, 1)
	fmt.Println("make boss busy")
	ch <- struct{}{}
	ctx, cancel := context.WithTimeout(context.Background(), 2 * time.Second)
	defer cancel()
	go todo(ch, ctx)
	time.Sleep(1 * time.Second)
	log.Println("boss is free now")
	<- ch
	time.Sleep(5 * time.Second)
}