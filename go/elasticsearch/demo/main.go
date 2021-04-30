package main

import (
	"bytes"
	"context"
	"encoding/json"
	"io/ioutil"
	"log"
	"strconv"
	"strings"
	"sync"

	"github.com/elastic/go-elasticsearch"
	"github.com/elastic/go-elasticsearch/esapi"
)

//获取stats
type Stats map[string]interface{}

func getStats(es *elasticsearch.Client)(Stats, error){
	res, err := es.Indices.Stats()
	if err != nil {
		return nil, err
	}
	defer res.Body.Close()
	stats := make(Stats)
	data, _ := ioutil.ReadAll(res.Body)
	err = json.Unmarshal(data, &stats)
	return stats, err
}

//获取info
type Info map[string]interface{}

func getInfo(es *elasticsearch.Client)(Info, error){
	res, err := es.Info()
	if err != nil {
		log.Fatal(err)
	}
	defer res.Body.Close()
	info := make(Info)
	data, _ := ioutil.ReadAll(res.Body)
	err = json.Unmarshal(data, &info)
	return info, err
}

//其他操作，其实就是http请求
func main(){

	es, err := elasticsearch.NewClient(elasticsearch.Config{
		Addresses: []string{
			"http://192.168.0.31:30920",
		},
	})
	if err != nil {
		log.Fatal(err)
	}

	// 创建索引 && 文件
    var wg sync.WaitGroup
    // 循环list
    for i, title := range []string{"Test One", "Test Two"} {
        wg.Add(1)
 
        go func(i int, title string) {
            defer wg.Done()
 
            // 建立请求主体.
            // Builder创建json串
            var b strings.Builder
            b.WriteString(`{"title" : "`)
            b.WriteString(title)
            b.WriteString(`"}`)
 
            // 设置请求对象.
            req := esapi.IndexRequest{
                Index:      "test",
                DocumentID: strconv.Itoa(i + 1),
                Body:       strings.NewReader(b.String()),
                Refresh:    "true",
            }
 
            // 与客户端执行请求.
 
            res, err := req.Do(context.Background(), es)
            if err != nil {
                log.Fatalf("Error getting response: %s", err)
            }
            defer res.Body.Close()
 
            if res.IsError() {
                log.Printf("[%s] Error indexing document ID=%d", res.Status(), i+1)
            } else {
                // Deserialize the response into a map.
                var r map[string]interface{}
                if err := json.NewDecoder(res.Body).Decode(&r); err != nil {
                    log.Printf("Error parsing the response body: %s", err)
                } else {
                    // 打印响应状态和索引文档版本.
                    log.Printf("[%s] %s; version=%d", res.Status(), r["result"], int(r["_version"].(float64)))
                }
            }
        }(i, title)
    }
    wg.Wait()
    log.Println(strings.Repeat("-", 37))
 
    // 搜索索引文件
 
    // 建立请求主体.
    var buf bytes.Buffer
    query := map[string]interface{}{
        "query": map[string]interface{}{
            "match": map[string]interface{}{
                "title": "test",
            },
        },
    }
    if err := json.NewEncoder(&buf).Encode(query); err != nil {
        log.Fatalf("Error encoding query: %s", err)
    }
 
    // 执行搜索请求.
    res, err := es.Search(
        es.Search.WithContext(context.Background()),
        es.Search.WithIndex("test"),
        es.Search.WithBody(&buf),
        es.Search.WithTrackTotalHits(true),
        es.Search.WithPretty(),
    )
    if err != nil {
        log.Fatalf("Error getting response: %s", err)
    }
    defer res.Body.Close()
 
    if res.IsError() {
        var e map[string]interface{}
        if err := json.NewDecoder(res.Body).Decode(&e); err != nil {
            log.Fatalf("Error parsing the response body: %s", err)
        } else {
            // Print the response status and error information.
            log.Fatalf("[%s] %s: %s",
                res.Status(),
                e["error"].(map[string]interface{})["type"],
                e["error"].(map[string]interface{})["reason"],
            )
        }
    }
	r := make(map[string]interface{})
    if err := json.NewDecoder(res.Body).Decode(&r); err != nil {
        log.Fatalf("Error parsing the response body: %s", err)
    }
    // 打印响应状态，结果数和请求持续时间.
    log.Printf(
        "[%s] %d hits; took: %dms",
        res.Status(),
        int(r["hits"].(map[string]interface{})["total"].(map[string]interface{})["value"].(float64)),
        int(r["took"].(float64)),
    )
    // 打印每次匹配的ID和文档来源.
    for _, hit := range r["hits"].(map[string]interface{})["hits"].([]interface{}) {
        log.Printf(" * ID=%s, %s", hit.(map[string]interface{})["_id"], hit.(map[string]interface{})["_source"])
    }
    log.Println(strings.Repeat("=", 37))
 
    // 更新
    //es.Update()
     
    // 删除
    //es.Delete()

}