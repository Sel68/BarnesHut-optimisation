#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;


struct Body {
    double x, y;     
    double vx, vy;  
    double mass;     
};


struct Force {
    double fx, fy;   
};

const double G = 1.0;      
const double DT = 0.01;  
const double EPS = 1e-3;   

int main() {
    int N, STEPS;
    cout << "Enter number of particles: ";
    cin >> N;

    cout << "Enter number of simulation steps: ";
    cin >> STEPS;

    Body bodies[N];       
    Force forces[N];      
    for (int i = 0; i < N; i++) {
        cin >> bodies[i].x >> bodies[i].y >> bodies[i].mass;
        bodies[i].vx = bodies[i].vy = 0.0;
    }

    ofstream fout("simulation_output.txt");

    
    fout << "x, y, mass\n";


    for (int step = 0; step < STEPS; step++) {
        
        for (int i = 0; i < N; i++) {
            forces[i].fx = 0.0;
            forces[i].fy = 0.0;
        }

    
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                double dx = bodies[j].x - bodies[i].x;
                double dy = bodies[j].y - bodies[i].y;
                double distSq = dx*dx + dy*dy + EPS;
                double dist = sqrt(distSq);

                double F = (G * bodies[i].mass * bodies[j].mass) / distSq;
                double Fx = F * dx / dist;
                double Fy = F * dy / dist;

                forces[i].fx += Fx;
                forces[i].fy += Fy;
                forces[j].fx -= Fx;   
                forces[j].fy -= Fy;
            }
        }

        
        for (int i = 0; i < N; i++) {
            bodies[i].vx += forces[i].fx / bodies[i].mass * DT;
            bodies[i].vy += forces[i].fy / bodies[i].mass * DT;

            bodies[i].x += bodies[i].vx * DT;
            bodies[i].y += bodies[i].vy * DT;

            
            fout << bodies[i].x << ", " << bodies[i].y << ", " << bodies[i].mass << "\n";
        }

        
        fout << "\n";
        cout << "Step " << step << " recorded.\n";
    }

    fout.close();
    cout << "Simulation complete. Output saved in simulation_output.txt\n";
    return 0;
}
