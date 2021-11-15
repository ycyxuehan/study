package node

import (
	"context"

	"study.bing89.com/happyplots/plot"
)

type Client struct {

}

func NewClient()*Client{
	return &Client{}
}

func(c *Client)HeartBeat(ctx context.Context, req *plot.Request)(*plot.NodeStatus, error){
	return nil, nil
}

func (c *Client)AllocateJob(ctx context.Context, job *plot.Job)(*plot.Response, error){
	return nil, nil
}

func(c *Client)KillJob(ctx context.Context, job *plot.Job)(*plot.Response, error){
	return nil, nil
}