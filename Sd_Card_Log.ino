extern String DateAndHora_Str = "", DataToFile = "",horaInicio = "",tempoDePausa = "Não houve Parada";
extern bool sdCardFault;


void Log_Carreta(){
showNotification("SALVANDO DADOS DA CARRETA!");
DataToFile +=".csv";

String carreta_position="Carreta ";
carreta_position += CarretaPosition;

String avesAbatida = String(Carreta_Abatida[CarretaPosition]);
String avesDescarte = String(Carreta_Descarte[CarretaPosition]);
 // Verifica se o arquivo já existe
bool fileExists = SD.exists(DataToFile);

File FileCsv = SD.open(DataToFile, FILE_APPEND);

if (FileCsv) {
  // Se o arquivo não existe, escreve a linha de cabeçalho
  if (!fileExists) {
     FileCsv.println("\"Carreta Nº\",\"Hora de Inicio\",\"Total Aves\",\"Total Descarte\",\"Parada/Intervalo\",\"Data e Hora Finalizada\"");  // CRIA COLUNAS

            dbSerial.println("Criando novo arquivo no sd card, file name:"+ DataToFile);
        }

    // Escreve os dados no arquivo
    FileCsv.print(carreta_position + ",");
    FileCsv.print(horaInicio + ",");
    FileCsv.print(avesAbatida + ",");
    FileCsv.print(avesDescarte + ",");
    FileCsv.print(tempoDePausa + ","); // parada ou intervalo
    FileCsv.println(DateAndHora_Str); //println finaliza a linha
    FileCsv.close();
    delay(100);
     //dbSerial.println("Gravação bem-sucedida no arquivo: " + DataToFile);
    LerArquivoCSV(DataToFile);
  } else {
   // se o arquivo não abriu, imprime um erro:
    dbSerial.println("ERRO NA GRAVAÇAO verifique o sdcard!");
       LerArquivoCSV(DataToFile);
       sdCardFault = true;
    } 



} // end void log_carreta

// Função para ler e imprimir o conteúdo de um arquivo CSV na Serial
void LerArquivoCSV(String nomeArquivo) {
    // Abre o arquivo no modo de leitura
    File file = SD.open(nomeArquivo);
    
    if (file) {
        // Imprime uma mensagem indicando que o arquivo está sendo lido
        dbSerial.println("Lendo o arquivo: " + nomeArquivo);

        // Lê o arquivo linha por linha
        while (file.available()) {
            String linha = file.readStringUntil('\n');
            dbSerial.println(linha); // Imprime cada linha na Serial
        }

        // Fecha o arquivo após a leitura
        file.close();
    } else {
        // Se o arquivo não abriu, imprime um erro
        dbSerial.println("ERRO AO ABRIR O ARQUIVO: " + nomeArquivo);
        sdCardFault = true;
    }
}



/*
void formatDH(){
if(QTpausa == 1){

FileCsv.print(" PAUSA AS: ");
FileCsv.print(pausaH1[0]);
FileCsv.print(":");
FileCsv.print(pausaH1[1]);
FileCsv.print(":");
FileCsv.print(pausaH1[2]);

FileCsv.print(" RETORNOU AS: ");
FileCsv.print(retornoH1[0]);
FileCsv.print(":");
FileCsv.print(retornoH1[1]);
FileCsv.print(":");
FileCsv.print(retornoH1[2]);

FileCsv.print(" ABATIDA NO ");
FileCsv.print(SVpausa);
FileCsv.print("º INTERVALO");

pausa += "DE " + String(pausaH1[0],DEC) + ":" + String(pausaH1[1],DEC) + "/" ;
pausa += "AS " + String(retornoH1[0],DEC) + ":" + String(retornoH1[1],DEC) + "," ;

QTpausa = 0;

Qpausa += String(SVpausa,DEC) + "º INTERVALO/" ;

} else {

FileCsv.print(" SEM PAUSA ");
FileCsv.print(" ABATIDA NO ");
FileCsv.print(SVpausa);
FileCsv.print("º INTERVALO");

pausa += "SEM PAUSA," ;

Qpausa += String(SVpausa,DEC) + "º INTERVALO/" ;


}

FileCsv.print(" FINALIZADA ");
FileCsv.print(" DATA: ");
FileCsv.print(HORA[0]);
FileCsv.print("/");
FileCsv.print(HORA[1]);
FileCsv.print("/");
FileCsv.print(HORA[2]);
FileCsv.print(" HORA: ");
FileCsv.print(HORA[3]);
FileCsv.print(":");
FileCsv.print(HORA[4]);
FileCsv.print(":");
FileCsv.println(HORA[5]);

}
*/