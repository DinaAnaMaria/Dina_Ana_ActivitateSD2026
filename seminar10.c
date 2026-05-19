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
	Masina info;
	struct Nod* urmator;
};
typedef struct Nod Nod;

struct Coada {
	Nod* cap;
	Nod* coada;
};
typedef struct Coada Coada;


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (fgets(buffer, 100, file) == NULL) {
		Masina mGol;
		mGol.id = -1;
		return mGol;
	}
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	if (aux == NULL) {
		m1.id = -1;
		return m1;
	}
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

void pushStack(Nod** varf, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->urmator = *varf;
	*varf = nou;
}

Masina popStack(Nod** varf) {
	if (*varf == NULL) {
		Masina mGol;
		mGol.id = -1;
		return mGol;
	}
	Nod* aux = *varf;
	Masina m = aux->info;
	*varf = (*varf)->urmator;
	free(aux);
	return m;
}

int emptyStack(Nod* varf) {
	return varf == NULL ? 1 : 0;
}

void* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) return NULL;

	Nod* varfStiva = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != -1) {
			pushStack(&varfStiva, m);
		}
	}
	fclose(f);
	return varfStiva;
}

void dezalocareStivaDeMasini(Nod** varf) {
	while (!emptyStack(*varf)) {
		Masina m = popStack(varf);
		free(m.model);
		free(m.numeSofer);
	}
}

int size(Nod* varf) {
	int contor = 0;
	Nod* curent = varf;
	while (curent != NULL) {
		contor++;
		curent = curent->urmator;
	}
	return contor;
}


void enqueue(Coada* coadaSec, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->urmator = NULL;

	if (coadaSec->coada == NULL) {
		coadaSec->cap = nou;
		coadaSec->coada = nou;
	}
	else {
		coadaSec->coada->urmator = nou;
		coadaSec->coada = nou;
	}
}

Masina dequeue(Coada* coadaSec) {
	if (coadaSec->cap == NULL) {
		Masina mGol;
		mGol.id = -1;
		return mGol;
	}
	Nod* aux = coadaSec->cap;
	Masina m = aux->info;
	coadaSec->cap = coadaSec->cap->urmator;

	if (coadaSec->cap == NULL) {
		coadaSec->coada = NULL;
	}
	free(aux);
	return m;
}

void* citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) return NULL;

	Coada* c = (Coada*)malloc(sizeof(Coada));
	c->cap = NULL;
	c->coada = NULL;

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != -1) {
			enqueue(c, m);
		}
	}
	fclose(f);
	return c;
}

void dezalocareCoadaDeMasini(Coada* coadaSec) {
	while (coadaSec->cap != NULL) {
		Masina m = dequeue(coadaSec);
		free(m.model);
		free(m.numeSofer);
	}
	free(coadaSec);
}

Masina getMasinaByID(Nod* varf, int id) {
	Nod* curent = varf;
	while (curent != NULL) {
		if (curent->info.id == id) {
			return curent->info;
		}
		curent = curent->urmator;
	}
	Masina mGol;
	mGol.id = -1;
	return mGol;
}

float calculeazaPretTotal(Nod* varf) {
	float total = 0;
	Nod* curent = varf;
	while (curent != NULL) {
		total += curent->info.pret;
		curent = curent->urmator;
	}
	return total;
}
int main() {
	Nod* stiva = (Nod*)citireStackMasiniDinFisier("masini.txt");

	if (stiva != NULL) {
		printf("Numarul de elemente in stiva: %d\n\n", size(stiva));
		printf("Pretul total al masinilor: %.2f\n\n", calculeazaPretTotal(stiva));

		int idCautat = 2;
		Masina gasita = getMasinaByID(stiva, idCautat);
		if (gasita.id != -1) {
			printf("Masina cu ID-ul %d gasita in stiva:\n", idCautat);
			afisareMasina(gasita);
		}

		printf("Extragere elemente din Stiva\n");
		while (!emptyStack(stiva)) {
			Masina m = popStack(&stiva);
			afisareMasina(m);
			free(m.model);
			free(m.numeSofer);
		}
		dezalocareStivaDeMasini(&stiva);
	}

	

	return 0;
}