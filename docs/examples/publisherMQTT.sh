#!/bin/bash
# Description: This script is used to mock publishing on `weatherData` to the MQTT broker

while true
do
    TEMPERATURE=$(( ( RANDOM % 4 )  + 20 ))
    HUMIDITY=$(( ( RANDOM % 7 )  + 60 ))
    echo "Publishing temperature: $TEMPERATURE, humidity: $HUMIDITY"
    mosquitto_pub -h localhost -t weatherData -m '{"temperature":'$TEMPERATURE',"humidity":'$HUMIDITY'}'
    sleep 3
done
