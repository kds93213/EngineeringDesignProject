#include <dht.h>
#include <uart.h>
#include <util.h>

#include <AddicoreRFID.h>
#include <SPI.h>
#include <new.h>

#include <Wire.h>                       // I2C control library
#include <LiquidCrystal_I2C.h>          // LCD library

#define    uchar    unsigned char
#define    uint    unsigned int

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
 
class Closet {
  private :
  String masterName;
  int masterUID;
  DailyLook* list;

 public :
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

 /* 
 * -*-*-*-*-*-*-*-*-*-*-*-*-MAIN PART-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
 
// for HW


// for data
 Serial.begin(9600);
 int curtemper;
 int curhumid;
 String weatherMsg[];
 MusicLib musics;
 DailyLook curChoices[];
 Closet closets[];

void setup(){
  Serial.begin(9600);
  
 //led setup
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
  
 //lcd setup
  lcd.init();                      // initialize the lcd 
  
  // start the SPI library:(RFID setup)
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);
 
  myRFID.AddicoreRFID_Init(); 
 // data setup
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


