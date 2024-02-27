#!/bin/bash
# Description: This script is used to mock publishing on `weatherData` to the MQTT broker

while true
do
    TEMPERATURE=$(( ( RANDOM % 4 )  + 20 ))
    HUMIDITY=$(( ( RANDOM % 7 )  + 60 ))
    LOCATION="office"
    TOPIC="weatherData"
    echo "Publishing temperature: $TEMPERATURE, humidity: $HUMIDITY to topic: $TOPIC"
    mosquitto_pub -h localhost -t $TOPIC -m "{\"temperature\": $TEMPERATURE, \"humidity\": $HUMIDITY, \"location\": \"$LOCATION\"}"
    sleep 5
done
