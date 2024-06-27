#include "WiFi.h"
#include <HTTPClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "ESP-Accesso";
const char* password = "12345678";

unsigned long previousMillis = 0;
const long intervalo = 2000;

const long intervaloTamanhoPacote = 3600000;
unsigned long previousMillisPacote = 0;
int tamanhoPacote = 19;

String mensagem = "";

IPAddress server(192,168,4,1);  //IP do Server
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Wire.begin(5, 4);

  IPAddress DHCP(192, 168, 4, 30);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);

  Serial.print("Conectando na rede: ");
  Serial.println(ssid); 

  WiFi.begin(ssid, password);
  WiFi.config(DHCP, gateway, subnet);

  display.clearDisplay();  
  display.setTextSize(2); 
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.println("Conectando... ");
  display.display();

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("\nWiFi conectado. Endereço de IP: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.println("CONECTADO!");
  display.setTextSize(1);   
  display.print("IP:");
  display.println(WiFi.localIP());
  display.display();

}

void loop() {
  unsigned long currentMillis = millis();
  mensagem = "";

  if (currentMillis - previousMillisPacote >= intervaloTamanhoPacote){
    tamanhoPacote = tamanhoPacote + 20;
    previousMillisPacote = currentMillis;
  }

  if (currentMillis - previousMillis >= intervalo) {
    if (WiFi.status() == WL_CONNECTED) {
      client.connect(server, 80);

      for (int i = 0; i < tamanhoPacote; i++) {
        char z = 'C';
        mensagem += z;
      }

      client.print(mensagem + "\r"); //Mensagem enviada para o Server
      Serial.println("ENVIOU MENSAGEM!!");

      ClientContinue();

    }  else {
      Serial.println("WiFi Desconectado");
    }
    previousMillis = currentMillis;
  }

}

void ClientContinue(){

  String answer = client.readStringUntil('\r'); // receives the answer from the sever
  client.flush();

  Serial.println("");
  if (answer != ""){
    Serial.println("Dado Recebido: " + answer);
  } else {
    Serial.println("Mensagem nao Recebida!!");
  }
}
