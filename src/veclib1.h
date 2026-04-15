#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef struct node {
	float pret;
	struct node *prev;
} Node;

typedef struct p{
	int zi;
	float dif;
	char nume[10];
	struct p *next;	
}op;

void arbitraj(FILE *fi, FILE *fo);
void read(char nume[3][20],Node **head1, Node **head2, Node **head3, FILE *fi);
void add(Node **head, const char buff[20]);
void write(Node *head);
int oportunitati(op *coada, char nume[3][20],Node *head1,Node *head2, Node *head3,int zi, FILE *fo);
op *add_coada(op *coada,const char piata[20],float diferenta, int ziua, FILE *fo);
void elibereaza_coada(op *coada);
void elibereaza_stiva(Node *head);