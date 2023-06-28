#include <stdio.h>
#include <Letter_hash.h>



int hash(const char* key);
void store(const char* key, struct Letter *val);




struct Letter A;
struct Letter B;
struct Letter C;
struct Letter D;
struct Letter E;
struct Letter F;
struct Letter G;
struct Letter H;
struct Letter I;
struct Letter J;
struct Letter K;
struct Letter L;
struct Letter M;
struct Letter N;
struct Letter O;
struct Letter P;
struct Letter Q;
struct Letter R;
struct Letter S;
struct Letter T;
struct Letter U;
struct Letter V;
struct Letter W;
struct Letter X;
struct Letter Y;
struct Letter Z;

struct Letter * letter_list[26]= {
	&A, &B, &C, &D, &E, &F, &G, &H, &I, &J, &K, &L, &M, &N, &O, &P, &Q, &R, &S, &T, &U, &V, &W, &X, &Y, &Z
};

struct Letter * table[30];

const char* alpha[26] = {
	"A", "B", "C", "D", "E", "F","G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
};
 ////////////////////////////////////////////////////////// Only have A B and V
volatile char values[26][2] = {

	{0x64, 0x72}, {0xE4, 0x70}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}
	, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}
	, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}, {0x62,0x04}
	, {0x62,0x04}
};



void letter_hash_init(void){

	for (int i = 0; i < 26; i++) {
		letter_list[i]->top = values[i][0];
		letter_list[i]->bottom = values[i][1];
		store(alpha[i], letter_list[i]);
	}


}


void store(const char* key, struct Letter * val) {
	int  hashedKey = hash(key);
	table[hashedKey] = val;
}

int hash(const char* key) {
	char cpyKey = *key;
	return (cpyKey % 30);
}

struct Letter* get_letter(const char* key) {

	return table[hash(key)];
}
