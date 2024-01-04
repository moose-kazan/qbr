FROM debian:bookworm
RUN ln -sf /usr/share/zoneinfo/Etc/UTC /etc/localtime && echo Etc/UTC > /etc/timezone
RUN apt update && apt install -y git debhelper dpkg-dev qt5-qmake qtwebengine5-dev libzip-dev pkg-config lsb-release
RUN apt install -y libquazip5-dev
COPY ./package/deb/helpers/build.sh /
RUN mkdir -p /release/
ENV PKG_VERSION=0.0.4
ENV PKG_PPAVER=1
WORKDIR /
ENTRYPOINT [ "/build.sh" ]
