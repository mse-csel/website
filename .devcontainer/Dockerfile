ARG VARIANT=bullseye
FROM mcr.microsoft.com/vscode/devcontainers/python:0-${VARIANT}

RUN apt-get update && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y install --no-install-recommends \
  imagemagick \
  zip

WORKDIR /mkdocs-edu
COPY mkdocs-edu/* /mkdocs-edu/

RUN pip install poetry && \
  poetry config virtualenvs.create false && \
  poetry install

COPY scripts/serve.sh /usr/local/bin/serve
COPY scripts/build.sh /usr/local/bin/build