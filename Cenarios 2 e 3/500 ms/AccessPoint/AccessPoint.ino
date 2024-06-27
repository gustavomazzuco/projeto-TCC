#include <WiFi.h>

const char* ssid = "ESP-Accesso";
const char* password = "12345678";

unsigned long previousMillisA = 0;
unsigned long previousMillisB = 0;
unsigned long previousMillisC = 0;

const long intervaloA = 750;
const long intervaloB = 750;
const long intervaloC = 750;

const long intervaloTamanhoPacote = 3600000;
unsigned long previousMillisPacote = 0;

int totalMensagemErroA = 0;
int totalMensagemErroB = 0;
int totalMensagemErroC = 0;

int totalMensagemSucessoA = 0;
int totalMensagemSucessoB = 0;
int totalMensagemSucessoC = 0;

int pacoteEsperado = 20;

IPAddress IPA(192, 168, 4, 10);
IPAddress IPB(192, 168, 4, 20);
IPAddress IPC(192, 168, 4, 30);

WiFiServer server(80);
WiFiClient client;

void setup() {
  Serial.begin(115200);

  IPAddress DHCP(192, 168, 4, 2);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.mode(WIFI_AP);

  WiFi.softAP(ssid, password);
  WiFi.config(DHCP, gateway, subnet);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (client.connected()){ //Detecta se há clientes conectados no servidor.
    if (client.available() > 0){ //Verifica se o cliente conectado tem dados para serem lidos.
      String req = "";
      int tamanho = 0;
      
      while (client.available() > 0) //Armazena cada Byte (letra/char) na String para formar a mensagem recebida.
      {
        char z = client.read();
        req += z;
        tamanho += sizeof(z);
      }

      IPAddress ipCliente = client.remoteIP();

      //Mostra a mensagem recebida do cliente no Serial Monitor.
      Serial.print("\nUm cliente enviou uma mensagem");
      Serial.print("\n...IP do cliente: ");
      Serial.print(client.remoteIP());
      Serial.print("\n...Mensagem: " + req + "\n"); 
      Serial.print("Tamanho: ");
      Serial.print(tamanho);  
      Serial.println(" bytes");

      //Envia uma resposta para o cliente
      client.print("O servidor recebeu sua mensagem");
      client.print("\n...Seu IP: ");
      client.print(client.remoteIP());
      client.print("\n...IP do Servidor: ");
      client.print(WiFi.softAPIP());
      client.print("\n...Sua mensagem: " + req + "\n");

      if (client.remoteIP() == IPA) {
        if (pacoteEsperado == tamanho) {
          totalMensagemSucessoA = totalMensagemSucessoA + 1;
        }
        previousMillisA = currentMillis;

        Serial.print("\nMensagem A: Sucesso = ");
        Serial.print(totalMensagemSucessoA);
        Serial.print(" / Erro = ");
        Serial.println(totalMensagemErroA);
      } 
      
      if (client.remoteIP() == IPB) {
        if (pacoteEsperado == tamanho) {
          totalMensagemSucessoB = totalMensagemSucessoB + 1;
        }
        previousMillisB = currentMillis;

        Serial.print("\nMensagem B: Sucesso = ");
        Serial.print(totalMensagemSucessoB);
        Serial.print(" / Erro = ");
        Serial.println(totalMensagemErroB);
      }
      
      if (client.remoteIP() == IPC) {
        if (pacoteEsperado == tamanho) {
          totalMensagemSucessoC = totalMensagemSucessoC + 1;
        }
        previousMillisC = currentMillis;

        Serial.print("\nMensagem C: Sucesso = ");
        Serial.print(totalMensagemSucessoC);
        Serial.print(" / Erro = ");
        Serial.println(totalMensagemErroC);
      }
    }
  } else { //Se nao houver cliente conectado,
    client = server.available(); //Disponabiliza o servidor para o cliente se conectar.
    delay(1);
  }

  if (currentMillis - previousMillisA >= intervaloA) {
    Serial.println("\nMensagem A não recebida");
    totalMensagemErroA = totalMensagemErroA + 1;
    previousMillisA = currentMillis;
  }

  if (currentMillis - previousMillisB >= intervaloB){
    Serial.println("Mensagem B não recebida");
    totalMensagemErroB = totalMensagemErroB + 1;
    previousMillisB = currentMillis;
  }

  if (currentMillis - previousMillisC >= intervaloC){
    Serial.println("Mensagem C não recebida");
    totalMensagemErroC = totalMensagemErroC + 1;
    previousMillisC = currentMillis;
  }

  if (currentMillis - previousMillisPacote >= intervaloTamanhoPacote){
    Serial.print("\n---------------RESULTADO------------------");
    Serial.print("\nMensagem A: Sucesso = ");
    Serial.print(totalMensagemSucessoA);
    Serial.print(" / Erro = ");
    Serial.println(totalMensagemErroA);;
    Serial.print("\nMensagem B: Sucesso = ");
    Serial.print(totalMensagemSucessoB);
    Serial.print(" / Erro = ");
    Serial.println(totalMensagemErroB);
    Serial.print("\nMensagem C: Sucesso = ");
    Serial.print(totalMensagemSucessoC);
    Serial.print(" / Erro = ");
    Serial.println(totalMensagemErroC);
    Serial.print("\n---------------RESULTADO------------------");

    totalMensagemSucessoA = 0;
    totalMensagemSucessoB = 0;
    totalMensagemSucessoC = 0;
    totalMensagemErroA = 0;
    totalMensagemErroB = 0;
    totalMensagemErroC = 0;
    previousMillisPacote = currentMillis;
    pacoteEsperado = pacoteEsperado + 20;
  }

}
