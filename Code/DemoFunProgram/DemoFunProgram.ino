#include <Arduino_CAN.h>

static uint32_t const CAN_ID = 0x20;
static uint32_t msg_cnt = 0;

float Aref=5.0;   //模拟量参考电压


void CAN_test_Demo(){

  uint8_t const msg_data[] = {0xCA,0xFE,0,0,0,0,0,0};
  memcpy((void *)(msg_data + 4), &msg_cnt, sizeof(msg_cnt));
  CanMsg const msg(CanStandardId(CAN_ID), sizeof(msg_data), msg_data);

  /* Transmit the CAN message, capture and display an
   * error core in case of failure.
   */
  if (int const rc = CAN.write(msg); rc < 0)
  {
    Serial.print  ("CAN.write(...) failed with error code ");
    Serial.println(rc);
    for (;;) { }
  }

  /* Increase the message counter. */
  msg_cnt++;
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  //if (!CAN.begin(CanBitRate::BR_250k))
  //{
  //  Serial.println("CAN.begin(...) failed.");
  //  for (;;) {}
  //}


  analogReadResolution(12);  // 设置ADC为12位（0-4095）
  analogWriteResolution(12); // 设置DAC为12位

  pinMode(11, INPUT);   //DIN1
  pinMode(12, INPUT);   //DIN2

  pinMode(2, OUTPUT); // 晶体管输出CH4
  pinMode(3, OUTPUT); // 晶体管输出CH3
  pinMode(4, OUTPUT); // 晶体管输出CH2
  pinMode(5, OUTPUT); // 晶体管输出CH1

  pinMode(9, OUTPUT); // 继电器输出CH1
  pinMode(8, OUTPUT); // 继电器输出CH2
  pinMode(7, OUTPUT); // 继电器输出CH3
  pinMode(6, OUTPUT); // 继电器输出CH4
 
}

void loop() {

  int DIN1_State =digitalRead(11);
  int DIN2_State =digitalRead(12);
 

  String Str_DIN1_State = "DIN1_State: " + String(DIN1_State, 2);
  String Str_DIN2_State = "DIN2_State: " + String(DIN2_State, 2);

  if(DIN1_State==0){

    digitalWrite(9, HIGH); 
    digitalWrite(8, HIGH); 

  }else if(DIN1_State==1){
    digitalWrite(9, LOW); 
    digitalWrite(8, LOW); 
  }

  if(DIN2_State==0){

    digitalWrite(7, HIGH); 
    digitalWrite(6, HIGH); 

  }else if(DIN2_State==1){
    digitalWrite(7, LOW); 
    digitalWrite(6, LOW); 
  }

  //CAN_test_Demo();

  
 
  
  int A1Value = analogRead(A1);  // 读取A1引脚模拟输入
  int A2Value = analogRead(A2);  // 读取A1引脚模拟输入
  int A3Value = analogRead(A3);  // 读取A1引脚模拟输入
  int A4Value = analogRead(A4);  // 读取A1引脚模拟输入
  int A5Value = analogRead(A5);  // 读取A1引脚模拟输入

  int AoValue=A1Value;    //DAC输出值

  float A1_Voltage=((float)A1Value/4096)*Aref;
  float A2_Voltage=((float)A2Value/4096)*Aref;
  float A3_Voltage=((float)A3Value/4096)*Aref;
  float A4_Current=(((float)A4Value/4096)*Aref)/180*1000;
  float A5_Current=(((float)A5Value/4096)*Aref)/180*1000;

  String Str_A1_Voltage = "A1_Voltage: " + String(A1_Voltage, 4)+ "V";
  String Str_A2_Voltage = "A2_Voltage: " + String(A2_Voltage, 4)+ "V";
  String Str_A3_Voltage = "A3_Voltage: " + String(A3_Voltage, 4)+ "V";
  String Str_A4_Current = "I1_Current: " + String(A4_Current, 4)+ "mA";
  String Str_A5_Current = "I2_Current: " + String(A5_Current, 4)+ "mA";


  analogWrite(A0, A1Value);     // 将ADC值直接输出至DAC


  Serial.println("*************************************");
  Serial.println(Str_DIN1_State);
  Serial.println(Str_DIN2_State);

  Serial.println("*************************************");
  Serial.println(Str_A1_Voltage);
  Serial.println(Str_A2_Voltage);
  Serial.println(Str_A3_Voltage);
  Serial.println(Str_A4_Current);
  Serial.println(Str_A5_Current);

  Serial1.println("RS485 OUTPUT OK!");

  delay(400); // 延迟以稳定采样

  digitalWrite(2, HIGH); 
  digitalWrite(3, HIGH); 
  digitalWrite(4, HIGH); 
  digitalWrite(5, HIGH); 
  delay(400); // 延迟以稳定采样
  digitalWrite(2, LOW); 
  digitalWrite(3, LOW); 
  digitalWrite(4, LOW); 
  digitalWrite(5, LOW); 
}

