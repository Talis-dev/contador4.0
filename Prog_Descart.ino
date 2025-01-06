extern uint32_t timeStopDescart, //min
timeDelaySensorDescart, //ms
hooksToRestartCount, //ganchos
bt0var,// botao liga descarte
imagem5; // imagem norea ligada,desligada,em espera

//extern uint32_t Carreta_Abatida[], Carreta_Descarte[];

extern int CarretaPosition,CarretaTotalAbatida,timeOffDescart,telaAtiva, CarretaTotalDescarte;
extern bool ContadorON,InputPCF[],noreaDescartPower;
int  CarretaTotalDescarte = 0;

bool previousNoreaDescartPower  = false, sensorAtivado = false; 

unsigned long int currentTime = 0,currentTimeSensor= 0, ativacaoTime = 0;

void noreaDescart(){

if (bt0var){ // se estiver habilitado na ihm

if(InputPCF[3]){  // botao verde NO
  if(millis() - currentTime > 500){
currentTime = millis();
noreaDescartPower =! noreaDescartPower;
  }
  }


if (noreaDescartPower) { // norea ligada
    imagem5 = 4;
    CountDisposalPerTrailer();
    if(timeOffDescart >= timeStopDescart){
      noreaDescartPower = false;
    }
  } else {
    imagem5 = 3;
  }


}else{
imagem5 = 2;
noreaDescartPower = false; 
}

 if(noreaDescartPower != previousNoreaDescartPower){
  previousNoreaDescartPower = noreaDescartPower;
    bitWrite(saida1, 0, noreaDescartPower);  // Atualiza o shift register
    bitWrite(saida1, 1, noreaDescartPower);  // Atualiza o shift register
    updateShiftRegister();   
    dbSerial.println("norea descarte mudou de stado: "+ String(noreaDescartPower));
   }

}

void CountDisposalPerTrailer(){ // executa quando norea descarte estiver ativa

if(InputPCF[5] && InputPCF[4] ){  //sensor 5 frnago // sensor 4 trolley
  if(millis() - currentTimeSensor > timeDelaySensorDescart*10){
currentTimeSensor = millis();

//sensorAtivado = true;
//ativacaoTime = millis();
    CarretaTotalDescarte++;
    Carreta_Descarte[CarretaPosition]++;
    timeOffDescart = 0;
}
}

// Verifica se a variável foi ativada e se já se passaram 250ms
  /*if (sensorAtivado && millis() - ativacaoTime >= 250) {
    sensorAtivado = false;  // Após 250ms, define a variável como false
    
    CarretaTotalDescarte++;
    Carreta_Descarte[CarretaPosition]++;
    timeOffDescart = 0;
  }*/



}