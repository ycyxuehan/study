## TriggerTemplate

```yaml
apiVersion: triggers.tekton.dev/v1alpha1
kind: TriggerTemplate
metadata:
  name: pipeline-template
spec:
  params:
  - name: gitrevision
    description: The git revision
    default: master
  - name: gitrepositoryurl
    description: The git repository url
  - name: message
    description: The message to print
    default: This is the default message
  - name: contenttype
    description: The Content-Type of the event
  resourcetemplates:
  - apiVersion: tekton.dev/v1beta1
    kind: PipelineRun
    metadata:
      generateName: simple-pipeline-run-
    spec:
      pipelineRef:
        name: simple-pipeline
      params:
      - name: message
        value: $(params.message)
      - name: contenttype
        value: $(params.contenttype)
      resources:
      - name: git-source
        resourceSpec:
          type: git
          params:
          - name: revision
            value: $(params.gitrevision)
          - name: url
            value: $(params.gitrepositoryurl)
```