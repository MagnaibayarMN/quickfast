# syntax=docker/dockerfile:1.7
# Copyright (c) 2009, Object Computing, Inc.
# All rights reserved.
# See the file license.txt for licensing information.
#
# Build QuickFAST and its examples without installing anything on the host:
#
#   docker build -t quickfast .
#   docker run --rm quickfast -?
#
# Dependencies come from vcpkg, pinned to the baseline in vcpkg.json, so the
# build does not change underneath you when Boost or Xerces-C publish a new
# release.

FROM ubuntu:24.04 AS build

ARG VCPKG_ROOT=/opt/vcpkg
# Keep this in step with the builtin-baseline in vcpkg.json.
ARG VCPKG_BASELINE=32305df7d0b9a308e6ac454dd98ebfe550342c09
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
      git curl zip unzip tar ca-certificates \
      build-essential cmake ninja-build pkg-config \
      autoconf automake libtool python3 \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/microsoft/vcpkg ${VCPKG_ROOT} \
    && git -C ${VCPKG_ROOT} checkout ${VCPKG_BASELINE} \
    && ${VCPKG_ROOT}/bootstrap-vcpkg.sh -disableMetrics
ENV VCPKG_ROOT=${VCPKG_ROOT}

WORKDIR /src
COPY . /src

# vcpkg builds Boost and Xerces-C from source, which is slow.  The cache mounts
# keep the downloads and the built packages between builds so that only
# QuickFAST itself is recompiled when a source file changes.
ENV VCPKG_DEFAULT_BINARY_CACHE=/vcpkg-cache
RUN --mount=type=cache,target=/vcpkg-cache \
    --mount=type=cache,target=/opt/vcpkg/downloads \
    mkdir -p /vcpkg-cache \
    && cmake --preset vcpkg -G Ninja \
    && cmake --build build \
    && ctest --test-dir build --output-on-failure

FROM ubuntu:24.04 AS runtime
RUN apt-get update && apt-get install -y --no-install-recommends \
      libstdc++6 ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /src/build/src/Examples/InterpretApplication /usr/local/bin/
COPY --from=build /src/build/src/Examples/PerformanceTest /usr/local/bin/
COPY --from=build /src/build/src/Examples/TutorialApplication /usr/local/bin/
COPY --from=build /src/build/src/Examples/FileToMulticast /usr/local/bin/
COPY --from=build /src/build/src/Examples/FileToTCP /usr/local/bin/
COPY --from=build /src/build/src/Examples/PCapToMulticast /usr/local/bin/

# InterpretApplication decodes FAST data according to a template file; run it
# with -? for the full list of options.
ENTRYPOINT ["InterpretApplication"]
CMD ["-?"]
