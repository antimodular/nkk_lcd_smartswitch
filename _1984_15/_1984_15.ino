//original NKK code was supplied by Steve Nordhauser. Thanks so much for that.
//integrated to work with arduino and teensy was done by Stephan Schulz for Antimodular Research

#include <SPI.h>
//#include <SD.h>
#include <SdFat.h>

#define USE_SERIAL

//#define SD_Dat_Diag


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

SdFat sd;
SdFile myFile;
const int chipSelect_SD = 10; //53; //10;//sparkfun sd card shield

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




int fileAmount[12] = {
  247,428,148,390,328,334,266,217,257,288,14,14}; //2;
//int fileAmount[12] = {
//  2,2,2,2,2,2,2,2,2,2,2,2}; //2;

int updateDelay = 100;

int cntTest = 0;
int cntTest2 = 0; //random(0,fileAmount);

unsigned long myTimer;

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

  // pinMode(chipSelect_SD, OUTPUT);

  // *********** initialize SPI:
  // Serial.println("SPI.begin");

  // change to SPI_FULL_SPEED for more performance.
  //SPI_HALF_SPEED
  // if (!sd.begin(chipSelect_SD, SPI_FULL_SPEED)){
  if (!sd.begin(chipSelect_SD, SPI_HALF_SPEED)){
    Serial.println("initialization failed!");
    sd.initErrorHalt();
    return;
  }
  //  if (!sd.begin(chipSelect_SD)) {
  //    Serial.println("initialization failed!");
  //    return;
  //  }
  Serial.println("initialization done.");
  //

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



  // root = SD.open("/");
  //  
  //  printDirectory(root, 0);

  // Serial.println("done!");

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

  //  Serial.println("empty buffer");
  // write empty buffer to all switches
  // for(int ctr=0; ctr<NUMSWITCH; ctr++){
  //SS_Image(FULLH, NBG, BLACK, ctr);
  //  delay(20);
  // }
  // Button logo displays

  //0-534-0, 0-2204-0, 0-2475-0
  // SD_Read8(XSIZE,FileName0[0]);  // get rotated logo image and place in  buffer

    // Serial.println("load bmp");
  //for(int ctr=0; ctr<4; ctr++) 
  //   SS_Image(FULLH, NBG, BLACK, 0);  //Logo on all number displays	
  //   SD_Read8(XSIZE,"TNF0.bmp");  // get non-rotated logo image and place in  buffer
  //   for(int ctr=4; ctr<NUMSWITCH; ctr++) SS_Image(FULLH, NBG, BLACK, ctr);  //Logo on all control displays	


  // SS_Testpattern(cntTest);
  // SS_Image(FULLH, NBG, BLACK, 0);  //Logo on all control displays	

  //  SD_Read8(XSIZE,FileName1[0]);
  //  SS_Image(FULLH, NBG, BLACK, 1);

  delay(100);	

  myTimer = millis();
  lastSerialTimer = millis();

  //  Serial.println("setup done");
}


void loop(){

  if (Serial.available()){
    handleInput(Serial.read());
  }

  sendSerial();




  int temp_buttonPushed = -1;
  for(int i=0; i<NUMSWITCH; i++){
    if( checkButton(i) == true){
      temp_buttonPushed = i;
      if(i == 7){
        Serial.println("i == 7");
        SPI.end();
        delay(50);
        SPI.begin();
        SPI.setClockDivider(SPI_CLOCK_DIV2);
      } 
    }
    // if(buttonWasPushed[i] == true) temp_buttonPushed = imageOrder_IDs[i];

  }

  checkPoti_2();

  //Serial.println("hi");

  if(millis() - myTimer > updateDelay){ //200){

    myTimer = millis();


    //    int imageGroupNum = random(0,NUMSWITCH);
    //    int imageFileNum = random(0,fileAmount);
    //
    //    if(initDone == false){
    imageGroupNum = sequenceArray[cntTest];
    if(temp_buttonPushed != -1){
      //Serial.println(temp_buttonPushed);
      imageGroupNum = temp_buttonPushed; 
    }

    int imageFileNum = random(0,fileAmount[imageOrder_IDs[imageGroupNum]]);

    //to cycle images from top left to bottom right, in a vertical fashion
    cntTest++;
    if(cntTest >= NUMSWITCH){
      cntTest = 0;
      shuffle(sequenceArray,12);

      /*
      cntTest2++;
       if(cntTest2 >= fileAmount){
       cntTest2 = 0;
       // initDone = true;
       
       }
       */
    }
    // }

    String temp_string = "f";
    temp_string.concat(imageOrder_IDs[imageGroupNum]);
    temp_string.concat("/"); 
    temp_string.concat(imageFileNum); //random(0,fileAmount));
    temp_string.concat(".bmp");

    char filename[temp_string.length()+1];
    temp_string.toCharArray(filename, sizeof(filename));


    updateDisplay(filename);



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


void shuffle(int *array, size_t n)
{
  if (n > 1) 
  {
    size_t i;
    for (i = 0; i < n - 1; i++) 
    {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}















