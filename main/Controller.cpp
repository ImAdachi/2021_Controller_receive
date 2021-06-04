#include <Arduino.h>
#include "define.h"
#include "Controller.h"

Controller::Controller(){
    conData.ButtonState=0;
    conData.RJoyX=127, conData.RJoyY=127, conData.LJoyX=127, conData.LJoyY=127;
}

void Controller::update(){
  char checksum = 0x00,receive_data[8];
  int loop_count=0;
  comCheck = false;
  
    while(loop_count<10 && CONTROL.available()){    
            if(serial_recieve()==10){

                for (int i=0;i<8;i++) receive_data[i] = serial_recieve();
                for (int i=0;i<8;i++) receive_data[i] -= 0x20;
                for(int i=0;i<7;i++)checksum ^=receive_data[i];

                if(receive_data[7]==checksum){

                    comCheck = true;
                
                    pre_conData.ButtonState = conData.ButtonState;
                    conData.ButtonState = ((receive_data[0] & 0x3F) << 2) | ((receive_data[1] & 0x30) >> 4);

                    conData.RJoyX = ((receive_data[1] & 0x0F) << 4) | ((receive_data[2] & 0x3C) >> 2) ;
                    conData.RJoyY = ((receive_data[2] & 0x03) << 6) | ( receive_data[3] & 0x3F);
                    conData.LJoyX = ((receive_data[4] & 0x3F) << 2) | ((receive_data[5] & 0x30) >> 4);
                    conData.LJoyY = ((receive_data[5] & 0x0F) << 4) | (receive_data[6] & 0x0F);
                    
                    break;
                }
            }
     loop_count++;
    }

}

void Controller::statePrint()
{
    Serial.print(comCheck);
    Serial.print("\t");
    Serial.print(conData.ButtonState);
    Serial.print("\t");
    Serial.print(conData.RJoyX);
    Serial.print("\t");
    Serial.print(conData.RJoyY);
    Serial.print("\t");
    Serial.print(conData.LJoyX);
    Serial.print("\t");
    Serial.println(conData.LJoyY);
}

bool Controller::readButton_bin(unsigned int ButtonNum){//放しているときは０，押しているときは１
    return ((conData.ButtonState & (0x0001 << (ButtonNum - 1))) == (0x0001 << (ButtonNum - 1)))? true:false;
    }
    
int Controller::readButton(unsigned int ButtonNum){//放しているときは０，押しているときは１，押した瞬間は２，放した瞬間は－１
    int result = 0;
    if((conData.ButtonState & (0x0001 << (ButtonNum - 1))) == (0x0001 << (ButtonNum - 1))) result += 2;
    if((pre_conData.ButtonState & (0x0001 << (ButtonNum - 1))) == (0x0001 << (ButtonNum - 1)))result -= 1;
    return result;
    }

unsigned int Controller::getButtonState(){
    return conData.ButtonState;
    }

ControllerData Controller::getConData(){
    return conData;
    }

double Controller::readJoyRX()
{   
    if(conData.RJoyX==127)return 0;
    return ((double)conData.RJoyX-127.5)/127.5;
}

double Controller::readJoyRY()
{
    if(conData.RJoyY==127)return 0;
    return ((double)conData.RJoyY-127.5)/127.5;
}

double Controller::readJoyLX()
{
    if(conData.LJoyX==127)return 0;
    return ((double)conData.LJoyX-127.5)/127.5;
}

double Controller::readJoyLY()
{
    if(conData.LJoyY==127)return 0;
    return ((double)conData.LJoyY-127.5)/127.5;
}
    
byte Controller::readJoyRXbyte()
{
    return conData.RJoyX;
}
    
byte Controller::readJoyRYbyte()
{
    return conData.RJoyY;
}

byte Controller::readJoyLXbyte()
{
    return conData.LJoyX;
}
    
byte Controller::readJoyLYbyte()
{
    return conData.LJoyY;
}