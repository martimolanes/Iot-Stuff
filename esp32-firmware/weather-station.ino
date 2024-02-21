// DHT - MQTT - Wifi
// This contains only temperature. Humidity is not used.

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Wifi settings
const char* ssid = "MB210-G";
const char* password = "studentMAMK";

// MQTT Server IP address
const char* mqtt_server = "172.20.49.12";

// WiFi client
WiFiClient espClient;
// MQTT client
PubSubClient mqttClient(espClient);

// DHT sensor settings
#define DHTPIN 27
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

/* Variables for tracking the measurements for average calculation */
int amountOfMeasurements = 5;
int measurementCounter = 0;

/* Measurement struct definition for linked list */
struct measurement {
	float temperature;
	struct measurement *next;
} ;

/* Struct pointer that is used to store the firstMeasurement (begining of the linked list) */
struct measurement *firstMeasurement; 
/* Struct pointer that is used to store the current position of the list */ 
struct measurement *currentMeasurement;

// Application setup
void setup() {
  // Serial
  Serial.begin(9600);
  // DHT temperature sensor
  dht.begin();
  // WiFi connection
  setup_wifi();
  // MQTT
  mqttClient.setServer(mqtt_server, 1883);

  /* Create the first linked list item */
	firstMeasurement = (struct measurement *) malloc(sizeof(struct measurement));
  /* Ensure that the next address is still empty */
  firstMeasurement->next = NULL;
	currentMeasurement = firstMeasurement;

  Serial.println("Using linked list for calculating the sliding average.");
}

// WiFi connection
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to WiFi SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT connection
void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("ESP32_Client")) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/* Function that is used to add or update new measurement in the list */
void addMeasurement(float temperature) {
	currentMeasurement->temperature = temperature;

	/* Update position */
	measurementCounter += 1;
	/* Start again from the first item is there are enough measurements */
	if (measurementCounter == amountOfMeasurements) {
		currentMeasurement = firstMeasurement;
		measurementCounter = 0;
	}
	else {
		if (!currentMeasurement->next) {
			struct measurement *nextMeasurement;
			nextMeasurement = (struct measurement *) malloc(sizeof(struct measurement));
			currentMeasurement->next = nextMeasurement;
			currentMeasurement = currentMeasurement->next;
      /* Ensure that the next address is still empty */
      currentMeasurement->next = NULL;
		}
		else {
			currentMeasurement = currentMeasurement->next;
		}
	}
}

/* Function to calculate the average of measured temperatures */
float calculateAverage() {
	float avgTemp = 0.0;
	/* Struct pointer for average calculation */
	struct measurement *avgMeasurement;
	avgMeasurement = firstMeasurement;
	/* Read the items and sum temperatures. */
	while(avgMeasurement) {
		avgTemp += avgMeasurement->temperature;
		avgMeasurement = avgMeasurement->next;
	}
	/* Calculate the average temp */
	avgTemp = avgTemp / amountOfMeasurements;
	return(avgTemp);
}

// Application main loop
void loop() {
  // Reconnect mqtt if connection has been lost
  if (!mqttClient.connected()) {
    reconnect();
  }

  // Variable for measured temperature
  float measuredTemp = 0.0;

  // Read temperature from sensor
  measuredTemp = dht.readTemperature();

  // Print measured temperature to serial console
  Serial.print("Temperature: ");
  Serial.println(measuredTemp);

  // Variable for average temperature
  float averageTemp = 0.0;
  // Add measurement to list, calculate average temp and send it to mqtt
  addMeasurement(measuredTemp);
  averageTemp = calculateAverage();

  // Print measured temperature to serial console
  Serial.print("Average (linked list used): ");
  Serial.println(averageTemp);

  // Convert the average temperature value to a char array and publish it to MQTT
  char tempString[8];
  dtostrf(averageTemp, 1, 2, tempString);
  mqttClient.publish("esp32/temperature", tempString);

  // Sleep X seconds before next measurement
  sleep(2);
}