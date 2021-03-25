# Deployment

## 创建deployment

```bash
kubectl apply -f demo.yaml
```

## 更新deployment

```bash
kubectl --record deployment.apps/nginx-deployment set image deployment.v1.apps/nginx-deployment nginx=nginx:1.16.1
```

或

```bash
kubectl set image deployment/nginx-deployment nginx=nginx:1.19 --record
```

## 查看上线状态

```bash
kubectl rollout status deployment/nginx-deployment
```

## 回滚

回滚到上一个版本

```bash
kubectl rollout undo deployment.v1.apps/nginx-deployment
```

回滚到指定版本

```bash
kubectl rollout undo deployment.v1.apps/nginx-deployment --to-revision=2
```

## 缩放

```bash
kubectl scale deployment.v1.apps/nginx-deployment --replicas=10
```

假设集群启用了Pod 的水平自动缩放， 你可以为 Deployment 设置自动缩放器，并基于现有 Pods 的 CPU 利用率选择 要运行的 Pods 个数下限和上限。

```bash
kubectl autoscale deployment.v1.apps/nginx-deployment --min=10 --max=15 --cpu-percent=80
```

## 暂停/恢复 deployment

注意，这里并不是暂停/恢复服务的运行，而是暂停/恢复处理deployment的变更

### 暂停

```bash
kubectl rollout pause deployment.v1.apps/nginx-deployment
```
**所有的deployment变更操作会被记录，在恢复deployment后会执行这些操作**

### 恢复

```bash
kubectl rollout resume deployment.v1.apps/nginx-deployment
```

## 指定终止时间参数

```bash
kubectl patch deployment.v1.apps/nginx-deployment -p '{"spec":{"progressDeadlineSeconds":600}}'
```