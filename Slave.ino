
/*
 * Wire : Slave
 * Board # : 2
 * Components : LCD, two button, two LED
 * 
 */
#include <Wire.h>
#include <LiquidCrystal.h>

// 자신의 슬레이브 주소를 설정해 줍니다.(슬레이브 주소에 맞게 수정해야 합니다.)
#define SLAVE 4  

#define LED1_RED 13
#define LED1_GRE 12
#define LED1_BLU 11

#define LED2_RED 10`
#define LED2_GRE 9
#define LED2_BLU 8

#define BTN_PIN1 7 // YES Button
#define BTN_PIN2 6 // No Button


LiquidCrystal lcd(1, 0, 4, 5, 6, 7);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display

// 카운터
byte count = 0; 
char temp;

void setup() {
  // Wire 라이브러리 초기화
  // 슬레이브로 참여하기 위해서는 주소를 지정해야 한다.
  Wire.begin(SLAVE);
  Wire.onReceive(receiveFromMaster);
  // 마스터의 데이터 전송 요구가 있을 때 처리할 함수 등록
  Wire.onRequest(sendBtnInfo);
  pinMode(BTN_PIN1,INPUT);
  pinMode(BTN_PIN2,INPUT);
  
  pinMode(LED1_RED, OUTPUT);
  pinMode(LED1_GRE, OUTPUT);
  pinMode(LED1_BLU, OUTPUT);
  
  pinMode(LED2_RED, OUTPUT);
  pinMode(LED2_GRE, OUTPUT);
  pinMode(LED2_BLU, OUTPUT);

  lcd.init();
  Serial.begin(9600);
}

void loop () {
  // Request
  if (temp == 'C') { // print msg on LCD
    printMsg(length);
  } else if (temp == 'E'){ // turn the leds on
    
  }
}

void receiveFromMaster(int bytes) {
  char ch[2];
  for (int i = 0 ; i < bytes ; i++) {
    // 수신 버퍼 읽기
    ch[i] = Wire.read(); 
  }
  temp = ch[0];
  // print Msg on LCD

  /*
   * 
lcd.backlight LCD 백라이트 on
lcd.nobacklight LCD 백라이트 off
lcd.setCursor(3,0)  커서를 0번째 라인 4번째 문자로 이동
lcd.print("xxxxx")  문자열 xxxxx 를 출력
lcd.write(char) 문자 하나를 현재 커서에 출력하고 커서를 다음 위치로 이동
lcd.clear 화면 삭제
   */
   data = "";
  while( Wire.available()){
    data += (char)Wire.read();
    Serial.println(data);
  }
}


void sendBtnInfo() {
  Wire.write("HI, I am  Slave. ");
  // send button information
  while(true){
    if (digitalRead(BTN_PIN1) == LOW) //if Yes was pressed
    {
      Wire.write(BTN_PIN1,byte);
      return;
    } else if (digitalRead(BTN_PIN2) == LOW) // if No was pressed
    {
      Wire.write(BTN_PIN2,byte);
      return;
    }
  }
}


