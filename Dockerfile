from ubuntu:18.04
run apt-get update -y
run apt-get upgrade -y
run apt-get install build-essential cmake -y
run mkdir /app
copy . /application
workdir /application
run mkdir build_docker
workdir /application/build_docker
run cmake ..
