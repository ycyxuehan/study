<!--
---
linkTitle: "Tasks"
weight: 1
---
-->
# Tasks

- [概览](#overview)
- [配置一个`Task`](#configuring-a-task)
  - [`Task`对比`ClusterTask`](#task-vs-clustertask)
  - [定义`Steps`](#defining-steps)
    - [保留目录](#reserved-directories)
    - [在`Steps`中运行脚本](#running-scripts-within-steps)
  - [指定`Parameters`](#specifying-parameters)
  - [指定`Resources`](#specifying-resources)
  - [指定`Workspaces`](#specifying-workspaces)
  - [反射`results`](#emitting-results)
  - [指定`Volumes`](#specifying-volumes)
  - [指定一个`Step`模板](#specifying-a-step-template)
  - [指定`Sidecars`](#specifying-sidecars)
  - [添加说明](#adding-a-description)
  - [使用变量替换](#using-variable-substitution)
    - [替换参数和资源](#substituting-parameters-and-resources)
    - [替换`Array`参数](#substituting-array-parameters)
    - [替换`Workspace`路径](#substituting-workspace-paths)
    - [替换`Volume`名称和类型](#substituting-volume-names-and-types)
- [代码示例](#code-examples)
  - [编译并发布Docker镜像](#building-and-pushing-a-docker-image)
  - [挂在多个`Volumes`](#mounting-multiple-volumes)
  - [挂载`ConfigMap`作为`Volume`资源](#mounting-a-configmap-as-a-volume-source)
  - [使用`Secret`作为环境资源](#using-a-secret-as-an-environment-source)
  - [在`Task`中使用`Sidecar`](#using-a-sidecar-in-a-task)
- [调试](#debugging)
  - [检查文件结构](#inspecting-the-file-structure)
  - [检查`Pod`](#inspecting-the-pod)

## 概览

`Task`是定义并按指定顺序排布的作为持续集成流一部分的执行器(`Steps`)的集合。`Task`在Kubernetes集群上作为Podcast运行。当`ClusterTask`在整个集群可用时，`Task`在指定的命名空间(`Namespace`)可用。

`Task` 声明包含下列元素:

- [Parameters](#specifying-parameters)
- [Resources](#specifying-resources)
- [Steps](#defining-steps)
- [Workspaces](#specifying-workspaces)
- [Results](#storing-execution-results)

## `Task`配置

`Task` 定义支持下列字段:

- 依赖字段:
  - [`apiVersion`][kubernetes概览] - 指定API版本，比如,`tekton.dev/v1beta1`.
  - [`kind`][kubernetes概览] - 声明是一个 `Task` 资源对象.
  - [`metadata`][kubernetes概览] - 指定说明`Task`资源对象唯一身份元数据，例如：`name`.
  - [`spec`][kubernetes概览] - `Task`资源的详细配置信息。
  - [`steps`](#defining-steps) - 运行`Task`的一个或多个容器镜像.
- 可选字段:
  - [`description`](#adding-a-description) - 有意义的`Task`说明信息.
  - [`params`](#specifying-parameters) - `Task`的执行器参数.
  - [`resources`](#specifying-resources) - **alpha only** 指定`Task`需要的[`PipelineResources`](resources.md), PipelineResources不存在将创建。
    - [`inputs`](#specifying-resources) - `Task`需要摄取的输入.
    - [`outputs`](#specifying-resources) - `Task`产生的输出.
  - [`workspaces`](#specifying-workspaces) - `Task`依赖的卷路径.
  - [`results`](#emitting-results) - 指定`Tasks` 要写入执行器结果的文件.
  - [`volumes`](#specifying-volumes) - 指定一个或多个卷用于`Task`的 `Steps` .
  - [`stepTemplate`](#specifying-a-step-template) - 指定一个 `Container` 步骤定义用于 `Task`的所有`Steps`.
  - [`sidecars`](#specifying-sidecars) - 指定 `Sidecar` 容器与`Task`的`Steps`一起运行.

[kubernetes概览]:
  https://kubernetes.io/docs/concepts/overview/working-with-objects/kubernetes-objects/#required-fields

下面的非功能性示例演示了上述大多数字段的使用:

```yaml
apiVersion: tekton.dev/v1beta1
kind: Task
metadata:
  name: example-task-name
spec:
  params:
    - name: pathToDockerFile
      type: string
      description: The path to the dockerfile to build
      default: /workspace/workspace/Dockerfile
  resources:
    inputs:
      - name: workspace
        type: git
    outputs:
      - name: builtImage
        type: image
  steps:
    - name: ubuntu-example
      image: ubuntu
      args: ["ubuntu-build-example", "SECRETS-example.md"]
    - image: gcr.io/example-builders/build-example
      command: ["echo"]
      args: ["$(params.pathToDockerFile)"]
    - name: dockerfile-pushexample
      image: gcr.io/example-builders/push-example
      args: ["push", "$(resources.outputs.builtImage.url)"]
      volumeMounts:
        - name: docker-socket-example
          mountPath: /var/run/docker.sock
  volumes:
    - name: example-volume
      emptyDir: {}
```

### `Task`对比`ClusterTask`
`ClusterTask`在整个集群范围可用，用于代替只在单个命名空间可用的`Task`.

`ClusterTask`与`Task`行为一致，因此此文档的所有内容都可以应用于它们。

**注意:** 当使用`ClusterTask`时, 你必须显式的设置`ClusterTask`的`taskRef`字段的子字段`kind`。如果不指定， `kind`子字段默认设置为`Task.`

下面是一个使用`ClusterTask`的Pipeline示例:

```yaml
apiVersion: tekton.dev/v1beta1
kind: Pipeline
metadata:
  name: demo-pipeline
  namespace: default
spec:
  tasks:
    - name: build-skaffold-web
      taskRef:
        name: build-push
        kind: ClusterTask
      params: ....
```

### 定义 `Steps`
`Step`是一个执行拥有特定输入并产生特定输出的工具的容器的引用。通过给`Task`添加`Steps`定义包含所需`Steps`列表的`step`字段(必需)。`Steps`按照其出现在列表中的顺序来顺序执行。

下列依赖应用于`steps`字段引用的每一个容器:

- 容器镜像必须遵守[容器规范](./container-contract.md).
- 每一个容器都运行到完成或者直到第一个错误发生为止。
- CPU,内存和瞬间的存储资源请求将会置零，如果指定这些值，若容器的资源请求没有超过`Task`的所有容器资源之和，则通过`Namespace`的`LimitRanges`设置最小值。这确保执行`Task`的Pod只请求足够的资源运行`Task`的单一容器而不是一次为`Task`的所有容器分配资源。

#### 保留目录

Tekton运行 `Tasks`时会将一些目录视为特殊目录：

* `/workspace` - 资源](#resources)和 [工作空间](#workspaces)的挂载目录. `Task`可以通过[变量](variables.md)找到它们。
* `/tekton` - 这个目录用于Tekton指定功能:
    * `/tekton/results` [results](#results)写入目录.`Task`可以通过[变量](variables.md)找到它们
    * 其他目录请查看 [Tekton行为详情](developers/README.md#reserved-directories)
      **注意：用户不应该依赖他们的特定行为，因为它可能在未来发生变化。**

#### 在`Steps`中运行脚本(script)

一个`step`可以指定一个`script`包含脚本内容的字段。
脚本如果储存在容器中，会被调用并且`args` 会直接传递给它。

**注意:** `step`的`script`(优先)字段和`command`字段是互斥的。

若脚本的第一行是[shebang](https://en.wikipedia.org/wiki/Shebang_(Unix))注释，则脚本无法启动。例如:

```bash
#!/bin/sh
set -xe
```

可以指定期望的脚本解析器来覆盖默认的shebang注解。解析器在`Step's`容器镜像中必须存在。

执行一个`bash`脚本:

```yaml
steps:
- image: ubuntu  # contains bash
  script: |
    #!/usr/bin/env bash
    echo "Hello from Bash!"
```

执行一个`python`脚本(py3.0):

```yaml
steps:
- image: python  # contains python
  script: |
    #!/usr/bin/env python3
    print("Hello from Python!")
```

执行一个`nodejs`脚本:

```yaml
steps:
- image: node  # contains node
  script: |
    #!/usr/bin/env node
    console.log("Hello from Node!")
```

直接执行一个位于`workspace`的脚本:

```yaml
steps:
- image: ubuntu
  script: |
    #!/usr/bin/env bash
    /workspace/my-script.sh  # provided by an input resource
```

运行一个位于容器镜像内的脚本:

```yaml
steps:
- image: my-image  # contains /bin/my-binary
  script: |
    #!/usr/bin/env bash
    /bin/my-binary
```

### 指定 `Parameters`
可以像编译标志和自定义名称一样在`Task`执行时提供一些指定参数。 `Parameters`通过相应的`TaskRun`传递给`Task`.

参数名称:

- 只能由字母、数字, 短线 (`-`)和下划线 (`_`)组成.
- 必须以字母或下划线 (`_`)开头.

例如, `fooIs-Bar_`是合法的参数名, `barIsBa$`或`0banana`则是非法参数名.

每一个声明的参数都有一个可以被设置为`array`或`string`的 `type`字段. 当`task`的整个执行过程中，为任务提供的编译标志的数量各不相同时，`array`非常有用。`type`字段在不指定时，默认设置为`string`。 实参的的值被提供时，它的解析类型会再次和`type`字段验证。

下面的例子展示了如何在`Task`中使用`Parameters`. 例子的`Task`声明了名为`flags`(`array`类型)和`someURL` (`string`类型)的两个字段，并在`steps.args`列表中使用了他们。 可以使用通配符`*`展开一个`array`的所有值. 例子中, `flags` 包含通配符: `$(params.flags[*])`.

**注意:** 输入参数值在整个`Task`中都可以通过[变量替换](#using-variable-substitution)作为变量使用。

```yaml
apiVersion: tekton.dev/v1beta1
kind: Task
metadata:
  name: task-with-parameters
spec:
  params:
    - name: flags
      type: array
    - name: someURL
      type: string
  steps:
    - name: build
      image: my-builder
      args: ["build", "$(params.flags[*])", "url=$(params.someURL)"]
```

下面的`TaskRun` 在`flags`参数中提供了动态数量的字符串:

```yaml
apiVersion: tekton.dev/v1beta1
kind: TaskRun
metadata:
  name: run-with-parameters
spec:
  taskRef:
    name: task-with-parameters
  params:
    - name: flags
      value:
        - "--set"
        - "arg1=foo"
        - "--randomflag"
        - "--someotherflag"
    - name: someURL
      value: "http://google.com"
```

### 指定 `Resources`

`Task`定义可以由[`PipelineResources`](resources.md#using-resources)提供指定输入输出资源。 

使用`input`字段提供`Task`执行需要的上下文和(或)数据。如果`Task`的输出是下一个`Task`的输入，必须要确保数据在`Task`的 `/workspace/output/resource_name/`可用。

例如：

```yaml
resources:
  outputs:
    name: storage-gcs
    type: gcs
steps:
  - image: objectuser/run-java-jar #https://hub.docker.com/r/objectuser/run-java-jar/
    command: [jar]
    args:
      ["-cvf", "-o", "/workspace/output/storage-gcs/", "projectname.war", "*"]
    env:
      - name: "FOO"
        value: "world"
```

**注意**：如果`Task`依赖输出资源功能，则不能在`Task`的`steps`字段的容器里挂载`/workspace/output`。

下面的例子中, `tar-artifact` 同时用于输入和输出。因此，输入资源被拷贝到使用`targetPath`字段指定的 `customworkspace` 目录。
`untar` `Step` 解压缩包到 `tar-scratch-space` 目录。`edit-tar` `Step` 添加了一个新文件,然后`tar-it-up` `Step`创建了一个新的压缩包并保存在`/workspace/customworkspace/`目录。
当`Task`执行完成,结果压缩包保存在`/workspace/customworkspace`目录并上传到`tar-artifact`字段定义的bucket中。

```yaml
resources:
  inputs:
    name: tar-artifact
    targetPath: customworkspace
  outputs:
    name: tar-artifact
steps:
 - name: untar
    image: ubuntu
    command: ["/bin/bash"]
    args: ['-c', 'mkdir -p /workspace/tar-scratch-space/ && tar -xvf /workspace/customworkspace/rules_docker-master.tar -C /workspace/tar-scratch-space/']
 - name: edit-tar
    image: ubuntu
    command: ["/bin/bash"]
    args: ['-c', 'echo crazy > /workspace/tar-scratch-space/rules_docker-master/crazy.txt']
 - name: tar-it-up
   image: ubuntu
   command: ["/bin/bash"]
   args: ['-c', 'cd /workspace/tar-scratch-space/ && tar -cvf /workspace/customworkspace/rules_docker-master.tar rules_docker-master']
```

### 指定 `Workspaces`

[`Workspaces`](workspaces.md#using-workspaces-in-tasks)允许指定`Task`在运行期间需要的一个或多个卷。`Tasks`推荐**至多**使用一个可写的`Workspace`。例如

```yaml
spec:
  steps:
  - name: write-message
    image: ubuntu
    script: |
      #!/usr/bin/env bash
      set -xe
      echo hello! > $(workspaces.messages.path)/message
  workspaces:
  - name: messages
    description: The folder where we write the message to
    mountPath: /custom/path/relative/to/root
```

获取更多信息, 查看[在`Tasks`中使用`Workspaces`](workspaces.md#using-workspaces-in-tasks)与 [`TaskRun`里的`Workspaces`](../examples/v1beta1/taskruns/workspace.yaml) 示例文件.

### Emitting results

使用`results`字段`Task`保存执行结果的一个或多个文件。这些文件保存在`/tekton/results`目录。如果至少一个文件在`results`字段中被指定，这个目录将会在运行时自动创建。提供`name`和`description`来指定一个文件。

下面的例子中, `Task`在`results`字段指定了两个文件:
`current-date-unix-timestamp` 和 `current-date-human-readable`。

```yaml
apiVersion: tekton.dev/v1beta1
kind: Task
metadata:
  name: print-date
  annotations:
    description: |
      A simple task that prints the date
spec:
  results:
    - name: current-date-unix-timestamp
      description: The current date in unix timestamp format
    - name: current-date-human-readable
      description: The current date in human readable format
  steps:
    - name: print-date-unix-timestamp
      image: bash:latest
      script: |
        #!/usr/bin/env bash
        date +%s | tee /tekton/results/current-date-unix-timestamp
    - name: print-date-human-readable
      image: bash:latest
      script: |
        #!/usr/bin/env bash
        date | tee /tekton/results/current-date-human-readable
```

存储结果可以使用于[`Task`级别](./pipelines.md#configuring-execution-results-at-the-task-level)或 [`Pipeline`级别](./pipelines.md#configuring-execution-results-at-the-pipeline-level).

**注意:** `Task's`结果的最大大小被kubernetes的容器终端消息功能(termination message feature)限制，结果通过这种机制(容器终端消息功能)回传给controller。
目前这个限制是["4096 bytes"](https://github.com/kubernetes/kubernetes/blob/96e13de777a9eb57f87889072b68ac40467209ac/pkg/kubelet/container/runtime.go#L632)。
结果以JSON格式写入到终端消息并且Tekton使用这些对象传递附件信息给controller。因此，`Task`结果最适合保存少量数据，比如提交SHA，分支名称，临时命名空间等等。

如果`Task`需要写入大量的小容量结果，你可以通过拆分`Step`分别保存结果来绕过这个限制，因此每个`step`都有它自己的终端消息。
关于容量限制, 一个有用的异常消息是: `Termination message is above max allowed size 4096, caused by large task result`。
因为Tekton也使用终端消息来获取内部信息，所以真实可用的大小都会小于4096字节。为大于1KB的数据使用[`Workspace`](#specifying-workspaces)在`Tasks`和 `Pipeline`之间传递数据。


### 指定 `Volumes`
指定`Task`的`Steps`执行依赖的一个或多个直接创建用于输入输出资源的附加[`Volumes`](https://kubernetes.io/docs/concepts/storage/volumes/)


例如，可以使用 `Volumes` 做以下事情:

- [挂载Kubernetes `Secret`](auth.md).
- 创建`emptyDir` 持久 `Volume` 缓存多个 `Steps`之间的数据。
- 挂载[Kubernetes `ConfigMap`](https://kubernetes.io/docs/tasks/configure-pod-container/configure-pod-configmap/)
  作为 `Volume`资源.
- 挂载主机的docker接口以使用`Dockerfile`创建镜像。

  **注意:** 在集群使用`docker build`创建镜像非常**不安全**并且仅用于演示目的。使用[kaniko](https://github.com/GoogleContainerTools/kaniko)代替这个操作。

### 指定`Step`模板
`stepTemplate`字段用于指定用于`task`的所有`Steps`的启动点的[`Container`](https://kubernetes.io/docs/concepts/containers/)配置。
在`Steps`中指定的各个配置将在出现重叠时取代模板。

下面的例子中, `Task`指定一个包含值为`bar`的`FOO`的环境变量的`stepTemplate`字段。`Task`的第一个`Step`使用了变量`FOO`的值，第二个`Step`将`FOO`的值覆盖为`baz`。

```yaml
stepTemplate:
  env:
    - name: "FOO"
      value: "bar"
steps:
  - image: ubuntu
    command: [echo]
    args: ["FOO is ${FOO}"]
  - image: ubuntu
    command: [echo]
    args: ["FOO is ${FOO}"]
    env:
      - name: "FOO"
        value: "baz"
```

### 指定 `Sidecars`

`sidecars`字段指定一个[`Containers`](https://kubernetes.io/docs/concepts/containers/)列表与`Task`的`Steps`一起运行。
可以使用`Sidecars`提供辅助功能, 就像[Docker Hub](https://hub.docker.com/_/docker)或运行一个你的APP在测试时可以命中的模拟API服务。
`Sidecars`在`Task`运行之前启动并在`Task`完成之后删除。
更多信息, 参考[`Sidecars` in `TaskRuns`](taskruns.md#specifying-sidecars).

下面的例子中,`Step`使用了Docker-in-Docker `Sidecar` 来编译一个镜像:

```yaml
steps:
  - image: docker
    name: client
    script: |
        #!/usr/bin/env bash
        cat > Dockerfile << EOF
        FROM ubuntu
        RUN apt-get update
        ENTRYPOINT ["echo", "hello"]
        EOF
        docker build -t hello . && docker run hello
        docker images
    volumeMounts:
      - mountPath: /var/run/
        name: dind-socket
sidecars:
  - image: docker:18.05-dind
    name: server
    securityContext:
      privileged: true
    volumeMounts:
      - mountPath: /var/lib/docker
        name: dind-storage
      - mountPath: /var/run/
        name: dind-socket
volumes:
  - name: dind-storage
    emptyDir: {}
  - name: dind-socket
    emptyDir: {}
```

像 `Steps`一样,Sidecar也可运行脚本:

```yaml
sidecars:
  image: busybox
  name: hello-sidecar
  script: |
    echo 'Hello from sidecar!'
```
**注意:** Tekton当前的`Sidecar`工具包含一个bug。
Tekton使用一个名为`nop`的容器来终结`Sidecars`。其镜像被配置为传递一个标志给Tekton控制器。如果配置的`nop`镜像包含`Sidecar`在接收到`stop`信号之前一直运行的额外命令，将会导致`TaskRun`因超时而出错。
更多信息，参考[issue 1347](https://github.com/tektoncd/pipeline/issues/1347).

### 添加说明

`description`字段是一个允许给`Task`添加说明信息的可选字段。

### 使用变量替换

`Tasks` 允许为以下项目使用变量:

- [parameters和resources](#substituting-parameters-and-resources)
- [`Array`参数](#substituting-array-parameters)
- [`Workspaces`](#substituting-workspace-paths)
- [`Volume`名称和类型](#substituting-volume-names-and-paths)

也可参考 [Tasks变量替换完成列表](./variables.md#variables-available-in-a-task).

#### 替换 parameters和resources

[`params`](#specifying-parameters)与[`resources`](#specifying-resources) 可以像下面那样替换变量值:

- 像下面语法一样为`Task`引用一个名为`<name>`的参数:
  ```shell
  $(params.<name>)
  ```
- 要访问一个变量的值,参考 [变量替换](resources.md#variable-substitution)

#### 替换 `Array` 参数

可以使用`*`通配符展开一个引用的参数。在参数名称后添加操作符`[*]`将数组元素插入到引用位置。

例如, 有一个包含列表内容的 `params`字段, 可以展开它 `command: ["first", "$(params.array-param[*])", "last"]` to `command: ["first", "some", "array", "elements", "last"]`:

```yaml
params:
  - name: array-param
    value:
      - "some"
      - "array"
      - "elements"
```

必须在一个大型的`string`数组里的一个完全隔离的字符串中引用`array`类型的参数。任何其他方式引用一个`array`参数将会引发错误。例如，如果`build-args`是一个`array`类型的参数，下面的例子是一个无效的`Step` 因为字符串不是隔离(就是要作为独立的字符串引用)的。

```yaml
 - name: build-step
      image: gcr.io/cloud-builders/some-image
      args: ["build", "additionalArg $(params.build-args[*])"]
```

同样地, 引用 `build-args` 在一个非`array` 字段也是无效的:

```yaml
 - name: build-step
      image: "$(params.build-args[*])"
      args: ["build", "args"]
```

有效的引用是 `build-args` 参数隔离的且在一个合适的字段里(例子中的`args`字段):

```yaml
 - name: build-step
      image: gcr.io/cloud-builders/some-image
      args: ["build", "$(params.build-args[*])", "additionalArg"]
```

#### 替换`Workspace`路径

可以像下面那样在`Task`里替换`Workspaces`的路径：

```yaml
$(workspaces.myworkspace.path)
```

自从`Volume`名称只在`Task`运行阶段随机生成后，可以使用下列方式替换卷名称：

```yaml
$(workspaces.myworkspace.volume)
```

#### 替换 `Volume` 名称和类型

可以使用参数替换`Volume`名称和[类型](https://kubernetes.io/docs/concepts/storage/volumes/#types-of-volumes). Tekton支持主流`Volume`类型，比如`ConfigMap`, `Secret`和`PersistentVolumeClaim`。关于如果在 `Task.`执行这一类型的替换， 参考 [例子](#using-kubernetes-configmap-as-volume-source)。

## 代码示例

学习下面的示例代码更好的理解如何创建`Tasks`:

- [编译并发布一个镜像](#编译并发布一个镜像)
- [挂载多个`Volumes`](#mounting-multiple-volumes)
- [挂载`ConfigMap`作为`Volume`资源](#mounting-a-configmap-as-a-volume-source)
- [使用`Secret` 作为环境变量](#using-a-secret-as-an-environment-source)
- [在`Task`中使用`Sidecar`](#using-a-sidecar-in-a-task)

小贴士: 简单的附加代码[示例集合](https://github.com/tektoncd/pipeline/tree/master/examples)

### 编译并发布一个镜像

下面的示例`Task`使用`Dockerfile`编译并发布一个镜像.

**注意:** 在集群使用`docker build`创建镜像非常**不安全**并且仅用于演示目的。使用[kaniko](https://github.com/GoogleContainerTools/kaniko)代替这个操作。


```yaml
spec:
  params:
    # These may be overridden, but provide sensible defaults.
    - name: directory
      type: string
      description: The directory containing the build context.
      default: /workspace
    - name: dockerfileName
      type: string
      description: The name of the Dockerfile
      default: Dockerfile
  resources:
    inputs:
      - name: workspace
        type: git
    outputs:
      - name: builtImage
        type: image
  steps:
    - name: dockerfile-build
      image: gcr.io/cloud-builders/docker
      workingDir: "$(params.directory)"
      args:
        [
          "build",
          "--no-cache",
          "--tag",
          "$(resources.outputs.image.url)",
          "--file",
          "$(params.dockerfileName)",
          ".",
        ]
      volumeMounts:
        - name: docker-socket
          mountPath: /var/run/docker.sock

    - name: dockerfile-push
      image: gcr.io/cloud-builders/docker
      args: ["push", "$(resources.outputs.image.url)"]
      volumeMounts:
        - name: docker-socket
          mountPath: /var/run/docker.sock

  # As an implementation detail, this Task mounts the host's daemon socket.
  volumes:
    - name: docker-socket
      hostPath:
        path: /var/run/docker.sock
        type: Socket
```

#### 挂载多个`Volumes`

下面例子展示了挂载多个`Volumes`:

```yaml
spec:
  steps:
    - image: ubuntu
      script: |
        #!/usr/bin/env bash
        curl https://foo.com > /var/my-volume
      volumeMounts:
        - name: my-volume
          mountPath: /var/my-volume

    - image: ubuntu
      script: |
        #!/usr/bin/env bash
        cat /etc/my-volume
      volumeMounts:
        - name: my-volume
          mountPath: /etc/my-volume

  volumes:
    - name: my-volume
      emptyDir: {}
```

#### `ConfigMap`作为`Volume`资源挂载

下面例子展示了如何将`ConfigMap`挂载为`Volume`资源:

```yaml
spec:
  params:
    - name: CFGNAME
      type: string
      description: Name of config map
    - name: volumeName
      type: string
      description: Name of volume
  steps:
    - image: ubuntu
      script: |
        #!/usr/bin/env bash
        cat /var/configmap/test
      volumeMounts:
        - name: "$(params.volumeName)"
          mountPath: /var/configmap

  volumes:
    - name: "$(params.volumeName)"
      configMap:
        name: "$(params.CFGNAME)"
```

#### 使用`Secret`作为环境资源

下面的例子展示了如何使用`Secret`作为环境资源:

```yaml
apiVersion: tekton.dev/v1beta1
kind: Task
metadata:
  name: goreleaser
spec:
  params:
  - name: package
    type: string
    description: base package to build in
  - name: github-token-secret
    type: string
    description: name of the secret holding the github-token
    default: github-token
  resources:
    inputs:
    - name: source
      type: git
      targetPath: src/$(params.package)
  steps:
  - name: release
    image: goreleaser/goreleaser
    workingDir: /workspace/src/$(params.package)
    command:
    - goreleaser
    args:
    - release
    env:
    - name: GOPATH
      value: /workspace
    - name: GITHUB_TOKEN
      valueFrom:
        secretKeyRef:
          name: $(params.github-token-secret)
          key: bot-token
```

#### 在`Task`中使用`Sidecar`

下面的例子展示了如何在`Task`中使用`Sidecar`:

```yaml
apiVersion: tekton.dev/v1beta1
kind: Task
metadata:
  name: with-sidecar-task
spec:
  params:
  - name: sidecar-image
    type: string
    description: Image name of the sidecar container
  - name: sidecar-env
    type: string
    description: Environment variable value
  sidecars:
  - name: sidecar
    image: $(params.sidecar-image)
    env:
    - name: SIDECAR_ENV
      value: $(params.sidecar-env)
  steps:
  - name: test
    image: hello-world
```

## 排错(Debug)

这一节介绍一些排查大部分`Task`中常见错误的技巧。

### 检查文件结构

一个常见的错误是源自配置`Task`的数据中未知的位置, 最常见的部分是，文件被摄入且`Task`的输出应该在`/workspace`目录，但是具体情况可能有所不同。检查`Task`配置文件结构，添加一个将储存在`/workspace`的每个文件都输出到编译日志中的step。例如：

```yaml
- name: build-and-push-1
  image: ubuntu
  command:
  - /bin/bash
  args:
  - -c
  - |
    set -ex
    find /workspace
```

还可以检查每一个用于`Task`的文件的内容：

```yaml
- name: build-and-push-1
  image: ubuntu
  command:
  - /bin/bash
  args:
  - -c
  - |
    set -ex
    find /workspace | xargs cat
```

### 检查`Pod`

通过在期望的阶段暂停`Task`并记录`Pod`,可以在`Task's`运行中的指定阶段检查`Task`使用的`Pod`的内容。例如：

```yaml
- name: pause
  image: docker
  args: ["sleep", "6000"]

```

### 使用非root用户运行Step容器
所有不依赖以root用户运行的步骤应该使用TaskRun功能设计步骤的容器以无root权限的用户运行。
非root运行容器应该内建在容器镜像中来避免任何使用root用户运行的可能是最佳实践。然而，作为执行这一做法的进一步措施，steps 可以使用`securityContext`来指定如何运行容器。

下面是以非root用户运行task的step的示例:

```yaml
apiVersion: tekton.dev/v1beta1
kind: Task
metadata:
  name: show-non-root-steps
spec:
  steps:
    # no securityContext specified so will use 
    # securityContext from TaskRun podTemplate
    - name: show-user-1001
      image: ubuntu
      command:
        - ps
      args:
        - "aux"
    # securityContext specified so will run as  
    # user 2000 instead of 1001
    - name: show-user-2000
      image: ubuntu
      command:
        - ps
      args:
        - "aux"
      securityContext:
        runAsUser: 2000
---
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

上面的例子中，`show-user-2000`步骤通过`securityContext`指定了容器必须以2000(UID)用户来运行。`securityContext`仍需要通过TaskRun的`podTemplate`指定这个TaskRun运行在一个强制非root用户运行的Kubernetes环境作为必要条件。

`runAsNonRoot`属性通过`podTemplate`确认这个`TaskRun`的步骤以非root用户运行并且当试图使用root用户运行step容器时将会失败。
只指定`runAsNonRoot: true`并不会真正以一个非root用户运行容器，只是作为一个简单验证step不以root用户运行的属性。`runAsUser`属性真正的设置了运行容器的非root用户ID。

如果一个步骤定义了自己的`securityContext`, 他将会覆盖通过TaskRun `podTemplate`指定的Pod级别的`securityContext`，并应用给自己。

关于Pod容器安全上下文的更多信息请参考[Kubernetes website](https://kubernetes.io/docs/tasks/configure-pod-container/security-context/#set-the-security-context-for-a-pod).

Task/TaskRun的示例可以在[TaskRun example](../examples/v1beta1/taskruns/run-steps-as-non-root.yaml)找到。

