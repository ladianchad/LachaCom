FROM ubuntu:latest
SHELL [ "/bin/bash", "-c" ]
RUN mkdir -p /opt/test/workspace/communication  &&\
  apt update  && \
  apt upgrade -y && \
  apt install build-essential cmake wget nlohmann-json3-dev libspdlog-dev -y
WORKDIR /opt/test/workspace/communication
COPY . /opt/test/workspace/communication
RUN mkdir build && cd build && cmake --build --install ..
# RUN cd test && cmake . && make