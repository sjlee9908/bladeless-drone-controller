#include <SoftwareSerial.h>

SoftwareSerial btSerial(2,3); 

void ParshingRead(){
//THROTTLE, throttle

int throttle_start, throttle_end;
int roll_start, roll_end;
int pitch_start, pitch_end;
int yaw_left_start, yaw_left_end;
int yaw_right_start, yaw_right_end;
String data;

  if(btSerial.available()){
    data = btSerial.readStringUntil('\n');
    Serial.println(data);  //삭제, 테스트용으로 추가
    pitch_start = data.indexOf(":")+1;
    pitch_end = data.indexOf(",", pitch_start+1);
    
    roll_start = data.indexOf(":", pitch_end)+1;
    roll_end = data.indexOf(",", roll_start+1);

    throttle_start = data.indexOf(":", roll_end)+1;
    throttle_end = data.indexOf(",", throttle_start+1);

    yaw_left_start = data.indexOf(":", throttle_end)+1;
    yaw_left_end = data.indexOf(":", yaw_left_start+1);

    yaw_right_start = data.indexOf(":", yaw_left_end)+1;
    yaw_right_end = data.indexOf(":", yaw_right_start+1);
    
    }

    //Serial.print(data.substring(throttle_start, throttle_end).toInt());
    //Serial.println("   ");
    /*
    Serial.print(data.substring(roll_start, roll_end).toInt());
    Serial.print("   ");
    Serial.print(data.substring(pitch_start, pitch_end).toInt());
    Serial.print("   ");
    Serial.print(data.substring(yaw_left_start, yaw_left_end).toInt());
    Serial.print("   ");
    Serial.println(data.substring(yaw_right_start, yaw_right_end).toInt());
    */
}


void setup()
{
  Serial.begin(9600);
  btSerial.begin(9600); 
  Serial.write("serial begin\n");
}

void loop()
{
  delay(200);
  ParshingRead();
}
