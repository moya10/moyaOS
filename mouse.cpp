#include "include/system.h"

uint8_t MousePointer[] = {
    0b11111111, 0b11100000, 
    0b11111111, 0b10000000, 
    0b11111110, 0b00000000, 
    0b11111100, 0b00000000, 
    0b11111000, 0b00000000, 
    0b11110000, 0b00000000, 
    0b11100000, 0b00000000, 
    0b11000000, 0b00000000, 
    0b11000000, 0b00000000, 
    0b10000000, 0b00000000, 
    0b10000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
};

void MouseWait(){
    uint64_t timeout = 100000;
    while (timeout--){
        if ((system.inportb(0x64) & 0b10) == 0){
            return;
        }
    }
}

void MouseWaitInput(){
    uint64_t timeout = 100000;
    while (timeout--){
        if (system.inportb(0x64) & 0b1){
            return;
        }
    }
}

void MouseWrite(uint8_t value){
    MouseWait();
    system.outportb(0x64, 0xD4);
    MouseWait();
    system.outportb(0x60, value);
}

uint8_t MouseRead(){
    MouseWaitInput();
    return system.inportb(0x60);
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
Point MousePosition;
Point MousePositionOld;

void HandlePS2Mouse(uint8_t data){

    ProcessMousePacket();
    static bool skip = true;
    if (skip) { skip = false; return; }

    switch(MouseCycle){
        case 0:
           
            if ((data & 0b00001000) == 0) break;
            MousePacket[0] = data;
            MouseCycle++;
            break;
        case 1:
           
            MousePacket[1] = data;
            MouseCycle++;
            break;
        case 2:
            
            MousePacket[2] = data;
            MousePacketReady = true;
            MouseCycle = 0;
            break;
    }
}

void ProcessMousePacket(){
    if (!MousePacketReady) return;

        bool xNegative, yNegative, xOverflow, yOverflow;

        if (MousePacket[0] & PS2XSign){
            xNegative = true;
        }else xNegative = false;

        if (MousePacket[0] & PS2YSign){
            yNegative = true;
        }else yNegative = false;

        if (MousePacket[0] & PS2XOverflow){
            xOverflow = true;
        }else xOverflow = false;

        if (MousePacket[0] & PS2YOverflow){
            yOverflow = true;
        }else yOverflow = false;

        if (!xNegative){
            MousePosition.X += MousePacket[1];
            if (xOverflow){
                MousePosition.X += 255;
            }
        } else
        {
            MousePacket[1] = 256 - MousePacket[1];
            MousePosition.X -= MousePacket[1];
            if (xOverflow){
                MousePosition.X -= 255;
            }
        }

        if (!yNegative){
            MousePosition.Y -= MousePacket[2];
            if (yOverflow){
                MousePosition.Y -= 255;
            }
        } else
        {
            MousePacket[2] = 256 - MousePacket[2];
            MousePosition.Y += MousePacket[2];
            if (yOverflow){
                MousePosition.Y += 255;
            }
        }

        if (MousePosition.X < 0) MousePosition.X = 0;
        if (MousePosition.X > 50) MousePosition.X = 2000-1; //dummy
        
        if (MousePosition.Y < 0) MousePosition.Y = 0;
        if (MousePosition.Y > 50) MousePosition.Y = 2000-1; // dummy
        
        // GlobalRenderer->ClearMouseCursor(MousePointer, MousePositionOld);
        // GlobalRenderer->DrawOverlayMouseCursor(MousePointer, MousePosition, 0x00daf7a6);

        if (MousePacket[0] & PS2Leftbutton){
            system.putch('R');
        }
        if (MousePacket[0] & PS2Middlebutton){
        }
        if (MousePacket[0] & PS2Rightbutton){
            system.putch('B');
        }

        MousePacketReady = false;
        MousePositionOld = MousePosition;
}

void InitPS2Mouse(){
 
    system.outportb(0x64, 0xA8); //enabling the auxiliary device - mouse

    MouseWait();
    system.outportb(0x64, 0x20); //tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
    uint8_t status = system.inportb(0x60);
    status |= 0b10;
    MouseWait();
    system.outportb(0x64, 0x60);
    MouseWait();
    system.outportb(0x60, status); // setting the correct bit is the "compaq" status byte

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
}