/*
  This is a simple example show the Heltec.LoRa recived data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16

  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn

  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include "heltec.h"
#include "images.h"
#include "stdio.h"
#include "stdlib.h"

#define BAND 915E6  //you can set band here directly,e.g. 868E6,915E6
#define INTERVAL 500 
String Rssi = "RSSI --";
String packSize = "--";
String packet;
String ID;
String NS;


//Tempo do último envio
long lastSendTime = 0;
char NA [] = "00000";
char GW [] = "000";
int counterTotal1 = 0;
int counterTotal2 = 0;
int counterTotal3 = 0;
int counterRecebido1 = 0;
int counterRecebido2 = 0;
int counterRecebido3 = 0;
int counterPerdido1 = 0;
int counterPerdido2 = 0;
int counterPerdido3 = 0;
int rssi1 = 0;
int rssi2 = 0;
int rssi3 = 0;
int ns = 0;
int id = 0;
int naAtual = 0;
float rssiMedio1 = 0, rssiMedio2 = 0, rssiMedio3 = 0;
int initialNS1 = 0, initialNS2 = 0, initialNS3 = 0;
int counterID = 0;
int na1 = 0, na2 = 0, na3 = 0;
bool controle = 1;

void logo() {
  Heltec.display->clear();
  Heltec.display->drawXbm(0, 5, logo_width, logo_height, logo_bits);
  Heltec.display->display();
}

void LoRaData() {
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  //Heltec.display->drawString(0 , 0 , "Received " + packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0, 0 , 128, packet);


  Heltec.display->drawString(0, 10, "rssi medio1: " + String(rssiMedio1));
  Heltec.display->drawString(0, 20, "ID: " + ID);
  Heltec.display->drawString(0, 30, "NS: " + NS);
  Heltec.display->drawString(0, 40, "PacTot1: " + String(counterTotal1));
  Heltec.display->drawString(0, 50, "PacPerdido1: " + String(counterPerdido1));


  Heltec.display->display();
}

void cbk(int packetSize) {

 
  ID = "";
  NS = "";
  packet = "";

  packSize = String(packetSize, DEC);

 //int packetSize = Heltec.LoRa.parsePacket();
  Serial.println(packetSize);
  if (packetSize) {
   // cbk(packetSize);
    id = ID.toInt();
    //Serial.println(id);
    ns = NS.toInt();
    ident(id, ns);
  }


  for (int i = 0; i < 2; i++) {
    Heltec.LoRa.read();
  }
  for (int i = 2; i < 5; i++) {
    ID += (char) Heltec.LoRa.read();
  }
  Serial.println(ID);
  for (int i = 0; i < 2; i++) {
    Heltec.LoRa.read();
  }
  for (int i = 7; i < 12; i++) {
    NS += (char) Heltec.LoRa.read();
  }
  Serial.print("NS: ");
  Serial.println(NS);
  for (int i = 12; i < packetSize; i++) {
    packet += (char) Heltec.LoRa.read();
  }

  Rssi = "RSSI: " + String(Heltec.LoRa.packetRssi(), DEC) + "dBm" ;

  LoRaData();
}

void ident(int id, int ns) {

  if (id == 1)
  {
    if (initialNS1 == 0)
    {
      initialNS1 = ns;
    }
    counterTotal1 = ns - initialNS1+1;
    rssi1 += Heltec.LoRa.packetRssi();

    counterRecebido1 += 1;
    counterPerdido1 = counterTotal1 - counterRecebido1;
    
    rssiMedio1 = int(rssi1 / counterRecebido1);

    Serial.print("Rssi Medio1: ");
    Serial.println(rssiMedio1);
    Serial.print("Pacotes recebidos1: ");
    Serial.println(counterRecebido1);
    Serial.print("Pacotes perdidos1: ");
    Serial.println(counterPerdido1);
    Serial.print("Pacotes totais1: ");
    Serial.println(counterTotal1);
  }
  else if (id == 2)
  {
    if (initialNS2 == 0)
    {
      initialNS2 = ns;
    }
    counterTotal2 = ns - initialNS2+1;
    rssi2 += Heltec.LoRa.packetRssi();

    counterRecebido2 += 1;
    counterPerdido2 = counterTotal2 - counterRecebido2;
    
    rssiMedio2 = int(rssi2 / counterRecebido2);

    Serial.print("Rssi Medio2: ");
    Serial.println(rssiMedio2);
    Serial.print("Pacotes recebidos2: ");
    Serial.println(counterRecebido2);
    Serial.print("Pacotes perdidos2: ");
    Serial.println(counterPerdido2);
    Serial.print("Pacotes totais2: ");
    Serial.println(counterTotal2);

  }
  else if (id == 3)
  {
    if (initialNS3 == 0)
    {
      initialNS3 = ns;
    }
    counterTotal3 = ns - initialNS3+1;
    rssi3 += Heltec.LoRa.packetRssi();
    counterRecebido3 += 1;
    
    counterPerdido3 = counterTotal3 - counterRecebido3;
    rssiMedio3 = int(rssi3 / counterRecebido3);

    Serial.print("Rssi Medio3: ");
    Serial.println(rssiMedio3);
    Serial.print("Pacotes recebidos3: ");
    Serial.println(counterRecebido3);
    Serial.print("Pacotes perdidos3: ");
    Serial.println(counterPerdido3);
    Serial.print("Pacotes totais3: ");
    Serial.println(counterTotal3);

  }
/*
  Serial.print("Rssi Medio1: ");
  Serial.println(rssiMedio1);
  Serial.print("Rssi Medio2: ");
  Serial.println(rssiMedio2);
  Serial.print("Rssi Medio3: ");
  Serial.println(rssiMedio3);
  Serial.print("Pacotes recebidos1: ");
  Serial.println(counterRecebido1);
  Serial.print("Pacotes perdidos1: ");
  Serial.println(counterPerdido1);
  Serial.print("Pacotes totais1: ");
  Serial.println(counterTotal1);
  */
  LoRaData();
}

void gateway() {
  counterID = counterID % 3;
  controle = 0;
  if (counterID == 0)
  {
    na1 += 1;
    naAtual = na1;
   // delay(3000);
  }
  if (counterID == 1)
  {
    na2 += 1;
    naAtual = na2;
    //delay(3000);
  }
  if (counterID == 2)
  {
    na3 += 1;
    naAtual = na3;
   // delay(3000);
  }
  counterID += 1;

  /*int num = atoi(counter);

    num++;*/

  char temp[6]; // Buffer temporário com tamanho para 5 caracteres + terminador nulo

  snprintf(temp, sizeof(temp), "%05d", naAtual);

  strcpy(NA, temp); // Copia o valor do buffer temporário para a string counter

  char temp2[4]; // Buffer temporário com tamanho para 5 caracteres + terminador nulo

  snprintf(temp2, sizeof(temp2), "%03d", counterID);

  strcpy(GW, temp2); // Copia o valor do buffer temporário para a string counter

  Heltec.LoRa.beginPacket();
  Heltec.LoRa.print("GW"); // Alterar o valor do ID que está enviando
  Heltec.LoRa.print(GW);
  Heltec.LoRa.print("NS");
  Heltec.LoRa.print(NA);
  Heltec.LoRa.print("CONTROL");
  Heltec.LoRa.endPacket();

  Serial.print("GW");
  Serial.print(GW);
  Serial.print("NA");
  Serial.print(NA);
  Serial.println("CONTROL");
}

void setup() {
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.begin(115200);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();

  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  Heltec.LoRa.onReceive(cbk);
  //Heltec.LoRa.receive();
}

void loop() {

 if (millis() - lastSendTime > INTERVAL){
    //Marcamos o tempo que ocorreu o último envio
    lastSendTime = millis();
    //Envia o pacote para informar ao Slave que queremos receber os dados
    gateway();
  }
   int packetSize = Heltec.LoRa.parsePacket();
 //Verificamos se há pacotes para recebermos
  cbk(packetSize);
//delay(3000);
  
 // delay(3000);
}
