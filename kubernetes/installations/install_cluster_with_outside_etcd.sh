#!/bin/bash

init_etcd(){
    echo "init etcd cluster: $@"
    echo "create config files..."
    kubeadm reset -f
    if [ ! -d /etc/systemd/system/kubelet.service.d ];then
        mkdir /etc/systemd/system/kubelet.service.d
    fi
    cat << EOF > /etc/systemd/system/kubelet.service.d/20-etcd-service-manager.conf
[Service]
ExecStart=
#  Replace "systemd" with the cgroup driver of your container runtime. The default value in the kubelet is "cgroupfs".
ExecStart=/usr/bin/kubelet --container-runtime=remote --container-runtime-endpoint=unix:///run/containerd/containerd.sock  --address=127.0.0.1 --pod-manifest-path=/etc/kubernetes/manifests --pod-infra-container-image=registry.bing89.com/kubernetes/pause:3.2
Restart=always
EOF
    if [[ -f etc/kubernetes/pki/etcd/ca.crt ]] && [[ -f etc/kubernetes/pki/etcd/ca.key ]];then
        echo 'using exists key.'
    else
        if [[ -f etc/kubernetes/pki/etcd/ca.crt ]] || [[ -f etc/kubernetes/pki/etcd/ca.key ]]; then
            echo 'ssl key config error, maybe there one key is exists. exit '
            exit 2
        fi
        echo 'init kubernetes ssh keys'
        kubeadm init phase certs etcd-ca
    fi
    NAME_PREFIX="infra"
    INDEX=0
    INIT_CLUSTERS=""
    for ETCD_HOST in $@; do
        if [ "x${INIT_CLUSTERS}" == "x" ]; then
            INIT_CLUSTERS="${NAME_PREFIX}${INDEX}=https://${ETCD_HOST}:2380"
        else
            INIT_CLUSTERS="${INIT_CLUSTERS},${NAME_PREFIX}${INDEX}=https://${ETCD_HOST}:2380"
        fi
        INDEX=$(expr ${INDEX} + 1)
    done
    echo "init_clusters: ${INIT_CLUSTERS}"
    INDEX=0
    for HOST in ${@};
    do
        if [ ! -d /tmp/${HOST} ];then
            mkdir /tmp/${HOST}
        else
            rm -rf /tmp/${HOST}/*
        fi
        cat <<EOF >/tmp/${HOST}/etcdcfg.yaml
apiVersion: kubeadm.k8s.io/v1beta2
kind: InitConfiguration
nodeRegistration:
    criSocket: /run/containerd/containerd.sock
    name: containerd
---
apiVersion: "kubeadm.k8s.io/v1beta2"
kind: ClusterConfiguration
imageRepository: registry.bing89.com/kubernetes
etcd:
    local:
        serverCertSANs:
        - "${HOST}"
        - "${APIHOST}"
        - "$1"
        peerCertSANs:
        - "${HOST}"
        - "${APIHOST}"
        - "$1"
        extraArgs:
            initial-cluster: ${INIT_CLUSTERS}
            initial-cluster-state: new
            name: ${NAME_PREFIX}${INDEX}
            listen-peer-urls: https://${HOST}:2380
            listen-client-urls: https://${HOST}:2379
            advertise-client-urls: https://${HOST}:2379
            initial-advertise-peer-urls: https://${HOST}:2380
EOF
        
        kubeadm init phase certs etcd-server --config=/tmp/${HOST}/etcdcfg.yaml
        kubeadm init phase certs etcd-peer --config=/tmp/${HOST}/etcdcfg.yaml
        kubeadm init phase certs etcd-healthcheck-client --config=/tmp/${HOST}/etcdcfg.yaml
        kubeadm init phase certs apiserver-etcd-client --config=/tmp/${HOST}/etcdcfg.yaml
        cp -R /etc/kubernetes/pki /tmp/${HOST}/
        # 清理不可重复使用的证书
        find /etc/kubernetes/pki -not -name ca.crt -not -name ca.key -type f -delete
        # 清理不应从此主机复制的证书
        if [ "x${INDEX}" != "x0" ];then
            find /tmp/${HOST} -name ca.key -type f -delete
        fi
        INDEX=$(expr ${INDEX} + 1)
    done
    echo "configure etcd hosts"
    for HOST in ${@};
    do
        scp -r /etc/systemd/system/kubelet.service.d ${HOST}:/etc/systemd/system/
        scp -r /tmp/${HOST}/* ${HOST}:/tmp
        ssh ${HOST} "systemctl daemon-reload"
        ssh ${HOST} "kubeadm reset -f && rsync -ivhPr /tmp/pki /etc/kubernetes/"
        ssh ${HOST} "systemctl restart kubelet && kubeadm init phase etcd local --config=/tmp/etcdcfg.yaml"
    done
    echo "cluster init finished. use this command to check cluster status"
    echo "docker run --rm -it --net host -v /etc/kubernetes:/etc/kubernetes registry.bing89.com/kubernetes/etcd:3.4.13-0 etcdctl --cert /etc/kubernetes/pki/etcd/peer.crt --key /etc/kubernetes/pki/etcd/peer.key --cacert /etc/kubernetes/pki/etcd/ca.crt --endpoints https://${HOST}:2379 endpoint health --cluster"
}

init_controller(){
    echo "init controller: $@"
    echo "warning: your kubelet will reset"
    echo "creating kubeadm config file"
    ENDPOINTS="["
    for ETCD_HOST in ${ETCD_HOSTS}; do
        if [ "x${ENDPOINTS}" == "x[" ];then
            ENDPOINTS="${ENDPOINTS}\"https://${ETCD_HOST}:2379\""
        else
            ENDPOINTS="${ENDPOINTS},\"https://${ETCD_HOST}:2379\""
        fi
    done
    ENDPOINTS="${ENDPOINTS}]"
    echo "etcd end points: ${ENDPOINTS}"
    APISERVERSANS="["
    for CONTROLLER_HOST in ${CONTROLLER_HOSTS}
    do
        if [ "x${APISERVERSANS}" == "x[" ];then
            APISERVERSANS="${APISERVERSANS}\"${CONTROLLER_HOST}\""
        else
            APISERVERSANS="${APISERVERSANS},\"${CONTROLLER_HOST}\""
        fi
    done
    APISERVERSANS="${APISERVERSANS}]"
    cat <<EOF >/tmp/kubeadmcfg.yaml
apiVersion: kubeadm.k8s.io/v1beta2
kind: InitConfiguration
nodeRegistration:
    criSocket: /run/containerd/containerd.sock
    name: containerd
---
apiVersion: kubeadm.k8s.io/v1beta2
kind: ClusterConfiguration
kubernetesVersion: stable
controlPlaneEndpoint: "${APIHOST}:6443"
imageRepository: registry.bing89.com/kubernetes
etcd:
    external:
        endpoints: ${ENDPOINTS}
        caFile: /etc/kubernetes/pki/etcd/ca.crt
        certFile: /etc/kubernetes/pki/apiserver-etcd-client.crt
        keyFile: /etc/kubernetes/pki/apiserver-etcd-client.key
apiServer:
  certSANs: ${APISERVERSANS}
EOF
    kubeadm reset -f
    mkdir /etc/kubernetes/pki/etcd/
    scp /tmp/$1/pki/etcd/ca.crt /etc/kubernetes/pki/etcd/ca.crt
    scp /tmp/$1/pki/apiserver-etcd-client.crt /etc/kubernetes/pki/apiserver-etcd-client.crt
    scp /tmp/$1/pki/apiserver-etcd-client.key /etc/kubernetes/pki/apiserver-etcd-client.key
    kubeadm init --config /tmp/kubeadmcfg.yaml --upload-certs
}

init_haproxy(){
    echo 'init haproxy'
    echo 'write haproxy config...'
    APISERVERS=""
    INDEX=1
    PREFIX="k8smaster"
    for HOST in ${CONTROLLER_HOSTS}
    do
        if [ "x${APISERVERS}" == "x" ];then
            APISERVERS="        server ${PREFIX}${INDEX} ${HOST}:6443 weight 1 maxconn 1000 check inter 2000 rise 2 fall 3\n"
        else
            APISERVERS="${APISERVERS}        server ${PREFIX}${INDEX} ${HOST}:6443 weight 1 maxconn 1000 check inter 2000 rise 2 fall 3\n"
        fi
        INDEX=$(expr ${INDEX} + 1)
    done
    cat <<EOF >/tmp/haproxy.cfg
# /etc/haproxy/haproxy.cfg
#---------------------------------------------------------------------
# Global settings
#---------------------------------------------------------------------
global
    log /dev/log local0
    log /dev/log local1 notice
    daemon

#---------------------------------------------------------------------
# common defaults that all the 'listen' and 'backend' sections will
# use if not designated in their block
#---------------------------------------------------------------------
defaults
    mode                    http
    log                     global
    option                  httplog
    option                  dontlognull
    option http-server-close
    option forwardfor       except 127.0.0.0/8
    option                  redispatch
    retries                 1
    timeout http-request    10s
    timeout queue           20s
    timeout connect         5s
    timeout client          20s
    timeout server          20s
    timeout http-keep-alive 10s
    timeout check           10s

#---------------------------------------------------------------------
# apiserver frontend which proxys to the masters
#---------------------------------------------------------------------
frontend apiserver
    bind *:6443
    mode tcp
    option tcplog
    default_backend apiserver

#---------------------------------------------------------------------
# round robin balancing for apiserver
#---------------------------------------------------------------------
backend apiserver
    option httpchk GET /healthz
    http-check expect status 200
    mode tcp
    option ssl-hello-chk
    balance     roundrobin
${APISERVERS}
EOF
echo "write haproxy pod yaml"
cat <<EOF >/tmp/haproxy.yaml
apiVersion: v1
kind: Pod
metadata:
  name: haproxy
  namespace: kube-system
spec:
  containers:
  - image: haproxy:2.1.4
    name: haproxy
    livenessProbe:
      failureThreshold: 8
      httpGet:
        host: localhost
        path: /healthz
        port: 6443
        scheme: HTTPS
    volumeMounts:
    - mountPath: /usr/local/etc/haproxy/haproxy.cfg
      name: haproxyconf
      readOnly: true
  hostNetwork: true
  volumes:
  - hostPath:
      path: /etc/haproxy/haproxy.cfg
      type: FileOrCreate
    name: haproxyconf
status: {}
EOF
    for HOST in ${CONTROLLER_HOSTS}
    do
        scp /tmp/haproxy.cfg ${HOST}:/etc/haproxy/haproxy.cfg
        scp /tmp/haproxy.yaml  ${HOST}:/etc/kubernetes/manifests/haproxy.yaml
    done
    echo 'init haproxy finished'
}

init_network(){
    echo "init cni: $@"

}

USAGE_EXITS(){
    echo "this is help message."
    exit 1
}

check_args(){
    if [ "x${ETCD_HOSTS}" == "x" ];then
        USAGE_EXITS
    fi
     if [ "x${CONTROLLER_HOSTS}" == "x" ];then
         USAGE_EXITS
     fi
    if [ "x${APIHOST}" == "x" ];then
        USAGE_EXITS
    fi
}
main(){
    COMMAND=$1
    shift
    while getopts "c:e:n:a:h" arg
    do
        case ${arg} in 
            e)
                ETCD_HOSTS=${OPTARG//,/ }
                ;;
            c)
                CONTROLLER_HOSTS=${OPTARG//,/ }
                ;;
            n)
                NETWORK_CNI=${OPTARG}
                ;;
            a)
                APIHOST=${OPTARG}
                ;;
            h)
                USAGE_EXITS
                ;;
        esac
    done
    check_args
    case ${COMMAND} in
    etcd)
        init_etcd ${ETCD_HOSTS}
        ;;
    controllplane)
        init_controller ${ETCD_HOSTS}
        ;;
    network)
        init_network ${NETWORK_CNI}
        ;;
    haproxy)
        init_haproxy
        ;;
    help)
        USAGE_EXITS
        ;;
    *)
        USAGE_EXITS
        ;;
    esac
}

main $@
