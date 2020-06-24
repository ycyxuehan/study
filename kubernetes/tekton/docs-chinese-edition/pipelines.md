<!--
---
linkTitle: "Pipelines"
weight: 3
---
-->
# Pipelines

- [概览](#pipelines)
- [配置`Pipeline`](#configuring-a-pipeline)
  - [指定 `Resources`](#specifying-resources)
  - [指定 `Workspaces`](#specifying-workspaces)
  - [指定 `Parameters`](#specifying-parameters)
  - [为`Pipeline`添加`Tasks`](#adding-tasks-to-the-pipeline)
    - [使用 `from` 参数](#using-the-from-parameter)
    - [使用 `runAfter` 参数](#using-the-runafter-parameter)
    - [使用 `retries` 参数](#using-the-retries-parameter)
    - [使用`Conditions`守护`Task`执行](#guard-task-execution-using-conditions)
    - [配置失败超时](#configuring-the-failure-timeout)
    - [配置`Task`级别的执行结果](#configuring-execution-results-at-the-task-level)
  - [配置`Pipeline`级别执行结果](#configuring-execution-results-at-the-pipeline-level)
  - [配置`Task`执行顺序](#configuring-the-task-execution-order)
  - [添加一个说明](#adding-a-description)
  - [添加 `Finally` 到 `Pipeline` (预览)](#adding-finally-to-the-pipeline-preview)
  - [代码示例](#code-examples)

## 概览

`Pipeline`是持续集成工作流中定义并有序排列的`Task`的集合。`Pipeline`中的每一个`Task`在Kubernetes集群中以`Pod`方式执行。可以依据业务需要配置各种执行条件。

## 配置 `Pipeline`

`Pipeline` 定义支持以下字段:

- 必选:
  - [`apiVersion`][kubernetes-overview] - 指定API版本，例如：`tekton.dev/v1beta1`.
  - [`kind`][kubernetes-overview] - 声明资源是`Pipeline`对象.
  - [`metadata`][kubernetes-overview] - 指定说明`Pipeline`对象的唯一身份元数据,比如`name`.
  - [`spec`][kubernetes-overview] - 指定`Pipeline`的配置，必须包括
    - [`tasks`](#adding-tasks-to-the-pipeline) - 指定`Pipeline`包括的 `Tasks` 以及它们的执行详情。
- 可选:
  - [`resources`](#specifying-resources) - **仅alpha** 指定`Pipeline`需要为其包含的`Tasks`提供或创建的[`PipelineResources`](resources.md)
  - [`tasks`](#adding-tasks-to-the-pipeline):
      - `resources.inputs` / `resource.outputs`
        - [`from`](#using-the-from-parameter) - 表明 [`PipelineResource`](resources.md)的数据来源自前一个 `Task` 的输出。
      - [`runAfter`](#using-the-runafter-parameter) - 表明`Task`必须在一个或多个没有输出链接的其他`Task`之后运行。
      - [`retries`](#using-the-retries-parameter) - 指定`Task`失败后的重试次数，不适用于取消执行的`Task`
      - [`conditions`](#guard-task-execution-using-conditions) - 指定仅允许符合条件的`Task`执行的`Conditions`。
      - [`timeout`](#configuring-the-failure-timeout) - 指定`Task`失败前的超时时间
  - [`results`](#configuring-execution-results-at-the-pipeline-level) - 指定`Pipeline`发送执行结果的位置。
  - [`description`](#adding-a-description) - 保存“Pipeline”对象的描述信息。

[kubernetes-overview]:
  https://kubernetes.io/docs/concepts/overview/working-with-objects/kubernetes-objects/#required-fields

## 指定 `Resources`
`Pipeline`为其包含的`Task`请求提供输入资源与保存输出资源的 [`PipelineResources`](resources.md)。可以在 `Pipeline` 定义的 `spec` 部分的`resources`字段声明它们。每一个资源请求唯一的`name`与`type`。例如:

```yaml
spec:
  resources:
    - name: my-repo
      type: git
    - name: my-image
      type: image
```

## 指定 `Workspaces`

`Workspaces`允许在运行时指定`Pipeline`中的每一个`Task`请求的一个或多个卷。在`workspaces`字段指定一个或多个`Workspaces`，例如：

```yaml
spec:
  workspaces:
    - name: pipeline-ws1 # The name of the workspace in the Pipeline
  tasks:
    - name: use-ws-from-pipeline
      taskRef:
        name: gen-code # gen-code expects a workspace with name "output"
      workspaces:
        - name: output
          workspace: pipeline-ws1
    - name: use-ws-again
      taskRef:
        name: commit # commit expects a workspace with name "src"
      runAfter:
        - use-ws-from-pipeline # important: use-ws-from-pipeline writes to the workspace first
      workspaces:
        - name: src
          workspace: pipeline-ws1
```

更多信息，参考
- [`Pipelines` 中使用 `Workspaces`](workspaces.md#using-workspaces-in-pipelines)
- [`PipelineRun`中的`Workspaces`示例代码 ](../examples/v1beta1/pipelineruns/workspaces.yaml)

## 指定 `Parameters`

可以指定在运行时希望提供给`Pipeline`的全局变量，比如编译标志或项目名称。`Parameters`被`PipelineRun`传给其相应的`Pipeline` 并且可以被`Pipeline`里的每一个`Task`指定的模板值替换。

参数名称:

- 只能由字母、数字, 短线 (`-`)和下划线 (`_`)组成.
- 必须以字母或下划线 (`_`)开头.

例如, `fooIs-Bar_`是合法的参数名, `barIsBa$`或`0banana`则是非法参数名.

每一个声明的参数都有一个可以被设置为`array`或`string`的 `type`字段. 当`task`的整个执行过程中，为任务提供的构建标志的数量各不相同时，`array`非常有用。`type`字段在不指定时，默认设置为`string`。 实参的的值被提供时，它的解析类型会再次和`type`字段验证。

下面例子展示了在`Pipeline`中使用 `Parameters`。 

下面的`Pipeline`声明了一个叫 `context`的输入参数并传递其值给 `Task` 以设定 `Task`的`pathToContext`参数的值。
如果为`default`字段指定了一个值并且在`PipelineRun`调用这个`Pipeline`时没有为 `context` 指定值，将使用这个默认值。

**注意:** 输入参数值可以作为变量在整个`Pipeline`中以[变量替换](variables.md#variables-available-in-a-pipeline)方式使用。

```yaml
apiVersion: tekton.dev/v1beta1
kind: Pipeline
metadata:
  name: pipeline-with-parameters
spec:
  params:
    - name: context
      type: string
      description: Path to context
      default: /some/where/or/other
  tasks:
    - name: build-skaffold-web
      taskRef:
        name: build-push
      params:
        - name: pathToDockerFile
          value: Dockerfile
        - name: pathToContext
          value: "$(params.context)"
```

下面的 `PipelineRun` 为 `context` 提供了一个值:

```yaml
apiVersion: tekton.dev/v1beta1
kind: PipelineRun
metadata:
  name: pipelinerun-with-parameters
spec:
  pipelineRef:
    name: pipeline-with-parameters
  params:
    - name: "context"
      value: "/workspace/examples/microservices/leeroy-web"
```

## 为`Pipeline`添加 `Tasks`

`Pipeline` 定义至少引用一个 [`Task`](tasks.md)。`Pipeline`中的每一个 `Tasks`必须有一个[有效的](https://kubernetes.io/docs/concepts/overview/working-with-objects/names/#names)`name` 与 `taskRef`.例如:

```yaml
tasks:
  - name: build-the-image
    taskRef:
      name: build-push
```

可以使用 [`PipelineResources`](#specifying-resources) 作为`Pipeline`中的 `Tasks` 的输入输出资源，例如:

```yaml
spec:
  tasks:
    - name: build-the-image
      taskRef:
        name: build-push
      resources:
        inputs:
          - name: workspace
            resource: my-repo
        outputs:
          - name: image
            resource: my-image
```

也可以提供 [`Parameters`](tasks.md#specifying-parameters):

```yaml
spec:
  tasks:
    - name: build-skaffold-web
      taskRef:
        name: build-push
      params:
        - name: pathToDockerFile
          value: Dockerfile
        - name: pathToContext
          value: /workspace/examples/microservices/leeroy-web
```

### 使用 `from` 参数
如果`Pipeline`的一个`Task`需要使用之前的`Task`的输出作为它的输入，使用可选参数`from`来指定一个必须在这个`Task`之前运行完成的`Tasks`列表。当目标`Task`执行时，仅列表中最后一个`Task`产生的期望版本的`PipelineResource`会被使用。输出`PipelineReource`的`name`必须与`Task`中指定的输入`PipelineReource`的`name`匹配，才会被使用。

下面的例子中,  `deploy-app`任务使用`build-app`任务名为`my-image`的输出作为输入。 因此, 无论这些`Tasks`在 `Pipeline`中定义的顺序如何，`build-app`将会在`deploy-app`之前运行：

```yaml
- name: build-app
  taskRef:
    name: build-push
  resources:
    outputs:
      - name: image
        resource: my-image
- name: deploy-app
  taskRef:
    name: deploy-kubectl
  resources:
    inputs:
      - name: image
        resource: my-image
        from:
          - build-app
```

### 使用 `runAfter` 参数

如果需要`Pipeline`中的`Tasks`以一个特定的顺序运行，但它们又没有依赖资源要请求`from`参数，使用`runAfter` 来表明一个 `Task` 必须在一个或多个 `Tasks` 之后运行。

下面的例子中, 我们将在构建代码之前测试它。由于`test-app`没有输出，`build-app`使用 `runAfter`表明其必须在`test-app`之后运行，而不去管它们在`Pipeline`定义中的先后顺序。

```yaml
- name: test-app
  taskRef:
    name: make-test
  resources:
    inputs:
      - name: workspace
        resource: my-repo
- name: build-app
  taskRef:
    name: kaniko-build
  runAfter:
    - test-app
  resources:
    inputs:
      - name: workspace
        resource: my-repo
```

### 使用 `retries` 参数

对于`Pipeline`中的每一个`Task`，都可以指定它们失败后的重试次数。当一个`Task`失败了，对应的`TaskRun`设置它的 `Succeeded` `Condition` 为 `False` 。`retries`参数指示Tekton在`Task`执行失败时的重试次数。如果可以预期`Task`执行期间遇到的问题(比如，已知当网络连接出错或缺少依赖项时会发生错误)，设置他的`retries`为一个大于0的合适值。如果没有显式的设置一个值，Tekton不会对失败的`Task`做任何重试。

下面的例子中, `build-the-image`会在失败后重试一次，如果重试也失败，则认为任务执行失败。

```yaml
tasks:
  - name: build-the-image
    retries: 1
    taskRef:
      name: build-push
```

### 使用`Conditions`守护 `Task` 执行
仅当满足一些条件时运行`Task`，可以使用`conditions`字段。 `conditions` 字段允许列出一系列的[`Condition`](./conditions.md)资源引用。声明的`Conditions`会在 `Task`运行之前运行。
如果所有的条件都成功通过，`Task`就会运行。任何一个条件失败，`Task`不会运行并且`TaskRun`的状态字段`ConditionSucceeded`会设置为`False`，同时失败原因会设置到`ConditionCheckFailed`字段。

这个例子中, `is-master-branch` 引用一个 [Condition](conditions.md) 资源.  `deploy`任务仅在条件成功通过时运行。


```yaml
tasks:
  - name: deploy-if-branch-is-master
    conditions:
      - conditionRef: is-master-branch
        params:
          - name: branch-name
            value: my-value
    taskRef:
      name: deploy
```

与常规任务失败不同，条件失败不会自动导致`PipelineRun`整个失败，其他不依赖(通过`from` 或`runAfter`)这个`Task`的`Task`仍然会运行。

在这个例子中, `(task C)` 有一个守护其运行的 `condition`. 如果条件没有成功通过， 任务 `(task D)`不会运行, 但是pipeline中不依赖`(task C)`的任务仍会运行且`PipelineRun`会成功完成。

```
        (task B) — (task E)
    / 
(task A) 
    \
        (guarded task C) — (task D)
```

条件资源也可以使用 [`from`](#using-the-from-parameter) 字段说明它期望使用前一个任务的输出作为输入。与常规操作流任务一样，使用`from`表明执行顺序--如果一个任务有一个使用其他任务输出资源作为条件，那么产出结果的任务最先运行:

```yaml
tasks:
  - name: first-create-file
    taskRef:
      name: create-file
    resources:
      outputs:
        - name: workspace
          resource: source-repo
  - name: then-check
    conditions:
      - conditionRef: "file-exists"
        resources:
          - name: workspace
            resource: source-repo
            from: [first-create-file]
    taskRef:
      name: echo-hello
```

### 配置失败超时

可以使用在`Pipeline`的`Task`配置中的`Timeout`字段配置运行`Pipeline`的`PipelineRun`中的运行`Task`的`TaskRun`的超时时间。`Timeout`是一个遵从Go语言[`ParseDuration`](https://golang.org/pkg/time/#ParseDuration)格式的`duration`。例如：`1h30m`, `1h`, `1m`, 和 `60s` 都是合法的值。

**注意:** 如果没有指定`Timeout` 值, Tekton尊重[`PipelineRun`](pipelineruns.md#configuring-a-pipelinerun)的超时时间。

下面的例子中, `build-the-image` `Task` 配置了90秒的超时时间:

```yaml
spec:
  tasks:
    - name: build-the-image
      taskRef:
        name: build-push
      Timeout: "0h1m30s"
```

### 配置`Task`级别的执行结果

任务在执行时可以发出他们的[`Results`](tasks.md#storing-execution-results)。可以在`Pipeline`中随后的`Tasks`里通过[变量替换](variables.md#variables-available-in-a-pipeline)以参数值的方式使用这些`Results`。Tekton会推断任务顺序，因此`Task`会在 `Results` 被消费之前执行并发出。

下面的例子中, `previous-task-name` `Task` 的结果声明为 `bar-result`:

```yaml
params:
  - name: foo
    value: "$(tasks.previous-task-name.results.bar-result)"
```

了解端对端示例, 参考 [`Task` `Results` in a `PipelineRun`](../examples/v1beta1/pipelineruns/task_results_example.yaml).

## 配置 `Pipeline` 级别执行结果

可以配置`Pipeline`在执行期间发出作为其包含的`Task`发出结果的引用的`Results`。

下面的例子中, `Pipeline`指定一个名为`sum`的`results`入口，`sum` 引用`second-add` `Task`发出的结果。

```yaml
  results:
    - name: sum
      description: the sum of all three operands
      value: $(tasks.second-add.results.sum)
```

了解端对端示例, 参考 [`Results` in a `PipelineRun`](../examples/v1beta1/pipelineruns/pipelinerun-results.yaml).

## 配置 `Task` 执行顺序

可以在`Pipeline`中连接`Tasks`，因此他们在一个有向无环图中执行。`Pipeline`中的每一个任务都会成为图中一个可以被边缘连接的节点，因此即便是某一个`Task`在另一个之前执行，它们也不会陷入无限循环。

这是因为使用下面配置:

- [`from`](#using-the-from-parameter) clauses on the [`PipelineResources`](resources.md) used by each `Task`
- [`runAfter`](#using-the-runafter-parameter) clauses on the corresponding `Tasks`
- By linking the [`results`](#configuring-execution-results-at-the-pipeline-level) of one `Task` to the params of another

示例 `Pipeline` 定义：

```yaml
- name: lint-repo
  taskRef:
    name: pylint
  resources:
    inputs:
      - name: workspace
        resource: my-repo
- name: test-app
  taskRef:
    name: make-test
  resources:
    inputs:
      - name: workspace
        resource: my-repo
- name: build-app
  taskRef:
    name: kaniko-build-app
  runAfter:
    - test-app
  resources:
    inputs:
      - name: workspace
        resource: my-repo
    outputs:
      - name: image
        resource: my-app-image
- name: build-frontend
  taskRef:
    name: kaniko-build-frontend
  runAfter:
    - test-app
  resources:
    inputs:
      - name: workspace
        resource: my-repo
    outputs:
      - name: image
        resource: my-frontend-image
- name: deploy-all
  taskRef:
    name: deploy-kubectl
  resources:
    inputs:
      - name: my-app-image
        resource: my-app-image
        from:
          - build-app
      - name: my-frontend-image
        resource: my-frontend-image
        from:
          - build-frontend
```

依据下图来执行:

```none
        |            |
        v            v
     test-app    lint-repo
    /        \
   v          v
build-app  build-frontend
   \          /
    v        v
    deploy-all
```

特别情况 :

1. The `lint-repo` and `test-app` `Tasks` have no `from` or `runAfter` clauses
   and start executing simultaneously.
2. Once `test-app` completes, both `build-app` and `build-frontend` start
   executing simultaneously since they both `runAfter` the `test-app` `Task`.
3. The `deploy-all` `Task` executes once both `build-app` and `build-frontend`
   complete, since it ingests `PipelineResources` from both.
4. The entire `Pipeline` completes execution once both `lint-repo` and `deploy-all`
   complete execution.

## Adding a description

The `description` field is an optional field and can be used to provide description of the `Pipeline`.

## Adding `Finally` to the `Pipeline` (Preview)

_Finally type is available in the `Pipeline` but functionality is in progress. Final tasks are can be specified and
are validated but not executed yet._

You can specify a list of one or more final tasks under `finally` section. Final tasks are guaranteed to be executed
in parallel after all `PipelineTasks` under `tasks` have completed regardless of success or error. Final tasks are very
similar to `PipelineTasks` under `tasks` section and follow the same syntax. Each final task must have a
[valid](https://kubernetes.io/docs/concepts/overview/working-with-objects/names/#names) `name` and a [taskRef or
taskSpec](taskruns.md#specifying-the-target-task). For example:

```yaml
spec:
  tasks:
    - name: tests
      taskRef:
        Name: integration-test
  finally:
    - name: cleanup-test
      taskRef:
        Name: cleanup
```

_[PR #2661](https://github.com/tektoncd/pipeline/pull/2661) is implementing this new functionality by adding support to enable
final tasks along with workspaces and parameters. `PipelineRun` status is being updated to include execution status of
final tasks i.e. `PipelineRun` status is set to success or failure depending on execution of `PipelineTasks`, this status
remains same when all final tasks finishes successfully but is set to failure if any of the final tasks fail._

## Code examples

For a better understanding of `Pipelines`, study [our code examples](https://github.com/tektoncd/pipeline/tree/master/examples).

---

Except as otherwise noted, the content of this page is licensed under the
[Creative Commons Attribution 4.0 License](https://creativecommons.org/licenses/by/4.0/),
and code samples are licensed under the
[Apache 2.0 License](https://www.apache.org/licenses/LICENSE-2.0).
