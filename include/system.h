#ifndef __SYSTEM_H

#include <stdint.h>
#include <stddef.h>

#define __SYSTEM_H
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000100
#define PS2Rightbutton 0b00000010
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

namespace QWERTYKeyboard{

    #define LeftShift 0x2A
    #define RightShift 0x36
    #define Enter 0x1C
    #define BackSpace 0x0E
    #define Spacebar 0x39

    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}

void HandleKeyboard(struct regs *r);
void kb_install();


struct Point{
    long X;
    long Y;
};

extern uint8_t MousePointer[];

void InitPS2Mouse();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();
extern Point MousePosition;
void m_install();
uint16_t get_cursor_position(void);
void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t colour);
void ClearMouseCursor(uint8_t* mouseCursor, Point position);
uint32_t GetPix(uint32_t x, uint32_t y);
void PutPix(uint32_t x, uint32_t y, uint32_t colour);
void PutChar(char chr, unsigned int xOff, unsigned int yOff);

class System {
    public:

    
    size_t Size;
    uint8_t* Buffer;
    bool operator[](uint64_t index);
    bool Set(uint64_t index, bool value);

    void LockPage(void* address);
    void* RequestPage();
    
    
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
    uint8_t inb (unsigned short _port);
    void outb (unsigned short _port, unsigned char _data);
    void io_wait();
    void scroll(void);
    void move_csr(void);
    void cls();
    void putch(const char c);
    void puts(const char *text);
    void settextcolor(unsigned char forecolor, unsigned char backcolor);
    void init_video(void);


};

extern System system;

#endif