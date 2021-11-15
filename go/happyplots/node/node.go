package node

import (
	"context"

	"study.bing89.com/happyplots/plot"
)

type Service struct {
	client plot.SchedulerClient
	*plot.Node
}

func NewNode(chedulerClient plot.SchedulerClient, n *plot.Node)*Service{
	node := Service{
		client: chedulerClient,
		Node: n,
	}
	return &node
}

func(n *Service)HeartBeat(ctx context.Context, req *plot.Request)(*plot.NodeStatus, error){
	return nil, nil
}

func (n *Service)AllocateJob(ctx context.Context, job *plot.Job)(*plot.Response, error){
	return nil, nil
}

func(n *Service)KillJob(ctx context.Context, job *plot.Job)(*plot.Response, error){
	return nil, nil
}

func (n *Service)UpdateJobStatus(ctx context.Context)error{
	return nil
}

func (n *Service)UpdateStatus(ctx context.Context)error{
	return nil
}

func (n *Service)Registe(ctx context.Context)error{
	return nil
}

func (n *Service)Run(ctx context.Context)error{
	return nil
}

