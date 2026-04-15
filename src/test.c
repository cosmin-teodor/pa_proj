#include "veclib1.h"

int main(int argc,const char* argv[]){
	if(argc<3){
		printf("Eroare: trebuie introduse ambele fisiere");
		return 1;
	}
	FILE *fi=fopen(argv[1],"r");
	FILE *fo=fopen(argv[2],"w");
	if(fi==NULL || fo==NULL){
		printf("Problema la fisiere");
		return 0;
	}
	arbitraj(fi,fo);
	fclose(fi);
	fclose(fo);
	return 0;
}