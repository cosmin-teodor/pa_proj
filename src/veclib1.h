#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct Node {
	double valoare;
	double randament;
	struct Node *next;
} node;

double volatilitate(node *n1, double med, int n);
void test_lista(node *n);
void calc_lista(node *n1, int n, FILE *f, FILE *fo);
void sharpe_ratio(FILE *fi, FILE *fo);