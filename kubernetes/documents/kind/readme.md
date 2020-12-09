# kind

[kind](https://kind.sigs.k8s.io/) 是一个使用名为`node`的container快速部署本地kubernetes集群的工具。

***kind一般用于学习环境***

## 安装

```bash
go get sigs.k8s.io/kind@v0.9.0
```

## 创建一个集群

```bash
kind create cluster --name forstudy
```


## 删除一个集群

```bash
kind delete cluster --name forstudy
```