# kubebuilder

## 安装方法

github下载release版本

## 使用方法

```bash
#注意，这个文件夹只能包含0-9a-z-这些字符
mkdir demo
cd demo
kubebuilder init --domain k8s.demo.io
```

创建一个kind

```bash
# 添加了group 就是 demo.k8s.demo.io/v1
kubebuilder create api --group demo --version v1 --kind Demo
```