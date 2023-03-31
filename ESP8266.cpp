#include <SD.h>
#include "web.h"
#include <SPI.h>
#include "DHT.h"
#include <SoftwareSerial.h>
const int chipSelect = D8; // use D0 for Wemos D1 Mini
File root;
#define chandht D1
#define loaidht DHT11
#define thietbi1 D4
#define thietbi2 D3
DHT dht(chandht,loaidht);
const byte RX = D6;
const byte TX = D5;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
const char *ssid = "NVT";
const char *pass = "123456789";
IPAddress local_IP(192, 168, 17, 120);
IPAddress gateway(192, 168, 17, 110);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);
void ConnectWiFi();
String inputString = "";
String inputString1 = "";
String vitrichay1 = "";
String vitrichay2 = "";
String vitrichay3 = "";
bool stringComplete = false;
long last = 0;
void Read_UART();
void Read_UART1();
String ChuoiSendWebJson ="";
float nhietdo;
float doam;
String Jdata[3][20];
String Jid[20];
String Jtm[20];
String Jlate[20];
String at = "";
String id = "";
String datefile = "";
String late = "";
String crdate = "";
String CRT = "";
String crtm = "";
String door = "";
int tttb1, tttb2;
void setup() {
  Serial.begin(115200);
  Serial.print("\r\nWaiting for SD card to initialise...");
  if (!SD.begin(chipSelect)) { // CS is D8 in this example
    Serial.println("Initialising failed!");
    return;
  }
  Serial.println("Initialisation completed");

  while(!Serial);
  last = millis();
  pinMode(chandht,INPUT);
  pinMode(thietbi1,OUTPUT);
  pinMode(thietbi2,OUTPUT);
  digitalWrite(thietbi1,LOW);
  digitalWrite(thietbi2,LOW);
  dht.begin();
  ConnectWiFi(); 
 server.on("/", [] {
    SendWebPage();
  });
  server.on("/Update", [] {  //=======Khi on duong dan Update len thi ham duoi se chay
    SendData();
  });
  server.on("/Update2", [] {  //=======Khi on duong dan Update len thi ham duoi se chay
    SendData2();
  });
    server.on("/Input", [] {  //=======Khi on duong dan input len thi ham duoi se chay
    Read_SDcard();
  });
   server.on("/Updatecrtm", [] {  //=======Khi on duong dan input len thi ham duoi se chay
    Send_Inputcrtm();
  }); 
  server.begin();
} 
void loop() { 
 server.handleClient();
 Read_UART();
}
void Send_Input1() 
{
  String input = "";
  input = server.arg("Input"); 
  Serial.println(input);
  Serial.flush();
}
void Read_UART()
{
   while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
    
    if (inChar == '\n')
    {
      stringComplete = true;
    }
    if (stringComplete == true)
    {
        if(inputString.indexOf("DOOR") >=0 )
      {
       TachDuLieu();  
      SendCrttime(); 
      }
        if(inputString.indexOf("DATE") >=0 )
      {
       TachDuLieu();
       WriteSD();
      }
      if(inputString.indexOf("Khu") >=0 )
      {
       Baochay();
      }
      inputString = "";
      stringComplete = false;
    }
  }
}
void Baochay()
{
  int TimKhuB, TimKhuA;
  TimKhuA = inputString.indexOf("Khu A");
  TimKhuB = inputString.indexOf("Khu B");
  if(TimKhuA >= 0 && TimKhuB >= 0)
  {
    vitrichay3 = "Khu A va B";
  }
  else if (TimKhuA >=0)
  {
    vitrichay1 = "Khu B";
  }
  else if (TimKhuB >=0)
  {
    vitrichay2 = "Khu A";
  }
}

void Read_SDcard()
{
  inputString = server.arg("Input");  
  Serial.println(inputString);
  Serial.flush();
  uint8_t   j, m, n, k;
  int i = 0;
  j = 0;
  m=0;
   for(i = 0; i<3; i++)
      {
      for(j = 0; j<20; j++)
        {
      Jdata[i][j] = "";
        }
      } 
  
  //Serial.println(inputString);
//  Serial.flush();
  TachDuLieu();  
  inputString = "";
//  Serial.print("datefile:");
//  Serial.println(datefile);
  root = SD.open("/");
  root.rewindDirectory();
//  printDirectory(root, 0); //Display the card contents
  root.close();
  Serial.flush(); Serial.begin(115200);

  String s = "";
  String file = datefile + ".txt";
  if (SD.exists(file)) {
      File testfile = SD.open(file, FILE_WRITE);
      uint8_t maxfile = testfile.position();
      testfile.close();
      testfile = SD.open(file, FILE_READ);
      i = 0;
      j = 0;
      for(m = 0; m<maxfile ; i++) {
         
          n = m+2;
          k = m+8; // data sample:  123,12:54,a            
         
         for(j = m; j <= n; j++){     //===========
             testfile.seek(j);
             char dataout = testfile.read();
             s = s + String(dataout); 
         }
         Jid[i] = s;
         s = ""; 
         for(j = n+2; j <= k; j++){     //===========
             testfile.seek(j);
             char dataout = testfile.read();
             s = s + String(dataout);
         }
         Jtm[i] = s;
         s = "";
           j=k+2;     //===========
           testfile.seek(j);
           char dataout = testfile.read();
           s = s + String(dataout);
         
         Jlate[i] = s;
         s = "";
         m = 12+m;
      }
      testfile.close();
      Serial.flush();
      for(j = 0; j < 20; j++){
        Jdata[0][j] = Jid[j];
      }
      for(j = 0; j < 20; j++){
        Jdata[1][j] = Jtm[j];
      }
      for(j = 0; j < 20; j++){
        Jdata[2][j] = Jlate[j];
      }
      for(j = 0; j<20; j++)
        {
          Jid[j] = "";
          Jtm[j] = "";
          Jlate[j] = "";
        }
      datefile = "";
      file = "";
      }
  else {
    Jdata[0][0] = "NO DATA";
  }
}

void WriteSD() //=======================Write data to SD card==================================
{ 
    root = SD.open("/");
  root.rewindDirectory();
//  printDirectory(root, 0); //Display the card contents
  root.close();
  String file = datefile + ".txt"; //12-02-2022.txt
//  Serial.print("filename:");
//  Serial.println(file);
  File testfile = SD.open(file, FILE_WRITE); // search for file named "file", if can't see any, creat a new. or else go to the end of the file
  if (testfile) {
        testfile.print(id+","+at+","+late+"\n");
  }
  testfile.close();
  Serial.flush();
  datefile = "";
  file = "";
  //-------------------------------------------------------------------------------
}

void Send_Inputcrtm()
{

  nhietdo = dht.readTemperature();
  doam  = dht.readHumidity();
  trangthaithietbi();
  Serial.println("CRT");
  Serial.flush();
  Read_UART();
  SendCrttime();
}
void ConnectWiFi() {
  WiFi.disconnect(); delay(1000);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
      Serial.println("Config ko dc");
    }
  delay(1000); WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Da ket noi, IP address la: ");
  Serial.println(WiFi.localIP());
  
}
void SendCrttime()
{   
      CRT = "{\"crtm\":\"" + String(crtm) + "\"," +
            "\"door\":\"" + String(door) + "\"," +
            "\"ND\":\"" + String(nhietdo) + "\"," +
            "\"DA\":\"" + String(doam) + "\"," +
            "\"tb1\":\"" + String(tttb1) + "\"," +
            "\"tb2\":\"" + String(tttb2) + "\"," +
            "\"bc1\":\"" + String(vitrichay1) + "\"," +
            "\"bc2\":\"" + String(vitrichay2) + "\"," +
            "\"bc3\":\"" + String(vitrichay3) + "\"," +
            "\"crdate\":\"" + String(crdate) + "\"}" ;  //JSON FAKE           
      server.send(200, "text/html", String(CRT)); 
      if(vitrichay1 != ""||vitrichay2 != ""||vitrichay3 != "")
      {
        vitrichay1 = "";
        vitrichay2 = "";
        vitrichay3 = "";
      }
}
void SendWebPage()
{
  server.send(200, "text/html", webpage);
}

  void TachDuLieu()
{ 
  int TimCT, TimTIM, TimDOOR, TimTA, TimBY, TimID, TimDATE,
      TimHS, TimSH, TimOT, TimCRD;
  TimCT = inputString.indexOf("CT");
  TimCRD = inputString.indexOf("CRD");
  TimTIM = inputString.indexOf("TIM");
  TimDOOR = inputString.indexOf("DOOR");
  TimTA = inputString.indexOf("TA");
  TimBY = inputString.indexOf("BY");
  TimID = inputString.indexOf("ID");
  TimDATE = inputString.indexOf("DATE");
  TimHS = inputString.indexOf("HS");
  TimSH = inputString.indexOf("SH");
  TimOT = inputString.indexOf("OT");
    if(TimDATE >= 0 && TimOT >= 0)
  {
    late = inputString.substring(TimDATE + 4, TimOT);
  }
    if(TimHS >= 0 && TimSH >= 0)
  {
    datefile = inputString.substring(TimSH + 2, TimHS);
  }
    if(TimCT >= 0 && TimTIM >= 0)
  {
    crtm = inputString.substring(TimCT + 2, TimTIM);
  }
  
  if(TimTIM >= 0 && TimDOOR >= 0)
  {
    door = inputString.substring(TimTIM + 3, TimDOOR);
  }
  if(TimDOOR >= 0 && TimCRD >= 0)
  {
    crdate = inputString.substring(TimDOOR + 4, TimCRD);
  }

  if(TimTA >= 0 && TimBY >= 0)
  {
    at = inputString.substring(TimTA + 2, TimBY);
  }
  if(TimBY >= 0 && TimID >= 0)
  {
    id = inputString.substring(TimBY + 2, TimID);
  }
  if(TimID >= 0 && TimDATE >= 0)
  {
    datefile = inputString.substring(TimID + 2, TimDATE);
  }

}

void DataJson( String Jdata[3][20])
  {
    ChuoiSendWebJson = "{\"i1\":\"" + Jdata[0][0] + "\"," +
                       "\"i2\":\"" + Jdata[0][1] + "\"," +
                       "\"i3\":\"" + Jdata[0][2] + "\"," +
                       "\"i4\":\"" + Jdata[0][3] + "\"," +
                       "\"i5\":\"" + Jdata[0][4] + "\"," +
                       "\"i6\":\"" + Jdata[0][5] + "\"," +
                       "\"i7\":\"" + Jdata[0][6] + "\"," +
                       "\"i8\":\"" + Jdata[0][7] + "\"," +
                       "\"i9\":\"" + Jdata[0][8] + "\"," +
                       "\"i10\":\"" + Jdata[0][9] + "\"," +
                       
                       "\"t1\":\"" + Jdata[1][0] + "\"," +
                       "\"t2\":\"" + Jdata[1][1] + "\"," +
                       "\"t3\":\"" + Jdata[1][2] + "\"," +
                       "\"t4\":\"" + Jdata[1][3] + "\"," +
                       "\"t5\":\"" + Jdata[1][4] + "\"," +
                       "\"t6\":\"" + Jdata[1][5] + "\"," +
                       "\"t7\":\"" + Jdata[1][6] + "\"," +
                       "\"t8\":\"" + Jdata[1][7] + "\"," +
                       "\"t9\":\"" + Jdata[1][8] + "\"," +
                       "\"t10\":\"" + Jdata[1][9] + "\"," +

                       "\"l1\":\"" + Jdata[2][0] + "\"," +
                       "\"l2\":\"" + Jdata[2][1] + "\"," +
                       "\"l3\":\"" + Jdata[2][2] + "\"," +
                       "\"l4\":\"" + Jdata[2][3] + "\"," +
                       "\"l5\":\"" + Jdata[2][4] + "\"," +
                       "\"l6\":\"" + Jdata[2][5] + "\"," +
                       "\"l7\":\"" + Jdata[2][6] + "\"," +
                       "\"l8\":\"" + Jdata[2][7] + "\"," +
                       "\"l9\":\"" + Jdata[2][8] + "\"," +
                       "\"l10\":\"" + Jdata[2][9] + "\"}" ; 
                                                  
 }

void DataJson2( String Jdata[3][20])
  {
    ChuoiSendWebJson = "{\"d1\":\"" + Jdata[0][10] + "\"," +
                       "\"d2\":\"" + Jdata[0][11] + "\"," +
                       "\"d3\":\"" + Jdata[0][12] + "\"," +
                       "\"d4\":\"" + Jdata[0][13] + "\"," +
                       "\"d5\":\"" + Jdata[0][14] + "\"," +
                       "\"d6\":\"" + Jdata[0][15] + "\"," +
                       "\"d7\":\"" + Jdata[0][16] + "\"," +
                       "\"d8\":\"" + Jdata[0][17] + "\"," +
                       "\"d9\":\"" + Jdata[0][18] + "\"," +
                       "\"d10\":\"" + Jdata[0][19] + "\"," +
                       
                       "\"m1\":\"" + Jdata[1][10] + "\"," +
                       "\"m2\":\"" + Jdata[1][11] + "\"," +
                       "\"m3\":\"" + Jdata[1][12] + "\"," +
                       "\"m4\":\"" + Jdata[1][13] + "\"," +
                       "\"m5\":\"" + Jdata[1][14] + "\"," +
                       "\"m6\":\"" + Jdata[1][15] + "\"," +
                       "\"m7\":\"" + Jdata[1][16] + "\"," +
                       "\"m8\":\"" + Jdata[1][17] + "\"," +
                       "\"m9\":\"" + Jdata[1][18] + "\"," +
                       "\"m10\":\"" + Jdata[1][19] + "\"," +

                       "\"e1\":\"" + Jdata[2][10] + "\"," +
                       "\"e2\":\"" + Jdata[2][11] + "\"," +
                       "\"e3\":\"" + Jdata[2][12] + "\"," +
                       "\"e4\":\"" + Jdata[2][13] + "\"," +
                       "\"e5\":\"" + Jdata[2][14] + "\"," +
                       "\"e6\":\"" + Jdata[2][15] + "\"," +
                       "\"e7\":\"" + Jdata[2][16] + "\"," +
                       "\"e8\":\"" + Jdata[2][17] + "\"," +
                       "\"e9\":\"" + Jdata[2][18] + "\"," +
                       "\"e10\":\"" + Jdata[2][19] + "\"}" ;   
   
 }
  void SendData()
{
  DataJson(Jdata);

   server.send(200, "text/html", String(ChuoiSendWebJson));
   int i = 0;
    int j = 0;
    for(i = 0; i<3; i++)
      {
      for(j = 0; j<10; j++)
        {
      Jdata[i][j] = "";
        }
      } 
}
  void SendData2()
{
  DataJson2(Jdata);
//  Serial.println("Chuoi Json:" + ChuoiSendWebJson);
   server.send(200, "text/html", String(ChuoiSendWebJson));
    int i = 0;
    int j = 0;
    for(i = 0; i<3; i++)
      {
      for(j = 10; j<20; j++)
        {
      Jdata[i][j] = "";
        }
      }
}
void trangthaithietbi(){
{
  if(doam > 85)
  {
    tttb1 = 1;
    digitalWrite(thietbi1, HIGH);
  }
  else{
    tttb1 = 0;
    digitalWrite(thietbi1, LOW);
    }
}
 {
  if(nhietdo >=32)
  {
    tttb2 = 1;
    digitalWrite(thietbi2, HIGH);
  }
  else{
    tttb2 = 0;
    digitalWrite(thietbi2, LOW);
    }
}
}