#include <stdint.h>
#define DISPLAY_SIZE 2000 // 2000 = 80 x 25 Characters - VGA Text-mode Display size

void ShiftDisplayUpOneLine()
{
	unsigned short* DisplayMemoryPtr = (unsigned short*)0xB8000;
	unsigned char BackgroundColour = 0x06; // Yellow background - distinct colour from the assembly code
	unsigned char ForegroundColour = 0x00; // Black text
	unsigned char Colour = ((BackgroundColour << 4) & 0xF0) | (ForegroundColour & 0x0F); // Calculate combined colour value
	unsigned int i;
	for(i = 0; i < DISPLAY_SIZE - 80; i++) 
	{
		DisplayMemoryPtr[i] = DisplayMemoryPtr[i + 80];
	}
	for(i = DISPLAY_SIZE - 1; i > DISPLAY_SIZE - 80; i--)
	{
		DisplayMemoryPtr[i] = (((unsigned short)Colour) << 8) | 0x00; //0x0600;
	}
}
void Print(char text[])
{
	static unsigned int CurrentLocation = 0;
	
	unsigned short* DisplayMemoryPtr = (unsigned short*)0xB8000;
	unsigned char BackgroundColour = 0x06; // Yellow background - distinct colour from the assembly code
	unsigned char ForegroundColour = 0x00; // Black text
	unsigned char Colour = ((BackgroundColour << 4) & 0xF0) | (ForegroundColour & 0x0F); // Calculate combined colour value
	
	int i;
	for(i = 0; text[i] != 0; i++)
	{
		if(CurrentLocation == DISPLAY_SIZE) 
		{
			ShiftDisplayUpOneLine();
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
                    ShiftDisplayUpOneLine();
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
void PrintLine(char text[])
{
    Print("\n");
	Print(text);
}

extern "C" void main(){

    
	unsigned char BackgroundColour = 0x06; // Yellow background - distinct colour from the assembly code
	unsigned char ForegroundColour = 0x00; // Black text
	unsigned char Colour = ((BackgroundColour << 4) & 0xF0) | (ForegroundColour & 0x0F); // Calculate combined colour value
	unsigned short* DisplayMemoryPtr = (unsigned short*)0xB8000 ;

	int i = 0; // Start at first character of the display
	while (i < DISPLAY_SIZE) // Loop through all characters
	{
		//	Set character cell to 0x6000 i.e. (Colour << 8) | NULL Character
		DisplayMemoryPtr[i++] = (((unsigned short)Colour) << 8) | 0x00; 
	}

	int u = 1;
	char c = 'A';
		
	while(c <= 'Z' && u < 27)
	{
		*(char*)(0xb8000+2*u) = c;
		c++;
		u++;
	}
	PrintLine("rafaela \tasimakopoulou");

}
