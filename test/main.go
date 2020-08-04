package main

import (
	"flag"
	"fmt"
	_ "github.com/jinzhu/gorm/dialects/mysql"
	"github.com/jinzhu/gorm"
)

const testDB="intbee:intbee@mysql@tcp(192.168.1.53:3306)/intbee?charset=utf8mb4&parseTime=true"

func connectDB(uri string)(*gorm.DB, error){
	db, err := gorm.Open("mysql", uri)
	if err != nil {
		return nil, err
	}
	db.DB().SetMaxIdleConns(1)
	db.DB().SetMaxOpenConns(2)
	return db, nil
}

type certCompany struct {
	m gorm.Model `gorm:"-"`
	ID int
	UUID string
	UserID string
	Mobile string
	CompanyName string
	BusiNo string
	BankName string
	BankNo string
	CompanyNo string
	BusinessLicense string
	IsVerify int
	Status int
	CreateTime int64
	UpdateTime int64
	VerifyMessage string
	QualificationStatus int
}
func (c *certCompany)Display(){
	fmt.Printf("用户：%s\n验证结果：%s\n认证状态：%s\n", c.Mobile, getVerify(c.IsVerify), getQualuficationStatus(c.QualificationStatus))
}
func getData(user string, db *gorm.DB)*certCompany{
	cc := certCompany{}
	db.Table("intbee_cert_company").Where("mobile=?", user).First(&cc)
	return &cc
}

func getVerify(v int)string{
	switch v {
	case 0:
		return "未认证"
	case 1:
		return "已通过"
	case 2:
		return "未通过"
	default:
		return "未知"
	}
}

func getQualuficationStatus(s int)string{
	//0:未认证 1：审核中 2：审核通过 3:审核不通过（驳回）
	switch s {
	case 0:
		return "未认证"
	case 1:
		return "1：审核中"
	case 2:
		return "2：审核通过"
	case 3:
		return "审核不通过（驳回）"
	default:
		return "未知"
	}
}


func main(){
	user := flag.String("u", "", "用户电话号码")
	uri := flag.String("d", testDB, "数据库URI")
	db, err := connectDB(*uri)
	if err != nil {
		panic(fmt.Errorf("连接数据库失败：%v", err))
	}
	defer db.Close()
	data:= getData(*user, db)
	if data != nil {
		data.Display()
	}
}