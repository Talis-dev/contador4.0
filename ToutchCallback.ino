extern uint32_t bt0var,bt1var;
extern int telaAtiva;
extern bool ContadorON, IntervaloButton;
bool ContadorON=false, IntervaloButton=false;

void nexattachPops(){
b0.attachPop(b0call, &b0);//bt chama tela carretas
b1.attachPop(b1call, &b1);//bt chama tela menu carretas
b2.attachPop(b2call, &b2);//bt chama tela ne horarios paradas
mt.attachPop(mtcall, &mt);//bt chama tela mortalidade
ed.attachPop(edcall, &ed);//bt chama tela editar dados da carreta
cf.attachPop(cfcall, &cf);//bt chama tela config
rd.attachPop(rdcall, &rd);//bt chama tela rede

cfs.attachPop(cfscall, &cfs);//bt salvar configuraçoes

evp.attachPop(evpcall, &evp);//bt salvar configuraçoes

p5.attachPop(p5call, &p5);//imagem db on/off, cptura toque


z0.attachPop(z0call, &z0);//bt  salvar e finalizar abate
z1.attachPop(z1call, &z1);//bt zera total page menu
z2.attachPop(z2call, &z2);//bt zera descarte page menu
z3.attachPop(z3call, &z3);//bt zera tudo page menu
z4.attachPop(z4call, &z4);//bt zera carreta page menu
z5.attachPop(z5call, &z5);//bt alterar carreta page menu
z6.attachPop(z6call, &z6);//bt alterar carreta para abater agr page menu

hor.attachPop(horcall, &hor);  //chama tela ajuste hora
botaoHR.attachPop(botaoHRcall, &botaoHR); // botao enviar data e hora 

vt1.attachPop(vt1call, &vt1); // botao voltar tela carretas
vt2.attachPop(vt2call, &vt2); // botao voltar tela ajute hora
vt3.attachPop(vt3call, &vt3); // botao voltar tela  menu
vt4.attachPop(vt4call, &vt4); // botao voltar tela  carretas mortalidade
vt5.attachPop(vt5call, &vt5); // botao voltar tela  config
vt6.attachPop(vt6call, &vt6); // botao voltar tela  editar

ps.attachPop(pscall, &ps); // botao pesquisar carreta
env.attachPop(envcall, &env); // botao enviar alteraçoes


bt0.attachPop(bt0call, &bt0);
bt1.attachPop(bt1call, &bt1);

}


//--------------------------------------------------------// page 0 home  //-----------------------------------------------------------//
void b0call(void *ptr) { // botao na page 0 para ir p/ page 1
 dbSerial.println("Tela ativa 1!");
 telaAtiva = 1;
 Load_Abatido();
 //Load_timers(); // executa o envio dos dados para tela 1, dlay 500ms
}

void horcall(void *ptr) { // chama tela ajuste hora
  dbSerial.println("Tela ativa 5!");
 telaAtiva = 5; }

void b1call(void *ptr) { // botao pge 4 menu
 dbSerial.println("Tela ativa 4!");
 telaAtiva = 4; }

void b2call(void *ptr) { // botao page horarios de parada
 dbSerial.println("Tela ativa 8!");
  telaAtiva = 8;
  
  }

void bt0call(void *ptr) { // bt liga desliga norea descarte
dbSerial.println("Botao bt0 prescionado!");
bt0.getValue(&bt0var);
}

void bt1call(void *ptr) { // bt abilita contador
dbSerial.println("Botao bt1 prescionado!");
bt1.getValue(&bt1var);
ContadorON = bt1var;
if(!bt1var){
pulsesPerMinute = 0;
pulsesPerHour = 0;
}
}

void p5call(void *ptr) { // bt liga desliga norea descarte
dbSerial.println("Evento de toque imagem p5 prescionado!");
reboot = 0; // reseta o contador para tentar se reeconectar ao mqtt
reconnect();
}
//--------------------------------------------------------//------------- //-----------------------------------------------------------//


//--------------------------------------------------------// page 1 carretas  //-----------------------------------------------------------//
void vt1call(void *ptr) {// botao voltar na pge 1
 dbSerial.println("Tela ativa 0!");
 telaAtiva = 0; }

void mtcall(void *ptr) { // bt chama page 3
   dbSerial.println("Tela ativa 3!");
 telaAtiva = 3;
  Load_Descarte();
}
//--------------------------------------------------------//------------- //-----------------------------------------------------------//

//--------------------------------------------------------// page 3 mortalidade  //-----------------------------------------------------------//

void vt4call(void *ptr) { // bt voltar na page 3 mort para page 1 carretas
   dbSerial.println("Tela ativa 1!");
 telaAtiva = 1;
 Load_Abatido();
}
void edcall(void *ptr) {
  dbSerial.println("Tela ativa 7!");
 telaAtiva = 7;
}

//--------------------------------------------------------//------------- //-----------------------------------------------------------//


//--------------------------------------------------------// page 5 adj hora  //-----------------------------------------------------------//
void vt2call(void *ptr) { // botao voltar na pge 5 aj hora
 dbSerial.println("Tela ativa 0!");
 telaAtiva = 0; }

void botaoHRcall(void *ptr) {
  dbSerial.println("Data e hora atualizada com sucesso!");
 DateHora();}
//--------------------------------------------------------//------------- //-----------------------------------------------------------//


//--------------------------------------------------------// page 6 config  //-----------------------------------------------------------//

void vt5call(void *ptr) {  // bt voltar da tela config
     dbSerial.println("Tela ativa 4!");
 telaAtiva = 4;
}

void cfscall(void *ptr) { // bt salva config
   dbSerial.println("Salvando configurações...");
   writeConfig();
}

void rdcall(void *ptr) { // bt page rede
   dbSerial.println("Tela ativa 9!");
 telaAtiva = 9;
 ipct.setText(ipAddress);
 sid.setText(ssid);
 
 char mqttIpStr[16];
  ipAddressToChar(mqtt_ip, mqttIpStr, sizeof(mqttIpStr));
  ipmq.setText(mqttIpStr);
  ptmq.setValue(mqtt_port);
}

//--------------------------------------------------------//------------- //-----------------------------------------------------------//


//--------------------------------------------------------// page 7 editar  //-----------------------------------------------------------//

void vt6call(void *ptr) {  // bt voltar da tela editar
     dbSerial.println("Tela ativa3!");
 telaAtiva = 3;
 Load_Descarte();
}

void pscall(void *ptr) { // bt pesquisar carreta
uint32_t carreta = 0;
   dbSerial.println("localizando...");
 edn.getValue(&carreta);
 //ab.setValue(Carreta_Abatida[carreta]);
 //ad.setValue(Carreta_Descarte[carreta]);

String numCarreta = String(carreta);
client.publish("setDataEdit", numCarreta.c_str());
}

void envcall(void *ptr) { // bt salva alteraçao
uint32_t carreta = 0, CA = 0, CD = 0;
   dbSerial.println("Salvando alterações...");
 edn.getValue(&carreta);
 ab.getValue(&CA);
 ad.getValue(&CD);

 Carreta_Abatida[carreta] = CA;
 Carreta_Descarte[carreta] = CD;

 if(editDataCarreta(CA,CD,carreta)){
 edn.setValue(0);
 ab.setValue(0);
 ad.setValue(0);    
  }

}

//--------------------------------------------------------//------------- //-----------------------------------------------------------//

//--------------------------------------------------------// page 9 rede  //-----------------------------------------------------------//
void evpcall(void *ptr) { // bt enviar alteraçao
char mqtt_ip_text[16];

 dbSerial.println("alterando...");

 ipmq.getText(mqtt_ip_text, sizeof(mqtt_ip_text));
 IPAddress mqtt_ip = parseIPAddress(mqtt_ip_text);
 preferences.putString("mqtt_ip", mqtt_ip_text);

 ptmq.getValue(&mqtt_port); 
 preferences.putUInt("mqtt_port", mqtt_port);

 conectMQTT();
dbSerial.println("...finalizado"); 
}


//--------------------------------------------------------//------------- //-----------------------------------------------------------//



//--------------------------------------------------------// page 4 menu  //-----------------------------------------------------------//

void cfcall(void *ptr) { // bt chama tela config
   dbSerial.println("Tela ativa 6!");
telaAtiva = 6;
delay(100);
setConfigDisplay();
}

void vt3call(void *ptr) { // botao voltar na pge 4
 dbSerial.println("Tela ativa 0!");
 telaAtiva = 0; }

void z0call(void *ptr) { //bt finaliza abate
tempoDePausa = "Não houve Parada";
postDataServer();// envia a ultima carreta e limpa os dados
finalizaAbate();// executa funcao para voltar do inicio
notificationSD = true; 
telaAtiva = 0;
}

void z1call(void *ptr) { //bt zera total
CarretaTotalAbatida = 0;
}

void z2call(void *ptr) {//bt zera descarte
CarretaTotalDescarte = 0;
}

void z3call(void *ptr) {//bt zera tudo
CarretaTotalAbatida = 0;
CarretaTotalDescarte = 0;
CarretaPosition = 1;
for (int i = 0; i < 21; i++) {
        Carreta_Abatida[i] = 0;
        Carreta_Descarte[i] = 0;
    }
}

void z4call(void *ptr) {//bt zera carreta
uint32_t carretaSelecionada = 0;
uint32_t zera = 0;
nc1.getValue(&carretaSelecionada);
Carreta_Abatida[carretaSelecionada] = 0;
Carreta_Descarte[carretaSelecionada] = 0;
delay(50);
nc1.setValue(zera);
nc2.setValue(zera);
}

void z5call(void *ptr) {//bt alterar carreta
uint32_t carretaSelecionada = 0,
carretaSelecionadaPara = 0,
valorcarretaSelecionada = 0,
valorDescarteSelecionada = 0,
valorUpdateCarreta = 0,
valorUpdateDescarte = 0,
zera = 0;
nc1.getValue(&carretaSelecionada);
nc3.getValue(&carretaSelecionadaPara);

valorcarretaSelecionada = Carreta_Abatida[carretaSelecionada];
valorDescarteSelecionada = Carreta_Descarte[carretaSelecionada];

valorUpdateCarreta = Carreta_Abatida[carretaSelecionadaPara];
valorUpdateDescarte = Carreta_Descarte[carretaSelecionadaPara];
delay(50);

Carreta_Abatida[carretaSelecionadaPara] = valorcarretaSelecionada;
Carreta_Descarte[carretaSelecionadaPara] = valorDescarteSelecionada;

Carreta_Abatida[carretaSelecionada] = valorUpdateCarreta;
Carreta_Descarte[carretaSelecionada] = valorUpdateDescarte;

delay(50);
nc1.setValue(zera);
nc2.setValue(zera);
nc3.setValue(zera);
}

void z6call(void *ptr) {//bt alterar carreta para abater agora
uint32_t carretaSelecionada = 0,
valorUpdateCarreta = 0,
valorUpdateDescarte = 0,
zera = 0;
nc1.getValue(&carretaSelecionada);
valorUpdateCarreta = Carreta_Abatida[CarretaPosition]; // pega o valor atual
valorUpdateDescarte = Carreta_Descarte[CarretaPosition];

Carreta_Abatida[CarretaPosition] = 0; // zera o valor 
Carreta_Descarte[CarretaPosition] = 0;
delay(50);

Carreta_Abatida[carretaSelecionada] = valorUpdateCarreta; //seta o valor atual para a posiçao selecionada
Carreta_Descarte[carretaSelecionada] = valorUpdateDescarte;
CarretaPosition = carretaSelecionada; // seta a nova posiçao atual
delay(50);
nc1.setValue(zera);
nc2.setValue(zera);
nc3.setValue(zera);
}

//--------------------------------------------------------//------------- //-----------------------------------------------------------//


IPAddress parseIPAddress(char* ip_str) { 
  byte ip_bytes[4]; 
  sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", &ip_bytes[0], &ip_bytes[1], &ip_bytes[2], &ip_bytes[3]); 
  return IPAddress(ip_bytes); 
  }
void ipAddressToChar(IPAddress ip, char* buffer, size_t size) { 
  snprintf(buffer, size, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
   }
