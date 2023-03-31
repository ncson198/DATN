#include <HardwareSerial.h>
HardwareSerial sim(PA3, PA2);
HardwareSerial Serial3(PB11, PB10);
int address=0;
byte value, q, w;
int _timeout;
String _buffer;
String number = "+84905587784";
//=============================================
#include <stm32f1_rtc.h>
#define FLAGS_REGISTER  1
/* Bit 0 used for internal RTC init flag */
#define HIJRI_MODE_FLAG  (1 << HIJRI_MODE_BIT)
#define HOUR_12_FLAG     (1 << HOUR_12_BIT)
#define ALARM_ENTRY_FLAG (1 << ALARM_ENTRY_BIT)
DateVar date;
TimeVar time;
STM32F1_RTC rtc;
//=============================================
String inputStringtime = "";        
bool stringCompletetime = false; 

String current = "";
String TimeAttendance = "";
uint32_t epochTime;
uint32_t unix = 1;
uint32_t Unix1 = 0;
uint32_t Unix2 = 0;
uint32_t UnixHold;
String data = "";
String data2 = "";
String bh = "";
String bm = "";
String bs = "";
String bd = "";
String by = "";
String bmonth = "";
String overtime = "";
int doorState = 0;
String vitrichay1 = "";
String vitrichay2 = "";
String vitrichay = "";
uint8_t id;// so te cua vantay co khoang 217 mau
#include <Adafruit_Fingerprint.h>
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27 , 16, 2);
#include <Keypad.h>
#include<EEPROM.h>
char cancle[1];
char password[4];
char pass[4], pass1[4], newpass1[4], newpass2[4];
int i = 0;
int lock = PA1;
int alert1 = PA6;
int detect1 = PA4;
int alert2 = PA5;
int detect2 = PB0;
int tittit = PA8;
int nut1 = PB9;
int nut2 = PA0;
int du = PA7;
int detectp = 0;
int detectn = 0;
char customKey = 0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {PB8, PB13, PB12, PB5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {PB4, PB3, PA15, PA12};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
#include <Adafruit_Fingerprint.h>
int h = 0;
int m = 5; // neu mo khoa sai m lan thi se khong duoc mo khoa nua.
int readepoch;
int pointepoch;
int pointAddr;
void setup()
{
 pinMode(lock, OUTPUT);
pinMode(alert1, OUTPUT);
pinMode(detect1, INPUT);
pinMode(alert2, OUTPUT);
pinMode(detect2, INPUT);
pinMode(tittit, OUTPUT);
pinMode(nut1, INPUT);
pinMode(nut2, INPUT);
pinMode(du, OUTPUT);

//==========================================================
lcd.begin();
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("DO AN TOT NGHIEP");
lcd.setCursor(0, 1);
lcd.print(" THANG 1/2023");
delay(2000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("NGUYEN CONG SON");
lcd.setCursor(0, 1);
lcd.print("NGUYEN VAN THANG");
delay(2000);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("NHAP MAT KHAU ");
  lcd.setCursor(1, 1);
  lcd.print("HOAC VAN TAY ");
  lcd.setCursor(8, 2);
//==========================================================
   rtc.begin(); // initialize RTC 24H format
   epochTime = rtc.getTime();
   rtc.epochToDate(epochTime, date);
   rtc.epochToTime(epochTime, time);
//==========================================================
  for (int j = 0; j < 4; j++)
    pass[j] = EEPROM.read(j);
  id = EEPROM.read(118);
 //==========================================================
  cancle[1] = 0;
  Serial1.begin(115200);
  sim.begin(9600);
  _buffer.reserve(50);
  Serial3.begin(115200);
//==========================================================
  while (!Serial1);
 // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial1.println("\n\nAdafruit finger detect test");
 finger.begin(57600);
//   set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial1.println("Found fingerprint sensor!");
  } else {
    Serial1.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  Serial1.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial1.print(F("Status: 0x")); Serial1.println(finger.status_reg, HEX);
  Serial1.print(F("Sys ID: 0x")); Serial1.println(finger.system_id, HEX);
  Serial1.print(F("Capacity: ")); Serial1.println(finger.capacity);
  Serial1.print(F("Security level: ")); Serial1.println(finger.security_level);
  Serial1.print(F("Device address: ")); Serial1.println(finger.device_addr, HEX);
  Serial1.print(F("Packet len: ")); Serial1.println(finger.packet_len);
  Serial1.print(F("Baud rate: ")); Serial1.println(finger.baud_rate);
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    Serial1.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial1.println("Waiting for valid finger...");
    Serial1.print("Sensor contains "); Serial1.print(finger.templateCount); Serial1.println(" templates");
  }
   while(!Serial3); 
}
//==========================================================
 uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial1.available());
    num = Serial1.parseInt();
  }
  return num;
}
void loop()  
{
    if (rtc.isCounterUpdated())//---------------Get time
    {
       rtc.clearSecondFlag();
       epochTime = rtc.getTime();
    } 
  Read_UART_8266() ;
  baochay();
  baochaynut();
 getFingerprintID();
 customKey = customKeypad.getKey();
if(customKey)
{
  digitalWrite(tittit, HIGH);
  delay(100);
  digitalWrite(tittit, LOW);
}
  if (customKey == 'A') {
    
    themdauvantay(); 
  }
  if (customKey == 'B') {
      doimatkhau();
    }   
  if (customKey == 'D') {
     xoavantay();
  }    
  if (customKey)
    {
     checkPass();
  }  
}
void checkPass()
{
  int j = 0;
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("NHAP MAT KHAU ");
      lcd.setCursor(8, 1);
      pass1[0] = customKey;
      lcd.print(customKey);
      j++;
  while (j < 4)
  {
    char key = customKeypad.getKey();
    if (key)
    {
      digitalWrite(tittit, HIGH);
      delay(100);
      digitalWrite(tittit, LOW);
      pass1[j++] = key;
      lcd.setCursor(8, 1);
      lcd.print(key);
    }
    key = 0;
  }
  cancle[1]= 0;
  if ((strncmp(pass1, pass, 4)))
    {
        h = h + 1;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("MAT KHAU SAI!");
        lcd.setCursor(2, 1);
        lcd.print("HAY NHAP LAI");
        delay(1000);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("SO LAN SAI: ");
        lcd.print(h);
        if(h>=3) {
          digitalWrite(tittit, HIGH);    
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("DA NHAP SAI");
          lcd.setCursor(1, 1);
          lcd.print("QUA NHIEU LAN!");         
          delay(1500);
          digitalWrite(tittit, LOW);
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("TIEN HANH TAM");
          lcd.setCursor(1, 1);
          lcd.print("KHOA HE THONG!");
          delay(10000);
          h = 0;
        }
        delay(1000);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("NHAP MAT KHAU ");
        lcd.setCursor(1, 1);
        lcd.print("HOAC VAN TAY ");
        lcd.setCursor(8, 2);
        i = 0;
  }
  else
  {
        h = 0;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("MAT KHAU DUNG");
        delay(700);
        Opendoor();
  }
  customKey = 0;
}
void doimatkhau(){
 if(id == 0) 
  {
        change();
  }
  else {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("CAN XAC NHAN");
    lcd.setCursor(0, 1);
    lcd.print("VAN TAY MASTER");
    delay(1000);
//    uint32_t tam = rtc.getEpoch();
    while(cancle[1] != 'C')
      {             
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("VAN TAY MASTER:");
        lcd.setCursor(1, 1);
        lcd.print("NHAN C DE HUY");
        char key = customKeypad.getKey();
        if (key == 'C')
         { 
          digitalWrite(tittit, HIGH);
          delay(100);
          digitalWrite(tittit, LOW);      
          cancle[1] = 'C';
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("DA HUY! ");
          delay(700);
          }
        else {        
    getFingerprintIDMaster3();
    delay(50);
        }
  }
// }
    
    cancle[1] = 0;
    
  }
    customKey = 0;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NHAP MAT KHAU ");
    lcd.setCursor(1, 1);
    lcd.print("HOAC VAN TAY ");
} 
void change()
{
    int j = 0;
    h = 0;
    j = 0;
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("MAT KHAU MOI:");
    lcd.setCursor(6, 2);
    while (j < 4)
    {
      char key = customKeypad.getKey();
      if (key)
      {
        digitalWrite(tittit, HIGH);
        delay(100);
        digitalWrite(tittit, LOW);
        newpass1[j] = key;
        lcd.print(key);
        j++;
      }
      key = 0;
    }
     j = 0;
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("NHAP LAI LAN 2:");
    lcd.setCursor(6, 2);
    while (j < 4)
    {
      char key = customKeypad.getKey();
      if (key)
      {
        digitalWrite(tittit, HIGH);
        delay(100);
        digitalWrite(tittit, LOW);
        newpass2[j] = key;
        lcd.print(key); 
        j++;
      }
      key = 0;
    }    
    delay(500);   
  if ((strncmp(newpass1, newpass2, 4)))
  {  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BAN DA NHAP SAI!");
    lcd.setCursor(0, 1);
    lcd.print("HAY THU LAI SAU");
    delay(1000);

    }
    else {
      j = 0;
      while (j < 4)
    {
          pass[j] = newpass2[j];
          int key = pass[j];
          EEPROM.update(j, key);
           delay(100);
          j++;
        }  
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("LUU THANH CONG!");
    delay(1000);
  }
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("NHAP MAT KHAU ");
      lcd.setCursor(1, 1);
      lcd.print("HOAC VAN TAY ");
      lcd.setCursor(8, 2);
      customKey = 0;
      cancle[1] = 'C';
}
void xoavantay(){
  customKey = 0;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("CAN XAC NHAN");
  lcd.setCursor(0, 1);
  lcd.print("VAN TAY MASTER");
  delay(1000);
  while(cancle[1] != 'C')
  { 
    lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("VAN TAY MASTER:");
  lcd.setCursor(1, 1);
  lcd.print("NHAN C DE HUY");
    char key = customKeypad.getKey();
    if (key == 'C')
    {       
          digitalWrite(tittit, HIGH);
          delay(100);
          digitalWrite(tittit, LOW);
          cancle[1] = 'C';
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("DA HUY! ");
          delay(700);         
        }
        else{
    getFingerprintIDMaster2();
    delay(50);
        }
  }    
    cancle[1] = 0;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NHAP MAT KHAU ");
    lcd.setCursor(1, 1);
    lcd.print("HOAC VAN TAY ");
}
void deleteFingerID() 
{
  int tam = id;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("NHAP ID CAN XOA ");
  
//  uint8_t id = readnumber();
  lcd.setCursor(6, 2);
  int j = 0;
  while (j < 3)
      {
        char t, c, dv;
//        char newpass2[2];
        char key = customKeypad.getKey();
        if (key)
        {
          digitalWrite(tittit, HIGH);
          delay(100);
          digitalWrite(tittit, LOW);
          newpass2[j] = key;
          lcd.print(key);
//          EEPROM.update(j, key);
          j++;  
        }
        i = (newpass2[0]-48)*100 + (newpass2[1]-48)*10 + (newpass2[2]-48);
//         id = newpass2[0]-48
        key = 0;
      }
      delay(200);
  if (i < 2 || i > 255) {// ID #0 not allowed, try again!
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("ID SAI!");
      lcd.setCursor(0, 1);
      lcd.print("HAY THU LAI SAU");
      delay(1000);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("NHAP MAT KHAU ");
      lcd.setCursor(1, 1);
      lcd.print("HOAC VAN TAY ");//.....................................................................................
      lcd.setCursor(8, 2);
      customKey = 0;
  }
  else {
      id = i;
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("DANG XOA.");
      delay(250);
      lcd.setCursor(3, 0);
      lcd.print("DANG XOA..");
      delay(250);
      lcd.setCursor(3, 0);
      lcd.print("DANG XOA..");
      delay(250);
      deleteFingerprint(id);     
  }
  customKey = 0;
  id = tam;
}
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    Serial1.println("Deleted!");
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("ID: ");
    lcd.setCursor(7, 0);
    lcd.print(id);
    lcd.setCursor(12, 0);
    lcd.print("DA");
    lcd.setCursor(1, 1);
    lcd.print("XOA THANH CONG!");
    delay(1000);
     
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("NHAP MAT KHAU ");
      lcd.setCursor(1, 1);
      lcd.print("HOAC VAN TAY ");
      lcd.setCursor(8, 2);
      customKey = 0;          
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial1.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial1.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial1.println("Error writing to flash");
  } else {
    Serial1.print("Unknown error: 0x"); Serial1.println(p, HEX);
  }
  return p;
}
void themdauvantay() {//................................................................
 
  customKey = 0;
  
  if(id == 0) 
  {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DAT VAN TAY MOI");
        lcd.setCursor(1, 1);
        lcd.print("LEN CAM BIEN ");
        Serial1.println("Ready to enroll a fingerprint!");
        Serial1.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
        id = id + 1;
        
        Serial1.print("Enrolling ID #");
        Serial1.println(id);
        while (!  getFingerprintEnroll() );
  }
  else {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("CAN XAC NHAN");
    lcd.setCursor(0, 1);
    lcd.print("VAN TAY MASTER");
    delay(1000);
    cancle[1] = 0;
    while(cancle[1] != 'C')
      { 
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("VAN TAY MASTER:");
        lcd.setCursor(1, 1);
        lcd.print("NHAN C DE HUY");
        char key = customKeypad.getKey();
        if (key == 'C')
         { 
          digitalWrite(tittit, HIGH);
          delay(100);
          digitalWrite(tittit, LOW);      
          cancle[1] = 'C';
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("DA HUY! ");
          delay(700);
          
           }
        else{
    getFingerprintIDMaster();
    delay(50);
        }
  }
    
    cancle[1] = 0;
    
  }
    customKey = 0;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NHAP MAT KHAU ");
    lcd.setCursor(1, 1);
    lcd.print("HOAC VAN TAY ");
}

uint8_t getFingerprintEnroll() {
  
  customKey = customKeypad.getKey();
  if(customKey == 'C')
{
  digitalWrite(tittit, HIGH);
  delay(100);
  digitalWrite(tittit, LOW);
}
  int p = -1;
  Serial1.print("Waiting for valid finger to enroll as #"); Serial1.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial1.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial1.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial1.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial1.println("Imaging error");
        break;
      default:
        Serial1.println("Unknown error");
        break;
    }
  }
  p = finger.image2Tz(1);//-------------------------------------------------------- van tay dung
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial1.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial1.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial1.println("Could not find fingerprint features");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }
  Serial1.println("Remove finger");
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("NHAC NGON TAY");
  lcd.setCursor(1, 1);
  lcd.print("VA CHO 2 GIAY");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial1.print("ID "); Serial1.println(id);
  p = -1;
  Serial1.println("Place same finger again");
  digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("DAT NGON TAY");
  lcd.setCursor(2, 1);
  lcd.print("LAI LAN NUA ");


  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial1.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial1.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial1.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial1.println("Imaging error");
        break;
      default:
        Serial1.println("Unknown error");
        break;
    }
  }
  
  //OK SUCCESS
  
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial1.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial1.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial1.println("Could not find fingerprint features");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }
    // OK CONVERTED!
  
  Serial1.print("Creating model for #");  Serial1.println(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial1.println("Prints matched!");                   //============MATCHED=================//
    


    // ..............................................................................................CHAP NHAN CAM BIEN VAN TAY..........
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial1.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial1.println("Fingerprints did not match");// ...................nhap van tay khong khop...............
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("VAN TAY");
    lcd.setCursor(3, 1);
    lcd.print("KHONG KHOP! ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("VUI LONG");
    lcd.setCursor(2, 1);
    lcd.print("THU LAI SAU!");
    id = id -1 ;
    delay(1000);
    customKey = 0;
   
    return p;
  } else {
    Serial1.println("Unknown error");
    return p;
  }
  Serial1.print("ID "); Serial1.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    
    Serial1.println("Stored!");
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("VAN TAY DA");
    lcd.setCursor(1, 1);
    lcd.print("LUU THANH CONG!");
    EEPROM.update(118, id);
    delay(1000);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("MA ID LA: ");
    lcd.setCursor(7, 1);
    lcd.print(id);
    if(id == 1) {
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.print("ID MASTER");
          lcd.setCursor(1, 1);
          lcd.print("DA DUOC LUU!");
          delay(1000);
    }
    delay(1000);
    cancle[1] = 'C';
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial1.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial1.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial1.println("Error writing to flash");
    return p;
  } else {
    Serial1.println("Unknown error");
    return p;
  }

  return true;

}


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial1.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial1.println("Imaging error");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial1.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial1.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial1.println("Could not find fingerprint features");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial1.println("Found a print match!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial1.println("Communication error");
    return p;
  }
  
    else if (p == FINGERPRINT_NOTFOUND) {
    Serial1.println("Did not find a match");
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("VAN TAY SAI ");
    lcd.setCursor(2, 1);
    lcd.print("HAY THU LAI ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NHAP MAT KHAU ");
    lcd.setCursor(1, 1);
    lcd.print("HOAC VAN TAY ");
    
    return p;    
  } else {
    Serial1.println("Unknown error");
    return p;
  }

  // found a match!
  Serial1.print("Found ID #"); Serial1.print(finger.fingerID);  //--------------------------------------
  Serial1.print(" with confidence of "); Serial1.println(finger.confidence);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MUNG BAN VE NHA!");
    digitalWrite(lock, HIGH);
    SendSDcard();
    Opendoor();
  
  return finger.fingerID;
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

 //--------- --------- --------- --------- --------- --------- ---------//FOUND A MATCH
  Serial1.print("Found ID #"); Serial1.print(finger.fingerID);
  Serial1.print(" with confidence of "); Serial1.println(finger.confidence);
  return finger.fingerID;
 

}




uint8_t getFingerprintIDMaster() {
   customKey = customKeypad.getKey();
   if(customKey == 'C')
{
  digitalWrite(tittit, HIGH);
  delay(100);
  digitalWrite(tittit, LOW);
}
//   cancle = customKey;
//   customKey = customKeypad.getKey();
//    if(customKey) cancle = customKey;
    delay(500);
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial1.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial1.println("Imaging error");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial1.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial1.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial1.println("Could not find fingerprint features");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial1.println("Found a print match!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial1.println("Communication error");
    return p;
  }
  
    else if (p == FINGERPRINT_NOTFOUND) {
    Serial1.println("Did not find a match");
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("VAN TAY SAI! ");
    lcd.setCursor(2, 1);
    lcd.print("HAY THU LAI ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NHAP MAT KHAU ");
    lcd.setCursor(1, 1);
    lcd.print("HOAC VAN TAY ");
//    customKey = 'C';
    return p;    
  } else {
    Serial1.println("Unknown error");
    return p;
  }


 //--------- --------- --------- --------- --------- --------- ---------//FOUND A MATCH
  Serial1.print("Found ID #"); Serial1.print(finger.fingerID);
  Serial1.print(" with confidence of "); Serial1.println(finger.confidence);
  
  
    if (finger.fingerID == 1)
    {
        digitalWrite(tittit, HIGH);
        delay(100);
        digitalWrite(tittit, LOW);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("DA XAC NHAN");
        lcd.setCursor(0, 1);
        lcd.print("VAN TAY MASTER!");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CHUYEN SANG BUOC");
        lcd.setCursor(2, 1);
        lcd.print("DANG KY MOI!");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DAT VAN TAY MOI");
        lcd.setCursor(1, 1);
        lcd.print("LEN CAM BIEN ");
        Serial1.println("Ready to enroll a fingerprint!");
        Serial1.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
        
        id = id + 1;
//        if (id == 0) {// ID #0 not allowed, try again!

//        }
        Serial1.print("Enrolling ID #");
        Serial1.println(id);
        while (!  getFingerprintEnroll() );
    }
      else{
          digitalWrite(tittit, HIGH);
          delay(100);
          digitalWrite(tittit, LOW);
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("VAN TAY SAI!");
          lcd.setCursor(2, 1);
          lcd.print("HAY THU LAI ");
          delay(1000);
          
         
   }
//    customKey = 0;
   return finger.fingerID;
}

uint8_t getFingerprintIDMaster2() {
   customKey = customKeypad.getKey();
   if(customKey == 'C')
{
  digitalWrite(tittit, HIGH);
  delay(100);
  digitalWrite(tittit, LOW);
}
//   cancle = customKey;
//   customKey = customKeypad.getKey();
//    if(customKey) cancle = customKey;
    delay(500);
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial1.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial1.println("Imaging error");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial1.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial1.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial1.println("Could not find fingerprint features");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial1.println("Found a print match!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial1.println("Communication error");
    return p;
  }
  
    else if (p == FINGERPRINT_NOTFOUND) {
    Serial1.println("Did not find a match");
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("VAN TAY SAI! ");
    lcd.setCursor(2, 1);
    lcd.print("HAY THU LAI ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NHAP MAT KHAU ");
    lcd.setCursor(1, 1);
    lcd.print("HOAC VAN TAY ");
//    customKey = 'C';
    return p;    
  } else {
    Serial1.println("Unknown error");
    return p;
  }



 //--------- --------- --------- --------- --------- --------- ---------//FOUND A MATCH
  Serial1.print("Found ID #"); Serial1.print(finger.fingerID);
  Serial1.print(" with confidence of "); Serial1.println(finger.confidence);
  
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    if (finger.fingerID == 1)
    {
     
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("DA XAC NHAN");
        lcd.setCursor(0, 1);
        lcd.print("VAN TAY MASTER!");
        delay(1000);
        
        deleteFingerID();
    }
      else{
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("VAN TAY SAI!");
          lcd.setCursor(2, 1);
          lcd.print("HAY THU LAI ");
          delay(1000);
          
         
   }
//    customKey = 0;
   return finger.fingerID;
}

uint8_t getFingerprintIDMaster3() {
   customKey = customKeypad.getKey();
   if(customKey == 'C')
{
  digitalWrite(tittit, HIGH);
  delay(100);
  digitalWrite(tittit, LOW);
}
//   cancle = customKey;
//   customKey = customKeypad.getKey();
//    if(customKey) cancle = customKey;
    delay(500);
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial1.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial1.println("Imaging error");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial1.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial1.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial1.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial1.println("Could not find fingerprint features");
      return p;
    default:
      Serial1.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial1.println("Found a print match!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial1.println("Communication error");
    return p;
  }
  
    else if (p == FINGERPRINT_NOTFOUND) {
    Serial1.println("Did not find a match");
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("VAN TAY SAI! ");
    lcd.setCursor(2, 1);
    lcd.print("HAY THU LAI ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NHAP MAT KHAU ");
    lcd.setCursor(1, 1);
    lcd.print("HOAC VAN TAY ");
//    customKey = 'C';
    return p;    
  } else {
    Serial1.println("Unknown error");
    return p;
  }


 //--------- --------- --------- --------- --------- --------- ---------//FOUND A MATCH
  Serial1.print("Found ID #"); Serial1.print(finger.fingerID);
  Serial1.print(" with confidence of "); Serial1.println(finger.confidence);
  
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    if (finger.fingerID == 1)
    {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("DA XAC NHAN");
        lcd.setCursor(0, 1);
        lcd.print("VAN TAY MASTER!");
        delay(1000);
        
        change();
    }
      else{
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("VAN TAY SAI!");
          lcd.setCursor(2, 1);
          lcd.print("HAY THU LAI ");
          delay(1000);
          
         
   }
//    customKey = 0;
   return finger.fingerID;
}


//========================= Doc du lieu Unix time duoc viet tu vi tri addr

//============================================================DOC UART TU ESP826666
void Read_UART_8266() 
{

 while (Serial3.available())  //ABCDVDAD\n
   {
    
    char inChar = (char)Serial3.read();
    
    inputStringtime += inChar;
    
    if (inChar == '\n') {
      stringCompletetime = true;
    }
    
    if (stringCompletetime == true)
    {
      //================================== xu ly du lieu
      if(inputStringtime.indexOf("SET") >=0 )
      {
      SetUnix(String(inputStringtime));
      }
      if(inputStringtime.indexOf("CRT") >=0 )
      {
        SendCurrenttime();
      }
      if(inputStringtime.indexOf("OPEN") >=0 )
      {
        digitalWrite(lock, HIGH);
        doorState = 1;
      }
      if(inputStringtime.indexOf("CLOSE") >=0 )
      {
        digitalWrite(lock, LOW);
        doorState = 0;
      }
      //==================================
      inputStringtime = "";
      stringCompletetime = false;
    }
  }
}

//============================================================SET UNIX
void SetUnix(String inputStringtime) 
{
  int TimSET, TimTM;
  TimSET = inputStringtime.indexOf("SET");
  TimTM = inputStringtime.indexOf("TM");
  
  if(TimSET >= 0 && TimTM >= 0)
  {
    String InputUnix = "";
    InputUnix = inputStringtime.substring(TimSET + 3, TimTM);
    unix = InputUnix.toInt();
    unix = unix + 25200;
    
    rtc.setTime(unix);
    
  }   
}

//============================================================SEND THONG TIN LEN ESP DUA VAO SD CARD
void SendSDcard()
{ 
  String byid = "";
  int ss;   
  uint32_t epochTime = rtc.getTime();  //=================Lay time
  uint32_t epoch = epochTime;
  ss = finger.fingerID;  //=================Lay ID
  if((ss<100)&&(ss>=10))
  {
    byid = "0"+ String(ss);
  }
  if(ss<10)
  {
    byid = "00" + String(ss);
  }
  epochTime = rtc.getTime();
  printTime();
  Overtime();

  String TimeAttendance = "TA" + String(bh) + ":" + String(bm)  + "BY" + String(byid) + "ID"  + String(bd)  +  String(bmonth)  + String(by) + "DATE" 
                            + String(overtime) + "OT";
    Serial3.println(TimeAttendance);
    Serial3.flush();
    TimeAttendance = "";
}

//===================================================LAY TIME O POINTEPOCH.TRUYEN VAO CHUOI BS,BM,BH
void SendCurrenttime()
{
    epochTime = rtc.getTime();
    printTime();
       
   String door;
   if(doorState == 1)
   {
    door = "DANGMO";
   }
   else 
   {
    door = "DANGDONG";
   }
    String current = "CT" + String(bh) + ":" + String(bm) +  "TIM" + String(door) 
                      + "DOOR" + String(bd) + "/" + String(bmonth) + "/" + String(by) + "CRD"; 
                      
    Serial3.println(current);
    Serial3.flush();
    current = "";
}

void printTime() {
  printTime(false);
}

void printTime(bool ms) {
  uint32_t tam;
  rtc.epochToTime(epochTime, time);
  tam = time.hours;
  if(tam < 10)
    {
      bh = "0" + String(tam);
    }
    else bh = String(tam);
    
  tam = time.minutes;
  if(tam < 10)
    {
      bm = "0" + String(tam);
    }
    else bm = String(tam);

  tam = time.seconds;//lay giay
    if(tam < 10)
    {
      bs = "0" + String(tam);
    }
    else bs = String(tam);
  rtc.epochToDate(epochTime, date);
  tam = date.day;//lay ngay
    if(tam < 10)
    {
      bd = "0" + String(tam);
    }
    else bd = String(tam);

    tam = date.month;//lay thang
    if(tam < 10)
    {
      bmonth = "0" + String(tam);
    }
    else bmonth = String(tam);

    tam = date.year;//lay nam
    
    by = String(tam);
}
void Overtime()
{   
    uint32_t tam;
    time.seconds = 0;
    time.minutes = 0;
    time.hours = 7;
    epochTime = rtc.dateTimeToEpoch(date, time); //MOC 7 H
    UnixHold = rtc.getTime(); // TG TOI
    if(UnixHold>=epochTime)  //Toi muon
    {
      tam = UnixHold-epochTime;
      if(tam>1800)
      {
        overtime = "3";
      }
      else if(tam>900)
      {
        overtime = "2";
      }
      else 
      {
        overtime = "1";
      }
    }
    else                    
    {
      overtime = "-" ;  // Toi som
    }
}

void Opendoor()
{
    lcd.clear();
    digitalWrite(lock, HIGH);
    digitalWrite(tittit, HIGH);
    delay(100);
    digitalWrite(tittit, LOW);
    doorState = 1;
    lcd.setCursor(2, 0);
    SendCurrenttime();
    lcd.print("CUA DANG MO.");
    delay(1000);
    SendCurrenttime();
    lcd.setCursor(2, 0);
    lcd.print("CUA DANG MO..");
    delay(1000);
    SendCurrenttime();
    lcd.setCursor(2, 0);
    lcd.print("CUA DANG MO...");
    delay(1000);
    digitalWrite(lock, LOW);
    doorState = 0;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NHAP MAT KHAU ");
    lcd.setCursor(1, 1);
    lcd.print("HOAC VAN TAY ");
}

void callNumber()
{
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
}
void baochay()
{
  detectp = digitalRead(detect1);
  detectn = digitalRead(nut1);
  if(detectp == 1)
  {
    vitrichay1 = "Khu A";
    digitalWrite(alert1, HIGH);
  }
  else
  {
    digitalWrite(alert1, LOW);
    vitrichay1 = "";
  }
  
  detectp = digitalRead(detect2);
  detectn = digitalRead(nut2);
  if(detectp == 0)
  {
    digitalWrite(alert2, HIGH);
    vitrichay2 = "Khu B";
  }
  else
  {
    digitalWrite(alert2, LOW);
    vitrichay2 = "";
  }
  vitrichay = String(vitrichay1) + " " + String(vitrichay2);
  if(vitrichay.indexOf("Khu") >=0 )
      {
        digitalWrite(du, HIGH);
        digitalWrite(lock, HIGH);
        doorState = 1;
        digitalWrite(tittit, HIGH);
       Serial3.println(vitrichay);
       Serial3.flush();
     callNumber();
     delay(4000);
       vitrichay = "";
//      }
      }
      else {
        digitalWrite(du, LOW);
        digitalWrite(tittit, LOW);
      }
}
void baochaynut()
{
  detectn = digitalRead(nut1);
  if(detectn == 1)
  {
    vitrichay1 = "Khu A";
  }
  else
  {
    vitrichay1 = "";
  }

  detectn = digitalRead(nut2);
  if(detectn == 1)
  {
    vitrichay2 = "Khu B";
  }
  else
  {
    vitrichay2 = "";
  }
  vitrichay = String(vitrichay1) + " " + String(vitrichay2);
  if(vitrichay.indexOf("Khu") >=0 )
      {
        digitalWrite(du, HIGH);
        digitalWrite(lock, HIGH);
        doorState = 1;
        digitalWrite(tittit, HIGH);
       Serial3.println(vitrichay);
       Serial3.flush();
     callNumber();
     delay(4000);
       vitrichay = "";
//      }
      }
      else {
        digitalWrite(du, LOW);
        digitalWrite(tittit, LOW);
      }
}