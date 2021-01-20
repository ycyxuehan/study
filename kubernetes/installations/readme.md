# 安装kubernetes集群

本文档使用centos7.2009作为基础操作系统，使用kubeadm为安装工具。可以使用本文档逐步操作安装kubernetes集群，也可使用自动化安装脚本[autoinstall.sh](autoinstall.md)来快速安装。

安装不同配置的集群需要不同的节点数，每节点CPU核心不低于2，内存不少于2GB，至多需要9个节点。

本文档使用192.168.0.0/24 为节点网络。

## 环境配置

使用[环境配置](configenv.md)文档来配置运行环境，建议所有节点使用完全一致的配置。

## 安装软件包

由于kubernetes已经放弃了docker兼容，这里是由contained作为容器运行时(CRI).

### 安装containerd

参见[安装containerd](containerd.md)

### 安装kubelet,kubeadm,kubectl

```bash
cat <<EOF > /etc/yum.repos.d/kubernetes.repo
[kubernetes]
name=Kubernetes
baseurl=https://mirrors.aliyun.com/kubernetes/yum/repos/kubernetes-el7-x86_64
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://mirrors.aliyun.com/kubernetes/yum/doc/yum-key.gpg https://mirrors.aliyun.com/kubernetes/yum/doc/rpm-package-key.gpg
EOF
yum install -y kubelet kubeadm kubectl
```

## 创建kubeadm配置

### 单节点

```yaml
# 配置kubelet使用systemd作为cgroups驱动
apiVersion: kubelet.config.k8s.io/v1beta1
kind: KubeletConfiguration
cgroupDriver: systemd
---
apiVersion: kubeadm.k8s.io/v1beta2
kind: ClusterConfiguration
kubernetesVersion: stable
controlPlaneEndpoint: "192.168.0.200:8443"
imageRepository: registry.bing89.com/kubernetes
networking:
  podSubnet: 10.244.0.0/16
```

### 基于堆叠etcd的cluster

```yaml
# 配置kubelet使用systemd作为cgroups驱动
apiVersion: kubelet.config.k8s.io/v1beta1
kind: KubeletConfiguration
cgroupDriver: systemd
---
apiVersion: kubeadm.k8s.io/v1beta2
kind: ClusterConfiguration
kubernetesVersion: stable
imageRepository: registry.bing89.com/kubernetes
controlPlaneEndpoint: 192.168.0.200:8443
apiServer:
  certSANs:
  - "k8smaster1"
  - "k8smaster2"
  - "k8smaster3"
  - "192.168.0.200"
  - "192.168.0.201"
  - "192.168.0.202"
  - "192.168.0.203"
networking:
  podSubnet: 10.244.0.0/16

---
#配置kubeproxy使用ipvs。似乎不配置也行
apiVersion: kubeproxy.config.k8s.io/v1alpha1
kind: KubeProxyConfiguration
mode: ipvs
EOF
```

### 基于外部etcd集群的cluster

```yaml
# 配置kubelet使用systemd作为cgroups驱动
apiVersion: kubelet.config.k8s.io/v1beta1
kind: KubeletConfiguration
cgroupDriver: systemd
---
apiVersion: kubeadm.k8s.io/v1beta2
kind: ClusterConfiguration
kubernetesVersion: stable
controlPlaneEndpoint: "192.168.0.200:8443"
imageRepository: registry.bing89.com/kubernetes
etcd:
    external:
        endpoints: ["https://192.168.0.211:2379","https://192.168.0.212:2379","https://192.168.0.213:2379"]
        caFile: /etc/kubernetes/pki/etcd/ca.crt
        certFile: /etc/kubernetes/pki/apiserver-etcd-client.crt
        keyFile: /etc/kubernetes/pki/apiserver-etcd-client.key
apiServer:
  certSANs: ["192.168.0.201","192.168.0.202","192.168.0.203"]
```

## 初始化集群

### 初始化单节点集群

```bash
kubeadm init --config /etc/kubernetes/kubeadmcfg.yaml
```

### 初始化基于堆叠etcd的集群

参照[使用kubeadm安装基于堆叠etcd的kubernetes](install_cluster_with_stacked_etcd.md)

### 初始化基于外部etcd集群的集群

参照[使用kubeadm安装基于外部etcd集群的kubernetes](install_cluster_with_outside_etcd.md)