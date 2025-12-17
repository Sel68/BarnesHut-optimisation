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

class NaiveSimulation {
    vector<ds::Particle> ps;
    double dt = 0.01;
    double K_val, Dist_Pow;
    ofstream file;

public:
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
                p.id = id++; p.pos = {x, y}; p.mass = m;
                p.vel = {0, 0}; p.acc = {0, 0}; p.isStatic = false;
                ps.push_back(p);
            }
        }
        cout << "Naive Loaded: " << ps.size() << "\n";
    }

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
        
        for(auto& p : ps) {
            if(p.isStatic) continue;
            p.vel = p.vel + p.acc * dt;
            p.pos = p.pos + p.vel * dt;
        }
    }

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
        sim.run(100, "simulation_output_naive.txt");
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;

        cout << "Execution time: " << elapsed.count() << "seconds\n";

        return 0;

}