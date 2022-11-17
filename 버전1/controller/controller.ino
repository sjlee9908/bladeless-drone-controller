#include <SoftwareSerial.h>
#define ANGLE_MAX 15
#define ANGLE_MIN -15
int throttle;
int pitch;
int roll;
int yaw_left;  //11
int yaw_right; //8

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

void check_joystick(){
  //throttle 조이스틱을 읽어, 평탄화
  throttle = analogRead(A1);
  throttle = map(throttle, 0,1023, 0,500);
  

  //방향 조이스틱을 읽어, 각도 조절
  //roll이 +면 right
  //roll이 -면 left
  roll = (analogRead(A0)-511)/11;

  //방향 조이스틱을 읽어, 각도 조절
  //pitch이 -면 f
  //pitch이 +면 b
  pitch = (analogRead(A2)-511)/11;


  if(analogRead(A2)/100 == 5 && analogRead(A0)/100 == 5) {
    roll=0;
    pitch=0; 
  }

  //Serial.println((String)"pitch: "+pitch + ", roll: " + roll);
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

void loop() {
  // put your main code here, to run repeatedly:
  check_joystick();
  check_button();

  
  btSerial.println((String)"pitch:"+pitch + ", roll:" + roll +  ",   throttle:"+ throttle +
  ", yaw_left:" + yaw_left + ", yaw_right:"+ yaw_right);
  Serial.println((String)"pitch:"+pitch + ", roll:" + roll +  ",   throttle:"+ throttle +
  ", yaw_left:" + yaw_left + ", yaw_right:"+ yaw_right);
  
}
