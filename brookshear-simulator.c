/*  brookshear-simulator by Jacob Warren
    Copyright (C) 2015, Jacob Warren.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>// CHAR_BIT

FILE *f;

int main(int argc, char *argv[]) {
	char file[999];
	unsigned char memory[512], registers[15], PC=0;
	unsigned int temp;
	int i;
        //First make sure the memory array has no random data
        for(i = 0; i < 512; i++) {
                memory[i] = 0x00;
        }
	//Allow the user to enter a file name as a argument
	if(argv[1] != NULL) {
		strcpy(file, argv[1]);
	}
	//If the user did not enter a file name then default is a3.txt
	else {
		strcpy(file, "a3.txt");
	}
	f = fopen(file, "r");
	if(f == NULL)
	{
		printf("Error: Cannot read file\n");
		exit(1);
	}
	i = 0;
	while(fscanf(f, "%x", &temp) != EOF) {
		memory[i++] = temp >> 8;
		memory[i++] = temp & 0x00FF;
	}
	fclose(f);
        //Before we move on we make sure the registers array have no random data
        for(i = 0; i < 15; i++) {
                registers[i] = 0x00;
        }
	while(memory[PC] != 0xC0) {
		unsigned char instr = memory[PC] >> 4;
		unsigned char R = memory[PC] & 0x0F; //?R??
		unsigned char S = (memory[PC + 1] & 0xF0) >> 4; //??S?
		unsigned char T = memory[PC + 1] & 0x0F; //???T
		unsigned char XY = memory[PC + 1]; //??XY
		printf("%02X %02X%02X - ", PC, memory[PC], memory[PC + 1]);
		printf("[ ");
                for(i = 0; i < 16; i++) {
                        printf("%02X ", registers[i]);
                }
                printf("]\n");
		if(instr == 0x01) {
			registers[R] = memory[XY]; //Load register R with value a mem address XY
		}
                if(instr == 0x02) {
			registers[R] = XY; //Load Register R with Value XY
                }
                if(instr == 0x03) {
			memory[XY] = registers[R]; //Store value on register R at Memory address XY
                }
                if(instr == 0x04) {
			registers[T] = registers[S]; //Copy Value in register S to register T
                }
                if(instr == 0x05) {
			registers[T] = registers[R] + registers[S]; //Add R and S and put answer in T
                }
                if(instr == 0x07) {
			registers[T] = registers[R] | registers[S]; //Bitwise OR  values in R and S and put answer in T
                }
                if(instr == 0x08) {
			registers[T] = registers[R] & registers[S]; //Bitwise AND values in R and S and put answer in T
                }
                if(instr == 0x09) {
			registers[T] = registers[R] ^ registers[S]; //Bitwise XOR values in R and S and put answer in T
                }
                if(instr == 0x0A) {
			registers[R] = (registers[R] >> T) | (registers[R] << (sizeof(registers[R])*CHAR_BIT - T));
                }
		PC+=2;
                if(instr == 0x0B && registers[R] == registers[0]) {
			PC = XY;
                }
	}
	printf("%02X %02X%02X - ", PC, memory[PC], memory[PC + 1]);
	printf("[ ");
	for(i = 0; i < 16; i++) {
        	printf("%02X ", registers[i]);
        }
        printf("]\n");
}
