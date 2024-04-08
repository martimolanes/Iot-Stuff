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

This project represents a comprehensive exploration into the realm of IoT,
integrating hardware, software, and networking components to create a flexible
and deployable solution for `Weather Monitoring`.

The core components of the system include an ESP32 microcontroller,
temperature and humidity sensors (DHT22/DHT11), RFIC RC522 sensor,
an MQTT broker server, and a self-made web API.
These elements work in concert to collect real-time environmental data,
analyze it, and make it visible to end users via a
visualization of the measurements using Grafana.

Furthermore, the project has features such as RFID authentication,
enabling the activation of the weather station only by authorized users.

## Table of Contents

1. [System Architecture](#system-architecture)
    - [ESP32 & DHT11](#esp32--dht11)
    - [MQTT Broker](#mqtt-broker)
    - [Server](#server)
    - [API](#api)
    - [InfluxDB](#influxdb)
    - [Grafana](#grafana)
    - [Docker](#docker)
2. [Deployment](#deployment)
3. [Usage](#usage)
    - [Pre-requisites](#pre-requisites)
    - [How to Run](#how-to-run)
4. [References](#references)

## System Architecture

Our system architecture is specifically designed with modularity and scalability
as core principles. This means that each component within the system serves a
unique function, ensuring that different parts of the project
(_e.g. API or MQTT Broker_), can be developed, tested, and deployed independently.

The design of each component allows it to operate autonomously while maintaining
the ability to communicate with others as necessary. This approach not only
enhances flexibility but also boosts scalability.
It provides the option to deploy each component on a separate machine if required,
further expanding the system's adaptability.

In essence, our architecture's primary goal is to promote independent development
and deployment, facilitating a more efficient and flexible system that can scale
according to the project's needs.

Here's a visual representation of the system architecture:

![System Architecture](./docs/images/system-architecture.png)

The architecture is as follows:

### ESP32 & DHT11

The _ESP32_ is a series of low-cost, low-power system on a chip microcontrollers
with integrated Wi-Fi and Bluetooth.
The _ESP32_ series employs a Tensilica Xtensa LX6 microprocessor in both dual-core
and single-core variations and includes built-in antenna switches, multiple GPIOs,
a wide range of communication protocols such as SPI, I2C, I2S interfaces and UART;
power amplifier, low-noise receive amplifier, filters, and power-management modules.

This is the hardware component of the weather station.

> Note : Deploy multiple ESP32s in different locations to collect weather data
> from various sources, is possible with this architecture.

The _DHT11_ is a basic, ultra low-cost digital temperature and humidity sensor.
It uses a capacitive humidity sensor and a thermistor
to measure the surrounding air, and spits out a digital signal on the data pin.

The _DHT11_ can measure temperature from 0°C to 50°C with a ±2°C accuracy,
and humidity from 20% to 80% with a ±5% accuracy.
The sensor operates on a 3 to 5.5V power supply and consumes very low power

So, the combination of an _ESP32 & DHT11_, collects temperature and humidity data
and publishes it to the MQTT Broker.

In the project, there is a specific folder (`esp32-firmware`)
that contains the code to be uploaded to the ESP32.

The firmware for the ESP32 is written in C++ (_Arduino_) and is responsible
for reading data from the DHT11 sensor and publishing it to the MQTT broker.
The firmware also includes code for handling RFID authentication.

### MQTT Broker

**MQTT** (_Message Queeting Telemetry Transport_) is a lightweight messaging protocol
that is designed for situations where a **small code footprint** is required
and **network bandwidth** is **limited**.

This is the **messaging hub** of the system. It receives the **data published**
by the ESP32 and forwards it to any interested subscribers (our Server).
The firmware of the ESP32 is configured to publish the data by default
to the topic `weatherData`.
The MQTT Broker used in this project is Eclipse Mosquitto, due to is
open-source, lightweight and easy to use. The MQTT Broker is encapsulated in a Docker
container for easy deployment.

### Server

This is a simple **NodeJS** application that **reads the weather** data from
the **MQTT Broker** and for each measurement, it makes a **write** in the Database.

It uses the **MQTT.js** library to subscribe to the MQTT Broker and the
**InfluxDB** library to write the data to the InfluxDB database.

In this project, the server could be substituted by
[**Telegraf**](https://www.influxdata.com/time-series-platform/telegraf/)
a **Go** agent that collects, processes, aggregates, and writes metrics. But
for learning purposes, of how InfluxDB works and how to read from a MQTT Broker,
we decided to use a NodeJS server.

### API

This API is written in [Go](https://go.dev/) and is the **interface** between
the Server and the end-users.

This API target is to expose to the end-user or to a web client application
the data stored in our database, in a more
well-structure, centralized and processed way.

For achieving this, we use a query language called
[Flux](https://docs.influxdata.com/flux/v0/) that
is an open source functional data scripting language designed for querying,
analyzing, and acting on data supporting Time Series Databases.

The API is encapsulated in a Docker container built from the source code using a
multi-stage build.

A multi-stage build is a feature of Docker that allows you to
use multiple build stages in a single Dockerfile. This is useful for creating
really final small images where you can copy the necessary artifacts from the
previous build stage.

In this project, this multi-stage build decreased size of the image from
`928MB` to `16MB`.

### InfluxDB

[InfluxDB](https://www.influxdata.com/) is an open-source lightweight
time series database (TSDB) that is used to store the weather data received
from the server.

### Grafana

[Grafana](https://grafana.com/) is an open-source platform for monitoring
and observability. It allows you to query, visualize, alert on, and understand
your metrics no matter where they are stored. In this project, Grafana is used
to visualize the weather data stored in the InfluxDB database.

### Docker

Docker is a platform that allows developers to package applications into
containers — standardized executable components that combine application
source code with every dependency required to run it - ensuring that the
application behaves the same way, regardless of where it is deployed.

This is the deployment tool used for the system.
Each component (except the ESP32) is encapsulated in a Docker container,
making the system easy to deploy and scale.

This architecture also allows for easy troubleshooting and maintenance.
If one component fails, the others can continue to operate independently.
The failed component can be repaired or replaced without affecting the rest
of the system.

## Deployment

Deployment of the system is made easy with the use of Docker and Docker-compose.
Each component (except the ESP32) is encapsulated in a Docker container,
allowing for easy deployment and scalability.
The `start-service.sh` script is used to start all the components of the system.

## Usage

### Pre-requisites

In order to run the system, the following prerequisites are required:

1. Docker and Docker-compose installed on the machine.
2. Platform to run the ESP32 firmware (Arduino IDE, CLI or similar).

### How to Run

The system is started using the `start-service.sh` script.

This concludes the technical overview of the Scalable Weather Station for ESP32
with DHT11. The system is designed to be flexible, scalable, and easy to deploy,
making it an ideal solution for weather monitoring applications.

## References

1. [Eclipse Mosquitto](https://hub.docker.com/_/eclipse-mosquitto/)
2. [InfluxDB Documentation](https://docs.influxdata.com/influxdb/v2/)
3. [InfluxDB Docker](https://hub.docker.com/_/influxdb)
4. [Grafana Documentation](https://grafana.com/docs/grafana/latest/)
5. [Grafana Docker](https://hub.docker.com/r/grafana/grafana)
6. [DHT11–Temperature and Humidity Sensor Specificacions](https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)
7. [ESP32 Technical Reference Manual](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/index.html)
8. [RFID RC522 Datasheet](https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf)
9. [NodeJS MQTT.js](https://www.npmjs.com/package/mqtt)
10. [NodeJS InfluxDB](https://www.npmjs.com/package/influx)
11. [Flux Documentation](https://docs.influxdata.com/flux/v0/)
12. [Go Programming Language](https://go.dev/)
13. [Telegraf](https://www.influxdata.com/time-series-platform/telegraf/)
14. [Docker Documentation](https://docs.docker.com/)
15. [Docker-compose Documentation](https://docs.docker.com/compose/)
16. [Arduino IDE](https://www.arduino.cc/en/software)
17. [Arduino CLI](https://arduino.github.io/arduino-cli/latest/)

---

This document was created from the `docs/REPORT.md` from this
[github repository](https://github.com/martimolanes/Iot-Stuff/tree/main).
