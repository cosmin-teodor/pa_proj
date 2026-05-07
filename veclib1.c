#include "veclib1.h"
#include <stdlib.h>
#include <string.h>

void add_symbol(char *buff,StockList *stock,int i){
	StockList *new=malloc(sizeof(StockList));
	strncpy(new->simbol,buff,6);
	new->simbol[6]='\0';
	new->index=i;
	new->next=stock->next;
	stock->next=new;
}

int get_symbol(FILE *fi,TreeNode *tree){
	char c,buff[6];
	int i=0,stocks_num=0;
	StockList *stock=tree->stocks;
	while((c=fgetc(fi))!='\n'){
		if(c==','){
			buff[i]='\0';
			add_symbol(buff,stock,stocks_num);
			stock=stock->next;
			stocks_num++;
			i=0;
		}else{
			buff[i++]=c;
		}
	}

	buff[i]='\0';
	add_symbol(buff,stock,stocks_num);
	return stocks_num+1;
}

void get_val(FILE *fi,float **m,int stocks_num,int *values_num){
	char sep;
	for(int i=0;i<stocks_num;i++){
		fscanf(fi,"%f",&m[*values_num][i]);
		sep=fgetc(fi);
	}
	if(sep=='\n'){
		(*values_num)++;
		get_val(fi,m,stocks_num,values_num);
	}
}

void print_stocks(StockList *stock){
	if(stock){
		printf("%s ",stock->simbol);
		print_stocks(stock->next);
	}
}

void down(float **m,int strocks_num,int values_num,TreeNode *tree,int depth){
	if(!tree)return;
	if(depth>=values_num-1)return;
	if(!tree->stocks||!tree->stocks->next)return;
	TreeNode *L=calloc(1,sizeof(TreeNode));
	TreeNode *R=calloc(1,sizeof(TreeNode));
	L->stocks=calloc(1,sizeof(StockList));
	R->stocks=calloc(1,sizeof(StockList));
	StockList *l=L->stocks, *r=R->stocks, *n=tree->stocks->next; //left right next
	while(n){
		if(m[depth][n->index]>m[depth-1][n->index]){
			add_symbol(n->simbol,r,n->index);
			r=r->next;
		}else{
			add_symbol(n->simbol,l,n->index);
			l=l->next;
		}
		n=n->next;
	}
	tree->left=L;
	tree->right=R;
	down(m,strocks_num,values_num,L,depth+1);
	down(m,strocks_num,values_num,R,depth+1);
}

void print_tree(TreeNode *tree,int level){
	if(!tree)return;
	for(int i=0;i<level;i++)printf("   ");
	if(!level)printf("ROOT: ");
	else printf("LEVEL %d: ",level);
	print_stocks(tree->stocks->next);
	printf("\n");
	print_tree(tree->left,level+1);
	print_tree(tree->right,level+1);
}

void diversification(FILE *fi,FILE *fo){
	TreeNode *tree=calloc(1,sizeof(TreeNode));
	tree->stocks=calloc(1,sizeof(StockList));
	int strocks_num=get_symbol(fi,tree);
	float **m=malloc(sizeof(float*)*100);
	for(int i=0;i<100;i++)
		m[i]=malloc(sizeof(float)*strocks_num);
	int values_num=0;
	get_val(fi,m,strocks_num,&values_num);
	values_num++;
	down(m,strocks_num,values_num,tree,1);
	print_tree(tree,0);
}