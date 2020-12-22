#!/bin/bash

init_etcd(){
    echo "init etcd cluster: $@"
    echo "create config files..."
    kubeadm reset -f
    if [ ! -d /tmp/kubelet.service.d ];then
        mkdir /tmp/kubelet.service.d
    fi
    cat << EOF > /tmp/kubelet.service.d/20-etcd-service-manager.conf
[Service]
ExecStart=
#  Replace "systemd" with the cgroup driver of your container runtime. The default value in the kubelet is "cgroupfs".
ExecStart=/usr/bin/kubelet --address=127.0.0.1 --pod-manifest-path=/etc/kubernetes/manifests --cgroup-driver=systemd --pod-infra-container-image=registry.bing89.com/kubernetes/pause:3.2
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
apiVersion: "kubeadm.k8s.io/v1beta2"
kind: ClusterConfiguration
imageRepository: registry.bing89.com/kubernetes
etcd:
    local:
        serverCertSANs:
        - "${HOST}"
        - "${CONTROLLER_HOST}"
        - "${APIHOSTS}"
        peerCertSANs:
        - "${HOST}"
        - "${CONTROLLER_HOST}"
        - "${APIHOSTS}"
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
        scp -r /tmp/kubelet.service.d ${HOST}:/etc/systemd/system/
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
    cat <<EOF >/tmp/kubeadmcfg.yaml
apiVersion: kubeadm.k8s.io/v1beta2
kind: ClusterConfiguration
kubernetesVersion: stable
controlPlaneEndpoint: "${APIHOSTS}:6443"
imageRepository: registry.bing89.com/kubernetes
etcd:
    external:
        endpoints: ${ENDPOINTS}
        caFile: /etc/kubernetes/pki/etcd/ca.crt
        certFile: /etc/kubernetes/pki/apiserver-etcd-client.crt
        keyFile: /etc/kubernetes/pki/apiserver-etcd-client.key

EOF
    kubeadm reset -f
    scp /tmp/${ETCD_HOSTS[0]}/pki/etcd/ca.crt /etc/kubernetes/pki/etcd/ca.crt
    scp /tmp/${ETCD_HOSTS[0]}/pki/apiserver-etcd-client.crt /etc/kubernetes/pki/apiserver-etcd-client.crt
    scp /tmp/${ETCD_HOSTS[0]}/pki/apiserver-etcd-client.key /etc/kubernetes/pki/apiserver-etcd-client.key
    kubeadm init --config /tmp/kubeadmcfg.yaml --upload-certs
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
    # if [ "x${CONTROLLER_HOST}" == "x" ];then
    #     USAGE_EXITS
    # fi
    if [ "x${APIHOSTS}" == "x" ];then
        USAGE_EXITS
    fi
}
main(){
    COMMAND=$1
    shift
    while getopts "e:n:a:h" arg
    do
        case ${arg} in 
            e)
                ETCD_HOSTS=${OPTARG//,/ }
                ;;
            # c)
            #     CONTROLLER_HOST=${OPTARG}
            #     ;;
            n)
                NETWORK_CNI=${OPTARG}
                ;;
            a)
                APIHOSTS=${OPTARG}
                ;;
            h)
                USAGE_EXITS
                ;;
        esac
    done
    check_args
    case ${COMMAND} in
    initetcd)
        init_etcd ${ETCD_HOSTS}
        ;;
    initcontrollplane)
        init_controller ${CONTROLLER_HOST}
        ;;
    initcni)
        init_network ${NETWORK_CNI}
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
