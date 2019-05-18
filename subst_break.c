// Timothée GUY

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

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

void array_swap(struct array *self, size_t i, size_t j) {
  char tmp = self->data[i];
  self->data[i] = self->data[j];
  self->data[j] = tmp;
}

void array_selection_sort(struct array *self) {
  for (size_t i = 0; i < self->size - 1; ++i) {
    size_t j = i;
    for (size_t k = j + 1; k < self->size; ++k) {
      if (self->data[k] < self->data[j]) {
        j = k;
      }
    }
    array_swap(self, i, j);
  }
}

int main(int argc, char *argv[]) {
	// Vérification du nombres d'arguments
	if (argc != 2) {
		printf("USAGE: ./subst_break [CRYPTED_MSG_PATH]\n\n");
		exit(1);
	}

	printf("-------------SUBSTITUTION BREAK-------------\n\n");

	// Création du tableau dynamique
	struct array txt;
	array_create(&txt);

	// Allocation mémoire de la clé
	char *key = calloc(ALPHA_SIZE+1, sizeof(char));
	char *alpha_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	// initialisation de la clé
	for (int i = 0; i < ALPHA_SIZE; ++i) {
		key[i] = '_';
	}

	// Ouverture du fichier
	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		printf("Error while opening file, please check the path\n\n");
		exit(1);
	}

	// Lecture du texte et ajout dans le tableau dynamique
	int c;
	while ((c = fgetc(f)) != EOF) {
		array_add(&txt, (char) c);
	}
	fclose(f);

	// Création du tableau de fréquences
	int *freq = calloc(ALPHA_SIZE, sizeof(int));
	for (int i = 0; i < txt.size; ++i) {
		if (txt.data[i] >= 'A' && txt.data[i] <= 'Z') {
			++freq[txt.data[i]-'A'];
		}
	}

	// On trouve la lettre E grâce aux fréquences
	int E = 0;
	for (int i = 0; i < ALPHA_SIZE; ++i) {
		if (freq[i] > freq[E]) {
			E = i;
		}
	}
	free(freq);

	key['E'-'A'] = 'A'+E;
	printf("%c -> E\n", 'A'+E);

	// On cherche la lettre A en calculant les fréquences des lettres seules
	int *freq_alone = calloc(ALPHA_SIZE, sizeof(int));
	for (int i = 1; i < txt.size; ++i) {
		if (isspace(txt.data[i-1]) && isspace(txt.data[i+1])
				&& txt.data[i] >= 'A' && txt.data[i] <= 'Z') {
			++freq_alone[txt.data[i]-'A'];
		}
	}

	// La lettre la plus fréquemment seule est un A
	int A = 0;
	for (int i = 0; i < ALPHA_SIZE; ++i) {
		if (freq_alone[i] > freq_alone[A]) {
			A = i;
		}
	}

	// La seconde lettre la plus fréquemment seule est un Y
	int Y = 0;
	for (int i = 0; i < ALPHA_SIZE; ++i) {
		if (freq_alone[i] > freq_alone[Y] && i != A) {
			Y = i;
		}
	}

	// Si les fréquences ne sont pas nulles, on change la clé en conséquence
	if (freq_alone[A] != 0) {
		key['A'-'A'] = 'A'+A;
		printf("%c -> A\n", 'A'+A);
	}
	if (freq_alone[Y] != 0) {
		key['Y'-'A'] = 'A'+Y;
		printf("%c -> Y\n", 'A'+Y);
	}
	free(freq_alone);

	// On trouve N en calculant les fréquences des mots de deux lettres commençant par E
	int *freq_two = calloc(ALPHA_SIZE, sizeof(int));
	for (int i = 2; i < txt.size-1; ++i) {
		if (txt.data[i-1] == 'A'+E && !isalpha(txt.data[i+1]) && isalpha(txt.data[i]) && !isalpha(txt.data[i-2])) {
			++freq_two[txt.data[i]-'A'];
		}
	}

	// Le mot de deux lettres le plus fréquents étant EN, on en déduit N
	int N = 0;
	for (int i = 0; i < ALPHA_SIZE; ++i) {
		if (freq_two[i] > freq_two[N] && key[i] == '_') {
			N = i;
		}
	}

	// Si la fréquence n'est pas nulle, on change la clé en conséquence
	if (freq_two[N] != 0) {
		key['N'-'A'] = 'A'+N;
		printf("%c -> N\n", 'A'+N);
	}
	free(freq_two);

	// On trouve Q et U en calculant les fréquences des mots de 3 lettres se finissant par E
	int n = 0;
	// Tableau contenant les mots de 3 lettres
	char trig_xxE[74][3]; // Il y a environ 74 mots de 3 lettres se terminant par E dans la langue française
	bool b = false;
	// Tableau contenant les fréquences de chaque mots de 3 lettres
	int	freq_xxE[74];
	// Initialisation des tableaux
	for (int i = 0; i < 74; ++i) {
		trig_xxE[i][0] = 'a';
		trig_xxE[i][1] = 'a';
		trig_xxE[i][2] = 'a';
		freq_xxE[i] = 0;
	}

	// Parcourt du texte
	for (int i = 0; i < txt.size-3; ++i) {
		// Si c'est un mot de 3 lettres se finissant par E
		if (txt.data[i+2] == 'A'+E && !isalpha(txt.data[i-1]) && !isalpha(txt.data[i+3]) && isalpha(txt.data[i]) && isalpha(txt.data[i+1])) {
			for (int j = 0; j < n; ++j) {
				// Si le mot est déjà contenu dans le tableau de mots, on augmente sa fréquence
				if (txt.data[i] == trig_xxE[j][0] && txt.data[i+1] == trig_xxE[j][1]) {
					++freq_xxE[j];
					b = true;
				}
			}

			// Sinon on ajoute le mot dans le tableau de mots
			if (!b) {
				trig_xxE[n][0] = txt.data[i];
				trig_xxE[n][1] = txt.data[i+1];
				trig_xxE[n][2] = txt.data[i+2];
				++n;
			}

			b = false;
		}
	}

	// Le mot de 3 lettres se terminant par E le plus fréquent étant QUE, on en déduit Q et U
	int QU = 0;
	if (trig_xxE[0][1] == key['N'-'A']) {
		++QU;
	}
	for (int i = 0; i < 74; ++i) {
		if (freq_xxE[i] > freq_xxE[QU] && trig_xxE[i][1] != key['N'-'A']) {
			QU = i;
		}
	}

	// Si la fréquence n'est pas nulle, on change la clé en conséquence
	if (freq_xxE[QU] != 0) {
		key['Q'-'A'] = trig_xxE[QU][0];
		printf("%c -> Q\n", trig_xxE[QU][0]);
		key['U'-'A'] = trig_xxE[QU][1];
		printf("%c -> U\n", trig_xxE[QU][1]);
	}

	// On trouve S et T en calculant les fréquences des mots de 3 lettres commençant par E
	n = 0;
	// Tableau contenant les mots de 3 lettres
	char trig_Exx[20][3]; // Il existe environ 20 mots de 3 lettres commençant par E dans la langue française
	b = false;
	// Fréquence des mots de 3 lettres
	int	freq_Exx[20];
	// Initialisation des tableaux
	for (int i = 0; i < 20; ++i) {
		trig_Exx[i][0] = 'a';
		trig_Exx[i][1] = 'a';
		trig_Exx[i][2] = 'a';
		freq_Exx[i] = 0;
	}

	for (int i = 0; i < txt.size-2; ++i) {
		// Si c'est un mot de 3 lettres commençant par E
		if (txt.data[i] == 'A'+E && !isalpha(txt.data[i-1]) && !isalpha(txt.data[i+3])) {
			for (int j = 0; j < n; ++j) {
				// Si le mot fait déjà partie du tableau de mots on augmente sa fréquence
				if (txt.data[i] == trig_Exx[j][0] && txt.data[i+1] == trig_Exx[j][1] && txt.data[i+2] == trig_Exx[j][2]) {
					++freq_Exx[j];
					b = true;
				}
			}

			// Sinon on ajoute le mot au tableau de mots
			if (!b) {
				trig_Exx[n][0] = txt.data[i];
				trig_Exx[n][1] = txt.data[i+1];
				trig_Exx[n][2] = txt.data[i+2];
				++n;
			}

			b = false;
		}
	}

	// Le mot de 3 lettres commençant par E le plus fréquent étant EST, on en déduit S et T
	int ST = 0;
	for (int i = 0; i < 20; ++i) {
		if (freq_Exx[i] > freq_Exx[ST]) {
			ST = i;
		}
	}

	// Si la fréquence n'est pas nulle, on change la clé en conséquence
	int S = 0, T = 0;
	if (freq_Exx[ST] != 0) {
		S = trig_Exx[ST][1]-'A';
		key['S'-'A'] = trig_Exx[ST][1];
		printf("%c -> S\n", trig_Exx[ST][1]);
		T = trig_Exx[ST][2]-'A';
		key['T'-'A'] = trig_Exx[ST][2];
		printf("%c -> T\n", trig_Exx[ST][2]);
	}

	// On trouve C grâce au tiret dans le mot composé EST-CE
	n = 3;
	while (n != txt.size-2 && (txt.data[n] != '-' || txt.data[n-3] != 'A'+E || txt.data[n+2] != 'A'+E)) {
		++n;
	}

	// Si on a trouvé la lettre avant la fin du texte, on change la clé en conséquence
	if (n < txt.size-2) {
		key['C'-'A'] = txt.data[n+1];
		printf("%c -> C\n", txt.data[n+1]);
	}

	/* On trouve R en regardant les mots ayant pour avant-dernière lettre un E,
	car c'est la lettre la plus fréquente en fin de ces mots après les lettres N, S, T
	mais celle-ci sont trouvées précédemment*/
	int *freq_end_verb = calloc(ALPHA_SIZE, sizeof(int));
	for (int i = 1; i < txt.size-1; ++i) {
		if (txt.data[i-1] == 'A'+E && !isalpha(txt.data[i+1]) && isalpha(txt.data[i])) {
			++freq_end_verb[txt.data[i]-'A'];
		}
	}

	int R = 0;
	for (int i = 0; i < ALPHA_SIZE; ++i) {
		if (freq_end_verb[i] > freq_end_verb[R] && 'A'+N != i+'A' && 'A'+S != i+'A' && 'A'+T != i+'A') {
			R = i;
		}
	}

	// Si la fréquence n'est pas nulle, on change la clé en conséquence
	if (freq_end_verb[R] != 0) {
		key['R'-'A'] = 'A'+R;
		printf("%c -> R\n", 'A'+R);
	}
	free(freq_end_verb);

	// Affichage tant que la clé n'est pas complète
	bool complete = false;
	do {
		int count;
		// Affichage du texte en remplaçant les lettres déjà trouvées
		for (int i = 0; i < txt.size; ++i) {
			count = 0;
			while (key[count] != txt.data[i] && count < ALPHA_SIZE) {
				++count;
			}

			if (count != ALPHA_SIZE) {
				printf("%c", alpha_table[count]);
			}
			else {
				printf("%c", txt.data[i]);
			}
		}

		// Affichage de la table de substitution
		printf("\n\nsubstitution table: \n\t|%s|\n\t|%s|\n\n", alpha_table, key);

		// Interaction avec l'utilisateur
		printf("Veuillez entrer la lettre de base et la lettre de substitution (si Z est substitué par I, on entre ZI):\n");
		char s = fgetc(stdin);
		char s2 = fgetc(stdin);
		fgetc(stdin);

		// Changement de clé en fonction des lettres entrées par l'utilisateur
		if (s >= 'A' && s2 >= 'A' && s <= 'Z' && s2 <= 'Z') {
			key[s-'A'] = s2;
		}

		// On vérifie si la clé est complète
		complete = true;
		for (int i = 0; i < ALPHA_SIZE; ++i) {
			if (key[i] == '_') {
				complete = false;
			}
		}
	} while (!complete);

	// Dernier affichage lorsque la clé est complète
	int count;
	for (int i = 0; i < txt.size; ++i) {
		count = 0;
		while (key[count] != txt.data[i] && count < ALPHA_SIZE) {
			++count;
		}

		if (count != ALPHA_SIZE) {
			printf("%c", alpha_table[count]);
		}
		else {
			printf("%c", txt.data[i]);
		}
	}

	printf("\n\nsubstitution table: \n\t|%s|\n\t|%s|\n\n", alpha_table, key);

	// Libération mémoire
	array_destroy(&txt);
	free(key);

	return 0;
}
