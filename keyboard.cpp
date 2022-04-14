#include "include/system.h"
#include "kbScancodeTranslator.cpp"
#include "CPU/irq.h"


bool shift_pressed = false;
bool caps_lock = false;

void HandleKeyboard(struct regs *r){
    uint8_t scancode = system.inb(0x60);
    switch (scancode){
        case LeftShift:
            shift_pressed = true; 
            return;
        case LeftShift + 0x80: // release
            shift_pressed = false; 
            return;
        case RightShift:
            shift_pressed = true; 
            return;
        case RightShift + 0x80: // release
            shift_pressed = false; 
            return;
        case Enter: 
            if(system.LOAD_SHELL) parseCommand();
            else kprintChar(10,0);
    		return;
        case F5: 
            if(system.LOAD_SHELL) load_shell(); 
            return;
        case F1:
            if(system.LOAD_SHELL) m_install();
            return;
        case Spacebar:
            kprintChar(' ', 0);
            return;
        case BackSpace:
            kprintChar(8, 0);
           return;
    	case 0x3a: 
            caps_lock = !caps_lock; 
            return;
        case Left: 
            MoveCursorLR(-1); 
            return;
    	case Right: 
            MoveCursorLR(1); 
            return;
    	case up: 
            MoveCursorUD(-1); 
            return;
    	case down: 
            MoveCursorUD(1); 
            return;
        default: 
            kprintChar(QWERTYKeyboard::Translate(scancode, shift_pressed | caps_lock), shift_pressed | caps_lock);
    }

}

void kb_install(){
    irq_install_handler(1, HandleKeyboard);

    // PIC MASTER
    system.outb(PIC1_COMMAND, PIC_EOI);
}