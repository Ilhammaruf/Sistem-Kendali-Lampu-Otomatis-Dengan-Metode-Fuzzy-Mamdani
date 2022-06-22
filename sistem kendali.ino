#include "RBDdimmer.h"
dimmerLamp dimmer(outputPin);
#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = "K-9EcNkQ1yjpnJ67G0wkfJKilqIBvN0x";
char ssid[] = "ilham";                            // Hostpot Wifi Smartphone
char pass[] = "12345678";
// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(8, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);
BlynkTimer timer;
WidgetLED led1(V11);
WidgetLED led2(V12);
int slider, menu;
float InGelap[4] = {0,0,10,30};         // Fungsi Keanggotaan Input LDR Dalam
float InAgakG[3] = {10,30,50};
float InRemang[3] = {30,50,70};
float InTerang[3] = {50,70,90};
float InSangatT[4] = {70,90,100,100};

float OutGelap[4] = {0,0,30,40};         // Fungsi Keanggotaan Input LDR Luar
float OutMendung[3] = {30,50,70};
float OutCerah[4] = {60,70,100,100};

float oPadam[4] = {0,0,40,80};            // Fungsi Keanggotaan Output PWM
float oSangatR[3] = {40,80,120};
float oRedup[3] = {80,120,160};
float oAgakT[3] = {120,160,200};
float oTerang[4] = {160,200,255,255};

int konversi1, konversi2;       //sebagai konversi nilai 0-1023 menjadi 0-100
int dalam;                   //menampung nilai hasil konversi1
int luar;                   //menampung nilai hasil konversi1 
int lampu;                    //menampung nilai hasil perhitungan fuzzy
int sensor1,sensor2;            // menampung nilai sensor 1 dan 2
float rul1, rul2, rul3, rul4, rul5, rul6, rul7, rul8, rul9,rul10,rul11,rul12,rul13,rul14,rul15; // nilai setiap rull
float maxRull1,maxRull2,maxRull3,maxRull4,maxRull5,maxRull6,maxRull7,maxRull8;                   //nilai max rul
float MaxTerang, MaxPadam,MaxAgakT,MaxSangatR,MaxRedup;                                   
float a1, a2, a3, a4, a5, a6, a7, a8;                         //nilai daerah hasil komposisi
float Z1, Z2, Z3, Z4, Z5, Z6, Z7,Z8,Z9,Z10,Z11,Z12,Z13,o,p;   //nilai luas daerah komposisi
float luas1,luas2,luas3;    
float M1, M2, M3, M4, M5, M6, M7,M8, M9, M10,M11,M12,M13;     //nilai moment 
float hasil; 
float uInGelap, uInAgakG,uInRemang,uInTerang,uInSangatT, uOutGelap, uOutMendung, uOutCerah;
int hasil1;// nilai hasil konversi ke persentase
BLYNK_WRITE(V5){
  menu = param.asInt();
  Serial.print(menu);
}
BLYNK_WRITE(V6){
  slider = param.asInt();
  Serial.print(slider);
}
void myTimerEvent()
{
  
  sensor();
 
  Blynk.virtualWrite(V0, dalam);
  Blynk.virtualWrite(V1, luar);
  
    if (dalam<20){
    Blynk.virtualWrite(V2, "GELAP");
  }else if (dalam>20 && dalam <40){
     Blynk.virtualWrite(V2, "AGAK GELAP");
  }else if (dalam>40 && dalam <60){
     Blynk.virtualWrite(V2, "REMANG");
  }else if (dalam>60 && dalam <80){
     Blynk.virtualWrite(V2, "TERANG");
  }else if (dalam>80){
     Blynk.virtualWrite(V2, "SANGAT TERANG");
  }
   if (luar<30){
    Blynk.virtualWrite(V3, "GELAP");
  }else if (luar>30 && luar <60){
     Blynk.virtualWrite(V3, "MENDUNG");
  }else if (luar>60){
     Blynk.virtualWrite(V3, "CERAH");
  }
    
    if (menu == 1){
     led1.on();
     led2.off();
     Blynk.virtualWrite(V4, slider);
     dimmer.setPower(slider); // setPower(0-100%);
 }else if(menu == 2){
  
  led2.on();
  led1.off();
   Blynk.virtualWrite(V4, hasil1);
    Blynk.virtualWrite(V7, lampu);
    dimmer.setPower(lampu); // setPower(0-100%);
 } 
}
void setup()
{
  // Debug console
  Serial.begin(9600);

  delay(10);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth,wifi, ssid, pass, "blynk.iot-cm.com",8080);
  timer.setInterval(1000L, myTimerEvent);

}

void loop()
{
   
  Blynk.run();
    timer.run();

}
void sensor(){
  sensor1 = analogRead(A0);   //ambil data dari pin A0
  sensor2 = analogRead(A1);   //ambil data dari pin A1
  konversi1=sensor1/10.23;              //konversi sensor1 dari 1023-0 -> 100-0
  konversi2=sensor2/10.23;              //konversi sensor2 dari 1023-0 -> 100-0
  dalam=map(konversi1,0,100,100,0);     //konversi menjadi 0-100
  luar=map(konversi2,0,100,100,0);      //konversi menjadi 0-100


if (dalam < InGelap[2] ) {uInGelap = 1;}
else if (dalam >= InGelap[2] && dalam <= InGelap[3]){ uInGelap = (InGelap[3] - dalam) / (InGelap[3]-InGelap[2]);}
else if (dalam > InGelap[3]) {uInGelap = 0;}

if (dalam < InAgakG[0] ) {uInAgakG = 0;}
else if (dalam >= InAgakG[0] && dalam <= InAgakG[1]){ uInAgakG = (dalam - InAgakG[0]) / (InAgakG[1] - InAgakG[0]);}
else if (dalam >= InAgakG[1] && dalam <= InAgakG[2]){ uInAgakG = (InAgakG[2] - dalam) / (InAgakG[2] - InAgakG[1]);}
else if (dalam > InAgakG[2]) {uInAgakG = 0;}

if (dalam < InRemang[0] ) {uInRemang = 0;}
else if (dalam >= InRemang[0] && dalam <= InRemang[1]){ uInRemang = (dalam - InRemang[0]) / (InRemang[1]- InRemang[0]);}
else if (dalam >= InRemang[1] && dalam <= InRemang[2]){ uInRemang = (InRemang[2] - dalam) / (InRemang[2]- InRemang[1]);}
else if (dalam > InRemang[2]) {uInRemang = 0;}

if (dalam < InTerang[0] ) {uInTerang = 0;}
else if (dalam >= InTerang[0] && dalam <= InTerang[1]){ uInTerang = (dalam - InTerang[0]) / (InTerang[1] - InTerang[0]);}
else if (dalam >= InTerang[1] && dalam <= InTerang[2]){ uInTerang = (InTerang[2] - dalam) / (InTerang[2] - InTerang[1]);}
else if (dalam > InTerang[2]) {uInTerang = 0;}

if (dalam < InSangatT[0] ) {uInSangatT = 0;}
else if (dalam >= InSangatT[0] && dalam <= InSangatT[1]){ uInSangatT = (dalam - InSangatT[0]) / (InSangatT[1] - InSangatT[0]) ;}
else if (dalam > InSangatT[1]) {uInSangatT = 1;}

if (luar < OutGelap[2] ) {uOutGelap = 1;}
else if (luar >= OutGelap[2] && luar <= OutGelap[3]){ uOutGelap = (OutGelap[3] - luar) / (OutGelap[3]-OutGelap[2]);}
else if (luar > OutGelap[3]) {uOutGelap = 0;}

if (luar < OutMendung[0] ) {uOutMendung = 0;}
else if (luar >= OutMendung[0] && luar <= OutMendung[1]){ uOutMendung = (luar - OutMendung[0]) / (OutMendung[1]- OutMendung[0]);}
else if (luar >= OutMendung[1] && luar <= OutMendung[2]){ uOutMendung = (OutMendung[2] - luar) / (OutMendung[2] -OutMendung[1]);}
else if (luar > OutMendung[2]) {uOutMendung = 0;}

if (luar < OutCerah[0] ) {uOutCerah = 0;}
else if (luar >= OutCerah[0] && luar <= OutCerah[1]){ uOutCerah = (luar - OutCerah[0]) / (OutCerah[1]-OutCerah[0]);}
else if (luar > OutCerah[1]) {uOutCerah = 1;}


//-----------------------Aplikasi Fungsi Implikasi------------------------------

//RULL 1 = If dalam gelap and luar GELAP then lampu TERANG
rul1 = min(uInGelap, uOutGelap);
//RULL 2 = If dalam gelap and luar MENDUNG then lampu AGAK TERANG
rul2 = min(uInGelap, uOutMendung);
//RULL 3 = If dalam gelap and luar CERAH then lampu REDUP
rul3 = min(uInGelap, uOutCerah);
//RULL 4 = If dalam AGAK GELAP and luar GELAP then lampu AGAK TERANG
rul4 = min(uInAgakG, uOutGelap);
//RULL 5 = If dalam AGAK GELAP and luar MENDUNG then lampu REDUP
rul5 = min(uInAgakG, uOutMendung);
//RULL 6 = If dalam AGAK GELAP and luar CERAH then lampu SANGAT REDUP
rul6 = min(uInAgakG, uOutCerah);
//RULL 7 = If dalam REMANG and luar GELAP then lampu AGAK TERANG
rul7 = min(uInRemang, uOutGelap);
//RULL 8 = If dalam REMANG and luar MENDUNG then lampu REDUP
rul8 = min(uInRemang, uOutMendung);
//RULL 9 = If dalam REMANG and luar CERAH then lampu SANGAT REDUP
rul9 = min(uInRemang, uOutCerah);
//RULL 10 = If dalam TERANG and luar GELAP then lampu AGAK TERANG
rul10 = min(uInTerang, uOutGelap);
//RULL 11 = If dalam TERANG and luar MENDUNG then lampu REDUP
rul11 = min(uInTerang, uOutMendung);
//RULL 12 = If dalam TERANG and luar CERAH then lampu SANGAT REDUP
rul12 = min(uInTerang, uOutCerah);
//RULL 13 = If dalam SANGAT TERANG and luar GELAP then lampu REDUP
rul13 = min(uInSangatT, uOutGelap);
//RULL 14 = If dalam SANGAT TERANG and luar MENDUNG then lampu SANGAT REDUP
rul14 = min(uInSangatT, uOutMendung);
//RULL 15 = If dalam SANGAT TERANG and luar CERAH then lampu PADAM
rul15 = min(uInSangatT, uOutCerah);




//padam
MaxPadam = rul15;
//Sangat Redup
maxRull1=max(rul6,rul9);
maxRull2=max(rul12,rul14);
MaxSangatR = max(maxRull1, maxRull2);
//Redup
maxRull4=max(rul3,rul5);
maxRull5=max(rul8,rul11);
maxRull6=max(rul13,maxRull4);
MaxRedup = max(maxRull5, maxRull6);
//Agak Terang
maxRull7=max(rul2,rul4);
maxRull8=max(rul7,rul10);
MaxAgakT = max(maxRull7, maxRull8);
//Terang
MaxTerang=rul1;


//--------------------Komposisi Aturan-------------------------

a1=oPadam[3] - (MaxPadam * (oPadam[3]-oPadam[2]));      //40<a<80
a2=oSangatR[0] + (MaxSangatR * (oSangatR[1]-oSangatR[0]));      //40<a<80
a3=oSangatR[2] - (MaxSangatR * (oSangatR[2]-oSangatR[1]));     //80<a<120
a4=oRedup[0] + (MaxRedup * (oRedup[1]-oRedup[0]));      //40<a<80
a5=oRedup[2] - (MaxRedup * (oRedup[2]-oRedup[1]));     //80<a<120
a6=oAgakT[0] + (MaxAgakT * (oAgakT[1]-oAgakT[0]));      //40<a<80
a7=oAgakT[2] - (MaxAgakT * (oAgakT[2]-oAgakT[1]));     //80<a<120
a8=oTerang[0] + (MaxTerang * (oTerang[1]-oTerang[0]));      //40<a<80



Z1=((a1-oPadam[2])*MaxPadam)/2;
Z2=((a1-oPadam[0])*MaxPadam);
Z3=((a2-oSangatR[0])*MaxSangatR)/2;
Z4=(a3-a2)*MaxSangatR;
Z5=((oSangatR[2]-a3)*MaxSangatR)/2;
Z6=((a4-oRedup[0])*MaxRedup)/2;
Z7=(a5-a4)*MaxRedup;
Z8=((oRedup[2]-a5)*MaxRedup)/2;
Z9=((a6-oAgakT[0])*MaxAgakT)/2;
Z10=(a7-a6)*MaxAgakT;
Z11=((oAgakT[2]-a7)*MaxAgakT)/2;
Z12=((a8-oTerang[0])*MaxTerang)/2;
Z13=((oTerang[3]-a8)*MaxTerang);


//------------------Penegasan (Defuzzy)-----------------------------------------

//(MaxPadam) ==================> x<a1 ==================> PADAM
M1 = (MaxPadam*(pow(a1,2)/2));
//(80-x/40) ==================> a1<x<80
M2 = (1/(oPadam[3]-oPadam[2]))*((oPadam[3]*(pow(oPadam[3],2)-pow(a1,2))/2) - ((pow(oPadam[3],3)-pow(a1,3))/3));
//x-40/40 ==================> 40<x<a2   ==================>SANGAT REDUP
M3 = (1/(oSangatR[2]-oSangatR[1])) * (((pow(a2,3)-pow(oSangatR[0],3))/3) - (oSangatR[0]*((pow(a2,2)-pow(oSangatR[0],2))/2)));
//SangatR   ==================>   a2<x<a3
M4 = (MaxSangatR*((pow(a3,2)-pow(a2,2))/2));
//120-x/40  ==================>   a3<x<120
M5 = (1/(oSangatR[2]-oSangatR[1]))*((oSangatR[2]*(pow(oSangatR[2],2)-pow(a3,2))/2) - ((pow(oSangatR[2],3)-pow(a3,3))/3));
//(x-80)/(40)   ==================> 80<x<a4 ==================>REDUP
M6 = (1/(oRedup[2]-oRedup[1])) * (((pow(a4,3)-pow(oRedup[0],3))/3) - (oRedup[0]*((pow(a4,2)-pow(oRedup[0],2))/2)));
//Redup     a4<x<a5
M7 = (MaxRedup*((pow(a5,2)-pow(a4,2))/2));
//(160-x)/(40)   ==================>   a5<x<160
M8 = (1/(oRedup[2]-oRedup[1]))*((oRedup[2]*(pow(oRedup[2],2)-pow(a5,2))/2) - ((pow(oRedup[2],3)-pow(a5,3))/3));
//(x-120)/(40)   ==================>   120<x<a6   ==================>     AGAK TERANG
M9 = (1/(oAgakT[2]-oAgakT[1])) * (((pow(a6,3)-pow(oAgakT[0],3))/3) - (oAgakT[0]*((pow(a6,2)-pow(oAgakT[0],2))/2)));
//AgakT   ==================>  a6<x<a7
M10 = (MaxAgakT*((pow(a7,2)-pow(a6,2))/2));
//(200-x)/(40) ==================> a7<x<200
M11 = (1/(oAgakT[2]-oAgakT[1]))*((oAgakT[2]*(pow(oAgakT[2],2)-pow(a7,2))/2) - ((pow(oAgakT[2],3)-pow(a7,3))/3));
//(x-160)/(40)  ==================>    160<x<a8  ==================>  TERANG
M12 = (1/(oTerang[2]-oTerang[1])) * (((pow(a8,3)-pow(oTerang[0],3))/3) - (oTerang[0]*((pow(a8,2)-pow(oTerang[0],2))/2)));
//Terang   ==================> x>a8
M13 = (MaxTerang*(pow(a8,2)/2));
 


//-----------------Hasil Akhir (Titik Pusat)-----------------------

float hasil=(M1+M2+M3+M4+M5+M6+M7+M8+M9+M10+M11+M12+M13)/(Z1+Z2+Z3+Z4+Z5+Z6+Z7+Z8+Z9+Z10+Z11+Z12+Z13);
lampu=ceil(hasil);
hasil1 = map(lampu, 47,302 , 0, 100);
}
