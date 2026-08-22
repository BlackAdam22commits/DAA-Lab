#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 6
typedef struct{int num;char col;} Item;

int sizes[N]={12,100,1000,10000,100000,1000000};
long long cOps,dOps,cnt[N],dnf[N];
int stable[N];
const char O[]="RBY";

int key(char c){return c=='R'?0:c=='B'?1:2;}
int rank(char c){return strchr(O,c)-O;}

void build(Item *a,int n){
    int v=0;
    for(int i=0;i<n;i++){
        v+=1+rand()%3;
        a[i].num=v;
        a[i].col=O[rand()%3];
    }
}

void countSort(Item *a,int n,Item *out){
    int c[3]={0},p[3];
    for(int i=0;i<n;i++){cOps++;c[key(a[i].col)]++;}
    p[0]=0;p[1]=c[0];p[2]=c[0]+c[1];
    for(int i=0;i<n;i++){
        int k=key(a[i].col);
        cOps+=2;
        out[p[k]++]=a[i];
    }
}

void dnf(Item *a,int n){
    int l=0,m=0,h=n-1;
    while(m<=h){
        int k=key(a[m]);
        Item t=a[m];
        dOps++;
        if(k==0){
            a[m++]=a[l];a[l++]=t;dOps+=2;
        }else if(k==1)m++;
        else{
            a[m]=a[h];a[h--]=t;dOps+=2;
        }
    }
}

int grouped(Item *a,int n){
    for(int i=1;i<n;i++)
        if(rank(a[i].col)<rank(a[i-1].col))return 0;
    return 1;
}

int ordered(Item *a,int n){
    for(int i=1;i<n;i++)
        if(a[i].col==a[i-1].col&&a[i].num<=a[i-1].num)return 0;
    return 1;
}

void fp(Item *a,int n,long long x[3][3]){
    memset(x,0,9*sizeof(long long));
    for(int i=0;i<n;i++){
        int k=rank(a[i].col);
        x[k][0]++;
        x[k][1]+=a[i].num;
        x[k][2]^=a[i].num;
    }
}

void show(char *s,Item *a,int n){
    printf("%-26s",s);
    for(int i=0;i<n;i++)printf(" %d%c",a[i].num,a[i].col);
    puts("");
}

void test(int z){
    int n=sizes[z];
    Item *a=malloc(n*sizeof(Item));
    Item *out=malloc(n*sizeof(Item));
    Item *b=malloc(n*sizeof(Item));
    long long x[3][3],y[3][3],before;

    build(a,n);
    memcpy(b,a,n*sizeof(Item));

    before=cOps;
    countSort(a,n,out);
    cnt[z]=cOps-before;

    before=dOps;
    dnf(b,n);
    dnf[z]=dOps-before;
    stable[z]=ordered(b,n);

    fp(a,n,x);fp(out,n,y);
    if(!grouped(out,n)||!ordered(out,n)||memcmp(x,y,sizeof x)){
        puts("Counting sort failed");exit(1);
    }

    fp(b,n,y);
    if(!grouped(b,n)||memcmp(x,y,sizeof x)||(n>=100&&stable[z])){
        puts("DNF check failed");exit(1);
    }

    if(z==0){
        printf("\nExample (n=%d):\n",n);
        show("Input:",a,n);
        show("Counting sort:",out,n);
        show("Dutch flag:",b,n);
    }

    free(a);free(out);free(b);
}

int main(){
    for(int i=0;i<N;i++)test(i);

    printf("\n=================================================\n");
    printf(" THREE-COLOUR SORT\n");
    printf("=================================================\n");
    printf("%8s %12s %10s %12s %8s %10s %10s\n",
           "n","count ops","3n","flag ops","ops/n","stable","DNF");
    printf("-------------------------------------------------\n");

    for(int i=0;i<N;i++)
        printf("%8d %12lld %10d %12lld %8.2f %10s %10s\n",
               sizes[i],cnt[i],3*sizes[i],dnf[i],
               (double)cnt[i]/sizes[i],"PASS",
               stable[i]?"PASS":"FAIL");

    printf("\nCounting sort: Theta(n), stable.\n");
    printf("Dutch National Flag: Theta(n), but unstable.\n");
    return 0;
}
