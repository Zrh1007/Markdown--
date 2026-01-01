#include<bits/stdc++.h>
using namespace std;
bool vis[1000010];
int t,pri[1000010],cnt=0;
bool check(int x){
    if(x<2) return 0;
    for(int i=2;i*i<=x;i++){
        if(x%i==0) return 0;
    }
    return 1;
}
int main(){
    ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
    vis[0]=vis[1]=1;
    for(int i=2;i<=1e5+7;i++){
        for(int j=i+i;j<=1e5+7;j+=i) vis[j]=1;
    }
    for(int i=1;i<=100000;i++){
        if(vis[i]==0) pri[++cnt]=i;
    }
    cin>>t;
    int pos=0;
    while(t--){
        int n,val=0;
        cin>>n;
        while(pos+1<=cnt&&pri[pos+1]<=n) pos++;
        long long sum=0;
        for(int j=1;j<=pos;j++){
            long long xx=n/pri[j];
            sum+=xx*(j-1);
        }
        sum*=2;
        cout<<sum<<'\n';
    }
    return 0;
}