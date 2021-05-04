# calico无法检测到IP的故障处理

报错信息

```bash
kubectl -n kube-system logs -f --tail=20 calico-node-xxxx
# Couldn't autodetect an IPv4 address. If auto-detecting, choose a different autodetection method. Otherwise provide an explicit address.....
```
解决方案

编辑daemonset `calico-node` 添加配置

```yaml
...
- name: IP
    value: autodetect
- name: IP_AUTODETECTION_METHOD
    value: cidr=192.168.0.1/24
- name: CALICO_IPV4POOL_IPIP
    value: Always
...
```
详情参见[calico文档](https://docs.projectcalico.org/reference/node/configuration#ip-autodetection-methods)
