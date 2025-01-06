// Declare objects that we are going to read from the display. This includes buttons, sliders, text boxes, etc:
// Format: <type of object> <object name> = <type of object>(<page id>, <object id>, "<object name>");
/* ***** Types of objects:
   NexButton - Button
   NexDSButton - Dual-state Button
   NexHotspot - Hotspot, that is like an invisible button
   NexCheckbox - Checkbox
   NexRadio - "Radio" checkbox, that it's exactly like the checkbox but with a rounded shape
   NexSlider - Slider
   NexGauge - Gauge
   NexProgressBar - Progress Bar
   NexText - Text box
   NexScrolltext - Scroll text box
   NexNumber - Number box
   NexVariable - Variable inside the nextion display
   NexPage - Page touch event
   NexGpio - To use the Expansion Board add-on for Enhanced Nextion displays
   NexRtc - To use the real Time clock for Enhanced Nextion displays
 * *****
*/
//I2C device found at address 0x39
//I2C device found at address 0x3A
#include "PCF8574.h" //Inclui a bliblioteca "PCF8574.h" para controlar o módulo expansor.
#include <24LC256.h>
#include <SPI.h>
#include  "Nextion.h"   //biblioteca Nextion
#include <WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Preferences.h> // Biblioteca para NVS

Preferences preferences;

char* mqtt_server ="192.168.3.200";

uint32_t mqtt_port = 1883;
char mqtt_ip_text[16];
IPAddress mqtt_ip;
extern uint32_t mqtt_port;
extern IPAddress mqtt_ip;

 PCF8574 PCF(0x3D); // enderesso modulos saidas i2c
int RL[17] = {0}, BitPcf = 0;
extern int RL[17], BitPcf;

#define latchPin 26 // st cp  reset
#define clockPin 27 // sh cp  srclk
#define dataPin  25 // ds    ser/si  
byte saida1 = 0x00;

#include <24LC256.h>
E24LC256 MyEEPROM(0x50); //define o endereço base da EEPROM 
#define WP_EEPROM 5  // wirte pin eeprom


#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
bool rtcConected = false;
//extern char DiasDaSemana[7][20] = {"Domingo", "Segunda-Feira", "Terca-Feira", "Quarta-Feira", "Quinta-Feira", "Sexta-Feira", "Sabado"}; // array 7 caracteres de valor 12 caracteres
//extern char semanaN[1] = {0}; // numero da semana atual setada pelo rtc ds1307


#include <Ethernet.h>

#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_SCLK 14
#define HSPI_CS   33   // CS do W5500

#define RESET_P	32				// Tie the Wiz820io/W5500 reset pin to ESP32 GPIO26 pin.


byte mac[] = { 0x02, 0xAD, 0xBE, 0x0D, 0xFE, 0xED };
IPAddress ip(192, 168, 3, 30);
IPAddress myDns(192, 168, 3, 1);
IPAddress gateway(192, 168, 3, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetClient ethClient;
//PubSubClient client(ethClient);


bool ehternet_disable = true; 
char ipAddress[16];
char ssid[32];

extern char ssid[32],ipAddress[16];

WiFiClient espClient;
PubSubClient client(espClient);

int Timex[6] = {0};
extern int Timex[6];


float tensaoEntrada = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DE TENSÃO DE ENTRADA DO SENSOR
float tensaoMedida = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DA TENSÃO MEDIDA PELO SENSOR
float valorR1 = 30000.0; //VALOR DO RESISTOR 1 DO DIVISOR DE TENSÃO
float valorR2 = 7500.0; // VALOR DO RESISTOR 2 DO DIVISOR DE TENSÃO
float tensaoMedia = 0.0, ResultMedia = 0.0;
float bufferMedia[6] = {0.0};
int bufferVR = 0;

char voltC[5] = {0};
extern char voltC[5];

extern bool sdCardFault,noreaDescartPower;
bool sdCardFault = false,noreaDescartPower = false;

int timeOffDescart = 0;
extern int CarretaPosition,CarretaTotalAbatida,CarretaTotalDescarte,timeOffDescart;
extern uint32_t Carreta_Abatida[], Carreta_Descarte[];
bool satInitialized = false; // variavel para atualizar data e hra apenas na inicializaçao

//----------------------------------------------------ENDEREÇO NEXTION IHM tela 0 home ---------------------------------------------------//
NexText nfy = NexText(0, 1, "nfy"); // notificaçao

NexText vel = NexText(0, 7, "vel"); // velocidade norea
NexText hor = NexText(0, 2, "hor"); // hora
NexText bat = NexText(0, 13, "bat"); // bateria
NexText t12 = NexText(0, 35, "t12"); // tempo off descarte

NexNumber C0 = NexNumber(0, 16, "C0");
NexNumber C1 = NexNumber(0, 21, "C1");
NexNumber C2 = NexNumber(0, 22, "C2");
NexNumber C3 = NexNumber(0, 18, "C3");
NexNumber C4 = NexNumber(0, 20, "C4");

extern uint32_t varC0,varC1,varC2,varC3,varC4;
uint32_t varC0 = 0,varC1 = 0,varC2 = 0,varC3 = 0,varC4 = 0;

NexButton b0 = NexButton(0, 3, "b0"); // bt chama pagina carretas
NexButton b1 = NexButton(0, 26, "b1"); // bt chama pagina menu
NexButton b2 = NexButton(0, 30, "b2"); // bt chama intervalo

NexDSButton bt0   = NexDSButton(0, 10, "bt0"); // habilita descarte
NexDSButton bt1   = NexDSButton(0, 11, "bt1"); // habilita contador


extern uint32_t bt0var,bt1var;
uint32_t bt0var = 0,bt1var = 0;

extern uint32_t imagem1,imagem2,imagem3,imagem4,imagem5,imagem6;
uint32_t imagem1=1,imagem2=1,imagem3=1,imagem4=1,imagem5=2,imagem6=5;

NexPicture p0 = NexPicture(0, 5, "p0"); // img 01
NexPicture p1 = NexPicture(0, 31, "p1"); // img 02
NexPicture p2 = NexPicture(0, 9, "p2"); // img 03
NexPicture p3 = NexPicture(0, 33, "p3"); // img 04
NexPicture p4 = NexPicture(0, 15, "p4"); // img 05
NexPicture p5 = NexPicture(0, 38, "p5"); // img 06 sdcard
uint32_t tempoAtt;


//----------------------------------------------------ENDEREÇO NEXTION IHM tela 1 carretas --------------------------------------------------//

int PageCarretas = 1;

NexDSButton atualiza   = NexDSButton(PageCarretas, 31, "atualiza");
extern uint32_t atualizaVar;
uint32_t atualizaVar = 0;
NexButton vt1 = NexButton(PageCarretas,29, "vt1");
NexButton mt = NexButton(PageCarretas,40, "mt"); // chama page mortalidade

NexNumber n0 = NexNumber(PageCarretas, 2, "n0");
NexNumber n1 = NexNumber(PageCarretas, 4, "n1");
NexNumber n2 = NexNumber(PageCarretas, 6, "n2");
NexNumber n3 = NexNumber(PageCarretas, 8, "n3");
NexNumber n4 = NexNumber(PageCarretas, 14, "n4");
NexNumber n5 = NexNumber(PageCarretas, 16, "n5");
NexNumber n6 = NexNumber(PageCarretas, 24, "n6");
NexNumber n7 = NexNumber(PageCarretas, 20, "n7");
NexNumber n8 = NexNumber(PageCarretas, 10, "n8");
NexNumber n9 = NexNumber(PageCarretas, 12, "n9");
NexNumber n10 = NexNumber(PageCarretas, 27, "n10");
NexNumber n11 = NexNumber(PageCarretas, 22, "n11");
NexNumber n12 = NexNumber(PageCarretas, 30, "n12");
NexNumber n13 = NexNumber(PageCarretas, 32, "n13");
NexNumber n14 = NexNumber(PageCarretas, 39, "n14");
NexNumber n15 = NexNumber(PageCarretas, 34, "n15");
NexNumber n16 = NexNumber(PageCarretas, 36, "n16");

NexVariable va0 = NexVariable(PageCarretas, 23, "va0"); // variavel de retorno


//----------------------------------------------------ENDEREÇO NEXTION IHM TELA  3 MORTALIDADE ---------------------------------------------------//

int PageMortalidade = 3;
NexButton vt4   = NexButton(PageMortalidade, 29, "vt4"); // botao voltar
NexButton ed   = NexButton(PageMortalidade, 46, "ed"); // botao editar dados da carreta

NexNumber n17 = NexNumber(PageMortalidade, 2, "n0");
NexNumber n18 = NexNumber(PageMortalidade, 4, "n1");
NexNumber n19 = NexNumber(PageMortalidade, 6, "n2");
NexNumber n20 = NexNumber(PageMortalidade, 8, "n3");
NexNumber n21 = NexNumber(PageMortalidade, 14, "n4");
NexNumber n22 = NexNumber(PageMortalidade, 16, "n5");
NexNumber n23 = NexNumber(PageMortalidade, 24, "n6");
NexNumber n24 = NexNumber(PageMortalidade, 20, "n7");
NexNumber n25 = NexNumber(PageMortalidade, 10, "n8");
NexNumber n26 = NexNumber(PageMortalidade, 12, "n9");
NexNumber n27 = NexNumber(PageMortalidade, 27, "n10");
NexNumber n28 = NexNumber(PageMortalidade, 22, "n11");
NexNumber n29 = NexNumber(PageMortalidade, 30, "n12");
NexNumber n30 = NexNumber(PageMortalidade, 32, "n13");
NexNumber n31 = NexNumber(PageMortalidade, 39, "n14");
NexNumber n32 = NexNumber(PageMortalidade, 34, "n15");
NexNumber n33 = NexNumber(PageMortalidade, 36, "n16");




//----------------------------------------------------ENDEREÇO NEXTION IHM TELA 5 ajustar hora rtc ---------------------------------------------------//
int pageHora=5; // endereço da pagina 

NexButton vt2   = NexButton(pageHora, 3, "vt2"); // botao voltar
NexButton botaoHR   = NexButton(pageHora, 13, "botaoHR"); // botao para atualizar
NexText hor2 = NexText(pageHora, 4, "hor2"); // hora


NexText titulo = NexText(pageHora, 2, "titulo"); 
NexNumber dia = NexNumber(pageHora, 7, "dia");
NexNumber mes = NexNumber(pageHora, 8, "mes");
NexNumber ano = NexNumber(pageHora, 9, "ano");
NexNumber hora = NexNumber(pageHora, 10, "hora");
NexNumber minut = NexNumber(pageHora, 11, "minut");
NexNumber segun = NexNumber(pageHora, 12, "segun");

extern uint32_t botaoHRvar;
uint32_t botaoHRvar = 0;

extern uint32_t diavar, mesvar, anovar, horavar, minutvar, segunvar;
uint32_t diavar = 0, mesvar = 0, anovar = 0, horavar = 0, minutvar = 0, segunvar = 0;
bool carregaeeprom = 0;
int horaAnterior = 0;

//----------------------------------------------------ENDEREÇO NEXTION IHM page 4 Menu --------------------------------------------------//
int pageMenu = 4;


NexButton vt3 = NexButton(pageMenu, 1, "vt3");
NexButton cf = NexButton(pageMenu, 14, "cf");

NexButton z0 = NexButton(pageMenu, 16, "z0");
NexButton z1 = NexButton(pageMenu, 2, "z1");
NexButton z2 = NexButton(pageMenu, 3, "z2");
NexButton z3 = NexButton(pageMenu, 4, "z3");
NexButton z4 = NexButton(pageMenu, 5, "z4");
NexButton z5 = NexButton(pageMenu, 7, "z5");
NexButton z6 = NexButton(pageMenu, 15, "z6");

NexNumber nc1 = NexNumber(pageMenu, 6, "nc1");
NexNumber nc2 = NexNumber(pageMenu, 8, "nc2");
NexNumber nc3 = NexNumber(pageMenu, 9, "nc3");
//-----------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------ENDEREÇO NEXTION IHM page 6 Config --------------------------------------------------//
int pageConfig = 6;
NexText txtcf = NexText(pageConfig, 1, "txtcf"); // titulo

NexButton vt5 = NexButton(pageConfig,2, "vt5");
NexButton cfs = NexButton(pageConfig,7, "cfs");
NexButton rd = NexButton(pageConfig,20, "rd"); // botao page rede

NexNumber cf1 = NexNumber(pageConfig, 3, "cf1");
NexNumber cf2 = NexNumber(pageConfig, 5, "cf2");

NexNumber cf4 = NexNumber(pageConfig, 14, "cf4");
NexNumber cf5 = NexNumber(pageConfig, 16, "cf5");
NexNumber cf6 = NexNumber(pageConfig, 8, "cf6");
NexNumber cf7 = NexNumber(pageConfig, 10, "cf7");
NexNumber cf8 = NexNumber(pageConfig, 18, "cf8");


//-----------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------ENDEREÇO NEXTION IHM page 7 editar --------------------------------------------------//

int pageEdit = 7;
NexText txted = NexText(pageEdit, 1, "txted"); // titulo

NexButton vt6 = NexButton(pageEdit,2, "vt6"); // botao voltar
NexButton ps = NexButton(pageEdit,5, "ps"); // botao pesquisar
NexButton env = NexButton(pageEdit,10, "env"); // botao enviar

NexNumber edn = NexNumber(pageEdit, 3, "edn");
NexNumber ab = NexNumber(pageEdit, 8, "ab");
NexNumber ad = NexNumber(pageEdit, 9, "ad");

//-----------------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------------ENDEREÇO NEXTION IHM page 9 rede --------------------------------------------------//
int pageRede = 9;
NexButton vt7 = NexButton(pageRede,2, "vt7"); // botao voltar
NexButton evp = NexButton(pageRede,20, "evp"); // botao enviar parametros
NexButton eth = NexButton(pageRede,7, "eth"); // botao alterna wifi/ethernet
NexButton wfm = NexButton(pageRede,19, "wfm"); // botao inicializa wifi maneger

NexText t0 = NexText(pageRede, 1, "t0"); // txt titulo page

NexText ipmq = NexText(pageRede, 4, "ipmq"); // txt IP mqtt
NexNumber ptmq = NexNumber(pageRede, 6, "ptmq"); // number Porta mqtt

NexText sid = NexText(pageRede, 18, "sid"); // txt IP ssid wifi
NexText ipct = NexText(pageRede, 15, "ipct"); // txt IP wifi/ethernet

NexText ipet = NexText(pageRede, 9, "ipet"); // txt IP ethernet
NexText sbet = NexText(pageRede, 11, "sbet"); // txt IP subnet
NexText gtet = NexText(pageRede, 13, "gtet"); // txt IP gateway
//-----------------------------------------------------------------------------------------------------------------------------------//


unsigned long cloock = 0, clk1 = 0, Tboucing = 0, Tvarre = 0; 
unsigned long currentTimeSendServer = 0;   
extern int telaAtiva,pulsesPerMinute;
extern bool connected,ContadorON;



NexTouch *nex_listen_list[] = 
{
 &atualiza,&vt1,&mt,//page 1 carretas
&b0,&b1,&b2,&p5, //page 0 home
&bt0,&bt1,&hor,//page 0 home
&vt2, &botaoHR,// page 5 hora
&vt3,&cf,&z0,&z1,&z2,&z3,&z4,&z5,&z6, //page 4 menu
&vt4,&ed, //page 3 mortalidade
&vt5,&cfs,&rd, //page 6 config
&vt6,&ps,&env, //page 7 editar
&vt7,&eth,&evp,&wfm,//page 9 rede
    NULL
};

#define RXD2 16  // Pino RX para Serial2 (ajuste conforme necessário)
#define TXD2 17  // Pino TX para Serial2 (ajuste conforme necessário)
#define LED_BUILTIN 2
#define ANALOG_PIN_1 4 //sensor de tensao

void conectMQTT(){
  client.setServer(mqtt_ip, mqtt_port);
  client.setCallback(callback);
  client.setBufferSize(4024);
}



void setup() {
  Serial.begin(115200);   
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  // Para comunicação com a IHM Nextion
  dbSerialBegin(115200); // Para monitoramento no PC
  dbSerialPrintln("serial iniciada");

  if ( rtc.begin()) { 
    rtcConected = true;
     dbSerial.println(" RTC carregado com sucesso");
  }else{
    dbSerial.println("falha no RTC");
  }
// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

delay(30);
pinMode(ANALOG_PIN_1, INPUT);
pinMode(LED_BUILTIN, OUTPUT);
pinMode(WP_EEPROM, OUTPUT);
digitalWrite(WP_EEPROM,HIGH); 
MyEEPROM.init();
readEEPROM();
 delay(30);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(2, OUTPUT);
  updateShiftRegister();

 nexInit();  //inicializa o tft
nexattachPops();
  PCF.begin(); //expansao i2c

  dbSerial.println("CARREGANDO IHM...");
  delay(100); // aguarde equanto a ihm inicializa

 if(!ehternet_disable){ 
  //Inicializar HSPI para W5500
 // hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS);
  SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS);
  dbSerialPrintln("hspi iniciada");
  delay(50);
   Ethernet.init(HSPI_CS);  // Define o pino CS
   WizReset();
  
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
    dbSerial.println("Ethernet W5500 inicializado");
    if (Ethernet.hardwareStatus() == EthernetNoHardware || Ethernet.linkStatus() == LinkOFF) {
       dbSerial.println("Falha na inicialização do Ethernet. Tentando Wi-Fi...");
       ehternet_disable = false;
       }else{
        dbSerial.println("Ethernet W5500 conectado");
        IPAddress ip = Ethernet.localIP();
        snprintf(ipAddress, sizeof(ipAddress), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]); 
        dbSerial.print("Endereço IP: ");
        dbSerial.println(ipAddress); 
        PubSubClient client(ethClient);
       }
    
 }else{
  PubSubClient client(espClient);
    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(240);
    if (!wifiManager.autoConnect("CONTADOR_V4", "contadorv4")) {
      dbSerial.println(F("Falha na conexão. Resetar e tentar novamente..."));
      delay(500);
      ESP.restart();
    }
  
 strncpy(ssid, WiFi.SSID().c_str(), sizeof(ssid)); 
 dbSerial.print("SSID: ");
 dbSerial.println(ssid);

 IPAddress ip = WiFi.localIP();
 snprintf(ipAddress, sizeof(ipAddress), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]); 
 dbSerial.print("Endereço IP: ");
 dbSerial.println(ipAddress); 
 }

  delay(150);

// Inicializar NVS 
preferences.begin("mqtt", false);

// Recuperar o endereço IP salvo 
String saved_ip = preferences.getString("mqtt_ip", "0.0.0.0");
 strncpy(mqtt_ip_text, saved_ip.c_str(), sizeof(mqtt_ip_text)); 
 mqtt_ip = parseIPAddress(mqtt_ip_text);

 mqtt_port = preferences.getUInt("mqtt_port", 1883);

 dbSerial.print("Endereço IP server MQTT: ");
 dbSerial.print(mqtt_ip); 
 dbSerial.print(" Porta: ");
 dbSerial.println(mqtt_port); 

  conectMQTT();
  reconnect();
  dbSerial.printf("Free Heap: %lu\n", ESP.getFreeHeap());


delay(300);
dbSerial.print("client status = ");
dbSerial.println(client.state());

} // end setup


void loop() {
nexLoop(nex_listen_list);
if (millis() - clk1 > 10){
  clk1 = millis();
client.loop(); 
}

if (!client.connected()) {
    reconnect(); 
    }
InputsBitPCF();// verifica as entradas
  //----------------------------------- Tempo de varredura troca de dados nextion --------------------------//
 if(carregaeeprom==0){
  carregaeeprom = 1;
  //LoadingEeprom();
 }
  if (millis() - Tvarre > 50 ) {
    BitPcf = PCF.read8(); // faz a leitura do pcf
    Tvarre = millis();
    Varredura();
    noreaDescart();
  }
  //----------------------------------------- end ----------------------------------------------//
notifyAndHorTxt();

if (millis() - cloock > 1000){
cloock = millis();

if(telaAtiva == 0){
  bateria();
  calculos();
   }


if(noreaDescartPower){
timeOffDescart ++;
}else{timeOffDescart = 0;}

} // end 1seg
 
 ResendIfFailed();

 if (connected && ContadorON && millis() - currentTimeSendServer > 10000){
  currentTimeSendServer = millis();

  StaticJsonDocument<256> doc;
  doc["CarretaPosition"] = CarretaPosition;
  doc["Carreta_Abatida"] = Carreta_Abatida[CarretaPosition];
  doc["CarretaTotalAbatida"] = CarretaTotalAbatida;
  doc["Carreta_Descarte"] = Carreta_Descarte[CarretaPosition];
  doc["CarretaTotalDescarte"] = CarretaTotalDescarte;
  doc["Velocidade_Norea"] = pulsesPerMinute;
  String json;
  serializeJson(doc, json); // Converte o JsonDocument em uma string JSON

  client.publish("response/currentDataBasic",json.c_str());
 }


}// end loop

  //-------------------------------------get mqtt broker---------------------------------------------------//

extern bool server_response;

 void callback(char* topic, byte* payload, unsigned int length) {
 dbSerial.print("Message Recebida no Topic [");
 dbSerial.print(topic);
 dbSerial.print("] ");
 String PayLoad;
 

  for (int i = 0; i < length; i++) {
   //Serial.print((char)payload[i]);
   PayLoad+=(char)payload[i];
  }
dbSerial.println();
dbSerial.println(PayLoad);

if (strcmp(topic,"server_response")==0){
  server_response = true;
  }


if (!satInitialized && strcmp(topic,"server_return_date")==0){
 StaticJsonDocument<256> doc;
DeserializationError error = deserializeJson(doc, PayLoad);
  if (error) {
    dbSerial.print("Erro ao analisar JSON: date");
    client.publish("contadorSat", "Erro ao analisar JSON: date!");
    return;
  }
  diavar = doc["dia"];
  mesvar = doc["mes"];
  anovar = doc["ano"];
  horavar = doc["hora"];
  minutvar = doc["min"];
  segunvar = doc["seg"];
  delay(100);
  rtc.adjust(DateTime(anovar,mesvar,diavar,horavar,minutvar,segunvar)); // sethra rtc ano/mes/dia  /hora/minuto/segundo
  satInitialized = true;
  }

if (strcmp(topic,"request/dataEdit")==0){
 StaticJsonDocument<256> doc;
DeserializationError error = deserializeJson(doc, PayLoad);
  if (error) {
    dbSerial.print("Erro ao analisar JSON: date");
    client.publish("contadorSat", "Erro ao analisar JSON: date!");
    return;
  }
  if(PayLoad == "false"){
    txted.setText("Dados da Carreta Nao encontrado!");
    dbSerialPrintln("Dados Carreta Nao encontrado!");
  }
 ab.setValue(doc["AvesAbatida"]);
 ad.setValue(doc["AvesDescarte"]);
  }


if ( strcmp(topic,"server_restore")==0){
 StaticJsonDocument<1360> doc;
DeserializationError error = deserializeJson(doc, PayLoad);
  if (error) {
    dbSerial.print("Erro ao analisar JSON: date");
    client.publish("contadorSat", "Erro ao analisar JSON: date!");
    return;
  }
for (int i = 1; i < 20; i++) {
  String key = "CA" + String(i);
  Carreta_Abatida[i] = doc[key];
}

for (int i = 1; i < 20; i++) {
  String key = "CD" + String(i);
  Carreta_Descarte[i] = doc[key];
}
  }

if ( strcmp(topic,"server_restore_current")==0){
 StaticJsonDocument<256> doc;
DeserializationError error = deserializeJson(doc, PayLoad);
  if (error) {
    dbSerial.print("Erro ao analisar JSON: date");
    client.publish("contadorSat", "Erro ao analisar JSON: date!");
    return;
  }

CarretaPosition = doc["CarretaPosition"];
Carreta_Abatida[CarretaPosition] = doc["Carreta_Abatida"];
CarretaTotalAbatida = doc["CarretaTotalAbatida"];
Carreta_Descarte[CarretaPosition] = doc["Carreta_Descarte"];
CarretaTotalDescarte  = doc["CarretaTotalDescarte"];

 }


if (strcmp(topic,"request/currentDataBasic")==0){
  StaticJsonDocument<256> doc;
  doc["CarretaPosition"] = CarretaPosition;
  doc["Carreta_Abatida"] = Carreta_Abatida[CarretaPosition];
  doc["CarretaTotalAbatida"] = CarretaTotalAbatida;
  doc["Carreta_Descarte"] = Carreta_Descarte[CarretaPosition];
  doc["CarretaTotalDescarte"] = CarretaTotalDescarte;
  String json;
  serializeJson(doc, json); // Converte o JsonDocument em uma string JSON

  client.publish("response/currentDataBasic",json.c_str());
  }


if (strcmp(topic, "request/currentData") == 0) {
   StaticJsonDocument<2560> doc;
  bool dataInsert = false; 

  for (int i = 1; i <= 20; i++) {
    if (Carreta_Abatida[i] == 0) {// Se o valor for zero, interrompe o loop
      break;
    }    
    String chaveCarretaAbatida = String(i) + "_Carreta_Abatida";
    String chaveCarretaDescarte = String(i) + "_Carreta_Descarte";
    

    doc[chaveCarretaAbatida] = Carreta_Abatida[i];
    doc[chaveCarretaDescarte] = Carreta_Descarte[i];
    dataInsert = true;

  }

  String json;
  if(dataInsert){
    serializeJson(doc, json); // Converte o JsonDocument em uma string JSON
  } else {
    json = "{\"mensagem\": \"sem dados\"}";  
  }
  
  client.publish("response/currentData", json.c_str());
}




} // end void callback

void calculos(){
if(rtcConected){
 DateTime now = rtc.now();
Timex[0] = now.day();
Timex[1] = now.month();
Timex[2] = now.year();
Timex[3] = now.hour();
Timex[4] = now.minute();
Timex[5] = now.second(); 
}
}//end calculos

void bateria() {
  int valorADC = analogRead(ANALOG_PIN_1);
dbSerial.print("Valor ADC lido: ");
dbSerial.println(valorADC);
    // Calcular a tensão de entrada
    float tensaoEntrada = analogRead(ANALOG_PIN_1) * 3.3 / 3750.0;
    dbSerial.print("Tensao Entrada: ");
    dbSerial.print(tensaoEntrada);

    // Pré-calcular fator de divisão para evitar operação constante
    const float divisor = valorR2 / (valorR1 + valorR2);
    float tensaoMedida = tensaoEntrada / divisor;
    dbSerial.print("   Tensao Calculada: ");
    dbSerial.print(tensaoMedida);

    // Armazenar valor no buffer e calcular média a cada 6 leituras
    static float bufferMedia[6] = {0.0};
    static int bufferIndex = 0;
    static float somaMedia = 0.0;

    // Subtrair o valor mais antigo da soma
    somaMedia -= bufferMedia[bufferIndex];
    
    // Armazenar o novo valor no buffer
    bufferMedia[bufferIndex] = tensaoMedida;
    
    // Adicionar o novo valor à soma
    somaMedia += bufferMedia[bufferIndex];
    
    // Avançar o índice do buffer
    bufferIndex = (bufferIndex + 1) % 6;

    // Calcular a média móvel
    float tensaoMedia = somaMedia / 6.0;

    // Convertendo float para char diretamente
    dtostrf(tensaoMedia, 4, 2, voltC);

    dbSerial.print("   Tensao Media: ");
    dbSerial.println(tensaoMedia);
}

void updateShiftRegister()
{  digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, saida1); 
   digitalWrite(latchPin, HIGH); 
   }


