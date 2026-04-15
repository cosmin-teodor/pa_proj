#include "veclib1.h"
#include<ctype.h>

void add(Node **head, const char buff[]){
	Node *n=malloc(sizeof(Node));
	n->prev=*head;
	n->pret=atof(buff);
	(*head)=n;
	// printf("%s ",buff);
}

void read(char nume[3][20],Node **head1, Node **head2, Node **head3, FILE *fi){
	char buff[20];
	int c;
	int i=-1, nr_piata=-1;
	do{
		c=fgetc(fi);
		//printf("%c",c);
		i++;
		if(c==EOF || c=='\n'){
			buff[i]='\0';
			if(isalpha(buff[0]))
				strcpy(nume[++nr_piata],buff);
			else{
				if(nr_piata==0)
					add(head1,buff);
				else if(nr_piata==1)
					add(head2,buff);
				else add(head3,buff);
			}
			i=-1;
		}
		else{
			buff[i]=c;
		}
	}while(c!=EOF);
}
/*
void write(Node *head){		//pt testul stackurilor
	printf("%d ",head->pret);
	if(head->prev->prev!=NULL)
		write(head->prev);
}
*/

op *add_coada(op *coada,const char piata[20],float diferenta, int ziua, FILE *fo){
	op *new=malloc(sizeof(op));
	if(diferenta<0)
		diferenta*=-1;
	new->dif=diferenta;
	//printf("%f",diferenta);
	strcpy(new->nume,piata);
	new->zi=ziua;
	coada->next=new;
	new->next=NULL;
	fprintf(fo,"ziua %d - %.2f - %s\n",new->zi,new->dif,new->nume);
	return new;
}

int oportunitati(op *coada, char nume[3][20], Node *head1,Node *head2, Node *head3, int zi, FILE *fo){
	if(head1->prev==NULL||head2->prev==NULL||head3->prev==NULL)
		return 0;
	int egal12=(head1->pret==head2->pret);
	int egal13=(head1->pret==head3->pret);
	int egal23=(head2->pret==head3->pret);
	if(egal12&&!egal13)
		coada=add_coada(coada,nume[2],head1->pret-head3->pret,zi,fo);
	if(egal13&&!egal12)
		coada=add_coada(coada,nume[1],head1->pret-head2->pret,zi,fo);
	if(egal23&&!egal13)
		coada=add_coada(coada,nume[0],head1->pret-head3->pret,zi,fo);
	oportunitati(coada,nume,head1->prev,head2->prev,head3->prev,zi+1,fo);
}

op *compare_oportunitati(op *coada, int dif_max, op *oportunitate_max){
	if(coada==NULL)
		return oportunitate_max;
	if(coada->dif>dif_max){
		//printf("%s in ziua %d e max\n",coada->nume,coada->zi);
		return compare_oportunitati(coada->next,coada->dif,coada);
	}
	else
		return compare_oportunitati(coada->next,dif_max,oportunitate_max);
}

void elibereaza_stiva(Node *head){
	Node *n=head->prev;
	free(head);
	if(n!=NULL){
		elibereaza_stiva(n);
	}
}

void elibereaza_coada(op *coada){
	op *n=coada->next;
	free(coada);
	if(n!=NULL)
		elibereaza_coada(n);
}

void arbitraj(FILE *fi, FILE *fo){
	Node *head1=malloc(sizeof(Node));
	Node *head2=malloc(sizeof(Node));
	Node *head3=malloc(sizeof(Node));
	head1->prev=NULL;
	head1->pret=-1;
	head2->prev=NULL;
	head2->pret=-1;
	head3->prev=NULL;
	head3->pret=-1;
	char nume[3][20];
	read(nume,&head1,&head2,&head3,fi);
	op *coada=calloc(1,sizeof(op));
	oportunitati(coada,nume,head1,head2,head3->prev,1,fo);
	op *oportunitate_max=malloc(sizeof(op));
	compare_oportunitati(coada,0,oportunitate_max);
	//printf("ziua %d - %d - %s",oportunitate_max->zi,oportunitate_max->dif,oportunitate_max->nume);
	free(oportunitate_max);
	elibereaza_stiva(head1);
	elibereaza_stiva(head2);
	elibereaza_stiva(head3); 
	elibereaza_coada(coada);
}