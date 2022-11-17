#include <SoftwareSerial.h>
#include <AESLib.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
const byte address[6] = "00001";

void ParshingRead(){
//THROTTLE, throttle

int up_down_start, up_down_end;
int rl_angle_start, rl_angle_end;
int fb_angle_start, fb_angle_end;
int left_button_start, left_button_end;
int right_button_start, right_button_end;
String data;
char data2[150];

  if(radio.available()){
    radio.read(&data2, sizeof(data2));
  }
    aes256_dec_single(key, data2);
    data = (String)data2;
    
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


void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN); 
  radio.startListening();
  Serial.write("serial begin\n");
}

void loop()
{
  delay(50);
  ParshingRead();
}
