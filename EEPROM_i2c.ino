

void readEEPROM(){

 uint8_t data0[20];
  for (uint8_t i = 0; i < 20; i++) {
    data0[i] = MyEEPROM.read(i);
    Serial.print("Config ");
     Serial.print(i);
     Serial.print("[");
    Serial.print(data0[i], DEC);
    Serial.print("] ");
delay(50);
  }
 Serial.println();
 readConfig();
}

void wirteEEPROM(int pos,int val){
digitalWrite(WP_EEPROM,LOW); 
delay(30);
MyEEPROM.write(pos,val);
delay(30);
digitalWrite(WP_EEPROM,HIGH); 
}

 void CleanEEPROM(){

   for (uint8_t i = 0; i < 256; i++) {
digitalWrite(WP_EEPROM,LOW); 
MyEEPROM.write(i,0);
if(i>=255){digitalWrite(4,HIGH);}
 }

 }
