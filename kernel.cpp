#include <stdint.h>
#include <stddef.h>
#include "include/system.h"
#include "math.cpp"
#include "memory.cpp"
#include "screen.cpp"
#include "CPU/idt.h"
#include "CPU/isr.h"
#include "CPU/irq.h"
#include "CPU/timer.h"
#include "keyboard.cpp"
#include "mouse.cpp"

extern "C" void main(){
    System system;


	system.init_video();
	system.settextcolor(0x0A,0x01);

    idt_install();
	isrs_install();
	irq_install();
	asm volatile ("sti");
	timer_install();

    kb_install();
    m_install();
    



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
