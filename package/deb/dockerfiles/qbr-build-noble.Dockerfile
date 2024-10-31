FROM ubuntu:noble
RUN ln -sf /usr/share/zoneinfo/Etc/UTC /etc/localtime && echo Etc/UTC > /etc/timezone
RUN apt update && apt install -y git debhelper dpkg-dev cmake qt6-webengine-dev libzip-dev lsb-release dh-cmake
COPY ./package/deb/helpers/build.sh /
RUN mkdir -p /release/
ENV PKG_VERSION=0.0.8
ENV PKG_PPAVER=1
WORKDIR /
ENTRYPOINT [ "/build.sh" ]
