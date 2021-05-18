package types

import (
	"encoding/json"
	"fmt"
)

const HPoolProfitDataURL = "https://www.hpool.com/api/pool/list?language=zh&type=opened"

type HPoolProfitData struct {
	APIKey        string  `json:"api_key"`
	Coin          string  `json:"coin"`
	Name          string  `json:"name"`
	Offline       int     `json:"offline"`
	Online        int     `json:"online"`
	WalletAddress string  `json:"pool_address"`
	Distributed   string `json:"pool_income"`
	Undistributed string `json:"undistributed_income"`
}

func (h *HPoolProfitData)Show(format DisplayFormat){
	switch format {
	case FormatJSON:
		data, err := json.MarshalIndent(h, "", "\t")
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(string(data))
	case FormatText:
		fmt.Printf("矿池名称 :%s\n挖矿密钥 :%s\n产出货币 :%s\n在线节点 :%d\n离线节点 :%d\n钱包地址 :%s\n结算收益 :%s\n尚未结算 :%s\n", 
			h.Name,
			h.APIKey,
			h.Coin,
			h.Online,
			h.Offline,
			h.WalletAddress,
			h.Distributed,
			h.Undistributed,
		)
	case FormatTable:
		fmt.Printf("")
	}
}
