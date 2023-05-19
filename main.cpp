#include <Arduino.h>
#include <Wire.h>
#include <DHTesp.h> // Click here to get the library: http://librarymanager/All#DHTesp
#include <ThingsBoard.h>
#include <WiFi.h>
// See https://thingsboard.io/docs/getting-started-guides/helloworld/
#define THINGSBOARD_SERVER  "demo.thingsboard.io"
#define WIFI_SSID "R_X3P"
#define WIFI_PASSWORD "@RudySusanto"
#define LED_GREEN  14
#define LED_YELLOW 12
#define LED_RED    13
#define DHT_PIN 19
#define LED_BUILTIN 33

//put your token ID here
#define THINGSBOARD_ACCESS_TOKEN "t5cFt4ZyUEO66RY06EfI" 

WiFiClient espClient;
ThingsBoard tb(espClient);
DHTesp dht;

void WifiConnect();
void onSendSensor();

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  dht.setup(DHT_PIN, DHTesp::DHT22);
  
  WifiConnect();
  if (tb.connect(THINGSBOARD_SERVER, THINGSBOARD_ACCESS_TOKEN))
    Serial.println("Connected to thingsboard");
  else
    Serial.println("Error connected to thingsboard");
  Serial.println("System ready.");
  digitalWrite(LED_BUILTIN, 0);
}

void loop() {
  if (millis() % 3000 ==0 && tb.connected())
    onSendSensor();
  tb.loop();
}

void onSendSensor()
{
  digitalWrite(LED_BUILTIN, 1);
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  if (dht.getStatus()==DHTesp::ERROR_NONE)
  {
    Serial.printf("Temperature: %.2f C, Humidity: %.2f %%\n",temperature, humidity);
    if (tb.connected())
    {
      tb.sendTelemetryFloat("temperature", temperature);
      tb.sendTelemetryFloat("humidity", humidity);
    }
  }
  else
    digitalWrite(LED_BUILTIN, 0);
}

void WifiConnect()
{
  //WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }  
  Serial.print("System connected with IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
}
