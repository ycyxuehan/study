# TriggerBindings

TriggerBindings 通过绑定事件与触发器，从而从事件捕获字段并存储为参数。 分离bindings和template是便于复用他们。

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: TriggerBinding
metadata:
  name: pipeline-binding
spec:
  params:
  - name: gitrevision
    value: $(body.head_commit.id)
  - name: gitrepositoryurl
    value: $(body.repository.url)
  - name: contenttype
    value: $(header.Content-Type)
```

TriggerBindings连接到EventListener中的TriggerTemplates，这是pod实际实例化的地方，它“侦听”各个事件

TriggerBindings 可以提供传递给triigerTemplate的参数，每一个参数都有一个name和一个value

## 事件变量插值

triggerbindings 可以使用$(jsonPath表达式)访问http json body 和headers的值。对于header的key是大小写不敏感的。 

```bash
$(body.key1)
$(.body.key)
```

一个$()可以包含另一个$(),最里面的$()的内容将作为外面的$()的jsonPath表达式

```bash
$($(body.b)) => $(body.b)
$($($(body.b))) -> $(body.b)
```

使用转义符\可以使json key包含‘.’。

```bash
# body contains a filed called "tekton.dev" e.g. {"body": {"tekton.dev": "triggers"}}
$(body.tekton\.dev) -> "triggers"
```

一些例子

```bash

`$(body)` is replaced by the entire body.

$(body) -> "{"key1": "value1", "key2": {"key3": "value3"}, "key4": ["value4", "value5", "value6"]}"

$(body.key1) -> "value1"

$(body.key2) -> "{"key3": "value3"}"

$(body.key2.key3) -> "value3"

$(body.key4[0]) -> "value4"

$(body.key4[0:2]) -> "{"value4", "value5"}"

# $(header) is replaced by all of the headers from the event.

$(header) -> "{"One":["one"], "Two":["one","two","three"]}"

$(header.One) -> "one"

$(header.one) -> "one"

$(header.Two) -> "one two three"

$(header.Two[1]) -> "two"
```

## Multiple Bindings

在EventListener中，可以指定多个绑定作为触发器的一部分。这允许您创建可与各种触发器混合和匹配的可重用绑定。例如，具有一个提取事件信息的绑定和另一个提供部署环境信息的绑定的触发器：

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: TriggerBinding
metadata:
  name: event-binding
spec:
  params:
    - name: gitrevision
      value: $(body.head_commit.id)
    - name: gitrepositoryurl
      value: $(body.repository.url)
---
apiVersion: triggers.tekton.dev/v1alpha1
kind: TriggerBinding
metadata:
  name: prod-env
spec:
  params:
    - name: environment
      value: prod
---
apiVersion: triggers.tekton.dev/v1alpha1
kind: TriggerBinding
metadata:
  name: staging-env
spec:
  params:
    - name: environment
      value: staging
---
apiVersion: triggers.tekton.dev/v1alpha1
kind: EventListener
metadata:
  name: listener
spec:
  triggers:
    - name: prod-trigger
      bindings:
        - name: event-binding
        - name: prod-env
      template:
        name: pipeline-template
    - name: staging-trigger
      bindings:
        - name: event-binding
        - name: staging-env
      template:
        name: pipeline-template
```