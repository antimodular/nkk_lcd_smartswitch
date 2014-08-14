int serialNode = 0;


static void handleInput (char c) {

#ifdef USE_SERIAL

  if ('0' <= c && c <= '9')
    serialNode = 10 * serialNode + c - '0'; //combines any number with multiple digits, i.e. multiple char
  else if (c == ',') {

  } 
  else if ('a' <= c && c <='z') {
    //  Serial.print("\n> ");
    // Serial.print((int) serialNode);

    if(serialNode <= 0) serialNode = 0;
    // Serial.println(c);
    switch (c) {


    case 'n':
      Serial.println("normal operation");
      CommandBuf[0] = 0xa4; //
      for(int ctr=0; ctr<NUMSWITCH; ctr++) {
        SS_Xmit(ctr,  1);
      }
      break;
    case 'h':
      Serial.println("highest brightness");
      CommandBuf[0] = 0xa5; //All pixels on at the highest brightness
      for(int ctr=0; ctr<NUMSWITCH; ctr++) {
        SS_Xmit(ctr,  1);
      }
      break;
    case 'l':
      Serial.println("lowest brightness");
      CommandBuf[0] = 0xa6; //All pixels on at the lowest brightness
      for(int ctr=0; ctr<NUMSWITCH; ctr++) {
        SS_Xmit(ctr,  1);
      }
      break;
    case 'd':
      Serial.println("dim display");
      CommandBuf[0] = 0xac; //
      for(int ctr=0; ctr<NUMSWITCH; ctr++) {
        SS_Xmit(ctr,  1);
      }
      break;

    case 'i':
      Serial.println("display on");
      CommandBuf[0] = 0xaf; //
      for(int ctr=0; ctr<NUMSWITCH; ctr++) {
        SS_Xmit(ctr,  1);
      }
      break;
    case 'o':
      Serial.println("display off");
      CommandBuf[0] = 0xae; //
      for(int ctr=0; ctr<NUMSWITCH; ctr++) {
        SS_Xmit(ctr,  1);
      }
    case 'x':
      Serial.println("inverse");
      CommandBuf[0] = 0xa7; //
      for(int ctr=0; ctr<NUMSWITCH; ctr++) {
        SS_Xmit(ctr,  1);
      }
      //      if(serialNode >= 0 || serialNode < 4){
      //        unitActive[serialNode] = true;
      //        unitPause[serialNode] = false;
      //      }
      break;

    default:
      showHelp();
      break;

    }

    serialNode  = 0;
  } 
#endif
}

void showHelp()
{
#ifdef USE_SERIAL
  Serial.println();
  Serial.println();
  Serial.println("send speed,type of movement");
  //Serial.println("s - start interrupt");
  Serial.println("b - print states");
  Serial.println("r - print IR valves");
  Serial.println("e - print encoder count");
  Serial.println("num + o - one unit OFF");
  Serial.println("num + i - one unit ON");    
  Serial.println("x - all Off");
  Serial.println("y - all On");
  Serial.println("num + k - go up");
  Serial.println("num + n go down");
  Serial.println("s - downduty slower");
  Serial.println("y - downduty faster");
  Serial.println("h - help menu");
  Serial.println();
#endif
}



















































