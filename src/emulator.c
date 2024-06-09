#include<stdio.h>
#include<stdint.h>



uint8_t parityCount(uint8_t a){
	int count = 0;
	while(a != 0){
		count += a & 0x01;
		a >>=1;
	}
	return count;
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

uint8_t tmp = 0;

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
		state->b += (state->c++) == 0;
		break;
	case 0x04:
		state->b++;
		state->flag.p = parityCount(state->b) % 2;
		state->flag.s = (state->b & 0x80) != 0;
		if ( state->b == 0){
			state->flag.z = 1;
			state->flag.ac = 1;
		}
		break;
	case 0x05:
		state->b--;
		state->flag.p = parityCount(state->b) % 2;
		state->flag.s = (state->b & 0x80) != 0;
		state->flag.z = state->b == 0;
		state->flag.ac = state->b == UINT8_MAX;
		break;
	case 0x06:
		state->b = opcode[1];
		state->pc++;
		break;
	case 0x07:
		tmp = state->a; 
		state->flag.c = (tmp & 0x80) != 0;
		state->a = (tmp << 1) | ((tmp & 0xF7) != 0) ;
		tmp = 0;
		break;
	case 0x08:
		printf("NOT DEFINED\n");
		break;
	case 0x09:
		state->flag.c = (state->h + state->b + ((state->l+state->c) >> 8)) >> 8;
		state->h = state->h + state->b + ((state->l+state->c) >> 8);
		state->l = state->l + state->c;
		break;
	case 0x0A:
		state->a = state->memory[(state->b << 8 | state->c)];
		break;
	case 0x0B:
		state->b -= (state->c--) == 255;
		break;
	case 0x0C:
		state->c++;
		state->flag.z = (state->c) == 0;
		state->flag.ac = (state->c) == 0;
		state->flag.s = (state->c & 0x80) != 0;
		state->flag.p = parityCount(state->c) % 2;
		break;
	case 0x0D:
		state->c--;
		state->flag.z = (state->c) == 0;
		state->flag.ac = (state->c) == UINT8_MAX;
		state->flag.s = (state->c & 0x80) != 0;
		state->flag.p = parityCount(state->c) % 2;
		break;
	case 0x0E:
		state->c = opcode[1];
		state->pc++;
		break;
	case 0x0F:
		tmp = state->a; 
		state->flag.c = (tmp & 0x01) != 0; 
		state->a = (tmp >> 1) | ((tmp & 0xFE) != 0);
		tmp = 0;
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
		state->b += (state->c++) == 0;
		break;
	case 0x14:
		state->d++;
		state->flag.z = (state->d) == 0;
		state->flag.c = (state->d) == 0;
		state->flag.s = (state->d & 0x80) != 0;
		state->flag.p = parityCount(state->d) % 2;
		break;
	case 0x15:
		state->d--;
		state->flag.z = (state->d) == 0;
		state->flag.ac = (state->d) == UINT8_MAX;
		state->flag.s = (state->d & 0x80) != 0;
		state->flag.p = parityCount(state->d) % 2;
		break;
	case 0x16:
		state->d = opcode[1];
		state->pc++;
		break;
	case 0x17:
		tmp = state->a; 
		state->flag.c = (tmp & 0x80) != 0; 
		state->a = (tmp << 1) | state->flag.c;
		tmp = 0;
		break;
	case 0x18:
		printf("NOT DEFINED");
		break;
	case 0x19:
		state->flag.c = (state->h + state->d + ((state->l+state->e) >> 8)) >> 8;
		state->l = state->l + state->e;
		state->h = state->h + state->d + ((state->l + state->e) >> 8);
		break;
	case 0x1A:
		state->a = state->memory[(state->d << 8 | state->e)];
		break;
	case 0x1B:
		state->d -= (state->e--) == UINT8_MAX;
		break;
	case 0x1C:
		state->e++;
		state->flag.z = (state->e) == 0;
		state->flag.c = (state->e) == 0;
		state->flag.s = (state->e & 0x80) != 0;
		state->flag.p = parityCount(state->e) % 2;
		break;
	case 0x1D:
		state->e--;
		state->flag.z = (state->e) == 0;
		state->flag.ac = (state->e) == UINT8_MAX;
		state->flag.s = (state->e & 0x80) != 0;
		state->flag.p = parityCount(state->e) % 2;
		break;
	case 0x1E:
		state->e = opcode[1];
		state->pc++;
		break;
	case 0x1F:
		tmp = state->a; 
		state->flag.c = (tmp & 0x01) != 0; 
		state->a = (tmp >> 1) | ((tmp & 0xf7) != 0);
		tmp = 0;
		break;
	case 0x20:
		printf("NOT DEFINED");
		break;


}
}

int main(){
	
}