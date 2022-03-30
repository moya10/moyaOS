#include "include/system.h"
#include "Bitmap.cpp"

uint64_t freeMemory;
uint64_t usedMemory;

void System::LockPage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    if (system[index] == true) return;
    if (system.Set(index, true)){
        freeMemory -= 4096;
        usedMemory += 4096;
    }
}
void* System::RequestPage(){
    for (uint64_t pageBitmapIndex = 0; pageBitmapIndex < system.Size * 8; pageBitmapIndex++){
        if (system[pageBitmapIndex] == true) continue;
        LockPage((void*)(pageBitmapIndex * 4096));
        return (void*)(pageBitmapIndex * 4096);
    }
    return NULL; // Page Frame Swap to file
}
