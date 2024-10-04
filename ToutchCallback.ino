extern uint32_t bt0var,bt1var;
extern int telaAtiva;
extern bool ContadorON=false, IntervaloButton=false;

void nexattachPops(){
b0.attachPop(b0call, &b0); //chama tela carretas
b1.attachPop(b1call, &b1);//chama tela menu carretas
b2.attachPop(b2call, &b2);//bt pulso itervalo na tela0
mt.attachPop(mtcall, &mt);//bt chama tela mortalidade
cf.attachPop(cfcall, &cf);//bt chama tela config
cfs.attachPop(cfscall, &cfs);//bt salvar configuraçoes

z1.attachPop(z1call, &z1);//bt zera total page menu
z2.attachPop(z2call, &z2);//bt zera descarte page menu
z3.attachPop(z3call, &z3);//bt zera tudo page menu
z4.attachPop(z4call, &z4);//bt zera carreta page menu
z5.attachPop(z5call, &z5);//bt alterar carreta page menu
z6.attachPop(z6call, &z6);//bt alterar carreta para abater agr page menu

hor.attachPop(horcall, &hor);  //chama tela ajuste hora

vt1.attachPop(vt1call, &vt1); // botao voltar tela carretas
vt2.attachPop(vt2call, &vt2); // botao voltar tela ajute hora
vt3.attachPop(vt3call, &vt3); // botao voltar tela  menu
vt4.attachPop(vt4call, &vt4); // botao voltar tela  carretas mortalidade
vt5.attachPop(vt5call, &vt5); // botao voltar tela  config

botaoHR.attachPop(botaoHRcall, &botaoHR); // botao enviar data e hora 


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

void b2call(void *ptr) { // botao intervalo
 dbSerial.println("Botao intervalo prescionado");
 IntervaloButton = true;
 showNotification("Aguardando PARADA para intervalo!",3);
  }

void bt0call(void *ptr) { // bt liga desliga norea descarte
dbSerial.println("Botao bt0 prescionado!");
bt0.getValue(&bt0var);
}

void bt1call(void *ptr) { // bt abilita contador
dbSerial.println("Botao bt1 prescionado!");
bt1.getValue(&bt1var);
ContadorON = bt1var;
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
for (int i = 0; i < 18; i++) {
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

}

//--------------------------------------------------------//------------- //-----------------------------------------------------------//