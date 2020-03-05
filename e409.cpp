#include<bits/stdc++.h>
using namespace std;
const int MaxN = 1000000+5;
const int MaxQ = 100000+5;
int A[MaxN];
bool C[MaxQ];
int X[MaxQ] , Y[MaxQ];
int k,m,N,Q;
void gen_dat()
{
    int i,j;
    for( i=k+1; i<=max(Q,N); ++i )
         A[i] = ( A[i-2]*97 + A[i-1]*3 )%m+1;
    for(i=1,j=max(Q,N); i<=Q; ++i,--j)
    {
        C[i] = A[i]&1;
        X[i] = (A[i]+A[j])%N+1;
        if(C[i]) Y[i] = X[i]+ ( (A[i]<<3)+(A[j]<<5)+m )%( N-X[i]+1 );
        else Y[i] = ((A[i]<<3)+(A[j]<<5))%m+1;
    }
}

class maxmin{
public:
    int M,m;
    maxmin(): M(INT_MIN),m(INT_MAX){};
    maxmin(int M,int m): M(M),m(m){};
}zkw_segment_tree[3*MaxN];
// init
int build(int n){
    int depth=floor(log2(n-1))+1;
    for(int i=0;i<n;++i){
        zkw_segment_tree[(1<<depth)|i]=maxmin(A[i+1],A[i+1]);
        for(int j=(1<<depth)|i;j>1;j>>=1){
            zkw_segment_tree[j>>1]=maxmin(max(zkw_segment_tree[j^1].M,zkw_segment_tree[j].M),min(zkw_segment_tree[j^1].m,zkw_segment_tree[j].m));
        }
    }
    return depth;
}
// query
maxmin query(int L,int R){
    maxmin ans(INT_MIN,INT_MAX);
    if(L>R)return ans;
    else if(L==R)return zkw_segment_tree[L];
    if(L&1){ans.M=max(ans.M,zkw_segment_tree[L].M); ans.m=min(ans.m,zkw_segment_tree[L].m); ++L;}
    if(!(R&1)){ans.M=max(ans.M,zkw_segment_tree[R].M); ans.m=min(ans.m,zkw_segment_tree[R].m); --R;}
    maxmin q=query(L>>1,R>>1);
    return maxmin(max(ans.M,q.M),min(ans.m,q.m));
}
// update
void update(int rt,int data){
    zkw_segment_tree[rt]=maxmin(data,data);
    for(int j=rt;j>1;j>>=1){
        zkw_segment_tree[j>>1]=maxmin(max(zkw_segment_tree[j^1].M,zkw_segment_tree[j].M),min(zkw_segment_tree[j^1].m,zkw_segment_tree[j].m));
    }
}

int main(){
    // input
    cin>>k>>m;
    for(int i=1;i<=k;++i)cin>>A[i];
    cin>>N>>Q;
    gen_dat();

    const int depth=build(N);
    for(int i=1;i<=Q;++i){
        if(!C[i]){
            update((1<<depth)|X[i]-1,Y[i]);
        }else{
            maxmin q=query((1<<depth)|X[i]-1,(1<<depth)|Y[i]-1);
            cout<<q.M-q.m<<endl;
        }
        /* test
        for(int i=1;i<=N;++i){
            cout<<zkw_segment_tree[(1<<depth)|i-1].M<<" ";
        }cout<<endl;
        */
    }
}
