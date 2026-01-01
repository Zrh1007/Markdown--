#include<bits/stdc++.h>
using namespace std;
string s;//__gcd
struct node{
    long long x,y;//x:分子 y：分母
}a[15];
int main(){
    ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
    cin>>s;
    long long n=s.size(),ans=0,cnt=1;
    for(long long i=0;i<n;i++){
        if(s[i]>='0'&&s[i]<='9'){
            ans*=10;
            ans+=s[i]-'0';
        }
        else{
            if(s[i]=='/'){
                a[cnt].x=ans;
                ans=0;
            }
            else if(s[i]=='+'){
                a[cnt].y=ans;
                cnt++;
                ans=0;
            }
        }
    }
    if(ans!=0) a[cnt].y=ans;
    long long p=a[1].x,q=a[1].y;
    for(long long i=2;i<=cnt;i++){
        p=p*a[i].y+a[i].x*q;
        q=q*a[i].y;
    }
    long long gcd=__gcd(p,q);
    if(p/gcd==q/gcd) cout<<1;
    else cout<<p/gcd<<'/'<<q/gcd;
    return 0;
}