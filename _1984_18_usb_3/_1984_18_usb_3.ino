//original NKK code was supplied by Steve Nordhauser. Thanks so much for that.
//integrated to work with arduino and teensy was done by Stephan Schulz for Antimodular Research

#include <SPI.h>
//#include <SD.h>
//#include <SdFat.h>

#define USE_SERIAL

//#define SD_Dat_Diag
#define MODE8 1 //0x30
#define MODE16 2 //0x70
#define MODE MODE16 //MODE16 //MODE8 //


// Hardware pin definitions
#define SHUTDNpin 2
#define DCpin 9
#define RESETpin 8
//#define SDpin 13

//File myFile;
//const int chipSelect_SD = 10;//sparkfun sd card shield
// ** MOSI - pin 11 , 51 SDI //Master Output, Slave Input
// ** MISO - pin 12 , 50 //Master Input, Slave Output
// ** CLK - pin 13  , 52

//arduino mega spi
//SPI: 50 (MISO), 51 (MOSI), 52 (SCK), 53 (SS)

boolean initDone = false;
int sequenceArray[12] = {
  0,1,2,3,4,5,6,7,8,9,10,11};

char serialMessage;
char old_serialMessage;
unsigned long lastSerialTimer;

int imageGroupNum;


#define NUMSWITCH 12
int DIM = 0; //255; //50; //char 0-255

int LCDpin[12] = { 
  22,20,18,16,14,4,6,24,26,28,30,32};


int imageOrder_IDs[12] = {
  1,4,7,10,2,5,8,0,3,6,9,11};
//int imageOrder_IDs[12] = {
//  1,1,2,3,4,5,6,7,8,9,10,11};


#define buttonON false //false
#define buttonOFF true //true

char buttonSerial_IDs[12] = {
  '0','1','2','3','4','5','6','7','8','9','b','c'};
char buttonSerialDown_IDs[12] = {
  'a','d','u','v','x','y','z',',','-','=','[',']'};

int buttonDownCnt[12] = {
  0,0,0,0,0,0,0,0,0,0};
int debounceMaxCount = 1; //50;
boolean buttonWasPushed[12] = {
  false,false,false,false,false,false,false,false,false,false,false,false};

int ButtonPin[12] = {
  23,21,19,17,15,3,5,7,25,27,29,31};

//----------------------------potentiometer--------------------
int potiPin = A13;
#define sensorCnt_max 20
int old_sensorValue = 0;
int sensorDiff = 5;
int old_SensorID;
int sensorCnt = 0;
int sensorValues[sensorCnt_max];
int lastRead = 0;   // Saved value of potentiometer
char potiSerial_IDs[16] ={
  'e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t'};
int averaged_sensorValue = 1024;


int updateDelay = 100;

int cntTest = 0;
int cntTest2 = 0; //random(0,fileAmount);

unsigned long myTimer;

char inputBuffer[64*48*MODE]; // * 2 for mode16,  * 1 for mode8 !!!!!!!!!!!!!!!
//char inputBuffer16bit[64*48*2]; 

void setup() {

  Serial.begin(9600);
  //Serial.println("1984_15");


  delay(1000);
  // Smartswitch setup
  pinMode(DCpin, OUTPUT);  //D/C control	
  pinMode(SHUTDNpin, OUTPUT);  // SHUTDOWN
  digitalWrite(SHUTDNpin, LOW);  // Power supply shutdown starts low
  digitalWrite(DCpin, LOW);  // Start in command mode
  pinMode(RESETpin, OUTPUT);  //RESET for smart switches
  digitalWrite(RESETpin, HIGH);  // Start with Reset high
  delay(10);
  digitalWrite(RESETpin, LOW);	// reset low
  delay(1);  // should be a 3 microsec delay
  digitalWrite(RESETpin,HIGH);  // reset high again
  digitalWrite(SHUTDNpin, HIGH);	// Enable power supply



  for(int i=0; i<NUMSWITCH; i++){
    pinMode(LCDpin[i], OUTPUT); 
    pinMode(ButtonPin[i],INPUT_PULLUP);
  }



  //SPI speed @ CPU Clock Teensy 3 @ 24 MHz @ 48 Mhz @ 96 MHz - (overclock)
  //SPI.setClockDivider(SPI_CLOCK_DIV2); // ~1.738 MHz 3.039 MHz 3.275 MHz
  //SPI.setClockDivider(SPI_CLOCK_DIV4); // ~1.211 MHz 2.202 MHz 2.351 MHz <-- Default Teensy 3 SPI ?
  //SPI.setClockDivider(SPI_CLOCK_DIV8); // ~754 KHz 1.420 MHz 1.516 MHz
  //SPI.setClockDivider(SPI_CLOCK_DIV16); // ~446 KHz 860 KHz 886 KHz
  //SPI.setClockDivider(SPI_CLOCK_DIV32); // ~238 KHz 466 KHz 476 KHz
  //SPI.setClockDivider(SPI_CLOCK_DIV64); // ~127 KHz 251 KHz 253 KHz
  //SPI.setClockDivider(SPI_CLOCK_DIV128); // ~ 65 KHz 129 KHz 130 KHz
  // SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  // ************** BUTTON SETUP
  // needs to be before ethernet so that setup mode will display
  //int ctr;


  for(int ctr=0; ctr<NUMSWITCH; ctr++) {
    SS_Init(ctr,DIM);  //init all displays

    //  delay(20);
  }


  //  SS_Init(0,DIM);
  //  SS_Init(1,DIM);
  //  SS_Init(2,DIM);
  //  SS_Init(3,DIM);
  //
  //  SS_Init(5,DIM);
  //  SS_Init(6,DIM);
  //  testInit();

  //  SS_Init(0,DIM);  //init all displays

  // write a blank to all switches
  clearDisplayBuffer();


  delay(100);	

  myTimer = millis();
  lastSerialTimer = millis();

  //  Serial.println("setup done");
}


void loop(){

  int startChar = Serial.read();
  unsigned int nkkID = 1;
  if (startChar == '*') {
    int count = Serial.readBytes((char*)&nkkID, 2);
    if (count != 2) return;
    count = Serial.readBytes(inputBuffer, sizeof(inputBuffer));

    if(count == sizeof(inputBuffer)){
      if(nkkID < 12){
        update_nkkSwitch(nkkID);
      }
      else{
        for(int i=0; i<NUMSWITCH; i++){
          update_nkkSwitch(i);
        }
      }
    }
  }
  //  if (Serial.available()){
  //handleInput(Serial.read());

  // Serial.println(count);



  //    for(int i=0; i<NUMSWITCH; i++){
  //      update_nkkSwitch(i);
  //    }

  //  }

  sendSerial();




  int temp_buttonPushed = -1;
  for(int i=0; i<NUMSWITCH; i++){
    if( checkButton(i) == true){
      temp_buttonPushed = i;
 
    }
    // if(buttonWasPushed[i] == true) temp_buttonPushed = imageOrder_IDs[i];
  }

  //checkPoti_2();

  //Serial.println("hi");
  updateDelay = map(averaged_sensorValue,0,1024,50,2000);
  if(millis() - myTimer > updateDelay){ //200){

    myTimer = millis();

    //    updateDisplay(filename);
  }


}

void sendSerial(){

  if(serialMessage != old_serialMessage){// || millis()-lastSerialTimer > 500){
    // lastSerialTimer = millis();
    if(serialMessage != '?') Serial.println(serialMessage);
    old_serialMessage = serialMessage;
    serialMessage = '?';
  } 

}

















