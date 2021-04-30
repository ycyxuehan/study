# 使用TSL加密gRPC

gRPC建立在HTTP/2协议之上，对TLS提供了很好的支持

## 创建证书

创建server证书

```bash
#域名为server.grpc.io，有效期10年(3650days)
openssl genrsa -out server.key 2048
openssl req -new -x509 -days 3650 -subj "/C=GB/L=China/O=grpc-server/CN=server.grpc.io" -key server.key -out server.crt
```
创建client证书
```bash
#域名为server.grpc.io，有效期10年(3650days)
openssl genrsa -out client.key 2048
openssl req -new -x509 -days 3650 -subj "/C=GB/L=China/O=grpc-client/CN=client.grpc.io" -key client.key -out client.crt
```
创建自签名证书

```bash
openssl genrsa -out ca.key 2048
openssl req -new -x509 -days 3650 -subj "/C=GB/L=China/O=gobook/CN=github.com" -key ca.key -out ca.crt
```

对服务端证书进行签名

```bash
openssl req -new -subj "/C=GB/L=China/O=server/CN=server.io" -key server.key -out server.csr
openssl x509 -req -sha256 -CA ca.crt -CAkey ca.key -CAcreateserial -days 3650 -in server.csr -out server.crt
```

对客户端证书进行签名

```bash
penssl req -new -subj "/C=GB/L=China/O=client/CN=client.io" -key client.key -out client.csr
$ openssl x509 -req -sha256 -CA ca.crt -CAkey ca.key -CAcreateserial -days 3650 -in client.csr -out client.crt
```

## 运行代码

服务端
```bash
go run main.go -s
```
接收端
```bash
GODEBUG=x509ignoreCN=0 go run main.go -r
```
发送端
```bash
GODEBUG=x509ignoreCN=0 go run main.go -c
```
***go.15 以上已经启用了 CommonName 了，上述的方式生成的证书需要添加GODEBUG=x509ignoreCN=0 这样的环境变量才能正常运行,否则会报错`rpc error: code = Unavailable desc = connection error: desc = "transport: authentication handshake failed: x509: certificate relies on legacy Common Name field, use SANs or temporarily enable Common Name matching with GODEBUG=x509ignoreCN=0"`***

