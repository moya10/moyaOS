#include <stddef.h>
#include <stdint.h>
#include "include/system.h"


int System::abs(int num){
	if(num<0){
		num = (-1)*num;
		
		return num;
	}else {return num;}
}

char* System::itoa(int num, char* buffer){  
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

char* System::int2String(int num, char intStr[128]) {
 	return itoa(num, intStr);
}

char* System::hex2string(uint64_t num, char hexStr[128]){
	uint64_t* valPtr = &num;
	uint8_t* ptr;
	uint8_t tmp;
	uint8_t size = 4 * 2 - 1;
	for(uint8_t i = 0; i < size; i++){
		ptr =((uint8_t*)valPtr + i);
		tmp = ((*ptr & 0xF0) >> 4);
		hexStr[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
		tmp = ((*ptr & 0x0F));
        hexStr[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexStr[size + 1] = 0;
    return hexStr;
}

char* System::peek(char a){
	char *p;
	p = &a;
	return p;
}

int System::mult(int x, int y){
	int sum =0;
	for(int i=0;i<=y-1;i++){
		sum = sum + x;
	}
	return sum;
}

int System::sqrt(int x, int n){
	int y = 0; 
	for(int j=(n/2-1);j>=0;j--){
		if(j==1){
			int num = y+2;
			if((mult(num,num)) <= x){
				y = y + 2;
				return y;
			}
		}
		else if(j==0){
			int num = y+1;
			if((mult(num,num)) <= x){
				y = y + 1;
				return y;
			}
		}
		else{
			int val= y+mult(2,2)*(j-1);
			
			if((mult(val,val)) >= x){
				y = val;
				return y;
			}
		}
	}
	return y;	
}

int System::division(int x, int y){
	if(y>x) return 0;
	int q = division(x, 2*y);
	if((x-2*q*y) < y) return 2*q;
	else return 2*q + 1;
}