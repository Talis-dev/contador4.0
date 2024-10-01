

  //  va0.setValue(tempoAtt);         //variavel de retorno para nextion confirmando atualizacao
  //    atualiza.setValue(ds0); // retorna estado 0 para o botao ds  



void Load_Abatido(){
delay(200);
n0.setValue(Carreta_Abatida[1]);
n1.setValue(Carreta_Abatida[2]);
n2.setValue(Carreta_Abatida[3]);
n3.setValue(Carreta_Abatida[4]);
n4.setValue(Carreta_Abatida[5]);
n5.setValue(Carreta_Abatida[6]);
n6.setValue(Carreta_Abatida[7]);
n7.setValue(Carreta_Abatida[8]);
n8.setValue(Carreta_Abatida[9]);
n9.setValue(Carreta_Abatida[10]);
n10.setValue(Carreta_Abatida[11]);
n11.setValue(Carreta_Abatida[12]);
n12.setValue(Carreta_Abatida[13]);
n13.setValue(Carreta_Abatida[14]);
n14.setValue(Carreta_Abatida[15]);
n15.setValue(Carreta_Abatida[16]);
n16.setValue(Carreta_Abatida[17]);
}


void Load_Descarte() {
  delay(200); // Atraso para garantir que a tela seja exibida

  n0.setValue(Carreta_Descarte[1]);
  n1.setValue(Carreta_Descarte[2]);
  n2.setValue(Carreta_Descarte[3]);
  n3.setValue(Carreta_Descarte[4]);
  n4.setValue(Carreta_Descarte[5]);
  n5.setValue(Carreta_Descarte[6]);
  n6.setValue(Carreta_Descarte[7]);
  n7.setValue(Carreta_Descarte[8]);
  n8.setValue(Carreta_Descarte[9]);
  n9.setValue(Carreta_Descarte[10]);
  n10.setValue(Carreta_Descarte[11]);
  n11.setValue(Carreta_Descarte[12]);
  n12.setValue(Carreta_Descarte[13]);
  n13.setValue(Carreta_Descarte[14]);
  n14.setValue(Carreta_Descarte[15]);
  n15.setValue(Carreta_Descarte[16]);
  n16.setValue(Carreta_Descarte[17]);
  
}

/*
void Load_Descarte() {
  unsigned long startMillis = millis();
  unsigned long currentMillis;
  const long interval = 50; // Intervalo de 50 milissegundos entre os envios

  int values[] = {varN17, varN18, varN19, varN20, varN21, varN22, varN23, varN24, varN25, varN26, varN27, varN28, varN29, varN30, varN31, varN32, varN33};
  NexNumber* nexNumbers[] = {&n0, &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9, &n10, &n11, &n12, &n13, &n14, &n15, &n16};

  for (int i = 0; i < 17; i++) {
    currentMillis = millis();
    while (currentMillis - startMillis < interval) {
      currentMillis = millis();
    }
    nexNumbers[i]->setValue(values[i]);
    startMillis = currentMillis;
  }
}
*/
