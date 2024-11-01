FROM ubuntu:jammy
RUN ln -sf /usr/share/zoneinfo/Etc/UTC /etc/localtime && echo Etc/UTC > /etc/timezone
RUN apt update && apt install -y git debhelper dpkg-dev lsb-release cmake qt6-webengine-dev \
    libzip-dev qt6-tools-dev qt6-webengine-dev-tools libgl1-mesa-dev libvulkan-dev \
    qt6-l10n-tools qt6-tools-dev-tools
COPY ./package/deb/helpers/build.sh /
RUN mkdir -p /release/
ENV PKG_VERSION=0.0.8
ENV PKG_PPAVER=1
WORKDIR /
ENTRYPOINT [ "/build.sh" ]
