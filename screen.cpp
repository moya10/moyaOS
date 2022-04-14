#include <stddef.h>
#include <stdint.h>
#include "include/system.h"
#include "CPU/irq.h"

#define DISPLAY_SIZE 2000 // 2000 = 80 x 25 Characters - VGA Text-mode Display size
#define DEFAULT_COLOR 0x50
#define BAR_COLOR 0x05
#define DARK_COLOR 0x0A
#define ERROR_COLOR 0x74
// /* These define our textpointer, our background and foreground
// *  colors (attributes), and x and y cursor coordinates */
// unsigned short *textmemptr;
// int attrib = 0x0F;
// int csr_x = 0, csr_y = 0;

// /* Scrolls the screen */
// void System::scroll(void)
// {
//     unsigned blank, temp;

//     /* A blank is defined as a space... we need to give it
//     *  backcolor too */
//     blank = 0x20 | (attrib << 8);

//     /* Row 25 is the end, this means we need to scroll up */
//     if(csr_y >= 25)
//     {
//         /* Move the current text chunk that makes up the screen
//         *  back in the buffer by a line */
//         temp = csr_y - 25 + 1;
//         memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

//         /* Finally, we set the chunk of memory that occupies
//         *  the last line of text to our 'blank' character */
//         memsetw (textmemptr + (25 - temp) * 80, blank, 80);
//         csr_y = 25 - 1;
//     }
// }

// /* Updates the hardware cursor: the little blinking line
// *  on the screen under the last character pressed! */
// void System::move_csr(void)
// {
//     unsigned temp;

//     temp = csr_y * 80 + csr_x;

//     system.outb(0x3D4, 14);
//     system.outb(0x3D5, temp >> 8);
//     system.outb(0x3D4, 15);
//     system.outb(0x3D5, temp);
// }




// /* Clears the screen */
// void System::cls()
// {
//     unsigned blank;
//     int i;

//     /* Again, we need the 'short' that will be used to
//     *  represent a space with color */
//     blank = 0x20 | (attrib << 8);

//     /* Sets the entire screen to spaces in our current
//     *  color */
//     for(i = 0; i < 25; i++)
//         memsetw (textmemptr + i * 80, blank, 80);

//     /* Update out virtual cursor, and then move the
//     *  hardware cursor */
//     csr_x = 0;
//     csr_y = 0;
//     move_csr();
// }

// /* Puts a single character on the screen */
// void System::putch(const char c)
// {
//     unsigned short *where;
//     unsigned att = attrib << 8 ;

//     /* Handle a backspace, by moving the cursor back one space */
//     if(c == 0x08)
//     {
//         where = textmemptr + (csr_y * 80 + csr_x);            
//         *where = c | (((short) 0x00000011) << 8) | 0x80;	/* Character AND attributes: color */
//         csr_x--;
	    
//     }

//     if(c == 0x39)
//     {
//         csr_x++;
       
//     }
//     /* Handles a tab by incrementing the cursor's x, but only
//     *  to a point that will make it divisible by 8 */
//     else if(c == 0x09)
//     {
//         csr_x = (csr_x + 8) & ~(8 - 1);
//     }
//     /* Handles a 'Carriage Return', which simply brings the
//     *  cursor back to the margin */
//     else if(c == '\r')
//     {
//         csr_x = 0;
//     }
//     /* We handle our newlines the way DOS and the BIOS do: we
//     *  treat it as if a 'CR' was also there, so we bring the
//     *  cursor to the margin and we increment the 'y' value */
//     else if(c == '\n')
//     {
//         csr_x = 0;
//         csr_y++;
//     }
//     /* Any character greater than and including a space, is a
//     *  printable character. The equation for finding the index
//     *  in a linear chunk of memory can be represented by:
//     *  Index = [(y * width) + x] */
//     else if(c >= ' ')
//     {
//         where = textmemptr + (csr_y * 80 + csr_x);
//         *where = c | att;	/* Character AND attributes: color */
//         csr_x++;
//     }

//     /* If the cursor has reached the edge of the screen's width, we
//     *  insert a new line in there */
//     if(csr_x >= 80)
//     {
//         csr_x = 0;
//         csr_y++;
//     }

//     /* Scroll the screen if needed, and finally move the cursor */
//     scroll();
//     move_csr();
// }

// /* Uses the above routine to output a string... */
// void System::puts(const char *text)
// {
//     int i;

//     for (i = 0; i < strlen(text); i++)
//     {
//         putch(text[i]);
//     }
// }

// /* Sets the forecolor and backcolor that we will use */
// void System::settextcolor(unsigned char forecolor, unsigned char backcolor)
// {
//     /* Top 4 bytes are the background, bottom 4 bytes
//     *  are the foreground color */
//     attrib = (backcolor << 4) | (forecolor & 0x0F);
// }

// /* Sets our text-mode VGA pointer, then clears the screen for us */
// void System::init_video(void)
// {
//     textmemptr = (unsigned short *)0xB8000;
//     int i = 0;
// 	while (i < DISPLAY_SIZE) 
// 	{
// 		textmemptr[i++] = (((short) 0x00000011) << 8) | 0x80; 
// 	}
//     //cls();
// }

#define VIDEO_MEMORY		(char*)0xB8000
#define VGA_WIDTH	80

int curColor = DEFAULT_COLOR;
uint16_t CursorPos = 0; 		// Holds the current position of the cursor


void SetCursorPosRaw(uint16_t pos){	// Does some I/O black magic 
	if(pos >= 0 && pos < DISPLAY_SIZE) {
		system.outb(0x3d4, 0x0f);
		system.outb(0x3d5, (uint8_t)(pos & 0xff));
		system.outb(0x3d4, 0x0e);
		system.outb(0x3d5, (uint8_t)((pos >> 8) & 0xff));
		CursorPos = pos;
	}
	return;
}


void SetCursorPos(int x, int y){
	uint16_t pos;

	pos = y * VGA_WIDTH + x;
	SetCursorPosRaw(pos);
	return;
}


void ClearScreen(int col){	//col...or
	if(col == -1) col = curColor;		//-1: maintain current colour
	for(int i = (int)VIDEO_MEMORY + 160; i < (int)VIDEO_MEMORY + 3840; i += 1) {
		if(i % 2 == 0) *((char*)i) = 32;	//Default is spaces
		else *((char*)i) = col;
	}
	CursorPos = 0;
}

// void ColScreen(int col){
// 	for(int i = (int)VIDEO_MEMORY + 1; i < (int)VIDEO_MEMORY + 4000; i += 2)
// 		*((char*)i) = col;
// }

// SET LINE COLOUR
void ColLine(int line, int col){
	for(int i = (int)VIDEO_MEMORY + VGA_WIDTH * 2 * line + 1;
	i < (int)VIDEO_MEMORY + VGA_WIDTH * 2 * (line + 1) + 1; i += 2)  *((char*)i) = col;
		
}

void ClrLine(int line){
	for(int i = (int)VIDEO_MEMORY + VGA_WIDTH * 2 * line;
	i < (int)VIDEO_MEMORY + VGA_WIDTH * 2 * (line + 1); i += 2)  *((char*)i) = 0; 	// CLR WITH 0s
		
}

void scrollPageUp(){
    for(int i = 160*2; i < 4000 - 160; i++) *(VIDEO_MEMORY + i - 160) = *(VIDEO_MEMORY + i);
}


void kprint(const char* s){		// Just a simple print function. Prints to screen at cursor position, moves the cursor at the end. 
	uint8_t* charPtr = (uint8_t*)s;
	uint16_t i = CursorPos;
	while(*charPtr != 0){
        
        switch (*charPtr) {
            case 10:	
                if(i < 1920){
                        
                    if(CursorPos >= 1760){
                
                        i = 1760;
                
                        scrollPageUp();
                
                    }else i+= VGA_WIDTH - i % VGA_WIDTH;	// ALSO ADDS RETURN TO NEWLINE!!
                        
                }
                
                    
                break;
            case 13:
                i -= i % VGA_WIDTH;
                break;
            default:
                if(i < 1840){
                    *(VIDEO_MEMORY + i * 2) = *charPtr;
                    i++;
                }
                else{
                    scrollPageUp();
                    i = 1760;
                }
                break;
        }

        charPtr++;
	}
	SetCursorPosRaw(i);
	return;
}


void kprintCol(const char* s, int col){		//Print: with colours!
  uint8_t* charPtr = (uint8_t*)s;
  uint16_t i = CursorPos;
  while(*charPtr != 0)
  {
	switch (*charPtr) {
	  case 10:	
	  		if(CursorPos < 1920)
	  			i+= VGA_WIDTH - i % VGA_WIDTH;	// ALSO ADDS RETURN TO NEWLINE!!
			break;
	  case 13:
			i -= i % VGA_WIDTH;
			break;
	  default:
        *(VIDEO_MEMORY + i*2) = *charPtr;
        *(VIDEO_MEMORY + i*2 + 1) = col;
        i++;
	}

	charPtr++;
  }
  SetCursorPosRaw(i);
  return;
}


void MoveCursorLR(int i){			// MOVE CURSOR HORIZONTALLY
	if(CursorPos >= 0 && CursorPos < DISPLAY_SIZE){
		
		CursorPos += i;
		SetCursorPosRaw(CursorPos);
		
	}
	return;
}

void MoveCursorUD(int i){			// MOVE CURSOR VERTICALLY
	if(CursorPos >= 0 && CursorPos < DISPLAY_SIZE){
		
		CursorPos += VGA_WIDTH * i;
		SetCursorPosRaw(CursorPos);
		
	}
	return;
}

void ScrollVMem(){
	int i;
	
	for(i = 0xB8000/2 + 1999 ; i >= CursorPos + 0xB8000/2; i--)
		*((char*)(i * 2)) = *((char*)(i * 2 - 2));
	return;
}

void kprintChar(const char c, bool caps){
	int curLine = 1 + CursorPos / VGA_WIDTH;
    
    
	if(c == 8 || c == 10 || (c >= 32 && c <= 255)) {
	switch(c){
		case 10:
			if(CursorPos < 1920){ 						// newline
                CursorPos+=VGA_WIDTH - CursorPos % VGA_WIDTH;	
            }
			break;
		case 8: 						// backspace
			if(CursorPos > 0){
				
				CursorPos--;
				*(VIDEO_MEMORY + CursorPos * 2) = (char)0;
				
			}
			break;
		case 127: 						// del
			if(CursorPos < DISPLAY_SIZE){
			*(VIDEO_MEMORY + CursorPos * 2 + 2) = (char)0;
			}
			break;
		default:
			ScrollVMem();
			if(c >= 97 && c <= 172 && caps) *(VIDEO_MEMORY + CursorPos * 2) = c-32; // CAPS
			else if(c >= 48 && c <= 57 && caps){
				//caps numbers
                *(VIDEO_MEMORY + CursorPos * 2) = c-32; // CAPS
			}
			else  *(VIDEO_MEMORY + CursorPos * 2) = c;
			
			if(CursorPos < DISPLAY_SIZE)
				CursorPos++;
            else scrollPageUp();
			
		}
	}
	
	SetCursorPosRaw(CursorPos);
	return;
}

void printError(const char* s){
    kprintCol(s, ERROR_COLOR);
}

int CommandCursor;


const char* currentTask;

void load_shell(){
	system.LOAD_SHELL = true;
    curColor = DEFAULT_COLOR;
    currentTask = "init";
	ClearScreen(DEFAULT_COLOR);
	ColLine(0, BAR_COLOR);
	ColLine(24, BAR_COLOR);
	ClrLine(0);
	ClrLine(24);
	SetCursorPos(0,0);
	kprint("moyaOS shell - ");
    kprint("task ");
	kprint("[");
    kprint(currentTask);
	kprint("]\n");
	CommandCursor = CursorPos;
	SetCursorPosRaw(1920);
	return;
}


char CommandBuffer[128];

void parseCommand()
{
    char* c = (char*)(0xB8000 + 2 * 1920);
	int i;
	for(i = 0; i < DISPLAY_SIZE - CursorPos; i++)
		CommandBuffer[i] = *(c + 2 * i);
	CommandBuffer[i] = 0; 
	
	SetCursorPosRaw(CommandCursor);
	currentTask = " ";

    if(system.StringsEqu(CommandBuffer, "help")){
        currentTask = "help";
        kprint("List of commands:\n");
        kprint("  help: shows command list\n");
        kprint("  echo [text]: prints text\n");
        kprint("  clear: clears the screen\n");   
    }
    else if(system.StringsEqu(CommandBuffer, "clear")){
        currentTask = "clear";
        ClearScreen(-1);  
    }
	else if(system.StringStartsWith(CommandBuffer, "echo")){
		currentTask = "print";
		char* src = CommandBuffer;
		kprint(system.removeChar(src,(char*)"echo "));
	}
    else {
        currentTask = "invalid"; 
        kprint("\""); 
        kprint(CommandBuffer); 
        kprint("\" is not a command");
    }
    kprint("\n");
    CommandCursor = CursorPos;
	ClrLine(24);
    ClrLine(0);
	SetCursorPos(0,0);
	kprint("moyaOS shell - ");
    kprint("task ");
	kprint("[");
    kprint(currentTask);
	kprint("]\n");
	SetCursorPosRaw(1920);
    return;
}


