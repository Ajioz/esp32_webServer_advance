#include <WiFi.h>
#include "aWOT.h"
#include "StaticFiles.h"

#define WIFI_SSID "AjiozDev"
#define WIFI_PASSWORD "sandiewhyte123"

#define LED_BUILTIN 2

WiFiServer server(80);
Application app;

bool ledOn;

void readLed(Request &req, Response &res) {
  res.print(ledOn);
}

void updateLed(Request &req, Response &res) {
  /*
    ledOn = (req.read() != '0');
    digitalWrite(LED_BUILTIN, ledOn);
    return readLed(req, res);
  */
  
  /* The above code wasn't working as expected, I came up with a simple fic below */
    int myRead = digitalRead(LED_BUILTIN);
    if(myRead == LOW){
      digitalWrite(LED_BUILTIN, HIGH);
      ledOn = 1;
    }else{
      digitalWrite(LED_BUILTIN, LOW);
      ledOn = 0;
    }
    Serial.print("ledON State: ");Serial.println(ledOn);
    return readLed(req, res);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  app.get("/led", &readLed);
  app.put("/led", &updateLed);
  app.route(staticFiles());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client.connected()) {
    app.process(&client);
  }
}
