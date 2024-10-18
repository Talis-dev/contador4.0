
 

int reboot = 0;

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected() && reboot < 8) {
   dbSerial.println(" MQTT connection... ");

    if (client.connect("ContadorStaClient")) {
     dbSerial.println("connected MQTT");
     showNotification("CLIENTE CONECTADO!",3);
     digitalWrite(2, HIGH); 
     client.publish("contadorSta", "Contador STA v4 conectado!");
 
      // ... and resubscribe
      Ssubscribe();
delay(10);
    } else {
     showNotification("FALHA CLIENTE DESCONECTADO! reconectando...",1);

     dbSerial.print("FALHA, client status = ");
     dbSerial.print(client.state());
     dbSerial.print(" NOVA TENTATIVA, Nº");
     dbSerial.print(reboot);
     dbSerial.println(" - RESET EM 3 SEGUNDOS");
     reboot++;
    digitalWrite(2, HIGH);                           
    dbSerial.println("tentando se conectar ao server..");  
    delay(1000);    
    digitalWrite(2, LOW);
    delay(1000); 

      if(reboot >= 8){ 
        showNotification("FALHA NO MQTT! Reinicie o contador",1);
        //ESP.restart();
        }
    }
  }




}
