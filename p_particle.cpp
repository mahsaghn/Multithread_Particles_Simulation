#include "constants.h"
#include "particle.h"

using namespace std;
using namespace constants;

void Particle ::collision(long map[N][N][2])
{
    bool same=0, other=0;
    if (map[x][y][color] > 1)
        same = 1;
    if (map[x][y][(color+1)%2] > 0)
        other = 1;
    if (other || same)
        calculateNewEquations();
    return;
}

void Particle ::calculateNewEquations(){
    setNewA();
    setNewB();
    setNewC();
    setNewD();
    return;
}

void Particle ::setNewA()
{
    long v = (10 + (prev_x - y)%10)%10;
    if(v!=0)
    {
        if ((prev_x - y) < 0)
            a = v;
        else if ((prev_x - y) > 0)
            a = -1 * v;
    }
}

void Particle ::setNewB()
{
    long v = (30 + (prev_x - prev_y)%30)%30;
    if(v!=0)
    {
        if ((prev_x - prev_y) < 0)
            b = v;
        else if ((prev_x - prev_y) > 0)
            b = -1 * v;
    }
}

void Particle ::setNewC()
{
    long v = (10 + (prev_y - x)%10)%10;
    if(v!=0)
    {
        if ((prev_y - x) < 0)
            c = v;
        else if ((prev_y - x) > 0)
            c = -1 * v;
    }
}

void Particle ::setNewD()
{
    if((30 + (x - y)%30)%30!=0)
    {
        if ((x - y) < 0)
            d = (30 + (x - y)%30)%30;
        else if ((x - y) > 0)
            d = -1 * ((30 + (x - y)%30)%30);        
    }
}

void Particle ::goNextLocation(long map[N][N][2])
{
    prev_x = x;
    prev_y = y;
    x = (N + ((a * x + b) % N))%N;
    y = (N + ((c * y + d) % N))%N;
    #pragma omp atomic
    map[x][y][color] += 1;
}


