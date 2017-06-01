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

//sangwu의 코디와 옷장
int scate1[] = {1, 2, -1};
int scolor1[2][3] = {{139, 188, 255}, {250, 244, 192}, {-1, -1, -1}};
int scate2[] = {1, 2, -1};
int scolor2[2][3] = {{255, 228, 0}, {255, 255, 255}, {-1, -1, -1}};
int scate3[] = {1, 2, -1};
int scolor3[2][3] = {{0, 0, 0}, {255, 255, 36}, {-1, -1, -1}};
int scate4[] = {1, 2, -1};
int scolor4[2][3] = {{255, 94, 0}, {0, 0, 0}, {-1, -1, -1}};
int scate5[] = {1, 2, -1};
int scolor5[2][3] = {{153, 0, 76}, {0, 0, 119}, {-1, -1, -1}}; 
int scate6[] = {1, 2, -1};
int scolor6[2][3] = {{255, 255, 255}, {0, 0, 0}, {-1, -1, -1}};
int scate7[] = {1, 2, -1};
int scolor7[2][3] = {{0, 0, 0}, {0, 103, 0}, {-1, -1, -1}};
int scate8[] = {1, 2, -1};
int scolor8[2][3] = {{255, 133, 133}, {250, 244, 192}, {-1, -1, -1}};
int scate9[] = {1, 2, -1};
int scolor9[2][3] = {{243, 97, 220}, {0, 0, 119}, {-1, -1, -1}};
int scate10[] = {1, 2, -1};
int scolor10[2][3] = {{5, 0, 153}, {255, 255, 36}, {-1, -1, -1}};
int scate11[] = {1, 2, -1};
int scolor11[2][3] = {{0, 103, 0}, {0, 0, 119}, {-1, -1, -1}};
int scate12[] = {0, 1, 2};
int scolor12[2][3] = {{243, 202, 90}, {250, 244, 193}, {0, 0, 119}}; 
int scate13[] = {0, 1, 2};
int scolor13[2][3] = {{0, 0, 119}, {255, 255, 255}, {0, 0, 0}};
int scate14[] = {0, 1, 2};
int scolor14[2][3] = {{0, 103, 0}, {153, 0, 76}, {0, 0, 119}};
int scate15[] = {1, 2, -1};
int scolor15[2][3] = {{0, 0, 119}, {243, 202, 90}, {-1, -1, -1}};
int scate16[] = {0, 1, 2};
int scolor16[2][3] = {{243, 202, 90}, {166, 166, 166}, {0, 0, 119}};
int scate17[] = {0, 1, 2};;
int scolor17[2][3] = {{0, 0, 119}, {153, 0, 76}, {0, 0, 119}};
int scate18[] = {0, 1, 2};
int scolor18[2][3] = {{0, 103, 0}, {166, 166, 166}, {250, 244, 192}};
int scate19[] = {0, 1, 2};
int scolor19[2][3] = {{153, 0, 76}, {0, 0, 0}, {0, 0, 119}};
int scate20[] = {0, 1, 2};
int scolor20[2][3] = {{166, 166, 166}, {255, 255, 255}, {0, 0, 0}};

DailyLook sdl1 = DailyLook(String("paleblue_shirt & ivory_pants",), scate1, scolor1, 11, 20, 65, 74);
DailyLook sdl2 = DailyLook(String("deepyellow_shirt & white_pants",), scate2, scolor2, 11, 20, 65, 74);
DailyLook sdl3 = DailyLook(String("black_shirt & yellow_pants",), scate3, scolor3, 11, 20, 65, 74);
DailyLook sdl4 = DailyLook(String("orange_shirt & black_pants",), scate4, scolor4, 11, 20, 65, 74);
DailyLook sdl5 = DailyLook(String("wine_shirt & deepblue_pants",), scate5, scolor5, 11, 20, 65, 74);
DailyLook sdl6 = DailyLook(String("white_shirt & black_pants",), scate6, scolor6, 21, 50, 75, 100);
DailyLook sdl7 = DailyLook(String("black_shirt & deepgreen_pants",), scate7, scolor7, 21, 50, 75, 100);
DailyLook sdl8 = DailyLook(String("pink_shirt & ivory_pants",), scate8, scolor8, 21, 50, 75, 100);
DailyLook sdl9 = DailyLook(String("palepurple_shirt & deepblue_pants",), scate9, scolor9, 21, 50, 75, 100);
DailyLook sdl10 = DailyLook(String("navy_shirt & yellow_pants",), scate10, scolor10, 21, 50, 0, 100);
DailyLook sdl11 = DailyLook(String("deepgreen_shirt & deepblue_pants",), scate11, scolor11, 11, 20 , 65, 74);
DailyLook sdl12 = DailyLook(String("palebrown_outer & ivory_shirt & deepblue_pants",), scate12, scolor12, 11, 20, 65, 74);
DailyLook sdl13 = DailyLook(String("deepblue_outer & white_shirt & black_pants",), scate13, scolor13, 11, 20, 65, 74);
DailyLook sdl14 = DailyLook(String("deepgreen_outer & wine_shirt & deepblue_pants",), scate14, scolor14, 11, 20, 65, 74);
DailyLook sdl15 = DailyLook(String("deepblue_shirt & palebrown_pants",), scate15, scolor15, 11, 20, 65, 74);
DailyLook sdl16 = DailyLook(String("palebrown_outer & grey_shirt & deepblue_pants",), scate16, scolor16, -40, 10, 0, 65);
DailyLook sdl17 = DailyLook(String("deepblue_outer & wine_shirt & deepblue_pants",), scate17, scolor17, -40, 10, 0, 65);
DailyLook sdl18 = DailyLook(String("deepgreen_outer & grey_shirt & ivory_pants",), scate18, scolor18, -40, 10, 0, 65);
DailyLook sdl19 = DailyLook(String("wine_outer & black_shirt & deepblue_pants',), scate19, scolor19, -40, 10, 0, 65);
DailyLook sdl20 = DailyLook(String("grey_outer & white_shirt & black_pants',), scate20, scolor20, -40, 10, 0, 65);

DailyLook* SangwuList[] = {&sdl1, &sdl2, &sdl3, &sdl4, &sdl5, &sdl6, &sdl7, &sdl8, &sdl9, &sdl10, &sdl11, &sdl12, &sdl13, &sdl14, &sdl15, &sdl16, &sdl17, &sdl18, &sdl19, &sdl20 };

Closet* sangwuCloset = new Closet("Sangwu", 23);



//eunjin의 코디와 옷장
int cate1[] = {1, 3, -1};
int color1[2][3] = {{255, 255, 255}, {255, 133, 133}, {-1, -1, -1}};
int cate2[] = {1, 3, -1};
int color2[2][3] = {{189, 255, 18}, {255, 255, 255}, {-1, -1, -1}};
int cate3[] = {1, 3, -1};
int color3[2][3] = {{255, 255, 255}, {153, 0, 76}, {-1, -1, -1}};
int cate4[] = {1, 2, -1};
int color4[2][3] = {{1, 0, 255}, {255, 255, 255}, {-1, -1, -1}};
int cate5[] = {1, 2, -1};
int color5[2][3] = {{255, 133, 133}, {255, 255, 36}, {-1, -1, -1}};
int cate6[] = {1, 2, -1};
int color6[2][3] = {{255, 255, 255}, {1, 0, 255}, {-1, -1, -1}};
int cate7[] = {1, 2, -1};
int color7[2][3] = {{255, 255, 36}, {1, 0, 255}, {-1, -1, -1}};
int cate8[] = {1, 3, -1};
int color8[2][3] = {{128, 65, 217}, {255, 133, 133}, {-1, -1, -1}};
int cate9[] = {1, 3, -1};
int color9[2][3] = {{255, 0, 0}, {128, 65 ,217}, {-1, -1, -1}};
int cate10[] = {1, 3, -1};
int color10[2][3] = {{255, 133, 133}, {5, 0, 153}, {-1, -1, -1}};
int cate11[] = {1, 2, -1};
int color11[2][3] = {{128, 65, 217}, {166, 166, 166}, {-1, -1, -1}};
int cate12[] = {0, 1, 2};
int color12[2][3] = {{232, 218, 179}, {206, 251, 201}, {1, 0, 255}};
int cate13[] = {0, 1, 2};
int color13[2][3] = {{135, 38, 0}, {255, 255, 255}, {0, 0, 0}};
int cate14[] = {0, 1, 3};
int color14[2][3] = {{250, 244, 192}, {128, 65, 217}, {255, 0, 0}};
int cate15[] = {0, 1, 3};int ecate1[] = {1, 3, -1};
int ecolor1[2][3] = {{255, 255, 255}, {255, 133, 133}, {-1, -1, -1}};
int ecate2[] = {1, 3, -1};
int ecolor2[2][3] = {{189, 255, 18}, {255, 255, 255}, {-1, -1, -1}};
int ecate3[] = {1, 3, -1};
int ecolor3[2][3] = {{255, 255, 255}, {153, 0, 76}, {-1, -1, -1}};
int ecate4[] = {1, 2, -1};
int ecolor4[2][3] = {{1, 0, 255}, {255, 255, 255}, {-1, -1, -1}};
int ecate5[] = {1, 2, -1};
int ecolor5[2][3] = {{255, 133, 133}, {255, 255, 36}, {-1, -1, -1}};
int ecate6[] = {1, 2, -1};
int ecolor6[2][3] = {{255, 255, 255}, {1, 0, 255}, {-1, -1, -1}};
int ecate7[] = {1, 2, -1};
int ecolor7[2][3] = {{255, 255, 36}, {1, 0, 255}, {-1, -1, -1}};
int ecate8[] = {1, 3, -1};
int ecolor8[2][3] = {{128, 65, 217}, {255, 133, 133}, {-1, -1, -1}};
int ecate9[] = {1, 3, -1};
int ecolor9[2][3] = {{255, 0, 0}, {128, 65 ,217}, {-1, -1, -1}};
int ecate10[] = {1, 3, -1};
int ecolor10[2][3] = {{255, 133, 133}, {5, 0, 153}, {-1, -1, -1}};
int ecate11[] = {1, 2, -1};
int ecolor11[2][3] = {{128, 65, 217}, {166, 166, 166}, {-1, -1, -1}};
int ecate12[] = {0, 1, 2};
int ecolor12[2][3] = {{232, 218, 179}, {206, 251, 201}, {1, 0, 255}};
int ecate13[] = {0, 1, 2};
int ecolor13[2][3] = {{135, 38, 0}, {255, 255, 255}, {0, 0, 0}};
int ecate14[] = {0, 1, 3};
int ecolor14[2][3] = {{250, 244, 192}, {128, 65, 217}, {255, 0, 0}};
int ecate15[] = {0, 1, 3};
int ecolor15[2][3] = {{0, 0, 0}, {255, 255, 255}, {89, 135, 0}};
int ecate16[] = {0, 1, 2};
int ecolor16[2][3] = {{153, 255, 217}, {1, 0, 255}, {255, 255, 255}};
int ecate17[] = {0, 1, 3};
int ecolor17[2][3] = {{243, 202, 90}, {0, 0, 0}, {153, 0, 76}};
int ecate18[] = {0, 1, 2};
int ecolor18[2][3] = {{0, 0, 0}, {0, 0, 119}, {0, 0, 0}};
int ecate19[] = {0, 1, 2};
int ecolor19[2][3] = {{166, 166, 166}, {255, 255, 255}, {1, 0, 255}};
int ecate20[] = {0, 1, 2};
int ecolor20[2][3] = {{0, 0, 199}, {166, 166, 166}, {0, 0, 0}};
                                  
DailyLook edl1 = DailyLook(String("white_shirt & pink_skirt"), ecate1, ecolor1, 11, 20, 65, 74);
DailyLook edl2 = DailyLook(String("yellowgreen_shirt & white_skirt"), ecate2, ecolor2, 11, 20, 65, 74);
DailyLook edl3 = DailyLook(String("white_shirt & wine_skirt"), ecate3, ecolor3, 11, 20, 65, 74);
DailyLook edl4 = DailyLook(String("blue_shirt & white_pants"), ecate4, ecolor4, 11, 20, 65, 74);
DailyLook edl5 = DailyLook(String"pink_shirt & yellow_pants"), ecate5, ecolor5, 11, 20, 65, 74);
DailyLook edl6 = DailyLook(String("white_shirt & blue_pants"), ecate6, ecolor6, 21, 50, 75, 100);
DailyLook edl7 = DailyLook(String("yellow_shirt & blue_pants"), ecate7, ecolor7, 21, 50, 75, 100);
DailyLook edl8 = DailyLook(String("purple_shirt & pink_skirt"), ecate8, ecolor8, 21, 50, 75, 100);
DailyLook edl9 = DailyLook(String("red_shirt & purple_skirt"), ecate9, ecolor9, 21, 50, 75, 100);
DailyLook edl10 = DailyLook(String("pink_shirt & navy_skirt"), ecate10, ecolor10, 21, 50, 0, 100);
DailyLook edl11 = DailyLook(String("purple_shirt & grey_pants"), ecate11, ecolor11, 11, 20 , 65, 74);
DailyLook edl12 = DailyLook(String("beige_outer & pea_shirt & blue_pants"), ecate12, ecolor12, 11, 20, 65, 74);
DailyLook edl13 = DailyLook(String("brown_outer & white_shirt & black_pants"), ecate13, ecolor13, 11, 20, 65, 74);
DailyLook edl14 = DailyLook(String("ivory_outer & purple_shirt & red_skirt"), ecate14, ecolor14, 11, 20, 65, 74);
DailyLook edl15 = DailyLook(String("black_outer & white_shirt & deepgreen_skirt"), ecate15, ecolor15, 11, 20, 65, 74);
DailyLook edl16 = DailyLook(String("mint_outer & blue_shirt & white_pants"), ecate16, ecolor16, -40, 10, 0, 65);
DailyLook edl17 = DailyLook(String("palebrown_outer & black_shirt & wine_skirt"), ecate17, ecolor17, -40, 10, 0, 65);
DailyLook edl18 = DailyLook(String("black_outer & deepblue_shirt & black_pants"), ecate18, ecolor18, -40, 10, 0, 65);
DailyLook edl19 = DailyLook(String("grey_outer & white_shirt & blue_pants"), ecate19, ecolor19, -40, 10, 0, 65);
DailyLook edl20 = DailyLook(String("deepblue_outet & grey_shirt & black_pants"), ecate20, ecolor20, -40, 10, 0, 65);

DailyLook* EunjinList[] = {&edl1, &edl2, &edl3, &edl4, &edl5, &edl6, &edl7, &edl8, &edl9, &edl10, &edl11, &edl12, &edl13, &edl14, &edl15, &edl16, &edl17, &edl18, &edl19, &edl20};

Closet* eunjinCloset = new Closet("Eunjin", 23);

                                 
void setup() {

  MusicLib musics;

  Serial.begin(9600); 
  sangwuCloset->setList(SangwuList);
  eunjinCloset->setList(EunjinList);
 
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
            } else if(str[0] == 64) {             //RFID 태그의 ID값이 64번이면 Sangwu의 카드
                Serial.print("Hello Sangwu!\n");
            } else {
               Serial.print("Wrong Input!\n");
            }
            Serial.println();
            delay(1000);
    } else {
     printWeather(curtemp, curhumid);
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
