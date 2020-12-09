# 高可用Kubernetes集群

## 高可用集群方案

kubeadm现在支持两种高可用集群方案：堆叠etcd集群和外部etcd集群。

### 堆叠etcd

![方案拓扑图](img/kubeadm-ha-topology-stacked-etcd.svg)

堆叠etcd集群每个控制节点运行一个kube-apiserver，kube-scheduler 和 kube-controller-manager实例，这些实例只与运行在该控制节点上的etcd服务通信。

这个方案设置简单，更易于副本管理。但是，堆叠集群存在耦合失败的风险，如果一个节点故障，etcd和control plane都会丢失，并且冗余会受到影响，可以通过添加更多控制节点来降低此风险。

**堆叠etcd集群至少需要三个控制节点。**

### 外部etcd

这种方案的 etcd 分布式数据存储集群在独立于控制节点的其他节点上运行。

![方案拓扑图](img/kubeadm-ha-topology-external-etcd.svg)

这种拓扑结构解耦了控制平面和 etcd 成员。因此，它提供了一种 HA 设置，其中失去控制平面实例或者 etcd 成员的影响较小，并且不会像堆叠的 HA 拓扑那样影响集群冗余。

但是，此拓扑需要两倍于堆叠 HA 拓扑的主机数量。

具有此拓扑的 HA 集群至少需要三个用于控制平面节点的主机和三个用于 etcd 节点的主机。

## 创建高可用集群

### 为apiserver创建负载均衡器

***使用负载均衡器需要许多配置。您的集群搭建可能需要不同的配置。下面的例子只是其中的一方面配置。***

参见[负载均衡选项指南](https://github.com/kubernetes/kubeadm/blob/master/docs/ha-considerations.md#options-for-software-load-balancing)

### 堆叠etcd

1. 初始化控制节点

```bash
 kubeadm init --control-plane-endpoint "LOAD_BALANCER_DNS:LOAD_BALANCER_PORT" --upload-certs
```

- --kubernetes-version 设置要使用的 Kubernetes 版本。建议将 kubeadm、kebelet、kubectl 和 Kubernetes 的版本匹配。
- --control-plane-endpoint 设置成负载均衡器的地址或 DNS 和端口。
- --upload-certs 将在所有控制平面实例之间的共享证书上传到集群。如果正好相反，你更喜欢手动地通过控制平面节点或者使用自动化 工具复制证书，请删除此标志并参考如下部分[证书分配手册](https://kubernetes.io/zh/docs/setup/production-environment/tools/kubeadm/high-availability/#manual-certs)。