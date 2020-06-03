# Tekton

tekton 是kubernetes官方的开源CI/CD工具

## 快速部署tekton

```bash
#部署tekton
kubectl apply --filename https://storage.googleapis.com/tekton-releases/pipeline/latest/release.yaml
#部署tekton dashboard
kubectl apply --filename https://github.com/tektoncd/dashboard/releases/latest/download/tekton-dashboard-release.yaml
```
由于镜像是从gcr.io下载的，可能需要代理。

```bash
#查看pod
kubectl get pods --namespace tekton-pipelines
NAME                                           READY   STATUS    RESTARTS   AGE
tekton-dashboard-5dd4cf8d8b-csz6b              1/1     Running   0          6h11m
tekton-pipelines-controller-6d55778887-7m9zn   1/1     Running   0          6h12m
tekton-pipelines-webhook-69796f78cf-zzlvg      1/1     Running   0          6h12m

```
## 配置存储

tekton 需要一个PresidentVolume用于运行CI/CD工作流。 默认申请一个5GB的存储，也可以自定义成其他容量。存储选项通过configmap来配置。

```bash
kubectl create configmap config-artifact-pvc \
                         --from-literal=size=10Gi \
                         --from-literal=storageClassName=manual \
                         -o yaml -n tekton-pipelines | kubectl replace -f -
```

## tekton CLI
为了方便使用，需要安装tekton CLI工具tkn。可以使用RPM/DEB安装，也可以二进制安装 [下载地址](https://github.com/tektoncd/cli/releases)。

下载完成之后解压就可以用了，是一个已经编译好的二进制文件。

## Hello World
使用tekton创建一个CI/CD工作流。
tekton通过运行一个镜像，执行指定命令来完成工作流。

下面是一个helloworld配置示例

```yaml
apiVersion: tekton.dev/v1beta1
kind: Task
metadata:
  name: echo
spec:
  steps:
    - name: echo
      image: ubuntu
      command:
        - echo
      args:
        - "Hello World!"
```

应用配置到kubernetes,创建一个工作流task

```bash
kubectl apply -f task.yaml
```

要使用tekton运行这个task，需要创建一个taskrun。

可以使用CLI来查看这个task对应的taskrun配置

```bash
tkn task start echo --dry-run
```

```yaml
apiVersion: tekton.dev/v1beta1
kind: TaskRun
metadata:
  generateName: echo-run-
spec:
  taskRef:
    name: echo
```

使用CLI工具tkn来启动echo task， 也可以使用kubectl

```bash
tkn task start echo
```

使用kubectl

```bash
tkn task start echo --dry-run > taskRun.yaml
kubectl create -f taskRun.yaml
```

可以使用tkn查看最后一个taskrun的日志

```bash
tkn taskrun logs --last -f
```