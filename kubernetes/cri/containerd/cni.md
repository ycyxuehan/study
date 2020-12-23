# CNI plugins
containerd 的网络是以插件形式提供的，如果是yum安装，会自动安装contianerd-cni 插件，位置在`/opt/cni/bin`。

## 配置cni
containerd的cni配置文件位于`/etc/cni/net.d`

### 创建网桥

```bash
#临时创建
NETBRIDGE=brcontainer0
ip link add name ${NETBRIDGE} type bridge
ip addr add 172.17.0.1/16 dev ${NETBRIDGE}
ip link set dev ${NETBRIDGE} up
```

```bash
cat <<EOF >/etc/sysconfig/network-scripts/ifcfg-brcontainer0
TYPE="Bridge"
UUID="ba72d9bd-9be9-4a87-a587-ee9e070052d2"
DEVICE="brcontainer0"
ONBOOT="yes"
BOOTPROTO="static"
IPADDR="172.17.0.1"
NETMASK="255.255.255.0"
EOF
systemctl restart network
```

