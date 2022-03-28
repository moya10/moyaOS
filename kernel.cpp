#include <stdint.h>
#include <stddef.h>
#include "include/system.h"
#include "math.cpp"
#include "print.cpp"
#include "memory.cpp"
#include "screen.cpp"
#include "IDT.cpp"
#include "interrupts.cpp"
#include "PageFrameAllocator.cpp"


IDTR idtr;

extern "C" void main(){
 
	idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)system.RequestPage(); 

    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + 0xE * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64_t)(void*)PageFault_Handler);
    interrupt->type_attr = IDT_TA_InterruptGate;
    interrupt->selector = 0x08;

	IDTDescEntry* kb_interrupt = (IDTDescEntry*)(idtr.Offset + 0x21 * sizeof(IDTDescEntry));
    kb_interrupt->SetOffset((uint64_t)(void*)KeyboardInt_Handler);
    kb_interrupt->type_attr = IDT_TA_InterruptGate;
    kb_interrupt->selector = 0x08;

	IDTDescEntry* m_interrupt = (IDTDescEntry*)(idtr.Offset + 0x2C * sizeof(IDTDescEntry));
    m_interrupt->SetOffset((uint64_t)(void*)MouseInt_Handler);
    m_interrupt->type_attr = IDT_TA_InterruptGate;
    m_interrupt->selector = 0x08;

	asm ("lidt %0" : : "m" (idtr));

	uint8_t a1, a2; 

    a1 = system.inportb(PIC1_DATA);
    system.io_wait();
    a2 = system.inportb(PIC2_DATA);
    system.io_wait();

    system.outportb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    system.io_wait();
    system.outportb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    system.io_wait();

    system.outportb(PIC1_DATA, 0x20);
    system.io_wait();
    system.outportb(PIC2_DATA, 0x28);
    system.io_wait();

    system.outportb(PIC1_DATA, 4);
    system.io_wait();
    system.outportb(PIC2_DATA, 2);
    system.io_wait();

    system.outportb(PIC1_DATA, ICW4_8086);
    system.io_wait();
    system.outportb(PIC2_DATA, ICW4_8086);
    system.io_wait();

    system.outportb(PIC1_DATA, a1);
    system.io_wait();
    system.outportb(PIC2_DATA, a2);

	system.init_video();
	system.settextcolor(0x0000FFaa,0x00000011);

	InitPS2Mouse();

    system.outportb(PIC1_DATA, 0b11111000);
    system.outportb(PIC2_DATA, 0b11101111);

    //asm ("sti");

	/* Interrupts */

	// int i = 0;
	// i = 10 / i;
	// system.putch(i);

	// asm("int $0x0e");
    
	/* alphabet */

	char c = 'A';

	while(c <= 'Z')
	{
		system.putch(c);
		c++;
		
	}
		
}
