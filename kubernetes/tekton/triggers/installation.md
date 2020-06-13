# 安装

```bash
kubectl apply --filename https://storage.googleapis.com/tekton-releases/triggers/latest/release.yaml

```

查看pod是否已经运行

```bash
kubectl get pods --namespace tekton-pipelines
NAME                                           READY   STATUS    RESTARTS   AGE
tekton-dashboard-5dd4cf8d8b-csz6b              1/1     Running   0          6h11m
tekton-pipelines-controller-6d55778887-7m9zn   1/1     Running   0          6h12m
tekton-pipelines-webhook-69796f78cf-zzlvg      1/1     Running   0          6h12m
tekton-triggers-controller-788d5f47f4-kmdxw    1/1     Running   0          34m
tekton-triggers-webhook-66dbd655d7-qh52p       1/1     Running   0          34m
```