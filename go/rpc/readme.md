[toc]
# gRPC概述
- 在gRPC中，一个客户端的应用可以直接调用在不同机器上的服务端应用的方法，使得创建分布式的应用和服务更为简单
- 围绕：定义服务；指定可以被远程调用的方法、参数和返回类型
- 服务端实现接口，运行一个给RPC服务响应客户端的调用请求
- 客服端有一个stub（看成是client）提供和服务端一样的方法


## Protocol Buffers
- gRPC默认使用Protocol Buffers

步骤：
1. 定义希望在proto文件中进行序列化的数据结构
2. 使用protocol buffer compiler（即protoc）生成特定编程语言的访问类，访问类提供每个field的访问方法，序列化/解析整个数据结构的方法
3. 在proto文件中定义gRPC服务，服务应包含RPC方法参数和返回的类型，参数和类型都是protocol buffer message


# 最佳实践准则
## 通用性准则
- 总是尽可能重用stubs和channel
- 使用keepalive pings保持HTTP/2链接alive
- 在处理长期存活的数据逻辑时，使用streaming RPCs
- 每个给RPC channel使用若干个HTTP/2链接，每个链接都会有最大的链接限制，当遇到最大链接限制时有两个解决方案：1. 为每个高负载的区域创建一个独立的channel；2. 使用gRPC channel pool

## C++
- 不要在性能敏感的服务上使用同步的API
- 对于多少RPCs而言，尽量使用回调API，可以避免阻塞
- 如果使用异步completion-queue API，最佳的扩展经验是创建等同于CPU核数的线程数
- 对于异步completion-queue API，确保为需要的并发等级注册了足够的server请求
- 如果message之间没有前后依赖关系，可以开启write batching in streams。例子```stream_writer->Write(message, WriteOptions().set_buffer_hint());```
- 当在序列化时出现高连接或CPU占有时间，可以采用[此方法](https://grpc.github.io/grpc/cpp/grpcpp_2generic_2generic__stub_8h.html)，允许直接发送原始的gRPC::ByteBuffer而不是序列化后的数据

## Python
- Python中的streaming RCPs会创建额外的线程来发送、接收message，导致streaming RPCs比一元的RPC要慢，这一点于其它语言相反
- 使用asyncio可以提高性能
- 避免使用future API


# C++
## 编译安装
```
apt-get install autoconf automake libtool curl make g++ unzip

# 下载相应的代码后
./configure
make
make check
sudo make install
sudo ldconfig # refresh shared library cache.
```



# Python
## Tutorial

基本内容：
- 在`$.proto$`文件中定义服务
- 使用protocol buffer compiler生成服务端和客户端代码
- 使用Python gRPC API写一个简单的服务端和客户端


### 定义服务
```
service XXX {
  rpc method_name(input_message_name) returns (output_message_name) {
      
  }
}
```
四种方法类型：
- simple RPC：客户端使用stub发送请求到服务端，等待返回响应，就好比一般形式的函数调用
- response-streaming RPC：客户端发送请求到服务端，获得读取一系列message的流
- request-streaming RPC：客户端写一系列message，通过流发送到服务端；写完后等待服务端读取并返回响应；需要在input_message_name前面加上stream关键字
- bidirectionally-streaming RPC：两个端都使用流传输一系列message；两个流是相互独立的；需要在input_message_name和output_message_name前面加上stream关键字


### 创建服务
分为两步：
- 实现服务接口（即继承自动生成的service类并实现相关的方法）
- 运行一个给RPC服务监听客户端请求，以及传输相应


启动服务例子：
```
def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    
    # 根据自动生成的代码进行调整
    route_guide_pb2_grpc.add_RouteGuideServicer_to_server(
        RouteGuideServicer(), server)
    
    server.add_insecure_port('[::]:50051')
    # 相当于启动线程
    server.start()
    # 相当于join()
    server.wait_for_termination()
```

### 创建client
#### 创建stub
- 要调用服务端的方法，需要先创建一个stub
- 具体要实现的类在自动生成的XXX_pb2_grpc模块中找

例子：
```
channel = grpc.insecure_channel('localhost:50051')
# 根据实际代码修改
stub = route_guide_pb2_grpc.RouteGuideStub(channel)
```

#### 调用服务端的方法
- 对于返回单一一个响应的RPC方法，gPRC Python支持同步或异步实现
- 对于response-streaming RPC方法，会立即返回一个关于响应内容的迭代器，可以在上面调用next()方法
- 对于request-streaming RPC方法，传入一个请求内容的迭代器，同步或异步调用均可
- 对于bidirectional streaming RPC方法，组合上述方式即可


例子：
```python
# 同步调用
feature = stub.GetFeature(point)

# 异步调用
feature_future = stub.GetFeature.future(point)
feature = feature_future.result()

# response-streaming RPC调用
for feature in stub.ListFeatures(rectangle):
    pass

# request-streamimg RPC调用
route_summary = stub.RecordRoute(point_iterator)
route_summary_future = stub.RecordRoute.future(point_iterator)
route_summary = route_summary_future.result()

# bidirectional streaming RPC
for received_route_note in stub.RouteChat(sent_route_note_iterator):
    pass


```


## Protocol Buffer
### 定义Protocol Format
- 以`$.proto$`作为文件扩展名
- 每个需要序列化的数据结构都作为message
- 为message中的每一个field指定类型、名字
- field类型包括：bool, int32, float, double, string；当然也可以将自定义的message作为field的类型
- =1之类的标记用于对应field的二进制编码，常用的field应该使用1-15
- Proto2中，每个field必须使用一个修饰符：optional（该field可有可无，无则使用默认值）；repeated（该field重复任意次）；required（该field必须提供，否则message将无法初始化，Proto3不在支持）

```
syntax = "proto2";
# 或者
syntax = "proto3";

# 引入包名，避免命名冲突
package XXX;

# 一个例子
message A {
    optional string name = 1;
    
    enum PhoneType {
        MOBILE = 0;
        HOME = 1;
    }
    
    message PhoneNumber {
        optional string number = 1;
        # proto3的话不支持显式定义默认值
        optional PhoneType type = 2 [default = HOME];
    }
    repeated PhoneNumber phones = 4;
}
```

### 编译Protocol Buffers
- 必须先安装编译器

编译命令原型：
```
protoc -I=$SRC_DIR --python_out=$DST_DIR $SRC_DIR/XXX.proto
```
- $SRC_DIR填`$.proto$`文件所在的目录
- $DST_DIR填生成的Python文件存放目录
- XXX.proto是要编译的`$.proto$`文件名

最终会生成文件：
- XXX_grpc.py
- XXX_pb2.py

### Protocol Buffer API
- 不像Java、C++那样，Python环境下，protocol buffer compiler不会自动生成数据访问代码
- 直接在py文件中import XXX_pb2
- 只能使用在proto文件中定义的field，
- 使用未定义的field会报AttributeError
- 使用的field的Type错误，会报AttributeError


#### 解析和序列化
- SerializeToString()：将message序列化为string
- ParseFromString(data)：将string解析成message

## 生成Python的代码
### Messages
- 当message的名称与Python关键字冲突时，使用setattr()和getattr()进行调用
- 不要继承自动生成的代码

### 常见的Message Type
#### Any
- 对于Any类型的Message，可以使用Pack()来将指定的message打包到Any类型的message中
- 可以使用Unpack()将指定的message从Any类型的message中解包出来
- 可以调用Is()方法检查Any类型的message表示了给定的protocol buffer类型

例子：
```
any_message.Pack(message)
any_message.Unpack(message)
assert any_message.Is(message.DESCRIPTOR)
```

#### Timestamp
- 可以使用ToJsonString()、FromJsonString()实现timestamp和string之间的互转
- 可以使用GetCurrentTime()来填充一个Timestamp message
- 可以使用ToDatetime()和FromDatetime()方法实现Timestamp message和Python的datetime对象之间的互转

例子：
```
timestamp_message.FromJsonString("1970-01-01T00:00:00Z")
assert timestamp_message.ToJsonString() == "1970-01-01T00:00:00Z"
timestamp_message.GetCurrentTime()

dt = datetime(2016, 1, 1)
timestamp_message.FromDatetime(dt)
```

#### Duration
- 有着与Timestamp像似的方法
- 可以使用ToTimedelta()和FromTimedelta实现duration message和Python Timedelta对象之间的互转

#### FieldMask


#### Struct
- struct message可以直接获得或设置对象
- 创建list或struct，可以调用get_or_create_list()、get_or_create_struct()

#### ListValue
- ListValue可看成时Python的序列；可以使用extend()、append()


## Service

如果proto文件中包含：
```
# 默认是 false
option py_generic_services = true;
```
protocol buffer compiler会生成proto文件中service定义对应的代码

### Interface
- 在自动生成代码后，需要在Service的子类中实现具体的方法

自动生成Service代码，包含如下实现了的方法：
- GetDescriptor：返回service的ServiceDescriptor
- CallMethod：基于提供的方法descriptor判断哪个方法将被调用，并直接调用
- GetRequestClass和GetResponseClass：返回给定方法的请求或响应的参数的类

### Stub
- Stub被client用在向实现了具体服务的服务端发送请求
- XXX_Stub（自动生成）是XXX Service的一个子类
- 需要提供RpcChannel和RpcController的实现

# Proto3语法
- [官方文档链接](https://developers.google.com/protocol-buffers/docs/proto3#generating)

## 代码风格说明
- 每行80个字符
- 缩进时采用2个空格
- string采用双引号
- proto文件名采用小写
- message名采用驼峰命名（每个单词首字母大写）
- enum的名字每个单词首字母大写；里面的值全部大写，单词之间采用下划线分割；0对应的value要采用UNSPECIFIED作为后缀
- Service的名字和里面的方法名都采用驼峰命名（每个单词首字母大写）

## 定义Message
- syntax不指定的话默认使用proto2
- syntax必须写在文件中非注释的第一行
- 多个message可以定义在同一个proto文件中
- 注释采用C++的格式
- Protocol Buffer不是设置成处理大的message的，一旦一个message超过1M，就应该考虑拆分

### 给Field赋唯一的number
- 1-15占1个byte
- 16-2047占2个byte
- 19000-19999不能用
- 最大可取2^29 -1

### Field的修饰符
- singular：proto3默认，如果某个field用此修饰，则该field在message中允许有0或1个
- repeated：如果某个field用此修饰，则该field在message中可重复任意次（包括0次），而且保留重复的顺序


### 保留Fields
为了防止在修改message过程中出现field名字或number重复被使用的情况

例子：
```
message XXX {
    reserved 2, 3, 5 to 10;
    reserved "foo";
}
```

如果出现重复使用，protocol buffer compiler会在编译时给出警告

## 不同语言中标量的类型对应关系


.proto Type | C++ Type | Python Type | Go Type | 备注
---|---|---|---|---
double | double | float | float64
float | float | float | float32
int32 | int32 | int | int32 | 使用变长的编码；对负整数低效；负整数使用sint32
int64 | int64 | int/long | int64 | 使用变长的编码；对负整数低效；负整数使用sint64
uint32 | uint32 | int/long | uint32 | 使用变长的编码
sint32 | int32 | int | int32 | 使用变长的编码
bool | bool | bool | bool |
string | string | str/unicode | string | 编码成UTF-8

## 默认值
- string：空字符串
- bytes：空bytes
- bool：false
- 数值类型：0
- enums：第一个enum值（number为0）
- 对于message类型的field，默认值取决于具体的编程语言
- repeated fields：空

## 枚举
- 枚举类型的field中，必须有一个值的number为0，且必须是第一个
- 


## 使用Message作为Type
- 如果Message在其他的proto文件，则需要import "xxx.proto";(引号不可少)
- proto2和3之间可以相互import

## 嵌套类型
- 父message中可以嵌套定义子message
- 在另一个message中要使用父message中的子message类型，可以使用.操作符

例子：
```
message A {
    message B {
        # 省略
    }
}

message C {
    A.B b = 1;
}
```
## 更新Message
- 当旧的Message不能满足需求，但需要同时使用新旧两套Message代码时

记住如下规则：
- 不要改变旧Message中的field的number
- 要留意新添加的field的默认值，保证新旧代码之间可以兼容
- 可以移除field，一旦确保某个field不再使用，可以将对应的number作为reserved，防止新添加的field误用旧的number
- int32, uint32, int64, uint64, bool之间可以相互兼容
- sint32和sint64兼容，但不能兼容其他的整数类型
- 只要bytes是有效的UTF-8格式，bytes和string可以兼容
- 只要bytes包含了内嵌message的一个已编码的版本，则可以和该内嵌message兼容
- fixed32和sfixed32兼容；fixed64和sfixed64兼容
- 对于string, bytes和message类型的field，optional和repeated兼容，但是注意解析时的取值
- enum和int32，uint32，int64，uint64兼容

## 未知的Fields
- proto3的message通常在解析阶段丢弃未知的field
- 但在3.5及以后的版本，未知的field会在解析和序列化输出时被保留

## Any
- 可以在没有proto文件原型的前提下，使用message作为内嵌的type
- 可以看成是C++中的模板参数<T>
- 必须import "google/protobuf/any.proto";

例子：
```
import "google/protobuf/any.proto";

message ErrorStatus {
  string message = 1;
  repeated google.protobuf.Any details = 2;
}
```

## Oneof
- 如果有一个message包含了许多的fields，但是一次最多只set一个field，（可理解为一次最多只使用一个field）此时就可以使用oneof来确保这种行为并节约内存
- oneof的field会共享一块内存
- 根据不同的编程语言，可以使用case()或者WhichOneof()方法来检查oneof中设置了什么值
- 除了map fields和repeated fields，其他都能加入到oneof
- Reflection API适用oneof的fields
- C++在使用具有oneof fields的message时要注意判断，防止内存泄漏

例子：
```
message SampleMessage {
  oneof test_oneof {
    string name = 4;
    SubMessage sub_message = 9;
  }
}
```
## Maps

语法：
```
map<key_type, value_type> map_field = N;
```
- N是该field对应的number
- key_type可以是任意的整数或string
- key_type不能是浮点数、bytes、enum
- value_type可以是任意的类型，除了map；即不允许嵌套定义map
- Map field不能是repeated
- 迭代Map是无序的
- 当合并Map存在key重复时，以最后一个key的值为准；当从文本格式中解析map时，如果有重复的key，解析将失败
- 如果提供了key，但没有提供对应的value，该field在序列化时的行为将由对应的编程语言决定

## Packages
- 包名用于解决命名冲突
- 包名对生成的代码有何种影响取决于具体的编程语言
- C++ 生成的类会被包装到一个C++ 的命名空间中
- Python会忽略包名，因为Python的模块是由它们在文件系统中的位置组织起来的
- GO中，包名会被作为Go的包名，除非在proto文件中显式提供一个option go_package

## 定义Services
- 要在RPC中使用Message类型，需要在proto文件中定义一个RPC服务，然后protocol buffer compiler将生成相应编程语言的接口代码和stubs

格式：
```
service service_name {
    rpc method_name(input_message_name) returns (output_message_name);
}
```

## JSON映射
- Proto3支持编码成JSON对象
- 在JSON编码时，如果缺少某个value或者该value是null，则在解码成protocol buffer时，该value会解释成一个合适的默认值



proto3 | JSON | JSON Example | 备注
---|---|---|---
message | object | | field名会全部转成小写
enum | string |
map<K, V> | object | |所有的key都转成string
repeated V | array | [v, ...] | 
bool | true, false | true, false|
string | string |
bytes | base64 string |
int32, fixed32, uint32 | number |
int64, fixed64, uint64 | string |
float, double | number |
Any | object |
Timestamp | string |
Duration | string |
Struct | object |
Wrapper types | 多种类型都有可能
FieldMask | string |
ListValue | array |
Value | value
NullValue | null
Empty | object | {}

Proto3的JSON实现提供了以下选项：
- 将field的默认值传递给JSON（这是默认选项）
- 忽略未知的field
- 使用proto内真实的field名字，而不是小写的field名字
- 传递enum时将其作为整数而不是string

## Options
- 全部可选项都定义在google/protobuf/descriptor.proto文件中
- 定义option的一般格式：
```
option option_name = value;
```

常用的几个可选项：
- optimize_for: SPEED(默认选项，生成高度优化过的代码), CODE_SIZE（生成尽可能小的类，常用于APP包含大量的proto文件时）, LITE_RUNTIME（生成依赖于libprotobuf-lite库的类，常用于受到平台约束的APP，如在手机上进行开发）
- cc_enable_arenas：在生成C++代码时启用arena allocation
- deprecated: 设置为true则表明该field将被弃用，不应该再使用。例子：
```
int32 old_field = 6 [deprecated = true];
```

## 生成对应的类
生成C++，Python的代码，需要以下几步：
1. 在proto文件中定义Message
2. 运行protocol buffer compiler对proto文件进行编译
3. 对于Go，还需要另外安装一个编译器插件，[链接](https://github.com/golang/protobuf/)

完整的编译命令如下：
```
protoc --proto_path=IMPORT_PATH --cpp_out=DST_DIR --java_out=DST_DIR --python_out=DST_DIR --go_out=DST_DIR --ruby_out=DST_DIR --objc_out=DST_DIR --csharp_out=DST_DIR path/to/file.proto
```
- 可以同时指定一个或多个XXX_out路径，即可以同时生成多种语言的代码



