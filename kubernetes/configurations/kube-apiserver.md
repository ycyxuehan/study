# kube-apiserver 配置详情

kube-apiserver 配置参数详解

## 通用参数


|参数名称和类型|使用说明|
|--|--|
|--alsologtostderr|设置为true表示将日志输出到文件的同时输出到 stderr
|-h,-help|查看参数列表的帮助信息
|-log-backtrace-at traceLocation|记录日志每到“file:行号”时打印一次stack trace,默认值为0
-log-dir string|日志文件路径
--log-file string|设置日志文件名
--log-flush-frequency duration|设置 flush日志文件的时间间隔,默认值为5s
--logtostderr|设置为true表示将日志输出到 stderr,不输出到日志文件
-skip-headers|设置为true表示在日志信息中不显示 header信息
--stderrthreshold severity|将该 threshold级别之上的日志输出到 stderr,默认值为2
-V,--v Level|设置日志级别
-version version[=true]|设置为true表示显示版本信息然后退出
--vmodule moduleSpec|设置基于glog模块的详细日志级别,格式为 pattem=n,以逗号分隔
通用参数|
--advertise-address ip|用于广播自己的IP地址给集群的所有成员,在不指定该地址时将使用-bid--address定义的IP地址
cloud-provider-cidrs-gceb-sc-cidrs|GC防火墙上开放的负载均衡器源CIDR列表,默认值为130.2110.0/22、209.85.152.0/22、209.85.204.022、35.191.0.0/16
-cors-allowed-origins strings|CORS|跨域资源共享设置允许访问的源域列表,以逗号分隔,并可使用正则表达式匹配子网。如果不指定,则表示不启用CORS
default-ready-toleration-secondsot-- int|等待notReadyNoExecute的toleration秒数,默认值为300默认会给所有未设置 toleration的pod添加该设置
-unreachable-toleration-seconds-- int|等待unreachableNoExecute的toleration秒数默认值为300默认会给所有未设置 toleration的od添加该设置
--extemal-hostname string|用于生成该 Master的对外URL地址,例如用于 swagger api文档中的URL地址
--feature-gates mapStringBool|用于实验性质的特性开关组,每个开关以 key value形式表示
mx--mutating-requests-inflightint|同时处理的最大突变请求数量,默认值为200,超过该数量的请求将被拒绝。设置为0表示无限制
--max-requests-inflight int|同时处理的最大请求数量,默认值为400,超过该数量的请求将被拒绝设置为0表示无限制
--min-request-timeout int|最小请求处理超时时间,单位为s,默认值为1800s,目前仅用于 watch requesthandler其将会在该时间值上加一个随机时间作为请求的超时时间
--request-timeout duration|请求处理超时时间,可以被min--request-timeout-参数覆盖,默认值为ms
--target-ram-mb int|API Server的内存限制,单位为MB,常用于设置缓存大小
etcd相关参数|
--default-watch-cache-size int|设置默认 watch缓存的大小,设置为0表示不缓存,默认值为100
-delete-collection-workers int|启动 DeleteCollection工作线程数,用于提高清理 Namespace的效率,默认值为1
--enable-garbage-collector|设置为true表示启用垃圾回收器必须与 kube-controller-mana的该参数设置为相同的值,默认值为true
-encryption-provider-config string|在ctcd中存储机密信息的配置文件
--eted-cafile string|到ctcd安全连接使用的 SSL CA文件
-etcd-certfile string|到tcd安全连接使用的SSL证书文件
-eted-compaction-interval duration|压缩请求的时间间隔,设置为0表示不压缩,默认值为5m0s
etcd-count-metri-poll-periodduration|按类型查询tcd中资源数量的时间频率,默认值为ms
--etcd-keyfile string|到etcd安全连接使用的 SSL key文件
-etcd-prefix string|在etcd中保存 Kubemetes集群数据的根目录名,默认值为registry
-etcd-servers strings|以逗号分隔的ctcd服务URL列表,tcd服务以tp etedurl eted http:/ip: port格式表示
--etcd-servers-overrides|按资源覆盖etcd服务的设置,以逗号分隔。单个覆盖格式为:oup p/resource#servers,servershttp://ip: port,其servers格htpport,以分号分隔
--storage-backend string|设置持久化存储类型,可选项为etcd2、ctd3,从 Kuberetes1.6版本开始默认值为ctcd3
--storage-media-type|持久化存储后端的介质类型。某些资源类型只能使用特定类型的介质进行保存,将忽略这个参数的设置,默认值为 application/vndkubemetesprotobuf
-watch-cache|设置为true表示缓存 watch,默认值为true
--watch-cache-sizes strings|设置各资源对象 watch缓存大小的列表,以逗号分隔每个资源对象的设置格式为 resourcesize, watch-cache-被设置为true时生效
安全服务相关参数|
-bind-address ip|Kubemetes API Server在本地址的6443端口开启 HTT安全的服务,默认值为0.0.0.0
-cert-dir string|tLs证书所在的目录,默认值为varrun/kuberetes如果设置r-scert-ile和tls-private-key-file,该设置将被忽略,默认值为/var/u/kubemetes
--http2-max-streams-per-connectionintcocoin|HTTP/(stream),服务器为客户端提供的HTTP2连接中最大流(tream)数量限制,设置为0表示使用 Golang程序的默认值
--secure-port int|设置  api server  HTTPS , 使用的HTTPS安全模式端口号,设置为0表示不启用hTTPS,默认值为6443
-tls-cert-file string|包含x509证书的文件路径,用于 x509 HTTPS认证
--tls-cipher-suites strings|服务器端加密算法列表,以逗号分隔,若不进行设置,则使用Gociphersuites的默认列表。
-tls-min-version string|设置支持的最小- TLS_版本号,可选的版本号包括: Version- TLS_10、 Version- TLS_I1 Version- TLS_12
-tls-private-key-file string|包含x509证书与 tls-cert-f-fle对应的私钥文件路径
|x509证书与私钥文件路径对,如果有多对设置,则需要指定多tss-sni-cert-k-ke参数,默认值为。常用配置示例如“ example.kyexample.c或 "foo.crt,foo.key. *. foo.com, foo.com"
不安全服务相关参数|
-insecure-bind-address ip|绑定的不安全P地址,与insecure--pot共同使用,认值为 localhost设置为0.0.0.0表示使用全部网络接口,默认值为127.0.0.1
--insecure-port in|提供非安全认证访问的监听端口,默认值为808应在防火墙中进行配置,以使外部客户端不可以通过非安全端口访问 API Server
审计相关参数|
--audit-dynamic-configuration|设置启用动态审计配置,要求启用 DynamicAuditing feature开关
lit-log-batch-buffer-size int|审计日志持久化Event的缓存大小,仅用于批量模式,默认值为10000
g-batch-max-size int|审计日志最大批量大小,仅用于批量模式,默认值为1
audit-log-batch-max-waitduration|审计日志持久化Event的最长等待时间,仅用于批量模式
--audit-log-batch-throttle-burst int|审计日志批量处理允许的并发最大数量,仅当之前没有启用过ThrottleQPS时生效,仅用于批量模式
--audit-log-batch-throttle-enable|设置是否启用批处理并发处理,仅用于批量模式
audtl-batch-throttle-qpsfloat322|设置每秒处理批次的最大值,仅用于批量模式
--audit-log-format string|审计日志的记录格式,设置为 legacy表示按每行文本方式记录日志:设置为json表示使用JSON格式进行记录,默认值为json
--audit-log-maxage int|审计日志文件保留的最长天数
--audit-log-maxbackup int|审计日志文件的个数
--audit-log-maxsize int|审计日志文件的单个大小限制,单位MB,默认值为100MB
-audit-log-mode string|审计日志记录模式,包括同步模式 blocking或 blocking-strict-异步模式 batch默认值为 blocking
--audit-log-path string|审计日志文件的全路径
--audit-log-truncate-enabled|设置是否启用记录 Event分批截断机制
--audit-log--truncate-max-batch-size-int|设置每批次最大可保存Event的字节数,超过时自动分成新的批次,默认值为10485760
audit-log-truncate-max-event-sizeint|设置可保存Event的最大字节数,超过时自动移除第1个请求和应答,仍然超限时将丢弃该 Event,默认值为102400
--audit-log-version string|审计日志的API版本号,默认值为 auditk8s.io/vl
-audit-policy-file string|审计策略配置文件的全路径
audit-webhook-batch-buffer-size-int|当使用Webhook保存审计日志时,审计日志持久化Event的缓存大小,仅用于批量模式,默认值为1000
audit-webhook-batch-max-sizeint|当使用Webhook保存审计日志时,审计日志的最大批量大小,仅用于批量模式,默认值为40
-audit-webhook-batch-max-wait duration|当使用Webhook保存审计日志时,审计日志持久化Event的最长等待时间,仅用于批量模式,默认值为30
audit-w-batch-throttle-burst-wboint|当使用Webhook保存审计日志时,审计日志批量处理允许的并发最大数量,仅当之前没有启用过 ThrottleQPS时生效,仅用于批量模式,默认值为15
dit-webhook-batch-throttle-enable-|当使用Webhook保存审计日志时,设置是否启用批处理并发处理,仅用于批量模式,默认值为true
adi-ho-batch-throttle-aps float32|当使用Webhook保存审计日志时,设置每秒处理批次的最大值,仅用于批量模式默认值为10
adi-bo-config-filestring|当使用Webhook保存审计日志时Webhook配置文件的全路径,格式为kubeconfig格式
audit-webhook-initial-backoff-duration|当使用Webhook保存审计日志时,对第1个失败请求重试的等待时间,默认值为10s
--audit-webhook-mode string|当使用 Webhook保存审计日志时审计日志记录模式,包括同步模式 blocking或blocking-strict-和异步模式batch,默认值为batch
--audit-webhook-truncate-enabled|当使用 Webhook保存审计日志时,设置是否启用记录 Event分批截断机制
audit-webhook-truncate-max-batch-s---siz e int|当使用WebhookEvent保存审计日志时,设置每批次最大可保存的字节数,超过时自动分成新的批次,默认值为10485760
adi-webhook-truncate-maxevent-siz e int|当使用Webhook保存审计日志时,设置可保存Event的最大字节数,超过时自动移除第1个请求和应答,仍然超限时将丢弃该 Event,默认值为102400
-audit-webhook-version string|当使用 Webhook保存审计日志时审计日志的API版本号,默认值为 auditk8siov
其他特性参数|
--contention-profiling|当性能分析功能打开时,设置是否启用锁竞争分析功能
--profiling|设置为true表示打开性能分析功能,可以通过< host SporDdebugpprof地址查看程序栈、线程等系统信息,默认值为true
认证相关参数|
--anonymous-autl|设置为truc表示APIServer的安全端口可以接收匿名请求,不会被任何authentication拒绝的请求将被标记为匿名请求。匿名请求的用户名为 systemanonymous用户组为 system:unauthenticated,认值为true
--api-audiences strings|API标识符列表,服务账户令牌身份验证器将验证针对AP使用的令牌是否绑定到设置的至少一个APl标识符。如果设置-saccount标志但未设置此标志,则此字段默认包含颁发者URL的单个元素列表
authentication-token-webhook-cache-tt- I duration|将WebhookTokenAuthenticator返回的响应保存在缓存内的时间,默认值为2m0s
authentication--webhook-config-fto- ile string|Webhook相关的配置文件,将用于Authentication
-basic-auth-filestring|HTTP APIserver
-client-ca-filestring|如果指定,则该客户端证书将被用于认证
-enable-bootstrap-token-auth|设置在- TLS_认证引导时是否允许使用kb--system名空间中类型为bootstrap.kubemetes.io/tokensecret
--oidc-ca-file string|在该文件内设置鉴权机构, OpenID Server的证书将被其中一个机构验证。如果不设置,则将使用主机的 root CA证书
-oidc-client-id string|OpenIDConnect的客户端D,在设置oidc-issuer-url时必须设置这个D
--oidc-groups-claim string|定制的 OpenID Connect用户组声明的设置,以字符串数组的形式表示,实验用
--oidc-groups-prefix string|设置 OpenID用户组的前缀
--oidc-issuer-url string|openidurl HTTPS scheme, OIDC JSON Web发行者的URL地址,仅支持HTPschem,用于验证oidcjsonweb Token
od-required-claimmapStringString|用于描述D令牌所需的声明,以key-value形式表示,设置之后,该声明必须存在于匹配的ID令牌中,系统会对此进行验证。可以重复该参数以设置多个声明
--oidc-signing-algs strings|设置允许的JOSE非对称签名算法列表,以逗号分隔。 JWT header中ag的值不在该列表中时将被拒绝。值RFC8htp rfc7518 (https:/tools. ietforg#section-3.)8o)义默认值为RS256]
--oidc-usemame-claim string|OpenlDclaim用户名,默认值为sub,实验用
--oidc-usemame-prefix string|设置 OpenID用户名的前缀,未指定时使用发行方RL作为前缀以避免冲突,设置为表示不使用前缀
-requestheader-allowed-names-strings|允许的客户端证书中的commonnamesheader列表,通过requestheader-usemame-headersrequesthe参数指定的字段获取未设置时表示经过--验证的客户端证书都会被认可
requestheader-client-ca-filestring|用于验证客户端证书的根证书,在信任requestheader-username-headers参数中的用户名之前进行验证
-requestheader-eheaders-prefix-tr strings|待审查请求header的前缀列表,建议用x--Remote-Extra-
requestheader-group-headersstrings|待审查请求header的用户组的列表,建用x--Remote-Group-
-requestheader-username-headers strings|待审查请求header的用户名的列表,通常用x--Remote--user
--service-account-issuerstring|设置ServiceAccount颁发者的标识符。颁发者将在已颁发令牌的“iss”字段中断言此标识符,以字符串或ULI格式表示
-service-account-kelestringAray|包含PEM-encodedx5509sa公钥和私钥的文件路径,用于验证ServiceAccount的 Token若不指定,则使ts- -private--ky-fle指定的文件
--service-account-lookup|设置为true时,系统会到etcd验证 Service Account Token是否存在
service-account-max-token-expiration--- duration|设置ServiceAccount令牌颁发者创建的令牌的最长有效期。如果一个合法 TokenRequest请求申请的有效期更长,则以该最长有效期为准
--token-auth-file string|用于访问 API Server安全端口的 Token认证文件路径
授权相关参数|
--authorization-mode string|到 API Server的安全访问的认证模式列表,以逗号分隔,可选值包括: AlwaysAllow AlwaysDenyabac、 Webhook、AC,默认值为AlwaysAllow
-authorization-policy-file string|当-authorization-m-modc被设置为ABAC时使用的sv格式的授权配置文件
authorization-webhook-cache-authoriz- ed-itl duration|将WebhookAuthorizer返回的已授权响应保存在缓存内的时间,默认值为5m0s
authorization-webhook-cache-unauthor--- ized-ttl duration|将WebhookAuthorizer返回的未授权响应保存在缓存内的时间,默认值为30s
authorization-webhook-config-file- stnng|当-authorization-mmode设置为Webhook时使用的授权配置文件
云服务商相关参数|
-cloud-configstring|云服务商的配置文件路径,若不配置,则表示不使用云服务商的配置文件
--cloud-provider string|云服务商的名称,若不配置,则表示不使用云服务商
API相关参数|
-runtime-configmapStringString|开或关闭对某个AP版本的支持。api/all和ailegy特别用于支持所有版本的A或支持旧版本的AP
准入控制相关参数|已用,-enable-admission--plgn-disable-admission-plugins参数
--admission-controlstring|对发送给 API Server的请求进行准入控制,配置为一个准入控制器的列表,多个准入控制器之间以逗号分隔。多个准入控制器将按顺序对发送给 API Server的请求进行拦截和过滤。可配置的准入控制器包括: AlwaysAdmit、 AlwaysDeny AlwaysPullmages. DefaultStorageClass. DefaultTolerationSeconds, DenyEscalatingExec. DenyExecOnPrivileged. EventRateLimit, Exten
|PodPreset, PodSecurityPolicy, PodTolerationRestriction, Priority. ResourceQuota, SecurityContextDeny, ServiceAccount, StorageObjectInUseProtection, TaintNodesByCondition, ValidatingAdmission Webhook
admission-control-config-filestring|控制规则的配置文件
-disable-admission-plugins strings|设置禁用的准入控制插件列表,不论其是否在默认启用的插件列表中,以逗号分隔。默认启用的插件列表为 NamespaceLifecycle、 LimitRanger、 ServiceAccount TaintNodesByCondition, Priority, DefaultTolerationSeconds, DefaultStorageClass. PersistentVolumeClaimResize. MutatingAdmission Webhook. ValidatingAdmission Webhook, ResourceQuota.可选插件包括:AlwgysAdmiwAlwmsDy《AlwusPulmualwaysdenyalwayspullages DefaultStorageClass, DefaultTolerationSeconds, DenyEscalatingExec, DenyExecOnPrivileged、 EventRateLimit、 ExtendedResouImagePolicyWebhook,LimitPodHardAntiAffinityTopology,LimitRanger, MutatingAdmission Webhook, NamespaceAutoProvision, NamespaceExists, NamespaceLifecycle NodeRestriction OwnerReferencesPer Persistent VolumeClaimResize Persistent VolumeLabel、 PodNodeSelector、 PodPreset、 PodSecurityPolicy, Pod TolerationRestriction, Priority, ResourceQuota, SecurityContextDeny、 ServiceAccount StorageObjectInUseProtection、 TaintNodesByCondition、 ValidatingAdmission Webhook,先后顺序没有影响
-enable-admission-pluginsstrings|设置启用的准入控制插件列表,以逗号分隔。默认启用的插件列表为 NamespaceLifecycle、 LimitRanger、 ServiceAccount TaintNodesByCondition, Priority, DefaultTolerationSeconds, DefaultStorageClass. Persistent VolumeClaimResize, MutatingAdmission Webhook, ValidatingAdmission Webhook. ResourceQuota.可选插件包括: AlwaysAdmit、 AlwaysDeny Always Pulllmages DefaultStorageClass, DefaultTolerationSeconds, DenyEscalatingExec, DenyExecOnPrivileged, EventRateLimit, ExtendedResource Toleration. ImagePolicyWebhook, LimitPodHardAntiAffinityTopology, LimitRanger, MutatingAdmission Webhook, NamespaceAutoProvision NamespaceExists,NamespaceLifecycle NodeRestriction OwnerReferencesPermi Persistent VolumeClaimResize, Persistent VolumeLabel, PodNodeSelector, PodPreset. PodSecurityPolicy, PodTolerationRestriction. Priority. ResourceQuota. SecurityContextDeny, ServiceAccount, StorageObjectInUseProtection. TaintNodesByCondition、 ValidatingAdmission Webhook,先后顺序没有影响
其他参数|
--allow-privileged|设置为true时, Kubemetes将允许在Pod中运行拥有系统特权的容器应用,与 docker run-privileged-的效果相同
-apiserver-count int|集群中运行的 API Server数量,默认值为1
--enable-aggregator-routing|设置为tme表示aggregator请求路由到Endpoint的IP地址,否则路由到ClusterI
-enable-logs-handler|设置为true表示为 API Server安装一个logs的处理程序,默认值为tue
-endpoint-reconciler-type string|设置 Endpoint协调器的类型,可选类型包括 master-count-、 lease、none,默认值为lease
-event-ttl duration|Kubemetes事件的保存时间,默认为1h0m0s
-kubelet-certificate-authoritystring|用于a授权的cert文件路径
--kubelet-client-certificate string|用于- TLS_的客户端证书文件路径
--kubelet-client-key string|用于- TLS_的客户端key文件路径
-kubelet-https|指定kubelet HTTPStrue是否使用HTTPS连接,默认值为true
-kubelet-preferred-address-typesstrings|连接kubelet时使用的节点地址类型(NodeAddressTypes,默认值为列表HostnameIntemalDNSIntemallPExtemaIDNSExtemallPLegacyHostIP表示可用其中任一地址类型
--kubelet-timeoutint|kubelet执行操作的超时时间,默认值为5s
-kubemetes-service-node-portint|设置Master服务是否使用NodePort模式,如果设置,则Master服务将映射到物理机的端口号:设置为0表示以 ClusterIP的形式启动 Master服务
max-connection-bytes-per-secint|设置为非0的值表示限制每个客户端连接的带宽为xx字节/s,目前仅用于需要长时间执行的请求
--proxy-client-cert-filestring|用于在请求期间验证aggregatorkube-apiserver-身份的客户端证书文件路径。将请求代理到用户api- -server并调用 Webhook准入控制插件时,要求此证书在-requestheader-client---ca-le指定的文件中包含来自CA的签名。该CA发布在kube e-system-命名空间中名为 extension-apiserver-authenticatio- Configmap中
--proxy-client-key-file string|用于在请求期间验证 aggregator或kube- -apiserver身份的客户端私钥文件路径
-service-account-signing--file--ky-string|设置ServiceAccount令牌颁发者的当前私钥的文件路径颁发者用该私钥对颁发的ID令牌进行签名 TokenRequest。要求启用特性开关
--service-cluster-ip-range ipNet|Service的 Cluster IP(虚拟P)池,例如19.169.0.0/16,这个IP地址池不能与物理机所在的网络重合
--service-node-port-range portRange|Service的 NodePort能使用的主机端口号范围,默认值为30000~32767,包括3000和32767


## feature gates 开关

Key|Value|Default|说明
--|--|--|--
APIListChunking|true\|false|BETA=false|
APIResponseCompression|true\|false|ALPHA=false
AllAlpha|true\|falseALPHA-false
AppArmor|true\|false|BETA=true
Attach VolumeLimit|true\|false |BETA=true
BalanceAttachedNode Volumes|true\|false|ALPHA=-false
BlockVolume|true\|false |BETA=true
BoundServiceAccount TokenVolume|true\|false|ALPHA=false
CPUManager|true\|false |BETA=true
CRIContainerLogRotation|true\|false|BETA-true
CSIBlock Volume|true\|false |BETA=true
CSIDriverRegistry|true\|false|BETA=true
CSIInline Volume|true\|false|ALPHA=false
CSIMigration|true\|false |ALPHA=-false
CSIMigrationAWS|true\|false|ALPHA=false
CSIMigrationGCE|true\|false |ALPHA=-false
CSIMigrationOpenStack|true\|false|ALPHA=false
CSINodelnfo|true\|false|BETA=-true
CustomCPUCFSQuotaPeriod|true\|false|ALPHA=-false
CustomResourcePublishOpenAPI|true\|false|ALPHA-false
CustomResourceSubresources|true\|false |BETA=true
CustomResource Validation|true\|false |BETA=true
CustomResourceWebhookConversion|true\|false|ALPHA=false
DebugContainers|true/false|ALPHA=false
DevicePlugins|true\|false|BETA=true
DryRun|true\|false |BETA=true
DynamicAuditing|true\|false|ALPHA-false
DynamicKubeletConfig|true\|false|BETA=true
ExpandCSIVolumes|true\|false |ALPHA=false
ExpandInUsePersistent Volumes|true\|false |ALPHA=false
ExpandPersistent Volumes|true\|false |BETA=true
ExperimentalCriticalPodAnnotation|true\|false |ALPHA=false
ExperimentalHostUserNamespaceDefaulting|true\|false |BETA=false
HyperVContainer-|true\|false |ALPHA=-false
KubeletPodResources|true\|false|ALPHA=-false
LocalStorageCapacitylsolation|true\|false |BETA=true
MountContainers|true\|false|ALPHA=false
NodeLease|true\|false|BETA-true
PodShareProctrue|true\|false|BETA=true
ProcMountType|true\|false|ALPHA=false
QOSReserved|true\|false |ALPHA=false
ResourceLimitsPriorityFunction|true\|false|ALPHA=-false
ResourceQuotaScopeSelectors|true\|false|BETA-true
RotateKubeletClientCertificate|true\|false |BETA-true
RotateKubeletServerCertificate|true\|false|BETA=true
RunAsGroup=|true\|false|BETA=true
RuntimeClass|true\|false|BETA=true
SCTPSupport|true\|false|ALPHA=false
ScheduleDaemonSetPods|true\|false |BETA-true
ServerSideApply|true\|false|ALPHA=false
ServiceNodeExclusion|true\|false|ALPHA=false
Storage VersionHash|true\|false|ALPHA--false
StreamingProxyRedirects|true\|false|BETA=true
SupportNodePidsLimit|true\|false|ALPHA=-false
SupportPodPidsLimit|true\|false|BETA=true
Sysctls|true\|false|BETA=true
TTLAfterFinished|true\|false|ALPHA=false
TaintBasedEvictions|truelfalse|BETA-true
TaintNodesByCondition|true\|false|BETA=true
TokenRequest|true\|false|BETA-true
TokenRequestProjection|true\|false |BETA=true
ValidateProxyRedirects|true\|false|BETA=true
VolumeSnapshotDataSource|true\|false|ALPHA=-false
VolumeSubpathEnvExpansion|true\|false|ALPHA-false
WinDSR|true\|false|ALPHA=-false
WinOverlay|true\|false|ALPHA=false
WindowsGMSA|true\|false|ALPHA=false


## tls-cipher-suites 可选加密算法

- TLS_ECDHE_ECDSA_WITH_AES128_CBC_SHA
- TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
- TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
- TLS_ECDHE_ECDSA_WITH_AES256_CBC_SHA
- TLS_ECDHE_ECDSA_WITH_AES256_GCM_SHA384
- TLS_ECDHE_ECDSA_WITH_CHACHA20_POLYI305
- TLS_RSA_WITH_AES_128_CBC_SHA
- TLS_RSA_WITH_AES_128_CBC_SHA256
- TLS_RSA_WITH_AES_128_GCM_SHA256
- TLS_RSA_WITH_AES_256_CBC_SHA
- TLS_RSA_WITH_AES_256_GCM_SHA384
- TLS_RSA_WITH_RC4_128_SHA
