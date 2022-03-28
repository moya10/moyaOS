#include "include/system.h"
#include "kbScancodeTranslator.cpp"

bool isLeftShiftPressed;
bool isRightShiftPressed;

void System::HandleKeyboard(uint8_t scancode){

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
            putch(65);
            return;
        case Spacebar:
            putch(' ');
            return;
        case BackSpace:
            puts("error");
           return;
    }

    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0){
        putch(ascii);
    }

}