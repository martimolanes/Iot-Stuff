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
    echo "    $0 server           - start server"
    echo "    $0 api              - start api"
    echo "  test scripts:"
    echo "    $0 publish-mqtt     - mock esp32 arduino"
    echo "  stop:"
    echo "    $0 stop             - stop all docker-services"
}

function start_mqtt() {
    echo "Starting mqtt service"
    docker-compose -f ./mqtt-broker/mqtt-broker.yml up -d
}

function start_database() {
    echo "Starting database service"
    docker-compose -f ./influx-db/influx.yml up -d
}

function start_server() {
    echo "Starting server"
    cd server
    npm ci
    npm start
}

function start_api() {
    echo "Starting api"
    docker build -t go-api-weather-station api/
    docker run --rm --name go-api --network="host" -d go-api-weather-station
}

function publish_mqtt() {
    bash ./docs/examples/publisherMQTT.sh
}

function stop_services() {
    echo "Stopping all services"
    docker-compose -f ./mqtt-broker/mqtt-broker.yml down
    docker-compose -f ./influx-db/influx.yml down
}

# Check if the script is run from the git root directory
if [[ $PWD != $(git rev-parse --show-toplevel) ]]; then
    echo "This script must be run from the root directory of the repository"
    exit 1
fi

command=${1:-}

case $command in
    mqtt-broker) start_mqtt ;;
    db) start_database ;;
    server) start_server ;;
    api) start_api ;;
    publish-mqtt) publish_mqtt ;;
    stop) stop_services ;;
    -h) help ;;
    --help) help ;;
    *) error_command ;;
esac
