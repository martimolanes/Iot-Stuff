import express from 'express';
import mqtt from 'mqtt';
import { MongoClient } from 'mongodb';

// Create an Express application
const app = express();

const PORT=6969
// TODO: try using a time-series database like InfluxDB
const MONGO_URI="mongodb://root:example@localhost:27017"
const dbName = 'Weather';

// MQTT Broker configuration
const MQTT_BROKER = 'mqtt://172.20.49.49';
const mqttClient = mqtt.connect(MQTT_BROKER);

// Connect to MongoDB
MongoClient.connect(MONGO_URI).then(client => {
    console.log('Connected to MongoDB');
    const db = client.db(dbName);

    // MQTT subscription
    mqttClient.on('connect', () => {
        console.log('Connected to MQTT broker');
        mqttClient.subscribe('weatherData');
    });

    mqttClient.on('message', (topic, message) => {
        console.log('Received message from topic:', topic.toString(), message.toString());
        const data = JSON.parse(message);
        // Store data in MongoDB
        db.collection('data').insertOne(data, (err, result) => {
            if (err) {
                console.error('Failed to insert data into MongoDB:', err);
                return;
            }
            console.log('Data inserted into MongoDB:', result.ops);
        });
    });
}).catch(err => {
    console.error('Failed to connect to MongoDB:', err);
});

// Start the Express server
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
