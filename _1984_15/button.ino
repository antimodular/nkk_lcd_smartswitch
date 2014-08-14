
boolean checkButton(int _id){

  // Serial.println(_id);

  //int temp_buttonState = digitalRead(ButtonPin[imageOrder_IDs[_id]]);
  int temp_buttonState = digitalRead(ButtonPin[_id]);

  //Serial.println(buttonDownCnt[_id]);

  if(temp_buttonState == buttonON){
    buttonDownCnt[_id]++;
  }
  else{
    buttonDownCnt[_id]--;
  }


  buttonDownCnt[_id] = constrain(buttonDownCnt[_id], 0, debounceMaxCount);
  //Serial.print("constrain ");
  // Serial.println(buttonDownCnt[_id]);

  if(buttonDownCnt[_id] >= debounceMaxCount){
   
    //        Serial.print(_id);
    //        Serial.print(" buttonWasPushed = ");
    //        Serial.println(buttonWasPushed[_id]);
    //Serial.println(buttonIDs[_id]);
    if(buttonWasPushed[_id] == false) serialMessage = buttonSerialDown_IDs[imageOrder_IDs[_id]];
     buttonWasPushed[_id] = true;

  }
  if(buttonDownCnt[_id] <= 0){
    if(buttonWasPushed[_id] == true) serialMessage = buttonSerial_IDs[imageOrder_IDs[_id]];
    buttonWasPushed[_id] = false;
  }

  return buttonWasPushed[_id];
  //   if(_id == 0 &&  buttonDownCnt[_id] > 0) {
  //    Serial.println(buttonDownCnt[_id]);
  //     Serial.println(buttonWasPushed[_id]);
  //
  //  }

  // digitalWrite(infoLedPin[_id],buttonWasPushed[_id]);

}







