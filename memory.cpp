#include <stddef.h>
#include <stdint.h>
#include "include/system.h"

void *System::memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *System::memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *System::memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}


uint8_t System::inb (unsigned short _port)
{
    uint8_t res;
	asm volatile("inb %1, %0" : "=a"(res) : "Nd"(_port));
	return res;
}

void System::outb (unsigned short _port, unsigned char _data)
{
    asm volatile("outb %0, %1" : : "a"(_data), "Nd"(_port));
	return;
}

void System::io_wait(){
    asm volatile("outb %%al, $0x80" : : "a"(0));
}