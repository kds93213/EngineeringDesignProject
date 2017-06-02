
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

#define LED2_RED 10
#define LED2_GRE 9
#define LED2_BLU 8

#define BTN_PIN1 7 // YES Button
#define BTN_PIN2 6 // No Button


LiquidCrystal lcd(1, 0, 4, 5, 6, 7);

// 카운터
byte count = 0; 


void setYesNoBtnLCD(){
  lcd.setCursor(0,1);
  lcd.write("   YES     NO   ");
  lcd.display();
}

void setColor(int ledNum, int red, int green, int blue)
{
  if (!ledNum){ // 0 ; jacket
    analogWrite(LED1_RED, red);
    analogWrite(LED1_GRE, green);
    analogWrite(LED1_BLU, blue); 
  } else { // 1; top
    analogWrite(LED2_RED, red);
    analogWrite(LED2_GRE, green);
    analogWrite(LED2_BLU, blue);     
  }
}
void receiveFromMaster(int bytes) {
  int temp = 0;
   int ledNum = -1;
   int r = 0;
   int g = 0;
   int b = 0;
   char data;
   data = "";
  while( Wire.available()){
    temp = Wire.read();
    if (temp == 1){ // set LED
      /*
       * LED send
       * 1. send 1
       * 2. send led light number, r, g, b value in order
       */
      ledNum = Wire.read();
      if (ledNum == 0){ //if it have jacket led color 
        Serial.print("Printing Jacket : "); 
      } else {
       Serial.print("Printing Top : ");        
      }
        r = Wire.read();
        g = Wire.read();
        b = Wire.read();
        Serial.print("r : ");
        Serial.print(r);
        Serial.print("g : ");
        Serial.print(g);
        Serial.print("b : ");
        Serial.println(b); 
       setColor(ledNum, r, g, b);
    } else if (temp == 2){
      /*
       * LCD send
       * 1. send 2
       * 2. send ! (if you want to print yes no button on second row)or sth else
       * 3. send msg
       */
      lcd.clear();
      data = Wire.read();
      if (data == '!'){ //use yes no button
        setYesNoBtnLCD();
      }
      lcd.home();       
      lcd.write(Wire.read());
      }
      lcd.autoscroll();
      lcd.display();
    } else if(temp == 3){// get temperature and humidity
      float curTemp = Wire.read();
      float curHumid = Wire.read();
      //printWeather(curTemp, curHumid);
    }
  }

void sendBtnInfo() {
  Wire.write("HI, I am  Slave. ");
  // send button information
  while(true){
    if (digitalRead(BTN_PIN1) == LOW) //if Yes was pressed
    {
      Wire.write(BTN_PIN1);
      return;
    } else if (digitalRead(BTN_PIN2) == LOW) // if No was pressed
    {
      Wire.write(BTN_PIN2);
      return;
    }
  }
}
void setup() {
  
  pinMode(BTN_PIN1,INPUT);
  pinMode(BTN_PIN2,INPUT);
  
  pinMode(LED1_RED, OUTPUT);
  pinMode(LED1_GRE, OUTPUT);
  pinMode(LED1_BLU, OUTPUT);
  
  pinMode(LED2_RED, OUTPUT);
  pinMode(LED2_GRE, OUTPUT);
  pinMode(LED2_BLU, OUTPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);

  // Wire 라이브러리 초기화
  // 슬레이브로 참여하기 위해서는 주소를 지정해야 한다.
  Wire.begin(SLAVE);
  Wire.onReceive(receiveFromMaster);
  // 마스터의 데이터 전송 요구가 있을 때 처리할 함수 등록
  Wire.onRequest(sendBtnInfo);
}

void loop () {
  // Request
}
