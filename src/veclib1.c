#include "veclib1.h"

double trei_z(double n){
	n*=1000.0;
	n=(int)n;
	n/=1000.0;
	return n;
}

double volatilitate(node *n1, double med, int n){
	double vol=0;
	for(int i=0; i<n; i++){
		vol+=(n1->randament-med)*(n1->randament-med);
		n1=n1->next;
	}
	vol/=n-1;
	vol=sqrt(vol);
	return vol;
}

void calc_lista(node *n1, int n, FILE *f, FILE *fo){
	node *head=n1;
	node *n2;
	//i incepe de la 1 pt ca am citit headul
	double randament_mediu=0;
	for(int i=1; i<n; i++){
		printf("Introduce valoarea actiunii din ziua %d: ",i+1);
		n2=(node *)malloc(sizeof(node));
		n1->next=n2;
		fscanf(f,"%lf",&n2->valoare);
		n2->randament=(n2->valoare-n1->valoare)/n1->valoare;
		randament_mediu+=n2->randament;
		n2->next=NULL;
		n1=n2;
	}
	randament_mediu=randament_mediu/(n-1);
	//VOLATILITATEA
	double vol = volatilitate(head,randament_mediu, n); 
	//SHARPE RATIO
	double sharpe_ratio=randament_mediu/vol;
	//OUTPUT
	fprintf(fo,"%.3lf\n",trei_z(randament_mediu));
	fprintf(fo,"%.3lf\n",trei_z(vol));
	fprintf(fo,"%lf", sharpe_ratio);
}

void sharpe_ratio(FILE *fi, FILE *fo){
	//INITIALIZAM LISTA
	node *head=(node *)malloc(sizeof(node));
	head->randament=0;
	//INPUTUL DATELOR
	int n;
	printf("Introdu numarul total de observatii: ");
	fscanf(fi,"%d",&n);
	//VALOAREA CAPULUI LISTEI TREBUIE INTRODUSA SEPARAT
	printf("Introdu valoarea actiunii din ziua 1: ");
	fscanf(fi,"%lf",&head->valoare);
	//INTRODUCEM RESTUL LISTEI
	calc_lista(head, n, fi, fo);
}