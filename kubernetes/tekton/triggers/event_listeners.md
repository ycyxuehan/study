


#  EventListener

EventListener是一个Kubernetes自定义资源，它允许用户以声明的方式使用JSON有效负载处理传入的基于HTTP的事件。EventListeners公开一个可寻址的“接收器”，传入的事件被定向到该接收器。用户可以声明TriggerBindings以从事件中提取字段，并将其应用于TriggerTemplates以创建Tekton资源。此外，事件侦听器允许使用事件侦听器进行轻量级事件处理.

## Multi-Tenant Concerns

EventListener实际上是Tekton的另一种客户机形式，而不是您在别处看到的通过kubectl或tkn使用的示例。特别是，基于HTTP的事件绕过了通过kubeconfig文件和kubectl config系列命令获得的普通Kubernetes身份验证路径。



因此，决定是否使用前需要考虑下列事项

- 最好将集群中的（每个）EventListener公开给外部世界。

- 最佳控制EventListener和下面描述的底层API对象如何访问、创建和更新集群中与Tekton相关的API对象。

至少，每个EventListener都有其ServiceAccountName，如下所述，所有通过“接收器”的事件都会导致使用分配给该ServiceAccount的权限进行任何Tekton资源交互。



但是，如果您需要对不同触发器和侦听器上的一组Tekton资源具有不同级别的权限，而不是所有触发器或侦听器都可以以相同的方式操作某些Tekton资源，那么一个简单的单个EventListener将不够。

## Multiple EventListeners 
- 每个namespace一个listener
    您可以创建多个EventListener对象，其中您的触发器和拦截器集分布在EventListeners中。

    如果您在这些事件侦听器各自的命名空间中创建它们，那么很容易将不同的权限分配给每个事件侦听器的ServiceAccount以满足您的需要。而且，命名空间的创建常常与一组默认的ServiceAccounts和Secrets耦合在一起，这些ServiceAccounts和Secrets也被定义。因此，可以想象的是，政府采取了一些措施。您只需更新自动创建的服务帐户的权限。

    可能的缺点：-总的来说，具有相关机密和服务帐户的名称空间被证明是基础etcd存储库Kubernetes中最昂贵的项。在更大的集群中，etcd存储容量可能会成为一个问题。-多个EventListeners意味着多个HTTP端口必须公开给访问“Sink”的外部实体。如果您碰巧在集群和外部实体之间有一个HTTP防火墙，这意味着要付出更多的管理成本，为每个服务打开防火墙中的端口，除非您可以使用Kubernetes入口作为一组事件侦听器的路由抽象层。

 - 每个namespace多个listener
    对于管理员来说，每个命名空间有多个EventListener很可能意味着更多的ServiceAccount/Secret/RBAC操作，因为作为命名空间创建一部分的那些工件的一些内置生成不适用。

    但是，通过减少名称空间的数量，可以节省一些etcd存储成本。

    多个事件侦听器和潜在的防火墙问题仍然适用（同样，除非使用入口）。

## EventListenerTrigger的ServiceAccount

通过在EventListenerTrigger上设置ServiceAccount实现更细粒度的权限控制。
您仍然需要创建额外的ServiceAccount。
但是保持在1个名称空间内，并将EventListeners及其相关“Sinks”的数量最小化，可以最大限度地减少对etcd存储和在不使用Ingress情况下的防火墙的端口关注。

## 语法

为一个EventListener定义一个配置文件，需要定义以下字段：

- 依赖项

    - apiVersion 指定api版本，例如 triggers.tekton.dev/v1alpha1
    - kind  指定 EventListener 资源对象.
    - metadata 指定唯一标识EventListener资源对象的数据，例如name.
    - spec  指定EventListener资源对象的配置信息。为了让EventListener执行一些操作，规范必须包括：
        - triggers 指定要运行的Triggers的列表
        - serviceAccountName - 指定EventListener用于创建资源的ServiceAccount
- 可选项
    - serviceType 指定接收器pod公开的服务类型

### ServiceAccountName

ServiceAccountName 是依赖项。EventListener的sink用于创建Tekton资源的ServiceAccount。ServiceAccount需要具有以下规则的角色

```yaml
kind: Role
apiVersion: rbac.authorization.k8s.io/v1
metadata:
  name: tekton-triggers-example-minimal
rules:
# Permissions for every EventListener deployment to function
- apiGroups: ["triggers.tekton.dev"]
  resources: ["eventlisteners", "triggerbindings", "triggertemplates"]
  verbs: ["get"]
- apiGroups: [""]
  # secrets are only needed for Github/Gitlab interceptors, serviceaccounts only for per trigger authorization
  resources: ["configmaps", "secrets", "serviceaccounts"]
  verbs: ["get", "list", "watch"]
# Permissions to create resources in associated TriggerTemplates
- apiGroups: ["tekton.dev"]
  resources: ["pipelineruns", "pipelineresources", "taskruns"]
  verbs: ["create"]
```

如果EventListener使用ClusterTriggerBindings,需要使用含有ClusterRole的ServiceAccount.

### Triggers

triggers是依赖项。每个EventListenerll可以包含一个或者多个triggers。一个trigger包括：

- name - (可选) 一个有效的kubernetes name
- interceptors - (可选)) 要使用的interceptors 列表
- bindings - 需要使用或嵌入到TriggerBindingsSpecs使用的TriggerBindings引用列表
- template - TriggerTemplate的名称

```yaml
triggers:
  - name: trigger-1
    interceptors:
      - github:
          eventTypes: ["pull_request"]
    bindings:
      - name: pipeline-binding
        ref:  pipeline-binding
      - name: message-binding
        spec:
            params:
              - name: message
                value: Hello from the Triggers EventListener!
    template:
      name: pipeline-template
```
此外，为了支持多租户样式的场景，其中管理员可能不希望所有触发器都具有与EventListener相同的权限，可以选择在触发器级别设置ServiceAccount，并在创建资源时使用ServiceAccount（如果存在）来代替EventListener的ServiceAccount：

```yaml
triggers:
  - name: trigger-1
    serviceAccount:
      name: trigger-1-sa
      namespace: event-listener-namespace
    interceptors:
      - github:
          eventTypes: ["pull_request"]
    bindings:
      - name: pipeline-binding
        ref:  pipeline-binding
      - name: message-binding
        ref:  message-binding
    template:
      name: pipeline-template
```

EventListener的默认ClusterRole允许从任何命名空间读取ServiceAccounts

### ServiceType
ServiceType字段是可选项。EventListener sinks通过kubernetes的Service来expose。默认情况下，serviceType是ClusterIP，这意味着运行在同一Kubernetes集群中的任何pod都可以通过其集群DNS访问服务。其他有效值是NodePort和LoadBalancer。

对于要连接到集群的外部服务（例如GitHub发送webhook），请查看有关公开EventListeners的指南。

## Logging

```bash
kubectl get pods --selector eventlistener=my-eventlistener
```
## Lables

默认情况下，EventListeners将自动将以下标签附加到它创建的所有资源

Name|Description
--|--
triggers.tekton.dev/eventlistener|生成资源的EventListener的名称
triggers.tekton.dev/trigger|生成资源的Trigger 的名称
triggers.tekton.dev/eventid|传入事件的uid

## Interceptors

EventListener中的触发器可以选择性地指定Interceptors，以修改触发器的行为或负载。

事件Interceptors目前可以有几种不同的形式：

- Webhook Interceptors
- GitHub Interceptors
- GitLab Interceptors
- CEL Interceptors

### Webhook Interceptors

Webhook拦截器允许用户配置包含业务逻辑的外部k8s对象。这些当前在Webhook字段下指定，该字段包含对Kubernetes服务的ObjectReference。如果指定了Webhook侦听器，EventListener接收器将通过HTTP将传入事件转发到侦听器引用的服务。服务需要处理事件并返回响应。响应的状态代码确定处理是否成功-200响应表示侦听器成功，该处理应继续，任何其他状态代码将停止触发器处理。返回的请求（body和headers）被EventListener用作新的事件负载，并传递给TriggerBinding。拦截器有一个带有键值对的可选头字段，将在发送前与事件头合并；必须指定规范名称。

当指定多个拦截器时，请求将按顺序通过每个拦截器进行管道传输以进行处理，例如，第一个拦截器的响应的头/正文将作为请求发送给第二个拦截器。如果需要，拦截器有责任保存头/体数据。最后一个拦截器的响应正文和头用于资源绑定/模板化。

### Event Interceptor Services

要成为Event Interceptor，Kubernetes对象应该：
- 在80号端口上有一个常规的Kubernetes v1服务
- 通过HTTP接受JSON有效负载
- 使用JSON有效负载接受HTTP POST请求。
- 如果EventListener应继续处理事件，则返回HTTP 200 OK状态
- 返回一个JSON主体。这将被EventListener用作任何进一步处理的事件负载。如果拦截器不需要修改主体，它可以简单地返回它接收到的主体。
- 返回其他链接拦截器或任何绑定可能需要的任何头。

**如果需要，拦截器有责任保存头/体数据。最后一个拦截器的响应正文和头用于资源绑定/模板化。**

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: EventListener
metadata:
  name: listener-interceptor
spec:
  serviceAccountName: tekton-triggers-example-sa
  triggers:
    - name: foo-trig
      interceptors:
        - webhook:
            header:
              - name: Foo-Trig-Header1
                value: string-value
              - name: Foo-Trig-Header2
                value:
                  - array-val1
                  - array-val2
            objectRef:
              kind: Service
              name: gh-validate
              apiVersion: v1
              namespace: default
      bindings:
        - ref: pipeline-binding
      template:
        name: pipeline-template
```

### GitHub Interceptors
GitHub拦截器包含验证和过滤来自GitHub的webhook的逻辑。支持的功能包括使用GitHub文档中概述的逻辑验证来自GitHub的webhook，以及过滤传入事件。
要将此拦截器用作验证程序，请使用您选择的方法创建一个机密字符串，并将GitHub webhook配置为使用该机密值。创建包含此值的Kubernetes机密，并将其作为对github拦截器的引用传递。
要将此侦听器用作筛选器，请将要接受的事件类型添加到eventTypes字段。在GitHub文档中可以找到有效值。
传入请求的正文/头将保留在此侦听器的响应中。

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: EventListener
metadata:
  name: github-listener-interceptor
spec:
  serviceAccountName: tekton-triggers-github-sa
  triggers:
    - name: github-listener
      interceptors:
        - github:
            secretRef:
              secretName: github-secret
              secretKey: secretToken
            eventTypes:
              - pull_request
      bindings:
        - ref: github-binding
      template:
        name: github-template
```

### GitLab Interceptors

GitLab拦截器包含用于验证和筛选来自GitLab的请求的逻辑。支持的功能包括使用GitLab文档中概述的逻辑验证webhook是否实际来自GitLab，以及根据事件类型筛选传入事件。事件类型可以在GitLab文档中找到。
要将此拦截器用作验证程序，请使用您选择的方法创建一个机密字符串，并将GitLab webhook配置为使用该机密值。创建包含此值的Kubernetes机密，并将其作为对gitlab拦截器的引用传递。
要将此侦听器用作筛选器，请将要接受的事件类型添加到eventTypes字段。
传入请求的正文/头将保留在此侦听器的响应中。

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: EventListener
metadata:
  name: gitlab-listener-interceptor
spec:
  serviceAccountName: tekton-triggers-example-sa
  triggers:
    - name: foo-trig
      interceptors:
        - gitlab:
            secretRef:
              secretName: foo
              secretKey: bar
            eventTypes:
              - Push Hook
      bindings:
        - name: pipeline-binding
          ref:  pipeline-binding
      template:
        name: pipeline-template
```

### CEL Interceptors

使用CEL表达式语言，CEL拦截器可用于筛选或修改传入事件。

有关表达式语言语法的详细信息，请阅读cel spec语言定义。

下面的cel trig with matches触发器筛选没有与“pull_request”匹配的“X-GitHub-Event”头的事件。

它还修改了传入的请求，向JSON主体添加了一个额外的键，并从钩子主体中截短了一个字符串。

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: EventListener
metadata:
  name: cel-listener-interceptor
spec:
  serviceAccountName: tekton-triggers-example-sa
  triggers:
    - name: cel-trig-with-matches
      interceptors:
        - cel:
            filter: "header.match('X-GitHub-Event', 'pull_request')"
            overlays:
            - key: extensions.truncated_sha
              expression: "body.pull_request.head.sha.truncate(7)"
      bindings:
      - name: pipeline-binding
      template:
        name: pipeline-template
    - name: cel-trig-with-canonical
      interceptors:
        - cel:
            filter: "header.canonical('X-GitHub-Event') == 'push'"
      bindings:
      - ref: pipeline-binding
      template:
        name: pipeline-template
```
除了CEL提供的标准表达式外，触发器还支持一些处理事件数据CEL表达式的有用函数。
传入请求的正文/头将保留在此侦听器的响应中。

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: EventListener
metadata:
  name: cel-listener-interceptor
spec:
  serviceAccountName: tekton-triggers-example-sa
  triggers:
    - name: cel-trig-with-matches
      interceptors:
        - cel:
            filter: "header.match('X-GitHub-Event', 'pull_request')"
            overlays:
            - key: extensions.truncated_sha
              expression: "body.pull_request.head.sha.truncate(7)"
      bindings:
      - name: pipeline-binding
      template:
        name: pipeline-template
    - name: cel-trig-with-canonical
      interceptors:
        - cel:
            filter: "header.canonical('X-GitHub-Event') == 'push'"
      bindings:
      - ref: pipeline-binding
      template:
        name: pipeline-template
```

如果要处理此触发器并应用覆盖，则筛选器表达式必须返回真值。
可选地，不能提供筛选器表达式，并且覆盖将应用于传入的正文

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: EventListener
metadata:
  name: cel-eventlistener-no-filter
spec:
  serviceAccountName: tekton-triggers-example-sa
  triggers:
    - name: cel-trig
      interceptors:
        - cel:
            overlays:
            - key: extensions.truncated_sha
              expression: "body.pull_request.head.sha.truncate(7)"
      bindings:
      - ref: pipeline-binding
      template:
        name: pipeline-template
```
#### Overlays
CEL拦截器支持“overlays”，这些是在返回到事件侦听器之前应用到主体的CEL表达式。
```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: EventListener
metadata:
  name: example-with-multiple-overlays
spec:
  serviceAccountName: tekton-triggers-example-sa
  triggers:
    - name: cel-trig
      interceptors:
        - cel:
            overlays:
            - key: extensions.truncated_sha
              expression: "body.pull_request.head.sha.truncate(7)"
            - key: extensions.branch_name
              expression: "body.ref.split('/')[2]"
      bindings:
      - ref: pipeline-binding
      template:
        name: pipeline-template
```
在本例中，绑定将看到两个附加字段：
假设输入看起来像这样：
```json
{
  "ref": "refs/heads/master",
  "pull_request": {
    "head": {
      "sha": "6113728f27ae82c7b1a177c8d03f9e96e0adf246"
    }
  }
}
```
输出看起来就会是这样
```json
{
  "ref": "refs/heads/master",
  "pull_request": {
    "head": {
      "sha": "6113728f27ae82c7b1a177c8d03f9e96e0adf246"
    }
  },
  "extensions": {
    "truncated_sha": "6113728",
    "branch_name": "master"
  }
}
```
overlay的key元素可以在body中创建一个新元素或者覆盖一个现有元素

```yaml
- key: body.pull_request.head.short_sha
  expression: "truncate(body.pull_request.head.sha, 7)"
```
short_sha将会添加到一个已存在的body
```json
{
  "ref": "refs/heads/master",
  "pull_request": {
    "head": {
      "sha": "6113728f27ae82c7b1a177c8d03f9e96e0adf246",
      "short_sha": "6113728"
    }
  }
}
```

甚至可以通过提供与现有值的路径匹配的键来替换现有字段。
作为overlay的内容apply之后都可以使用绑定提取
```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: TriggerBinding
metadata:
  name: pipeline-binding-with-cel-extensions
spec:
  params:
  - name: gitrevision
    value: $(body.extensions.branch_name)
  - name: branch
    value: $(body.pull_request.head.short_sha)
```
## Example
[参见Github](https://github.com/tektoncd/triggers/tree/master/examples)