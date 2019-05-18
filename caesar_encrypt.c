// Timothée GUY

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int i;

	// Vérification du nombre d'arguments
	if (argc != 2) {
		printf("USAGE >> ./caesar_encrypt key\nThen paste your text in the terminal\n");
		exit(1);
	}

	// Vérification que la clé est bien un chiffre
	int length = strlen(argv[1]);
  for (i = 0; i < length; i++) {
		if (!isdigit(argv[1][i])) {
			printf("key must be a number!\nUSAGE >> ./caesar_encrypt key\nThen paste your text in the terminal\n");
			exit(1);
		}
	}

	// Vérification que la clé est bien contenue entre 0 et 25
	int key = atoi(argv[1]);
	if (key < 0 || key > 25) {
		printf("key must be between 0 and 25\n");
		exit(1);
	}

	printf("-------------CAESAR ENCRYPT-------------\n\n");

	// Cryptage des messages entrés à l'aide du chiffrement de César
	while ((i = fgetc(stdin)) != EOF) {
		unsigned char c = (unsigned char) i;
		if (i >= 65 && i <= 90) {
			c = (unsigned char) ((i - 65 + key) % 26) + 65;
		}
		printf("%c", c);
	}

	return 0;
}
