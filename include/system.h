#ifndef __SYSTEM_H

#include <stdint.h>
#include <stddef.h>

#define __SYSTEM_H


#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1
#define PIC_EOI         0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PS2Leftbutton   0b00000001
#define PS2Middlebutton 0b00000100
#define PS2Rightbutton  0b00000010
#define PS2XSign        0b00010000
#define PS2YSign        0b00100000
#define PS2XOverflow    0b01000000
#define PS2YOverflow    0b10000000

namespace QWERTYKeyboard{

    #define LeftShift   0x2A
    #define RightShift  0x36
    #define Enter       0x1C
    #define BackSpace   0x0E
    #define Spacebar    0x39
    #define Left        0x4B
    #define Right       0x4D
    #define up          0x48
    #define down        0x50
    #define F5          0x3f
    #define F1          0x3b

    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}

void HandleKeyboard(struct regs *r);
void kb_install();


struct Point{
    long X;
    long Y;
};



void InitPS2Mouse();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();

void m_install();
uint16_t get_cursor_position(void);
// void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t colour);
// void ClearMouseCursor(uint8_t* mouseCursor, Point position);
// uint32_t GetPix(uint32_t x, uint32_t y);
// void PutPix(uint32_t x, uint32_t y, uint32_t colour);
void PutChar(const char s, unsigned int xOff, unsigned int yOff,uint32_t colour);
// void move_cursor(Point position);

void SetCursorPosRaw(uint16_t pos);
void SetCursorPos(int x, int y);
void kprint(const char* s);
void kprintChar(const char c, bool caps);
void MoveCursorLR(int i);
void MoveCursorUD(int i);
void ClearScreen(int col);
void ColLine(int line, int col);
void kprintCol(const char* s, int col);
void printError(const char* s);
void ClrLine(int line);
// void ColScreen(int col);
void scrollPageUp();
void load_shell();
void parseCommand();


class System {
    public:

    
    size_t Size;
    uint8_t* Buffer;
    bool LOAD_SHELL;
    bool operator[](uint64_t index);
    bool Set(uint64_t index, bool value);

    void LockPage(void* address);
    void* RequestPage();
    
    
    int abs(int num);
    char* itoa(int num, char* buffer);
    char* int2String(int num, char intStr[128]);
    const char* to_hstring(uint64_t value);
    const char* to_hstring(uint32_t value);
    const char* to_hstring(uint16_t value);
    const char* to_hstring(uint8_t value);
    char* peek(char a);
    int mult(int x, int y);
    int sqrt(int x, int n);
    int division(int x, int y);    
    bool StringStartsWith(const char* s, const char* t);
    bool StringsEqu(const char* s, const char* t);
    char* removeChar(char *str, char *garbage); 
    void *memcpy(void *dest, const void *src, size_t count);
    void *memset(void *dest, char val, size_t count);
    unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
    int strlen(const char* str);
    uint8_t inb (unsigned short _port);
    void outb (uint16_t _port, uint8_t _data);
    void io_wait();
    // void scroll(void);
    // void move_csr(void);
    // void cls();
    // void putch(const char c);
    // void puts(const char *text);
    // void settextcolor(unsigned char forecolor, unsigned char backcolor);
    // void init_video(void);


};

extern System system;

#endif