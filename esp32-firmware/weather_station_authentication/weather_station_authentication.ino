#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <SPI.h>
#include <MFRC522.h>

// WiFi configuration
const char* ssid = "MB210-G";
const char* password = "studentMAMK";

// MQTT Server IP address
const char* mqtt_server = "172.20.49.52"; // Network IP address

WiFiClient espClient; // WiFi client
PubSubClient mqttClient(espClient); // MQTT client

// DHT sensor settings
#define DHTPIN 27
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// RFID settings
#define SS_PIN 21
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

bool isAuthorized = false;

// Measurement struct definition for linked list
struct measurement {
    float temperature;
    float humidity;
    struct measurement *next;
};

struct avgMeasurements {
  float Temp;
  float Hum;
};

// Struct pointers for linked list
struct measurement *firstMeasurement; 
struct measurement *currentMeasurement;
int amountOfMeasurements = 5;
int measurementCounter = 0;

void setup() {
    Serial.begin(9600);
    SPI.begin(); // Initiate SPI bus
    dht.begin(); // Initiate DHT sensor
    mfrc522.PCD_Init(); // Initiate MFRC522
    setup_wifi(); // Connect to WiFi
    mqttClient.setServer(mqtt_server, 1883); // Set MQTT server
    Serial.println("Please insert a card to the reader for authorization.");

    // Initialize linked list
    firstMeasurement = (struct measurement*) malloc(sizeof(struct measurement));
    firstMeasurement->next = NULL;
    currentMeasurement = firstMeasurement;
}

void setup_wifi() {
    delay(10);
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

void reconnect() {
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (mqttClient.connect("ESP32_Client")) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}


bool isCardAuthorized() {
    String readUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        readUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        readUID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    readUID.toUpperCase();
    // UID of authorized card
    if (readUID == " 32 C7 E5 1B") {
        return true;
    }
    return false;
}

/*
 * Add a new measurement to the linked list
 * If the list is full (only 5 measures stored), the oldest measurement is overwritten
 */
void addMeasurement(float temperature, float humidity) {
    currentMeasurement->temperature = temperature;
    currentMeasurement->humidity = humidity;
    measurementCounter += 1;

    if (measurementCounter == amountOfMeasurements) {
        currentMeasurement = firstMeasurement;
        measurementCounter = 0;
    } else {
        if (!currentMeasurement->next) {
            struct measurement *nextMeasurement = (struct measurement*) malloc(sizeof(struct measurement));
            // Initialize new node
            nextMeasurement->temperature = 0;
            nextMeasurement->humidity = 0;
            nextMeasurement->next = NULL; // Ensure the new node points to NULL
            currentMeasurement->next = nextMeasurement;
            currentMeasurement = nextMeasurement;
        } else {
            currentMeasurement = currentMeasurement->next;
        }
    }
}

void calculateAverage(struct avgMeasurements* avgMeasurements) {
    struct measurement* measurement = firstMeasurement;
    float tempAccum = 0;
    float humAccum = 0;
    int count = 0;

    while (measurement && measurement->humidity != 0) {
        tempAccum += measurement->temperature;
        humAccum += measurement->humidity;
        measurement = measurement->next;
        count++;
    }

    if (count > 0) {
        avgMeasurements->Temp = tempAccum / count;
        avgMeasurements->Hum = humAccum / count;
    }
}

void loop() {
    if (!mqttClient.connected()) {
        reconnect();
    }
    mqttClient.loop();

    // If not authorized, keep checking for a card
    if (!isAuthorized) {
      
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
            if (isCardAuthorized()) {
                Serial.println("Card authorized successfully!");
                Serial.println("--- --- --- ---");
                isAuthorized = true;
            } else {
                Serial.println("Unauthorized card. Please try again.");
                delay(2000); // Add a delay to prevent immediate re-prompt
            }
        }
    } else { // Authorized - Proceed with temperature measurement and publication
        float measuredTemp = dht.readTemperature();
        float measuredHum = dht.readHumidity();
        Serial.print("Temperature: ");
        Serial.println(measuredTemp);
        Serial.print("Humidity: ");
        Serial.println(measuredHum);

        addMeasurement(measuredTemp, measuredHum);
        struct avgMeasurements averageMeasurement = { 
          0.0, 0.0
        };
        calculateAverage(&averageMeasurement);
        char strBuf[50];
        sprintf(strBuf, "\ttemperature: %.2f\n\thumidity: %.2f", averageMeasurement.Temp, averageMeasurement.Hum);
        Serial.println("Average measurement: ");
        Serial.println(strBuf);

        char tempString[50];
        //{"temperature": $TEMPERATURE, "humidity": $HUMIDITY, "location": "$LOCATION"}
        sprintf(tempString, "{\"temperature\": %.2f, \"humidity\": %.2f, \"location\": \"%s\"}", averageMeasurement.Temp, averageMeasurement.Hum, "office");
        mqttClient.publish("weatherData", tempString);

        delay(2000); // Delay between measurements
    }
}
