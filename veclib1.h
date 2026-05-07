#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef struct stockList {
	int index;
	char simbol[6];
	float vals[10];
	struct stockList *next;
}StockList;

typedef struct TreeNode {
	StockList *stocks;
	// toate actiunile care trec prin acest nod
	struct TreeNode *left; // aici se duc actiunile care scad
	struct TreeNode *right; // aici se duc actiunile care cresc
	int depth;
	// poate simplifica niste calcule
} TreeNode;

void diversification(FILE *fi, FILE *fo);
void get_val(FILE *fi, float **m, int stocks_num, int *values_num);
int get_symbol(FILE *fi, TreeNode *tree);
void add_symbol(char buff[6], StockList *stock, int i);
void down(float **m, int strocks_num, int values_num, TreeNode *tree, int depth);
void print_tree(TreeNode *tree, int level);