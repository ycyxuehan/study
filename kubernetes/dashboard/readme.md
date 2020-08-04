# Kubernetes Dashboard

安装参见 https://github.com/kubernetes/dashboard.git

## 获取登录token

先创建service account

```bash
kubectl -n kubernetes-dashboard describe secret $(kubectl -n kubernetes-dashboard get secr
et | grep admin-user | awk '{print $1}')
```
复制打印的token
