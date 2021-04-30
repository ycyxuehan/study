# kubectl rollout

kubernetes资源历史版本管理。

查看当前的历史版本

```bash
kubectl -n demo rollout history deploy/demo
```

回滚到指定版本

```bash
kubectl -n demo rollout undo deploy/demo --to-revision=42
```