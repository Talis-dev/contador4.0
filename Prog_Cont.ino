extern uint32_t timeStopDescart, //min
timeDelaySensorDescart, //ms
hooksToRestartCount; //ganchos
 extern String horaInicio, tempoDePausa;

extern int CarretaPosition = 1,CarretaTotalAbatida = 0;
extern uint32_t Carreta_Abatida[21]={0}, Carreta_Descarte[21]={0};
extern bool ContadorON,InputPCF[],IntervaloButton;
extern int telaAtiva;

int countToSwap = 0,quntidade_pausa = 1;
unsigned long currentTimeSwap = 0, TimeBauncing = 0,lastDebounceTime = 0,lastPulseTime = 0, lastCountTimeStopped = 0;
bool SwapActivate = false,functionExecuted = false,resumedCounting = false;
unsigned int pulseCount = 0;
extern int pulsesPerMinute = 0,pulsesPerHour = 0;


void Contagem_Abatida() { 


  if(ContadorON){


 if(InputPCF[7] && InputPCF[6] && millis() - TimeBauncing > timeBauncingTrolleyPendura ){
  TimeBauncing = millis();

 if(Carreta_Abatida[CarretaPosition] == 0){ // registra hora do inicio da contagem
  horaInicio = String(Timex[3],DEC);
  horaInicio += ":";
  horaInicio += String(Timex[4],DEC);
  horaInicio += ":";
  horaInicio += String(Timex[5],DEC);
   }



    dbSerial.print("Pulsos por minuto: ");
    dbSerial.println(pulsesPerMinute);
    dbSerial.print("Pulsos por hora: ");
    dbSerial.println(pulsesPerHour);

  Carreta_Abatida[CarretaPosition]++;
  CarretaTotalAbatida ++;
   
  lastCountTimeStopped = millis();  // Atualiza o tempo da última contagem
  functionExecuted = false; // Reseta a variável de controle
 // resumedCounting = false; // Reseta a variável de controle para retomada
checkIfItHasReturnedAfterStopping();
  }

if(SwapActivate){ // inicia a contagem ate a defeiniçao para troca de carreta
  countToSwap++;
}

medirVelocidade();
SwapTrailer();
setStoppedTime();
 } 

  }


void medirVelocidade() {
    unsigned long currentTime = millis();

    if (InputPCF[7] && (currentTime - lastDebounceTime) > 150) {
        lastDebounceTime = currentTime;
        pulseCount++;
        unsigned long pulseDuration = currentTime - lastPulseTime;
        lastPulseTime = currentTime;

        // Calcula a velocidade em pulsos por minuto
        pulsesPerMinute = 60000.0 / pulseDuration;
        pulsesPerHour = pulsesPerMinute * 60;
    }
}

void SwapTrailer(){ // troca de carreta

if(InputPCF[2]){  // botao branco NO
  if(millis() - currentTimeSwap >= 1000){
currentTimeSwap = millis();

SwapActivate = true;
    bitWrite(saida1, 2, true);  // Atualiza o shift register
    updateShiftRegister(); 
    dbSerial.println("solicitado a troca de carreta, aguardando contagem ");
    showNotification("INICIOU A TROCA DE CARRETA!",3);
  }
}

if(SwapActivate && countToSwap >= hooksToRestartCount){ // se a contagem chegar a quantidade definida de aves, resete e mude de carreta
Log_Carreta();
SwapActivate = false;
countToSwap = 0;
    bitWrite(saida1, 2, false);  // Atualiza o shift register
    updateShiftRegister(); 
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

}

void setStoppedTime(){

if (CarretaTotalAbatida > 0 && millis() - lastCountTimeStopped > breakTime && !functionExecuted) {
String setText = "";
int numcolor = 3;
  if(IntervaloButton){

    setText = "PARADA P/ INTERVALO ";  
    numcolor = 2;
   }else{
    setText = "PARADA " + String(quntidade_pausa,DEC) + " REGISTRADA ";
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
    setText = "RETORNO DO INTERVALO " ;
    }else{
      setText = "RETORNO REGISTRADO ";
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


