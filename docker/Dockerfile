ARG IMAGE_BASE

FROM ${IMAGE_BASE} AS base

LABEL maintainer="Kirill Kazakhmedov <k.kirill777@gmail.com>"

RUN apt update && apt install -y \
        gcc-12 \
        g++-12 \
        software-properties-common \
        curl \
        cmake \
        build-essential \
        make \
        libgtest-dev \
        libgmock-dev \
        pre-commit \
        libssl-dev \
        git \
        libtclap-dev \
        libjsoncpp-dev \
        libjsoncpp25 \
        nano

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 12 --slave /usr/bin/g++ g++ /usr/bin/g++-12

FROM base AS develop
WORKDIR "/root/workspace"
