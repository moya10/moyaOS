#include "include/system.h"
#include "CPU/irq.h"

#define VGA_WIDTH 80

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
        if ((system.inb(0x64) & 0b10) == 0){
            return;
        }
    }
}

void MouseWaitInput(){
    uint64_t timeout = 100000;
    while (timeout--){
        if (system.inb(0x64) & 0b1){
            return;
        }
    }
}

void MouseWrite(uint8_t value){
    MouseWait();
    system.outb(0x64, 0xD4);
    MouseWait();
    system.outb(0x60, value);
}

uint8_t MouseRead(){
    MouseWaitInput();
    return system.inb(0x60);
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
Point MousePosition;
Point MousePositionOld;

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    system.outb(0x3D4, 0x0F);
    pos |= system.inb(0x3D5);
    system.outb(0x3D4, 0x0E);
    pos |= ((uint16_t)system.inb(0x3D5)) << 8;
    return pos;
}

void HandlePS2Mouse(struct regs *r){

    uint8_t data = system.inb(0x60);
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
void move_cursor(Point position)
{
    unsigned temp;

    temp = position.Y * 80 + position.X;

    system.outb(0x3D4, 14);
    system.outb(0x3D5, temp >> 8);
    system.outb(0x3D4, 15);
    system.outb(0x3D5, temp);
}

// void PutPix(uint32_t x, uint32_t y, uint32_t colour){
//     *(uint32_t*)( (x) + (y * 80)) = (((0x01 << 4) | (colour & 0x0F)) << 8);
// }

// uint32_t GetPix(uint32_t x, uint32_t y){
//     return *(uint32_t*)((x) + (y * 80));
// }

void PutChar(char chr, unsigned int xOff, unsigned int yOff, uint32_t colour)
{
    unsigned short *where;
    
    where = (unsigned short *)0xB8000 + (yOff * 80 + xOff);            
    *where = chr | (((BAR_COLOR << 4) | (colour & 0x0F)) << 8);	/* Character AND attributes: color */

}

// uint32_t MouseCursorBuffer[16 * 16];
// uint32_t MouseCursorBufferAfter[16 * 16];
// bool MouseDrawn;

// void ClearMouseCursor(uint8_t* mouseCursor, Point position){
//     if (!MouseDrawn) return;

//     int xMax = 16;
//     int yMax = 16;
//     int differenceX = get_cursor_position() % VGA_WIDTH - position.X;
//     int differenceY = get_cursor_position() / VGA_WIDTH - position.Y;

//     if (differenceX < 16) xMax = differenceX;
//     if (differenceY < 16) yMax = differenceY;

//     for (int y = 0; y < yMax; y++){
//         for (int x = 0; x < xMax; x++){
//             int bit = y * 16 + x;
//             int byte = bit / 8;
//             if ((mouseCursor[byte] & (0b10000000 >> (x % 8))))
//             {
//                 if (GetPix(position.X + x, position.Y + y) == MouseCursorBufferAfter[x + y *16]){
//                     PutPix(position.X + x, position.Y + y, MouseCursorBuffer[x + y * 16]);
//                 }
//             }
//         }
//     }
// }

// void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t colour){


//     int xMax = 16;
//     int yMax = 16;
//     int differenceX = get_cursor_position() % VGA_WIDTH - position.X;
//     int differenceY = get_cursor_position() / VGA_WIDTH - position.Y;

//     if (differenceX < 16) xMax = differenceX;
//     if (differenceY < 16) yMax = differenceY;

//     for (int y = 0; y < yMax; y++){
//         for (int x = 0; x < xMax; x++){
//             int bit = y * 16 + x;
//             int byte = bit / 8;
            
//             if ((mouseCursor[byte] & (0b10000000 >> (x % 8))))
//             {
//                 MouseCursorBuffer[x + y * 16] = GetPix(position.X + x, position.Y + y);
//                 PutPix(position.X + x, position.Y + y, colour);
                
//                 MouseCursorBufferAfter[x + y * 16] = GetPix(position.X + x, position.Y + y);

//             }
//         }
//     }

//     MouseDrawn = true;
// }



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
        if (MousePosition.X > 120) MousePosition.X = get_cursor_position() % VGA_WIDTH; 
        
        if (MousePosition.Y < 0) MousePosition.Y = 0;
        if (MousePosition.Y > 120) MousePosition.Y = get_cursor_position() / VGA_WIDTH; 
        
        // ClearMouseCursor(MousePointer, MousePositionOld);
        // DrawOverlayMouseCursor(MousePointer, MousePosition, 0x0C);
        move_cursor(MousePosition);

        if (MousePacket[0] & PS2Leftbutton){
            PutChar('@', MousePosition.X, MousePosition.Y,0x0B);
            //kprintCol("@",0x0B);
        }
        if (MousePacket[0] & PS2Middlebutton){
            PutChar('O', MousePosition.X, MousePosition.Y,0x0C);
        }
        if (MousePacket[0] & PS2Rightbutton){
            PutChar('$', MousePosition.X, MousePosition.Y,0x0D);
        }

        MousePacketReady = false;
        MousePositionOld = MousePosition;
}

void InitPS2Mouse(){
 
    system.outb(0x64, 0xA8); //enabling the auxiliary device - mouse

    MouseWait();
    system.outb(0x64, 0x20); //tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
    uint8_t status = system.inb(0x60);
    status |= 0b10;
    MouseWait();
    system.outb(0x64, 0x60);
    MouseWait();
    system.outb(0x60, status); // setting the correct bit is the "compaq" status byte

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
}

void m_install(){
    irq_install_handler(12, HandlePS2Mouse);

    // PIC SLAVE
    system.outb(PIC2_COMMAND, PIC_EOI);
    system.outb(PIC1_COMMAND, PIC_EOI);

    InitPS2Mouse();
}