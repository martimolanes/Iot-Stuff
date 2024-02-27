# Iot final Project

## Structure
```bash
.
├── api
│   ├── app.js
│   ├── package.json
│   └── routes
│       └── weather.js
├── docs
│   └── examples
│       └── publisherMQTT.sh
├── esp32-firmware
│   └── weather-station.ino
├── mongo-db
│   ├── mongo-arm64v8.yml
│   └── mongo.yml
├── mqtt-broker
│   ├── mosquitto.conf
│   └── mqtt-broker.yml
├── server
│   ├── package.json
│   └── server.js
└── start-service.sh
```
##### 1. mongo-db
This folder contains the docker-compose file to run the database for the architectures amd64 and arm64v8.
##### 2. mqtt-broker
This folder contains the docker-compose file to run the mqtt-broker.
##### 3. esp32-firmware
This folder contains the .ino file to be uploaded to the esp32.
##### 4. server
This folder contains the server code to receive the data from the topic `weatherData` of the mqtt-broker and store it in the database.
##### 5. api
This folder contains the api code to get the data from the database and show it in the route `/weather` of the server.
##### 6. web
This folder contains the web code to show the data from the api in a web page.
##### 7. docs
This folder contains some useful scripts to test the system.
- `publisherMQTT.sh`: script to publish data to the mqtt-broker.

## How to Run
### Pre-requisites
- Docker
- Docker-compose
- Platform to run the esp32 firmware

Then, we are using a shell script (`start-service.sh`) to run every part of the project. 
We have this project structured by parts, so we can run every part separately (so, we can run it in different machines).
```bash
# run the help and see the options
./start-service.sh -h
```

## Troubleshooting Commands
There is some useful scripts to help you in the direcotry `docs/examples`.
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

## References
> Structure generated with command `tree -I node_modules -I package-lock.json -I mongodb_data -I README.md`
[mosquitto docker](https://hub.docker.com/_/eclipse-mosquitto/)
