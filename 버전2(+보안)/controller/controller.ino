#include <SoftwareSerial.h>
#include <AESLib.h>
#define ANGLE_MAX 15
#define ANGLE_MIN -15
int up_down;
int fb_angle;
int rl_angle;
int left_turn;  //11
int right_turn; //8

/*
 * 블루투스 https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=rlrkcka&logNo=221379291417
 * up_down 조이스틱   a1-vry
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

uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
SoftwareSerial btSerial(2,3);

void check_joystick(){
  //UP_DOWN 조이스틱을 읽어, 평탄화
  up_down = analogRead(A1);
  up_down = map(up_down, 0,1023, 0,150);
  

  //방향 조이스틱을 읽어, 각도 조절
  //rl_angle이 +면 right
  //rl_angle이 -면 left
  rl_angle = (analogRead(A0)-511)/11;

  //방향 조이스틱을 읽어, 각도 조절
  //fb_angle이 -면 f
  //fb_angle이 +면 b
  fb_angle = (analogRead(A2)-511)/11;


  if(analogRead(A2)/100 == 5 && analogRead(A0)/100 == 5) {
    rl_angle=0;
    fb_angle=0; 
  }

  //Serial.println((String)"fb_angle: "+fb_angle + ", rl_angle: " + rl_angle);
}

void check_button(){
  left_turn = digitalRead(11);
  right_turn = digitalRead(8);
  if(left_turn ==1 && right_turn == 1){
    left_turn = 0;
    right_turn = 0;
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

  fb_angle = 42, rl_angle=23;
  up_down = 12;
  left_turn = 35, right_turn = 35;


  

  String data = (String)"fb_angle:"+fb_angle + ", rl_angle:" + rl_angle +  ",   up_down:"+ up_down +
  ", left_turn:" + left_turn + ", right_turn:"+ right_turn;
  char encrypted_data[80];
  
  data.toCharArray(encrypted_data, 80);
  Serial.println(encrypted_data);
  aes256_enc_multiple(key, encrypted_data, 80);
  
  //btSerial.println(be_encrypt);
  Serial.println(encrypted_data);
  
  aes256_dec_multiple(key, encrypted_data, 80);
  Serial.println(encrypted_data);
  
  
}
