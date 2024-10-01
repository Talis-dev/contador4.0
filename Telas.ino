extern uint32_t atualizaVar,timeStopDescart;
extern uint32_t imagem1,imagem2,imagem3,imagem4,imagem5,imagem6;
int Ant1=0,Ant2=0,Ant3=0,Ant4=0,Ant5=0;
extern uint32_t botaoHRvar,diavar,mesvar,anovar,horavar,minutvar,segunvar;
extern int Numsemana, BitPcf,timeOffDescart;
int sethra[5] = {0};
extern int  soma = 0, telaAtiva = 0;
extern int BAR;
extern bool InputPCF[7] = {false} , sdCardFault, sdCardFaultPrevious = true, mensageActive = 1;
bool SinglePageLoad = false, notify = false;
char time_buf[30] = {0};
extern String voltFloat, DateAndHora_Str,DataToFile;
int Tempo_anterior = 0, soma2 = 0;
int long dlyihm = 0, tempo_espera = 0,tempo_espera2 = 0, dlay = 0, dlay4 = 0;
extern float tensaoMedia;
unsigned long currentTimeMessage = 0, currentTimeHorToTxt = 0;

//----------------void atualiza imagem --------------//

void InputsBitPCF(){
if(bitRead(BitPcf,0) == 0){InputPCF[0]=true;}else{InputPCF[0]=false;}
if(bitRead(BitPcf,1) == 0){InputPCF[1]=true;}else{InputPCF[1]=false;}
if(bitRead(BitPcf,2) == 0){InputPCF[2]=true;}else{InputPCF[2]=false;}
if(bitRead(BitPcf,3) == 0){InputPCF[3]=true;}else{InputPCF[3]=false;}
if(bitRead(BitPcf,4) == 0){InputPCF[4]=true;}else{InputPCF[4]=false;}
if(bitRead(BitPcf,5) == 0){InputPCF[5]=true;}else{InputPCF[5]=false;}
if(bitRead(BitPcf,6) == 0){InputPCF[6]=true;}else{InputPCF[6]=false;}
if(bitRead(BitPcf,7) == 0){InputPCF[7]=true;}else{InputPCF[7]=false;}
}


void Varredura() {  // le a tela selecionada 100ms

Contagem_Abatida();// chama funçao de contagem apos verificar as entradas


if(millis() - dlay4 > 1000){ // faz a conversão a cada 1 seg
dlay4 = millis();


time_buf[30] = {0};
String HR;
HR += "DATA ";                 /// formatação da hora e data
HR += String(Timex[0],DEC);
DataToFile="/";
DataToFile += String(Timex[0],DEC);
HR += "/";
HR += String(Timex[1],DEC);
DataToFile += String(Timex[1],DEC);
HR += "/";
HR += String(Timex[2],DEC);
DataToFile += String(Timex[2],DEC);
HR += " HORA ";
HR += String(Timex[3],DEC);
HR += ":";
HR += String(Timex[4],DEC);
HR += ":";
HR += String(Timex[5],DEC);
DateAndHora_Str=HR; // string para salvar no sd card
HR.toCharArray(time_buf, 30); // converte string para char com tamanho 30
}


if(telaAtiva==0){               // sequencial de telas // carrega somente a tela aberta
// tela home 
if(sdCardFaultPrevious){
  sdCardFaultPrevious = false;

  if(sdCardFault){
    imagem6 = 5; // sd em falha
    }else{
      imagem6 = 6;} // sd ok

p5.setPic(imagem6);
p4.setPic(imagem5);
}

if(InputPCF[7]){
imagem1=0;
 }else{imagem1=1;}

 if(InputPCF[6]){ 
imagem2=0;
 }else{imagem2=1;}

 if(InputPCF[5]){ 
imagem3=0;
 }else{imagem3=1;}

  if(InputPCF[4]){ 
imagem4=0;
 }else{imagem4=1;}


if(imagem1 != Ant1){Ant1 = imagem1;p0.setPic(imagem1);  }

if(imagem2 != Ant2){Ant2 = imagem2;p1.setPic(imagem2);  }

if(imagem3 != Ant3){Ant3 = imagem3;p2.setPic(imagem3);  }

if(imagem4 != Ant4){Ant4 = imagem4;p3.setPic(imagem4);  }

if(imagem5 != Ant5){Ant5 = imagem5;p4.setPic(imagem5);  }


if(millis() - tempo_espera > 1010){
tempo_espera = millis();

/*
if(!mensageActive){
hor.setText(time_buf); // envia txt formato char data e hora  
 Num7.setValue(Numsemana+1);
 }*/


char bat_buf[5];
voltFloat.toCharArray(bat_buf, 5);
bat.setText(bat_buf);

char timeDesc_buf[4];
int timeDec = timeStopDescart - timeOffDescart;
itoa(timeDec,timeDesc_buf, 10);
t12.setText(timeDesc_buf); // envia tempo corrido para desligar descarte


if(ContadorON){


static int positionAnterior=0,
abatidaAnterior=0,
descartAnterior=0;

if(positionAnterior != CarretaPosition){
  C0.setValue(CarretaPosition);
  positionAnterior = CarretaPosition; 
}
if(abatidaAnterior != CarretaTotalAbatida){
C1.setValue(Carreta_Abatida[CarretaPosition]);
C4.setValue(CarretaTotalAbatida);
}

if(descartAnterior != CarretaTotalDescarte){
C2.setValue(Carreta_Descarte[CarretaPosition]);
C3.setValue(CarretaTotalDescarte);
}


}
}

}else{sdCardFaultPrevious = true;} // inverte o stado somente para obter feedback


if(telaAtiva == 5){
  if(millis() - tempo_espera > 1010){
tempo_espera = millis();
  hor2.setText(time_buf); // envia txt formato char data e hora
  }
}

}// end void varredura acoes das telas




void DateHora(){ //ajusta hora rtc
dia.getValue(&diavar);
sethra[0] = diavar;
mes.getValue(&mesvar);
sethra[1] = mesvar;
ano.getValue(&anovar);
sethra[2] = anovar;
hora.getValue(&horavar);
sethra[3] = horavar;
minut.getValue(&minutvar);
sethra[4] = minutvar;
segun.getValue(&segunvar);
sethra[5] = segunvar;
delay(100);

rtc.adjust(DateTime(sethra[2],sethra[1],sethra[0],sethra[3],sethra[4],sethra[5])); // sethra rtc ano/mes/dia  /hora/minuto/segundo
titulo.setText("SUCESSO"); 

  delay(100);
}

void notifyAndHorTxt(){

if(telaAtiva == 0 && millis() - currentTimeHorToTxt >= 1001){
currentTimeHorToTxt = millis();


if(mensageActive && millis() - currentTimeMessage >= notificationDuration){ // tempo de duraçao da notificaçao
//bt1.setText("CONTADOR  ON");
mensageActive = false;
sendCommand("vis nfy,0");
dbSerial.println("mensagem finalizada");

} 

if(!mensageActive){
 hor.setText(time_buf); // envia txt formato char data e hora  
 //Num7.setValue(Numsemana+1);
 semana.setText(DiasDaSemana[semanaN[1]]);
}
 }
}

void showNotification(const char* message) {
  currentTimeMessage = millis();
   mensageActive = true;
    sendCommand("vis nfy,1");
    delay(20);
    mensageActive = true;
    nfy.setText(message);
    dbSerial.println("mensagem iniciada");
    //sendCommand("vis nfy,0");
}


// Função para enviar comandos ao Nextion
void sendCommand(String cmd) {
    Serial2.print(cmd);
    Serial2.write(0xFF); // Envia três bytes 0xFF para indicar o fim do comando
    Serial2.write(0xFF);
    Serial2.write(0xFF);
}