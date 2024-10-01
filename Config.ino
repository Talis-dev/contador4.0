extern uint32_t timeStopDescart = 1, // min
timeDelaySensorDescart = 2, // ms
hooksToRestartCount = 3, // ganchos
timeBauncingTrolleyPendura = 300, //
notificationDuration = 1000, // tempo de duraçao da notificçao na home page
breakTime = 10; // tempo de norea parada para registrar horario de pausa


extern bool sdCardFault;




void writeConfig(){

cf1.getValue(&timeStopDescart);
cf2.getValue(&timeDelaySensorDescart);

cf5.getValue(&notificationDuration);
cf6.getValue(&hooksToRestartCount);
cf7.getValue(&timeBauncingTrolleyPendura);
cf8.getValue(&breakTime);


config.timeStopDescart = timeStopDescart;
config.timeDelaySensorDescart = timeDelaySensorDescart;
config.hooksToRestartCount = hooksToRestartCount;
config.timeBauncingTrolleyPendura = timeBauncingTrolleyPendura;
config.notificationDuration = notificationDuration;
config.breakTime = breakTime;

 File configFile = SD.open("/config.bin", FILE_WRITE);

  if (!configFile) {
        dbSerial.println("Erro ao abrir config.bin para escrita.");
        sdCardFault = true;
        return;
    }

    // Escreve a estrutura config no arquivo binário
    configFile.write((uint8_t*)&config, sizeof(config));
    configFile.close();
    dbSerial.println("Configurações salvas com sucesso.");

delay(50);
txtcf.setText("Salvo com Sucesso!");
readConfig();
}

void readConfig(){
  // Abre o arquivo config.bin no modo de leitura binária
    File configFile = SD.open("/config.bin", FILE_READ);

  if (!configFile) {
        dbSerial.println("Erro ao abrir config.bin para leitura.");
        sdCardFault = true;
        return;
    }

    // Lê o conteúdo do arquivo e armazena na estrutura config
    configFile.read((uint8_t*)&config, sizeof(config));
    configFile.close();
    dbSerial.println("Configurações carregadas com sucesso.");


timeStopDescart = config.timeStopDescart;
timeDelaySensorDescart = config.timeDelaySensorDescart;
hooksToRestartCount = config.hooksToRestartCount;
timeBauncingTrolleyPendura = config.timeBauncingTrolleyPendura;
notificationDuration = config.notificationDuration;
breakTime = config.breakTime;

    // Exibe os valores lidos
    dbSerial.println("timeStopDescart: " + String(config.timeStopDescart));
    dbSerial.println("timeDelaySensorDescart: " + String(config.timeDelaySensorDescart));
    dbSerial.println("hooksToRestartCount: " + String(config.hooksToRestartCount));
    dbSerial.println("timeBauncingTrolleyPendura: " + String(config.timeBauncingTrolleyPendura));
    dbSerial.println("notificationDuration: " + String(config.notificationDuration));
    dbSerial.println("breakTime: " + String(config.breakTime));


}


void setConfigDisplay(){
cf1.setValue(timeStopDescart);
cf2.setValue(timeDelaySensorDescart);

cf5.setValue(notificationDuration);
cf6.setValue(hooksToRestartCount);
cf7.setValue(timeBauncingTrolleyPendura);
cf8.setValue(breakTime);
}

