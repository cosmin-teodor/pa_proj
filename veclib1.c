#include "veclib1.h"
#include <stdlib.h>
#include <string.h>

void read_obs(const float vals[30], float base, float dim_interv, float T[30][30], int nr_obs, int nr_intervale){
    for(int i=0; i<nr_intervale; i++)
        for(int j=0; j<nr_intervale; j++)
            T[i][j]=0;
    float p_prev, p;
    for(int i=0; i<nr_obs-1; i++){
        p_prev = vals[i];
        p = vals[i+1];
        //printf("p:%f \n %d %d\n", p, (int)((p_prev-p_start)/dim_interv), (int)((p-p_start)/dim_interv));
        T[(int)((p_prev-base)/dim_interv)][(int)((p-base)/dim_interv)]++;
        p_prev = p;
    }
}

Node *enqueue(Node *node, int index, prob *p, int day){
    Node *tail = malloc(sizeof(Node));
    tail->day = day;
    tail->index = index;
    if(node)
        node->next = tail;
    tail->next = NULL;
    tail->p = p;
    return tail;
}

int red(int a, int b){
    if(!a)
        return b;
    if(!b)
        return a;
    while(b){
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

void simplify(prob *p){
    int div = red(p->numarator, p->numitor);
    if(!div) return;
    p->numarator /= div;
    p->numitor /= div;
}

void add_frac(prob *prob_target, prob *p){
    prob_target->numarator = prob_target->numarator * p->numitor + prob_target->numitor * p->numarator;
    prob_target->numitor *= p->numitor;
}

prob *multyp_frac(prob *p, int neigb, int total){
    prob *new = malloc(sizeof(prob));
    new->numarator = p->numarator * neigb;
    new->numitor = p->numitor * total;
    return new;
}

void daily_prob(FILE *f, float T[30][30], float dim_interv, float p_target, int nr_interv, float base, float p_start, int nr_zile){
    int day=1;
    int ind_start = (int)((p_start - base) / dim_interv);
    int ind_target = (int)((p_target - base) / dim_interv);    
    Queue *q = malloc(sizeof(Queue));
    prob *start_prob = malloc(sizeof(prob));
    start_prob->numarator = 1;
    start_prob->numitor = 1;
    prob *prob_target = malloc(sizeof(prob));
    prob_target->numarator = 0;
    prob_target->numitor = 1;
    q->tail = enqueue(NULL, ind_start, start_prob, day);
    q->head = q->tail;
    Node *aux = q->head;
    int last_day = day;
    while(q->head != NULL){
        int total = 0;
        Node *curr = q->head;
        if(curr->day != last_day){
            if(prob_target->numarator == 0)
                fprintf(f,"0\n");
            else if(prob_target->numitor == 1)
                fprintf(f,"%d\n",prob_target->numarator);
            else{
                //simplify(prob_target);
                fprintf(f, "%d/%d",prob_target->numarator, prob_target->numitor);
                if(curr->day<=nr_zile)
                    fprintf(f,"\n");
            }
            last_day++;
            if(last_day > nr_zile) 
                break;
            prob_target->numarator = 0;
            prob_target->numitor   = 1;
        }
        if(curr->index == ind_target){
            if(prob_target->numarator == 0){
                prob_target->numarator = curr->p->numarator;
                prob_target->numitor = curr->p->numitor;
            } 
            else {
                add_frac(prob_target, curr->p);
                simplify(prob_target);
            }
        }
        for(int j = 0; j<nr_interv; j++)
            total += T[curr->index][j];
        for(int j = 0; j<nr_interv; j++){
            if(T[curr->index][j]){
                prob *p_nou = multyp_frac(curr->p, T[curr->index][j], total);
                simplify(p_nou);
                q->tail = enqueue(q->tail, j, p_nou, curr->day + 1);        
            }
        }
        q->head = curr->next; 
        free(curr->p);
        free(curr);
    }
    while(q->head != NULL) { 
        Node *temp = q->head;
        q->head = q->head->next;
        free(temp->p);
        free(temp);
    }
    free(prob_target);
    free(q);
}

void Markov(FILE *fi, FILE *fo){
    int nr_obs, nr_zile;
    float dim_interv, p_start, p_target;
    fscanf(fi, "%d", &nr_obs);
    fscanf(fi, "%f",&dim_interv);
    fscanf(fi, "%d",&nr_zile);
    fscanf(fi, "%f", &p_start);
    fscanf(fi, "%f", &p_target);
    float vals[30], v_max=0, v_min = 9999;
    for(int i=0; i<nr_obs; i++){
        fscanf(fi, "%f", &vals[i]);
        //printf("%f ", vals[i]);
        if(vals[i]>v_max)
            v_max = vals[i];
        if(vals[i]<v_min)
            v_min = vals[i];
    }
    float base = (float)((int)(v_min / dim_interv) * dim_interv);    
    int nr_interv = (int)((v_max - base) / dim_interv) + 1;
    float T[30][30];
    read_obs(vals, base, dim_interv, T, nr_obs, nr_interv);
    //print_mat(T, nr_intervale);
    daily_prob(fo, T,dim_interv, p_target, nr_interv, base, p_start, nr_zile);
}