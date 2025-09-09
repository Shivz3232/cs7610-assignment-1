FROM ubuntu:22.04

RUN apt-get update
RUN apt-get install -y gcc
# RUN apt-get install -y netcat iputils-ping

ENV APP_HOME=/usr/src/prj1

RUN mkdir -p ${APP_HOME}
WORKDIR ${APP_HOME}

COPY . .

ENV PORT=3000

RUN gcc main.c -o main

ENTRYPOINT ${APP_HOME}/main
