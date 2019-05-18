// Timothée GUY

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ALPHA_SIZE 26

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
	// Vérification du nombre d'arguments
	if (argc != 2) {
		printf("USAGE: ./caesar_break [PATH]\n\n");
		exit(1);
	}

	printf("-------------CAESAR BREAK-------------\n\n");

	struct array txt;
	array_create(&txt);

	// Lecture du texte dans le fichier passé en paramètres
	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		printf("Error while opening file, please check the path\n\n");
		exit(1);
	}

	int c;
	while ((c = fgetc(f)) != EOF) {
		array_add(&txt, (char) c);
	}
	fclose(f);

	// Allocation mémoire pour le tableau de fréquences
	int* freq = calloc(ALPHA_SIZE, sizeof(int));

	// Calcul des fréquences de chaque lettre du message crypté
	for (int i = 0; i < txt.size; i++) {
		unsigned int letter = (unsigned int) txt.data[i];
		if (letter >= 'A' && letter <= 'Z') {
			++freq[letter-'A'];
		}
	}

	// Détermination de la clé suivant la lettre la plus souvent répété (E en français)
	int key = 0;
	for (int i = 0; i < ALPHA_SIZE; i++) {
		if (freq[i] > freq[key]) {
			key = i;
		}
	}

	printf("Most repeted letter = %c\n", 'A'+key);
	key = (key - 4) % 26;
	printf("Key = %d\n\n", key);

	// Décryptage du message
	for (int i = 0; i < txt.size; i++) {
		unsigned char c = txt.data[i];
		if (txt.data[i] >= 'A' && txt.data[i] <= 'Z') {
			int uncrypted = txt.data[i] - 'A' - key;
			if (uncrypted < 0) {
				uncrypted += ALPHA_SIZE;
			}
			c = (unsigned char) uncrypted + 'A';
		}
		printf("%c", c);
	}

	printf("\n");

	array_destroy(&txt);
	free(freq);

	return 0;
}
