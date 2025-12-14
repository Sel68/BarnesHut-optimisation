#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include "ds.hpp"

using namespace std;

class Simulation {
private:
    vector<ds::Particle> particles;
    unique_ptr<ds::BarnesHutTree> tree;
    double timeStep;
    ds::BoundingBox boundaries;

    // Force Config
    double K_val;
    double Dist_Pow;

    ofstream dataFile;

public:
    Simulation(){
        timeStep = 0.01;
        boundaries = {ds::Vec2D(0.0,0.0), 1000};
    }

    void initFromFile(const string& filename, double k, double pow) {
        K_val = k;
        Dist_Pow = pow;
        particles.clear(); //clear state

        ifstream infile(filename);
        if (!infile.is_open()) {
            throw runtime_error("Cannot open input file: " + filename);
        }
        string line;
        int idCounter = 0;

        while (getline(infile, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            double x, y, m;
            char comma; 

            // parser for: x, y, mass
            if (ss >> x >> comma >> y >> comma >> m) {
                ds::Particle p;
                p.id = idCounter++;
                p.pos = {x, y};
                p.mass = m;
                p.vel = {0.0, 0.0}; // Default set to zero velocity
                p.acc = {0.0, 0.0};
                p.isStatic = false; // Default non-static
                particles.push_back(p);
            }
        }

        // Initialize tree with size detected from file
        tree = make_unique<ds::BarnesHutTree>(particles.size() * 2);
        updateBounds();
        
        cout << "Loaded " << particles.size() << " particles from file.\n";
    }

    void updateBounds() {
        double maxCoord = 0;
        for(const auto& p : particles) {
            maxCoord = max({maxCoord, abs(p.pos.x), abs(p.pos.y)});
        }
        boundaries.halfDim = maxCoord * 1.5 + 10.0;
    }

    void step() {
        // tree init
        updateBounds();
        tree->build(particles, boundaries);

        // Compute Forces (maybe parallelize with pragma later)
        for (auto& p : particles) {
            if(p.isStatic) continue;
            ds::Vec2D force = tree->getForceOn(&p, K_val, Dist_Pow);
            p.acc = force / p.mass;
        }

        // Integrate, Symplectic Euler
        for (auto& p : particles) {
            if(p.isStatic) continue;
            p.vel += p.acc *timeStep;
            p.pos += p.vel * timeStep;
        }
    }

    void run(int steps, const string& filename) {
        dataFile.open(filename);
        if(!dataFile.is_open()) throw runtime_error("Cannot open file");
        
        cout << "Starting Simulation: " << steps << " steps.\n";
        
        for(int i=0; i<steps; i++) {
            step();
            // Save every frame (for now)
            for(size_t j=0; j<particles.size(); ++j) {
                dataFile << particles[j].pos.x << ", " << particles[j].pos.y  <<  ", " << particles[j].mass << "\n";
            }
            dataFile << "\n\n";
            int MOD = steps/10;
            if (i % MOD == 0) cout << "Step " << i << " complete.\n";
        }
        dataFile.close();
        cout << "Done.\n";
    }
};


int main() {
    try {
        Simulation sim;
        
        sim.initFromFile("random_coordinates.txt", 1, 1.0); 
        sim.run(100, "simulation_output.txt");
        
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
    }

    return 0;
}