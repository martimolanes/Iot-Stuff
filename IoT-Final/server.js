// Import required modules
const express = require('express');
const mqtt = require('mqtt');
const { MongoClient } = require('mongodb');

// Create an Express application
const app = express();

// MQTT Broker configuration
const mqttBroker = 'mqtt://172.20.49.49'; // Replace with your MQTT broker address
const mqttClient = mqtt.connect(mqttBroker);

// MongoDB configuration
const mongoURI = 'mongodb+srv://Lawunn:BlackPink1@cluster.1qhaeok.mongodb.net/Weather'; // Replace with your MongoDB connection URI
const dbName = 'Weather'; // Name of your MongoDB database

// Connect to MongoDB
MongoClient.connect(mongoURI, { useNewUrlParser: true, useUnifiedTopology: true })
    .then(client => {
        console.log('Connected to MongoDB');
        const db = client.db(dbName);

        // MQTT subscription
        mqttClient.on('connect', () => {
            console.log('Connected to MQTT broker');
            mqttClient.subscribe('weatherData');
        });

        mqttClient.on('message', (topic, message) => {
            console.log('Received message from MQTT:', message.toString());
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
    })
    .catch(err => {
        console.error('Failed to connect to MongoDB:', err);
    });

// Start the Express server
const port = 3000; // Port for your web server
app.listen(port, () => {
    console.log(`Server is running on port ${port}`);
});
