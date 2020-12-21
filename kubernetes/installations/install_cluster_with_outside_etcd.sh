#!/bin/bash

init_etcd(){
    echo "init etcd cluster: $@"
    echo "create config files..."
    cat << EOF > /tmp/kubelet.service.d/20-etcd-service-manager.conf
[Service]
ExecStart=
#  Replace "systemd" with the cgroup driver of your container runtime. The default value in the kubelet is "cgroupfs".
ExecStart=/usr/bin/kubelet --address=127.0.0.1 --pod-manifest-path=/etc/kubernetes/manifests --cgroup-driver=systemd
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
    for i in ${![@]}; do
        ETCD_HOST=${$@[$i]}
        if [ "x${INIT_CLUSTERS}" == "x" ]; then
            INIT_CLUSTERS="${NAME_PREFIX}${i}=https://${ETCD_HOST}:2380"
        else
            INIT_CLUSTERS="${INIT_CLUSTERS},${NAME_PREFIX}${i}=https://${ETCD_HOST}:2380"
        fi
    done
    echo "init_clusters: ${INIT_CLUSTERS}"
    for HOST in ${@};
    do
        mkdir /tmp/${HOST}
        cat <<EOF >/tmp/${HOST}/etcdcfg.yaml
apiVersion: "kubeadm.k8s.io/v1beta2"
kind: ClusterConfiguration
imageRegistry: registry.bing89.com/kubernetes
etcd:
    local:
        serverCertSANs:
        - "${HOST}"
        peerCertSANs:
        - "${HOST}"
        extraArgs:
            initial-cluster: ${INIT_CLUSTERS}
            initial-cluster-state: new
            name: ${NAME}
            listen-peer-urls: https://${HOST}:2380
            listen-client-urls: https://${HOST}:2379
            advertise-client-urls: https://${HOST}:2379
            initial-advertise-peer-urls: https://${HOST}:2380
EOF
    kubeadm init phase certs etcd-server --config=/tmp/${HOST}/kubeadmcfg.yaml
    kubeadm init phase certs etcd-peer --config=/tmp/${HOST}/kubeadmcfg.yaml
    kubeadm init phase certs etcd-healthcheck-client --config=/tmp/${HOST}/kubeadmcfg.yaml
    kubeadm init phase certs apiserver-etcd-client --config=/tmp/${HOST}/kubeadmcfg.yaml
    cp -R /etc/kubernetes/pki /tmp/${HOST}/
    # 清理不可重复使用的证书
    find /etc/kubernetes/pki -not -name ca.crt -not -name ca.key -type f -delete
    # 清理不应从此主机复制的证书
    find /tmp/${HOST} -name ca.key -type f -delete
    done
    echo "configure etcd hosts"
    for HOST in ${@};
    do
        scp /tmp/kubelet.service.d /etc/systemd/system/
        scp /tmp/${HOST}/* ${HOST}:~/
        ssh ${HOST} "systemctl daemon-reload && systemctl restart kubelet"
        ssh ${HOST} "mv ~/pki /etc/kubernetes/"
        ssh ${HOST} "kubeadm init phase etcd local --config=~/etcdcfg.yaml"
    done
    echo "cluster init finished. use this command to check cluster status"
    echo "docker run --rm -it --net host -v /etc/kubernetes:/etc/kubernetes k8s.gcr.io/etcd:3.4.13-0 etcdctl --cert /etc/kubernetes/pki/etcd/peer.crt --key /etc/kubernetes/pki/etcd/peer.key --cacert /etc/kubernetes/pki/etcd/ca.crt --endpoints https://${HOST}:2379 endpoint health --cluster"
}

init_controller(){
    echo "init controller: $@"
    echo "warning: your kubelet will reset"
    echo "creating kubeadm config file"
    ENDPOINTS="["
    for ETCD_HOST in ETCD_HOSTS; do
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
    scp /tmp/kubeadmcfg.yaml ${CONTROLLER_HOST}:/tmp/kubeadmcfg.yaml
    scp /etc/kubernetes/pki/etcd/ca.crt ${CONTROLLER_HOST}:/etc/kubernetes/pki/etcd/ca.crt
    scp /etc/kubernetes/pki/apiserver-etcd-client.crt "${CONTROLLER_HOST}":/etc/kubernetes/pki/apiserver-etcd-client.crt
    scp /etc/kubernetes/pki/apiserver-etcd-client.key "${CONTROLLER_HOST}":/etc/kubernetes/pki/apiserver-etcd-client.key
    ssh ${CONTROLLER_HOST} "kubeadm init --config /tmp/kubeadmcfg.yaml --upload-certs"
}

init_network(){
    echo "init cni: $@"

}

USAGE_EXITS(){
    echo "this is help message."
}

main(){
    while getopts "e:c:n:a:h" arg
    do
        case ${arg} in 
            e)
                ETCD_HOSTS=${OPTARG//,/ }
                INIT_ETCD=True
                ;;
            c)
                CONTROLLER_HOST=${OPTARG}
                INIT_CONTROLLER=True
                ;;
            n)
                NETWORK_CNI=${OPTARG}
                INIT_NETWORK=True
                ;;
            a)
                APIHOSTS=${OPTARG}
                ;;
            h)
                USAGE_EXITS
                ;;
        esac
    done
    echo "init_etcd${INIT_ETCD}"
    if [ "x${INIT_ETCD}" == "xTrue" ];then
        init_etcd ${ETCD_HOSTS}
    fi
        if [ "x${INIT_CONTROLLER}" == "xTrue" ];then
        init_controller ${CONTROLLER_HOST}
    fi
        if [ "x${INIT_NETWORK}" == "xTrue" ];then
        init_network ${NETWORK_CNI}
    fi
}

main $@