// Timothée GUY

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define ALPHA_SIZE 26

int main(int argc, char *argv[]) {
	// Vérification du nombres d'arguments
	if (argc != 2) {
		printf("USAGE: ./subst_decrypt KEY\nThen paste your text in the terminal\n");
		exit(1);
	}

	// Vérification que la clé est bien un chiffre
	int length = strlen(argv[1]);
	for (int i = 0; i < length; ++i) {
		if (!isalpha(argv[1][i])) {
			printf("key must be a string!\nUSAGE >> ./subst_encrypt KEY\nThen paste your text in the terminal\n");
			exit(1);
		}
	}

	printf("-------------SUBSTITUTION DECRYPT-------------\n\n");

	// Création de la table de substitution
	char *alpha_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *subst_table = malloc(sizeof(char)*ALPHA_SIZE);
	int key_len = strlen(argv[1]);
	int count = 0;

	// Suppression des lettres doubles
	for (int i = 0; i < key_len; ++i) {
		bool ok = true;
		for (int j = 0; j < count; ++j) {
			if (subst_table[j] == argv[1][i]) {
				ok = false;
			}
		}
		if (ok == true) {
			subst_table[count] = argv[1][i];
			++count;
		}
	}

	// Complètement de la table avec le reste de l'alphabet
	for (int i = 0; i < ALPHA_SIZE; ++i) {
		bool ok = true;
		for (int j = 0; j < count; ++j) {
			if (subst_table[j] == 'A'+i) {
				ok = false;
			}
		}
		if (ok == true) {
			subst_table[count] = 'A'+i;
			++count;
		}
	}

	printf("substitution table: \n\t|%s|\n\t|%s|\n\n", alpha_table, subst_table);

	int letter;
	// Déryptage des messages entrés à l'aide du chiffrement de César
	while ((letter = fgetc(stdin)) != EOF) {
		count = 0;
		unsigned char c = (unsigned char) letter;
		if (letter >= 65 && letter <= 90) {
			while (subst_table[count] != c) {
				++count;
			}
			c = alpha_table[count];
		}
		printf("%c", c);
	}

	return 0;
}
