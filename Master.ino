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
    int score = 0;
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
int scate[][3] = {
	{1, 2, -1},
	{1, 2, -1},
 {1, 2, -1},
	{1, 2, -1},
	{1, 2, -1},
	{1, 2, -1},
	{1, 2, -1},
	{1, 2, -1},
	{1, 2, -1},
	{1, 2, -1},
	{1, 2, -1},
	{0, 1, 2},
	{0, 1, 2},
	{0, 1, 2},
	{1, 2, -1},
	{0, 1, 2},
	{0, 1, 2},
	{0, 1, 2},
	{0, 1, 2}, 
	{0, 1, 2},
};

int scolor[][3][3] = {
	{
		{139, 188, 255},
		{250, 244, 192},
		{-1, -1, -1}
	},
	{
		{255, 228, 0},
		{255, 255, 255},
		{-1, -1, -1}
	},
	{
		{0, 0, 0},
		{255, 255, 36},
		{-1, -1, -1}
	},
	{
		{255, 94, 0},
		{0, 0, 0},
		{-1, -1, -1}
	},
	{
		{153, 0, 76},
		{0, 0, 119},
		{-1, -1, -1}
	},
	{
		{255, 255, 255},
		{0, 0, 0},	
		{-1, -1, -1}
	},
	{
		{0, 0, 0},
		{0, 103, 0},
		{-1, -1, -1}
	},
	{
		{255, 133, 133},
		{250, 244, 192},
		{-1, -1, -1}
	},
	{
		{243, 97, 220},
		{0, 0, 119},
		{-1, -1, -1}
	},
	{
		{5, 0, 153},
		{255, 255, 36},
		{-1, -1, -1}
	},
	{
		{0, 103, 0},
		{0, 0, 119},
		{-1, -1, -1}
	},
	{
		{243, 202, 90},
		{250, 244, 193},
		{0, 0, 119}
	},
	{
		{0, 0, 119},
		{255, 255, 255},
		{0, 0, 0}
	},
	{
		{0, 103, 0},
		{153, 0, 76},
		{0, 0, 119}
	},
	{
		{0, 0, 119},
		{243, 202, 90},
		{-1, -1, -1}
	},
	{
		{243, 202, 90},
		{166, 166, 166},
		{0, 0, 119}
	},
	{
		{0, 0, 119},
		{153, 0, 76},
		{0, 0, 119}
	},
	{
		{0, 103, 0},
		{166, 166, 166},
		{250, 244, 192}
	},
	{
		{153, 0, 76},
		{0, 0, 0},
		{0, 0, 119}
	},
	{
		{166, 166, 166},
		{255, 255, 255},
		{0, 0, 0}
	}
};

DailyLook sdl1 = DailyLook(String("paleblue_shirt & ivory_pants"), scate[0][0], scolor[0][0][0], 11, 20, 65, 74);
DailyLook sdl2 = DailyLook(String("deepyellow_shirt & white_pants"), scate[1][0], scolor[1][0][0], 11, 20, 65, 74);
DailyLook sdl3 = DailyLook(String("black_shirt & yellow_pants"), scate[2][0], scolor[2][0][0], 11, 20, 65, 74);
DailyLook sdl4 = DailyLook(String("orange_shirt & black_pants"), scate[3][0], scolo[3][0][0]r, 11, 20, 65, 74);
DailyLook sdl5 = DailyLook(String("wine_shirt & deepblue_pants"), scate[4][0], scolor[4][0][0], 11, 20, 65, 74);
DailyLook sdl6 = DailyLook(String("white_shirt & black_pants"), scate[5][0], scolor[5][0][0], 21, 50, 75, 100);
DailyLook sdl7 = DailyLook(String("black_shirt & deepgreen_pants"), scate[6][0], scolor[6][0][0], 21, 50, 75, 100);
DailyLook sdl8 = DailyLook(String("pink_shirt & ivory_pants"), scate[7][0], scolor[7][0][0], 21, 50, 75, 100);
DailyLook sdl9 = DailyLook(String("palepurple_shirt & deepblue_pants"), scate[8][0], scolor[8][0][0], 21, 50, 75, 100);
DailyLook sdl10 = DailyLook(String("navy_shirt & yellow_pants"), scate[9][0], scolor[9][0][0], 21, 50, 0, 100);
DailyLook sdl11 = DailyLook(String("deepgreen_shirt & deepblue_pants"), scate[10][0], scolor[10][0][0], 11, 20 , 65, 74);
DailyLook sdl12 = DailyLook(String("palebrown_outer & ivory_shirt & deepblue_pants"), scate[11][0], scolor[11][0][0], 11, 20, 65, 74);
DailyLook sdl13 = DailyLook(String("deepblue_outer & white_shirt & black_pants"), scate[12][0], scolor[12][0][0], 11, 20, 65, 74);
DailyLook sdl14 = DailyLook(String("deepgreen_outer & wine_shirt & deepblue_pants"), scate[13][0], scolor[13][0][0], 11, 20, 65, 74);
DailyLook sdl15 = DailyLook(String("deepblue_shirt & palebrown_pants"), scate[14][0], scolor[14][0][0], 11, 20, 65, 74);
DailyLook sdl16 = DailyLook(String("palebrown_outer & grey_shirt & deepblue_pants"), scate[15][0], scolor[15][0][0], -40, 10, 0, 65);
DailyLook sdl17 = DailyLook(String("deepblue_outer & wine_shirt & deepblue_pants"), scate[16][0], scolor[16][0][0], -40, 10, 0, 65);
DailyLook sdl18 = DailyLook(String("deepgreen_outer & grey_shirt & ivory_pants"), scate[17][0], scolor[17][0][0], -40, 10, 0, 65);
DailyLook sdl19 = DailyLook(String("wine_outer & black_shirt & deepblue_pants"), scate[18][0], scolor[18][0][0], -40, 10, 0, 65);
DailyLook sdl20 = DailyLook(String("grey_outer & white_shirt & black_pants"), scate[19][0], scolor[19][0][0], -40, 10, 0, 65);



DailyLook* SangwuList[] = {&sdl1, &sdl2, &sdl3, &sdl4, &sdl5, &sdl6, &sdl7, &sdl8, &sdl9, &sdl10, &sdl11, &sdl12, &sdl13, &sdl14, &sdl15, &sdl16, &sdl17, &sdl18, &sdl19, &sdl20 };

Closet* sangwuCloset = new Closet("Sangwu", 23);



//eunjin의 코디와 옷장
int ecate[][3] = {
	{1, 3 -1},
	{1, 3 -1},
	{1, 3 -1},
	{1, 2, -1},
	{1, 2, -1},
	{1, 2, -1},
	{1, 2, -1},
	{1, 3, -1},
	{1, 3, -1},
	{1, 3, -1},
	{1, 2, -1},
	{0, 1, 2},
	{0, 1, 2},
	{0, 1, 3},
	{0, 1, 3},
	{0, 1, 2},
	{0, 1, 3},
	{0, 1, 2},
	{0, 1, 2},
	{0, 1, 2}
};

int ecolor[][3][3] = {
 {
		{255, 255, 255},
		{255, 133, 133},
		{-1, -1, -1}
	},
	{
		{189, 255, 18},
		{255, 255, 255},
		{-1, -1, -1}
	},
{
		{255, 255, 255},
		{153, 0, 76},
		{-1, -1, -1}
},
{
		{1, 0, 255},
		{255, 255, 255},
		{-1, -1, -1}
},
{
		{255, 133, 133},
		{255, 255, 36},
		{-1, -1, -1}
},
{
		{255, 255, 255},
		{1, 0, 255},
		{-1, -1, -1}
},
{
		{255, 255, 36},
		{1, 0, 255},
		{-1, -1, -1}
},
{
		{128, 65, 217},
		{255, 133, 133},
		{-1, -1, -1}
},
{
		{255, 0, 0},
		{128, 65 ,217},
		{-1, -1, -1}
},
{
		{255, 133, 133},
		{5, 0, 153},
		{-1, -1, -1}
},
{
		{128, 65, 217},
		{166, 166, 166},
		{-1, -1, -1}
},
{
		{232, 218, 179},
		{206, 251, 201},
		{1, 0, 255}
},
{
		{135, 38, 0},
		{255, 255, 255},
		{0, 0, 0}
},
{
		{250, 244, 192},
		{128, 65, 217},
		{255, 0, 0}
},
{
		{0, 0, 0},
		{255, 255, 255},
		{89, 135, 0}
},
{
		{153, 255, 217},
		{1, 0, 255},
		{255, 255, 255}
},
{
		{243, 202, 90},
		{0, 0, 0},
		{153, 0, 76}
},
{
		{0, 0, 0},
		{0, 0, 119},
		{0, 0, 0}
},
{
		{166, 166, 166},
		{255, 255, 255},
		{1, 0, 255}
},
{
		{0, 0, 199},
		{166, 166, 166},
		{0, 0, 0}
 }
};

DailyLook edl1 = DailyLook(String("white_shirt & pink_skirt"), ecate[0][0], ecolor[0][0][0], 11, 20, 65, 74);
DailyLook edl2 = DailyLook(String("yellowgreen_shirt & white_skirt"), ecate[1][0], ecolor[1][0][0], 11, 20, 65, 74);
DailyLook edl3 = DailyLook(String("white_shirt & wine_skirt"), ecate[2][0], ecolor[2][0][0], 11, 20, 65, 74);
DailyLook edl4 = DailyLook(String("blue_shirt & white_pants"), ecate[3][0], scolo[3][0][0]r, 11, 20, 65, 74);
DailyLook edl5 = DailyLook(String("pink_shirt & yellow_pants"), ecate[4][0], ecolor[4][0][0], 11, 20, 65, 74);
DailyLook edl6 = DailyLook(String("white_shirt & blue_pants"), ecate[5][0], ecolor[5][0][0], 21, 50, 75, 100);
DailyLook edl7 = DailyLook(String("yellow_shirt & blue_pants"), ecate[6][0], ecolor[6][0][0], 21, 50, 75, 100);
DailyLook edl8 = DailyLook(String("purple_shirt & pink_skirt"), ecate[7][0], ecolor[7][0][0], 21, 50, 75, 100);
DailyLook edl9 = DailyLook(String("red_shirt & purple_skirt"), ecate[8][0], ecolor[8][0][0], 21, 50, 75, 100);
DailyLook edl10 = DailyLook(String("pink_shirt & navy_skirt"), ecate[9][0], ecolor[9][0][0], 21, 50, 0, 100);
DailyLook edl11 = DailyLook(String("purple_shirt & grey_pants"), ecate[10][0], ecolor[10][0][0], 11, 20 , 65, 74);
DailyLook edl12 = DailyLook(String("beige_outer & pea_shirt & blue_pants"), ecate[11][0], ecolor[11][0][0], 11, 20, 65, 74);
DailyLook edl13 = DailyLook(String("brown_outer & white_shirt & black_pants"), ecate[12][0], ecolor[12][0][0], 11, 20, 65, 74);
DailyLook edl14 = DailyLook(String("ivory_outer & purple_shirt & red_skirt"), ecate[13][0], ecolor[13][0][0], 11, 20, 65, 74);
DailyLook edl15 = DailyLook(String("black_outer & white_shirt & deepgreen_skirt"), ecate[14][0], ecolor[14][0][0], 11, 20, 65, 74);
DailyLook edl16 = DailyLook(String("mint_outer & blue_shirt & white_pants"), ecate[15][0], ecolor[15][0][0], -40, 10, 0, 65);
DailyLook edl17 = DailyLook(String("palebrown_outer & black_shirt & wine_skirt"), ecate[16][0], ecolor[16][0][0], -40, 10, 0, 65);
DailyLook edl18 = DailyLook(String("black_outer & deepblue_shirt & black_pants"), ecate[17][0], ecolor[17][0][0], -40, 10, 0, 65);
DailyLook edl19 = DailyLook(String("grey_outer & white_shirt & blue_pants"), ecate[18][0], ecolor[18][0][0], -40, 10, 0, 65);
DailyLook edl20 = DailyLook(String("deepblue_outet & grey_shirt & black_pants"), ecate[19][0], ecolor[19][0][0], -40, 10, 0, 65);

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
           if(str[0] == 64) {             //RFID 태그의 ID값이 64번이면 Sangwu의 카드
                
             //반복문으로 코디 탐색하다가 온도 만족하면 코디 string 출력, RGB값 전송		//그렇지 않으면 continue
             //no 선택 시 반복문 반복			//yes 선택 시 프로그램 종료
             //i가 19가 되면 추천 코디 소진 알림
            }
                
           else if(str[0] == 100) {            //RFID 태그의 ID값이 100번이면 Eunjin의 카드
                Serial.print("Hello Eunjin!\n");
            
            //반복문으로 코디 탐색하다가 온도 만족하면 코디 string 출력, RGB값 전송		//그렇지 않으면 continue
             //no 선택 시 반복문 반복			//yes 선택 시 프로그램 종료
             //i가 19가 되면 추천 코디 소진 알림
            } 
           else {
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
