package main

import (
	"context"
	"fmt"
	"sync"
	"time"
)

var wg = sync.WaitGroup{}

func todo(ctx context.Context){
	fmt.Println("i am a working goroutine, i will complete work after 10 seconds")
	i := 1
	for {
		select {
		case <- ctx.Done():
			fmt.Println("i am killed by main thread.")
			wg.Done()
			return
		default:
			wg.Add(1)
			go func() {
				i = i + 1
				time.Sleep(10 * time.Second)
				fmt.Println("i have completed my work.")
				wg.Wait()
			}()
			time.Sleep(1 * time.Second)
		}
	}
}

func main(){
	ctx, cancel := context.WithTimeout(context.Background(), 3 * time.Second)
	defer cancel()
	go todo(ctx)
	fmt.Println("i have only 3 seconds to do work")
	time.Sleep(4 * time.Second)
	fmt.Println("time is up, i will exit.")
}