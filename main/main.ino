#include "define.h"     //CONTROLのために必要
#include "Controller.h"   
#include <MsTimer2.h>

bool flag_10ms =false;
Controller RECEIVE;  // ()を付けるとうまくいかないので注意　setup()の外で宣言


void time(){
  flag_10ms = true;
}

void setup() {
  Serial.begin(115200);
  CONTROL.begin(115200);   //CONTROL　は　define.h　参照
  Serial.print("ready...\n");
  
  MsTimer2::set(10,time); 
  MsTimer2::start();
}

void loop(){
  if(flag_10ms){
    RECEIVE.update();     //繰り返し処理の中に置いておく必要がある．これやらないとボタンの情報が更新されない
    RECEIVE.statePrint(); //受信した(checksumを通った)値をprintするもの．多分使わない．

    delay(1);
    if(RECEIVE.readButton(BUTTON_UP)==2) Serial.println("open");   //押した瞬間にprintf  BUTTON_UPは1にしても良い（define参考に）
    if(RECEIVE.readButton(BUTTON_UP)==-1) Serial.println("close"); //放した瞬間にprintf
    delay(1);

    flag_10ms=false;
  }

}

