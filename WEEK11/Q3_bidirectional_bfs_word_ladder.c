/* Q3: Bidirectional BFS on a word graph (N<=1e5, L<=10, lowercase).
 * Input representation (avoids O(N^2 L) pairwise comparison):
 *   Wildcard patterns. For word w and position p, pattern key = base-27
 *   number of w with position p replaced by digit 0 ('*'), letters = 1..26.
 *   27^10 < 2^63 so the key is exact (no hash collisions). Two words are
 *   neighbours iff they share a key. Sort the (key,word) pairs (N*L of them),
 *   giving groups; neighbours of w = members of its L groups.
 * Bidirectional BFS: always expand the smaller frontier one full level; when a
 * node seen by the other side is hit, candidate = dF+1+dB; finish the level and
 * take the minimum.
 * Preprocess O(N L log(NL)), BFS O(N L) edges-touched (each group scanned
 * at most once per side), Space O(N L).
 * Input: N, N words, then A B.  Output: k or -1.
 * Usage: ./q3 < input ; ./q3 test
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned long long u64;
typedef struct{u64 key;int id;}E;
static int cmp(const void*a,const void*b){const E*x=a,*y=b; return x->key<y->key?-1:x->key>y->key?1:x->id-y->id;}

static int solve(int N,int L,char (*W)[12],const char*A,const char*B){
    int a=-1,b=-1;
    for(int i=0;i<N;i++){ if(!strcmp(W[i],A))a=i; if(!strcmp(W[i],B))b=i; }
    if(b<0) return -1;
    if(!strcmp(A,B)) return 0;
    int M=N; if(a<0){ strcpy(W[N],A); a=N; M=N+1; }
    E *e=malloc((size_t)M*L*sizeof(E)); int *grp=malloc((size_t)M*L*sizeof(int));
    for(int i=0;i<M;i++){
        u64 full=0,pw[10]; 
        for(int p=0;p<L;p++){ pw[p]=1; }
        u64 mul=1; for(int p=L-1;p>=0;p--){ pw[p]=mul; mul*=27; }
        for(int p=0;p<L;p++) full+=(u64)(W[i][p]-'a'+1)*pw[p];
        for(int p=0;p<L;p++) { e[i*L+p].key=full-(u64)(W[i][p]-'a'+1)*pw[p]; e[i*L+p].id=i; }
    }
    size_t T=(size_t)M*L; qsort(e,T,sizeof(E),cmp);
    int *gs=malloc((T+1)*sizeof(int)); int G=0;
    for(size_t i=0;i<T;i++){ if(i==0||e[i].key!=e[i-1].key) gs[G++]=(int)i; }
    gs[G]=(int)T;
    /* map each entry to its group; need per-(word,pos) group: recompute via position in sorted array */
    int g=-1; int *gof=malloc(T*sizeof(int));
    for(size_t i=0;i<T;i++){ if(i==0||e[i].key!=e[i-1].key) g++; gof[i]=g; }
    /* grp[word*L+p]: iterate sorted array, p unknown -> recover by key check: instead store index list */
    /* we store for each word its groups by scanning sorted entries and appending */
    int *cnt=calloc(M,sizeof(int));
    for(size_t i=0;i<T;i++){ int w=e[i].id; grp[(size_t)w*L+cnt[w]++]=gof[i]; }
    int *dF=malloc(M*sizeof(int)),*dB=malloc(M*sizeof(int));
    for(int i=0;i<M;i++){dF[i]=dB[i]=-1;}
    int *fa=malloc(M*sizeof(int)),*fb=malloc(M*sizeof(int)),*nx=malloc(M*sizeof(int));
    char *gdoneF=calloc(G,1),*gdoneB=calloc(G,1); /* group fully scanned by that side */
    int na=1,nb=1; fa[0]=a;fb[0]=b;dF[a]=0;dB[b]=0; int ans=-1;
    while(na>0&&nb>0){
        int fwd = na<=nb;
        int *fr=fwd?fa:fb,*d=fwd?dF:dB,*od=fwd?dB:dF; char*gd=fwd?gdoneF:gdoneB; int nf=fwd?na:nb,nn=0,best=-1;
        for(int i=0;i<nf;i++){ int v=fr[i];
            for(int p=0;p<L;p++){ int gg=grp[(size_t)v*L+p]; if(gd[gg]) continue; gd[gg]=1;
                for(int k=gs[gg];k<gs[gg+1];k++){ int u=e[k].id; if(u==v) continue;
                    if(od[u]>=0){ int c=d[v]+1+od[u]; if(best<0||c<best)best=c; }
                    if(d[u]<0){ d[u]=d[v]+1; nx[nn++]=u; } } } }
        if(best>=0){ans=best;break;}
        memcpy(fr,nx,nn*sizeof(int)); if(fwd)na=nn; else nb=nn;
    }
    free(e);free(grp);free(gs);free(gof);free(cnt);free(dF);free(dB);free(fa);free(fb);free(nx);free(gdoneF);free(gdoneB);
    return ans;
}
/* brute force: plain BFS with O(N^2 L) adjacency by Hamming distance */
static int ham1(const char*x,const char*y,int L){int c=0;for(int i=0;i<L;i++)c+=x[i]!=y[i];return c==1;}
static int brute(int N,int L,char (*W)[12],const char*A,const char*B){
    int b=-1; for(int i=0;i<N;i++) if(!strcmp(W[i],B)) b=i; if(b<0) return -1; if(!strcmp(A,B)) return 0;
    int M=N+1; strcpy(W[N],A); int d[64]; for(int i=0;i<M;i++)d[i]=-1;
    int q[64],h=0,t=0; /* A copy at index N; if A in D, the copy is just same word; BFS never steps 0-hamming */
    d[N]=0;q[t++]=N;
    while(h<t){int v=q[h++]; for(int u=0;u<N;u++) if(d[u]<0&&ham1(W[v],W[u],L)){d[u]=d[v]+1;q[t++]=u;}}
    return d[b];
}
int main(int argc,char**argv){
    if(argc>1&&!strcmp(argv[1],"test")){
        srand(5); int bad=0;
        for(int it=0;it<3000;it++){
            int L=1+rand()%3,N=1+rand()%12; static char W[100005][12]; char A[12],B[12];
            int al=2+rand()%2; int n=0;
            while(n<N){ char w[12]; for(int p=0;p<L;p++)w[p]='a'+rand()%al; w[L]=0; int dup=0; for(int i=0;i<n;i++) if(!strcmp(W[i],w))dup=1; if(!dup){strcpy(W[n++],w);} if(rand()%7==0)break; }
            N=n; for(int p=0;p<L;p++){A[p]='a'+rand()%al;B[p]='a'+rand()%al;} A[L]=B[L]=0;
            static char W2[100005][12]; memcpy(W2,W,sizeof(char[12])*(N+1));
            int r1=solve(N,L,W,A,B), r2=brute(N,L,W2,A,B);
            if(r1!=r2){bad++;printf("MISMATCH it=%d got %d exp %d\n",it,r1,r2);} }
        printf(bad?"FAILED %d\n":"All 3000 random tests passed\n",bad); return bad!=0;
    }
    int N; if(scanf("%d",&N)!=1) return 1;
    char (*W)[12]=malloc((size_t)(N+1)*12); char A[12],B[12];
    for(int i=0;i<N;i++) scanf("%11s",W[i]);
    scanf("%11s %11s",A,B); int L=strlen(A);
    printf("%d\n",solve(N,L,W,A,B)); return 0;
}
