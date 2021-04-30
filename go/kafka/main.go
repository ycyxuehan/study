package main

import (
	"context"
	"flag"
	"fmt"
	"log"
	"os"
	"sync"
	"time"

	"github.com/Shopify/sarama"
)

// Consumer represents a Sarama consumer group consumer
type Consumer struct {
	ready chan bool
}

// Setup is run at the beginning of a new session, before ConsumeClaim
func (consumer *Consumer) Setup(sarama.ConsumerGroupSession) error {
	// Mark the consumer as ready
	close(consumer.ready)
	return nil
}

// Cleanup is run at the end of a session, once all ConsumeClaim goroutines have exited
func (consumer *Consumer) Cleanup(sarama.ConsumerGroupSession) error {
	return nil
}

// ConsumeClaim must start a consumer loop of ConsumerGroupClaim's Messages().
func (consumer *Consumer) ConsumeClaim(session sarama.ConsumerGroupSession, claim sarama.ConsumerGroupClaim) error {

	// NOTE:
	// Do not move the code below to a goroutine.
	// The `ConsumeClaim` itself is called within a goroutine, see:
	// https://github.com/Shopify/sarama/blob/master/consumer_group.go#L27-L29
	for message := range claim.Messages() {
		log.Printf("Message claimed: value = %s, timestamp = %v, topic = %s", string(message.Value), message.Timestamp, message.Topic)
		session.MarkMessage(message, "")
	}

	return nil
}

func producer()error{
	config := sarama.NewConfig()
	config.Producer.RequiredAcks = sarama.WaitForAll
	config.Producer.Partitioner = sarama.NewCustomPartitioner()
	config.Producer.Return.Successes = true

	msg := sarama.ProducerMessage{}
	msg.Topic = "test"
	msg.Value = sarama.StringEncoder("this is a test message")

	client, err := sarama.NewSyncProducer([]string{"kafka-svc.study.svc:9092"}, config)
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

//fmt.Printf("partition: %d, offset: %d, key: %v, value: %v", msg.Partition, msg.Offset, msg.Key, msg.Value)

func consumer(ctx context.Context)error{
	client, err := sarama.NewConsumerGroup([]string{"kafka-svc.study.svc:9092"}, "group1",  &sarama.Config{})
	if err != nil {
		return err
	}
	defer client.Close()
	consumer := Consumer{
		ready: make(chan bool),
	}
	wg := &sync.WaitGroup{}
	wg.Add(1)
	go func(){
		defer wg.Done()
		for {
			if err := client.Consume(ctx, []string{"test"}, &consumer); err != nil {
				log.Fatal(err)
			}
			if ctx.Err() != nil {
				return
			}
			consumer.ready = make(chan bool)
		}
	}()
	<- consumer.ready
	wg.Wait()
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
		ctx, cancel := context.WithTimeout(context.Background(), 5 * time.Second)
		defer cancel()
		err := consumer(ctx)
		if err != nil {
			log.Fatal(err)
		}
		return
	}
}

