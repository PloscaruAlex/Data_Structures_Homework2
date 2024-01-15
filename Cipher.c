#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Cipher.h"



/* Construieste un multi-dictionar pe baza unui fisier text
 * Cheia (elem) unui nod va fi reprezentata de un cuvant din text
 * iar valoarea (info) va fi indexul de inceput al acelui cuvant
 * ignorand caracterele separatoare (",.? \n\r") - i.e. numarul
 * de caractere A-Z precedente
 *
 * E.g: THIS IS AN EXAMPLE
 *
 * 			IS (4)
 *		    / \
 * 	  (6) AN  THIS (0)
 *         \
 *	 	 EXAMPLE (8)
 * */
void buildTreeFromFile(char* fileName, TTree* tree) {

	// Verificarea argumentelor
	if(fileName == NULL || tree == NULL) {
		return;
	}
	FILE* f = fopen(fileName, "r");
	if (f == NULL) {
		return;
	}
	char* buffer = (char*) malloc(BUFLEN * sizeof(char));
	char* text = (char*) malloc(BUFLEN * sizeof(char));
	text[0] = '\0';
	int contor = 0;
	int *p = &contor;

	while (fgets(buffer, BUFLEN, f) != NULL) {
		if (buffer == NULL) {
			break;
		}
		char* token = strtok(buffer, ",.? \n\r");
		while (token != NULL) {
			insert(tree, token, p);
			for(int i = 0; i < strlen(token); i++) {
				text[contor] = token[i];
				contor++;
			}
			token = strtok(NULL, ",.? \n\r");
		}
	}
	free(buffer);
	free(text);
	fclose(f);
}


/* Functie pentru afisarea unei chei de criptare
 * O cheie este reprezentata print-o serie de offset-uri
 *
 * E.g: key = [1, 2, 3, 4]
 * input text  = A    A    A    A
 *			   +1|  +2|  +3|  +4|
 * 				 V    V    V    V
 * output text = B    C    D    E
 *
 */
void printKey(char *fileName, Range *key) {

	FILE *f = fopen(fileName, "w");

	if (key == NULL) {
		fprintf(f, "No key provided!\n");
		fclose(f);
		return;
	}

	fprintf(f, "Decryption key of length %d is:\n", key->size);

	// Afiseaza fiecare offset din cheie % 26 (lungimea
	// alfabetului A-Z)
	for (int i = 0; i < key->size; i++) {
		fprintf(f, "%d ", key->index[i] % 26);

		// Rand nou pentru lizibilitate
		if ((i + 1) % 10 == 0)
			fprintf(f, "\n");
	}

	fclose(f);
}


/* Returneaza cheia obtinuta de parcurgerea in ordine
 * crescatoare a nodurilor arborelui (parcurgand si listele
 * de duplicate)
 */
Range* inorderKeyQuery(TTree* tree) {
	Range* key = (Range*) malloc(sizeof(Range));
	key->capacity = key->size = tree->size;
	TreeNode* list = minimum(tree->root);
	int contor = 0;
	key->index = (int*) malloc(key->size * sizeof(int));
	while (list != NULL) {
		key->index[contor] = *(int*)(list->info);
		list = list->next;
		contor++;
	}
	return key;
}

void getLevel(TTree* tree, TreeNode* node, int* levels, int levelIndex) {
	TreeNode* iter = tree->root;
	int l = 1;
	int c = tree->compare(iter->elem, node->elem);
	while (c != 0) {
		l++;
		if (c == 1) {
			iter = iter->left;
		} else {
			iter = iter->right;
		}
		c = tree->compare(iter->elem, node->elem);
	}
	levels[levelIndex] = l;
}

/* Functie pentru extragerea cheii formate din valorile
 * nodurilor de la nivelul ce contine cel mai frecvent cuvant
 * (in cazul in care exista mai multe cuvinte cu numar maxim
 * de aparitii atunci se va considera primul nod dintre acestea conform
 * parcurgerii in inordine a arborelui)
 */
Range* levelKeyQuery(TTree* tree) {
	int maxNumberAparitions = 0;
	TreeNode* list = minimum(tree->root); 
	TreeNode* mostFrequent = NULL;
	while (list != NULL) {
		int nr = 0;
		TreeNode* iter = list;
		while (iter != list->end) {
			nr++;
			iter = iter->next;
		}
		if (nr > maxNumberAparitions) {
			maxNumberAparitions = nr;
			mostFrequent = list;
		}
		list = list->end->next;
	}

	int* levels = (int*) malloc(tree->size * sizeof(int));
	int levelIndex = 0;
	list = minimum(tree->root);
	while (list != NULL) {
		getLevel(tree, list, levels, levelIndex);
		list = list->next;
		levelIndex++;
	}

	TreeNode* iter = tree->root;
	int l = 1;
	while (tree->compare(iter->elem, mostFrequent->elem) != 0) {
		l++;
		if (tree->compare(iter->elem, mostFrequent->elem) == 1) {
			iter = iter->left;
		} else {
			iter = iter->right;
		}
	}

	int contor = 0;
	iter = minimum(tree->root);
	levelIndex = 0;
	while (iter != NULL) {
		if (levels[levelIndex] == l) {
			contor++;
		}
		iter = iter->next;
		levelIndex++;
	}

	Range* key = (Range*) malloc(sizeof(Range));
	key->capacity = key->size = contor;
	key->index = (int*) malloc(key->size * sizeof(int));
	contor = 0;

	iter = minimum(tree->root);
	levelIndex = 0;
	while (iter != NULL) {
		if (levels[levelIndex] == l) {
			key->index[contor] = *(int*)(iter->info);
			contor++;
		}
		iter = iter->next;
		levelIndex++;
	}
	free(levels);
	return key;
}


/* Extragerea cheii din nodurile aflate intr-un anumit
 * domeniu de valori specificat
 */
Range* rangeKeyQuery(TTree* tree, char* q, char* p) {
	TreeNode* first = NULL;
	TreeNode* last = NULL;
	TreeNode* list = minimum(tree->root);

	while (list != NULL) {
		if (first == NULL) {
			if (tree->compare(list->elem, q) >= 0) {
				first = list;
			}
		}
		if (tree->compare(list->elem, p) <= 0) {
			last = list;
		}
		list = list->next;
	}
	int s = 0;
	TreeNode* iter = first;
	while (iter != last->next) {
		s++;
		iter = iter->next;
	}

	Range* key = (Range*) malloc(sizeof(Range));
	key->capacity = key->size = s;
	key->index = (int*) malloc(key->size * sizeof(int));
	int contor = 0;

	iter = first;
	while (iter != last->next) {
		key->index[contor] = *(int*)(iter->info);
		contor++;
		iter = iter->next;
	}
	return key;
}


void encrypt(char *inputFile, char *outputFile, Range *key) {

	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') + 
				    key->index[idx] % 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}


void decrypt(char *inputFile, char *outputFile, Range *key) {
 	
	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') - 
				    (key->index[idx] % 26) + 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}