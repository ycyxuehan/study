IMAGE ?= registry.bing89.com/tools/hpademo:1.0

build:
	docker build -t ${IMAGE} .

push: build
	docker push ${IMAGE}