#ifndef __SYSTEM_H
#include <stdint.h>
#define __SYSTEM_H

#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

struct IDTDescEntry {
    uint16_t offset0; 
    uint16_t selector;  
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset1;
    uint32_t offset2;
    uint32_t ignore;
    void SetOffset(uint64_t offset);
    uint64_t GetOffset();
};

struct IDTR {
    uint16_t Limit;
    uint64_t Offset;
} __attribute__((packed));


#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

namespace QWERTYKeyboard{

    #define LeftShift 0x2A
    #define RightShift 0x36
    #define Enter 0x1C
    #define BackSpace 0x0E
    #define Spacebar 0x39

    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}

struct Point{
    long X;
    long Y;
};

struct interrupt_frame;
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame);

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000100
#define PS2Rightbutton 0b00000010
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

extern uint8_t MousePointer[];

void InitPS2Mouse();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();
extern Point MousePosition;

class System {
    public:
    void Panic(const char* panicMessage);

    size_t Size;
    uint8_t* Buffer;
    bool operator[](uint64_t index);
    bool Set(uint64_t index, bool value);

    void LockPage(void* address);
    void* RequestPage();
    
    void Print(char text[]);
    void PrintLine(char text[]);
    void putchar(int xOff, int yOff);
    void drawVerticalLine(int xOff, int start_itr, int end_itr);
    void drawHorizontalLine(int start_itr, int end_itr, int yOff);
    void drawDiagonicalLine(int start_itr, int start, int end);
    void drawCircle(int x, int y, int r);
    int abs(int num);
    char* itoa(int num, char* buffer);
    char* int2String(int num, char intStr[128]);
    char* hex2string(uint64_t num, char hexStr[128]);
    char* peek(char a);
    int mult(int x, int y);
    int sqrt(int x, int n);
    int division(int x, int y);

    void *memcpy(void *dest, const void *src, size_t count);
    void *memset(void *dest, char val, size_t count);
    unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
    size_t strlen(const char *str);
    unsigned char inportb (unsigned short _port);
    void outportb (unsigned short _port, unsigned char _data);
    void io_wait();
    void scroll(void);
    void move_csr(void);
    void cls();
    void putch(const char c);
    void puts(const char *text);
    void settextcolor(unsigned char forecolor, unsigned char backcolor);
    void init_video(void);

    void HandleKeyboard(uint8_t scancode);

};

extern System system;

#endif