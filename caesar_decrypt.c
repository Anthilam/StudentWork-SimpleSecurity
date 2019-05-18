// Timothée GUY

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int i;

	// Vérification du nombre d'arguments
	if (argc != 2) {
		printf("USAGE >> ./caesar_decrypt key\nThen paste your text in the terminal\n");
		exit(1);
	}

	// Vérification que la clé est bien un chiffre
	int length = strlen(argv[1]);
  for (i = 0; i < length; i++) {
		if (!isdigit(argv[1][i])) {
			printf("key must be a number!\nUSAGE >> ./caesar_decrypt key\nThen paste your text in the terminal\n");
			exit(1);
		}
	}

	// Vérification que la clé est bien contenue entre 0 et 25
	int key = atoi(argv[1]);
	if (key < 0 || key > 25) {
		printf("key must be between 0 and 25\n");
		exit(1);
	}

	printf("-------------CAESAR DECRYPT-------------\n\n");

	// Décryptage des messages à l'aide de clé passé en argument selon le chiffrement de César
	while ((i = fgetc(stdin)) != EOF) {
		unsigned char c = (unsigned char) i;
		if (i >= 65 && i <= 90) {
			int uncrypted = i - 65 - key;
			if (uncrypted < 0) {
				uncrypted += 26;
			}
			c = (unsigned char) uncrypted + 65;
		}
		printf("%c", c);
	}

	return 0;
}
