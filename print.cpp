#include <stddef.h>
#include <stdint.h>
#include "include/system.h"

#define DISPLAY_SIZE 2000 // 2000 = 80 x 25 Characters - VGA Text-mode Display size

void System::Print(char text[])
{
	static unsigned int CurrentLocation = 0;
	
	unsigned short* DisplayMemoryPtr = (unsigned short*)0xB8000;
	unsigned char BackgroundColour = 0xaacceb7d; 
	unsigned char ForegroundColour = 0x00000000; 
	unsigned char Colour = ((BackgroundColour) & 0xF0) | (ForegroundColour >> 4 & 0x08); // Calculate combined colour value
	
	
	int i;
	for(i = 0; text[i] != 0; i++)
	{
		if(CurrentLocation == DISPLAY_SIZE) 
		{
			CurrentLocation -= 80;
		}
		
		if(text[i] == '\n') 
		{
			CurrentLocation += (80 - (CurrentLocation % 80));
		}
        else if(text[i] == '\t')
        {
            int spaces;
            for(spaces = 0; spaces < 4;spaces++){
                if(CurrentLocation == DISPLAY_SIZE){
                    CurrentLocation -=80;
                }
                DisplayMemoryPtr[CurrentLocation++] = (((unsigned short)Colour) << 8) | ' ';
            }
        }else if(text[i] == '\b'){
            if(CurrentLocation > 0){
                DisplayMemoryPtr[--CurrentLocation] = (((unsigned short)Colour) << 8) | ' ';
            }
        }
		else
		{
			DisplayMemoryPtr[CurrentLocation++] = (((unsigned short)Colour) << 8) | text[i];
		}
	}
}

void System::PrintLine(char text[])
{
    Print("\n");
	Print(text);
}

void System::putchar(int xOff, int yOff)
{
	int Colour = 0xeeffdacb;	
    int* pixPtr = (int*)0xb8000;
	*(int*)(pixPtr + xOff + yOff) = Colour;
}

void System::drawVerticalLine(int xOff, int start_itr, int end_itr){
	int y = 40*start_itr;
	if((40*end_itr)<=960){
		while(y<=40*end_itr){
			putchar(xOff,y);
			y = y + 40;			
		}
	}else{
		while(y<=960){
			putchar(xOff,y);
			y = y + 40;			
		}
	}
}

void System::drawHorizontalLine(int start_itr, int end_itr, int yOff){
	for(int x=start_itr;x<=end_itr;x++){
		putchar(x,yOff);
	}
}

void System::drawDiagonicalLine(int start_itr, int start, int end){
	int y = 40*start;
	int x = start_itr;
	if(start <= end){
		while(y<=40*end){	
			putchar(x,y);
			x = x+1;
			y = y + 40;	
		}
	}else{
		while(y>=40*end){
			putchar(x,y);
			x = x+1;
			y = y - 40;	
		}
	}
}


void System::drawCircle(int x, int y, int r){
	for(int i=-r;i<=r;i++){
		drawHorizontalLine(x-sqrt((mult(r,r)-mult(i,i)),(mult(r,r)-mult(i,i))),x+sqrt((mult(r,r)-mult(i,i)),(mult(r,r)-mult(i,i))),y+i);
	}
}
