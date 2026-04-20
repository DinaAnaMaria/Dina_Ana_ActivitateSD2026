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

struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct listaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct listaDubla listaDubla;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[] = ",\n"; 
	Masina m1;
	m1.id = -1; 

	if (fgets(buffer, 100, file)) {
		char* aux;
		aux = strtok(buffer, sep);
		if (aux) m1.id = atoi(aux);

		aux = strtok(NULL, sep);
		if (aux) m1.nrUsi = atoi(aux);

		aux = strtok(NULL, sep);
		if (aux) m1.pret = (float)atof(aux);

		aux = strtok(NULL, sep);
		if (aux) {
			m1.model = (char*)malloc(strlen(aux) + 1);
			strcpy(m1.model, aux);
		}

		aux = strtok(NULL, sep);
		if (aux) {
			m1.numeSofer = (char*)malloc(strlen(aux) + 1);
			strcpy(m1.numeSofer, aux);
		}

		aux = strtok(NULL, sep);
		if (aux) m1.serie = aux[0];
	}
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d | Usi: %d | Pret: %.2f | Model: %s | Sofer: %s | Serie: %c\n",
		masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}

void afisareListaMasiniDeLaInceput(listaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void adaugaMasinaInLista(listaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->last;

	if (lista->last != NULL) {
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

listaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	listaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;

	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			if (m.id != -1) { 
				adaugaMasinaInLista(&lista, m);
			}
		}
		fclose(f);
	}
	return lista;
}

void dezalocareLDMasini(listaDubla* lista) {
	Nod* p = lista->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		free(aux->masina.model);
		free(aux->masina.numeSofer);
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

void stergeMasinaDupaID(listaDubla* lista, int id) {
	if (lista->first == NULL) return;

	Nod* p = lista->first;
	while (p != NULL && p->masina.id != id) {
		p = p->next;
	}

	if (p == NULL) return; 

	
	if (p->prev == NULL) { 
		lista->first = p->next;
		if (lista->first) {
			lista->first->prev = NULL;
		}
		else {
			lista->last = NULL; 
		}
	}
	else {
		p->prev->next = p->next;
	}

	if (p->next == NULL) { 
		lista->last = p->prev;
		if (lista->last) {
			lista->last->next = NULL;
		}
		else {
			lista->first = NULL;
		}
	}
	else {
		p->next->prev = p->prev;
	}

	free(p->masina.model);
	free(p->masina.numeSofer);
	free(p);
	lista->nrNoduri--;
}

float calculeazaPretMediu(listaDubla lista) {
	if (lista.nrNoduri <= 0) return 0;
	float suma = 0;
	Nod* p = lista.first;
	while (p) {
		suma += p->masina.pret;
		p = p->next;
	}
	return suma / lista.nrNoduri;
}

int main() {
	listaDubla lista = citireLDMasiniDinFisier("masini.txt");

	printf("Lista initiala:\n");
	afisareListaMasiniDeLaInceput(lista);

	printf("Pretul mediu inainte de stergere: %.2f\n", calculeazaPretMediu(lista));

	int idDeSters = 3;
	printf("\nStergem masina cu ID %d...\n", idDeSters);
	stergeMasinaDupaID(&lista, idDeSters);

	printf("\nLista dupa stergere:\n");
	afisareListaMasiniDeLaInceput(lista);

	dezalocareLDMasini(&lista);
	return 0;
}