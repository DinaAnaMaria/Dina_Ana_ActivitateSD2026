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

struct nod {
	Masina info;
	struct nod* stanga;
	struct nod* dreapta;
};
typedef struct nod nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (fgets(buffer, 100, file) == NULL) {
		Masina m;
		m.id = -1;
		return m;
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
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

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

int calculeazaInaltimeArbore(nod* arbore) {
	if (arbore != NULL) {
		int inaltimeStanga = calculeazaInaltimeArbore(arbore->stanga);
		int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dreapta);
		return (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta) + 1;
	}
	return 0;
}

int calculeazaGE(nod* arbore) {
	if (arbore == NULL) return 0;
	return calculeazaInaltimeArbore(arbore->stanga) - calculeazaInaltimeArbore(arbore->dreapta);
}

void rotireStanga(nod** arbore) {
	nod* aux = (*arbore)->dreapta;
	(*arbore)->dreapta = aux->stanga;
	aux->stanga = (*arbore);
	(*arbore) = aux;
}

void rotireDreapta(nod** arbore) {
	nod* aux = (*arbore)->stanga;
	(*arbore)->stanga = aux->dreapta;
	aux->dreapta = (*arbore);
	(*arbore) = aux;
}

void adaugaMasinaInArboreEchilibrat(nod** arbore, Masina masinaNoua) {
	if ((*arbore) != NULL)
	{
		if ((*arbore)->info.id < masinaNoua.id)
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->dreapta), masinaNoua);
		}
		else
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->stanga), masinaNoua);
		}

		int gradEchilibru = calculeazaGE(*arbore);
		if (gradEchilibru == 2)
		{
			if (calculeazaGE((*arbore)->stanga) == 1)
			{
				rotireDreapta(arbore);
			}
			else
			{
				rotireStanga(&((*arbore)->stanga));
				rotireDreapta(arbore);
			}
		}
		else if (gradEchilibru == -2)
		{
			if (calculeazaGE((*arbore)->dreapta) != -1)
			{
				rotireDreapta(&((*arbore)->dreapta));
			}
			rotireStanga(arbore);
		}
	}
	else
	{
		(*arbore) = (nod*)malloc(sizeof(nod));
		(*arbore)->info = masinaNoua;
		(*arbore)->stanga = NULL;
		(*arbore)->dreapta = NULL;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) {
		printf("Eroare la deschiderea fisierului!\n");
		return NULL;
	}
	nod* nodNou = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		if (m.id != -1) {
			adaugaMasinaInArboreEchilibrat(&nodNou, m);
		}
	}
	fclose(f);
	return nodNou;
}

void afisarePreOrdineRSD(nod* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisarePreOrdineRSD(radacina->stanga);
		afisarePreOrdineRSD(radacina->dreapta);
	}
}

void afisareInOrdineSRD(nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdineSRD(radacina->stanga);
		afisareMasina(radacina->info);
		afisareInOrdineSRD(radacina->dreapta);
	}
}

void afisarePostOrdineSDR(nod* radacina) {
	if (radacina != NULL) {
		afisarePostOrdineSDR(radacina->stanga);
		afisarePostOrdineSDR(radacina->dreapta);
		afisareMasina(radacina->info);
	}
}

void afisareMasiniDinArbore(nod* radacina) {
	printf("--- AFISARE PREORDINE (RSD) ---\n");
	afisarePreOrdineRSD(radacina);

	printf("--- AFISARE INORDINE (SRD) ---\n");
	afisareInOrdineSRD(radacina);

	printf("--- AFISARE POSTORDINE (SDR) ---\n");
	afisarePostOrdineSDR(radacina);
}

void dezalocareArboreDeMasini(nod** radacina) {
	if (*radacina != NULL) {
		dezalocareArboreDeMasini(&((*radacina)->stanga));
		dezalocareArboreDeMasini(&((*radacina)->dreapta));

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);

		free(*radacina);
		*radacina = NULL;
	}
}

Masina getMasinaByID(nod* radacina, int id) {
	if (radacina != NULL) {
		if (id < radacina->info.id) {
			return getMasinaByID(radacina->stanga, id);
		}
		else if (id > radacina->info.id) {
			return getMasinaByID(radacina->dreapta, id);
		}
		else {
			return radacina->info; 
		}
	}
	Masina mGol;
	mGol.id = -1; 
	return mGol;
}

int determinaNumarNoduri(nod* radacina) {
	if (radacina == NULL) return 0;
	return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}

float calculeazaPretTotal(nod* radacina) {
	if (radacina == NULL) return 0;
	return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(nod* radacina, const char* numeSofer) {
	if (radacina == NULL) return 0;

	float pretCurent = 0;
	if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
		pretCurent = radacina->info.pret;
	}

	return pretCurent + calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer) +
		calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
}

int main() {
	nod* radacina = (nod*)citireArboreDeMasiniDinFisier("masini.txt");

	if (radacina != NULL) {
		afisareMasiniDinArbore(radacina);
		printf("Inaltime arbore: %d\n\n", calculeazaInaltimeArbore(radacina));

		printf("Numar total de noduri: %d\n\n", determinaNumarNoduri(radacina));

		printf("Pret total al tuturor masinilor: %.2f\n\n", calculeazaPretTotal(radacina));

		int idCautat = 3;
		Masina masinaCautata = getMasinaByID(radacina, idCautat);
		if (masinaCautata.id != -1) {
			printf("--- Masina gasita cu ID %d ---\n", idCautat);
			afisareMasina(masinaCautata);
		}
		else {
			printf("Masina cu ID %d nu exista in arbore.\n\n", idCautat);
		}

		const char* soferCautat = "Ionescu";
		printf("Pret total masini pentru soferul %s: %.2f\n\n", soferCautat, calculeazaPretulMasinilorUnuiSofer(radacina, soferCautat));

		dezalocareArboreDeMasini(&radacina);
		printf("Arbore dezalocat cu succes.\n");
	}

	return 0;
}