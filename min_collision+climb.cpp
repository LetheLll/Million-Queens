#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define mem(a,b) memset(a,b,sizeof a)

const int MAXN=5e7+100;

int collision_pos[2*MAXN+1],collision_neg[2*MAXN+1],d[MAXN],collision,N;
bool pos[2*MAXN+1],neg[2*MAXN+1];
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

bool is_bigendian()
{
    int a = 0x1234;
    char b =  *(char *)&a;  //b == the Low address part of a
    //printf("%c\n", b);
    if (b == 0x34)
    {
        return false;
    }
    return true;
}


default_random_engine r(time(NULL));


int random(int x,int y)
{
    uniform_int_distribution <int> u(x,y);
    return u(r);
}


//初始化棋盘
int init(int N)
{
    collision = 0;
    for(int i = 0; i <= N; i++)
    {
        d[i] = i;
        neg[i] = false;
        pos[i] = false;
        collision_neg[i] = 0;
        collision_pos[i] = 0;
    }
    for(int i = N + 1; i <= 2*N; i++)
    {
        neg[i] = false;
        pos[i] = false;
        collision_neg[i] = 0;
        collision_pos[i] = 0;
    }
    //生成无冲突的部分
    int idx = 1,collision1 = 0;
    int up = N*3.08,cnt = 0;
    for(int i = 1; i <= up; i++)
    {
        if(idx == N)
            break;
        int m = random(idx,N);
        if(pos[d[m] - idx + N] == 0 && neg[d[m] + idx] == 0)
        {
            swap(d[idx],d[m]);
            collision_pos[d[idx] - idx + N]++;
            collision_neg[d[idx] + idx]++;
            pos[d[idx] - idx + N] = 1;
            neg[d[idx] + idx] = 1;
            idx++;
        }
    }
    //剩余部分随机填充
    for(int i = idx; i <= N; i++)
    {
        int m = random(i,N);
        swap(d[i],d[m]);
        collision_pos[d[i] - i + N]++;
        collision_neg[d[i] + i]++;
    }
    for(int i = 1; i <= 2*N-1; i++)
        collision+=collision_pos[i] > 1 ? (collision_pos[i] - 1) : 0;
    for(int i = 2; i <= 2*N; i++)
        collision+=collision_neg[i] > 1 ? (collision_neg[i] - 1) : 0;
    return N - idx + 1;

}


void do_swap1(int i,int j,int gain)
{
    collision_pos[d[i] - i + N]--;
    collision_pos[d[j] - j + N]--;
    collision_neg[d[i] + i]--;
    collision_neg[d[j] + j]--;

    swap(d[i],d[j]);

    collision_pos[d[i] - i + N]++;
    collision_pos[d[j] - j + N]++;
    collision_neg[d[i] + i]++;
    collision_neg[d[j] + j]++;

    collision -= gain;
}


void do_swap2(int i,int j)
{
    collision_pos[d[i] - i + N]--;
    collision_pos[d[j] - j + N]--;
    collision_neg[d[i] + i]--;
    collision_neg[d[j] + j]--;

    swap(d[i],d[j]);

    collision_pos[d[i] - i + N]++;
    collision_pos[d[j] - j + N]++;
    collision_neg[d[i] + i]++;
    collision_neg[d[j] + j]++;

}


int reduce(int i,int j)
{
    int cnt = 0;
    //在同一条负对角线上
    if (d[i] - i == d[j] - j)
    {
        cnt = (collision_pos[d[i] - i + N] > 2 ? 2: 1) + (collision_neg[i + d[i]] > 1 ? 1 : 0) +
              (collision_neg[j + d[j]] > 1 ? 1 : 0) -
              (collision_neg[i + d[j]] > 0 ? 2 : 1) -
              (collision_pos[d[j] - i + N] > 0 ? 1: 0) - (collision_pos[d[i] - j + N] > 0 ? 1: 0);

        return cnt;
    }
    //在同一条主对角线上
    if (i + d[i] == j + d[j])
    {
        cnt = (collision_neg[i + d[i]] > 2 ? 2: 1) + (collision_pos[d[i] - i + N] > 1 ? 1 : 0) +
              (collision_pos[d[j] - j + N] > 1 ? 1 : 0) -
              (collision_pos[d[j] - i + N] > 0 ? 2 : 1) -
              (collision_neg[i + d[j]] > 0 ? 1: 0) - (collision_neg[j + d[i]] > 0 ? 1: 0);

        return cnt;
    }
    //普通情况
    cnt = (collision_pos[d[i] - i + N] > 1 ? 1 : 0) + (collision_neg[i + d[i]] > 1 ? 1 : 0) +
          (collision_pos[d[j] - j + N] > 1 ? 1 : 0) + (collision_neg[j + d[j]] > 1 ? 1 : 0) -
          (collision_pos[d[j] - i + N] > 0 ? 1 : 0) - (collision_neg[i + d[j]] > 0 ? 1 : 0) -
          (collision_pos[d[i] - j + N] > 0 ? 1 : 0) - (collision_neg[j + d[i]] > 0 ? 1 : 0);

    return cnt;
}


void work()
{
    //N<1000时每种组合都要尝试
    int step = 0;
    int cnt = 0;
    do
    {
        int k = init(N);
        do
        {
            step = 0;
            for(int i = N-k+1; i <= N; i++)
            {
                if(collision_pos[d[i] - i + N] == 1 && collision_neg[d[i] + i] == 1)
                    continue;
                for(int j = 1; j <= N; j++)
                {
                    if(i != j)
                    {
                        int gain = reduce(i,j);
                        if(gain > 0)
                        {
                            do_swap1(i,j,gain);
                            step++;
                        }
                    }
                }
            }
        }
        while(step != 0);
    }
    while(collision > 0);
}


int total(int i)
{
    int sum = 0;
    if(collision_neg[i + d[i]] > 1)
        sum += (collision_neg[d[i] + i] - 1);
    if(collision_pos[d[i] - i + N]>1)
        sum += (collision_pos[d[i] - i + N] - 1);
    return sum;
}


int work1()
{
    //N>1000时用随机化的取点
    int k = init(N);
    for(int i = N - k + 1; i <= N; i++)
    {
        if(total(i)>0)
        {
            bool flag = 0;
            do
            {
                int j = random(1,N);
                if(i == j)
                    continue;
                do_swap2(i,j);
                if((total(i) == 0) && (total(j) == 0))
                {
                    flag = 1;
                }
                else
                    do_swap2(i,j);
            }
            while(!flag);
        }
    }
}

int main()
{
    FILE *fp;
    fp = fopen("out.dat","wb");
    printf("The number of queens: ");
    while(~scanf("%d",&N))
    {
        double ave=0;
//        for(int i=0; i<10; i++)
//        {
            if(N == 1)
            {
                break;
            }

            int tmp=BigtoLittle32(N);
            fwrite(&tmp,sizeof(int),1,fp);
            clock_t start,finish;
            start=clock();
            if(N > 1000)
            {
                work1();
            }
            else
            {
                work();
            }
            for(int i = 1; i <= N; i++)
            {
                d[i]=BigtoLittle32(d[i]);
                fwrite(&d[i],sizeof(int),1,fp);
            }
            finish=clock();
            double doration = (double)(finish - start)/CLOCKS_PER_SEC;
            ave+=doration;
            cout<<"duration: "<<doration<<'s'<<endl;
//        }
//        ave=ave/10.0;
//        cout<<fixed<<setprecision(5)<<ave<<endl;
        printf("The number of queens: ");
    }
    fclose(fp);
    return 0;
}
