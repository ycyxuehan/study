# go 使用protobuff

## 安装依赖

```bash
mkdir /opt/protoc 
cd /opt/protoc  
wget https://github.com/protocolbuffers/protobuf/releases/download/v3.15.8/protoc-3.15.8-linux-x86_64.zip
unzip protoc-3.15.8-linux-x86_64.zip
echo 'export PATH=$PATH:/opt/protoc/bin' > /etc/profile.d/protoc.sh

go get github.com/golang/protobuf/protoc-gen-go

```

***注意，请将`$HOME/go/bin`加入到PATH***

## 示例

```protobuf
syntax = "proto3";

option go_package = "./;main";

package main;

message String {
    string value = 1;
}
```

***这里`option go_package = "./;main";`一定要写，指定文件位置与包名，用`;`隔开，文件位置必须以`/`结尾***

## 生产go代码

```bash
protoc --go_out=. hello.proto
```

使用grpc插件

```bash
protoc --go_out=plugins=grpc:. hello.proto
```