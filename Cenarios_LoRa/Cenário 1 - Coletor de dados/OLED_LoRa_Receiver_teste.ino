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
String Rssi = "RSSI --";
String packSize = "--";
String packet;
String ID;
String NS;
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
float rssiMedio1 = 0;
float rssiMedio2 = 0;
float rssiMedio3 = 0;
int initialNS1 = 0;
int initialNS2 = 0;
int initialNS3 = 0;

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
  //Heltec.LoRa.onReceive(cbk);
  Heltec.LoRa.receive();
}

void loop() {
  int packetSize = Heltec.LoRa.parsePacket();
  if (packetSize) {
    cbk(packetSize);
    id = ID.toInt();
    //Serial.println(id);
    ns = NS.toInt();
    ident(id, ns);
  }
  delay(10);
}
