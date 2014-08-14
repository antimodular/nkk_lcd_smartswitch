void checkPoti_2(){

  //read sensor value in to array
  sensorValues[sensorCnt] = analogRead(potiPin);
  sensorCnt++;
  sensorCnt = sensorCnt % sensorCnt_max;

  //get avverage from last x stored readings
  int averaged_sensorValue = 0;
  for(int i=0; i<=sensorCnt_max; i++){
    averaged_sensorValue += sensorValues[i];
  }
  averaged_sensorValue = averaged_sensorValue / sensorCnt_max;

  //if diff is great enough and id is different then send new message 
  if(abs(averaged_sensorValue - old_sensorValue) > sensorDiff) {

    int temp_id = map(averaged_sensorValue,0,1023,0,15);

    if(temp_id != old_SensorID){
      old_SensorID = temp_id;
      serialMessage = potiSerial_IDs[temp_id];
    }

  }

}
void checkPoti(){


  int sensorValue = analogRead(potiPin);


  if (sensorValue < 67){
    if (lastRead >= 67){
      //Serial.println('e');
      serialMessage = potiSerial_IDs[0];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 134){
    if (lastRead < 67 || lastRead >= 134){
      //Serial.println('f');
      serialMessage = potiSerial_IDs[1];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 201){
    if (lastRead < 134 || lastRead >= 201){
      //Serial.println('g');
      serialMessage = potiSerial_IDs[2];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 268){
    if (lastRead < 201 || lastRead >= 268){
      //Serial.println('h');
      serialMessage = potiSerial_IDs[3];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 335){
    if (lastRead < 268 || lastRead >= 335){
      //Serial.println('i');
      serialMessage = potiSerial_IDs[4];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 402){
    if (lastRead < 335 || lastRead >= 402){
      //Serial.println('j');
      serialMessage = potiSerial_IDs[5];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 469){
    if (lastRead < 402 || lastRead >= 469){
      //Serial.println('k');
      serialMessage = potiSerial_IDs[6];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 536){
    if (lastRead < 469 || lastRead >= 536){
      // Serial.println('l');
      serialMessage = potiSerial_IDs[7];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 603){
    if (lastRead < 536 || lastRead >= 603){
      //Serial.println('m');
      serialMessage = potiSerial_IDs[8];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 670){
    if (lastRead < 603 || lastRead >= 670){
      // Serial.println('n');
      serialMessage = potiSerial_IDs[9];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 737){
    if (lastRead < 670 || lastRead >= 737){
      //Serial.println('o');
      serialMessage = potiSerial_IDs[10];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 804){
    if (lastRead < 737 || lastRead >= 804){
      //Serial.println('p');
      serialMessage = potiSerial_IDs[11];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 871){
    if (lastRead < 804 || lastRead >= 871){
      //Serial.println('q');
      serialMessage = potiSerial_IDs[12];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 938){
    if (lastRead < 871 || lastRead >= 938){
      //Serial.println('r');
      serialMessage = potiSerial_IDs[13];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue < 1015){
    if (lastRead < 938 || lastRead >= 1015){
      //Serial.println('s');
      serialMessage = potiSerial_IDs[14];
      lastRead = sensorValue;
    }
  } 
  else if (sensorValue <= 1024){
    if (lastRead < 1015){
      //Serial.println('t');
      serialMessage = potiSerial_IDs[15];
      lastRead = sensorValue;
    }
  }
}






