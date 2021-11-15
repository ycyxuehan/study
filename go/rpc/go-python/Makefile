create-go:
	protoc --go_out=plugins=grpc:. rpcgopython.proto
create-python:
	python3 -m grpc_tools.protoc -I ./ --python_out=pyrpc --grpc_python_out=pyrpc rpcgopython.proto
create: create-go create-python
	echo "rpc service file created"