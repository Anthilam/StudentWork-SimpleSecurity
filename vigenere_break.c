// Timothée GUY

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IC_FR 0.0746
#define ALPHA_SIZE 26

// Structure représentant un tableau dynamique
struct array {
  char *data;
  size_t capacity;
  size_t size;
};

// Fonctions relatives aux tableaux dynamiques
void array_create(struct array *self) {
  self->data = NULL;
  self->capacity = 0;
  self->size = 0;
}

void array_destroy(struct array *self) {
  if (self != NULL) {
    free(self->data);
    self->size = 0;
    self->capacity = 0;
  }
}

void array_add(struct array *self, char value) {
  if (self->size == self->capacity) {
    if (self->capacity == 0) {
      self->capacity++;
    }
    self->capacity *= 2;
    char *data = calloc(self->capacity, sizeof(char));
    memcpy(data, self->data, self->size * sizeof(char));
    free(self->data);
    self->data = data;
  }
  self->data[self->size] = value;
  ++self->size;
}

void array_dump(const struct array *self) {
  for (int i = 0; i < self->size; ++i) {
    printf("%c", self->data[i]);
  }
}

int main(int argc, char *argv[]) {
	// Vérification du nombres d'arguments
	if (argc != 2) {
		printf("USAGE: ./vigenere_break [PATH]\n\n");
		exit(1);
	}

	printf("-------------VIGENERE BREAK-------------\n\n");

	// Variables
	double maxIC = 0; // Indice de coïncidence maximum calculé
	int key_size = 0; // Taille de la clé
	int count = 0; // Compteur de lettres

	/* Création des tableaux dynamiques, un pour le texte lu, un pour le texte
	sans caractère spéciaux utilisé pour décrypter */
	struct array txt;
	struct array txt_upper_only;
	array_create(&txt);
	array_create(&txt_upper_only);

	// Ouverture du fichier
	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		printf("Error while opening file, please check the path\n\n");
		exit(1);
	}

	// Lecture du texte et ajout de celui-ci dans un tableau dynamique
	int c;
	while ((c = fgetc(f)) != EOF) {
		array_add(&txt, (char) c);
	}
	fclose(f);

	// Création du tableau de fréquences
	int* freq = calloc(ALPHA_SIZE, sizeof(int));

	// Ajout dans le tableau txt_upper_only de tout les caractères non-spéciaux de txt
	for (int i = 0; i < txt.size; ++i) {
		if (isupper(txt.data[i]) != 0) {
			array_add(&txt_upper_only, txt.data[i]);
		}
	}

	// On calcule l'IC jusqu'à ce qu'il soit supérieur à l'IC d'un texte français
	int test = 1;
	while (maxIC < IC_FR) {
		double testIC = 0; // Indice de coïncidence calculé pour les sous-textes
		// On parcourt le texte comme s'il était découpé en plusieurs sous-textes
		for (int i = 0; i < test; ++i) {
			count = 0;
			// Calcul des fréquences
			for (int j = i; j < txt_upper_only.size;) {
				++freq[(int)txt_upper_only.data[j]-'A'];
				++count;
				j += test;
			}

			// Calcul de l'IC pour chaque sous-texte
			double tmpIC = 0;
			for (int j = 0; j < ALPHA_SIZE; ++j) {
				double a = (freq[j]*(freq[j]-1));
				double b = count*(count-1);
				tmpIC = tmpIC + (a/b);
				freq[j] = 0;
			}
			testIC += tmpIC;
		}

		// On calcule l'IC des sous-textes en faisant la moyenne de leurs IC
		testIC /= test;
		// On augmente maxIC et key_size ou non en fonction de testIC
		if (testIC > maxIC) {
			maxIC = testIC;
			key_size = test;
		}

		++test;
	}

	/* Lorsque que l'on a trouvé la taille des clés, on décrypte selon le
	chiffrement de César */
	char key_alpha[key_size];
	for (int i = 0; i < key_size; ++i) {
		// Calcul des fréquences
		for (int j = i; j < txt_upper_only.size;) {
			++freq[(int)txt_upper_only.data[j]-'A'];
			++count;
			j += key_size;
		}

		// Calcul de la clé
		int key = 0;
		for (int j = 0; j < ALPHA_SIZE; j++) {
			if (freq[j] > freq[key]) {
				key = j;
			}
		}

		if (key <= 3) {
			key += ALPHA_SIZE;
		}
		key = (key - 4) % ALPHA_SIZE;
		key_alpha[i] = 'A'+key;

		// Décryptage du message
		for (int j = i; j < txt_upper_only.size;) {
			int uncrypted = txt_upper_only.data[j] - 'A' - key;
			if (uncrypted < 0) {
				uncrypted += ALPHA_SIZE;
			}
			txt_upper_only.data[j] = (unsigned char) uncrypted + 'A';
			j += key_size;
		}

		// Réinitialisation du tableau de fréquences
		for (int j = 0; j < ALPHA_SIZE; ++j) {
			freq[j] = 0;
		}
	}

	/* Changement des lettres de txt par le message décrypter afin de retrouver
	les caractères spéciaux */
	count = 0;
	for (int i = 0; i < txt.size; ++i) {
		if (isupper(txt.data[i]) != 0) {
			txt.data[i] = txt_upper_only.data[count];
			++count;
		}
	}

	// Affichage du texte décrypté
	array_dump(&txt);

	// Affichage d'informations diverses
	printf("\n\nKEY_SIZE=%d / IC(CryptedText)=%f / KEY=", key_size, maxIC);
	for (int i = 0; i < key_size; ++i) {
		printf("%c", key_alpha[i]);
	}
	printf("\n");

	// Libération mémoire
	array_destroy(&txt);
	array_destroy(&txt_upper_only);
	free(freq);

	return 0;
}
