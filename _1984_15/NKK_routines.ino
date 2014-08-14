// Smart switch routines

// NKK OLED switch ISC15ANP4
// 64x48 pixels, 12 bit/pixel

// 8 bit colors for displays

//#define SD_Dat_Diag //data debug

#define RED 0x03  //toned down
#define GREEN 0x10  // toned down
#define BLUE 0xe0
#define WHITE 0xff
#define BLACK 0x00
#define LRED 0x02
#define LGREEN 0x10
#define GREY 0x92
#define AMBER 0x1f
#define LAMBER 0x0e
#define VLRED 0x02
#define VLGREEN 0x08

byte colors[12] ={
  RED,GREEN,BLUE,WHITE,BLACK,LRED,LGREEN,GREY,AMBER,LAMBER,VLRED,VLGREEN};

#define MODE8 0x30
#define MODE16 0x70
#define MODE MODE8 //MODE16 //MODE8 //

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

void updateDisplay(char* filename){
  if(MODE==MODE8){
    SD_Read8(XSIZE,filename);
  }
  else{
    SD_Read16(XSIZE,filename);
  }
  //SS_Testpattern(random(4,10));
  SS_Image(FULLH, NBG, BLACK, imageGroupNum);
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
    CommandBuf[1] = MODE8; //Remap and color depth setting - 64K - 0x70 256-0x30 color mode
  else
    CommandBuf[1] = MODE16;
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
void SS_Image(byte lrtbf, byte Abg, byte BGColor, int SwitchNum) {
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

  /*
  if(lrtbf == FULL) {
   CommandBuf[0] = 0x15;
   CommandBuf[1] = 0x10;
   CommandBuf[2] = 0x4f;
   SS_Xmit(SwitchNum,  3);  // Set column range
   CommandBuf[0] = 0x75;
   CommandBuf[1] = 0x00;
   CommandBuf[2] = 0x2f;
   SS_Xmit(SwitchNum,  3);  // Set row range
   xsize = XSIZE;
   } 
   else if((lrtbf == RIGHT)||(lrtbf == BOTTOM)) {
   CommandBuf[0] = 0x15;
   CommandBuf[1] = 0x10;
   CommandBuf[2] = 0x2f;
   SS_Xmit(SwitchNum,  3);  // Set column range
   CommandBuf[0] = 0x75;
   CommandBuf[1] = 0x00;
   CommandBuf[2] = 0x2f;
   SS_Xmit(SwitchNum,  3);  // Set row range
   xsize = XHSIZE;		
   } 
   else {
   */
  /*
  CommandBuf[0] = 0x15;
   CommandBuf[1] = 0x30;
   CommandBuf[2] = 0x4f;
   SS_Xmit(SwitchNum,  3);  // Set column range
   CommandBuf[0] = 0x75;
   CommandBuf[1] = 0x00;
   CommandBuf[2] = 0x2f;
   SS_Xmit(SwitchNum,  3);  // Set row range
   xsize = XHSIZE;
   */
  //  }

  Mode(IMAGEDATA);
  // Block the SS pin low to select the chip:
  SwitchSelect(SwitchNum,ENABLE);	
  // Window set on chip puts the data in the right spot
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
  // Block the SS pin high to de-select the chip:
  SwitchSelect(SwitchNum,DISABLE);

  // delay(10);
}


// creates vertical RGB color stripes offset by the seed number
void SS_Testpattern(byte seed)  {
  char rowctr;
  char colctr;
  char tcolctr; 

  int a = random(0,12);
  int b = random(0,12);
  int c = random(0,12);

  for(rowctr=0; rowctr<YSIZE; rowctr++)  {
    for(colctr=0; colctr<XSIZE; colctr+=seed)  {
      tcolctr = (colctr) ;
      DisplayDat[rowctr][tcolctr] = colors[a];
      DisplayDat[rowctr][tcolctr+1] = colors[a];
      DisplayDat[rowctr][tcolctr+2] = colors[a];
      tcolctr += 3;
      DisplayDat[rowctr][tcolctr] = colors[b];
      DisplayDat[rowctr][tcolctr+1] = colors[b];
      tcolctr += 2;
      DisplayDat[rowctr][tcolctr] = colors[c];
      DisplayDat[rowctr][tcolctr+1] = colors[c];
      DisplayDat[rowctr][tcolctr+2] = colors[c];
    }
  }
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

  //  digitalWrite(LCDpin[0], HIGH);
  //  Serial.print("SwitchSelect Dat = ");
  //  Serial.println(Dat);
  //
  if(Dir == ENABLE) {
    // Serial.println("Dir == ENABLE");
    // digitalWrite(LCDpin[(7-Dat)], LOW);   // Make one selected if required
    // digitalWrite(LCDpin[imageOrder_IDs[Dat]], LOW);
    digitalWrite(LCDpin[Dat], LOW);  
  }

  // Data complete.  Bang Address line high, then low to latch
  //Serial.print("Switch select data dir:");
  //Serial.print(""%d"",Dat);
  //Serial.print("  ");
  //Serial.print(""%d"",Dir);
  //Serial.print("\n");
  // PORTD |= 0x80;
  //Serial.print("Enable high");
  // PORTD &= 0x7f;
  // Serial.print("  Enable low\n");
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



// SD_Read8
// Reads a 24 bit bmp image file from SD
// Converts it to the 332 color format for thxe NKK switches
// and stores the data in the global image buffer
// XSIZE is a paramater so either XSIZE or XHSIZE can be passed to it

void SD_Read8(int xsize, char* FileName) {	  
  //	 int16_t buf;
  int x; // Local buffer indexes for the 8 bit per pixel output array
  int y;
  int16_t n;
  uint16_t pixel;
  int ctr;  //byte counter for the bmp file
  uint8_t buf[32];// nothing special about 32, just a lucky number.
  boolean getout = false;

  // open a file
  // if (file.open(&root, FileName, O_READ)) {
  if(myFile.open(FileName,O_READ)){
    //    Serial.print(FileName);
    //    Serial.println(" file found");
    //		Serial.print("Opened: ");
    //		Serial.print(FileName);	
    //		Serial.print("\n");	
    // copy file for now to display to look at headers
    ctr=0;
    x = 0;   //bmp files are from the lower left, scanning up
    y = YSIZE-1;

    // print 32 byte lines
    //	Serial.print("Header data: ");
    while (((n = myFile.read(buf, sizeof(buf))) > 0) && getout == false) {
      for (uint8_t i = 0; i < n; i++) {
        //			if(ctr==54) Serial.print("Image data: \n");

        if(ctr>=54) {  // skip header
          if(x > (xsize-1)) {
            x = 0;
            y--;
          }

          if(y>=0) {  // truncate file if too large
#ifdef  SD_Dat_Diag
            Serial.println();
            Serial.print(i);
            Serial.print(" buf[i]:  ");
            Serial.print(buf[i], HEX);
            Serial.print(" ");
            Serial.print(buf[i], DEC);
            Serial.print(" ");
            Serial.print(buf[i], BIN);
            Serial.println();
#endif
            if(ctr % 3 == 0) {	// note: can use ctr because the header is 54 bytes, divisible by 3
              pixel = buf[i] & 0xe0;  //Blue is 3 MSBs, zero rest

#ifdef  SD_Dat_Diag
              Serial.print("  Blue dat: ");
              Serial.print(pixel, HEX);
              Serial.print(" ");
              Serial.print(pixel, DEC);
              Serial.print(" ");
              Serial.print(pixel, BIN);
#endif
            }						
            else if (ctr % 3 == 1) {
              pixel |= (buf[i] & 0xe0)>>3; //Green is 3 MSBs, shifted into position
#ifdef  SD_Dat_Diag
              Serial.print("  Blue+Green dat: ");
              Serial.print(pixel, HEX);
              Serial.print(" ");
              Serial.print(pixel, DEC);
              Serial.print(" ");
              Serial.print(pixel, BIN);
#endif
            }
            else {
              pixel |= (buf[i] & 0xc0)>>6; //Red is 2 MSBs, shifted into position
              DisplayDat[y][x] = pixel;
              x++;  // Increment x with every pixel stored
#ifdef  SD_Dat_Diag				
              Serial.print("  BGR dat: ");
              Serial.print(pixel, HEX);
              Serial.print(" ");
              Serial.print(pixel, DEC);
              Serial.print(" ");
              Serial.print(pixel, BIN);
              Serial.print("  x,y: ");
              Serial.print(x);
              Serial.print(",");
              Serial.print(y);
              Serial.print("\n");

              delay(100);
#endif
            }
          }//end if(y>=0)
        }//end if(ctr>=54)

        //			if(i %32 == 31) {
        //				Serial.print(buf[i],HEX);
        //				Serial.print("\n");
        //			} else Serial.print(buf[i],HEX);
        ctr++;
      }// end  for (uint8_t i = 0; i < n; i++)  
    }//while (((n = myFile.read(buf, sizeof(buf))) > 0) && getout == false

    //	Serial.print("byte count: ");
    //	Serial.print("%d",ctr);
    //	Serial.print("\n");

    myFile.close();
    //if(!myFile.close()) {
    //			error("close failed");
    //}
  } 
  else	{

    Serial.print("can't read file = ");
    Serial.println(FileName);
    /*
    //		error("file.open failed");
     //		delay(20);
     if(file.open(&root, FileName, O_READ))	{
     } 
     else	{
     //			error("file.open failed 2");
     //			delay(20);
     if(file.open(&root, FileName, O_READ))	{
     } 
     else	{
     //				error("file.open failed 3");
     SS_Fill(WHITE);  // fill buffer with background color
     //				getout = TRUE;
     }
     }
     */
  }
  //	Serial.print("\nDone\n");

}

// SD_Read8
// Reads a 24 bit bmp image file from SD
// Converts it to the 332 color format for thxe NKK switches
// and stores the data in the global image buffer
// XSIZE is a paramater so either XSIZE or XHSIZE can be passed to it


void SD_Read16(int xsize, char* FileName) {	  

  int x; // Local buffer indexes for the 8 bit per pixel output array
  int y;
  int16_t n;
  uint16_t pixel; // data sheet says display wants 2 bytes per pixel
  int ctr;  //byte counter for the bmp file
  //  uint8_t buf[32];// nothing special about 32, just a lucky number.
  uint8_t buf[32];//
  boolean getout = false;
  // open a file




    // if (file.open(&root, FileName, O_READ)) {
  if(myFile.open(FileName,O_READ)){

    ctr=0;
    x = 0;   //bmp files are from the lower left, scanning up
    y = YSIZE-1;

    // print 32 byte lines
    //	Serial.print("Header data: ");
    while (((n = myFile.read(buf, sizeof(buf))) > 0) && getout == false) {
      for (uint8_t i = 0; i < n; i++) {
        //			if(ctr==54) Serial.print("Image data: \n");

        if(ctr>=54) {  // skip header
          if(x > (xsize-1)) {
            x = 0;
            y--;
          }

          if(y>=0) {  // truncate file if too large
#ifdef  SD_Dat_Diag
            Serial.println();
            Serial.print(i);
            Serial.print(" buf[i]:  ");
            Serial.print(buf[i], HEX);
            Serial.print(" ");
            Serial.print(buf[i], DEC);
            Serial.print(" ");
            Serial.print(buf[i], BIN);
            Serial.println();
#endif
            if(ctr % 3 == 0) {	// note: can use ctr because the header is 54 bytes, divisible by 3
              pixel = (buf[i] & 0xf8)<<8;  //take 5 bits from blue 8 bit channel and shift them to the far left of 16bit pixel

#ifdef  SD_Dat_Diag
              Serial.print("  Blue dat: ");
              Serial.print(pixel, HEX);
              Serial.print(" ");
              Serial.print(pixel, DEC);
              Serial.print(" ");
              Serial.print(pixel, BIN);
#endif
            }						
            else if (ctr % 3 == 1) {
              pixel |= (buf[i] & 0xfc)<<3; ////take 6 bits from green 8 bit channel and shift them to the 3 to the left, to set in the middle of 16bit pixel
#ifdef  SD_Dat_Diag{
              Serial.print("  Blue+Green dat: ");
              Serial.print(pixel, HEX);
              Serial.print(" ");
              Serial.print(pixel, DEC);
              Serial.print(" ");
              Serial.print(pixel, BIN);
#endif
            }
            else {
              pixel |= (buf[i] & 0xf8)>>3; // //take 5 bits from red 8 bit channel and shift them 3 bits to right to set at end of 16bit pixel
              DisplayDat[y][x] = pixel;
              x++;  // Increment x with every pixel stored
#ifdef  SD_Dat_Diag				
              Serial.print("  BGR dat: ");
              Serial.print(pixel, HEX);
              Serial.print(" ");
              Serial.print(pixel, DEC);
              Serial.print(" ");
              Serial.print(pixel, BIN);
              Serial.print("  x,y: ");
              Serial.print(x);
              Serial.print(",");
              Serial.print(y);
              Serial.print("\n");

              delay(100);
#endif
            }
          }
        }
        ctr++;
      }
    }  

    myFile.close();

  } 
  else	{

    Serial.print("can't read file = ");
    Serial.println(FileName);

  }
  //	Serial.print("\nDone\n");

}







