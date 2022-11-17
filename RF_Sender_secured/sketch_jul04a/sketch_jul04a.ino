#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <AESLib.h>
#define ANGLE_MAX 15
#define ANGLE_MIN -15
int throttle;
int pitch;
int roll;
int yaw_left;  //5
int yaw_right; //4
int yaw;
#define THROTTLE_TEST 1
#define ROLL_TEST 1
#define PITCH_TEST 1

//추가할거
//켜지면 부저, led on
//버튼 1개를 누르면 시동 걸림
//버튼 1개는 카메라 찍는거

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

uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

void check_joystick_throttle(){
  //throttle 조이스틱을 읽어, 평탄화
  throttle = analogRead(A0);
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
  pitch = analogRead(A3);
  //Serial.print(pitch); 
  //Serial.print("\t"); 
  if(pitch <= 509 && pitch >=490){ 
    pitch = 0;
    return;
  }
  pitch = map(pitch, 0,1023, -15,15);
}

void check_joystick_yaw(){
  //throttle 조이스틱을 읽어, 평탄화
  yaw_left = 0;
  yaw_right = 0;
  yaw = analogRead(A1);
  yaw = map(yaw, 0,1023, -15,15);
  if(yaw <= -14) yaw_left = 1;
  else if(yaw >= 14) yaw_right = 1;
}


/*
void check_button(){
  yaw_left = digitalRead(4);
  yaw_right = digitalRead(5);
  if(yaw_left ==1 && yaw_right == 1){
    yaw_left = 0;
    yaw_right = 0;
  }
}
*/
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
    pitch_start = data.indexOf(" ")+1;
    pitch_end = data.indexOf(" ", pitch_start+1);
    
    roll_start = data.indexOf(" ", pitch_end)+1;
    roll_end = data.indexOf(" ", roll_start+1);

    throttle_start = data.indexOf(" ", roll_end)+1;
    throttle_end = data.indexOf(" ", throttle_start+1);

    yaw_left_start = data.indexOf(" ", throttle_end)+1;
    yaw_left_end = data.indexOf(" ", yaw_left_start+1);

    yaw_right_start = data.indexOf(" ", yaw_left_end)+1;
    yaw_right_end = data.indexOf(" ", yaw_right_start+1);

    throttle = data.substring(throttle_start, throttle_end).toInt();
    roll = data.substring(roll_start, roll_end).toInt();
    pitch = data.substring(pitch_start, pitch_end).toInt();
    yaw_left = data.substring(yaw_left_start, yaw_left_end).toInt();
    yaw_right = data.substring(yaw_right_start, yaw_right_end).toInt();
    }
}

RF24 radio(7, 8); // SPI통신을 위한 (CE, CSN) 핀 선언
const byte address[6] = "00001"; // 송신기와 수신기를 동일한 값으로 주소 설정함(5자

void setup() {
  radio.begin();
  Serial.begin(9600);
  radio.openWritingPipe(address); // 데이터를 보낼 수신 주소를 설정
  radio.setPALevel(RF24_PA_HIGH); // 송신거리에 따른, 전원공급 파워레벨 설정
  //(최소) RF24_PA_MIN → RF24_PA_LOW → RF24_PA_HIGH → RF24_PA_MAX (최
  //송신이 약하다고 판단될 경우 nRF24모듈의 GND와 3.3V 단자에 전해콘덴서(10uF이
  radio.stopListening(); // 모듈을 송신기로 설정
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  check_joystick_throttle();
  check_joystick_roll();
  check_joystick_pitch();
  check_joystick_yaw();
  //check_button();

  String stringText1 =
        (String)"pitch:"+pitch + ", roll:" + roll +  ",   throttle:"+ throttle +
        ", yaw_left:" + yaw_left + ", yaw_right:"+ yaw_right;
  
  String stringText2 =
        (String)" "+pitch + "  " + roll +  "  "+ throttle +
        "  " + yaw_left + "  "+ yaw_right;
  char charText[32];
  
  stringText2.toCharArray(charText, 32);
  Serial.println(stringText1);
  //Serial.println(charText);
  aes256_enc_multiple(key, charText, 32);
  //Serial.println(charText);
  //Serial.println(sizeof(charText));
  Serial.println(radio.write(&charText, sizeof(charText)));
  delay(50);
}
