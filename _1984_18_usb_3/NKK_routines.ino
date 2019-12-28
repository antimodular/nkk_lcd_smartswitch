// Smart switch routines

// NKK OLED switch ISC15ANP4
// 64x48 pixels, 12 bit/pixel

// 8 bit colors for displays

//#define SD_Dat_Diag //data debug




#define ENABLE 1
#define DISABLE 0

#define COMMAND 0
#define IMAGEDATA 1 //or maybe 1

// SmartSwitch
#define XSIZE  64
#define XHSIZE 64 //32
#define YSIZE 48

#define FULLH 0
#define FULLV 1
#define LEFT 2
#define RIGHT 3
#define TOP 4
#define BOTTOM 5
#define BG 0   // Use background color
#define NBG 1  // No background color


//byte DisplayDat [YSIZE] [XSIZE]; //Storage for current display data (8 bit mode)
uint16_t DisplayDat [YSIZE] [XSIZE]; //Storage for current display data (8 bit mode)

byte CommandBuf[10];

void clearDisplayBuffer(){
  for(int ctr=0; ctr < XSIZE; ctr++) {  // empty the buffer
    for(int ctr1=0; ctr1 < YSIZE; ctr1++) {
      DisplayDat [ctr1] [ctr] = 0;
    }
  }
}

void update_nkkSwitch(int switchNum){
  SS_Image(FULLH, NBG, switchNum);
}

void updateDisplay(char* filename){


  //SS_Testpattern(random(4,10));
  SS_Image(FULLH, NBG, imageGroupNum);

}

//  SS_Init
// Initializes the switch number passed to it
void SS_Init(int SwitchNum, char Bright) {

  if (Bright == DIM) {
    CommandBuf[0] = 0x81;
    CommandBuf[1] = 0x15;
    SS_Xmit(SwitchNum,  2);  //Contrast for color A
    CommandBuf[0] = 0x82;
    CommandBuf[1] = 0x1A;
    SS_Xmit(SwitchNum,  2);  //Contrast for color B
    CommandBuf[0] = 0x83;
    CommandBuf[1] = 0x17;
    SS_Xmit(SwitchNum,  2);  //Contrast for color C
  } 
  else {
    CommandBuf[0] = 0x81;
    CommandBuf[1] = 0x19;
    SS_Xmit(SwitchNum,  2);  //Contrast for color A
    CommandBuf[0] = 0x82;
    CommandBuf[1] = 0x14;
    SS_Xmit(SwitchNum,  2);  //Contrast for color B
    CommandBuf[0] = 0x83;
    CommandBuf[1] = 0x24;
    SS_Xmit(SwitchNum,  2);  //Contrast for color C
  }

  CommandBuf[0] = 0x87;
  CommandBuf[1] = 0x0f; //Master current control
  SS_Xmit(SwitchNum,  2);	

  CommandBuf[0] = 0xa0;
  if(MODE==MODE8)
    CommandBuf[1] = 0x30; //MODE8; //Remap and color depth setting - 64K - 0x70 256-0x30 color mode
  else
    CommandBuf[1] = 0x70; //MODE16;
  SS_Xmit(SwitchNum,  2);

  CommandBuf[0] = 0xa1;
  CommandBuf[1] = 0x00; //Set display start line
  SS_Xmit(SwitchNum,  2);
  CommandBuf[0] = 0xa2;
  CommandBuf[1] = 0x10; //Set display offset
  SS_Xmit(SwitchNum,  2);
  CommandBuf[0] = 0xa4; //Normal display
  SS_Xmit(SwitchNum,  1);
  CommandBuf[0] = 0xa8;
  CommandBuf[1] = 0x2f; //Multiplex ratio
  SS_Xmit(SwitchNum,  2);	
  CommandBuf[0] = 0xab;
  CommandBuf[1] = 0x00;
  CommandBuf[2] = 0x12;
  CommandBuf[3] = 0x0c;
  CommandBuf[4] = 0x14;
  CommandBuf[5] = 0x12; //Dim mode setting for color A,B,C
  SS_Xmit(SwitchNum,  6);
  CommandBuf[0] = 0xad;
  CommandBuf[1] = 0x8e; //Master configuration
  SS_Xmit(SwitchNum,  2);	
  CommandBuf[0] = 0xb0;
  CommandBuf[1] = 0x0b; //Power save mode
  SS_Xmit(SwitchNum,  2);
  CommandBuf[0] = 0xb1;
  CommandBuf[1] = 0x44; //Phase 1 and 2 period adjustment
  SS_Xmit(SwitchNum,  2);
  CommandBuf[0] = 0xb3;
  CommandBuf[1] = 0xa0; //Display clock divider
  SS_Xmit(SwitchNum,  2);
  CommandBuf[0] = 0xb9; //Enable linear gray scale
  SS_Xmit(SwitchNum,  1);
  CommandBuf[0] = 0xbb;
  CommandBuf[1] = 0x12; //Pre charge level
  SS_Xmit(SwitchNum,  2);

  if (Bright == DIM) {
    CommandBuf[0] = 0xbe;
    CommandBuf[1] = 0x3e;
    SS_Xmit(SwitchNum,  2);  //Set Vcomh
  } 
  else {
    CommandBuf[0] = 0xbe;
    CommandBuf[1] = 0x28;
    SS_Xmit(SwitchNum,  2);  //Set Vcomh
  }

  CommandBuf[0] = 0x2e; //Scroll deactivate
  SS_Xmit(SwitchNum,  1);	
  CommandBuf[0] = 0xaf; //Display on in normal mode
  SS_Xmit(SwitchNum,  1);		
}


// SS_Image
// Sends the bitmapped image to the selected switch
// Can send to left, right or full
// Use background or no background
// For NBG, color is just a placeholder
// Parameters are: LEFT/RIGHT/TOP/BOTTOM/FULL  Position of button data on button
// 					BG/NBG for background used
//					BGColor  color value to be substituted for BG
//					SwitchNum - which switch to place image
void SS_Image(byte lrtbf, byte Abg, int SwitchNum) {
  int rowctr;
  int colctr;
  //byte tval;
  byte tval;
  int xsize;
  // Serial.print("Switch number: ");
  // Serial.print(""%d"",SwitchNum);
  // Serial.print("  BG Color: ");
  // Serial.print(""%d"",BGColor);
  //	Serial.print("\n");

  CommandBuf[0] = 0x15;
  CommandBuf[1] = 0x10;
  CommandBuf[2] = 0x4f;
  SS_Xmit(SwitchNum,  3);  // Set column range
  CommandBuf[0] = 0x75;
  CommandBuf[1] = 0x00;
  CommandBuf[2] = 0x2f;
  SS_Xmit(SwitchNum,  3);  // Set row range
  xsize = XSIZE;


  Mode(IMAGEDATA);
  // Block the SS pin low to select the chip:
  SwitchSelect(SwitchNum,ENABLE);	
  // Window set on chip puts the data in the right spot

  for(int i=0; i<sizeof(inputBuffer); i++){
    tval = inputBuffer[i];
    SPI.transfer(tval);
  }

  /*
  for(rowctr=0; rowctr<YSIZE; rowctr++)  {
   for(colctr=0; colctr<xsize; colctr++)  {
   
   // 
   if(MODE==MODE8){
   tval = DisplayDat[rowctr][colctr];
   SPI.transfer(tval);  // write data 
   }
   else{
   char hi =  DisplayDat[rowctr][colctr] & 0xFF;
   char lo =  DisplayDat[rowctr][colctr] >> 8;
   tval = lo;
   SPI.transfer(tval);  // write data 
   
   tval = hi;
   SPI.transfer(tval);  // write data 
   
   }
   
   }
   }
   */
  // Block the SS pin high to de-select the chip:
  SwitchSelect(SwitchNum,DISABLE);

  // delay(10);
}





// SS_Xmit
// Transmits an array of any length to a selected switch
// need to generalize array since it could be hex values
void SS_Xmit(int SwitchNum,  int strlen) {
  int cnt;
  Mode(COMMAND);
  for(cnt=0; cnt < strlen; cnt++) SPI_Write(SwitchNum, CommandBuf[cnt]);
}

// Sets or clears SS slave selects 0-6
// Inputs: SS #, ON or OFF
// Outputs: None
void SwitchSelect (int Dat, byte Dir)  {
  // int ctr;
  //Write Correct slave select - others are off
  // digitalWrite(LCDEpin, LOW);  // be sure we don't write to the LCD

  for(int ctr=0; ctr<NUMSWITCH; ctr++)  digitalWrite(LCDpin[ctr], HIGH);   // set all selects off

  //
  if(Dir == ENABLE) {
    digitalWrite(LCDpin[Dat], LOW);  
  }

}



void SPI_Write(int SwitchNum, byte value) {
  // Block the SS pin low to select the chip:
  SwitchSelect(SwitchNum,ENABLE);
  //  send value via SPI:
  SPI.transfer(value);
  // Block the SS pin high to de-select the chip:
  SwitchSelect(SwitchNum,DISABLE);
}

void Mode(boolean DC) {
  if(DC == COMMAND) digitalWrite(DCpin, LOW); 
  else digitalWrite(DCpin, HIGH);
}

void SS_Fill(byte color) {
  char rowctr;
  char colctr; 
  for(rowctr=0; rowctr<YSIZE; rowctr++)  {
    for(colctr=0; colctr<XSIZE; colctr++)  {
      DisplayDat[rowctr][colctr] = color;
    }
  }
}













