#include <stdint.h>
#include <stddef.h>

#define DISPLAY_SIZE 2000 // 2000 = 80 x 25 Characters - VGA Text-mode Display size
struct HeapSegHdr{
    size_t length;
    HeapSegHdr* next;
    HeapSegHdr* last;
    bool free;
	void CombineForward();
    void CombineBackward();
    HeapSegHdr* Split(size_t splitLength);
};

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

int abs(int num){
	if(num<0){
		num = (-1)*num;
		
		return num;
	}else {return num;}
}

char* itoa(int num, char* buffer){  
	int current = 0;  
	if(num <0){
		current = 1;
		num *= -1;
		buffer[0] = '-';
	}
	int size = 0;
	int sizeTest = num;
	while(sizeTest / 10 > 0){
		sizeTest /= 10;
		size++;
	}
	int index = 0;
	while(num / 10 > 0){
		int remainder = num % 10;
		num /= 10;
		buffer[current + size - index] = remainder + '0';
		index++;
	}
	int remainder = num % 10;
	buffer[current + size - index] = remainder + '0';
	buffer[current + size + 1] = 0;
	return buffer;	
}

char* int2String(int num, char intStr[128]) {
 	return itoa(num, intStr);
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

	
	int u = 0;
	char c = 'A';
	char buffer[128];  
	//*(char*)(0xb8000 + 2*u) = string;
	PrintLine("Example\t01");
	PrintLine("Printing the English Alphabet\n");
	while(c <= 'Z' && u < 27)
	{
		buffer[u]=(char)c;
		c++;
		u++;
	}
	PrintLine(buffer);

	PrintLine("\n");
	PrintLine("Example\t02");
	PrintLine("Converting Integers to String\n");
	char* ab = int2String(-123,buffer);
	PrintLine(ab);
	
}
