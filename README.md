# Deployable Weather Station

## Structure

```bash
.
├── api
│   ├── Dockerfile
│   ├── go.mod
│   └── main.go
├── docs
│   └── examples
│       └── publisherMQTT.sh
├── esp32-firmware
│   └── weather_station_authentication
│       └── weather_station_authentication.ino
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

### Structure Explanation

#### 1. influx-db

Contains the docker-compose file to run the database for the architectures
amd64 and arm64v8.

#### 2. mqtt-broker

Contains the docker-compose file to run the mqtt-broker.
It has the mosquitto configuration file also.

#### 3. esp32-firmware

Contains the .ino file to be uploaded to the esp32.
The firmware is responsible to read the data from the sensors and publish it to
the mqtt-broker.

#### 4. server

Contains the server code to read the data from the topic `weatherData` from the
mqtt-broker and store it in the database.

#### 5. api

Contains the api code to get the data from the database and show it in the route
`/weather` of the server.

#### 6. docs

Contains some useful scripts to test the system:

- `publisherMQTT.sh`: script to publish data to the mqtt-broker.

#### 7. start-service

`start-service.sh` is a script to run every part of the project.

## How to Run

### Pre-requisites

- Docker and Docker-compose
- Platform to run the esp32 firmware

Then, we are using a shell script (`start-service.sh`) to run every part of the
project.
We have this project structured by parts, so we can run every part separately
(so, we can run it in different machines).

```bash
# run the help and see the options
./start-service.sh -h
```

> Note: the script uses docker-compose to run the containers.
So, if you cannot execute docker without sudo,
you need to add your user to the docker group.

```bash
sudo usermod -aG docker $USER
```

## Troubleshooting Commands

There is some useful scripts to help you in the direcotry `docs/examples`.

```bash
# send data to the broker to test the topic
mosquitto_pub -h localhost -t weatherData -m '{"temperature": 22, "humidity": 50}'
# subscribe to the some topic of the broker
mosquitto_sub -h localhost -t weatherData
```

> Note: For mocking the esp32, we are using the `publisherMQTT.sh` script in the
`docs/examples` directory.

## Miscelaneous

### Completions for ZSH

If you are using zsh, you can use the `_start-service` file to have
the completions for the `start-service.sh` script.

```bash
cp _start-service ~/.zfunc/
source ~/.zshrc
```

### Useful Queries

```sql
SELECT "temperature", "humidity" FROM "weather"
```

### How to Docker

This is a simple guide to help you with docker commands in the project.
[How to Docker](./docs/how-to-docker.md)
> There is no need to know docker to run the project. All is automated with the
`start-service.sh` script.

### Other How to's

- [How to InfluxDB](./docs/how-to-influxdb.md)
- [How to Grafana](./docs/how-to-grafana.md)

## References

> Structure generated with command
`tree
-I node_modules -I package-lock.json -I go.sum -I README.md
-I LICENSE -I _start-service -I TODO.md`

[mosquitto docker](https://hub.docker.com/_/eclipse-mosquitto/)

[influxDB docs](https://docs.influxdata.com/influxdb/v2/)

[influxDB docker](https://hub.docker.com/_/influxdb)

[grafana docs](https://grafana.com/docs/grafana/latest/)

[grafana docker](https://hub.docker.com/r/grafana/grafana)
