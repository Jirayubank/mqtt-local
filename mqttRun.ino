#include <WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE  (50)
#define relay1 18
#define relay2 19
#define relay3 21
#define relay4 22

const char* ssid = "Localhost:1880";
const char* password = "for i in range(100)";
const char* mqtt_server = "192.168.43.138";
const int mqtt_port = 1883;
const char* mqtt_Client = "esp_8266_bankkv";
const char* mqtt_username = "";
const char* mqtt_password = "";

WiFiClient esp;
PubSubClient client(esp);

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup() {
  Serial.begin(9600);
  setupWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
}  

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
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
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("/py");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }

 if (String(topic) == "/py") {
    Serial.println("message sussess LED");
    if (message == "{\"out\":0}") {
      relayControl(1, 0, 1, 0);
      Serial.println("wear");
    }
    else if ( message == "{\"out\":1}") {
      relayControl(0, 1, 0, 1);
      Serial.println("not wear");
    }
    else if ( message == "{\"out\":2}"){
      relayControl(0, 0, 0, 0);
      Serial.println("idle");
    }
  }

}
void relayControl(bool ch1, bool ch2, bool ch3, bool ch4){
  digitalWrite(relay1, ch1==1?1:0);
  digitalWrite(relay2, ch2==1?1:0);
  digitalWrite(relay3, ch3==1?1:0);
  digitalWrite(relay4, ch4==1?1:0);
}
