FROM ubuntu:16.04
ENTRYPOINT /bin/bash
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
RUN export set HOME="/firecast" && cd /firecast && tar -xf DATA.tar.bz
RUN export set HOME="/firecast" && cd /firecast/lib && unzip HDFLOOK_LINUX_INTEL64 && unzip gctpc && unzip gscript && cd /firecast/lib/gctpc/source && make
ADD lib/netcdf-3.6.2.zip /firecast/lib/
RUN export set HOME="/firecast" && cd /firecast/SOURCE/ && ./MakeAll.sh && chmod 777 ../BIN/MOD7DownloaderHEMI.jar
ENV AWS_DEFAULT_PROFILE="$AWS_DEFAULT_PROFILE" AWS_SECRET_ACCESS_KEY="$AWS_SECRET_ACCESS_KEY" AWS_ACCESS_KEY_ID="$AWS_ACCESS_KEY_ID" AWS_DEFAULT_PROFILE="default" AWS_DEFAULT_REGION="us-east-1"
CMD /bin/bash -c aws configure -
