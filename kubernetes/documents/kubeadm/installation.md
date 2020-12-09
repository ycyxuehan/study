# 安装kubeadm

## 安装之前

准备环境：

- 一台或多台Linux服务器
- 服务器内存不低于2GB， CPU核心数不低于2
- 节点之中不可以有重复的主机名、MAC 地址或 product_uuid
- 禁用swap分区
- 如果开启了防火墙，需要开放必要端口
- iptables 工具不使用 nftables 后端

## 确保iptables 工具不使用 nftables 后端

在 Linux 中，nftables 当前可以作为内核 iptables 子系统的替代品。 iptables 工具可以充当兼容性层，其行为类似于 iptables 但实际上是在配置 nftables。 nftables 后端与当前的 kubeadm 软件包不兼容：它会导致重复防火墙规则并破坏 kube-proxy。

**RHEL8 (CentOS8)不支持切换到旧版本模式，因此与当前的 kubeadm 软件包不兼容。**

在 Debian 10 (Buster)、Ubuntu 19.04、Fedora 29 及更新版本中需要切换iptables 工具使用旧版模式。

### debian/ubuntu

```bash
update-alternatives --set iptables /usr/sbin/iptables-legacy
update-alternatives --set ip6tables /usr/sbin/ip6tables-legacy
update-alternatives --set arptables /usr/sbin/arptables-legacy
update-alternatives --set ebtables /usr/sbin/ebtables-legacy
```

### fedora

```bash
update-alternatives --set iptables /usr/sbin/iptables-legacy
```

## 开放防火墙端口

### 控制节点

协议|方向|端口范围|作用|使用者
--|--|--|--|--
TCP|入站|6443*|Kubernetes API 服务器|所有组件
TCP|入站|2379-2380|etcd server client API|kube-apiserver, etcd
TCP|入站|10250|Kubelet API|kubelet 自身、Control plane
TCP|入站|10251|kube-scheduler|kube-scheduler 自身
TCP|入站|10252|kube-controller-manager|kube-controller-manager 自身

### 工作节点

协议|方向|端口范围|作用|使用者
--|--|--|--|--
TCP|入站|10250|Kubelet API|kubelet 自身、控制平面组件
TCP|入站|30000-32767|NodePort 服务**|所有组件

## 安装runtime

kubeadm支持以下runtime

运行时|域套接字
--|--
Docker|/var/run/docker.sock
containerd|/run/containerd/containerd.sock
CRI-O|/var/run/crio/crio.sock

kubeadm(v1.14.0或更新版本) 使用上表中的域套接字路径自动检测系统中的运行时，如果同时检测到 docker 和 containerd，则优先选择 docker。 在非linux节点，默认使用docker。

## 安装 `kubelet`、`kubectl`和`kubeadm` 

这里使用阿里云源

### ubuntu/debian/hypriotos

```bash
apt-get update && apt-get install -y apt-transport-https
curl https://mirrors.aliyun.com/kubernetes/apt/doc/apt-key.gpg | apt-key add - 
cat <<EOF >/etc/apt/sources.list.d/kubernetes.list
deb https://mirrors.aliyun.com/kubernetes/apt/ kubernetes-xenial main
EOF  
apt-get update
apt-get install -y kubelet kubeadm kubectl
```

### RHEL/centos/fedora

```bash
cat <<EOF > /etc/yum.repos.d/kubernetes.repo
[kubernetes]
name=Kubernetes
baseurl=https://mirrors.aliyun.com/kubernetes/yum/repos/kubernetes-el7-x86_64/
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://mirrors.aliyun.com/kubernetes/yum/doc/yum-key.gpg https://mirrors.aliyun.com/kubernetes/yum/doc/rpm-package-key.gpg
EOF
setenforce 0
yum install -y kubelet kubeadm kubectl
systemctl enable kubelet && systemctl start kubelet
```

***如果出现gpg检查失败的情况,请用 yum install -y --nogpgcheck kubelet kubeadm kubectl 安装***

## 配置cgroup驱动程序

使用 docker 时，kubeadm 会自动为其检测 cgroup 驱动并在运行时对 /var/lib/kubelet/kubeadm-flags.env 文件进行配置

## 开启kubelet服务自启动

```bash
systemctl enable --now kubelet
```

**kubelet 现在每隔几秒就会重启，因为它陷入了一个等待 kubeadm 指令的死循环。**

## 故障排查

参见[故障排查](https://kubernetes.io/zh/docs/setup/production-environment/tools/kubeadm/troubleshooting-kubeadm/)