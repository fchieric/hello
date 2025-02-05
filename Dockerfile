FROM ubuntu:22.04

# A norminette serve python per venire installata con pip (e git). Scarico i pacchetti di python, pip e git.
RUN apt-get update && apt-get install -y python3 python3-pip git

# Clona la repo, vi entra ed esegue i comandi per installare norminette
RUN git clone https://github.com/42School/norminette.git && cd norminette && \
    python3 -m pip install --upgrade pip setuptools && \
    python3 -m pip install .
