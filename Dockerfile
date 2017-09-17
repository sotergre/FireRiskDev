FROM ubuntu:14.04
SHELL ["/bin/bash", "-c"]
ENV HOME="/firecast"
RUN apt-get update && apt-get install -y \
 gcc \
 curl \
 zip \
 bzip2 \
 git \
 clang \
 grads \
 awscli \
 ncftp \
 gdal-bin \
 java-common \
 ssmtp \
 make \
 default-jdk \
 wget \
 libxft2
 
RUN git clone https://github.com/sotergre/FireRiskDev.git /firecast && cd /firecast && git checkout Expanded_Window
WORKDIR /firecast
ENV HOME="/firecast"
RUN tar -xf DATA.tar.bz
WORKDIR /firecast/lib
RUN unzip HDFLOOK_LINUX_INTEL64 && unzip gctpc && unzip gscript && cd /firecast/lib/gctpc/source && make
WORKDIR /firecast/SOURCE
RUN ./MakeAll.sh && chmod 777 ../BIN/MOD7DownloaderHEMI.jar
ENV AWS_SECRET_ACCESS_KEY="$AWS_SECRET_ACCESS_KEY" AWS_ACCESS_KEY_ID="$AWS_ACCESS_KEY_ID" AWS_DEFAULT_PROFILE="default" AWS_DEFAULT_REGION="us-east-1"
WORKDIR /firecast

CMD ./firecast_init_manual_HEMI.sh
