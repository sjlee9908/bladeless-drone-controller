//https://rasino.tistory.com/255
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // SPI통신을 위한 (CE, CSN) 핀 선언
const byte address[6] = "00001"; // 송신기와 수신기가 동일한 값으로 주소 설정함(5자리)
int throttle;
int roll;
int pitch;
int yaw_left;
int yaw_right;


void ParshingRead(){
//THROTTLE, throttle

int throttle_start, throttle_end;
int roll_start, roll_end;
int pitch_start, pitch_end;
int yaw_start, yaw_end;
char text[32];

  if(radio.available()){
    radio.read(&text, sizeof(text));
    String data= text;
    Serial.println(text);
    pitch_start = data.indexOf(" ")+1;
    pitch_end = data.indexOf(" ", pitch_start+1);
    
    roll_start = data.indexOf(" ", pitch_end)+1;
    roll_end = data.indexOf(" ", roll_start+1);

    throttle_start = data.indexOf(" ", roll_end)+1;
    throttle_end = data.indexOf(" ", throttle_start+1);

    yaw_start = data.indexOf(" ", throttle_end)+1;
    yaw_end = data.indexOf(" ", yaw_end+1);

    throttle = data.substring(throttle_start, throttle_end).toInt();
    roll = data.substring(roll_start, roll_end).toInt();
    pitch = data.substring(pitch_start, pitch_end).toInt();
    yaw_left = data.substring(yaw_start, yaw_end).toInt();
    }
}

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address); // 데이터를 받을 송신기 주소를 설정
  radio.setPALevel(RF24_PA_HIGH); // 송신거리에 따른, 전원공급 파워레벨 설정
                                 //(최소) RF24_PA_MIN → RF24_PA_LOW → RF24_PA_HIGH → RF24_PA_MAX (최대) 설정가능
                                  //송신이 약하다고 판단될 경우 nRF24모듈의 GND와 3.3V 단자에 전해콘덴서(10uF이상:+를3.3V연결)사용권장
  radio.startListening(); // 모듈을 수신기(상태)로 설정
}

void loop() {
  ParshingRead();
  Serial.print("throttle: ");
  Serial.print(throttle);
  Serial.print("    roll: ");
  Serial.print(roll);
  Serial.print("    pitch: ");
  Serial.print(pitch);
  Serial.print("    yaw_left: ");
  Serial.print(yaw_left);
  Serial.print("    yaw_right: ");
  Serial.println(yaw_right);
}
