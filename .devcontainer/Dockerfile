# Development container for QuickFAST: a C++ toolchain, vcpkg, and a debugger.
# The dependencies themselves are built by vcpkg the first time the project is
# configured with the vcpkg preset.
FROM ubuntu:24.04

ARG VCPKG_ROOT=/opt/vcpkg
# Keep this in step with the builtin-baseline in vcpkg.json.
ARG VCPKG_BASELINE=32305df7d0b9a308e6ac454dd98ebfe550342c09
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
      git curl zip unzip tar ca-certificates \
      build-essential cmake ninja-build pkg-config gdb \
      autoconf automake libtool python3 perl \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/microsoft/vcpkg ${VCPKG_ROOT} \
    && git -C ${VCPKG_ROOT} checkout ${VCPKG_BASELINE} \
    && ${VCPKG_ROOT}/bootstrap-vcpkg.sh -disableMetrics
ENV VCPKG_ROOT=${VCPKG_ROOT}
