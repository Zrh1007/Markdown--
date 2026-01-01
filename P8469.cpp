#include<bits/stdc++.h>
using namespace std;
int n,a[100010],minn=1e9;
const int mod=1e9+7;
int main(){
    ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
    cin>>n;
    for(int i=1;i<=n;i++){
        cin>>a[i];
        minn=min(minn,a[i]);
    }
    long long ans=1;
    for(int i=1;i<=n;i++){
        ans*=a[i]/minn;
        ans%=mod;
    }
    cout<<minn<<' '<<ans;
    return 0;
}