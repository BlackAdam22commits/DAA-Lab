/* Q2: Multi-source BFS, radius-constrained Voronoi partition.
 * All hubs enter the queue at distance 0 in increasing index order. BFS queue
 * stays sorted by owner index within each distance level (induction), so the
 * first discoverer of a vertex has the smallest hub index among all hubs at
 * the minimum distance => tie-breaking is automatic.
 * Vertices with dist > R are never expanded (stop at depth R) -> owner=-1.
 * Input representation: CSR adjacency list (undirected => 2m entries).
 * Time O(V+E+K), Space O(V+E).
 * Usage: ./q2 < input   (n m K R; K hubs (1-indexed); m lines "u v")
 *        ./q2 test
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void solve(int n,int m,int K,int R,int*hub,int*eu,int*ev,int*own,int*dist){
    int *st=calloc(n+2,sizeof(int)),*to=malloc((2*m+1)*sizeof(int));
    for(int i=0;i<m;i++){st[eu[i]+1]++;st[ev[i]+1]++;}
    for(int i=1;i<=n+1;i++) st[i]+=st[i-1];
    int *pos=malloc((n+2)*sizeof(int)); memcpy(pos,st,(n+2)*sizeof(int));
    for(int i=0;i<m;i++){to[pos[eu[i]]++]=ev[i];to[pos[ev[i]]++]=eu[i];}
    int *q=malloc(n*sizeof(int)),h=0,t=0;
    for(int v=0;v<n;v++){own[v]=-1;dist[v]=-1;}
    for(int i=0;i<K;i++){
        int v=hub[i];
        if(own[v]==-1){ own[v]=i+1; dist[v]=0; q[t++]=v; } /* duplicate hub: smaller index wins */
    }
    while(h<t){
        int v=q[h++]; if(dist[v]>=R) continue;
        for(int k=st[v];k<st[v+1];k++){ int u=to[k];
            if(own[u]==-1){own[u]=own[v];dist[u]=dist[v]+1;q[t++]=u;} }
    }
    free(st);free(to);free(pos);free(q);
}
/* brute force: independent BFS from every hub, pick min dist then min index */
static void bfs1(int n,int m,int*eu,int*ev,int s,int*d){
    for(int i=0;i<n;i++)d[i]=-1; d[s]=0;
    for(int lv=0;lv<n;lv++){
        for(int i=0;i<m;i++){
            if(d[eu[i]]==lv&&d[ev[i]]<0) d[ev[i]]=lv+1;
            if(d[ev[i]]==lv&&d[eu[i]]<0) d[eu[i]]=lv+1;
        }
    }
}
int main(int argc,char**argv){
    if(argc>1&&!strcmp(argv[1],"test")){
        srand(11); int bad=0;
        for(int it=0;it<3000;it++){
            int n=1+rand()%10,m=rand()%15,K=1+rand()%4,R=rand()%5;
            int eu[16],ev[16],hub[4],own[16],dist[16],d[4][16];
            for(int i=0;i<m;i++){eu[i]=rand()%n;ev[i]=rand()%n;}
            for(int i=0;i<K;i++)hub[i]=rand()%n;
            solve(n,m,K,R,hub,eu,ev,own,dist);
            for(int i=0;i<K;i++)bfs1(n,m,eu,ev,hub[i],d[i]);
            for(int v=0;v<n;v++){
                int bd=-1,bo=-1;
                for(int i=0;i<K;i++) if(d[i][v]>=0&&(bd<0||d[i][v]<bd)){bd=d[i][v];bo=i+1;}
                if(bd>R){bd=-1;bo=-1;}
                if(bo!=own[v]||bd!=dist[v]){bad++;printf("MISMATCH it=%d v=%d\n",it,v);break;}
            }
        }
        printf(bad?"FAILED %d\n":"All 3000 random tests passed\n",bad); return bad!=0;
    }
    int n,m,K,R; if(scanf("%d %d %d %d",&n,&m,&K,&R)!=4) return 1;
    int *hub=malloc(K*sizeof(int)),*eu=malloc(m*sizeof(int)),*ev=malloc(m*sizeof(int));
    for(int i=0;i<K;i++){scanf("%d",&hub[i]);hub[i]--;}
    for(int i=0;i<m;i++){scanf("%d %d",&eu[i],&ev[i]);eu[i]--;ev[i]--;}
    int *own=malloc(n*sizeof(int)),*dist=malloc(n*sizeof(int));
    solve(n,m,K,R,hub,eu,ev,own,dist);
    printf("vertex owner dist\n");
    for(int v=0;v<n;v++) printf("%d %d %d\n",v+1,own[v],own[v]==-1?-1:dist[v]);
    return 0;
}
