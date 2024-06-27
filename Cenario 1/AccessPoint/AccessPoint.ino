#include <WiFi.h>

const char* ssid = "ESP-Accesso";
const char* password = "12345678";

unsigned long previousMillisA = 0;

const long intervaloA = 1500;

const long intervaloTamanhoPacote = 3600000;
unsigned long previousMillisPacote = 0;

int totalMensagemErroA = 0;

int totalMensagemSucessoA = 0;

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

      totalMensagemSucessoA = totalMensagemSucessoA + 1;
      previousMillisA = currentMillis; 

      Serial.print("\nMensagem A: Sucesso = ");
      Serial.print(totalMensagemSucessoA);
      Serial.print(" / Erro = ");
      Serial.println(totalMensagemErroA);     
    }
  } else { //Se nao houver cliente conectado,
    client = server.available(); //Disponabiliza o servidor para o cliente se conectar.
    delay(1);
  }

  if (currentMillis - previousMillisA >= intervaloA){
    Serial.println("\nMensagem A não recebida");
    totalMensagemErroA = totalMensagemErroA + 1;
    previousMillisA = currentMillis;
  }

  if (currentMillis - previousMillisPacote >= intervaloTamanhoPacote){
    Serial.print("\n---------------RESULTADO------------------");
    Serial.print("\nMensagem A: Sucesso = ");
    Serial.print(totalMensagemSucessoA);
    Serial.print(" / Erro = ");
    Serial.println(totalMensagemErroA);
    Serial.print("\n---------------RESULTADO------------------");

    totalMensagemSucessoA = 0;
    totalMensagemErroA = 0;
    previousMillisPacote = currentMillis;
  }

}
