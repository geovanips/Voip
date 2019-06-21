#include <ESP8266WiFi.h> //bilbioteca para usar funções wifi do esp
#include <PubSubClient.h> //cliente MQTT

// WIFI
const char* SSID = "Relicario"; //nome da rede
const char* PASSWORD = "21011993"; //senha

// MQTT
const char* BROKER_MQTT = "192.168.10.7"; //endereço do broker
int BROKER_PORT = 1883; //porta de comunicação do mqtt

WiFiClient espClient;           //cria objeto cliente
PubSubClient MQTT(espClient);   //instancia o cliente mqtt passando o espClient

//define o pino do led
#define ledPin 4

int statusrele;
const char* status = "desligada";

//prototypes
void initPins();
void initSerial();
void initWiFi();
void initMQTT();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup() {
  initPins();
  initSerial();
  initWiFi();
  initMQTT();
}
//define o modo dos pinos e inicializa
void initPins() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
}
void initSerial() {
  Serial.begin(9600); //inicia monitor serial com boundrate 115200
}
 //inicializa e conecta na wifi
void initWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se em: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD); //conecta-se a rede usando o SSID e PASSWORD
  while (WiFi.status() != WL_CONNECTED) { //enquando o sataus da conexão for diferente de conectado, fica esperando até conectar
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado na Rede ");
  Serial.print(SSID);
  Serial.println(" | IP ");
  Serial.println(WiFi.localIP());
}
//conecta ao broker
void initMQTT(){
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);     //seta parametros de conexão MQTT
  MQTT.setCallback(mqtt_callback);              //atribui função callback
}
//função callback, chamada sempre que qualquer informação de um dos tópicos chega
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String message;
  //obtem a string do payload recebido
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  //printa informações
  Serial.print("Tópico ");
  Serial.print(topic);
  Serial.print(" | ");
  Serial.println(message);
  //realiza ação de acordo com a string recebida
    if (message == "1") {
      digitalWrite(ledPin, 1);
      MQTT.publish("statusrele", "ligada");
      status = "ligada";
    } else if (message == "0") {
        digitalWrite(ledPin, 0);
        MQTT.publish("statusrele", "desligada");
        status = "desligada";
      } else if(message == "status"){
      MQTT.publish("statusrele", status);
    }

  //"zera" string e espera transmissão de dados de saída para completar (?)
  message = "";
  Serial.println();
  Serial.flush();
}
//reconecta ao broker caso a conexão caia ou não esteja conectado
void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    //se conseguir conexão o subscribe do(s) topico(s) é feito
    if (MQTT.connect("ESP8266-ESP12-E")) {
      Serial.println("Conectado");
      MQTT.subscribe("rele");
      MQTT.subscribe("status");
    } else { //caso não consiga conectar espera 2 segundos para tentar de novo
      Serial.println("Falha ao Reconectar");
      Serial.println("Tentando se reconectar em 2 segundos");
      delay(2000);
    }
  }
}
//reconecta wifi
void recconectWiFi() {
  //enquando o status for diferente, fica tentando conecctar
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
}
void loop() {
  if (!MQTT.connected()) {//se a conexão cair
    reconnectMQTT();       //tenta conectar novamente
  }
  recconectWiFi(); // se a conexão do esp cair, tenta conectar de novo
  MQTT.loop(); //keep-alive de comunicação com o broker MQTT
}
