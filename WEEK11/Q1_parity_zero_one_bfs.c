/* Q1: Shortest path on parity-constrained graph via 0-1 BFS.
 * Idea: state (v,p), p = parity of number of weight-1 edges used so far.
 *   w=0 edge: (v,p)->(u,p)   cost 0  (push front)
 *   w=1 edge: (v,p)->(u,1-p) cost 1  (push back)
 * Since weights are 0/1, W(P)=k1(P). Answer(v)=min(d[v][0], d[v][1]+C).
 * Input representation: adjacency list in CSR form (arrays), O(V+E) space.
 * Time O(V+E), Space O(V+E).
 * Usage: ./q1 < input      (n m S C, then m lines "u v w", 1-indexed)
 *        ./q1 test         (random tests against brute-force walk enumeration)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF (1LL<<60)
typedef long long ll;

static void solve(int n,int m,int S,ll C,int*eu,int*ev,int*ew,ll*ans){
    int *st=calloc(n+2,sizeof(int)),*to=malloc((m+1)*sizeof(int)),*wt=malloc((m+1)*sizeof(int));
    for(int i=0;i<m;i++) st[eu[i]+1]++;
    for(int i=1;i<=n+1;i++) st[i]+=st[i-1];
    int *pos=malloc((n+2)*sizeof(int)); memcpy(pos,st,(n+2)*sizeof(int));
    for(int i=0;i<m;i++){int k=pos[eu[i]]++;to[k]=ev[i];wt[k]=ew[i];}
    int S2=2*n; /* states id = v*2+p, v in [0,n) */
    ll *d=malloc(S2*sizeof(ll)); for(int i=0;i<S2;i++) d[i]=INF;
    int cap=2*(2*m+2)+4, *dq=malloc(cap*sizeof(int)); int h=cap/2,t=cap/2; /* [h,t) */
    d[(S)*2]=0; dq[t++]=S*2;
    ll *dd=d; (void)dd;
    while(h<t){
        int s=dq[h++]; int v=s>>1,p=s&1; ll dv=d[s];
        for(int k=st[v];k<st[v+1];k++){
            int u=to[k],w=wt[k]; int ns=u*2+(p^w); ll nd=dv+w;
            if(nd<d[ns]){ d[ns]=nd; if(w==0) dq[--h]=ns; else dq[t++]=ns; }
        }
    }
    for(int v=0;v<n;v++){
        ll a=d[v*2], b=d[v*2+1]; if(b<INF) b+=C;
        ans[v]=a<b?a:b; if(ans[v]>=INF) ans[v]=-1;
    }
    free(st);free(to);free(wt);free(pos);free(d);free(dq);
}
/* brute force: DFS over all walks of length <= 2n, cost by definition */
static ll best[64];
static int bn,bm,*bu,*bv,*bw; static ll bC;
static void dfs(int v,int len,int k1,int W){
    ll c=W+((k1&1)?bC:0); if(best[v]<0||c<best[v]) best[v]=c;
    if(len>=2*bn) return;
    for(int i=0;i<bm;i++) if(bu[i]==v) dfs(bv[i],len+1,k1+bw[i],W+bw[i]);
}
int main(int argc,char**argv){
    if(argc>1&&!strcmp(argv[1],"test")){
        srand(7); int bad=0;
        for(int it=0;it<3000;it++){
            int n=1+rand()%5,m=rand()%9; int eu[16],ev[16],ew[16];
            for(int i=0;i<m;i++){eu[i]=rand()%n;ev[i]=rand()%n;ew[i]=rand()%2;}
            ll C=rand()%6; int S=rand()%n; ll a[8];
            solve(n,m,S,C,eu,ev,ew,a);
            for(int i=0;i<n;i++) best[i]=-1;
            bn=n;bm=m;bu=eu;bv=ev;bw=ew;bC=C; dfs(S,0,0,0);
            for(int i=0;i<n;i++) if(a[i]!=best[i]){bad++;printf("MISMATCH it=%d v=%d got %lld exp %lld\n",it,i,a[i],best[i]);break;}
        }
        printf(bad?"FAILED %d\n":"All 3000 random tests passed\n",bad); return bad!=0;
    }
    int n,m,S; ll C;
    if(scanf("%d %d %d %lld",&n,&m,&S,&C)!=4) return 1;
    int *eu=malloc(m*sizeof(int)),*ev=malloc(m*sizeof(int)),*ew=malloc(m*sizeof(int));
    for(int i=0;i<m;i++){scanf("%d %d %d",&eu[i],&ev[i],&ew[i]);eu[i]--;ev[i]--;}
    ll *ans=malloc(n*sizeof(ll)); solve(n,m,S-1,C,eu,ev,ew,ans);
    for(int v=0;v<n;v++){ if(ans[v]<0) printf("%d: unreachable\n",v+1); else printf("%d: %lld\n",v+1,ans[v]); }
    return 0;
}
