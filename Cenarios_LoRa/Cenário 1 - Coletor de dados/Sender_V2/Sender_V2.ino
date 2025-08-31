/*
  This is a simple example show the Heltec.LoRa sended data in OLED.

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

//unsigned int counter = 0;
char counter [] = "00000";
String rssi = "RSSI --";
String packSize = "--";
String packet ;



void onReceive(int packetSize) 
{
 Serial.println("Resposta Recebida !!");
}


void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void setup()
{
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);

  Heltec.LoRa.onReceive(onReceive);
  Heltec.LoRa.receive();
  
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "ID001"); // Alterar o valor do ID que está enviando
  Heltec.display->drawString(0, 15, "Sending packet... ");
  Heltec.display->drawString(0, 25, "Packet number: ");
  Heltec.display->drawString(80, 25, String(counter));
  Heltec.display->display();

  // send packet IDXXXNS00000DADOS
  Heltec.LoRa.beginPacket();
  Heltec.LoRa.print("ID001"); // Alterar o valor do ID que está enviando
  Heltec.LoRa.print("NS");
  Heltec.LoRa.print(counter);
  Heltec.LoRa.print("SENSOR");
  Heltec.LoRa.endPacket();

  int num = atoi(counter);

  num++;

  char temp[6]; // Buffer temporário com tamanho para 5 caracteres + terminador nulo

  snprintf(temp, sizeof(temp), "%05d", num);

  strcpy(counter, temp); // Copia o valor do buffer temporário para a string counter

  //sprintf(counter, "%d", num);

  //counter++;

  
  
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
