extern uint32_t timeStopDescart, //min
timeDelaySensorDescart, //ms
hooksToRestartCount; //ganchos
 extern String horaInicio, tempoDePausa;

extern int CarretaPosition = 1,CarretaTotalAbatida = 0;
extern uint32_t Carreta_Abatida[21]={0}, Carreta_Descarte[21]={0};
extern bool ContadorON,InputPCF[],IntervaloButton;
extern int telaAtiva;

int countToSwap = 0,quntidade_pausa = 1;
unsigned long currentTimeSwap = 0, TimeBauncing = 0,lastCountTimeStopped = 0;
bool SwapActivate = false,functionExecuted = false,resumedCounting = false;

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

  Carreta_Abatida[CarretaPosition]++;
  CarretaTotalAbatida ++;

  lastCountTimeStopped = millis();  // Atualiza o tempo da última contagem
  functionExecuted = false; // Reseta a variável de controle
 // resumedCounting = false; // Reseta a variável de controle para retomada
checkIfItHasReturnedAfterStopping();


if(SwapActivate){ // inicia a contagem ate a defeiniçao para troca de carreta
  countToSwap++;
}

 } 

SwapTrailer();
setStoppedTime();
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
    showNotification("INICIOU A TROCA DE CARRETA!");
  }
}

if(SwapActivate && countToSwap >= hooksToRestartCount){ // se a contagem chegar a quantidade definida de aves, resete e mude de carreta
Log_Carreta();
SwapActivate = false;
countToSwap = 0;
    bitWrite(saida1, 2, false);  // Atualiza o shift register
    updateShiftRegister(); 
if(CarretaPosition <= 20){
  CarretaPosition++;
  String setText = "CARRETA N";
  setText += String(CarretaPosition,DEC);
  setText += " INICIADA!";
  showNotification(setText.c_str());
  tempoDePausa = "Não houve Parada";
  quntidade_pausa = 1;
}else{
  CarretaPosition=1;
  showNotification("LImite de carretas atigido!, vontando p/ 1");
  tempoDePausa = "Não houve Parada";
  quntidade_pausa = 1;  
  }
  

}

}

void setStoppedTime(){

if (CarretaTotalAbatida > 0 && millis() - lastCountTimeStopped > breakTime && !functionExecuted) {
String setText = "";
  if(IntervaloButton){

    setText = "PARADA P/ INTERVALO ";  
   }else{
    setText = String(quntidade_pausa,DEC);
    setText += "ª PARADA REGISTRADA ";    
    }  

  setText += String(Timex[3],DEC);
  setText += ":";
  setText += String(Timex[4],DEC);
  setText += ":";
  setText += String(Timex[5],DEC);
  showNotification(setText.c_str());
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
  showNotification(setText.c_str());
  setText += " / ";
  tempoDePausa += setText;
  quntidade_pausa ++;
  resumedCounting = false; // Marca a função como executada após retomada
  IntervaloButton = false; // reseta button intervalo depois de retornar
  }
 }


