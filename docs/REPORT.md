---
title: Scalable Weather Station for ESP32 with DHT11
output: 
  pdf_document:
    latex_engine: pdflatex
    toc: true
    toc_depth: 2
    number_sections: true
geometry: margin=1in
fontsize: 11pt
linestretch: 1.2
---

## Introduction

This project represents a comprehensive exploration into the realm of IoT, integrating hardware, software, and networking components to create a flexible and deployable solution for weather monitoring. The core components of the system include an ESP32 microcontroller, temperature and humidity sensors (DHT22/DHT11), RFIC RC522 sensor,an MQTT broker server, and a self-made web server. These elements work in concert to collect real-time environmental data, analyze it, and make it visible to end users via a web interface. Furthermore, the project has advanced features such as RFID authentication, enabling enhanced security and control over access to the weather station.

## Table of Contents

1. [System Architecture](#system-architecture)
2. [Components](#components)
    - [Docker](#docker)
    - [ESP32 Firmware](#esp32-firmware)
    - [MQTT Broker](#mqtt-broker)
    - [Server](#server)
    - [API](#api)
3. [Deployment](#deployment)
4. [Usage](#usage)
    - [Pre-requisites](#pre-requisites)
    - [How to Run](#how-to-run)
    - [Troubleshooting](#troubleshooting)
5. [References](#references)

## System Architecture

The system is designed to be modular and scalable, with each component serving
a specific function. The target of this architecture is that each part of the
project (_Ex. API, MQTT Broker_)can be deployable in different machines.
The architecture is as follows:

1. ESP32 with DHT11 Sensor:
This is the hardware component of the weather station.
You can deploy multiple ESP32s in different locations to collect weather data
from various sources.
It collects temperature and humidity data and publishes it to the MQTT Broker.
In the project, there is a specific folder (`esp32-firmware`)
that contains the code to be uploaded to the ESP32.

2. MQTT Broker:
This is the messaging hub of the system. It receives the data published
by the ESP32 and forwards it to any interested subscribers
(in this case, the Server).

3. Server:
This is the data processing component of the system.
It subscribes to the MQTT Broker and receives the weather data.
The server then processes this data and stores it in a database for future use.

4. API:
This is the interface between the Server and the end users.
It retrieves the weather data from the database
and presents it in a user-friendly format.

5. Docker:
This is the deployment tool used for the system.
Each component (except the ESP32) is encapsulated in a Docker container,
making the system easy to deploy and scale.

6. Grafana and InfluxDB:
These tools are used for data visualization and storage respectively.
InfluxDB stores the weather data received from the server,
and Grafana provides a graphical interface for viewing this data.

Here's a visual representation of the system architecture:

![System Architecture](./docs/images/system-architecture.png)

Each component is designed to operate independently, communicating with each other as needed. This allows for great flexibility and scalability, as each component can be deployed on a separate machine, if needed.

This architecture also allows for easy troubleshooting and maintenance. If one component fails, the others can continue to operate independently. The failed component can be repaired or replaced without affecting the rest of the system.

In the following sections, each component will be discussed in more detail.


## Components

### Docker

Docker is a platform that allows developers to package applications into containers — standardized executable components that combine application source code with all the related configuration files, libraries and dependencies required for it to run. This ensures that the application behaves the same way, regardless of where it is deployed.

In this project, Docker is used to containerize the MQTT Broker, Server, and API. Each of these components is defined in a Dockerfile and can be easily deployed using Docker commands. Docker-compose is used to manage the multi-container application.

### ESP32 Firmware

The ESP32 is a series of low-cost, low-power system on a chip microcontrollers with integrated Wi-Fi and dual-mode Bluetooth. The ESP32 series employs a Tensilica Xtensa LX6 microprocessor in both dual-core and single-core variations and includes built-in antenna switches, RF balun, power amplifier, low-noise receive amplifier, filters, and power-management modules.

The firmware for the ESP32 is written in C++ and is responsible for reading data from the DHT11 sensor and publishing it to the MQTT broker. The firmware also includes code for handling RFID authentication.

### MQTT Broker

The MQTT Broker is the central hub of the system, receiving data from the ESP32 and forwarding it to the server. MQTT (Message Queeting Telemetry Transport) is a lightweight messaging protocol that is designed for situations where a small code footprint is required and network bandwidth is limited.

In this project, the MQTT Broker is implemented using Eclipse Mosquitto, an open-source MQTT broker that is easy to use and configure.

### Server

The server is responsible for processing the data received from the MQTT Broker. It is written in Node.js and uses the MQTT.js library to subscribe to the MQTT Broker and receive weather data. The server stores this data in an InfluxDB database for future use.

### API

The API is the interface between the server and the end users. It is written in Go and retrieves the weather data from the InfluxDB database. The API presents this data in a user-friendly format at the `/weather` endpoint of the server.

## Deployment

Deployment of the system is made easy with the use of Docker and Docker-compose. Each component (except the ESP32) is encapsulated in a Docker container, allowing for easy deployment and scalability. The `start-service.sh` script is used to start all the components of the system.

## Usage

### Pre-requisites

In order to run the system, the following prerequisites are required:

1. Docker and Docker-compose installed on the machine.
2. Platform to run the ESP32 firmware (Arduino IDE or similar).

### How to Run

The system is started using the `start-service.sh` script. This script uses Docker-compose to start the containers for the MQTT Broker, Server, and API. The script also provides options to start individual components, if needed.

### Troubleshooting

In case of any issues, the `docs/examples` directory contains useful scripts for testing the system. The `publisherMQTT.sh` script can be used to publish data to the MQTT broker, and the `mosquitto_sub` command can be used to subscribe to a topic on the broker.

## References

1. [Eclipse Mosquitto](https://hub.docker.com/_/eclipse-mosquitto/)
2. [InfluxDB Documentation](https://docs.influxdata.com/influxdb/v2/)
3. [InfluxDB Docker](https://hub.docker.com/_/influxdb)
4. [Grafana Documentation](https://grafana.com/docs/grafana/latest/)
5. [Grafana Docker](https://hub.docker.com/r/grafana/grafana)

This concludes the technical overview of the Scalable Weather Station for ESP32 with DHT11. The system is designed to be flexible, scalable, and easy to deploy, making it an ideal solution for weather monitoring applications.





