# FROM alpine:3.20

FROM ubuntu:22.04

# RUN apk --no-cache --upgrade add \
#     git \
#     git-lfs \
#     curl \
#     jq \
#     bash \
#     build-base \
#     python3 \
#     valgrind \
#     cppcheck \
#     alpine-sdk \
#     pcre-dev \
#     pcre2 \
#     clang18-extra-tools \
#     check \
#     check-dev \
#     gtest-dev \
#     lcov

RUN apt-get update
RUN apt-get install -y clang-format
RUN apt-get install -y gcc
RUN apt-get install -y g++
RUN apt-get install -y valgrind
RUN apt-get install -y make
RUN apt-get install -y cmake
RUN apt-get install -y nano
RUN apt-get install -y check
RUN apt-get install -y gcovr
RUN apt-get install -y lcov
RUN apt-get install -y libgtest-dev
RUN apt-get install -y build-essential
RUN apt-get install -y --fix-missing qt6-base-dev
RUN rm -rf /var/lib/apt/lists/*
# RUN apt-get install -y pkg-config

# COPY . /project
