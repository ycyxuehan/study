package types

type Reward struct{
	BlockReward string `json:"block_reward"`
	Coin string `json:"coin"`
	Height string `json:"height"`
	HugeReward string `json:"huge_reward"`
	MortgateRateK int `json:"mortgage_rate_k"`
	Name string `json:"name"`
	RecordTime int64 `json:"record_time"`
	Status int `json:"status"`
	StatusStr string `json:"status_str"`
	Type string `json:"type"`
}