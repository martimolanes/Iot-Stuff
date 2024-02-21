# Iot final Project

## Structure
##### 1. config-files 
This folder contains the configuration files for the mqtt-broker.
##### 2. docker
This folder contains the docker-compose files to run the mqtt-broker and the database.
##### 3. esp32-firmware
This folder contains the .ino file to be uploaded to the esp32.
##### 4. server
This folder contains the server code to receive the data from the topic `weatherData` of the mqtt-broker and store it in the database.
##### 5. 
##### 6. 

# Troubleshooting Commands
```bash
# send data to the broker
mosquitto_pub -h localhost -t weatherData -m '{"temperature": 22, "humidity": 50}'
# subscribe to the some topic of the broker
mosquitto_sub -h localhost -t weatherData
```

## How to Docker
This are some useful commands to manage docker containers:
```bash
# list all running containers
docker ps
# list all containers
docker ps -a
# stop a container
docker stop <container_id>
# remove a container
docker rm <container_id>
# stop all containers
docker stop $(docker ps -aq)
# remove all containers
docker rm $(docker ps -aq)
```
> Note: Due to we are using docker-compose, we don't need to remove the containers manually, just run `docker-compose down` to remove all containers and networks.

## References
[mosquitto docker](https://hub.docker.com/_/eclipse-mosquitto/)
