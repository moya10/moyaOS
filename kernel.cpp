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
    
	/* Get Connected Devices */
	ACPI::SDTHeader* xsdt = (ACPI::SDTHeader*)(0x40E); // 2 byte real mode segment pointer located within the first 1 KB of the EBDA (Extended BIOS Data Area)
    ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");
    PCI::EnumeratePCI(mcfg);

	/* Test Interrupts */

	// int i = 0;
	// i = 10 / i;       //// Division by zero
	// system.putch(i);

	// asm("int $0x0e"); //// Page fault


	/* alphabet */

	char c = 'A';

	while(c <= 'Z')
	{
		system.putch(c);
		c++;
		
	}
		
}
