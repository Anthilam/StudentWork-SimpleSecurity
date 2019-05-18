FLAGS=-Wall -std=c99 -O2 -g -o
PROGS=caesar_encrypt caesar_decrypt caesar_break vigenere_encrypt vigenere_decrypt vigenere_break subst_encrypt subst_decrypt subst_break

all: $(PROGS)

caesar_encrypt: caesar_encrypt.c
	gcc $(FLAGS) caesar_encrypt caesar_encrypt.c

caesar_decrypt: caesar_decrypt.c
	gcc $(FLAGS) caesar_decrypt caesar_decrypt.c

caesar_break: caesar_break.c
	gcc $(FLAGS) caesar_break caesar_break.c

vigenere_encrypt: vigenere_encrypt.c
	gcc $(FLAGS) vigenere_encrypt vigenere_encrypt.c

vigenere_decrypt: vigenere_decrypt.c
	gcc $(FLAGS) vigenere_decrypt vigenere_decrypt.c

vigenere_break: vigenere_break.c
	gcc $(FLAGS) vigenere_break vigenere_break.c

subst_encrypt: subst_encrypt.c
	gcc $(FLAGS) subst_encrypt subst_encrypt.c

subst_decrypt: subst_decrypt.c
	gcc $(FLAGS) subst_decrypt subst_decrypt.c

subst_break: subst_break.c
	gcc $(FLAGS) subst_break subst_break.c

clean:
	rm -f *.o

mrproper: clean
	rm -f $(PROGS)
