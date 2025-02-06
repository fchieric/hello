FROM ubuntu:22.04

# Prevent interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies in a single RUN to reduce layers
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    git \
    ca-certificates \
    curl \
    gnupg \
    lsb-release && \
    rm -rf /var/lib/apt/lists/*

# Install Norminette
RUN git clone https://github.com/42School/norminette.git && \
    cd norminette && \
    python3 -m pip install --upgrade pip setuptools && \
    python3 -m pip install . && \
    cd .. && \
    rm -rf norminette

WORKDIR /app

# Keep container running
CMD ["tail", "-f", "/dev/null"]
