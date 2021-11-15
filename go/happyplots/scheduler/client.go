package scheduler

import "study.bing89.com/happyplots/plot"

type Client struct {
	nodeClient plot.NodeServiceClient
}

func NewClient(client plot.NodeServiceClient)*Client{
	return &Client{
		nodeClient: client,
	}
}