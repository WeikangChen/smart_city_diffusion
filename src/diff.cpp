#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using namespace std;

#define DIST2(dx, dy) (dx*dx+dy*dy)
#define DIST1(dx, dy) sqrt(dx*dx+dy*dy)

const int light_num = 200;
const double map_size = 1000;
const double close_size = 50;
const double map_x = map_size;
const double map_y = map_size;
double chem_x = 0.5 * map_x;
double chem_y = 0.5 * map_y;

const double PI = 3.141592;
const double DIFF = 2500.0;
const double R = 0.05;
const double DT = 0.1;
const double T0 = 0.01;
    
struct Light {
    static int total;
    int id;
    double loc_x;
    double loc_y;
    double dis_x;
    double dis_y;
    double dist2;
    double dist1;
    double derdis;
    vector<double> chem_den;
    Light(double x, double y, double den = 0.0): loc_x(x), loc_y(y) {
        chem_den.clear();
        id = ++total;
        dis_x = loc_x - chem_x;
        dis_y = loc_y - chem_y;
        dist2 = DIST2(dis_x, dis_y);
        dist1 = DIST1(dis_x, dis_y);
    }
};

int Light::total = 0;

void print_source()
{
    ofstream logfile;
    logfile.open ("source.txt");
    logfile << chem_x <<  " " << chem_y << endl;

}


void print_lights(vector<Light> &lights)
{
    ofstream logfile;
    logfile.open ("lights_dist.txt");
    vector<Light>::iterator it = lights.begin();
    for(;it != lights.end(); ++it)
    {
        logfile << it->loc_x <<  " " << it->loc_y << " " << it->dist1 << endl;
    }
}

void generate_source()
{
    for(int i = 0; i < 3; ++i)
        rand();
    chem_x = map_x * rand() / RAND_MAX;
    chem_y = map_y * rand() / RAND_MAX;


    return;
}
void generate_lights(vector<Light> &lights, int required)
{
    int created = 0;
    while(created < required)
    {
        double ran_x = map_x * rand() / RAND_MAX;
        double ran_y = map_y * rand() / RAND_MAX;
        double dx = ran_x - chem_x;
        double dy = ran_y - chem_y;
        if(DIST1(dx, dy) < close_size) continue;
        bool too_close = false;
        for(int i = 0; i < created; ++i)
        {
            double dx = lights[i].loc_x - ran_x;
            double dy = lights[i].loc_y - ran_y;
            if(DIST1(dx, dy) < close_size)
            {
                too_close = true;
                break;
            }
        }
        if(!too_close)
        {
            lights.push_back(Light(ran_x, ran_y));
            created++;
        }
        
    }
    return;
}


double diffusion_sol(double dist2, double T)
{
    double res = 1.0 / (sqrt(4*PI*DIFF*T));
    res *= exp(-(dist2/(4.0*DIFF*T)));
    res *= (1 + R*(2.0*rand()/RAND_MAX - 1.0));
    return res;
}

void diffusion_start(vector<Light> &lights)
{
    double T = T0;
    for(int i = 0; i < 50; i++)
    {
        vector<Light>::iterator it = lights.begin();
        for(;it != lights.end(); ++it)
        {
            double density = diffusion_sol(it->dist2, T);
            it->chem_den.push_back(density);
        }
        T += DT;
    }
}

Light *get_closest(vector<Light> lights)
{
    double min_distance = DIST1(map_x, map_y);
    Light *closest_light = NULL;
    vector<Light>::iterator it = lights.begin();
    for(;it != lights.end(); ++it)
    {
        if(it->dist1 < min_distance) {
            min_distance = it->dist1;
            closest_light = &(*it);
        }
    }
    return closest_light;
}

void print_density(Light &l, string filename)
{
    ofstream logfile;
    logfile.open (filename.c_str());
    vector<double>::iterator it = l.chem_den.begin();
    double T = T0;

    for(; it != l.chem_den.end(); ++it)
    {
        logfile << T << "\t"<< *it << endl;
        T +=  DT;
    }
}

bool comp_dist(Light a, Light b) {return a.dist1 < b.dist1;}

double deriveX(double x1, double y1, double r1, double x2, double y2, double r2, double x3, double y3, double r3)
{
    return ((x2*x2+y2*y2-r2*r2)*(y3-y1) + (x3*x3+y3*y3-r3*r3)*(y1-y2) + (x1*x1+y1*y1-r1*r1)*(y2-y3)) / (2*(x3*(y1-y2) + x1*(y2-y3) + x2*(-y1+y3)));
}

double deriveY(double x1, double y1, double r1, double x2, double y2, double r2, double x3, double y3, double r3)
{
    return ((r3*r3-x3*x3-y3*y3)*(x1-x2) + (r1*r1-x1*x1-y1*y1)*(x2-x3) + (r2*r2-y2*y2-x2*x2)*(x3-x1)) / (2*(x3*(y1-y2) + x1*(y2-y3) + x2*(-y1+y3)));
}

void derive_source(vector<Light> &lights)
{
    for(int i = 2; i < 4; i ++)
    {
        Light li = lights[i];
        for(int j = 1; j < i; ++j)
        {
            Light lj = lights[j];
            for(int k = 0; k < j; ++k)
            {
                Light lk = lights[k];
                double der_x = deriveX(li.loc_x, li.loc_y, li.dist1, lj.loc_x, lj.loc_y, lj.dist1, lk.loc_x, lk.loc_y, lk.dist1);
                double der_y = deriveY(li.loc_x, li.loc_y, li.dist1, lj.loc_x, lj.loc_y, lj.dist1, lk.loc_x, lk.loc_y, lk.dist1);
                cout << "derived pos = " << der_x << ", " << der_y << endl;
            }
        }
    }
}

int main(int argc, char ** argv)
{
    vector<Light> lights;

    //srand(time(NULL));
    generate_source();
    generate_lights(lights, light_num);

    diffusion_start(lights);
    sort(lights.begin(), lights.end(), comp_dist);
    
    print_source();
    print_lights(lights);

    //Light *closest = get_closest(lights);
    //cout << "The closest light is at " << closest->loc_x << ", " << closest->loc_y << endl; 



    lights[0].derdis = 62.6065747585342;
    lights[2].derdis = 78.3182850872055;
    lights[4].derdis = 96.8308358828801;
    Light &li = lights[0];
    Light &lj = lights[2];
    Light &lk = lights[4];
    double der_x = deriveX(li.loc_x, li.loc_y, li.derdis, lj.loc_x, lj.loc_y, lj.derdis, lk.loc_x, lk.loc_y, lk.derdis);
    double der_y = deriveY(li.loc_x, li.loc_y, li.derdis, lj.loc_x, lj.loc_y, lj.derdis, lk.loc_x, lk.loc_y, lk.derdis);
    cout << "source placed = (" << chem_x <<  ", " << chem_y << ")" << endl;
    cout << "detected pos  = (" << der_x << ", " << der_y << ")" << endl;
    //derive_source(lights);


    cout << "Total light# = " << lights.size() << endl;
    for(int i = 0; i < 6; ++i)
    {
        cout << "\t" << i+1 << ": Light#" << lights[i].id 
             << ", dist=" << lights[i].dist1 
             << ", coor=(" << lights[i].loc_x 
             << ", " << lights[i].loc_y << ")" <<endl;
        char filename[32] = "";
        sprintf(filename,"density_%d.txt", i+1);
        print_density(lights[i], filename);
    }
    return 0;
}
