<!--
---
title: "Tasks and Pipelines"
linkTitle: "Tasks and Pipelines"
weight: 2
description: >
  Building Blocks of Tekton CI/CD Workflow
cascade:
  github_project_repo: https://github.com/tektoncd/pipeline
---
-->
# Tekton Pipelines

Tekton Pipelines 是一个Kubernetes扩展。它定义用于组装CI/CD流水线各个组成部分的Kubernetes[自定义资源](https://kubernetes.io/docs/concepts/extend-kubernetes/api-extension/custom-resources/)集。只需一次安装， Tekton Pipelines 就可以像Pod或其他资源一样被Kubernetes CLI(kubectl)或API调用。Tekton是一个开源项目。

## Tekton Pipelines 独立资源

Tekton Pipelines定义下列独立的资源：

资源|说明
--|--
Task |定义启动读取特定输入并产生特定输出的编译或交付工具的一系列步骤。
TaskRun|为包含特定输入输出和执行器选项的执行器实例化一个 Task 。可以独立运行也可以作为Pipeline的一部分。
Pipeline|定义一系列任务来完成一个特定的编译或交付目标。可以被event出发或PipelineRun引用。
PipelineRun|为包含特定输入输出和执行器选项的执行器实例化一个Pipeline。
PipelineResource| 为Task的steps要读取的输入和产生的输出定义位置

## 入门

入门需要完成[Tekton Pipelines Tutorial](https://github.com/tektoncd/pipeline/blob/master/docs/tutorial.md)并跑通[示例](https://github.com/tektoncd/pipeline/tree/master/examples).

## 理解Tekton Pipelines

阅读下列主题来学习如何在你的项目中使用Tekton Pipelines:

- [创建Task](tasks.md)
- [运行独立的Task](taskruns.md)
- [创建Pipeline](pipelines.md)
- [运行Pipeline](pipelineruns.md)
- [定义Workspaces](workspaces.md)
- [创建PipelineResources](resources.md)
- [配置认证](auth.md)
- [使用标签](labels.md)
- [查看日志](logs.md)
- [流水线指标](metrics.md)
- [变量替换](variables.md)

## Tekton Pipelines 进阶

如果希望对Tekton项目有更深入的了解，请阅读[Tekton Pipeline 进阶向导](https://github.com/tektoncd/pipeline/blob/master/CONTRIBUTING.md)。
