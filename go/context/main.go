package main

import (
	"context"
	"fmt"
	"time"
)

func one(ctx context.Context){
	fmt.Printf("one started @%s\n", time.Now().Format("2006-01-02T15:04:05"))
	select {
	case <- ctx.Done():
		fmt.Printf("one exited @%s\n", time.Now().Format("2006-01-02T15:04:05"))
	}
}

func two(ctx context.Context){
	fmt.Printf("two started @%s\n", time.Now().Format("2006-01-02T15:04:05"))
	select {
	case <- ctx.Done():
		fmt.Printf("two exited @%s\n", time.Now().Format("2006-01-02T15:04:05"))
	default:
		time.Sleep(1* time.Second)
		fmt.Printf("two recived %v @%s\n", ctx.Value("hello"), time.Now().Format("2006-01-02T15:04:05"))
		fmt.Println("two 还想玩，他不想退出, 让我再玩10秒钟")
		time.Sleep(10* time.Second)
		return
	}
}

func three(ctx context.Context){
	fmt.Printf("three started @%s\n", time.Now().Format("2006-01-02T15:04:05"))
	go two(ctx)
	select {
	case <- ctx.Done():
		fmt.Printf("超时了，我和two都要退出了 @%s\n", time.Now().Format("2006-01-02T15:04:05"))
	}
}

func main(){
	ctx, done := context.WithTimeout(context.TODO(), 5 * time.Second)
	ctx1, _ := context.WithTimeout(context.TODO(), 5 * time.Second)
	ctx2  := context.WithValue(ctx1, "hello", "我还可以带值，你说有趣不有趣")
	defer done()
	go one(ctx)
	go three(ctx2)
	time.Sleep(10*time.Second)
}