//data:2016/03/20
#include <SoftwareSerial.h>
#include <EEPROM.h>
// HC-05    Arduino
// TX       RX/Pin7
// RX       TX/Pin8
SoftwareSerial BTSerial(7,8); // Arduino RX/TX
char data[20];//存放資料變數
int datasize=0;//傳來的資料數量
char save[]="save"; //儲存到eeprom的參數
int R=0,G=0,B=0,Rpin=3,Gpin=5,Bpin=6,button=2,button_state=1,mode=0,time=100,temp_r,temp_g,temp_b,temp_var;
//mode=0開機模式 上次儲存的顏色(可藍牙或不藍牙)(目前要藍牙)
//mode=1藍牙模式 (可開機模式與藍牙共存但會多跑一行程式)
//mode=2按鈕模式 紅
//mode=3按鈕模式 綠
//mode=4按鈕模式 藍
//mode=5按鈕模式 黃
//mode=6按鈕模式 青
//mode=7按鈕模式 紫
//mode=8按鈕模式 白
//mode=9按鈕模式 彩虹
void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(Rpin,OUTPUT);
  pinMode(Gpin,OUTPUT);
  pinMode(Bpin,OUTPUT);
  pinMode(button,INPUT);
  R=EEPROM.read(0);
  G=EEPROM.read(1);
  B=EEPROM.read(2);
}
void loop() {
  button_state=digitalRead(button);
  if (button_state==0){//平時接正
    mode=mode+1;
    mode=mode%10;
    //    Serial.print("mode=");
    //    Serial.println(mode);
    button_state=1;
    delay(250);
  }
  switch (mode)//功能選擇
  {
  case 0://開機模式
    R=EEPROM.read(0);
    G=EEPROM.read(1);
    B=EEPROM.read(2);
    time=100;
    break;
  case 1://藍牙模式
    if ( (datasize=(BTSerial.available()))>0 ){//讀藍牙訊息
      for (int i=0; i<datasize; i++ ){//合併資料
        data[i]=char(BTSerial.read());
      }
      if ( strcmp(data,save)==0 ){
        //        Serial.println("save to eeprom mode");
        delay(100);
        clean_data();
        wait_data();
        EEPROM.write(0,data[0]);
        EEPROM.write(1,data[1]);
        EEPROM.write(2,data[2]);
        //        Serial.println("save ok");
      }
      else {
        R=data[0];
        G=data[1];
        B=data[2];
      }
    }
    break;
  case 2://紅
    R=255;
    G=0;
    B=0;
    break;
  case 3://綠
    R=0;
    G=255;
    B=0;
    break;
  case 4://藍
    R=0;
    G=0;
    B=255;
    break;
  case 5://黃
    R=255;
    G=255;
    B=0;
    break;
  case 6://青
    R=0;
    G=255;
    B=255;
    break;
  case 7://紫
    R=255;
    G=0;
    B=255;
    break;
  case 8://白
    R=255;
    G=255;
    B=255;
    break;
  case 9://彩虹
    rainbow(1);
    time=25;
    break;
  }
  analogWrite(Rpin,~R);
  analogWrite(Gpin,~G);
  analogWrite(Bpin,~B);
  clean_data();//要連續儲存到eeprom必須先清除資料暫存
  delay(time);
}

//////////函數區
void clean_data()//清空資料陣列
{
  for(int i=0;i<20;i++){
    data[i] ='\0';
  }
  datasize=0;
}

void wait_data()//等待藍牙傳入資料
{
  do{
    if ( (datasize=(BTSerial.available()))>0 ){
      for (int i=0; i<datasize; i++ ){
        data[i]=char(BTSerial.read());
      }
    }
    else {
      Serial.println("wait data");
      delay(1000);
    }
  }
  while( datasize<=0 );
}

void rainbow(int wait)//彩虹功能
{
  Wheel(temp_var & 255);
  R=temp_r;
  G=temp_g;
  B=temp_b;
  temp_var++;
}

void Wheel(int WheelPos)//給rainbow用的顏色設定
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    temp_r = 255 - WheelPos * 3;
    temp_g = 0;
    temp_b = WheelPos * 3;
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    temp_r = 0;
    temp_g = WheelPos * 3;
    temp_b = 255 - WheelPos * 3;
  }
  else
  {
    WheelPos -= 170;
    temp_r = WheelPos * 3;
    temp_g = 255 - WheelPos * 3;
    temp_b = 0;
  }
}
