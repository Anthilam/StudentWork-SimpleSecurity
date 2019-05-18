// Timothée GUY

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int i;

	// Vérification du nombre d'arguments
	if (argc != 2) {
		printf("USAGE >> ./vigenere_decrypt key\nThen paste your text in the terminal\n");
		exit(1);
	}

	// Vérification que la clé est bien un chiffre
	int length = strlen(argv[1]);
  for (i = 0; i < length; i++) {
		if (!isalpha(argv[1][i])) {
			printf("key must be a string!\nUSAGE >> ./vigenere_decrypt key\nThen paste your text in the terminal\n");
			exit(1);
		}
	}

	printf("-------------VIGENERE DECRYPT-------------\n\n");

	int count = 0;

	// Cryptage des messages entrés à l'aide du chiffrement de César
	while ((i = fgetc(stdin)) != EOF) {
		if (count == length) {
			count = 0;
		}

		unsigned char c = (unsigned char) i;
		if (i >= 65 && i <= 90) {
			c = (unsigned char) (i - (unsigned int)argv[1][count] - 126) % 26 + 65;
			count++;
		}
		printf("%c", c);
	}

	return 0;
}
