import express from "express";
import mqtt from "mqtt";
import { InfluxDB, Point } from '@influxdata/influxdb-client'
import dotenv from "dotenv";

dotenv.config();
// Create an Express application
const app = express();

const PORT = 6969;
const TOPIC_NAME = "weatherData";

// InfluxDB configuration
const url = process.env.INFLUX_URL;
const token = process.env.INFLUX_TOKEN;
const org = process.env.INFLUX_ORG;
const bucket = process.env.INFLUX_BUCKET;

const influxDB = new InfluxDB({ url, token });

const writeApi = influxDB.getWriteApi(org, bucket);

// MQTT Broker configuration
const MQTT_BROKER = "mqtt://localhost:1883";

console.log("Listening to MQTT Broker: ", MQTT_BROKER);
const mqttClient = mqtt.connect(MQTT_BROKER);

const subscribeToMQTTTopic = (topic) => {
    mqttClient.subscribe(topic);
    console.log("Subscribed to topic: ", topic);
    mqttClient.on("message", handleMQTTMessage);
};

const handleMQTTMessage = (topic, message) => {
    console.log(
        "Received message from topic ",
        topic + ": " + message.toString(),
    );
    // TODO: Check message format
    const weatherData = JSON.parse(message);
    saveToDatabase(weatherData);
};

const saveToDatabase = (weatherData) => {
    const point = new Point("weather")
        .tag("location", weatherData.location)
        .floatField("temperature", weatherData.temperature)
        .floatField("humidity", weatherData.humidity)
    console.log("Writing to InfluxDB: ", point.toLineProtocol());
    writeApi.writePoint(point);
};

function init() {
    subscribeToMQTTTopic(TOPIC_NAME);
    // Start the Express server
    app.listen(PORT, () => {
        console.log(`Server is running on port ${PORT}`);
    });
}

init();
