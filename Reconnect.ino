extern int reboot = 0;
unsigned long lastReconnectAttempt = 0;  // Armazena o tempo do último loop de tentativa de conexão
const long reconnectInterval = 8000;     // Intervalo de 5 segundos para tentar reconectar

void reconnect() {
  if (!client.connected() && reboot < 9) {
    unsigned long currentMillis = millis();  // Tempo atual

    // Verifica se o intervalo de tempo entre as tentativas foi atingido
    if (currentMillis - lastReconnectAttempt >= reconnectInterval) {
      lastReconnectAttempt = currentMillis;  // Atualiza o tempo da última tentativa

      testaEthernet();
      dbSerial.print("Conectando ao broker MQTT...");
      showNotification("Conectando ao broker MQTT...",2);
      
      // Tenta se conectar ao broker MQTT
      if (client.connect("ContadorStaClient")) {
        Serial.println("ok Conectado ao broker MQTT!");
        client.publish("contadorSta", "Contador STA V4.0 conectado!");
        showNotification("Contador STA V4.0 conectado!",2);
        Ssubscribe();  // Inscreve nos tópicos necessários
        reboot = 0;    // Reseta o contador de tentativas
      } else {
          String mqttError = "ERROR: ";
          mqttError += client.state();
          mqttError +=" Falha na conexao.";
          showNotification(mqttError.c_str(),1);
          conectMQTT();

        dbSerial.println(mqttError);
        reboot++;
        
        // Verifica se excedeu o número máximo de tentativas
        if (reboot >= 8) {
          // Realiza uma ação específica, como reiniciar o ESP, se necessário
          //ESP.restart();
          showNotification("Exedido limite de tentativas reconexao",1);
        }
      }
    }
  }
}
