# 使用kubeadm创建集群

安装[kubeadm](installation.md)

## 初始化控制节点

控制节点运行control plane组件，包括 `etcd` 和 `apiserver`。

1. （推荐）如果计划将单个控制平面 kubeadm 集群升级成高可用， 你应该指定 --control-plane-endpoint 为所有控制平面节点设置共享端点。 端点可以是负载均衡器的 DNS 名称或 IP 地址。

2. 选择一个Pod网络插件，并验证是否需要为 kubeadm init 传递参数。 根据你选择的第三方网络插件，你可能需要设置 --pod-network-cidr 的值。 请参阅 [安装Pod网络附加组件](##安装Pod网络附加组件)。

3. （可选）从版本1.14开始，`kubeadm` 可以自动检测系统上的运行时，如需要自定义，使用`--cri-socket` 参数指定。请参阅[安装运行时](installation.md)

4. kubeadm 使用与默认网关关联的网络接口来设置api server的监听地址。可以使用参数 `--apiserver-advertise-address=<ip-address>` 来指定。

5. 初始化之前，可以使用 `kubeadm config images pull`下载需要的镜像，使用 `kubeadm config images list` 查看需要下载哪些镜像

**kubeadm 不支持将没有 --control-plane-endpoint 参数的单个控制平面集群转换为高可用性集群。**

初始化控制节点

```bash
kubeadm init --kubernetes-version=v1.15.0 --pod-network-cidr=10.244.0.0/16 --apiserver-advertise-address=192.168.100.2
```

**请记录 kubeadm init 输出的 kubeadm join 命令。 你需要此命令将节点加入集群**

若要使非`root`用户可以运行kubectl，需要运行以下命令，它们可以在`kubeadm init`的输出中找到

```bash
mkdir -p $HOME/.kube
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
sudo chown $(id -u):$(id -g) $HOME/.kube/config
```

如果是root用户，则可以运行

```bash
export KUBECONFIG=/etc/kubernetes/admin.conf
```

## 安装Pod网络附加组件

必须部署一个基于 Pod 网络插件的 容器网络接口 (CNI)，以便你的 Pod 可以相互通信。 在安装网络之前，集群 DNS (CoreDNS) 将不会启动。

- 注意 pod网络不可与任何节点网络重叠。

- kubeadm默认设置集群强制使用RBAC。网络插件必须支持RBAC

- 若要使用ipv6，网络插件需要支持ipv6

**目前 Calico 是 kubeadm 项目中执行 e2e 测试的唯一 CNI 插件**

### 安装calico

```
kubectl apply -f https://docs.projectcalico.org/manifests/calico.yaml
```

## 解除控制节点隔离

集群默认不在控制节点上调度pod，使用如下命令解除：

``` bash
kubectl taint nodes --all node-role.kubernetes.io/master-
```

## 加入节点

在工作节点上执行以下命令

```bash
kubeadm join --token <token> <control-plane-host>:<control-plane-port> --discovery-token-ca-cert-hash sha256:<hash>
```

如果没有token，可以使用以下命令获取

```bash
kubeadm token list
```

token默认有效期为24小时，如果在token过期后想加入节点到集群，执行以下命令创建一个token

```bash
kubeadm token create
```

如果没有--discovery-token-ca-cert-hash 的值，可以使用如下命令来获取

```bash
openssl x509 -pubkey -in /etc/kubernetes/pki/ca.crt | openssl rsa -pubin -outform der 2>/dev/null | \
   openssl dgst -sha256 -hex | sed 's/^.* //'
```

## 将API服务器代理到本地主机

如果要从集群外部连接到 API 服务器，则可以使用 kubectl proxy：

```bash
scp root@<control-plane-host>:/etc/kubernetes/admin.conf .
kubectl --kubeconfig ./admin.conf proxy
```

## 清理

### 删除节点

重置节点的kubeadm安装状态

```bash
kubeadm reset
```

删除节点

```bash
kubectl drain <node name> --delete-local-data --force --ignore-daemonsets
```

清理iptables

```bash
iptables -F && iptables -t nat -F && iptables -t mangle -F && iptables -X
```

清理ipvs

```bash
ipvsadm -C
```

删除节点

```bash
kubectl delete node <node name>
```