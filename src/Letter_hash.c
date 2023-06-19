#include <stdio.h>
#include <Letter_hash.h>



void store(const char* key, int val) {
	int  hashedKey = hash(key);
	table[hashedKey] = val;
}

int hash(const char* key) {
	char cpyKey = *key;
	return (cpyKey % 30);
}

int get(const char* key) {
	return table[hash(key)];
}