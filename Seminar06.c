#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod nod;
struct Nod {
	Masina info;
	nod* next;
};

struct HashTable {
	int dim;
	nod** vector; 
};
typedef struct HashTable HashTable;


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[] = ",\n";
	Masina m1;
	m1.id = -1;
	if (fgets(buffer, 100, file)) {
		char* aux = strtok(buffer, sep);
		if (aux) m1.id = atoi(aux);
		aux = strtok(NULL, sep);
		if (aux) m1.nrUsi = atoi(aux);
		aux = strtok(NULL, sep);
		if (aux) m1.pret = (float)atof(aux);
		aux = strtok(NULL, sep);
		if (aux) {
			m1.model = _strdup(aux);
		}
		aux = strtok(NULL, sep);
		if (aux) {
			m1.numeSofer = _strdup(aux);
		}
		aux = strtok(NULL, sep);
		if (aux) m1.serie = aux[0];
	}
	return m1;
}

void afisareMasina(Masina masina) {
	printf("ID: %d, Sofer: %s, Model: %s, Pret: %.2f\n", masina.id, masina.numeSofer, masina.model, masina.pret);
}

void afisareListaMasini(nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(nod** cap, Masina masinaNoua) {
	nod* temp = (nod*)malloc(sizeof(nod));
	temp->info = masinaNoua;
	temp->next = NULL;
	if ((*cap) == NULL) {
		(*cap) = temp;
	}
	else {
		nod* aux = (*cap);
		while (aux->next != NULL) aux = aux->next;
		aux->next = temp;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (nod**)malloc(sizeof(nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* numeSofer, int dimensiune) {
	if (dimensiune <= 0) return -1;
	unsigned int suma = 0;
	for (int i = 0; i < (int)strlen(numeSofer); i++) {
		suma += (unsigned char)numeSofer[i];
	}
	return suma % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	if (masina.id != -1) {
		int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
		adaugaMasinaInLista(&(hash.vector[pozitie]), masina);
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dim) {
	FILE* f = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dim);
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			if (m.id != -1) inserareMasinaInTabela(hash, m);
		}
		fclose(f);
	}
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL) {
			printf("Cluster %d:\n", i);
			afisareListaMasini(ht.vector[i]);
		}
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		nod* p = ht->vector[i];
		while (p) {
			nod* aux = p;
			p = p->next;
			free(aux->info.model);
			free(aux->info.numeSofer);
			free(aux);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL) (*nrClustere)++;
	}

	if (*nrClustere == 0) return NULL;

	float* medii = (float*)malloc(sizeof(float) * (*nrClustere));
	int k = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL) {
			float suma = 0;
			int contor = 0;
			nod* aux = ht.vector[i];
			while (aux) {
				suma += aux->info.pret;
				contor++;
				aux = aux->next;
			}
			medii[k++] = suma / contor;
		}
	}
	return medii;
}

Masina getMasinaDupaCheie(HashTable ht, const char* numeSofer) {
	Masina m; m.id = -1;
	int poz = calculeazaHash(numeSofer, ht.dim);
	nod* aux = ht.vector[poz];
	while (aux) {
		if (strcmp(aux->info.numeSofer, numeSofer) == 0) {
			m = aux->info;
			m.model = _strdup(aux->info.model);
			m.numeSofer = _strdup(aux->info.numeSofer);
			return m;
		}
		aux = aux->next;
	}
	return m;
}

int main() {
	HashTable hash = citireMasiniDinFisier("masini.txt", 5);
	afisareTabelaDeMasini(hash);

	int nrRezultate = 0;
	float* medii = calculeazaPreturiMediiPerClustere(hash, &nrRezultate);
	if (medii) {
		printf("\nMedii per clustere ocupate:\n");
		for (int i = 0; i < nrRezultate; i++) printf("%.2f ", medii[i]);
		printf("\n");
		free(medii);
	}

	dezalocareTabelaDeMasini(&hash);
	return 0;
}