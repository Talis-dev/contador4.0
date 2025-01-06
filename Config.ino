extern uint32_t timeStopDescart,timeDelaySensorDescart,hooksToRestartCount,timeBauncingTrolleyPendura,notificationDuration,breakTime,warningLight;

uint32_t timeStopDescart = 2, // min
timeDelaySensorDescart = 20, // ms
hooksToRestartCount = 15, // ganchos
timeBauncingTrolleyPendura = 20, //
notificationDuration = 2, // tempo de duraçao da notificçao na home page
breakTime = 1, // tempo de norea parada para registrar horario de pausa
warningLight = 1;// tempo de luz qando troca de carreta


void writeConfig(){

cf1.getValue(&timeStopDescart);
cf2.getValue(&timeDelaySensorDescart);

cf4.getValue(&warningLight);
cf5.getValue(&notificationDuration);
cf6.getValue(&hooksToRestartCount);
cf7.getValue(&timeBauncingTrolleyPendura);
cf8.getValue(&breakTime);


if (MyEEPROM.read(1) != timeStopDescart) { MyEEPROM.write(1,timeStopDescart); }
if (MyEEPROM.read(2) != timeDelaySensorDescart) { MyEEPROM.write(2,timeDelaySensorDescart); }

if (MyEEPROM.read(4) != warningLight) { MyEEPROM.write(4,warningLight); }
if (MyEEPROM.read(5) != notificationDuration) { MyEEPROM.write(5,notificationDuration); }
if (MyEEPROM.read(6) != hooksToRestartCount) { MyEEPROM.write(6,hooksToRestartCount); }
if (MyEEPROM.read(7) != timeBauncingTrolleyPendura) { MyEEPROM.write(7,timeBauncingTrolleyPendura); }
if (MyEEPROM.read(8) != breakTime) { MyEEPROM.write(8,breakTime); }



dbSerial.println("Configurações salvas com sucesso.");
delay(50);
txtcf.setText("Salvo com Sucesso!");
readConfig();
}

void readConfig(){


timeStopDescart = MyEEPROM.read(1);
timeDelaySensorDescart = MyEEPROM.read(2);

warningLight = MyEEPROM.read(4);
notificationDuration = MyEEPROM.read(5);
hooksToRestartCount = MyEEPROM.read(6);
timeBauncingTrolleyPendura = MyEEPROM.read(7);
breakTime = MyEEPROM.read(8);

dbSerial.println("Configurações carregadas com sucesso.");

}


void setConfigDisplay(){
cf1.setValue(timeStopDescart);
cf2.setValue(timeDelaySensorDescart);

cf4.setValue(warningLight);
cf5.setValue(notificationDuration);
cf6.setValue(hooksToRestartCount);
cf7.setValue(timeBauncingTrolleyPendura);
cf8.setValue(breakTime);
}
