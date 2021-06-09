FROM debian:buster
RUN apt-get -y update && apt-get -y upgrade && apt-get -y install valgrind build-essential clang vim
COPY . . 
CMD ["sleep", "infinity"]
