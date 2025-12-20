#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include "ds.hpp"

using namespace std;

// Defined simulation constants
class NaiveSimulation {
    vector<ds::Particle> ps;
    double dt = 0.01;
    double K_val, Dist_Pow;
    ofstream file;

public:
//function to read particle data from a  text file 
    void init(const string& f, double k, double p) {
        K_val = k; Dist_Pow = p; ps.clear();
        ifstream in(f);
        if(!in.is_open()) throw runtime_error("File error");
        
        string l; int id = 0;
        while(getline(in, l)) {
            if(l.empty()) continue;
            stringstream ss(l);
            double x, y, m, vx, vy;
            char c1, c2, c3, c4;
            
            if(ss >> x >> c1 >> y >> c2 >> m >> c3 >> vx >> c4 >> vy) {
                ds::Particle p;
                p.id = id++; p.pos = {x, y}; p.mass = m;
                p.vel = {vx, vy}; p.acc = {0, 0}; p.isStatic = false;
                ps.push_back(p);
            }
        }
        if(ps.empty()) cout << "Warning: No particles loaded. Check file format!\n";
        else cout << "Naive Loaded: " << ps.size() << " particles.\n";
    }
    //function to perform a single simulation step using naive O(N^2) approach
    void step() {
        int n = ps.size();
        
        for(auto& p : ps) p.acc = {0.0, 0.0};

        
        for(int i = 0; i < n; ++i) {
            if(ps[i].isStatic) continue;
            ds::Vec2D force = {0.0, 0.0};

            for(int j = 0; j < n; ++j) {
                if(i == j) continue;

                ds::Vec2D r = ps[j].pos - ps[i].pos;
                double dist = sqrt(r.x*r.x + r.y*r.y); 
                double effD = max(dist, 1e-5);

                
                double fMag = (K_val * ps[i].mass * ps[j].mass) / pow(effD, Dist_Pow);
                ds::Vec2D fDir = r / effD;

                force = force + (fDir * fMag);
            }
            ps[i].acc = force / ps[i].mass;
        }
        //Integrated positions and velocities (Euler Method)
        for(auto& p : ps) {
            if(p.isStatic) continue;
            p.vel = p.vel + p.acc * dt;
            p.pos = p.pos + p.vel * dt;
        }
    }
// Used to run the simulation loop for 'steps' iterations
    void run(int steps, const string& outName) {
        file.open(outName);
        cout << "Naive Run: " << steps << " steps.\n";
        
        for(int i = 0; i < steps; ++i) {
            step();
            for(const auto& p : ps) {
                file << p.pos.x << ", " << p.pos.y << ", " << p.mass << "\n";
            }
            file << "\n\n";
            
            if(steps >= 10 && i % (steps/10) == 0) cout << "Step " << i << "\n";
        }
        file.close();
        cout << "Done.\n";
    }
};

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    try {
        NaiveSimulation sim;
        sim.init("random_coordinates.txt", 1.0, 1.0);
        sim.run(200, "simulation_output_naive.txt");
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
// To print total time (Used by test.sh to compare performance)
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Execution time: " << elapsed.count() << " seconds\n";

    return 0;

}