#include <Arduino.h>
#define ID_meter  1
#define Total_of_Reg  23

#define Reg_FQ 3109   //  1. // frequencia 426F xxxx ; 
#define Reg_VRN 3027   //  2
#define Reg_VSN 3029   //  3
#define Reg_VTN 3031   //  4
#define Reg_VRS 3019 // 5
#define Reg_VST 3021 // 6
#define Reg_VTR 3023 // 7
#define Reg_CR 2999 //8
#define Reg_CS 3001 //9
#define Reg_CT 3003 //10
#define Reg_CN 3005 //11
#define Reg_PFTOTAL 3195 // 12
#define Reg_kWTOTAL 3059 // 13
#define Reg_kVARTOTAL 3067 // 14
#define Reg_kVATOTAL 3075 // 15
#define Reg_CR_UNBAL 3011 //16
#define Reg_CS_UNBAL 3013  //17
#define Reg_CT_UNBAL 3015 //18
#define Reg_THD_CR 21299 //19
#define Reg_THD_CS 21301 //20
#define Reg_THD_CT 21303 //21
#define Reg_THD_TOTAL 21319 //22
#define Reg_kWH 2701 //23

uint16_t Reg_addr[23] = {
  Reg_FQ,
  Reg_VRN,
  Reg_VSN,
  Reg_VTN,
  Reg_VRS,
  Reg_VST,
  Reg_VTR,
  Reg_CR,
  Reg_CS,
  Reg_CT,
  Reg_CN,
  Reg_PFTOTAL,  
  Reg_kWTOTAL,
  Reg_kVARTOTAL,
  Reg_kVATOTAL,
  Reg_CR_UNBAL,
  Reg_CS_UNBAL,
  Reg_CT_UNBAL,
  Reg_THD_CR,
  Reg_THD_CS,
  Reg_THD_CT,
  Reg_THD_TOTAL,
 
};

float DATA_METER [Total_of_Reg] ;