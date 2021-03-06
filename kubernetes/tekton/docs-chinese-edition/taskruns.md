<!--
---
linkTitle: "TaskRuns"
weight: 2
---
-->

# `TaskRuns`

- [概览](#taskruns)
- [配置`TaskRun`](#configuring-a-taskrun)
  - [指定目标`Task`](#specifying-the-target-task)
  - [指定 `Parameters`](#specifying-parameters)
  - [指定 `Resources`](#specifying-resources)
  - [指定 `ServiceAccount` 证书](#specifying-serviceaccount-credentials)
  - [指定 `Pod` 模板](#specifying-a-pod-template)
  - [指定 `Workspaces`](#specifying-workspaces)
  - [指定 `Sidecars`](#specifying-sidecars)
  - [指定 `LimitRange` ](#specifying-limitrange-values)
  - [配置失败超时时间](#configuring-the-failure-timeout)
- [监控运行状态](#monitoring-execution-status)
  - [监控 `Steps`](#monitoring-steps)
  - [监控 `Results`](#monitoring-results)
- [取消运行`TaskRun`](#cancelling-a-taskrun)
- [事件](events.md#taskruns)
- [代码示例](#code-examples)
  - [示例`TaskRun` 和被引用的`Task`](#example-taskrun-with-a-referenced-task)
  - [示例`TaskRun`和增强的`Task`](#example-taskrun-with-an-embedded-task)
  - [重复使用`Task`](#reusing-a-task)
  - [使用自定义`ServiceAccount`证书](#using-custom-serviceaccount-credentials)

# 概览

`TaskRun`在集群中实例化并运行[`Task`](tasks.md)。`Task`指定一个或多个执行指定部分构建工作的容器并运行的`Steps`。 `TaskRun`按顺序执行这些`Task`中的`Steps`直到所有`Steps` 执行完成或发生错误。

## 配置 `TaskRun`

`TaskRun`定义支持以下字段:

- 必要:
  - [`apiVersion`][kubernetes-overview] - 指定API版本，例如：`tekton.dev/v1beta1`.
  - [`kind`][kubernetes-overview] - 声明资源是`TaskRun`对象.
  - [`metadata`][kubernetes-overview] - 指定说明`TaskRun`对象的唯一身份元数据,比如`name`.
  - [`spec`][kubernetes-overview] - 指定`TaskRun`的配置.
    - [`taskRef` or `taskSpec`](#specifying-the-target-task) - 指定`TaskRun`要运行的 `Tasks`
- 可选:
  - [`serviceAccountName`](#specifying-serviceaccount-credentials) - 指定提供给`TaskRun`执行的自定义证书的`ServiceAccount`对象
  - [`params`](#specifying-parameters) - 为`Task`指定期望的运行时参数
  - [`resources`](#specifying-resources) -指定期望的`PipelineResource` 值。
    -[`inputs`](#specifying-resources) - 指定输入资源。
    -[`outputs`](#specifying-resources) - 指定输出资源。
  - [`timeout`](#configuring-the-failure-timeout) - 指定`TaskRun`失败之前的超时时间。
  - [`podTemplate`](#specifying-a-pod-template) - 指定用于为`Task`启动点配置`Pod`的[`Pod` template](podtemplates.md)
  - [`workspaces`](#specifying-workspaces) - 指定用于为`Task`声明的[`Workspaces`](workspaces.md#using-workspaces-in-tasks)的物理卷。

[kubernetes-overview]:
  https://kubernetes.io/docs/concepts/overview/working-with-objects/kubernetes-objects/#required-fields

### 指定目标 `Task`

指定`TaskRun`中要运行的`Task`，使用`taskRef`字段：

```yaml
spec:
  taskRef:
    name: read-task
```

也可以使用`taskSpec`字段直接嵌入期望的`Task`定义到`TaskRun`中：

```yaml
spec:
  taskSpec:
    resources:
      inputs:
        - name: workspace
          type: git
    steps:
      - name: build-and-push
        image: gcr.io/kaniko-project/executor:v0.17.1
        # specifying DOCKER_CONFIG is required to allow kaniko to detect docker credential
        env:
          - name: "DOCKER_CONFIG"
            value: "/tekton/home/.docker/"
        command:
          - /kaniko/executor
        args:
          - --destination=gcr.io/my-project/gohelloworld
```

### 指定 `Parameters`

如果`Task`拥有[`parameters`](tasks.md#parameters), 可以使用`params`字段指定他们的值:

```yaml
spec:
  params:
    - name: flags
      value: -someflag
```

**注意:** 如果一个参数没有隐式的默认值, 必须显式指定它的值.

### 指定 `Resources`

如果`Task`请求[`Resources`](tasks.md#specifying-resources) (输入或输出) 则必须在`TaskRun`定义中指定它们. 可以使用已存在的[`PipelineResource` objects](resources.md)引用指定`Resources`或直接内嵌它们的定义到`TaskRun`中.

**注意:** `TaskRun`使用 任何引用*或*内嵌的 `Resource` 但不能同时使用它们.

下面例子使用引用指定`Resources`:

```yaml
spec:
  resources:
    inputs:
      - name: workspace
        resourceRef:
          name: java-git-resource
    outputs:
      - name: image
        resourceRef:
          name: my-app-image
```

下面例子使用内嵌定义指定`Resources`:

```yaml
spec:
  resources:
    inputs:
      - name: workspace
        resourceSpec:
          type: git
          params:
            - name: url
              value: https://github.com/pivotal-nader-ziada/gohelloworld
```

**注意:** 可以使用 `paths` 字段来[覆盖一个`Resource`的路径](resources.md#overriding-where-resources-are-copied-from).

### 指定 `Pod` 模板

可以指定充当在`Task`将要运行的指定容器的`Pod`启动点的配置的[`Pod` 模板](podtemplates.md)配置。这允许为`TaskRun`定制自定义`Pod`配置。

下面的例子中, `Task` 指定一个 `volumeMount` (`my-cache`) 对象， 挂载一个由`TaskRun`提供的 `PersistentVolumeClaim` 卷。
也使用 `SchedulerName` 字段配置了一个定制的调度器。`Pod`使用规范的权限(non-root)执行。

```yaml
apiVersion: tekton.dev/v1beta1
kind: Task
metadata:
  name: mytask
  namespace: default
spec:
  steps:
    - name: writesomething
      image: ubuntu
      command: ["bash", "-c"]
      args: ["echo 'foo' > /my-cache/bar"]
      volumeMounts:
        - name: my-cache
          mountPath: /my-cache
---
apiVersion: tekton.dev/v1beta1
kind: TaskRun
metadata:
  name: mytaskrun
  namespace: default
spec:
  taskRef:
    name: mytask
  podTemplate:
    schedulerName: volcano
    securityContext:
      runAsNonRoot: true
      runAsUser: 1001
    volumes:
    - name: my-cache
      persistentVolumeClaim:
        claimName: my-volume-claim
```

### 指定 `Workspaces`

如果一个`Task`指定了一个或多个`Workspaces`, 必须在`TaskRun`中为他们挂载对应物理卷。例如，可以像下面那样为`workspace`挂载一个`persistentVolumeClaim`卷：

```yaml
workspaces:
- name: myworkspace # must match workspace name in the Task
  persistentVolumeClaim:
    claimName: mypvc # this PVC must already exist
  subPath: my-subdir
```

更多信息，参考以下主题:
- 挂载`Workspaces`到`Volumes`, 查看 [Using `Workspace` variables in `TaskRuns`](workspaces.md#using-workspace-variables-in-taskruns).
- 受支持的`Volume` 类型列表, 查看 [Specifying `VolumeSources` in `Workspaces`](workspaces.md#specifying-volumesources-in-workspaces).
- 端到端示例, 查看 [`Workspaces` in a `TaskRun`](../examples/v1beta1/taskruns/workspace.yaml).

### 指定 `Sidecars`

A `Sidecar` is a container that runs alongside the containers specified
in the `Steps` of a task to provide auxiliary support to the execution of
those `Steps`. For example, a `Sidecar` can run a logging daemon, a service
that updates files on a shared volume, or a network proxy.

Tekton supports the injection of `Sidecars` into a `Pod` belonging to
a `TaskRun` with the condition that each `Sidecar` running inside the
`Pod` are terminated as soon as all `Steps` in the `Task` complete execution. 
This might result in the `Pod` including each affected `Sidecar` with a 
retry count of 1 and a different container image than expected.

We are aware of the following issues affecting Tekton's implementation of `Sidecars`:

- The configured `nop` image **must not** provide the command that the
`Sidecar` is expected to run, otherwise it will not exit, resulting in the `Sidecar`
running forever and the Task eventually timing out. For more information, see the
[associated issue](https://github.com/tektoncd/pipeline/issues/1347).

- The `kubectl get pods` command returns the status of the `Pod` as "Completed" if a
`Sidecar` exits successfully and as "Error" if a `Sidecar` exits with an error,
disregarding the exit codes of the container images that actually executed the `Steps`
inside the `Pod`. Only the above command is affected. The `Pod's` description correctly
denotes a "Failed" status and the container statuses correctly denote their exit codes
and reasons.

### Specifying `LimitRange` values

In order to only consume the bare minimum amount of resources needed to execute one `Step` at a
time from the invoked `Task`, Tekton only requests the *maximum* values for CPU, memory, and ephemeral
storage from within each `Step`. This is sufficient as `Steps` only execute one at a time in the `Pod`.
Requests other than the maximum values are set to zero.

When a [`LimitRange`](https://kubernetes.io/docs/concepts/policy/limit-range/) parameter is present in
the namespace in which `TaskRuns` are executing and *minimum* values are specified for container resource requests,
Tekton searches through all `LimitRange` values present in the namespace and uses the *minimums* instead of 0.

For more information, see the [`LimitRange` code example](../examples/v1beta1/taskruns/no-ci/limitrange.yaml).

## Configuring the failure timeout

You can use the `timeout` field to set the `TaskRun's` desired timeout value in minutes.
If you do not specify this value in the `TaskRun`, the global default timeout value applies.
If you set the timeout to 0, the `TaskRun` fails immediately upon encountering an error.

The global default timeout is set to 60 minutes when you first install Tekton. You can set
a different global default timeout value using the `default-timeout-minutes` field in
[`config/config-defaults.yaml`](./../config/config-defaults.yaml).

The `timeout` value is a `duration` conforming to Go's
[`ParseDuration`](https://golang.org/pkg/time/#ParseDuration) format. For example, valid
values are `1h30m`, `1h`, `1m`, and `60s`. If you set the global timeout to 0, all `TaskRuns`
that do not have an individual timeout set will fail immediately upon encountering an error.

### Specifying `ServiceAccount' credentials

You can execute the `Task` in your `TaskRun` with a specific set of credentials by 
specifying a `ServiceAccount` object name in the `serviceAccountName` field in your `TaskRun`
definition. If you do not explicitly specify this, the `TaskRun` executes with the credentials
specified in the `configmap-defaults` `ConfigMap`. If this default is not specified, `TaskRuns`
will execute with the [`default` service account](https://kubernetes.io/docs/tasks/configure-pod-container/configure-service-account/#use-the-default-service-account-to-access-the-api-server)
set for the target [`namespace`](https://kubernetes.io/docs/concepts/overview/working-with-objects/namespaces/).

For more information, see [`ServiceAccount`](auth.md).

## 监控运行状态

As your `TaskRun` executes, its `status` field accumulates information on the execution of each `Step`
as well as the `TaskRun` as a whole. This information includes start and stop times, exit codes, the
fully-qualified name of the container image, and the corresponding digest.

**Note:** If any `Pods` have been [`OOMKilled`](https://kubernetes.io/docs/tasks/administer-cluster/out-of-resource/)
by Kubernetes, the `TaskRun` is marked as failed even if its exit code is 0.

The following example shows the `status` field of a `TaskRun` that has executed successfully:

```yaml
completionTime: "2019-08-12T18:22:57Z"
conditions:
- lastTransitionTime: "2019-08-12T18:22:57Z"
  message: All Steps have completed executing
  reason: Succeeded
  status: "True"
  type: Succeeded
podName: status-taskrun-pod-6488ef
startTime: "2019-08-12T18:22:51Z"
steps:
- container: step-hello
  imageID: docker-pullable://busybox@sha256:895ab622e92e18d6b461d671081757af7dbaa3b00e3e28e12505af7817f73649
  name: hello
  terminated:
    containerID: docker://d5a54f5bbb8e7a6fd3bc7761b78410403244cf4c9c5822087fb0209bf59e3621
    exitCode: 0
    finishedAt: "2019-08-12T18:22:56Z"
    reason: Completed
    startedAt: "2019-08-12T18:22:54Z"
  ```

The following tables shows how to read the overall status of a `TaskRun`:

`status`|`reason`|`completionTime` is set|Description
:-------|:-------|:---------------------:|--------------:
Unknown|Started|No|The TaskRun has just been picked up by the controller.
Unknown|Pending|No|The TaskRun is waiting on a Pod in status Pending.
Unknown|Running|No|The TaskRun has been validate and started to perform its work.
Unknown|TaskRunCancelled|No|The user requested the TaskRun to be cancelled. Cancellation has not be done yet.
True|Succeeded|Yes|The TaskRun completed successfully.
False|Failed|Yes|The TaskRun failed because one of the steps failed.
False|\[Error message\]|No|The TaskRun encountered a non-permanent error, and it's still running. It may ultimately succeed.
False|\[Error message\]|Yes|The TaskRun failed with a permanent error (usually validation).
False|TaskRunCancelled|Yes|The TaskRun was cancelled successfully.
False|TaskRunTimeout|Yes|The TaskRun timed out.

When a `TaskRun` changes status, [events](events.md#taskruns) are triggered accordingly.

### Monitoring `Steps`

If multiple `Steps` are defined in the `Task` invoked by the `TaskRun`, you can monitor their execution
status in the `steps.results` field using the following command, where `<name>` is the name of the target
`TaskRun`:

The exact Task Spec used to instantiate the TaskRun is also included in the Status for full auditability.

### Steps

The corresponding statuses appear in the `status.steps` list in the order in which the `Steps` have been
specified in the `Task` definition.

### Monitoring `Results`

If one or more `results` fields have been specified in the invoked `Task`, the `TaskRun's` execution
status will include a `Task Results` section, in which the `Results` appear verbatim, including original
line returns and whitespace. For example:

```yaml
Status:
  # […]
  Steps:
  # […]
  Task Results:
    Name:   current-date-human-readable
    Value:  Thu Jan 23 16:29:06 UTC 2020

    Name:   current-date-unix-timestamp
    Value:  1579796946

```

## Cancelling a `TaskRun`

To cancel a `TaskRun` that's currently executing, update its definition
to mark it as cancelled. When you do so, all running `Pods` associated with
that `TaskRun` are deleted. For example:

```yaml
apiVersion: tekton.dev/v1alpha1
kind: TaskRun
metadata:
  name: go-example-git
spec:
  # […]
  status: "TaskRunCancelled"
```

## 代码示例

为更好的理解 `TaskRuns`, 学习以下示例:

- [`TaskRun`使用引用的`Task`](#example-taskrun-with-a-referenced-task)
- [`TaskRun`使用内嵌的`Task`](#example-taskrun-with-an-embedded-task)
- [重复使用`Task`](#example-of-reusing-a-task)
- [指定`ServiceAccount`](#example-of-specifying-a-service-account)

### Example `TaskRun` with a referenced `Task`

在这个示例中，名为`read-repo-run`的`TaskRun`调用并运行了一个已存在的名为`read-task`的`Task`。这个`Task`使用一个被`TaskRun`引用为`go-example-git`的git类型输入资源(`PipelineResource`)。

```yaml
apiVersion: tekton.dev/v1alpha1
kind: PipelineResource
metadata:
  name: go-example-git
spec:
  type: git
  params:
    - name: url
      value: https://github.com/pivotal-nader-ziada/gohelloworld
---
apiVersion: tekton.dev/v1beta1
kind: Task
metadata:
  name: read-task
spec:
  resources:
    inputs:
      - name: workspace
        type: git
  steps:
    - name: readme
      image: ubuntu
      script: cat workspace/README.md
---
apiVersion: tekton.dev/v1beta1
kind: TaskRun
metadata:
  name: read-repo-run
spec:
  taskRef:
    name: read-task
  resources:
    inputs:
      - name: workspace
        resourceRef:
          name: go-example-git
```

### `TaskRun`使用内嵌`Task`

这个例子中，一个名为`build-push-task-run-2`的`TaskRun`直接运行了`taskSpec`字段的内嵌定义的`Task`:

```yaml
apiVersion: tekton.dev/v1alpha1
kind: PipelineResource
metadata:
  name: go-example-git
spec:
  type: git
  params:
    - name: url
      value: https://github.com/pivotal-nader-ziada/gohelloworld
---
apiVersion: tekton.dev/v1beta1
kind: TaskRun
metadata:
  name: build-push-task-run-2
spec:
  resources:
    inputs:
      - name: workspace
        resourceRef:
          name: go-example-git
  taskSpec:
    resources:
      inputs:
        - name: workspace
          type: git
    steps:
      - name: build-and-push
        image: gcr.io/kaniko-project/executor:v0.17.1
        # specifying DOCKER_CONFIG is required to allow kaniko to detect docker credential
        env:
          - name: "DOCKER_CONFIG"
            value: "/tekton/home/.docker/"
        command:
          - /kaniko/executor
        args:
          - --destination=gcr.io/my-project/gohelloworld
```

也可以在`TaskRun`中使用内嵌的资源定义. 下面的例子中，名为`read-repo`的 `TaskRun`提供了一个内嵌的git类型资源:

```yaml
apiVersion: tekton.dev/v1beta1
kind: TaskRun
metadata:
  name: read-repo
spec:
  taskRef:
    name: read-task
  resources:
    inputs:
      - name: workspace
        resourceSpec:
          type: git
          params:
            - name: url
              value: https://github.com/pivotal-nader-ziada/gohelloworld
```

### 重复使用 `Task`

下面的例子说明了如何复用 `Task`，接下来，会有一系列的`TaskRuns`实例化一个名为`dockerfile-build-and-push`的`Task`
`TaskRuns` 引用不同的 `Resources` 作为它们的输入。示例`Task.`的完整定义，参考 [构建和发布一个Docker镜像](tasks.md#building-and-pushing-a-docker-image)

`TaskRun` 构建 `mchmarny/rester-tester`:

```yaml
# This is the referenced PipelineResource
metadata:
  name: mchmarny-repo
spec:
  type: git
  params:
    - name: url
      value: https://github.com/mchmarny/rester-tester.git
```

```yaml
# This is the TaskRun
spec:
  taskRef:
    name: dockerfile-build-and-push
  params:
    - name: IMAGE
      value: gcr.io/my-project/rester-tester
  resources:
    inputs:
      - name: workspace
        resourceRef:
          name: mchmarny-repo
```

`TaskRun` 从`googlecloudplatform/cloud-builder`构建`wget` builder:

```yaml
# This is the referenced PipelineResource
metadata:
  name: cloud-builder-repo
spec:
  type: git
  params:
    - name: url
      value: https://github.com/googlecloudplatform/cloud-builders.git
```

```yaml
# This is the TaskRun
spec:
  taskRef:
    name: dockerfile-build-and-push
  params:
    - name: IMAGE
      value: gcr.io/my-project/wget
    # Optional override to specify the subdirectory containing the Dockerfile
    - name: DIRECTORY
      value: /workspace/wget
  resources:
    inputs:
      - name: workspace
        resourceRef:
          name: cloud-builder-repo
```

`TaskRun`从`googlecloudplatform/cloud-builder`的`17.06.1`分支构建 `docker` builder:

```yaml
# This is the referenced PipelineResource
metadata:
  name: cloud-builder-repo
spec:
  type: git
  params:
    - name: url
      value: https://github.com/googlecloudplatform/cloud-builders.git
```

```yaml
# This is the TaskRun
spec:
  taskRef:
    name: dockerfile-build-and-push
  params:
    - name: IMAGE
      value: gcr.io/my-project/docker
    # Optional overrides
    - name: DIRECTORY
      value: /workspace/docker
    - name: DOCKERFILE_NAME
      value: Dockerfile-17.06.1
  resources:
    inputs:
      - name: workspace
        resourceRef:
          name: cloud-builder-repo
```

### 使用自定义`ServiceAccount`证书

下面的示例展示了如何使用一个指定的`ServiceAccount`来访问私有的`git`仓库：

```yaml
apiVersion: tekton.dev/v1beta1
kind: TaskRun
metadata:
  name: test-task-with-serviceaccount-git-ssh
spec:
  serviceAccountName: test-task-robot-git-ssh
  resources:
    inputs:
      - name: workspace
        type: git
  steps:
    - name: config
      image: ubuntu
      command: ["/bin/bash"]
      args: ["-c", "cat README.md"]
```

上面的代码片段中, `serviceAccountName: test-build-robot-git-ssh`引用下面的`ServiceAccount`:

```yaml
apiVersion: v1
kind: ServiceAccount
metadata:
  name: test-task-robot-git-ssh
secrets:
  - name: test-git-ssh
```

`name: test-git-ssh`引用下面的`Secret`:

```yaml
apiVersion: v1
kind: Secret
metadata:
  name: test-git-ssh
  annotations:
    tekton.dev/git-0: github.com
type: kubernetes.io/ssh-auth
data:
  # Generated by:
  # cat id_rsa | base64 -w 0
  ssh-privatekey: LS0tLS1CRUdJTiBSU0EgUFJJVk.....[example]
  # Generated by:
  # ssh-keyscan github.com | base64 -w 0
  known_hosts: Z2l0aHViLmNvbSBzc2g.....[example]
```

### 以非root用户运行Step容器

All steps that do not require to be run as a root user should make use of TaskRun features to 
designate the container for a step runs as a user without root permissions. As a best practice, 
running containers as non root should be built into the container image to avoid any possibility 
of the container being run as root. However, as a further measure of enforcing this practice, 
TaskRun pod templates can be used to specify how containers should be run within a TaskRun pod. 

An example of using a TaskRun pod template is shown below to specify that containers running via this 
TaskRun's pod should run as non root and run as user 1001 if the container itself does not specify what 
user to run as:

```yaml
apiVersion: tekton.dev/v1beta1
kind: TaskRun
metadata:
  generateName: show-non-root-steps-run-
spec:
  taskRef:
    name: show-non-root-steps
  podTemplate:
    securityContext:
      runAsNonRoot: true
      runAsUser: 1001
```

If a Task step specifies that it is to run as a different user than what is specified in the pod template, 
the step's `securityContext` will be applied instead of what is specified at the pod level. An example of 
this is available as a [TaskRun example](../examples/v1beta1/taskruns/run-steps-as-non-root.yaml).

More information about Pod and Container Security Contexts can be found via the [Kubernetes website](https://kubernetes.io/docs/tasks/configure-pod-container/security-context/#set-the-security-context-for-a-pod).

---

Except as otherwise noted, the content of this page is licensed under the
[Creative Commons Attribution 4.0 License](https://creativecommons.org/licenses/by/4.0/),
and code samples are licensed under the
[Apache 2.0 License](https://www.apache.org/licenses/LICENSE-2.0).
