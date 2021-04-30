package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"strings"
	"time"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/bson/primitive"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

type Order struct {
	gorm.Model
	OrderNo string `json:"OrderNo" bson:"order_no" gorm:"column:order_no;type:char(32)" `
	SubcardNo string `json:"SubcardNo" gorm:"column:subcard_no;type:char(64)"`
}

func main(){
	clt, err := mongo.NewClient(options.Client().ApplyURI("mongodb://root:qxdWvopFQcnbHf8C@10.0.1.44:27017/admin"))
	if err != nil {
		log.Fatal(err)
	}
	err = clt.Connect(context.Background())
	if err != nil {
		log.Fatal(err)
	}
	defer clt.Disconnect(context.Background())
	opensku := clt.Database("opensku")
	openskuOrder := opensku.Collection("opensku_order")
	t, err := time.Parse("2006-01-02T15:04:05.999", "2021-04-21T11:30:58.883")
	if err != nil {
		log.Fatal(err)
	}
	t1 := primitive.NewDateTimeFromTime(t.Add(-8 * time.Hour))
	t, err = time.Parse("2006-01-02T15:04:05.999", "2021-04-21T11:43:58.883")
	if err != nil {
		log.Fatal(err)
	}
	t2 := primitive.NewDateTimeFromTime(t.Add(-8 * time.Hour))
	
	f2 := bson.M{
		"order_source" : 1,
		"order_history": bson.M{
			"$elemMatch": bson.M{
				"order_status": "27", 
				"change_at": bson.M{ "$gte": t1,"$lte": t2},
			},
		},
	}
	data, _ := json.Marshal(f2)
	fmt.Println(string(data))

	opt := options.FindOptions{}
	opt.SetSort(bson.D{{"order_no",1}})

	cursor, err := openskuOrder.Find(context.Background(), f2, &opt)
	if err != nil {
		log.Fatal("find ", err)
	}
	orders := []Order{}
	err = cursor.All(context.Background(), &orders)
	if err != nil {
		log.Fatal("all", err)
	}
	orderNos := ""
	for _, order := range orders {
		orderNos = orderNos + "'"+order.OrderNo + "',"
	}
	orderNos = strings.Trim(orderNos, ",")
	dsn := "root:abc123456@tcp(10.0.1.44:3306)/intbee?charset=utf8mb4&parseTime=True&loc=Local"
	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
		log.Fatal(err)
	}
	sql := fmt.Sprintf("SELECT order_no, subcard_no from intbee4_activity_plan_open_group where order_no in (%s) order by order_no", orderNos)
	// fmt.Println(sql)
	rows, err := db.Raw(sql).Rows()
	defer rows.Close()
	orders = []Order{}
	for rows.Next() {
		order := Order{}
		db.ScanRows(rows, &order)
		// fmt.Println(order)
		orders = append(orders, order)
	}
	for _, order := range orders {
		fmt.Printf("%s,%s\n", order.OrderNo, order.SubcardNo)
	}
}