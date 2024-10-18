
extern int ehternet_conected = 0;

void WizReset() {

  /* Função de reinicialização do Wiz W5500. Altere isso para a redefinição específica
 sequência necessária para sua placa ou módulo específico.*/
    dbSerial.print("Resetting Wiz Ethernet Board...  ");
    pinMode(RESET_P, OUTPUT);
    digitalWrite(RESET_P, HIGH);
    delay(100);
    digitalWrite(RESET_P, LOW);
    delay(150);
    digitalWrite(RESET_P, HIGH);
    delay(550);
    dbSerial.println("Done.");
}


void prt_hwval(uint8_t refval) {
    switch (refval) {
    case 0:
        dbSerial.println("Nenhum hardware detectado.");
        break;
    case 1:
        dbSerial.println("WizNet W5100 detectado.");
        break;
    case 2:
        dbSerial.println("WizNet W5200 detectado.");
        break;
    case 3:
        dbSerial.println("WizNet W5500 detectado.");
        break;
    default:
        dbSerial.println
            ("DESCONHECIDO - Atualize espnow_gw.ino para corresponder a Ethernet.h");
    }
}


void prt_ethval(uint8_t refval) {
    switch (refval) {
    case 0:
        dbSerial.println("Estado desconhecido.");
        break;
    case 1:
        dbSerial.println("Link sinalizado como UP");
        break;
    case 2:
        dbSerial.println("Link sinalizado como DOWN. Verificar a conexão do cabo.");
        break;
    default:
        dbSerial.println
            ("DESCONHECIDO - Atualize espnow_gw.ino para corresponder a Ethernet.h");
    }
}


void testaEthernet(){
//Verificações de integridade do W5500 e da conexão do cabo.
    dbSerial.print("Checking connection.");
    bool rdy_flag = false;
    for (uint8_t i = 0; i <= 20; i++) {
        if ((Ethernet.hardwareStatus() == EthernetNoHardware)
            || (Ethernet.linkStatus() == LinkOFF)) {
            dbSerial.print(".");
            rdy_flag = false;
            delay(100);
        } else {
            rdy_flag = true;
            break;
        }
    }
    if (rdy_flag == false) {
        dbSerial.println
            ("\n\r\tHardware fault, or cable problem... cannot continue.");
        dbSerial.print("Hardware Status: ");
        prt_hwval(Ethernet.hardwareStatus());
        dbSerial.print("   Cable Status: ");
        prt_ethval(Ethernet.linkStatus());
        ehternet_conected = 0;
       // while (true) {
          delay(100);          // Halt.
      // }
    } else {
        dbSerial.println(" Ethernet OK");
        ehternet_conected = 1;
    }

}