#include <dht.h>
#include <uart.h>
#include <util.h>

#include <AddicoreRFID.h>
#include <SPI.h>
#include <new.h>

#include <Wire.h>                       /* I2C control library*/
/* LCD library */

#define    uchar    unsigned char
#define    uint     unsigned int

class DailyLook {
private :
  static const byte COLOR_SIZE = 3; //0; 자켓, 1; 윗도리, 2; 바지

  String name;
  unsigned byte* category; // 0;jacket, 1;top, 2;pants, 3;skirt
  unsigned byte* colors[COLOR_SIZE];
  unsigned byte  score = 0;
  byte temperBase;
  byte temperLimit;
  unsigned byte humidBase;
  unsigned byte humidLimit;
  boolean currentUse = false;

public :
  DailyLook (){
    Serial.println("no input!!");
  }

  DailyLook(String name, unsigned byte category[3], unsigned byte colors[COLOR_SIZE][3],
            byte temperBase, byte temperLimit, unsigned byte humidBase, unsigned byte humidLimit)
    : name(name)
    , category(category)
    , temperBase(temperBase)
    , temperLimit(temperLimit)
    , humidBase(humidBase)
    , humidLimit(humidLimit) {

    for (int i = 0; i < 3; ++i) {
      
    }
  }

  String getName() {
    return name;
  }
  void toString(){
    Serial.print("name : ");Serial.print(name);
    Serial.print("\tscore : ");Serial.print(score);
    Serial.print("\ttemperBase : ");Serial.print(temperBase);Serial.print("\ttemperLimit : ");Serial.print(temperLimit);
    Serial.print("\thumidBase : ");Serial.print(humidBase);Serial.print("\thumidLimit : ");Serial.print(humidLimit);
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

class Closet {
private :
  String masterName;
  int masterUID;
  DailyLook* list[20];

public :
  Closet(String name, int UID):
    masterName(name), masterUID(UID){}

  void toString(){
    for(int i=0; i<20;i++) list[i]->toString();
  }
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
  /*
    DailyLook* getList() {
    return this->list;
    }
  */
  void setList(DailyLook* list[20]) {
    for (int i=0; i<20; ++i){
      this->list[i] = list[i];
    }
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
  /* print current Weather
   *  
   */
}
  
  
/*
  DailyLook chooseDailyLook(Closet person, int temper, int humid){
    
  * DailyLook loook = new DailyLook();
  * return loook;
     
    
  }
*/
void scoreDailyLook(DailyLook look){
  /*
   * TODO 점수 매기기
   */
}

/* 
 * -*-*-*-*-*-*-*-*-*-*-*-*-MAIN PART-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
 
// for HW


// for data
int curtemper;
int curhumid;
String weatherMsg[10];
MusicLib musics;/*
                  DailyLook curChoices[];
                  Closet closets[];
                */

int cate1[] =  {1,2};
int color1[2][3] = {{255,255,255},{0,0,255}};
int cate2[] = {1,3};
int color2[2][3] = {{0,0,0}, {255,0,0}};
DailyLook dl1 = DailyLook(String("Whiteshirt&bluepants"), cate1, color1,15, 28, 0, 100);
DailyLook dl2 = DailyLook(String("BlackSweater&redpants"), cate2, color2,0, 50, 0, 100);
DailyLook* MinsuList[] = {&dl1, &dl2};

Closet* minsuCloset = new Closet("Minsu", 23);


/*
    #include <Wire.h>
    #define SLAVE 4 // 슬레이브 주소
    void i2c_communication() {
    Wire.requestFrom(SLAVE, 1); // 1 바이트 크기의 데이터 요청
    char c = Wire.read(); // 수신 데이터 읽기
    Serial.println(c); // 수신 데이터 출력
    }
*/

void setup(){
  Wire.begin(); // Wire 라이브러리 초기화
  Serial.begin(9600); // 직렬 통신 초기화

  
  minsuCloset->setList(MinsuList);
  /*
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
  */
  /*data setup
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
  //i2c_communication(); // 슬레이브로 데이터 요구 및 수신 데이터 처리
  delay(1000);
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
