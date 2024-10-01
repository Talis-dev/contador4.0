extern uint32_t timeStopDescart, //min
timeDelaySensorDescart, //ms
hooksToRestartCount; //ganchos
 extern String horaInicio, tempoDePausa;

extern int CarretaPosition = 1,CarretaTotalAbatida = 0;
extern uint32_t Carreta_Abatida[18]={0}, Carreta_Descarte[18]={0};
extern bool ContadorON,InputPCF[];
extern int telaAtiva;

int countToSwap = 0;
unsigned long currentTimeSwap = 0, TimeBauncing = 0,lastCountTimeInterval = 0;
bool SwapActivate = false,functionExecuted = false;

void Contagem_Abatida() { 


  if(ContadorON){


 if(InputPCF[7] && InputPCF[6] && millis() - TimeBauncing > timeBauncingTrolleyPendura ){

 if(Carreta_Abatida[CarretaPosition] == 0){ // registra hora do inicio da contagem
  horaInicio = String(Timex[3],DEC);
  horaInicio += ":";
  horaInicio += String(Timex[4],DEC);
  horaInicio += ":";
  horaInicio += String(Timex[5],DEC);
   }

  Carreta_Abatida[CarretaPosition]++;
  CarretaTotalAbatida ++;

  lastCountTimeInterval = millis();  // Atualiza o tempo da última contagem
  functionExecuted = false; // Reseta a variável de controle

if(SwapActivate){ // inicia a contagem ate a defeiniçao para troca de carreta
  countToSwap++;
}
  TimeBauncing = millis();

 } 

SwapTrailer();
setIntervalTime();
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
if(CarretaPosition <= 17){
  CarretaPosition++;
  String setText = "CARRETA N";
  setText += String(CarretaPosition,DEC);
  setText += " INICIADA!";
  showNotification(setText.c_str());
}else{
  CarretaPosition=1;}


}

}

void setIntervalTime(){

if (CarretaTotalAbatida > 0 && millis() - lastCountTimeInterval > breakTime && !functionExecuted) {
    executeFunction(); // Chama a função desejada
    functionExecuted = true; // Marca a função como executada
  }



  
}



