# 配置运行环境

## 升级内核

由于centos内核实在太老旧，需要升级系统内核。

```bash
rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org
rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
yum --enablerepo=elrepo-kernel install kernel-ml -y
sed -i s/saved/0/g /etc/default/grub
grub2-set-default "$(cat /boot/efi/EFI/centos/grub.cfg |grep menuentry|grep 'menuentry '|head -n 1|awk -F "'" '{print $2}')"
#查看默认启动版本
grub2-editenv list
grub2-mkconfig -o /boot/efi/EFI/centos/grub.cfg && reboot

```

***注意：这里默认升级到最新版本的linux内核， 也可以自行指定内核版本。不升级也能够运行kubernetes***

## 配置selinux

```bash
sed -i 's/SELINUX=enforcing/SELINUX=permissive/' /etc/selinux/config
setenforce 0
```

***也可完全关闭selinux(`sed -i 's/SELINUX=enforcing/SELINUX=disabled/' /etc/selinux/config`)，但并不推荐。***

## 配置防火墙

为kubernetes创建一个zone

```bash
firewall-cmd --new-zone=kubernetes --permanent
firewall-cmd --zone=kubernetes --add-source=192.168.0.0/24
```

kubernetes 使用下表所述端口：

**控制节点**

协议|方向|端口范围|作用|使用者
--|--|--|--|--
TCP|入站|6443*|Kubernetes API 服务器|所有组件
TCP|入站|10250|Kubelet API|kubelet 自身、Control plane
TCP|入站|10251|kube-scheduler|kube-scheduler 自身
TCP|入站|10252|kube-controller-manager|kube-controller-manager 自身

```bash
firewall-cmd --zone=kubernetes --add-port=6443/tcp --permanent
firewall-cmd --zone=kubernetes --add-port=10250/tcp --permanent
firewall-cmd --zone=kubernetes --add-port=10251/tcp --permanent
firewall-cmd --zone=kubernetes --add-port=10252/tcp --permanent
```

**工作节点**

协议|方向|端口范围|作用|使用者
--|--|--|--|--
TCP|入站|10250|Kubelet API|kubelet 自身、控制平面组件
TCP|入站|30000-32767|NodePort 服务**|所有组件

```bash
firewall-cmd --zone=kubernetes --add-port=10250/tcp --permanent
```

**etcd节点**

协议|方向|端口范围|作用|使用者
--|--|--|--|--
TCP|入站|2379-2380|etcd server client API|kube-apiserver, etcd

```bash
firewall-cmd --zone=kubernetes --add-port=2379/tcp --permanent
firewall-cmd --zone=kubernetes --add-port=2380/tcp --permanent
```

***也可完全关闭防火墙(`systemctl disable --now firewalld`)，但并不推荐***

## 关闭swap

```bash
swapoff -a && sysctl -w vm.swappiness=0
#/etc/fstab中swap相关的需要删除，否则会导致重启时kubelet启动失败
sed -i 's|\(^/dev/mapper/centos-swap.*\)|#\1|' /etc/fstab
```

## 配置系统参数

```bash
iptables -P FORWARD ACCEPT
cat <<EOF >  /etc/sysctl.d/k8s.conf
net.bridge.bridge-nf-call-ip6tables=1
net.bridge.bridge-nf-call-iptables=1
vm.swappiness=0
net.ipv4.ip_forward=1
EOF
sysctl --system
```

## 配置hosts

```bash
cat <<EOF >>/etc/hosts
#私有仓库地址
192.168.0.248 registry.bing89.com
#kubernetes cluster nodes
192.168.0.201 k8smaster1
192.168.0.202 k8smaster2
192.168.0.203 k8smaster3
192.168.0.211 k8snode1
192.168.0.212 k8snode2
192.168.0.213 k8snode3
192.168.0.221 etcd1
192.168.0.222 etcd2
192.168.0.223 etcd3
EOF
```

## 配置ssh免密登录

***如果不使用自动安装脚本，可以不配置***

```bash
HOSTS=(k8smaster1 k8smaster2 k8smaster3 k8snode1 k8snode2 k8snode3)
ssh-keygen
for host in ${HOSTS[@]};do ssh-copyid ${host}; done
```

## 配置ipvs内核模块

***如果不安装高可用集群，可以不配置***

```bash
cat >>/etc/profile<<EOF
modprobe ip_vs
modprobe ip_vs_rr
modprobe ip_vs_wrr
modprobe ip_vs_sh
modprobe nf_conntrack
EOF
source /etc/profile
```

