
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Masina info; //informatia utila de tip masina 
	struct Nod* next; //pointer care retine adresa urmatorului nod
};

typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua; //shallow copy
	nou->next = NULL;
	if (*cap) {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *cap;
	*cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			adaugaMasinaInLista(&cap, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return cap;
}

void dezalocareListaMasini(Nod** cap) {
	//sunt dezalocate toate masinile si lista de elemente
	while (*cap) {
		Nod* p = *cap;
		(*cap) = p->next;
		if (p->info.model) {
			free(p->info.model);
		}
		if (p->info.numeSofer) {
			free(p->info.numeSofer);
		}
		free(p);
	}

}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int k = 0;
	while (cap) {
		suma += cap->info.pret;
		k++;
		cap = cap->next;
	}
	if (k > 0) {
		return suma / k;
	}
	return 0;
}

void stergeMasiniDinSeria(Nod** cap, const char* numeSofer) {
	while ((*cap) && (*cap)->info.serie == numeSofer) {
		Nod* aux = *cap;
		(*cap) = aux->next;
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		if (aux->info.model) {
			free(aux->info.model);
		}
		free(aux);
	}
	if ((*cap)) {
		Nod* p = *cap;
		while (p) {
			while (p->next && p->next->info.serie != numeSofer) {
				p = p->next;
			}
			if (p->next) {
				Nod* aux = p->next;
				p->next = aux->next;
				if (aux->info.numeSofer) {
					free(aux->info.numeSofer);
				}
				if (aux->info.model) {
					free(aux->info.model);
				}
				free(aux);
			}
			else {
				p = NULL;
			}
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

int main() {

	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);
	printf("Pretul mediu este: %.2f\n", calculeazaPretMediu(cap));
	printf("Pretyl masinilor unui sofer este: %.2f\n", calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));
	dezalocareListaMasini(&cap);

	return 0;
}