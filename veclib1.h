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
void daily_prob(FILE *f, float T[30][30], float dim_interv, float p_target, int nr_interv, float base, float p_start, int nr_zile);
prob *multyp_frac(prob *p, int neigb, int total);
void add_frac(prob *prob_target, prob *p);
void simplify(prob *p);
int red(int a, int b);
Node *enqueue(Node *node, int index, prob *p, int day);