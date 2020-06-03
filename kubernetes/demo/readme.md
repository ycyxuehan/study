# kubernetes demo
用于kubernetes学习与测试

## 编译

```bash
CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o main main.go
```

## 打包

```bash
docker build -t demo:1.0 .
```

