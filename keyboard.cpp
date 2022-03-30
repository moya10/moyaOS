#include "include/system.h"
#include "kbScancodeTranslator.cpp"
#include "CPU/irq.h"

bool isLeftShiftPressed;
bool isRightShiftPressed;

void HandleKeyboard(struct regs *r){
    uint8_t scancode = system.inb(0x60);
    switch (scancode){
        case LeftShift:
            isLeftShiftPressed = true;
            return;
        case LeftShift + 0x80:
            isLeftShiftPressed = false;
            return;
        case RightShift:
            isRightShiftPressed = true;
            return;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            return;
        case Enter:
            system.putch('\n');
            return;
        case Spacebar:
            system.putch(' ');
            return;
        case BackSpace:
            system.putch(0x08);
           return;
    }

    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0){
        system.putch(ascii);
    }

}

void kb_install(){
    irq_install_handler(1, HandleKeyboard);

    // PIC MASTER
    system.outb(PIC1_COMMAND, PIC_EOI);
}