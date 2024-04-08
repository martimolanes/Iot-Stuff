#!/bin/bash

function error_command() {
    echo "no such command $command"
    help
}

function help() {
    echo "usage:"
    echo "  run:"
    echo "    $0 mqtt-broker      - start mqtt service"
    echo "    $0 db               - start database service with visualization (influxdb, grafana)"
    echo "    $0 server           - start server (write data to influxdb)"
    echo "    $0 api              - start api (expose data from influxdb)"
    echo "  test scripts:"
    echo "    $0 publish-mqtt     - mock esp32 arduino (publish data to mqtt broker)"
    echo "  stop:"
    echo "    $0 stop             - stop all project services"
    echo "  docs:"
    echo "    $0 build-docs       - build project overview (report.pdf)"
}

function start_mqtt() {
    source ./mqtt-broker/.env 2>/dev/null
    echo "Starting mqtt service"
    docker-compose -f ./mqtt-broker/mqtt-broker.yml up -d
    echo "----------------------------------------"
    echo "Accessing the Mosquitto:"
    echo "----------------------------------------"
    echo "  - Access the mosquitto at http://localhost:${MQTT_PORT:-1883}"
}

function start_database() {
    source ./influx-grafana/.env 2>/dev/null
    echo "Starting influxdb and grafana services"
    docker-compose -f ./influx-grafana/influx-grafana.yml up -d
    echo "----------------------------------------"
    echo "Accessing the services:"
    echo "----------------------------------------"
    echo "InfluxDB:"
    echo "  - Access the InfluxDB at http://localhost:${INFLUXDB_PORT:-8086}"
    echo "  - Username: ${INFLUXDB_USER:-my-user}"
    echo "  - Password: ${INFLUXDB_PASSWORD:-my-password}"
    echo "----------------------------------------"
    echo "Grafana:"
    echo "  - Access Grafana at http://localhost:${GRAFANA_PORT:-3000}"
    echo "----------------------------------------"
}

function start_server() {
    echo "Starting server"
    echo "Data is readed from mqtt-broker and written to influxdb"
    cd server
    npm ci
    npm start
}

function start_api() {
    echo "Starting weather api"
    docker build -t go-api-weather-station api/
    docker run --rm --name go-api --network="host" -d go-api-weather-station
}

function publish_mqtt() {
    bash ./docs/examples/publisherMQTT.sh
}

function stop_services() {
    echo "Stopping all weather station services"
    docker-compose -f ./mqtt-broker/mqtt-broker.yml down
    docker-compose -f ./influx-grafana/influx-grafana.yml down
}

# Check if the script is run from the git root directory
if [[ $PWD != $(git rev-parse --show-toplevel) ]]; then
    echo "This script must be run from the root directory of the repository"
    exit 1
fi

# Check if command is empty
if [ $# -eq 0 ]; then
    help
    exit 1
fi

for command in "$@"; do
    case $command in
        mqtt-broker) start_mqtt ;;
        db) start_database ;;
        server) start_server ;;
        api) start_api ;;
        publish-mqtt) publish_mqtt ;;
        stop) stop_services ;;
        -h) help ;;
        --help) help ;;
        build-docs)  pandoc -f markdown docs/REPORT.md -s -o report.pdf;;
        *) error_command ;;
    esac
done
