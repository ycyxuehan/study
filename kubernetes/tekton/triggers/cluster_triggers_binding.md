# ClusterTriggerBindings

ClusterTriggerBindings类似于TriggerBinding，它用于从事件负载中提取字段。唯一的区别是它是集群范围的，并且是为了鼓励集群范围内的可重用性而设计的。您可以在任何命名空间中的任何EventListener中引用ClusterTriggerBinding。

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: ClusterTriggerBinding
metadata:
  name: pipeline-clusterbinding
spec:
  params:
    - name: gitrevision
      value: $(body.head_commit.id)
    - name: gitrepositoryurl
      value: $(body.repository.url)
    - name: contenttype
      value: $(header.Content-Type)
```

可以在触发器中指定多个ClusterTriggerBindings。可以在多个触发器中使用ClusterTriggerBinding。
如果使用ClusterTriggerBinding，则应添加绑定类型。默认类型是TriggerBinding，它表示一个名称空间的TriggerBinding。

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: EventListener
metadata:
  name: listener-clustertriggerbinding
spec:
  serviceAccountName: tekton-triggers-example-sa
  triggers:
    - name: foo-trig
      bindings:
        - ref: pipeline-clusterbinding
          kind: ClusterTriggerBinding
        - ref: message-clusterbinding
          kind: ClusterTriggerBinding
      template:
        name: pipeline-template
```
