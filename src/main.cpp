#include <Arduino.h>
#include <ModbusMaster.h>
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include "REG_PM5560.h"

#define baud 9600
#define timeout 1000
#define polling 200
#define retry_count 10
ModbusMaster node;

#define RXD1 17
#define TXD1 16

void preTransmission()
{
  digitalWrite(TXD1, 1);
  digitalWrite(RXD1, 1);
}

void postTransmission()
{
  digitalWrite(TXD1, 0);
  digitalWrite(RXD1, 0);
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

float Read_Meter_float(char addr , uint16_t  REG) 
{
  float i = 0;
  uint8_t result,j;

  uint16_t data[2];
  uint32_t value = 0;
  node.begin(ID_meter,Serial1);
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


//**************************************************************************************************************
void setup() 
{
  Serial.begin(9600);
  Serial1.begin(19200, SERIAL_8E1, RXD1, TXD1);

}

void loop() 
{
//  float x = Read_Meter_float(ID_meter,Reg_Volt);
  GET_METER();
  Serial.println();
  Serial.print("FREQ =  "); Serial.print(DATA_METER[0],0);Serial.println(" Hz");
  Serial.print("V-RN =  "); Serial.print(DATA_METER[1],0);Serial.println(" V");
  Serial.print("V-SN =  "); Serial.print(DATA_METER[2],0);Serial.println(" V");
  Serial.print("V-TN =  "); Serial.print(DATA_METER[3],0);Serial.println(" V");
  Serial.print("V-RS =  "); Serial.print(DATA_METER[4],0);Serial.println(" V");
  Serial.print("V-ST =  "); Serial.print(DATA_METER[5],0);Serial.println(" V");
  Serial.print("V-TR =  "); Serial.print(DATA_METER[6],0);Serial.println(" V");
  Serial.print("C-R =  "); Serial.print(DATA_METER[7],0);Serial.println(" A");
  Serial.print("C-S =  "); Serial.print(DATA_METER[8],0);Serial.println(" A");
  Serial.print("C-T =  "); Serial.print(DATA_METER[9],0);Serial.println(" A");
  Serial.print("C-N =  "); Serial.print(DATA_METER[10],0);Serial.println(" A");
  Serial.print("PF-Total =   "); Serial.print(DATA_METER[11],1);Serial.println(" ");
  Serial.print("KW-Total =   "); Serial.print(DATA_METER[12],2);Serial.println(" kW");
  Serial.print("KVAR-Total = "); Serial.print(DATA_METER[13],2);Serial.println(" kVAR");
  Serial.print("KVA-Total =  "); Serial.print(DATA_METER[14],2);Serial.println(" kVA");
  Serial.print("C-R Unbal =  "); Serial.print(DATA_METER[15],1);Serial.println(" %");
  Serial.print("C-S Unbal =  "); Serial.print(DATA_METER[16],1);Serial.println(" %");
  Serial.print("C-T Unbal =  "); Serial.print(DATA_METER[17],1);Serial.println(" %");
  Serial.print("THDi_R =  "); Serial.print(DATA_METER[18],1);Serial.println(" %");
  Serial.print("THDi_S =  "); Serial.print(DATA_METER[19],1);Serial.println(" %");
  Serial.print("THDi_T =  "); Serial.print(DATA_METER[20],1);Serial.println(" %");
  Serial.print("THD-Total =  "); Serial.print(DATA_METER[21],1);Serial.println(" %");
  Serial.print("KW-Meter =   "); Serial.print(DATA_METER[22],3);Serial.println(" KWh");
  delay(2000);
}