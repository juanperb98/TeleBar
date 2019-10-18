from ubuntu:18.04
# install basic complation tools
run apt-get update -y
run apt-get upgrade -y
run apt-get install build-essential cmake -y

# install the postgresql library
run apt-get install libmysqlclient-dev -y

# copy the app to the docker
run mkdir /app
copy . /application
workdir /application

# build the server application
run mkdir build_docker
workdir /application/build_docker
run cmake ..
