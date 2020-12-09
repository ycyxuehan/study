# 定制控制节点参数

kubeadm ClusterConfiguration 对象公开了 extraArgs 字段，它可以覆盖传递给控制平面组件（如 APIServer、ControllerManager 和 Scheduler）的默认参数。各组件配置使用如下字段定义：

- apiServer
- controllerManager
- scheduler

使用方式： ```kubeadm init --config <YOUR CONFIG YAML>```

## APIServer 参数

详细信息，请参阅 [kube-apiserver 参考文档](https://kubernetes.io/docs/reference/command-line-tools-reference/kube-apiserver/)

示例：

```yaml
apiVersion: kubeadm.k8s.io/v1beta2
kind: ClusterConfiguration
kubernetesVersion: v1.16.0
apiServer:
  extraArgs:
    advertise-address: 192.168.0.103
    anonymous-auth: "false"
    enable-admission-plugins: AlwaysPullImages,DefaultStorageClass
    audit-log-path: /home/johndoe/audit.log
```

## ControllerManager 参数

有关详细信息，请参阅 [kube-controller-manager 参考文档](https://kubernetes.io/docs/reference/command-line-tools-reference/kube-controller-manager/)。

使用示例：

```yaml
apiVersion: kubeadm.k8s.io/v1beta2
kind: ClusterConfiguration
kubernetesVersion: v1.16.0
controllerManager:
  extraArgs:
    cluster-signing-key-file: /home/johndoe/keys/ca.key
    bind-address: 0.0.0.0
    deployment-controller-sync-period: "50"
```

## Scheduler 参数
有关详细信息，请参阅 [kube-scheduler 参考文档](https://kubernetes.io/docs/reference/command-line-tools-reference/kube-scheduler/)。

使用示例：

```yaml
apiVersion: kubeadm.k8s.io/v1beta2
kind: ClusterConfiguration
kubernetesVersion: v1.16.0
scheduler:
  extraArgs:
    address: 0.0.0.0
    config: /home/johndoe/schedconfig.yaml
    kubeconfig: /home/johndoe/kubeconfig.yaml
```