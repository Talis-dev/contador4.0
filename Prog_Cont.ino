extern uint32_t hooksToRestartCount; //ganchos
 extern String horaInicio, tempoDePausa;

extern int CarretaPosition = 1,CarretaTotalAbatida = 0;
extern uint32_t Carreta_Abatida[21]={0}, Carreta_Descarte[21]={0};
extern bool ContadorON,InputPCF[],IntervaloButton,sdCardWriteSuccessful, notificationSD = false, noreaRun = false;
extern int telaAtiva;

int countToSwap = 0,quntidade_pausa = 1;
unsigned long currentTimeSwap = 0, TimeBauncing = 0,lastDebounceTime = 0,lastPulseTime = 0,
 lastCountTimeStopped = 0, warningLightTime = 0,
 currentTimeMessageSD = 0;
bool SwapActivate = false,functionExecuted = false,resumedCounting = false,warningLightBool = false;
unsigned int pulseCount = 0;
extern int pulsesPerMinute = 0,pulsesPerHour = 0;


#define NUM_PULSOS 10
unsigned long pulseDurations[NUM_PULSOS] = {0}; // Armazena os últimos 10 pulsos
int pulseIndex = 0;
bool leituraEstavel = false;

void Contagem_Abatida() { 


  if(ContadorON){


 if(InputPCF[7] && InputPCF[6] && noreaRun && millis() - TimeBauncing > timeBauncingTrolleyPendura*10 ){ // [7] gancho aves  [6] sensor gancho
  TimeBauncing = millis();

 if(Carreta_Abatida[CarretaPosition] == 0){ // registra hora do inicio da contagem
  horaInicio = String(Timex[3],DEC);
  horaInicio += ":";
  horaInicio += String(Timex[4],DEC);
  horaInicio += ":";
  horaInicio += String(Timex[5],DEC);
   }

  Carreta_Abatida[CarretaPosition]++;
  CarretaTotalAbatida ++;
   
  lastCountTimeStopped = millis();  // Atualiza o tempo da última contagem
  functionExecuted = false; // Reseta a variável de controle
  
 // resumedCounting = false; // Reseta a variável de controle para retomada
checkIfItHasReturnedAfterStopping();

if(SwapActivate){ // inicia a contagem ate a defeiniçao para troca de carreta
  countToSwap++;
}

  } // enquanto sensor for atuado

medirVelocidade();
SwapTrailer();
setStoppedTime();
notificationSdCardAfterChange();
 }else{ // contador habilitado

 noreaRun = false; // norea parada}
  }
}

#define NUM_PULSOS 10
unsigned long pulseDurations[NUM_PULSOS] = {0};
int pulseIndex = 0;
bool leituraEstavel = false;

void medirVelocidade() {
    unsigned long currentTime = micros();
    unsigned long debounceTime = (avgPulseDuration > 2000000) ? 200000 : 100000; // Debounce ajustável

    if (InputPCF[6] && (currentTime - lastDebounceTime) > debounceTime) {
        lastDebounceTime = currentTime;

        unsigned long pulseDuration = currentTime - lastPulseTime;
        lastPulseTime = currentTime;

        // Ignora pulsos anômalos
        if (leituraEstavel && (pulseDuration < (avgPulseDuration * 0.5))) {
            return;
        }

        pulseDurations[pulseIndex] = pulseDuration;
        pulseIndex = (pulseIndex + 1) % NUM_PULSOS;

        if (pulseIndex == 0) leituraEstavel = true;

        if (leituraEstavel) {
            unsigned long totalDuration = 0;
            for (int i = 0; i < NUM_PULSOS; i++) {
                totalDuration += pulseDurations[i];
            }
            float avgPulseDuration = totalDuration / (float)NUM_PULSOS;

            pulsesPerMinute = (60000000.0 / avgPulseDuration);
            pulsesPerHour = pulsesPerMinute * 60;

            if (pulsesPerMinute > 360) { // Caso o sensor fique acionado direto // Limite máximo de segurança
                noreaRun = false;
                pulsesPerMinute = 0;
                pulsesPerHour = 0;
            } else {
                noreaRun = true;
            }
        }
    }
    
    // Se não houver pulsos por mais de 2 segundos
    if ((micros() - lastPulseTime) > 2000000) {
        noreaRun = false;
        pulsesPerMinute = 0;
        pulsesPerHour = 0;
    }
}


void SwapTrailer(){ // troca de carreta

if(InputPCF[2]){  // botao branco NO
  if(millis() - currentTimeSwap >= 1000){
currentTimeSwap = millis();

SwapActivate = true;
    bitWrite(saida1, 2, true);  // Atualiza o shift register/ chama saida led button white
    updateShiftRegister(); 
    dbSerial.println("solicitado a troca de carreta, aguardando contagem ");
    showNotification("INICIOU A TROCA DE CARRETA!",3);
  }
}

if(SwapActivate && countToSwap >= hooksToRestartCount){ // se a contagem chegar a quantidade definida de aves, resete e mude de carreta
postDataServer();
SwapActivate = false;
countToSwap = 0;

bitWrite(saida1, 3, true); // chama saida luz indicaçao evisceraçao
updateShiftRegister(); 
warningLightBool = true; // habilita luz de indicaçao na evisceraçao
warningLightTime = millis();// zera timer lus indicaçao

    bitWrite(saida1, 2, false);  // Atualiza o shift register
    updateShiftRegister(); 

    notificationSD = true;         //chama a funçao para notificar a gravaçao no sd card apos trocar carreta
    currentTimeMessageSD = millis();

if(CarretaPosition <= 20){
  //CarretaPosition++;
    for (int i = 1; i <= 20; i++) {
        if (Carreta_Abatida[i] == 0) {
            CarretaPosition = i;
            break;
        }
    }
    
  String setText = "CARRETA N";
  setText += String(CarretaPosition,DEC);
  setText += " INICIADA!";
  showNotification(setText.c_str(),3);
  tempoDePausa = "Não houve Parada";
  quntidade_pausa = 1;
}else{
  CarretaPosition=1;
  showNotification("LImite de carretas atigido!, vontando p/ 1",1);
  tempoDePausa = "Não houve Parada";
  quntidade_pausa = 1;  
  }

}

 if(warningLightBool && millis() - warningLightTime >= warningLight*1000){ // funcao para manter luz indicaçao acessa ate o presset
  warningLightBool = false;
    bitWrite(saida1, 3, false); // chama saida luz indicaçao evisceraçao
    updateShiftRegister();
 }


} // end function

void setStoppedTime(){ // quando parar de contar

if (CarretaTotalAbatida > 0 && millis() - lastCountTimeStopped > breakTime *1000 && !functionExecuted) {
String setText = "";
int numcolor = 3;
pulsesPerMinute = 0;
pulsesPerHour = 0; // resenta o medidor de velocidade qando parar
noreaRun = false; // norea parada

  if(IntervaloButton){
    sendCommand("b2.bco=1040");
    setText = "Parada P/ Intervalo ";  
    numcolor = 2;
   }else{
    setText = "Parada " + String(quntidade_pausa,DEC) + " registrada ";
    numcolor = 1;
   // setText += ";    
    }  

  setText += String(Timex[3],DEC);
  setText += ":";
  setText += String(Timex[4],DEC);
  setText += ":";
  setText += String(Timex[5],DEC);
  showNotification(setText.c_str(),numcolor);
  setText += " - ";

  if(quntidade_pausa == 1){
    tempoDePausa = setText;
  }else if(quntidade_pausa > 1){
  tempoDePausa += setText;
  }
  
    functionExecuted = true; // Marca a função como executada
    resumedCounting = true; // Reseta a variável de controle para retomada
  }


}
 void checkIfItHasReturnedAfterStopping(){

  if (resumedCounting ) {// executa após retomada
   String setText = "";
    if(IntervaloButton){
    setText = "Retorno do intervalo " ;
    }else{
      setText = "Retorno registrado ";
    } 
 
  setText += String(Timex[3],DEC);
  setText += ":";
  setText += String(Timex[4],DEC);
  setText += ":";
  setText += String(Timex[5],DEC);
  showNotification(setText.c_str(),3);
  setText += " / ";
  tempoDePausa += setText;
  quntidade_pausa ++;
  resumedCounting = false; // Marca a função como executada após retomada
  IntervaloButton = false; // reseta button intervalo depois de retornar
  }
 }


void notificationSdCardAfterChange(){ // funcao executada apos tentativa de gravaçao no servidor evia notfy se sucesso ou falha
if(notificationSD && millis() - currentTimeMessageSD >= notificationDuration*1000 + 800){ // tempo de duraçao da notificaçao sd card apos trocar de carrta

if(server_response){
showNotification("Dados enviados com sucesso!",2);
}else{
  String mqttError = "ERROR ";
  mqttError += client.state();
  mqttError +=" Falha ao enviar dados!";
showNotification(mqttError.c_str(),1);
}
notificationSD = false;
} 
}


void finalizaAbate(){
    tempoDePausa = "Não houve Parada";
    functionExecuted = false; 
    resumedCounting = false; 
    quntidade_pausa = 1;

}