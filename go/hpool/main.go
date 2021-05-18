package main

import (
	"encoding/json"
	"flag"
	"io/ioutil"
	"log"
	"math/rand"
	"net/http"
	"os"
	"path"
	"strings"
	"time"

	"study.bing89.com/hpool/types"
)

/*
{
                "api_key": "43b38cf9-7db8-4d5f-b3e9-b069115a2556",
                "block_reward": "2.00000000",
                "block_time": 18,
                "capacity": 37472,
                "coin": "chia",
                "deposit_mortgage_balance": "0.00000000",
                "deposit_mortgage_effective_balance": "0.00000000",
                "deposit_mortgage_free_balance": "0.00000000",
                "deposit_rate": "0",
                "fee": 0,
                "loan_mortgage_balance": "0.00000000",
                "mortgage": "0.00000000",
                "name": "CHIA",
                "offline": 0,
                "online": 2,
                "payment_time": "12:00:00 (UTC+8)",
                "point_deposit_balance": "0.00000000",
                "pool_address": "xch1f0ryxk6qn096hefcwrdwpuph2hm24w69jnzezhkfswk0z2jar7aq5zzpfj",
                "pool_income": "0.36922131",
                "pool_type": "ECO",
                "previous_income_pb": "0.00223639",
                "theory_mortgage_balance": "0.00000000",
                "type": "chia",
                "undistributed_income": "0.04920139"
            }
*/


var UserAgentMap [] string

func init(){
	UserAgentMap = []string{
		"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.93 Safari/537.36 Edg/90.0.818.56",
	}
}


type ResponseData struct {
	Total int `json:"total"`
	List []types.HPoolProfitData `json:"list"`
}

type Response struct {
	Code int `json:"code"`
	Data ResponseData `json:"data"`
}

func GetCookie(name string) (string, error) {
	data, err := ioutil.ReadFile(name)
	return strings.Trim(string(data), "\n"), err
}

func randUserAgent()string{
	rand.Seed(time.Now().Unix())
	n := rand.Intn(len(UserAgentMap))
	return UserAgentMap[n]
}

func GetPoolProfitData(url string, cookie string)(*Response, error){
	request, err := http.NewRequest(http.MethodGet, url, nil)
	if err != nil {
		return nil, err
	}
	request.Header.Add("User-Agent", randUserAgent())
	request.Header.Add("cookie", cookie)
	request.Header.Add("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(request)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()
	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}
	hppd := Response{}
	err = json.Unmarshal(data, &hppd)
	if err != nil {
		return nil, err
	}
	return &hppd, nil
}

func main(){
	d, err := os.UserHomeDir()
	if err == nil {
		d = path.Join(d, ".hpool", "cookie")
	}
	var cookieFile string
	flag.StringVar(&cookieFile, "cookie", d, "cookie file path")
	flag.Parse()
	cookie, err := GetCookie(cookieFile)
	if err != nil {
		log.Fatal(err)
	}
	resp, err := GetPoolProfitData(types.HPoolProfitDataURL, cookie)
	if err != nil {
		log.Fatal(err)
	}
	for _, hppd := range resp.Data.List {
		hppd.Show(types.FormatText)
	}
}