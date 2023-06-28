
#ifndef LETTER_HASH_
#define LETTER_HASH_



struct Letter{
	volatile char top;
	volatile char bottom;
};


void letter_hash_init(void);
struct Letter* get_letter(const char* key);


#endif
