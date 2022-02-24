ARG VARIANT=bullseye
FROM mcr.microsoft.com/vscode/devcontainers/python:0-${VARIANT}

RUN apt-get update && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y install --no-install-recommends \
  zip

RUN pip install poetry
RUN poetry config virtualenvs.create false

WORKDIR /app
RUN git clone --depth 1 --branch v0.1.0 https://github.com/heia-fr/mkdocs-edu-poetry.git mkdocs-edu
WORKDIR /app/mkdocs-edu
RUN poetry install

COPY scripts/serve.sh /usr/local/bin/serve
COPY scripts/build.sh /usr/local/bin/build