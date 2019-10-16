/*
 * To start mDNS monitor (OSX) exec:   dns-sd -B _arduino._tcp
 */

#include <EasyOTA.h>  // https://github.com/foxis/EasyOTA/blob/master/EasyOTA.h

#define WIFI_SSID        "your-wifi-ssid"
#define WIFI_PASSWORD    "your-wifi-password"
#define ARDUINO_HOSTNAME "ota-flash-demo"
EasyOTA OTA(ARDUINO_HOSTNAME);

void setup() {
	Serial.begin(115200);

	OTA.addAP(WIFI_SSID, WIFI_PASSWORD);

	OTA.onMessage([](const String& message, int line_nr) {
		Serial.println(message);
	});
	OTA.onConnect([](const String& ssid, EasyOTA::STATE state){
		Serial.println("Connection changed: " + ssid + ";" + String(state));
	});

	OTA.onScan([](const String& ssid, uint8_t sec, int32_t rssi, const uint8_t * BSSID, int32_t chan, bool hidden){
		Serial.println("Network found: " + ssid + "; " + String(chan) + "; " + String(rssi));
	});

	OTA.allowOpen(true);

	pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
	Serial.println("Starting...");
}

void loop() {
	static long last_m = millis();
	long now = millis();
	OTA.loop(now);

	if (now - last_m > 100)
	{
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		last_m = now;
		Serial.println("wifi status: " + String(WiFi.status()));
	}
}
