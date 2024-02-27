!#/bin/bash

function error_command() {
    echo "no such command $command"
    help
}

function help() {
    echo "usage:"
    echo "  $0 mqtt-broker      - start mqtt service"
    echo "  $0 db               - start database service for x86_64 (default)"
    echo "  $0 db:arm           - start database service for arm64v8 (Mac M1)"
    echo "  $0 server           - start server"
    echo "  $0 api              - start api"
}

function start_mqtt() {
    echo "Starting mqtt service"
    sudo docker-compose -f ./mqtt-broker/mqtt-broker.yml up -d
}

function start_database() {
    echo "Starting database service"
    sudo docker-compose -f ./mongo-db/mongo.yml up -d
}

function start_database_arm() {
    echo "Starting database service"
    sudo docker-compose -f ./mongo-db/mongo-arm64v8.yml up -d
}

function start_server() {
    echo "Starting server"
    cd server
    npm ci
    npm start
}

function start_api() {
    echo "Starting api"
    cd api
    npm ci
    npm start
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
    db:arm) start_database_arm ;;
    server) start_server ;;
    api) start_api ;;
    -h) help ;;
    --help) help ;;
    *) error_command ;;
esac
