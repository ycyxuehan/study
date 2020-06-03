# Pod 完整配置

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: string
  labels:
   - name: string
  namespace:  string
  annotations:
   - name: string
spec:
  containers:
  - name: string
    image: string
    imagePullPolicy: [Always|Never|ifNotPresent]
    command: [string]
    args: [string]
    workingDir: string
    volumeMounts:
    - name: string
      mountPath: string
      readOnly: [true|false]
    ports:
    - name: string
      containerPort: int
      hostPort: int
      protocol: string
    env:
    - name: string
      value: string
    resources:
      limits:
        memory: string
        cpu: string
      requests:
        cpu: string
        memory: string
    livenessProbe:
      exec:
        command: [string]
      httpGet:
        path: string
        port: int
        host: string
        scheme: string
        httpHeaders:
        - name: string
          value: string
      tcpGet:
        port: int
      initailDelaySenconds: 0
      timeoutSeconds: 0
      periodSeconds: 0
      successThreshold: 0
      failureThreshold: 0
    securityContext:
      privileged: false
  restartPolicy: [Always|Never|OnFailure]
  nodeSelector: object
  imagePulllSecrets:
  - name: string
  hostNetwork: false
  volumes:
  - name: string
    emptyDir: {}
    hostPath:
      path: string
    secret:
      secretName: string
      items:
      - key: string
        path: string
    configMap:
      name: string
      items:
      - key: string
        path: string
```