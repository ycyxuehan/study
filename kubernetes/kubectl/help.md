# kubectl help

```bash
kubectl controls the Kubernetes cluster manager.

 Find more information at: https://kubernetes.io/docs/reference/kubectl/overview/

Basic Commands (Beginner):
  create        Create a resource from a file or from stdin.
  expose        Take a replication controller, service, deployment or pod and expose it as a new
Kubernetes Service
  run           Run a particular image on the cluster
  set           Set specific features on objects

Basic Commands (Intermediate):
  explain       Documentation of resources
  get           Display one or many resources
  edit          Edit a resource on the server
  delete        Delete resources by filenames, stdin, resources and names, or by resources and label
selector

Deploy Commands:
  rollout       Manage the rollout of a resource
  scale         Set a new size for a Deployment, ReplicaSet or Replication Controller
  autoscale     Auto-scale a Deployment, ReplicaSet, or ReplicationController

Cluster Management Commands:
  certificate   Modify certificate resources.
  cluster-info  Display cluster info
  top           Display Resource (CPU/Memory/Storage) usage.
  cordon        Mark node as unschedulable
  uncordon      Mark node as schedulable
  drain         Drain node in preparation for maintenance
  taint         Update the taints on one or more nodes

Troubleshooting and Debugging Commands:
  describe      Show details of a specific resource or group of resources
  logs          Print the logs for a container in a pod
  attach        Attach to a running container
  exec          Execute a command in a container
  port-forward  Forward one or more local ports to a pod
  proxy         Run a proxy to the Kubernetes API server
  cp            Copy files and directories to and from containers.
  auth          Inspect authorization

Advanced Commands:
  diff          Diff live version against would-be applied version
  apply         Apply a configuration to a resource by filename or stdin
  patch         Update field(s) of a resource using strategic merge patch
  replace       Replace a resource by filename or stdin
  wait          Experimental: Wait for a specific condition on one or many resources.
  convert       Convert config files between different API versions
  kustomize     Build a kustomization target from a directory or a remote url.

Settings Commands:
  label         Update the labels on a resource
  annotate      Update the annotations on a resource
  completion    Output shell completion code for the specified shell (bash or zsh)

Other Commands:
  alpha         Commands for features in alpha
  api-resources Print the supported API resources on the server
  api-versions  Print the supported API versions on the server, in the form of "group/version"
  config        Modify kubeconfig files
  plugin        Provides utilities for interacting with plugins.
  version       Print the client and server version information

Usage:
  kubectl [flags] [options]

Use "kubectl <command> --help" for more information about a given command.
Use "kubectl options" for a list of global command-line options (applies to all commands).

```

## 资源缩写

|API 资源| 缩写
--|--
clusters|
componentstatuses|cs
configmaps|cm
daemonsets|ds
deployments|deploy
endpoints|ep
events|ev
horizontalpodautoscalers|hpa
ingresses|ing
jobs|
limitranges|limits
nodes|no
namespaces|ns
networkpolicies|
nodes|no
statefulsets|
persistentvolumeclaims|pvc
persistentvolumes|pv
pods|po
podsecuritypolicies|psp
podtemplates|
replicasets|rs
replicationcontrollers|rc
resourcequotas|quota
cronjob|
secrets|
serviceaccounts|
services|svc
storageclasses|sc
thirdpartyresources|


## 子命令详解

|子命令|语法|说明|
|:---|:---|:---|
annotate|kubectl annotate (-f FILENAME\|TYPE NAME\|TYPE/NAME) <br>KEY_1=VAL_1  ... KEY_N=VAL_N [--overwrite] [--all] [--resource-version=version] [flags]|添加或更新资源对象的annotation信息
api-versions|kubectl api-versions [flags]|列出当前系统支持的API版本列表格式为"group/version"
apply|kubeetl apply-f FILENAME [flags]|从配置文件或 stdin中对资源对象进行配置更新
attach|kubectl attach POD -c CONTAINER [flags]|附着到一个正在运行的容器上
auth|kubectl auth [flags] [options]|检测RBAC权限设置
autoscale|kubectl autoscale (-f FILENAME\|TYPE NAME\|TYPE/NAME) [--min=MINPODS] [--max=MAXPODS] [--cpu--percent=CPU] [--flags]|对Deployment,ReplicaSet,ReplicationController进行水平自动扩容和缩容的设置
cluster-info|kubectl cluster-info [flags]|显示集群 Master和内置服务的信息
completion|kubectl completion SHELL [flags]|输出 shell命令的执行结果码(bash或zsh)
config|kubectl config SUBCOMMAND [flags]|修改 kubeconfig文件
convert|kubectl convert -f FILENAME [flags]|转换配置文件为不同的API版本
cordon|kubectl cordon NODE [flags]|将Node标记为"unschedulable,即隔离"出集群调度范围
create|kubectl create -f FILENAME [flags]|从配置文件或stdin中创建资源对象
delete|kubectl delete (-f FILENAME\|TYPE [NAME\|/NAME\|-l label\|--all]) [flags]|根据配置文件、stdin、资源名称或label selector删除资源对象
describe|kubectl describe (-f FILENAME|TYPE [NAME_PREFIX\|/NAME\|-l label]) [flags]|描述一个或多个资源对象的详细信息
diff|kubectl diff -f FILENAME [options]|查看配置文件与当前系统中正在运行的资源对象的差异
drain|kubectl drain NODE [flags]|首先将Node设置为unschedulable,然后删除在该Node上运行的所有Pod,但不会删除不由 API Server管理的Pod
edit|kubectl edit (-f FILENAME\|TYPE NAME\|TYPE/NAME) [fags]|编辑资源对象的属性,在线更新
excc|kubectl exec POD [-c CONTAINER] [-i] [-t] [flas] [--COMMAND [args..]]|执行一个容器中的命令
explain|kubectl explain [--include-extended-apis=true] [--recursive=false] [flags]|对资源对象属性的详细说明
get|kubectl get (-f FILENAME|TYPE [NAME\|/NAME\|-l label]) [--watch] [--sort-by=FIELD] [[-o\|--output]=OUTPUT_FORMAT] [flags]|显示一个或多个资源对象的概要信息
label|kubectl label (-f FILENAME\|TYPE NAME\|TYPE/NAME) KEY_1=VAL_1 ... KEY _N=VAL_N [--overwrite] [--all [--resource-version=version] [flags]|设置或更新资源对象的 labels
logs|kubectl logs POD [-c CONTAINER] [--follow] [flags]|在屏幕上打印一个容器的日志
patch|kubectl patch (-f FILENAME\|TYPE NAME\|TYPE/NAME) --patch PATCH [flags]<br>kubeetl patch (-f FILENAME\|TYPENAME\|TYPE/NAME) --patch PATCH [flags]|以 merge形式对资源对象的部分字段的值进行修改
plugin|kubectl plugin [flags] [options]|在 kubectl命令行使用用户自定义的插件
replace|kubectl replace -f FILENAME [flags]|从配置文件或 stdin替换资源对象
rollout|kubectl rollout SUBCOMMAND [flags]|对Deployment进行管理可用操作包括: history、 pause resume、 undo status
expose|kubectl expose (-f FILENAME\|TYPE NAME\|TYPE/NAME) [--port=port] [--protocol=TCP\|UDP] [--target-port=number-or-name] [--name=name] [--external-ip=external-ip-of-service)] [--type=type] [flags]|将已经存在的一个RC、ServiceDeployment或pod暴露为一个新的Service
port-forward|kubectl port-forward POD [LOCA_LPORT:]REMOTE_PORT [...[LOCAL_PORT_N: ]REMOTE_PORT_N] [flags]|将本机的某个端口号映射到Pod的端口号,通常用于测试
proxy|kubectl proxy [-port=PORT] [--www=static-dir] [--www-prefix=prefix] [--api-prefix=prefix] [flags]|将本机某个端口号映射到 API Server
rolling-update|kubectl rolling-update OLD_CONTROLLER_NAME (INEW_CONTROLLER_NAME] --image=NEW_CONTAINER_IMAGE|-f NEW_CONTROLLER_SPEC) [flags]|对RC进行滚动升级
run|kubectl run NAME-image=image [-env="key=value"] [--port -port] [--replicas=replicas] [-dry-run=bool] [-overrides=inline-json] [flags]|基于一个镜像在 Kubernetes集群上启动一个 Deployment
scale|kubectl scale-f FILENAME TYPE NAME TYPE/name) --replicas=COUNT [--resource-version=version] [-current-replicas=count] [flags]|扩容、缩容一个 Deployment、 ReplicaSet、rc或job中od的数量
set|kubectl set SUBCOMMAND [flags]|设置资源对象的某个特定信息,目前仅支持修改容器的镜像
taint|kubectltaintNODENAMEKEYvali:TAINTEFFECT KEY N=VAL: TAINT EFFECT N [flags]|设置Node的taint信息,用于将特定的Pod调度到特定的Node的操作,为 Alpha版本的功能
top|kubectl top node kubectl top pod|查看Node或Pod的资源使用情况,需要在集群中运行 Metrics Server
uncordon|kubectl uncordon NODE [flags]|将Node设置为 schedulable
version|kubectl version [-client] [flags]|打印系统的版本信息






