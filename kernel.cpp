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
#include "acpi.cpp"
#include "pci.cpp"

extern const unsigned short MemSize; 
extern const char moya[];
extern "C" void main(){
    System system;


    idt_install();
	isrs_install();
	irq_install();

	asm volatile ("sti");
	timer_install();
	// m_install();
    kb_install();
	
	kprintCol(moya,0x0D);
	sleep(100);
	load_shell();

	
	return;
}

