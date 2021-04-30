package main

import (
	"flag"
	"fmt"
	"log"
	"os"

	"github.com/Shopify/sarama"
)

func producer()error{
	config := sarama.NewConfig()
	config.Producer.RequiredAcks = sarama.WaitForAll
	config.Producer.Partitioner = sarama.NewCustomPartitioner()
	config.Producer.Return.Successes = true

	msg := sarama.ProducerMessage{}
	msg.Topic = "test"
	msg.Value = sarama.StringEncoder("this is a test message")

	client, err := sarama.NewSyncProducer([]string{"192.168.0.31:30092"}, config)
	if err != nil {
		return err
	}
	defer client.Close()

	pid, offset, err := client.SendMessage(&msg)
	if err != nil {
		return err
	}

	fmt.Printf("message sent, pid: %d, offset: %d", pid, offset)
	return nil
}

func consumer()error{
	consumer, err := sarama.NewConsumer([]string{"192.168.0.31:9092"}, nil)
	if err != nil {
		return err
	}
	defer consumer.Close()

	partitionList, err := consumer.Partitions("test")
	if err != nil {
		return err
	}
	fmt.Println(partitionList)

	for _, partition := range partitionList {
		pc, err := consumer.ConsumePartition("test", partition, sarama.OffsetNewest)
		if err != nil {
			return err
		}
		defer pc.AsyncClose()
		go func(){
			for msg := range pc.Messages() {
				fmt.Printf("partition: %d, offset: %d, key: %v, value: %v", msg.Partition, msg.Offset, msg.Key, msg.Value)
			}
		}()
	}
	return nil
}

func main() {
	var p, c bool
	flag.BoolVar(&p, "p", false, "run producer")
	flag.BoolVar(&c, "c", false, "run consumer")
	flag.Parse()
	runMode := os.Getenv("RUN_MODE")
	switch {
	case p||runMode == "Producer":
		err := producer()
		if err != nil {
			log.Fatal(err)
		}
		return
	case c||runMode == "Consumer":
		err := consumer()
		if err != nil {
			log.Fatal(err)
		}
		return
	}
}

