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

void down(float **m,int stocks_num,int values_num,TreeNode *tree,int depth){
    if(!tree)return;
    if(depth>=values_num-1)return;
    if(!tree->stocks||!tree->stocks->next)return;
    TreeNode *L=calloc(1,sizeof(TreeNode));
    TreeNode *R=calloc(1,sizeof(TreeNode));
    L->stocks=calloc(1,sizeof(StockList));
    R->stocks=calloc(1,sizeof(StockList));
    StockList *l=L->stocks, *r=R->stocks, *n=tree->stocks->next;
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
    down(m,stocks_num,values_num,L,depth+1);
    down(m,stocks_num,values_num,R,depth+1);
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

/* --- ADAUGAT: functii noi necesare pentru output --- */

/* Colecteaza path-ul si simbolul fiecarei actiuni din frunze */
void collect_leaves(TreeNode *node,char *path,int depth,
                    char leaf_paths[][20],char leaf_symbols[][7],int *count){
    if(!node||!node->stocks)return;
    if(!node->left&&!node->right){
        StockList *s=node->stocks->next;
        while(s){
            path[depth]='\0';
            strcpy(leaf_paths[*count],path);
            strcpy(leaf_symbols[*count],s->simbol);
            (*count)++;
            s=s->next;
        }
        return;
    }
    if(node->left){ path[depth]='L'; collect_leaves(node->left,path,depth+1,leaf_paths,leaf_symbols,count); }
    if(node->right){ path[depth]='R'; collect_leaves(node->right,path,depth+1,leaf_paths,leaf_symbols,count); }
}

/* Gaseste path-ul unei actiuni in arbore */
int find_stock_path(TreeNode *node,const char *simbol,char *path,int depth){
    if(!node||!node->stocks)return 0;
    StockList *s=node->stocks->next;
    while(s){
        if(strcmp(s->simbol,simbol)==0){ path[depth]='\0'; return 1; }
        s=s->next;
    }
    if(node->left){ path[depth]='L'; if(find_stock_path(node->left,simbol,path,depth+1))return 1; }
    if(node->right){ path[depth]='R'; if(find_stock_path(node->right,simbol,path,depth+1))return 1; }
    return 0;
}

/* Numarul de pozitii diferite intre doua path-uri */
int path_distance(const char *a,const char *b){
    int diff=0,i=0;
    while(a[i]&&b[i]){ if(a[i]!=b[i])diff++; i++; }
    return diff;
}

/* Gaseste opusul: actiunea cu path-ul cel mai diferit */
void find_opposite(TreeNode *tree,const char *simbol,char result[7],
                   char leaf_paths[][20],char leaf_symbols[][7],int leaf_count){
    char my_path[20];
    find_stock_path(tree,simbol,my_path,0);
    int best_dist=-1;
    result[0]='\0';
    for(int i=0;i<leaf_count;i++){
        if(strcmp(leaf_symbols[i],simbol)==0)continue;
        int d=path_distance(my_path,leaf_paths[i]);
        if(d>best_dist){ best_dist=d; strcpy(result,leaf_symbols[i]); }
    }
}

/* --- MODIFICAT: diversification --- codul tau pastrat, doar outputul adaugat la final --- */

void diversification(FILE *fi,FILE *fo){
    TreeNode *tree=calloc(1,sizeof(TreeNode));
    tree->stocks=calloc(1,sizeof(StockList));
    int stocks_num=get_symbol(fi,tree);
    float **m=malloc(sizeof(float*)*100);
    for(int i=0;i<100;i++)
        m[i]=malloc(sizeof(float)*stocks_num);
    int values_num=0;
    get_val(fi,m,stocks_num,&values_num);
    values_num++;
    down(m,stocks_num,values_num,tree,1);

    /* ADAUGAT: colectam frunzele cu path-urile lor */
    char leaf_paths[100][20];
    char leaf_symbols[100][7];
    int leaf_count=0;
    char tmp_path[20];
    collect_leaves(tree,tmp_path,0,leaf_paths,leaf_symbols,&leaf_count);

    /* ADAUGAT: reconstruim ordinea originala dupa index */
    char ordered[100][7];
    StockList *s=tree->stocks->next;
    while(s){ strcpy(ordered[s->index],s->simbol); s=s->next; }

    /* ADAUGAT: afisam fiecare pereche in ordinea indexului original */
    for(int i=0;i<stocks_num;i++){
        char opposite[7];
        find_opposite(tree,ordered[i],opposite,leaf_paths,leaf_symbols,leaf_count);
        if(opposite[0]!='\0')
            //printf(fo,"%s-%s\n",ordered[i],opposite);
			printf("%s-%s\n",ordered[i],opposite);
    }
}