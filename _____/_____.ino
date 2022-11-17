#include <SoftwareSerial.h>
#include <AESLib.h>
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

uint8_t sender_key;
SoftwareSerial btSerial(2,3);



void sender_key_exchange(){
  String tmp;
  randomSeed(analogRead(A5));
  //1
  while(1){
    int flag = 0;
    int p = random(2147483647);
    for(int i = 2; i<sqrt(randnum); i++)
      if(p % i == 0) flag = 1,break;
    if(flag == 0) break;
  }
  int g = random(1, p);
  btSerial.println(p);
  btSerial.println(g);
  //2
  int a = random(10);
  //3
  int A = pow(g, a) % p;
  //5
  btSerial.println(A);
  tmp = btSerial.readString();
  int B = tmp.toInt();
  //6
  sender_key = pow(B,a)%p
  
  
    
    
}


void receiver_key_exchange(){
  String tmp;
  
  //1
  tmp = btSerial.readString()
  int p = tmp.toInt();
  tmp = btSerial.readString()
  int g = tmp.toInt();
  //4
  int b = random(10);
  int B = pow(g, b) % p;
  //5
  btSerial.println(B);
  tmp = btSerial.readString();
  int A = tmp.toInt();
  //6
  receiver_key = pow(A, b) % p;
}



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

void ParshingRead(String Received_data){
//THROTTLE, throttle

  int up_down_start, up_down_end;
  int rl_angle_start, rl_angle_end;
  int fb_angle_start, fb_angle_end;
  int left_button_start, left_button_end;
  int right_button_start, right_button_end;
  String data = Received_data;
  
  fb_angle_start = data.indexOf(":")+1;
  fb_angle_end = data.indexOf(",", fb_angle_start+1);
  
  rl_angle_start = data.indexOf(":", fb_angle_end)+1;
  rl_angle_end = data.indexOf(",", rl_angle_start+1);
  up_down_start = data.indexOf(":", rl_angle_end)+1;
  up_down_end = data.indexOf(",", up_down_start+1);

  left_button_start = data.indexOf(":", up_down_end)+1;
  left_button_end = data.indexOf(":", left_button_start+1);
  right_button_start = data.indexOf(":", left_button_end)+1;
  right_button_end = data.indexOf(":", right_button_start+1);    

  Serial.print(data.substring(up_down_start, up_down_end).toInt());
  Serial.print("   ");
  Serial.print(data.substring(rl_angle_start, rl_angle_end).toInt());
  Serial.print("   ");
  Serial.print(data.substring(fb_angle_start, fb_angle_end).toInt());
  Serial.print("   ");
  Serial.print(data.substring(left_button_start, left_button_end).toInt());
  Serial.print("   ");
  Serial.println(data.substring(right_button_start, right_button_end).toInt());
}



void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i<80; i++){
    key[i] = i;
  }
  
  Serial.begin(9600);
  btSerial.begin(9600);
  pinMode(8, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  check_joystick();
  check_button();

  fb_angle = -39, rl_angle=-29;
  up_down = 150;
  left_turn = 1, right_turn = 1;


  

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

  String received_data = encrypted_data;

  ParshingRead(received_data);

  
  
}
