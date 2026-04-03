#include "veclib1.h"

int main(int argc,const char* argv[]){
	FILE *fi=fopen(argv[1],"r");
	FILE *fo=fopen(argv[2],"w");
	sharpe_ratio(fi,fo);
	fclose(fi);
	fclose(fo);
	return 0;
}