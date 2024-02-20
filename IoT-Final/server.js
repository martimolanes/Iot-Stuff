import express from 'express';
import mqtt from 'mqtt';
import { MongoClient } from 'mongodb';

// Create an Express application
const app = express();

const PORT=6969
// TODO: try using a time-series database like InfluxDB
const MONGO_URI="mongodb://root:example@localhost:27017"
const dbName = 'Weather';
const TOPIC_NAME = 'weatherData';

// MQTT Broker configuration
const MQTT_BROKER = 'mqtt://localhost:1883';
console.log('Listening to MQTT Broker: ', MQTT_BROKER);
const mqttClient = mqtt.connect(MQTT_BROKER);

let db;

// Connect to MongoDB
const connectToMongoDB = async () => {
    const client = new MongoClient(MONGO_URI);
    try {
        await client.connect();
        console.log('Connected to MongoDB in: ', MONGO_URI);
        db = client.db(dbName);
    } catch (err) {
        console.error('Failed to connect to MongoDB: ', err);
    }
}

const subscribeToMQTTTopic = (topic) => {
    mqttClient.subscribe(topic);
    console.log('Subscribed to topic: ', topic);
    mqttClient.on('message', handleMQTTMessage);
}

const handleMQTTMessage = (topic, message) => {
    console.log('Received message from topic ', topic + ': ' + message.toString());
    // TODO: Check message format
    const weatherData = JSON.parse(message);
    save(weatherData);
}

const save = async (weatherData) => {
    const collection = db.collection('weather');
    try {
        await collection.insertOne(weatherData);
        console.log('Saved to MongoDB: ', weatherData);
    } catch (err) {
        console.error('Failed to save to MongoDB: ', err);
    }
}

async function init() {
    await connectToMongoDB();
    subscribeToMQTTTopic(TOPIC_NAME);
    // Start the Express server
    app.listen(PORT, () => {
        console.log(`Server is running on port ${PORT}`);
    });
}

init();
