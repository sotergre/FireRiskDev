FROM ubuntu:16.04
ENTRYPOINT /bin/bash
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
 default-jdk 
 
RUN git clone https://github.com/sotergre/FireRiskDev.git /firecast && cd /firecast && git checkout Expanded_Window
RUN cd /firecast && tar -xf DATA.tar.bz
RUN export set HOME="/firecast" && cd /firecast/SOURCE/ && ./MakeAll.sh && chmod 777 ../BIN/MOD7DownloaderHEMI.jar

RUN cd /firecast/lib && unzip HDFLOOK_LINUX_INTEL64 && unzip gctpc && unzip gscript && unzip netcdf-3.6.2 && cd /firecast/lib/gctpc/source && make