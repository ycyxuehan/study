package scheduler

import (
	"context"

	"study.bing89.com/happyplots/plot"
)

type Scheduler struct {
	nodeClient plot.NodeServiceClient
}

func NewScheduler(client plot.NodeServiceClient)*Scheduler{
	return &Scheduler{
		nodeClient: client,
	}
}


func (s *Scheduler)Registe(ctx context.Context, node *plot.Node)(*plot.Response, error){

	return nil, nil
}

func (s *Scheduler)UpdateJobStatus(ctx context.Context, job *plot.Job)(*plot.Response, error){
	return nil, nil
}

func (s *Scheduler)UpdateNodeStatus(ctx context.Context, node *plot.Node)(*plot.Response, error){
	return nil, nil
}

func (s *Scheduler)Nodes(ctx context.Context, req *plot.Request)(*plot.NodeList, error){
	return nil, nil
}

func (s *Scheduler)Node(ctx context.Context, req *plot.Request)(*plot.Node, error){
	return nil, nil
}

func (s *Scheduler)Jobs(ctx context.Context, filter *plot.JobFilter)(*plot.JobList, error){
	return nil, nil
}

func (s *Scheduler)AddJob(ctx context.Context, cap *plot.PlotCapacity)(*plot.Response, error){
	return nil, nil
}

func (s *Scheduler)HeartBeat(ctx context.Context, req *plot.Request)error{
	return nil
}

func (s *Scheduler)AllocateJob(ctx context.Context, job *plot.Job)error{
	return nil
}

