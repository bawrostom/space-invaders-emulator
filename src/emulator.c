#include<stdio.h>
#include<stdint.h>
#include <stdlib.h>

#define COUNT 20
#define BYTE 1

uint8_t parityCount(uint16_t a){
	int count = 0;
	while(a != 0){
		count += a & 0x01;
		a >>=1;
	}
	return ((count % 2) == 0) ;
}


typedef struct flags{
	uint8_t z:1;
	uint8_t c:1;
	uint8_t s:1;
	uint8_t p:1;
	uint8_t ac:1;
	uint8_t pad:3;
} flags;

typedef struct state8080 {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t pc;
	uint16_t sp;
	uint8_t *memory;
	struct flags flag;
	uint8_t int_enable;
} state8080;

void emulate8080(state8080* state){

uint8_t tmp;
uint16_t tmp1;
uint16_t tmp2;

uint8_t* opcode = &state->memory[state->pc];
switch(*opcode){
	case 0x00: break;
	case 0x01:
		state->c = opcode[1];
		state->b = opcode[2];
		state->pc += 2;
		break;
	case 0x02:
		state->memory[(state->b << 8 | state->c)] = state->a;
		break;
	case 0x03:
		tmp1 = ((state->b << 8) | state->c) + 1;
		state->c = tmp1 & 0xff;
		state->b = (tmp1 >> 8) & 0xff;
		break;
	case 0x04:
		state->flag.ac = ((state->b & 0x0f) + 1) > 0x0f;
		state->b++;
		state->flag.p = parityCount((uint16_t)state->b);
		state->flag.s = (state->b & 0x80) != 0;
		state->flag.z = state->b == 0;
		break;
	case 0x05:
		state->flag.ac = (state->b & 0x0f) == 0;
		state->b--;
		state->flag.p = parityCount((uint16_t)state->b);
		state->flag.s = (state->b & 0x80) != 0;
		state->flag.z = state->b == 0;
		break;
	case 0x06:
		state->b = opcode[1];
		state->pc++;
		break;
	case 0x07:
		tmp = state->a;
		state->flag.c = tmp >> 7;
		state->a = (tmp << 1) | (tmp >> 7 ) ;
		break;
	case 0x08:
		printf("NOT DEFINED\n");
		break;
	case 0x09:
		tmp1 = (state->h << 8) | state->l;
		tmp2 = (state->b << 8) | state->c;
		state->flag.c = (tmp1 + tmp2 ) > 0xffff;
		tmp1 = tmp1 + tmp2;
		state->l = tmp1 & 0xff;
		state->h = (tmp1 >> 8) & 0xff;
		break;
	case 0x0A:
		state->a = state->memory[(state->b << 8 | state->c)];
		break;
	case 0x0B:
		tmp1 = ((state->b << 8) | state->c) - 1;
		state->c = tmp1 & 0xff;
		state->b = (tmp1 >> 8) & 0xff;
		break;
	case 0x0C:
		state->flag.ac = ((state->c & 0x0f) + 1 ) > 0x0f;
		state->c++;
		state->flag.z = (state->c == 0);
		state->flag.s = (state->c & 0x80) != 0;
		state->flag.p = parityCount((uint16_t)state->c);
		break;
	case 0x0D:
		state->flag.ac = ((state->c & 0x0f) == 0);
		state->c--;
		state->flag.z = (state->c == 0);
		state->flag.s = (state->c & 0x80) != 0;
		state->flag.p = parityCount((uint16_t)state->c);
		break;
	case 0x0E:
		state->c = opcode[1];
		state->pc++;
		break;
	case 0x0F:
		tmp = state->a; 
		state->flag.c = (tmp & 0x01) != 0; 
		state->a = ((uint8_t)tmp << 7) | (tmp >> 1);
		break;
	case 0x10:
		printf("NOT DEFINED");
		break;
	case 0x11:
		state->d = opcode[2];
		state->e = opcode[1];
		state->pc += 2;
		break;
	case 0x12:
		state->memory[(state->d << 8 | state->e)] = state->a;
		break;
	case 0x13:
		tmp1 = ((state->d << 8) | state->e) + 1;
		state->e = tmp1 & 0xff;
		state->d = (tmp1 >> 8) & 0xff;
		break;
	case 0x14:
		state->flag.ac = ((state->d & 0x0f) + 1) > 0x0f;
		state->d++;
		state->flag.z = (state->d == 0);
		state->flag.s = (state->d & 0x80) != 0;
		state->flag.p = parityCount(uint16_t state->d);
		break;
	case 0x15:
		state->flag.ac = (((state->d & 0x0f) - 1) == 0);
		state->d--;
		state->flag.z = (state->d) == 0;
		state->flag.s = (state->d & 0x8) != 0;
		state->flag.p = parityCount(uint16_t state->d);
		break;
	case 0x16:
		state->d = opcode[1];
		state->pc++;
		break;
	case 0x17:
		tmp = state->a;
		state->a = (tmp << 1) | state->flag.c; 
		state->flag.c = tmp >> 7;
		break;
	case 0x18:
		printf("NOT DEFINED");
		break;
	case 0x19:
		tmp1 = (state->h << 8) | state->l;
		tmp2 = (state->d << 8) | state->e;
		state->flag.c = (tmp1 + tmp2 ) > 0xffff;
		tmp1 = tmp1 + tmp2;
		state->l = tmp1 & 0xff;
		state->h = (tmp1 >> 8) & 0xff;
		break;
	case 0x1A:
		state->a = state->memory[(state->d << 8 | state->e)];
		break;
	case 0x1B:
		tmp1 = ((state->d << 8) | state->e) + 1;
		state->d = tmp1 & 0xff;
		state->e = (tmp1 >> 8) & 0xff;
		break;
	case 0x1C:
		state->flag.ac = ((state->e & 0x0f) + 1 ) > 0x0f;
		state->e++;
		state->flag.z = (state->e == 0);
		state->flag.s = (state->e & 0x80) != 0;
		state->flag.p = parityCount((uint16_t)state->e);
		break;
	case 0x1D:
		state->flag.ac = ((state->e & 0x0f) == 0);
		state->e--;
		state->flag.z = (state->e == 0);
		state->flag.s = (state->e & 0x80) != 0;
		state->flag.p = parityCount((uint16_t)state->e);
		break;
	case 0x1E:
		state->e = opcode[1];
		state->pc++;
		break;
	case 0x1F:
		tmp = state->a; 
		state->flag.c = (tmp & 0x01) != 0; 
		state->a = (tmp & 0x80) | (tmp >> 1);
		break;
	case 0x20:
		printf("NOT DEFINED");
		break;
	//--------------------------------------------------------
	case 0x21:
		state->h = opcode[2];
		state->l = opcode[1];
		state->pc += 2;
		break;
	case 0x22:
		state->memory[(opcode[2] << 8 | opcode[1])] = state->l;
		state->memory[(opcode[2] << 8 | opcode[1])+1] = state->h;
		state->pc += 2;
		break;
	case 0x23:
		state->h += (state->l++) == 0;
		break;
	case 0x24:
		state->h++;
		state->flag.z = (state->h) == 0;
		state->flag.c = (state->h) == 0;
		state->flag.s = (state->h & 0x8) != 0;
		state->flag.p = parityCount(uint16_t state->h) % 2;
		break;
	case 0x25:
		state->h--;
		state->flag.z = (state->h) == 0;
		state->flag.ac = (state->h) == UINT8_MAX;
		state->flag.s = (state->h & 0x8) != 0;
		state->flag.p = parityCount(uint16_t state->h) % 2;
		break;
	case 0x26:
		state->h = opcode[1];
		state->pc++;
		break;
	case 0x27:
		break;
	case 0x28:
		printf("NOT DEFINED");
		break;
	case 0x29:
		state->flag.c = (state->h + state->h + ((state->l+state->l) >> 8)) >> 8;
		state->l = state->l + state->l;
		state->h = state->h + state->h + ((state->l + state->l) >> 8);
		break;
	case 0x2A:
		state->l = state->memory[(opcode[2] << 8 | opcode[1])];
		state->h = state->memory[(opcode[2] << 8 | opcode[1])+1];
		state->pc += 2;
		break;
	case 0x2B:
		state->h -= (state->l--) == UINT8_MAX;
		break;
	case 0x2C:
		state->l++;
		state->flag.z = (state->l) == 0;
		state->flag.c = (state->l) == 0;
		state->flag.s = (state->l & 0x8) != 0;
		state->flag.p = parityCount(uint16_t state->l) % 2;
		break;
	case 0x2D:
		state->l--;
		state->flag.z = (state->l) == 0;
		state->flag.ac = (state->l) == UINT8_MAX;
		state->flag.s = (state->l & 0x8) != 0;
		state->flag.p = parityCount(uint16_t state->l) % 2;
		break;
	case 0x2E:
		state->l = opcode[1];
		state->pc++;
		break;
	case 0x2F:
		state->a = ~state->a;
		break;
	case 0x30:
		printf("NOT DEFINED");
		break;
	case 0x31:
		state->sp = (uint16_t opcode[1] << 8);
		state->sp = (uint16_t opcode[2]);
		state->pc += 2;
		berak;
	case 0x32:
		*opcode[1] = state->a;
		state->pc += 2;
		break;
	case 0x33:
		state->sp++;
		break;
	case 0x34:
		uint16_t content = *(state->h << 8 | state->l );
		content++;
		state->flag.z = content == 0;
		state->flag.s = (content & 0x80) != 0;
		state->flag.p = parityCount(content) % 2;
		state->

}
}

void validate(){
	

	uint8_t *buffer = malloc(sizeof(uint8_t) * 3);

	scanf("Enter the first opcode: %x", buffer[0]);
	scanf("Enter the first opcode: %x", buffer[1]);
	scanf("Enter the first opcode: %x", buffer[2]);

	state8080 state = {
	.a=0,
	.b=0,
	.c=0,
	.d=0,
	.e=0,
	.h=0,
	.l=0,
	.pc=0,
	.sp=0,
	.flag={0},
	.memory=buffer,
	.int_enable=0
	};
	
	emulate8080(&state);
}

int main(){

	state8080 *state;
	state = malloc(sizeof(state8080));
	uint8_t *buffer = malloc(sizeof(uint8_t) * 20);
	
	FILE *fd = fopen("file","r");
	
	if ( fd == NULL ){
		perror("Failed opening file\n");
		return 1;
	}
	
	fread(buffer, BYTE, COUNT,fd);
	fclose(fd);

	state->memory = buffer;
	
	for(int i=0; i<20; i++){
		printf("buffer[%d]= %x \n", i,buffer[i]);
	}

	emulate8080(state);

	free(buffer);
	return 0;

}