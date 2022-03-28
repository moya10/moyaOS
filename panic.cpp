#include <stddef.h>
#include <stdint.h>
#include "include/system.h"

void System::Panic(const char* panicMessage){
    system.init_video();
	system.settextcolor(0x0000FFaa,0x00000011);
    system.puts("Kernel Panic Screen");
    system.puts(panicMessage);
}