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

// Informações sobre as funções da bibliteca
// https://github.com/HelTecAutomation/Heltec_ESP32/blob/master/src/lora/API.md

#include "heltec.h" 
#include "images.h"

#define BAND 915E6  //you can set band here directly,e.g. 868E6,915E6

//String rssi = "RSSI --";
//String packSize = "--";
//String packet ;
//String snr = "SNR --";
//String ID;

String rssi;
String packSize;
String packet;
String snr;
String ID;

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void LoRaData(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
  Heltec.display->drawString(0, 36, ID);  
  Heltec.display->display();
}

void cbk(int packetSize) {
  ID="";
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < 4; i++) { ID += (char) Heltec.LoRa.read(); }
  for (int i = 4; i < packetSize; i++) { packet += (char) Heltec.LoRa.read(); }
  rssi = "RSSI: " + String(Heltec.LoRa.packetRssi(), DEC) + "dBm" ;
  snr = "SNR: " + String(Heltec.LoRa.packetSnr(), DEC) + "dB" ;
  Serial.println(rssi);
  Serial.println(snr);

  if (ID=="ID10")
  {
    Serial.println("Entrei ...");
    // send packet
    Heltec.LoRa.beginPacket();
    Heltec.LoRa.print("ID20");
    Heltec.LoRa.print("Resposta do Hello ");
    //Heltec.LoRa.print(counter);
    Heltec.LoRa.endPacket();
  }
  
  //for(int i=0; i<17; i=i+8) {
  //  chipId |= ((Heltec.LoRa.getEfuseMac() >> (40 - i)) & 0xff) << i;
  //}
  LoRaData();
}

void setup() { 
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
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

void loop() 
{
  int packetSize = Heltec.LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);
}
