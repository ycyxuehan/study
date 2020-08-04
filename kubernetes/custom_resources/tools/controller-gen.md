# controller-gen

根据API代码，自动生成对应的CRD配置yaml文件

## 安装方法

```
git clone https://github.com/kubernetes-sigs/controller-tools.git
cd controller-tools/cmd/controller-gen/
go build -o ~/go/bin/controller-gen main.go
```

## 用法

```bash
# controller-gen crd --paths=[api code path]/... output:dir=[output dir]
controller-gen crd --paths=/git/demo/api/... output:dir=/git/demo/crd
```