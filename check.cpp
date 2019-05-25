#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int MAXN=2e8+100;
int d[MAXN];
bool pos[MAXN*2],neg[MAXN*2];
typedef unsigned char uint8;      /* Unsigned 8 bit quantity  */
typedef signed char int8;         /* Signed 8 bit quantity    */
typedef unsigned short uint16;    /* Unsigned 16 bit quantity */
typedef signed short int16;       /* Signed 16 bit quantity   */
typedef unsigned int uint32;      /* Unsigned 32 bit quantity */
typedef signed int int32;         /* Signed 32 bit quantity   */
typedef float fp32;               /* Single precision         */
                                  /* floating point           */
typedef double fp64;              /* Double precision         */
#define BigtoLittle32(A)   ((( (uint32)(A) & 0xff000000 ) >> 24) | \
                           (( (uint32)(A) & 0x00ff0000 ) >> 8)   | \
                           (( (uint32)(A) & 0x0000ff00 ) << 8)   | \
                           (( (uint32)(A) & 0x000000ff ) << 24))

int main(){
    int N;
    FILE *fp;
    fp = fopen("out.dat","rb");
    while(~fread(&N,sizeof(int),1,fp)){
        N=BigtoLittle32(N);
        memset(pos,0,sizeof pos);
        memset(neg,0,sizeof neg);
        memset(d,0,sizeof d);
        for(int i=1;i<=N;i++){
            fread(&d[i],sizeof(int),1,fp);
            d[i]=BigtoLittle32(d[i]);
        }
        bool flag=0;
        for(int i=1;i<=N;i++){
            if(pos[d[i]-i+N]==1||neg[d[i]+i]==1){
                flag=1;
                break;
            }
            pos[d[i]-i+N]=1;
            neg[d[i]+i]=1;
        }
        if(!flag)
            cout<<"Yes"<<endl;
        else
            cout<<"NO"<<endl;
    }
    fclose(fp);
    return 0;
}
