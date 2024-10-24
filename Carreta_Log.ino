extern String DateAndHora_Str = "", DataToFile = "",horaInicio = "",tempoDePausa = "Não houve Parada";
extern bool sdCardFault;
extern bool sdCardWriteSuccessful = false, server_response = false, ReesendActive = false;

String quarantineData = "";
unsigned long timeReesend = 1;
int contReesend = 0;

void postDataServer(){
server_response = false;
delay(50);
client.publish("customer_response","ACK"); // envia um sinal de status para testar a conexao
Log_Carreta();
}

void Log_Carreta(){
// TODO: adicionar confg n de tentativas de reenvio e tempo para reenviar


//DataToFile +=".csv";

String carreta_position="Carreta ";
carreta_position += CarretaPosition;

String avesAbatida = String(Carreta_Abatida[CarretaPosition]);
String avesDescarte = String(Carreta_Descarte[CarretaPosition]);
 
  StaticJsonDocument<2560> jsonDoc;

  // Adicionar dados ao JSON
  jsonDoc["Carreta"] = CarretaPosition;
  jsonDoc["HoraInicio"] = horaInicio;
  jsonDoc["AvesAbatida"] = avesAbatida;
  jsonDoc["AvesDescarte"] = avesDescarte;
  jsonDoc["TempoPausa"] = tempoDePausa;
  jsonDoc["HoraFinalizada"] = DateAndHora_Str;
    // FileCsv.println("\"Carreta Nº\",\"Hora de Inicio\",\"Total Aves\",\"Total Descarte\",\"Parada/Intervalo\",\"Data e Hora Finalizada\"");  // CRIA COLUNAS

  String jsonStr;
  serializeJson(jsonDoc, jsonStr);
  quarantineData = jsonStr;
delay(50);
 if (client.publish("topic/carretaData", jsonStr.c_str())) {
  
  dbSerial.println("Dados enviados com sucesso!!");
  sdCardWriteSuccessful = true;
 }else{
    dbSerial.println("ERRO NA GRAVAÇAO verifique a conexao!");
    
    sdCardWriteSuccessful = false;
    } 

  timeReesend = millis(); // ativa o temporizador para reenviar
  contReesend = 0; // zera o contador
  ReesendActive = true;

} // end void log_carreta

void ResendIfFailed(){

if(ReesendActive && millis() - timeReesend > 5000){

    if(!server_response && !contReesend){
    dbSerial.println("ERRO verifique a conexao! sinal de vida nao recebido");
      String mqttError = "Falha ";
  mqttError += client.state();
  mqttError +=". Tentando reenviar os dados!";
  showNotification(mqttError.c_str(),1);
    sdCardWriteSuccessful = false;
    }else if(server_response && !contReesend){
      showNotification("Server response ACK!",3);
      server_response = false; // reseta resposta
      contReesend = 0; //finaliza funcao)
      ReesendActive = false;
      sdCardWriteSuccessful = true;
    }

   if( contReesend && contReesend <= 10){

 if (server_response && client.publish("topic/carretaData", quarantineData.c_str())) {
  
  dbSerial.println("Dados reenviados com sucesso!!");
  showNotification("Dados reenviado com sucesso!",3);
  contReesend = 0; //finaliza funcao
  ReesendActive = false;
  sdCardWriteSuccessful = true;

 }else{
  client.publish("customer_response","ACK"); // envia um sinal de status para testar a conexao
   showNotification("Falha ao reenviar. nova Tentativa em 5s",1);
    dbSerial.println("ERROR SERVIDOR SEM SINAL. Reenviando!");
 }

   }

  if(contReesend > 10){
  showNotification("Tentativas exedidas, Falha Na REDE!!!",1);
  contReesend = 0; //finaliza funcao
  ReesendActive = false;
 }
  timeReesend = millis();
  contReesend ++; 
}

}
