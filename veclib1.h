#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef struct prob{
	int numarator, numitor;
}prob;

typedef struct node{
	int index, day;
	prob *p;
	struct node *next;
}Node;

typedef struct queue{
	Node *head;
	Node *tail;
}Queue;

void Markov(FILE *fi, FILE *fo);
void print_mat(float T[30][30], float nr_intervale);
void read_obs(const float vals[30], float base, float dim_interv, float T[30][30], int nr_obs, int nr_intervale);