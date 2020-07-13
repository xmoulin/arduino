//
// Created by Andri Yadi on 10/29/16.
//

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <AzureIoTHubMQTTClient.h>

const char *AP_SSID = "Wifi_Moulin";
const char *AP_PASS = "BaptisteEtPauline";

// Azure IoT Hub Settings --> CHANGE THESE
#define IOTHUB_HOSTNAME         "cloudSwitchIoTHub.azure-devices.net"
#define DEVICE_ID               "cloudSwithAppareil1"
#define DEVICE_KEY              "AwljHcAyw2MDVMh7NEKFLevDZBoC1auMLEGyuAzEFz8=" //Primary key of the device

#define USE_BMP180              0 //Set this to 0 if you don't have the sensor and generate random sensor value to publish

WiFiClientSecure tlsClient;
AzureIoTHubMQTTClient client(tlsClient, IOTHUB_HOSTNAME, DEVICE_ID, DEVICE_KEY);
WiFiEventHandler  e1, e2;

#if USE_BMP180
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
#endif

const int LED_PIN = LED_BUILTIN; //Pin to turn on/of LED a command from IoT Hub
unsigned long lastMillis = 0;

void connectToIoTHub(); // <- predefine connectToIoTHub() for setup()
void onMessageCallback(const MQTT::Publish& msg);

void onSTAGotIP(WiFiEventStationModeGotIP ipInfo) {
    Serial.printf("onSTAGotIP - Got IP: %s\r\n", ipInfo.ip.toString().c_str());

    //do connect upon WiFi connected
    connectToIoTHub();
}

void onSTADisconnected(WiFiEventStationModeDisconnected event_info) {
    Serial.printf("onSTADisconnected - Disconnected from SSID: %s\n", event_info.ssid.c_str());
    Serial.printf("onSTADisconnected - Reason: %d\n", event_info.reason);
}

void onClientEvent(const AzureIoTHubMQTTClient::AzureIoTHubMQTTClientEvent event) {
    Serial.println("onClientEvent - begin");
    Serial.println(event);
    if (event == AzureIoTHubMQTTClient::AzureIoTHubMQTTClientEventConnected) {

        Serial.println("onClientEvent - Connected to Azure IoT Hub");

        //Add the callback to process cloud-to-device message/command
        client.onMessage(onMessageCallback);
    }
}

void onActivateRelayCommand(String cmdName, JsonVariant jsonValue) {
    Serial.println("onActivateRelayCommand - begin");
    //Parse cloud-to-device message JSON. In this example, I send the command message with following format:
    //{"Name":"ActivateRelay","Parameters":{"Activated":0}}

    const JsonObject& jsonObject = jsonValue.as<JsonObject>();
    if (jsonObject.containsKey("Parameters")) {
        auto params = jsonValue["Parameters"];
        auto isAct = (params["Activated"]);
        if (isAct) {
            Serial.println("Activated true");
            digitalWrite(LED_PIN, HIGH); //visualize relay activation with the LED
        }
        else {
            Serial.println("Activated false");
            digitalWrite(LED_PIN, LOW);
        }
    }
}

void setup() {

    Serial.begin(115200);

    while(!Serial) {
        yield();
    }
    delay(2000);

    Serial.setDebugOutput(true);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

#if USE_BMP180
    if (bmp.begin()) {
        Serial.println("BMP INIT SUCCESS");
    }
#endif

    Serial.print("setup - Connecting to WiFi...");
    //Begin WiFi joining with provided Access Point name and password
    WiFi.begin(AP_SSID, AP_PASS);

    //Handle WiFi events
    e1 = WiFi.onStationModeGotIP(onSTAGotIP);// As soon WiFi is connected, start the Client
    e2 = WiFi.onStationModeDisconnected(onSTADisconnected);

    //Handle client events
    client.onEvent(onClientEvent);

    //Add command to handle and its handler
    //Command format is assumed like this: {"Name":"[COMMAND_NAME]","Parameters":[PARAMETERS_JSON_ARRAY]}
    client.onCloudCommand("ActivateRelay", onActivateRelayCommand);
}

void onMessageCallback(const MQTT::Publish& msg) {
    Serial.println("onMessageCallback - begin");
    //Handle Cloud to Device message by yourself.

    if (msg.payload_len() == 0) {
        return;
    }

    Serial.println(msg.payload_string());
}

void connectToIoTHub() {

    Serial.println("connectToIoTHub - Beginning Azure IoT Hub Client... ");
    if (client.begin()) {
        Serial.println("connectToIoTHub - OK");
    } else {
        Serial.println("connectToIoTHub - Could not connect to MQTT");
    }
}

void readSensor(float *temp, float *press) {
    Serial.println("readSensor - begin");
#if USE_BMP180
    *temp = bmp.readTemperature();
    *press = 1.0f*bmp.readPressure()/1000; //--> kilo
#else
    //If you don't have the sensor
    *temp = 20 + (rand() % 10 + 2);
    *press = 90 + (rand() % 8 + 2);
#endif

}

void loop() {
    Serial.println("loop - begin");
    //MUST CALL THIS in loop()
    client.run();

    if (client.connected()) {
        Serial.println("loop - client.connected");

        // Publish a message roughly every 3 second. Only after time is retrieved and set properly.
        if(millis() - lastMillis > 3000 && timeStatus() != timeNotSet) {
           Serial.println("loop - code metier");

            lastMillis = millis();

            //Read the actual temperature from sensor
            float temp, press;
            readSensor(&temp, &press);

            //Get current timestamp, using Time lib
            time_t currentTime = now();

            // You can do this to publish payload to IoT Hub
            String payload = "{\"DeviceId\":\"" + String(DEVICE_ID) + "\", \"MTemperature\":" + String(temp) + ", \"EventTime\":" + String(currentTime) + "}";
            Serial.println(payload);

            //client.publish(MQTT::Publish("devices/" + String(DEVICE_ID) + "/messages/events/", payload).set_qos(1));
            client.sendEvent(payload);
            

            //Or instead, use this more convenient way
            /*AzureIoTHubMQTTClient::KeyValueMap keyVal = {{"MTemperature", temp}, {"MPressure", press}, {"DeviceId", DEVICE_ID}, {"EventTime", currentTime}};
            client.sendEventWithKeyVal(keyVal);
            */
        }
    }
    else {

    }

    delay(10); // <- fixes some issues with WiFi stability
}
