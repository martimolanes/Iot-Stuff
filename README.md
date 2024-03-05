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
├── influx-db
│   └── influx.yml
├── mqtt-broker
│   ├── mosquitto.conf
│   └── mqtt-broker.yml
├── server
│   ├── package.json
│   └── server.js
└── start-service.sh
```
##### 1. influx-db
Contains the docker-compose file to run the database for the architectures amd64 and arm64v8.
##### 2. mqtt-broker
Contains the docker-compose file to run the mqtt-broker. It has the mosquitto configuration file also.
##### 3. esp32-firmware
Contains the .ino file to be uploaded to the esp32. The firmware is responsible to read the data from the sensors and publish it to the mqtt-broker.
##### 4. server
Contains the server code to read the data from the topic `weatherData` from the mqtt-broker and store it in the database.
##### 5. api
Contains the api code to get the data from the database and show it in the route `/weather` of the server.
##### 6. docs
Contains some useful scripts to test the system.
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
> Note: For mocking the esp32, we are using the `publisherMQTT.sh` script in the `docs/examples` directory.

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
# stop all containers in the system
docker stop $(docker ps -aq)
# remove all containers in the system (use with caution)
docker rm $(docker ps -aq)
# see logs of a container
docker logs <container_id>
```
> We are using docker-compose to manage the containers and we are using the `start-service.sh` to run the docker-compose files and stop them. Try `start-service.sh -h`.

This two are useful for deleting the configurations of the influxDB:
```bash
# see the volumes
docker volume ls
# remove a volume. In this case, influx-db_influx-config influx-db_influx-data.
docker volume rm <volume_name>
```

## References
> Structure generated with command `tree -I node_modules -I package-lock.json -I mongodb_data -I README.md`

[mosquitto docker](https://hub.docker.com/_/eclipse-mosquitto/)

[influxDB docs](https://docs.influxdata.com/influxdb/v2/)

[influxDB docker](https://hub.docker.com/_/influxdb)

[grafana docs](https://grafana.com/docs/grafana/latest/)

[grafana docker](https://hub.docker.com/r/grafana/grafana)
