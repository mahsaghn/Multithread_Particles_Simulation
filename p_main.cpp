#include <iostream>
#include <tuple>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <cstring>
#include <omp.h>
#include <vector>
#include <cmath>

// #include <stdio.h>
#include "particle.h"
#include "constants.h"
using namespace std::chrono;
using namespace std;

long blue_energy = 0;
long red_energy = 0;
long colisions_blue=0;
long colisions_red=0;
long colisions_tot=0;
long map[N][N][2]= {0};

vector<string> split(const string &s, char delim)
{
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }
    return result;
}

Particle* read_record(Particle particles[NPARTICLE])
{
    ifstream fin("input.txt", ios ::in);
    if (!fin.is_open())
    {
        cout << "No particles.csv file.";
        throw std::runtime_error("Could not open file");
    }
    long x, y, a, b, c, d;
    bool color;
    string line, word, temp;
    fin >> temp;
    long i = 0;
    while (i < NPARTICLE)
    {
        fin >> temp;
        vector<string> row = split(temp, ',');
        color = (bool)stoi(row[0]);
        x = stoi(row[1]);
        y = stoi(row[2]);
        a = stoi(row[3]);
        b = stoi(row[4]);
        c = stoi(row[5]);
        d = stoi(row[6]);
        particles[i] = Particle(a, b, c, d,x, y,  color);
        i++;
    }
    return particles;
}

void calc_col_map(Particle particles[NPARTICLE])
{
    #pragma omp parallel 
    {
        #pragma omp for collapse(2) reduction(+:colisions_blue,colisions_red,colisions_tot, blue_energy, red_energy) nowait
        for (long i = 0; i < N; i++)
        {
            for (long j = 0; j < N; j++)
            {
                bool m_blue=false, m_red=false, m_multi=false;
                if(map[i][j][1]>1){
                    m_blue = 1;
                    colisions_blue++;
                }
                if(map[i][j][0]>1){
                    m_red = 1; 
                    colisions_red++;
                }
                if(map[i][j][0] >0 && map[i][j][1] >0)
                {
                    m_multi = 1;
                    red_energy += (map[i][j][0]);
                    blue_energy += (map[i][j][1]);
                }
                if(m_blue || m_red || m_multi)
                    colisions_tot++;
            }
        }
    }

}

void CleanMap()
{
    memset(map, 0, 2*N*N*(sizeof(long)));
}

void generate_output(Particle particles[NPARTICLE]){
    ofstream out("output.txt");
    if (!out.is_open())
    {
        cout << "No output_particles.csv file.";
        throw std::runtime_error("Could not open file");
    }
    string header = "color,i,j,a,b,c,d\n";
    string temp;
    out << header;
    for(long i=0; i< NPARTICLE; i++)
    {       
        temp = to_string(particles[i].color) + "," + to_string(particles[i].x) + "," 
            + to_string(particles[i].y) + "," +to_string(particles[i].a) + ","
            + to_string(particles[i].b) + "," +to_string(particles[i].c) + ","
            + to_string(particles[i].d) + "\n" ;
        out << temp;
    }
}

int main()
{
    long time_clear=0, time_collision=0, time_clean=0, time_update=0, time_calc=0;
    Particle *particles = (Particle*)malloc((sizeof(Particle))*NPARTICLE);

    particles = read_record(particles);
    auto start_time = high_resolution_clock::now();;

    for (long t = 0; t < 15000; t++)
    {  
        #pragma omp parallel for
        for (long i = 0; i < NPARTICLE; i++)
            particles[i].goNextLocation(map);
        
        #pragma omp parallel for
        for (long i = 0; i < NPARTICLE; i++)
            particles[i].collision(map);

        calc_col_map(particles);
        CleanMap();
    }
    generate_output(particles);
    free(particles);
    auto stop_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_time - start_time);
    cout << "Time taken by function: " << duration.count()/1000000.0 << " seconds" << endl;
    cout<< "Blue : \t: COL: "<< colisions_blue << "\tENRGY: "<< blue_energy*ENERGY << endl;
    cout<< "Red  : \t: COL: "<< colisions_red << "\tENRGY: "<< red_energy*ENERGY << endl;
    cout<< "TOTAL COLLISION: " << colisions_tot << endl;

}