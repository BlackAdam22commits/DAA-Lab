/*
DAA Lab-02, Q2: Merge Sort (2-way) vs Modified Merge Sort (3-way)
Both are Theta(n log n).
This program times both algorithms over growing values of n
and writes the results to q2_timings.csv.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------- Classic 2-way Merge Sort ---------- */

void merge2(int *a,int l,int m,int r,int *tmp)
{
    int i=l,j=m+1,k=l;

    while(i<=m && j<=r)
        tmp[k++]=(a[i]<=a[j])?a[i++]:a[j++];

    while(i<=m)
        tmp[k++]=a[i++];

    while(j<=r)
        tmp[k++]=a[j++];

    for(int x=l;x<=r;x++)
        a[x]=tmp[x];
}

void mergesort2(int *a,int l,int r,int *tmp)
{
    if(l>=r)
        return;

    int m=(l+r)/2;

    mergesort2(a,l,m,tmp);
    mergesort2(a,m+1,r,tmp);
    merge2(a,l,m,r,tmp);
}

/* ---------- Modified 3-way Merge Sort ---------- */

void merge3(int *a,int l,int m1,int m2,int r,int *tmp)
{
    int i=l,j=m1+1,k=m2+1,t=l;

    while(i<=m1 && j<=m2 && k<=r)
    {
        if(a[i]<=a[j] && a[i]<=a[k])
            tmp[t++]=a[i++];
        else if(a[j]<=a[i] && a[j]<=a[k])
            tmp[t++]=a[j++];
        else
            tmp[t++]=a[k++];
    }

    while(i<=m1 && j<=m2)
        tmp[t++]=(a[i]<=a[j])?a[i++]:a[j++];

    while(j<=m2 && k<=r)
        tmp[t++]=(a[j]<=a[k])?a[j++]:a[k++];

    while(i<=m1 && k<=r)
        tmp[t++]=(a[i]<=a[k])?a[i++]:a[k++];

    while(i<=m1)
        tmp[t++]=a[i++];

    while(j<=m2)
        tmp[t++]=a[j++];

    while(k<=r)
        tmp[t++]=a[k++];

    for(int x=l;x<=r;x++)
        a[x]=tmp[x];
}

void mergesort3(int *a,int l,int r,int *tmp)
{
    if(l>=r)
        return;

    int len=r-l+1;
    int m1=l+len/3-1;
    int m2=l+(2*len)/3-1;

    if(m1<l)
        m1=l;

    if(m2<=m1)
        m2=m1+1;

    if(m2>=r)
        m2=r-1;

    mergesort3(a,l,m1,tmp);
    mergesort3(a,m1+1,m2,tmp);
    mergesort3(a,m2+1,r,tmp);
    merge3(a,l,m1,m2,r,tmp);
}

/* ---------- Timing Function ---------- */

static double now_sec(void)
{
    return (double)clock()/CLOCKS_PER_SEC;
}

/* ---------- Check if Array is Sorted ---------- */

int is_sorted(int *a,int n)
{
    for(int i=1;i<n;i++)
        if(a[i-1]>a[i])
            return 0;

    return 1;
}

/* ---------- Main Function ---------- */

int main(void)
{
    srand(7);

    int sizes[]={1000,5000,10000,50000,100000,300000,500000,1000000,2000000};
    int ns=sizeof(sizes)/sizeof(sizes[0]);

    FILE *fp=fopen("q2_timings.csv","w");

    if(fp==NULL)
    {
        printf("Error: Could not create q2_timings.csv\n");
        return 1;
    }

    fprintf(fp,"n,mergesort2_time,mergesort3_time\n");

    for(int s=0;s<ns;s++)
    {
        int n=sizes[s];

        int *a1=malloc(n*sizeof(int));
        int *a2=malloc(n*sizeof(int));
        int *tmp=malloc(n*sizeof(int));

        if(a1==NULL || a2==NULL || tmp==NULL)
        {
            printf("Memory allocation failed for n=%d\n",n);

            free(a1);
            free(a2);
            free(tmp);
            fclose(fp);

            return 1;
        }

        for(int i=0;i<n;i++)
        {
            int v=rand();
            a1[i]=v;
            a2[i]=v;
        }

        double t0=now_sec();
        mergesort2(a1,0,n-1,tmp);
        double t1=now_sec();
        double time2=t1-t0;

        double t2=now_sec();
        mergesort3(a2,0,n-1,tmp);
        double t3=now_sec();
        double time3=t3-t2;

        if(!is_sorted(a1,n) || !is_sorted(a2,n))
            fprintf(stderr,"SORT ERROR at n=%d\n",n);

        fprintf(fp,"%d,%.6f,%.6f\n",n,time2,time3);

        printf("n=%d  mergesort2=%.6fs  mergesort3=%.6fs\n",
               n,time2,time3);

        free(a1);
        free(a2);
        free(tmp);
    }

    fclose(fp);

    printf("Wrote q2_timings.csv\n");

    return 0;
}
