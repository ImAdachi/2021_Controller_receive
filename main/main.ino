/*
 *IDE for GR 1.12
 *ボード：GR-PEACH(OpenCV)  /  GR-SAKURA
 */

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
  Serial.println("ready...");
  
  MsTimer2::set(10,time); 
  MsTimer2::start();
}

void loop(){
  if(flag_10ms){
    if(RECEIVE.update()){     //繰り返し処理の中に置いておく必要がある．これやらないとボタンの情報が更新されない.
    RECEIVE.statePrint(); //受信した(checksumを通った)値をprintするもの．多分使わない．
    }

    delay(1);
    if(RECEIVE.readButton(BUTTON_UP)== 2) Serial.println("open");   //押した瞬間にprintf  BUTTON_UPは1にしても良い（define参考に）
    if(RECEIVE.readButton(BUTTON_UP)==-1) Serial.println("close"); //放した瞬間にprintf
    
    /*  //↓構造体で受け取れているかの確認用．上のstatePrint()と内容が被るので見にくくなる．
    ControllerData myConData = RECEIVE.getConData();  //構造体 ControllerData は Controller.h の中にある
    Serial.print(myConData.ButtonState);
    Serial.print(", ");
    Serial.print(myConData.RJoyX);
    Serial.print(", ");
    Serial.print(myConData.RJoyY);
    Serial.print(", ");
    Serial.print(myConData.LJoyX);
    Serial.print(", ");
    Serial.print(myConData.LJoyY);
    Serial.print("  ");
    */
    
    

    delay(1);

    flag_10ms=false;
  }

}

