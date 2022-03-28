#include <stddef.h>
#include <stdint.h>
#include "include/system.h"
#include "panic.cpp"
#include "keyboard.cpp"
#include "mouse.cpp"

System system;
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame){
    system.Panic("Page Fault Detected");
    while(true);
}


__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame){

    uint8_t mouseData = system.inportb(0x60);

    HandlePS2Mouse(mouseData);

    // PIC SLAVE
    system.outportb(PIC2_COMMAND, PIC_EOI);
    system.outportb(PIC1_COMMAND, PIC_EOI);
}


__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame){
    uint8_t scancode = system.inportb(0x60);

    system.HandleKeyboard(scancode);
    // PIC MASTER
    system.outportb(PIC1_COMMAND, PIC_EOI);
}