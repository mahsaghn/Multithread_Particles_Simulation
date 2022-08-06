#include "constants.h"
#include <iostream>
#include <tuple>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <functional>
#include <vector>

using namespace std;
using namespace constants;

class Particle
{
    public:
        long a = 0, b = 0, c = 0, d = 0;
        long x = 0, y = 0;
        long prev_x, prev_y;
        long energy = 0;
        bool color = 0;
        Particle(long ai, long bi, long ci, long di, long xi, long yi, bool colori)
        {
            energy = 0;
            x = xi;
            y = yi;
            prev_x = x;
            prev_y = y;
            a = ai;
            b = bi;
            c = ci;
            d = di;
            color = colori;
        }
        void collision(long [N][N][2]);
        void goNextLocation(long [N][N][2]);
        void calculateNewEquations();
        // virtual ~Particle(){}

    private:
        void setNewA();
        void setNewB();
        void setNewC();
        void setNewD();
};
