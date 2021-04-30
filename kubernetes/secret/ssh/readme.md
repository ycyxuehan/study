# 包含 SSH 密钥的 Secret

创建密钥

```bash
kubectl create secret generic ssh-key-secret --from-file=ssh-privatekey=/home/`whoami`/.ssh/id_rsa  --from-file=ssh-publickey=/home/`whoami`/.ssh/id_rsa.pub
```