#include <dht.h>
#include <uart.h>
#include <util.h>

#include <AddicoreRFID.h>
#include <SPI.h>
#include <new.h>

#include <Wire.h>                            // I2C control library
#include <LiquidCrystal_I2C.h>          // LCD library

#define    uchar    unsigned char
#define    uint    unsigned int

//dkjf;alksjdf;alksdjfa;

 
/*
 * dfajsdfj;laksjdfl
 */
class DailyLook{
  private :
  String name;
  int* category; // 0;jacket, 1;top, 2;pants, 3;skirt, 4;socks, 5;dress
  int score = 0;
  int temperBase;
  int temperLimit;
  int humidBase;
  int humidLimit;
  boolean currentUse = false;
  
 public  :
  DailyLook(String name, int* category, int temperBase, int temperLimit, int humidBase, int humidLimit):
    name(name), category(category),temperBase(temperBase),temperLimit(temperLimit),humidBase(humidBase), humidLimit(humidLimit){}
  String getName() {
    return name;
  }
  void setName(String name) {
    name = name;
  }
  int* getCategory() {
    return category;
  }
  void setCategory(int* category) {
    category = category;
  }
  int getScore() {
    return score;
  }
  void setScore(int score) {
    score = score;
  }
  int getTemperBase() {
    return temperBase;
  }
  void setTemperBase(int temperBase) {
    temperBase = temperBase;
  }
  int getTemperLimit() {
    return temperLimit;
  }
  void setTemperLimit(int temperLimit) {
    temperLimit = temperLimit;
  }
  int getHumidBase() {
    return humidBase;
  }
  void setHumidBase(int humidBase) {
    humidBase = humidBase;
  }
  int getHumidLimit() {
    return humidLimit;
  }
  void setHumidLimit(int humidLimit) {
    humidLimit = humidLimit;
  }
  boolean isCurrentUse() {
    return currentUse;
  }
  void setCurrentUse(boolean currentUse) {
    currentUse = currentUse;
  }
};
lookcolor = [[255,255,255], [0,0,255]]
class Closet {
  private :
  String masterName;
  int masterUID;
  DailyLook* list;

 public :
 /*
  * QQ how to express generic in C++
  */
  Closet(String name, int UID, DailyLook* list):
  masterName(name), masterUID(UID), list(list){}
  
   String getMasterName() {
   return masterName;
 }
   void setMasterName(String masterName) {
   masterName = masterName;
 }
   int getMasterUID() {
   return masterUID;
 }
   void setMasterUID(int masterUID) {
   masterUID = masterUID;
 }
   DailyLook* getList() {
   return list;
 }
   void setList(DailyLook* list) {
   list = list;
 }
 
};

class MusicLib{
  
  public :
  /*
   * 
   void MusicLib(){}
  
  void playMusic(String Title){
    
  }
  */
};

void printWeather(int temper, int humid){
    // print current Weather
  }
  
  
  /*
  DailyLook chooseDailyLook(Closet person, int temper, int humid){
    
     * DailyLook loook = new DailyLook();
     * return loook;
     
    
  }
  */
  void scoreDailyLook(DailyLook look){
    //TODO 점수 매기기
  }

 * 4 bytes tag serial number, the first 5 bytes for the checksum byte
 */
 
uchar serNumA[5];
 
uchar fifobytes;
uchar fifoValue;
 
AddicoreRFID myRFID; /* create AddicoreRFID object to control the RFID module*/

 /*
/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
*/
const int chipSelectPin = 10;
const int NRSTPD = 5;
const int speakerPin = 8; //스피커가 연결된 디지털핀 설정
 
/* Maximum length of the array */
#define MAX_LEN 16
 
 Serial.begin(9600);
 int curtemper;
 int curhumid;
 String weatherMsg[];
 MusicLib musics;
 DailyLook curChoices[];
 Closet closets[];
 
dht DHT;
int DHT11 = 12;

int speakerpin = 12;

#define BTN_PIN 2

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display


void setup(){
  Serial.begin(9600);
  
  // 
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
  
  
  lcd.init();                      // initialize the lcd 
  
  // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);
 
  myRFID.AddicoreRFID_Init(); 

 /*
  * TODO : 
 1.TODO : 옷장 및 코디 구성
  */

/*
 * 1) 코디리스트 생성
 * DailyLook MinsuCloset[] = { DailyLook(String name, int* category, int temperBase, int temperLimit, int humidBase, int humidLimit),
 *                             // 민수의 코디 목록 };
 * DailyLook SuzyCloset[] = { DailyLook(String name, int* category,int*rgb, int temperBase, int temperLimit, int humidBase, int humidLimit),
 * DailyLook(String name, int* category, int temperBase, int temperLimit, int humidBase, int humidLimit),
 *                             // 수지의 코디 목록 };
 */
  /*
   * 2) 옷장 생성
  Closet Minsu = new Closet("Minsu", 0x~~~, MinsuCloset); 
  Closet Suzy = new Closet(Suzy, 0x~~~, SuzyCloset);
  */
  /*
  * 3) 
  */
  
  
}//end of setup

void loop(){
 String hi = "hello";
 Serial.println(hi);
 
 //test temperature humidity sensor
 int chk = DHT.read11(DHT11);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  
  //test piezo speaker
  tone(speakerpin,500,1000);  //500: 음의 높낮이(주파수), 1000: 음의 지속시간(1초)
  delay(2000); 

  //test button
  int btn = digitalRead(BTN_PIN);
  
  //test 3color led
  setColor(255, 0, 0); // red
  delay(2000);
  setColor(0, 255, 0); // green
  delay(2000);
  setColor(0, 0, 255); // blue
  delay(2000);
  setColor(255, 255, 0); // yellow
  delay(2000); 
  setColor(255, 0, 255); // purple
  delay(2000);
  setColor(0, 255, 255); // aqua
  delay(2000);
  setColor(255, 255, 255); // white
  delay(2000);
  setColor(0, 0, 0); // Off
  delay(2000);  


  //test lcd panel
  lcd.backlight();  // turn on backlight
  lcd.print("Hello, world!");


  //test RFID module
    uchar i, tmp, checksum1;
    uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;    //Selection operation block address 0 to 63
  String mynum = "";
  str[1] = 0x4400;
        
    //RFID 태그의 타입을 리턴
    status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);    
    if (status == MI_OK)    //MIFARE 카드일때만 작동
    {
          tone(speakerPin,2000,100);
          Serial.println("RFID tag detected");
            Serial.print(str[0],BIN);
          Serial.print(" , ");
            Serial.print(str[1],BIN);
          Serial.println(" ");
    }
 
  //RFID 충돌방지, RFID 태그의 ID값(시리얼넘버) 등 저장된 값을 리턴함. 4Byte
    status = myRFID.AddicoreRFID_Anticoll(str);
    if (status == MI_OK)      //MIFARE 카드일때만 작동
    {
          checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
          Serial.println("The tag's number is  : ");
            //Serial.print(2);
            Serial.print(str[0]);
          Serial.print(" , ");
            Serial.print(str[1],BIN);
          Serial.print(" , ");
            Serial.print(str[2],BIN);
          Serial.print(" , ");
            Serial.print(str[3],BIN);
          Serial.print(" , ");
            Serial.print(str[4],BIN);
          Serial.print(" , ");
          Serial.println(checksum1,BIN);
           
            // Should really check all pairs, but for now we'll just use the first
            if(str[0] == 224)                      //RFID 태그의 ID값이 224번이면 Gil Dong의 카드
            {
                Serial.print("Hello Gil Dong!\n");
            } else if(str[0] == 170) {             //RFID 태그의 ID값이 170번이면 Kang Min의 카드
                Serial.print("Hello Kang Min!\n");
            }
            Serial.println();
            delay(1000);
    }
        myRFID.AddicoreRFID_Halt();           //Command tag into hibernation     

/*
 // TODO : 날씨별 메시지 작성 
 // TODO : 추천 목록 Looks recommands[10];
 // TODO : 통신 setup
 */
 
/*
 * UID 입력 없을시
 */
/*
 * 날씨를 출력한다.
 */
 
 

 /*
  * UID입력 들어올시
  */
/* 
 *  현재 날씨 상태 저장하고 이를 바탕으로 코디를 추천한다.
 *  UID에 따라 옷장을 구별한다.
 *  
 */
  
/*
 * 옷추천 +음악
 * -> 옷 추천 하는 함수 만들기
 * chooseDailyLook(Closet person, int temper, int humid)
 */ 

/*
 * 평가
 * -> 코디를 평가하는 함수 만들기
  scoreDailyLook(DailyLook look)
 */
  
}


