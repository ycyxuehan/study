FROM golang:1.16 as builder

WORKDIR /workspace
ENV GOPROXY=https://goproxy.cn,direct GO111MODULE=on
# Copy the Go Modules manifests
COPY go.mod go.mod
COPY go.sum go.sum
# cache deps before building and copying source so that we don't need to re-download as much
# and so that source changes don't invalidate our downloaded layer
RUN go mod download

# Copy the go source
COPY main.go main.go

# Build
RUN CGO_ENABLED=0 GOOS=linux GOARCH=amd64 GO111MODULE=on go build -a -o main main.go

# Use distroless as minimal base image to package the manager binary
# Refer to https://github.com/GoogleContainerTools/distroless for more details
FROM registry.bing89.com/google_containers/static:nonroot-amd64
WORKDIR /
COPY --from=builder /workspace/main .
USER nonroot:nonroot

ENTRYPOINT ["/main"]
