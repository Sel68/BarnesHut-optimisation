#include <iostream>
#include <vector>       // Currently using vector for simplicity
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "ds.hpp"       // to contain ds::Particle and ds::Vec2D

using namespace std;


// Implements the Naive O(N^2) N-body simulation
class NaiveSimulation {
    vector<ds::Particle> ps;   
    double dt = 0.01;          
    double K_val, Dist_Pow;  
    ofstream file;             

public:
    
    // we initialize simulation by loading particles from file
    void init(const string& f, double k, double p) {
        K_val = k; Dist_Pow = p; ps.clear();
        ifstream in(f);
        if(!in.is_open()) throw runtime_error("File error");
        
        string l; int id = 0;
        while(getline(in, l)) {
            if(l.empty()) continue;       
            stringstream ss(l);
            double x,y,m; char c;
            
            if(ss >> x >> c >> y >> c >> m) {
                ds::Particle p;
                p.id = id++; 
                p.pos = {x, y};
                p.mass = m;
                p.vel = {0, 0};
                p.acc = {0, 0};
                p.isStatic = false;        // we are setting all particles to dynamic
                ps.push_back(p);
            }
        }
        cout << "Naive Loaded: " << ps.size() << " particles\n"; 
    }

    
    void step() {
        int n = ps.size();

        
        for(auto& p : ps) p.acc = {0.0, 0.0};

        // Naive O(N^2) used for force calculation
        for(int i = 0; i < n; ++i) {
            if(ps[i].isStatic) continue; 
            ds::Vec2D force = {0.0, 0.0};

            for(int j = 0; j < n; ++j) {
                if(i == j) continue;     

                
                ds::Vec2D r = ps[j].pos - ps[i].pos;
                double dist = sqrt(r.x*r.x + r.y*r.y); 
                double effD = max(dist, 1e-5); // Avoid division by zero

                double fMag = (K_val * ps[i].mass * ps[j].mass) / pow(effD, Dist_Pow);

                
                ds::Vec2D fDir = r / effD;

                
                force = force + (fDir * fMag);
            }

            
            ps[i].acc = force / ps[i].mass;
        }
        
        // we are updating velocities and positions using simple Euler integration
        for(auto& p : ps) {
            if(p.isStatic) continue;
            p.vel = p.vel + p.acc * dt;
            p.pos = p.pos + p.vel * dt;
        }
    }

    
    // we ran simulation for given number of steps and wrote to file
    void run(int steps, const string& outName) {
        file.open(outName);
        cout << "Naive Run: " << steps << " steps.\n";
        
        for(int i = 0; i < steps; ++i) {
            step(); 

            //for loop used to write particle positions to file
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
    try {
        NaiveSimulation sim;

        
        // we loaded particles from random_coordinates.txt with K=1.0 and P=1.0
        sim.init("random_coordinates.txt", 1.0, 1.0);

        
        // we ran simulation for 100 steps and saved output
        sim.run(100, "simulation_output_naive.txt");
        
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}
