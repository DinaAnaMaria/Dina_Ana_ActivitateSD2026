#include<stdio.h>
#include<malloc.h>

struct Film {
	int id;
	int durata;
	char* denumire;
	float buget;
	char varstaMinima;
};
struct Film initializare(int _id, int _durata, const char* _denumire, float _buget, char _varstaMinima){
	struct Film s;
	s.id = _id;
	s.durata = _durata;
	s.denumire = malloc((strlen(_denumire) + 1) + sizeof(char));
	strcpy(s.denumire, _denumire);
	s.buget = _buget;
	s.varstaMinima = _varstaMinima;
	return s;
}

struct Film copyFilm(struct Film s) {
	return initializare(s.id, s.durata, s.denumire, s.buget, s.varstaMinima);
}

void afisare(struct Film s) {
	printf("Id: %d\n", s.id);
	printf("Nume: %s\n", s.denumire);
	printf("Durata in minute: %d\n", s.durata);
	printf("Buget: %.2f\n", s.buget); 
	printf("Varsta minima: %d\n", s.varstaMinima);
}

void afisareVector(struct Film* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
		printf("\n");
	}
}

struct Film* copiazaPrimeleNElemente(struct Film* vector, int nrElemente, int nrElementeCopiate) {
	struct Film* vectorNou = malloc(nrElementeCopiate * sizeof(struct Film));
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = copyFilm(vector[i]);
	}

	return vectorNou;
}

void dezalocare(struct Film** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].denumire); 
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaAnumiteElemente(struct Film* vector, char nrElemente, float bugetMaxim, struct Film** vectorNou, int* dimensiune) {
	int count = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].buget >= bugetMaxim) {
			count++;
		}
	}

	*dimensiune = count;
	if (count > 0) {
		*vectorNou = (struct Film*)malloc(sizeof(struct Film) * (*dimensiune));

		int k = 0;
		for (int i = 0; i < nrElemente; i++) {
			if (vector[i].buget >= bugetMaxim) {
				(*vectorNou)[k] = copyFilm(vector[i]);
				k++;
			}
		}
	}
}

struct Film getPrimulElementConditionat(struct Film* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Film s;
	s.id = 1;

	return s;
}
	


int main() {
	struct Film f1 = initializare(1, 120, "Dune", 20.6, 14);

	int nrFilme = 3;
	struct Film* filme = malloc(nrFilme * sizeof(struct Film));
	filme[0] = copyFilm(f1);
	*(filme + 1) = initializare(2, 360, "James Bond", 16, 12);
	filme[2] = initializare(3, 222, "Cloud9", 50.6, 12);

	printf("Vector\n");
	afisareVector(filme, nrFilme);

	int nrFilmeCopiate = 2;
	struct Film* vectorNou = copiazaPrimeleNElemente(filme, nrFilme, nrFilmeCopiate);
	printf("\nVector Copiatn", nrFilmeCopiate);
	afisareVector(vectorNou, nrFilmeCopiate);

	struct Film* vectorBuget = NULL;
	int nrBuget = 0;
	float prag = 20.0;
	copiazaAnumiteElemente(filme, nrFilme, prag, &vectorBuget, &nrBuget);

	printf("\n--- Filme cu buget >= %.2f ---\n", prag);
	afisareVector(vectorBuget, nrBuget);

	dezalocare(&vectorBuget, &nrBuget);
	dezalocare(&vectorNou, &nrFilmeCopiate);
	dezalocare(&filme, &nrFilme);

	printf("\nDezalocare reusita!\n");

	return 0;
}