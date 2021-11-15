from pyrpc import rpcgopython_pb2_grpc
from pyrpc import rpcgopython_pb2
from datetime import datetime
import grpc
from concurrent import futures
class HelloServer(rpcgopython_pb2_grpc.HelloServiceServicer):
    def Hello(self, request, context):
        print("request:", request)
        return rpcgopython_pb2.Result(res=True, error = "", createdAt = 0)

def register():
    channel = grpc.insecure_channel('localhost:8081')
    stub = rpcgopython_pb2_grpc.GatewayServiceStub(channel)
    response = stub.Register(rpcgopython_pb2.User(name="hello", ip="localhost:8082", createdAt=0))
    print("gateway client received: ", response)

def serve():
  server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
  rpcgopython_pb2_grpc.add_HelloServiceServicer_to_server(
      HelloServer(), server)
  server.add_insecure_port('localhost:8082')
  server.start()
  #server started, register
  register()
  server.wait_for_termination()

if __name__ == '__main__':
    serve()