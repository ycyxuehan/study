FROM golang:1.16
RUN apt update && apt install -y openssh-server vim net-tools && cp /usr/share/zoneinfo/Asia/Shanghai  /etc/localtime
COPY kubectl /usr/bin/kubectl
WORKDIR /git
CMD [ "sleep", "1314000" ]