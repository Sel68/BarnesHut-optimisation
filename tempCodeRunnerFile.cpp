#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

// Structure to store particle information
struct Body {
    double x, y;     // position
    double vx, vy;   // velocity
    double mass;     // mass
};

// Structure to store forces on a particle
struct Force {
    double fx, fy;   // force components
};

const double G = 1.0;      // gravitational constant
const double DT = 0.01;    // time step
const double EPS = 1e-3;   // softening to avoid singularities

int main() {
    int N, STEPS;
    cout << "Enter number of particles: ";
    cin >> N;

    cout << "Enter number of simulation steps: ";
    cin >> STEPS;

    Body bodies[N];       // array of particles
    Force forces[N];      // array of forces

    cout << "Enter x y mass for each particle:\n";
    for (int i = 0; i < N; i++) {
        cin >> bodies[i].x >> bodies[i].y >> bodies[i].mass;
        bodies[i].vx = bodies[i].vy = 0.0;
    }

    ofstream fout("simulation_output.txt");

    // Write header line
    fout << "x, y, mass\n";

    // Simulation loop
    for (int step = 0; step < STEPS; step++) {
        // Reset forces to zero
        for (int i = 0; i < N; i++) {
            forces[i].fx = 0.0;
            forces[i].fy = 0.0;
        }

        // Compute pairwise forces
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
                forces[j].fx -= Fx;   // Newton's 3rd law
                forces[j].fy -= Fy;
            }
        }

        // Update velocities and positions
        for (int i = 0; i < N; i++) {
            bodies[i].vx += forces[i].fx / bodies[i].mass * DT;
            bodies[i].vy += forces[i].fy / bodies[i].mass * DT;

            bodies[i].x += bodies[i].vx * DT;
            bodies[i].y += bodies[i].vy * DT;

            // Write to file: x, y, mass
            fout << bodies[i].x << ", " << bodies[i].y << ", " << bodies[i].mass << "\n";
        }

        // Blank line separates steps
        fout << "\n";
        cout << "Step " << step << " recorded.\n";
    }

    fout.close();
    cout << "Simulation complete. Output saved in simulation_output.txt\n";
    return 0;
}
