#ifndef CLOUDSWITCH_CONFIG_H
#define CLOUDSWITCH_CONFIG_H

/**
 * WiFi setup
 */
#define WLAN_SSID       "Wifi_Moulin"
#define WLAN_PASS       "BaptisteEtPauline"

/**
 * MQTT setup
 */
#define MY_MQTT_SERVER      "192.168.0.47"
#define MY_MQTT_SERVERPORT  1883                   // use 8883 for SSL
#define MY_MQTT_USERNAME    ""
#define MY_MQTT_KEY         ""

#define SENSORS_TOPIC_ALL_SUB       "/test123/#"
#define SENSORS_TOPIC_HAUT_SUB       "/sensors/haut"
#define SENSORS_TOPIC_SALON_SUB       "/sensors/salon"
#define SENSORS_TOPIC_EXT_SUB       "/sensors/ext"

/**
 * REFERENCE SETUP
 */
#define SEUIL_CO2_WARN       700
#define SEUIL_CO2_DANGER     900

#define SEUIL_HUMIDITY_WARN       65
#define SEUIL_HUMIDITY_DANGER     75

#endif 
