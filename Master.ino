/*
 * Wire : Master
 * Board # : 1
 * Components : Piezo Speaker, Temperature Humidity senser, RFID module, two LED
 * 
 */

#include <AddicoreRFID.h> // RFID library
#include <SPI.h> // RFID library
#include <Wire.h> // I2C communication library
#include <DHT11.h> // Temperature & Humidity sensor library

#define SLAVE 4 // 슬레이브 주소

#define MAX_LEN 16

#define    uchar    unsigned char
#define    uint    unsigned int

#define chipSelectPin 10

#define NRSTPD 9
#define LED3_RED 8
#define LED3_GRE 7
#define LED3_BLU 6

#define LED4_RED 5
#define LED4_GRE 4
#define LED4_BLU 3

#define TEM_HUM 1

#define SPEAKER 0

//4 bytes tag serial number, the first 5 bytes for the checksum byte
uchar serNumA[5];
 
uchar fifobytes;
uchar fifoValue;
 
AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module

DHT11 dht11(TEM_HUM); 

int  err;
float curtemp, curhumid;
String weatherMsg[10];       

/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////

 
class DailyLook {
  public :
    static const int COLOR_SIZE = 3; //0; 자켓, 1; 윗도리, 2; 바지

    String name = " ";
    int* category = {}; // 0;jacket, 1;top, 2;pants, 3;skirt
    int colors[COLOR_SIZE][3] = {};
    int  score = 0;
    int temperBase = -1;
    int temperLimit = -1;
    int humidBase = -1;
    int humidLimit = -1;
    boolean currentUse = false;

  //public :
    DailyLook () {
      Serial.println("no input!!");
    }

    DailyLook(String name,  int category[3],  int colors[COLOR_SIZE][3],
              int temperBase, int temperLimit,  int humidBase,  int humidLimit)
      : name(name)
      , category(category)
      , temperBase(temperBase)
      , temperLimit(temperLimit)
      , humidBase(humidBase)
      , humidLimit(humidLimit) {

      for (int i = 0; i < 3; ++i) {
         for (int j = 0; j < COLOR_SIZE; ++j) {
          this->colors[j][i] = colors[j][i];
         }
      }
    }

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
    void toString() {
        Serial.print("name : "); Serial.print(this->getName());
        Serial.print("\tscore : "); Serial.print(this->getScore());
        Serial.print("\tcategory : ");
        for (int i=0; this->getCategory()[i]!=-1 && i<COLOR_SIZE; ++i){
          switch(this->getCategory()[i]){
            case 0 :
             Serial.print("Outer");
             break;
            case 1 :
              Serial.print("Top");
              break;
            case 2 :
              Serial.print("Pants");
              break;
            case 3 :
              Serial.print("Skirt");
              break;
            default:
              break;
          }
          if(this->getCategory()[i+1]!=-1 && i!=2){
              Serial.print(", ");
          }
        }
        Serial.print("\ttemperBase : "); Serial.print(this->getTemperBase()); Serial.print("\ttemperLimit : "); Serial.print(this->getTemperLimit());
        Serial.print("\thumidBase : "); Serial.print(this->getHumidBase()); Serial.print("\thumidLimit : "); Serial.print(this->getHumidLimit());
        Serial.println("");
    }
};

class Closet {
  public :
    String masterName = "";
    int masterUID = -1;
    DailyLook* list[20]= {};

  //public :
    Closet(String name, int UID):
      masterName(name), masterUID(UID) {}

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
      for (int i = 0; i < 20; ++i) {
        this->list[i] = list[i];
      }
    }
    void toString() {
      for (int i = 0; i < 20; i++) {
        if(list[i]->getName() != ""){
          Serial.println(list[i]->getName());
          list[i]->toString();
          delay(1000);
        }
        else {
          Serial.println("out");
          return;
        }
      }
    } //end fo toString
};

class MusicLib {

  public :
    /*

      void MusicLib(){}

      void playMusic(String Title){

      }
    */
};
/*
 * read button number
 */
int readButton(){
  // read button info
  Wire.requestFrom(SLAVE, 2);
  return Wire.read();
}
/*
 * send a message to slave
 */
void sendRGB(int category, int* rgb){
  Wire.beginTransmission(2);
  Wire.write(1); //sending RGB light value
  Wire.write(category);
  for (int i=0; i<3; ++i){
    Wire.write(rgb[i]);   
  }
  Wire.endTransmission();
}

void sendMessage(char yesno, String msg)
{      /*
       * LCD send
       * 1. send 2
       * 2. send ! (if you want to print yes no button on second row)or sth else
       * 3. send msg
       */
  Wire.beginTransmission(2);
  Wire.write(2); // 1. sending LED message
  if (yesno == '!'){// 2. add YES NO Button or not
    Wire.write('!');
  } // 3. send Message
  for (int i=0; i<msg.length(); ++i){
    Wire.write(msg[i]);   
  }
  Wire.endTransmission(); //message buffer is sent with Wire.endTransmission()                   
  delay(500);
}


void scoreDailyLook () {
  /*
     TODO 점수 매기기
  */
}

int cate1[] =  {1, 2};
int color1[2][3] = {{255, 255, 255}, {0, 0, 255}};
int cate2[] = {1, 3};
int color2[2][3] = {{0, 0, 0}, {255, 0, 0}};
DailyLook dl1 = DailyLook(String("Whiteshirt&bluepants"), cate1, color1, 15, 28, 0, 100);
DailyLook dl2 = DailyLook(String("BlackSweater&redpants"), cate2, color2, 0, 50, 0, 100);

DailyLook* MinsuList[] = {&dl1, &dl2};

Closet* minsuCloset = new Closet("Minsu", 23);

void setup() {

  MusicLib musics;

  Serial.begin(9600); 
  minsuCloset->setList(MinsuList);
  //led setup  
  pinMode(LED3_RED, OUTPUT);
  pinMode(LED3_GRE, OUTPUT);
  pinMode(LED3_BLU, OUTPUT);
  
  pinMode(LED4_RED, OUTPUT);
  pinMode(LED4_GRE, OUTPUT);
  pinMode(LED4_BLU, OUTPUT);
  // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);
 
  myRFID.AddicoreRFID_Init();  

}//end of setup

void loop() {
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
          /*
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
          */ 
            // Should really check all pairs, but for now we'll just use the first
            if(str[0] == 100)                      //RFID 태그의 ID값이 100번이면 Eunjin의 카드
            {
                Serial.print("Hello Eunjin!\n");
            } else if(str[0] == 170) {             //RFID 태그의 ID값이 64번이면 Sangwu의 카드
                Serial.print("Hello Sangwu!\n");
            } else {
               Serial.print("Wrong Input!\n");
            }
            Serial.println();
            delay(1000);
    } else {
      /*
       * 날씨 출력함수
       */
    }
        myRFID.AddicoreRFID_Halt();           //Command tag into hibernation    
}

void printWeather(int temper, int humid) {
  /*
    int  err;
    float curtemp, curhumid;
    String weatherMsg[10];       
  */
  if((err=dht11.read(curhumid, curtemp))==0) //온도, 습도 읽어와서 표시
  {
    Serial.print("temperature:");
    Serial.print(curtemp);
    Serial.print(" humidity:");
    Serial.print(curtemp);
    Serial.println();
  }
  else                                //에러일 경우 처리
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();    
  }
  delay(1000);                        //1초마다 측정
  /* 
   *  print current Weather

  */
}
