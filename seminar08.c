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
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

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

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if ((*radacina) != NULL) {
		if (masinaNoua.id < (*radacina)->info.id) {
			adaugaMasinaInArbore(&((*radacina)->st), masinaNoua);
		}
		else if (masinaNoua.id > (*radacina)->info.id) {
			adaugaMasinaInArbore(&((*radacina)->dr), masinaNoua);
		}
	}
	else {
		(*radacina) = (Nod*)malloc(sizeof(Nod));
		(*radacina)->info = masinaNoua;
		(*radacina)->st = NULL;
		(*radacina)->dr = NULL;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) {
		printf("Eroare la deschiderea fisierului!\n");
		return NULL;
	}
	Nod* rad = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != -1) {
			adaugaMasinaInArbore(&rad, m);
		}
	}
	fclose(f);
	return rad;
}

void afisarePreOrdineRSD(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisarePreOrdineRSD(radacina->st);
		afisarePreOrdineRSD(radacina->dr);
	}
}

void afisareInOrdineSRD(Nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdineSRD(radacina->st);
		afisareMasina(radacina->info);
		afisareInOrdineSRD(radacina->dr);
	}
}

void afisarePostOrdineSDR(Nod* radacina) {
	if (radacina != NULL) {
		afisarePostOrdineSDR(radacina->st);
		afisarePostOrdineSDR(radacina->dr);
		afisareMasina(radacina->info);
	}
}

void afisareMasiniDinArbore(Nod* radacina) {
	printf("PARCURGERE PREORDINE\n");
	afisarePreOrdineRSD(radacina);

	printf("PARCURGERE INORDINE\n");
	afisareInOrdineSRD(radacina);

	printf("PARCURGERE POSTORDINE\n");
	afisarePostOrdineSDR(radacina);
}
void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina != NULL) {
		dezalocareArboreDeMasini(&((*radacina)->st));
		dezalocareArboreDeMasini(&((*radacina)->dr));

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		*radacina = NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	if (radacina != NULL) {
		if (id < radacina->info.id) {
			return getMasinaByID(radacina->st, id);
		}
		else if (id > radacina->info.id) {
			return getMasinaByID(radacina->dr, id);
		}
		else {
			return radacina->info;
		}
	}
	Masina mGol;
	mGol.id = -1;
	return mGol;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina == NULL) return 0;
	return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina == NULL) return 0;
	int altSt = calculeazaInaltimeArbore(radacina->st);
	int altDr = calculeazaInaltimeArbore(radacina->dr);
	return (altSt > altDr ? altSt : altDr) + 1;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina == NULL) return 0;
	return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina == NULL) return 0;

	float pretCurent = 0;
	if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
		pretCurent = radacina->info.pret;
	}
	return pretCurent + calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer) +
		calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);
}

int main() {

	return 0;
}