#include <Arduino.h>
#include "define.h"
#include "Controller.h"

Controller::Controller(){
    ButtonState=0;
    RJoyX=127, RJoyY=127, LJoyX=127, LJoyY=127;
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
                
                    preButtonState = ButtonState;
                    ButtonState = ((receive_data[0] & 0x3F) << 2) | ((receive_data[1] & 0x30) >> 4);

                    RJoyX = ((receive_data[1] & 0x0F) << 4) | ((receive_data[2] & 0x3C) >> 2) ;
                    RJoyY = ((receive_data[2] & 0x03) << 6) | ( receive_data[3] & 0x3F);
                    LJoyX = ((receive_data[4] & 0x3F) << 2) | ((receive_data[5] & 0x30) >> 4);
                    LJoyY = ((receive_data[5] & 0x0F) << 4) | (receive_data[6] & 0x0F);
                    
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
    Serial.print(ButtonState);
    Serial.print("\t");
    Serial.print(RJoyX);
    Serial.print("\t");
    Serial.print(RJoyY);
    Serial.print("\t");
    Serial.print(LJoyX);
    Serial.print("\t");
    Serial.println(LJoyY);
}

bool Controller::readButton_bin(unsigned int ButtonNum){//放しているときは０，押しているときは１
    return ((ButtonState & (0x0001 << (ButtonNum - 1))) == (0x0001 << (ButtonNum - 1)))? true:false;
    }
    
int Controller::readButton(unsigned int ButtonNum){//放しているときは０，押しているときは１，押した瞬間は２，放した瞬間は－１
    int result = 0;
    if((ButtonState & (0x0001 << (ButtonNum - 1))) == (0x0001 << (ButtonNum - 1))) result += 2;
    if((preButtonState & (0x0001 << (ButtonNum - 1))) == (0x0001 << (ButtonNum - 1)))result -= 1;
    return result;
    }

unsigned int Controller::getButtonState(){
    return ButtonState;
    }

double Controller::readJoyRX()
{   
    if(RJoyX==127)return 0;
    return ((double)RJoyX-127.5)/127.5;
}

double Controller::readJoyRY()
{
    if(RJoyY==127)return 0;
    return ((double)RJoyY-127.5)/127.5;
}

double Controller::readJoyLX()
{
    if(LJoyX==127)return 0;
    return ((double)LJoyX-127.5)/127.5;
}

double Controller::readJoyLY()
{
    if(LJoyY==127)return 0;
    return ((double)LJoyY-127.5)/127.5;
}
    
uint8_t Controller::readJoyRXbyte()
{
    return RJoyX;
}
    
uint8_t Controller::readJoyRYbyte()
{
    return RJoyY;
}

uint8_t Controller::readJoyLXbyte()
{
    return LJoyX;
}
    
uint8_t Controller::readJoyLYbyte()
{
    return LJoyY;
}