#include <SoftwareSerial.h>
#define ANGLE_MAX 15
#define ANGLE_MIN -15
int throttle;
int pitch;
int roll;
int yaw_left;  //11
int yaw_right; //8
#define THROTTLE_TEST 1
#define ROLL_TEST 1
#define PITCH_TEST 1


/*
 * 블루투스 https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=rlrkcka&logNo=221379291417
 * throttle 조이스틱   a1-vry
 * angle 조이스틱   a0-vrx, a2-vry
 */

/*
 * gnd  블루투스1개, 조이스틱1개
 * 5v   버튼 2개, 조이스틱2개
 * 3.3v 블루투스1개
 * 아날로그핀3개 조이스틱3개
 * 신호핀 2,3  블루투스1개
 * 신호핀 2개  버튼
*/

//납땜을 그라운드(input_pullup)에 할건지, 5v에 할건지 결정

SoftwareSerial btSerial(2,3);

void check_joystick_throttle(){
  //throttle 조이스틱을 읽어, 평탄화
  throttle = analogRead(A1);
  throttle = map(throttle, 0,1023, 800,1400);
  
}


void check_joystick_roll(){
  //throttle 조이스틱을 읽어, 평탄화
  roll = analogRead(A2);
 // Serial.print(roll); 
 // Serial.print("\t"); 
  if(roll <= 525 && roll >=524){
    roll = 0;
    return;
  }
  roll = map(roll, 0,1023, -15,15);
}

void check_joystick_pitch(){
  //throttle 조이스틱을 읽어, 평탄화
  pitch = analogRead(A0);
  Serial.print(pitch); 
  Serial.print("\t"); 
  if(pitch <= 509 && pitch >=490){ 
    pitch = 0;
    return;
  }
  pitch = map(pitch, 0,1023, -15,15);
}

void check_button(){
  yaw_left = digitalRead(11);
  yaw_right = digitalRead(8);
  if(yaw_left ==1 && yaw_right == 1){
    yaw_left = 0;
    yaw_right = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  btSerial.begin(9600);
  pinMode(8, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
}


void ParshingRead(){
//THROTTLE, throttle

int throttle_start, throttle_end;
int roll_start, roll_end;
int pitch_start, pitch_end;
int yaw_left_start, yaw_left_end;
int yaw_right_start, yaw_right_end;
String data;

  if(Serial.available()){
    data = Serial.readStringUntil('\n');
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

    throttle = data.substring(throttle_start, throttle_end).toInt();
    roll = data.substring(roll_start, roll_end).toInt();
    pitch = data.substring(pitch_start, pitch_end).toInt();
    yaw_left = data.substring(yaw_left_start, yaw_left_end).toInt();
    yaw_right = data.substring(yaw_right_start, yaw_right_end).toInt();
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  check_joystick_throttle();
  check_joystick_roll();
  check_joystick_pitch();
  //check_button();

  btSerial.println((String)"pitch:"+pitch + ", roll:" + roll +  ",   throttle:"+ throttle +
  ", yaw_left:" + yaw_left + ", yaw_right:"+ yaw_right);
  Serial.println((String)"pitch:"+pitch + ", roll:" + roll +  ",   throttle:"+ throttle +
  ", yaw_left:" + yaw_left + ", yaw_right:"+ yaw_right);

  delay(50);
}
