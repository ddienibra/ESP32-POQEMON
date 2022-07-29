#include <Arduino.h>
#include <ModbusMaster.h>
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include "REG_PM5560.h"

#define baud 19200
#define timeout 1000
#define polling 200
#define retry_count 10
ModbusMaster node;

#define RXD1 17
#define TXD1 16

//---------------------- Lora 
const long frequency = 923E6;  // LoRa Frequency
const int resetPin = 26;        // LoRa radio reset
const int csPin = 27;          // LoRa radio chip select
const int irqPin = 33;          // change for your board; must be a hardware interrupt pin

//---------------------- Json
#include <ArduinoJson.h>
StaticJsonDocument<400> doc;
String json_lora;

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("Node Receive: ");
  Serial.println(message);
}

void onTxDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}



void preTransmission()
{
  Serial2.flush();
  digitalWrite(TXD1, 1);
//  digitalWrite(RXD1, 1);
}

void postTransmission()
{
  delayMicroseconds(200);
  digitalWrite(TXD1, 0);
//  digitalWrite(RXD1, 0);
}

//01 04 00 00 00 02 71 3F // Test 30001
//------------------------------------------------
// Convent 32bit to float
//------------------------------------------------
float HexTofloat(uint32_t x) 
{
  return (*(float*)&x);
}
 
uint32_t FloatTohex(float x) 
{
  return (*(uint32_t*)&x);
}
// convert to reduce decimal place
double round2(double value) {
   return (int)(value * 100 + 0.5) / 100.0;
}

// reading Meter on float format
float Read_Meter_float(char addr , uint16_t  REG) 
{
  float i = 0;
  uint8_t result,j;

  uint16_t data[2];
  uint32_t value = 0;
  
  node.begin(ID_meter,Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  
  result = node.readHoldingRegisters(REG,2); ///< Modbus function 0x03 Read Holding Registers
  delay(500);
  if (result == node.ku8MBSuccess) 
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = (node.getResponseBuffer(j));
    }
    //uint16_t x = data[1]; // para o valor da frequência tenho que ler algo em torno de 426F xxxx;
    //uint16_t y = data[0];
    
//    Serial.print(data[1],HEX); // print nilai register dlm HEX
//    Serial.println(data[0],HEX); // print nilai register dlm HEX
    
    value = data[0];
    value = value << 16;
    value = value + data[1];
    i = HexTofloat(value);
    //Serial.println("Connect modbus Ok.");
    return i;
  } else 
  {
    Serial.print("Connect modbus fail. REG >>> "); 
    Serial.println(REG); // Debug hasil register
    delay(1000); 
    return 0;
  }
}

void GET_METER() 
{     // Update read all data
  delay(1000);                            
    for (char i = 0; i < Total_of_Reg ; i++)
    {
      DATA_METER [i] = Read_Meter_float(ID_meter, Reg_addr[i]);
    } 
}


void setup() 
{
//  Serial1.begin(19200, SERIAL_8E1, RXD1, TXD1);
//  node.begin(ID_meter,Serial1);
  Serial.begin(115200);                   // initialize serial
  delay(1500);
  Serial2.begin(19200, SERIAL_8E1, RXD1, TXD1);
  delay(1500);
  LoRa.setPins(csPin, resetPin, irqPin);
  while (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    delay(1000);// if failed, do nothing
  }
  delay(1000);
  Serial.println("LoRa init succeeded.");
  Serial.println();
  
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();

}



void loop() 
{
   float FQ = Read_Meter_float(ID_meter,Reg_FQ);
   float VRN = Read_Meter_float(ID_meter,Reg_VRN);
   float VSN = Read_Meter_float(ID_meter,Reg_VSN);
   float VTN = Read_Meter_float(ID_meter,Reg_VTN);
   float VRS = Read_Meter_float(ID_meter,Reg_VRS);
   float VST = Read_Meter_float(ID_meter,Reg_VST);
   float VTR = Read_Meter_float(ID_meter,Reg_VTR);
   float CR = Read_Meter_float(ID_meter,Reg_CR);
   float CS = Read_Meter_float(ID_meter,Reg_CS);
   float CT = Read_Meter_float(ID_meter,Reg_CT);
   float CN = Read_Meter_float(ID_meter,Reg_CN);
   float PFtot = Read_Meter_float(ID_meter,Reg_PFTOTAL);
   float KWtot = Read_Meter_float(ID_meter,Reg_kWTOTAL);
   float KVARtot = Read_Meter_float(ID_meter,Reg_kVARTOTAL);
   float KVAtot = Read_Meter_float(ID_meter,Reg_kVATOTAL);
   float CRunbal = Read_Meter_float(ID_meter,Reg_CR_UNBAL);
   float CTunbal = Read_Meter_float(ID_meter,Reg_CS_UNBAL);
   float CSunbal = Read_Meter_float(ID_meter,Reg_CT_UNBAL);
   float THDiR = Read_Meter_float(ID_meter,Reg_THD_CR);
   float THDiS = Read_Meter_float(ID_meter,Reg_THD_CS);
   float THDiT = Read_Meter_float(ID_meter,Reg_THD_CT);
   float THDtot = Read_Meter_float(ID_meter,Reg_THD_TOTAL);
   float KWMetr = Read_Meter_float(ID_meter,Reg_kWH);

   doc["FQ"] = round2(FQ);
   doc["VRN"] = round2(VRN);
   doc["VSN"] = round2(VSN);
   doc["VTN"] = round2(VTN);
   doc["VRS"] = round2(VRS);
   doc["VST"] = round2(VST);
   doc["VTR"] = round2(VTR);
   doc["CR"] = round2(CR);
   doc["CS"] = round2(CS);
   doc["CT"] = round2(CT);
   doc["CN"] = round2(CN);
   doc["PFtot"] = round2(PFtot);
   doc["KWtot"] = round2(KWtot);
   doc["KVARtot"] = round2(KVARtot);
   doc["KVAtot"] = round2(KVAtot);
   doc["CRunbal"] = round2(CRunbal);
   doc["CSunbal"] = round2(CSunbal);
   doc["CTunbal"] = round2(CTunbal);
   doc["THDiR"] = round2(THDiR);
   doc["THDiS"] = round2(THDiS);
   doc["THDiT"] = round2(THDiT);
   doc["KWMetr"] = round2(KWMetr);
   
   serializeJson(doc, json_lora);   
   Serial.println(json_lora);
   LoRa_sendMessage(json_lora);
   json_lora = "";

   Serial.println("------------------");  
   Serial.print("FREQ =  "); Serial.print(FQ,1);Serial.println(" Hz");
   Serial.print("V-RN =  "); Serial.print(VRN,1);Serial.println(" V");
   Serial.print("V-SN =  "); Serial.print(VSN,1);Serial.println(" V");
   Serial.print("V-TN =  "); Serial.print(VTN,1);Serial.println(" V");
   Serial.print("V-RS =  "); Serial.print(VRS,1);Serial.println(" V");
   Serial.print("V-ST =  "); Serial.print(VST,1);Serial.println(" V");
   Serial.print("V-TR =  "); Serial.print(VTR,1);Serial.println(" V");
   Serial.print("C-R =  "); Serial.print(CR,1);Serial.println(" A");
   Serial.print("C-S =  "); Serial.print(CS,1);Serial.println(" A");
   Serial.print("C-T =  "); Serial.print(CT,1);Serial.println(" A");
   Serial.print("C-N =  "); Serial.print(CN,1);Serial.println(" A");
   Serial.print("PF-Total =   "); Serial.print(PFtot,1);Serial.println(" ");
   Serial.print("KW-Total =   "); Serial.print(KWtot,2);Serial.println(" kW");
   Serial.print("KVAR-Total = "); Serial.print(KVARtot,2);Serial.println(" kVAR");
   Serial.print("KVA-Total =  "); Serial.print(KVAtot,2);Serial.println(" kVA");
   Serial.print("C-R Unbal =  "); Serial.print(CRunbal,1);Serial.println(" %");
   Serial.print("C-S Unbal =  "); Serial.print(CTunbal,1);Serial.println(" %");
   Serial.print("C-T Unbal =  "); Serial.print(CSunbal,1);Serial.println(" %");
   Serial.print("THDi_R =  "); Serial.print(THDiR,1);Serial.println(" %");
   Serial.print("THDi_S =  "); Serial.print(THDiS,1);Serial.println(" %");
   Serial.print("THDi_T =  "); Serial.print(THDiT,1);Serial.println(" %");
   Serial.print("THD-Total =  "); Serial.print(THDtot,1);Serial.println(" %");
   Serial.print("KW-Meter =   "); Serial.print(KWMetr,3);Serial.println(" KWh");

 
//  GET_METER();
//  Serial.println();
//  Serial.print("FREQ =  "); Serial.print(DATA_METER[0],0);Serial.println(" Hz");
//  Serial.print("V-RN =  "); Serial.print(DATA_METER[1],0);Serial.println(" V");
//  Serial.print("V-SN =  "); Serial.print(DATA_METER[2],0);Serial.println(" V");
//  Serial.print("V-TN =  "); Serial.print(DATA_METER[3],0);Serial.println(" V");
//  Serial.print("V-RS =  "); Serial.print(DATA_METER[4],0);Serial.println(" V");
//  Serial.print("V-ST =  "); Serial.print(DATA_METER[5],0);Serial.println(" V");
//  Serial.print("V-TR =  "); Serial.print(DATA_METER[6],0);Serial.println(" V");
//  Serial.print("C-R =  "); Serial.print(DATA_METER[7],0);Serial.println(" A");
//  Serial.print("C-S =  "); Serial.print(DATA_METER[8],0);Serial.println(" A");
//  Serial.print("C-T =  "); Serial.print(DATA_METER[9],0);Serial.println(" A");
//  Serial.print("C-N =  "); Serial.print(DATA_METER[10],0);Serial.println(" A");
//  Serial.print("PF-Total =   "); Serial.print(DATA_METER[11],1);Serial.println(" ");
//  Serial.print("KW-Total =   "); Serial.print(DATA_METER[12],2);Serial.println(" kW");
//  Serial.print("KVAR-Total = "); Serial.print(DATA_METER[13],2);Serial.println(" kVAR");
//  Serial.print("KVA-Total =  "); Serial.print(DATA_METER[14],2);Serial.println(" kVA");
//  Serial.print("C-R Unbal =  "); Serial.print(DATA_METER[15],1);Serial.println(" %");
//  Serial.print("C-S Unbal =  "); Serial.print(DATA_METER[16],1);Serial.println(" %");
//  Serial.print("C-T Unbal =  "); Serial.print(DATA_METER[17],1);Serial.println(" %");
//  Serial.print("THDi_R =  "); Serial.print(DATA_METER[18],1);Serial.println(" %");
//  Serial.print("THDi_S =  "); Serial.print(DATA_METER[19],1);Serial.println(" %");
//  Serial.print("THDi_T =  "); Serial.print(DATA_METER[20],1);Serial.println(" %");
//  Serial.print("THD-Total =  "); Serial.print(DATA_METER[21],1);Serial.println(" %");
//  Serial.print("KW-Meter =   "); Serial.print(DATA_METER[22],3);Serial.println(" KWh");
  delay(3000);
}
