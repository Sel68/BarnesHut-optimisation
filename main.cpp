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
#include <chrono>

using namespace std;

constexpr double G_CONST = 6.67430e-11;
constexpr double COULOMB_K = 8.98755e9;

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
        particles.clear();
        ifstream infile(filename);
        if (!infile.is_open()) throw runtime_error("File error");

        string line;
        int idCounter = 0;
        while (getline(infile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            double x, y, m, vx, vy;
            char c1, c2, c3, c4; // eat commas

            if (ss >> x >> c1 >> y >> c2 >> m >> c3 >> vx >> c4 >> vy) {
                ds::Particle p;
                p.id = idCounter++;
                p.pos = {x, y};
                p.mass = m;
                p.vel = {vx, vy}; 
                p.acc = {0.0, 0.0};
                // p.isStatic = (m > 1e7);
                particles.push_back(p);
            }
        }
        tree = make_unique<ds::BarnesHutTree>(particles.size() * 2);
        updateBounds();
    }

    void initFromManual(int n, double k, double pow) {
        K_val = k;
        Dist_Pow = pow;
        particles.clear();
        cout << "Enter " << n << " particles (x, y, mass):\n";
        for(int i=0; i<n; ++i) {
            double x, y, m;
            cout << "P" << i << ": "; cin >> x >> y >> m;
            ds::Particle p;
            p.id = i; p.pos = {x, y}; p.mass = m;
            p.vel = {0.0,0.0}; p.acc = {0.0,0.0}; p.isStatic = false;
            particles.push_back(p);
        }
        tree = make_unique<ds::BarnesHutTree>(particles.size() * 2);
        updateBounds();
    }
    //demonstrating O(1) particle lookup using hashing.
    void hashingFunc() {
        if (particles.empty()) return;

        
        ds::HashTable<int, ds::Particle*> particleMap(1000);

        
        for (size_t i = 0; i < particles.size(); ++i) {
            particleMap.insert(particles[i].id, &particles[i]);
        }

        
        try {
            int targetID = 42;
            if (targetID < particles.size()) {
                ds::Particle* p = particleMap.search(targetID); 
                cout << "[Hash Search] Success! Found Particle " << targetID 
                     << " at Pos: " << p->pos << endl;
            } else {
                cout << "[Hash Search] Skipped search (Particle " << targetID 
                     << " does not exist in this small dataset)." << endl;
            }
        } catch (...) {
            cout << "[Hash Search] Particle not found." << endl;
        }
        cout << "----------------------------\n";
    }

    void updateBounds() {
        double maxCoord = 0;
        for(const auto& p : particles) {
            maxCoord = max({maxCoord, abs(p.pos.x), abs(p.pos.y)});
        }
        boundaries.halfDim = maxCoord * 1.5 + 10.0;
    }

    void step() {
        auto cmp = [](const ds::Particle& a, const ds::Particle& b) {
            return a.pos.x < b.pos.x;
        };
        ds::merge_sort(particles.begin(), particles.end(), cmp);

        // tree init
        updateBounds();
        tree->build(particles, boundaries);

        ds::Queue<ds::Particle*> jobQueue;
        for (auto& p: particles){
            if (!p.isStatic) jobQueue.enqueue(&p);
        }

        ds::Stack<ds::Particle*> integrationStack;

        while (!jobQueue.empty()){
            ds::Particle* p = jobQueue.front();
            jobQueue.dequeue();
            ds::Vec2D force = tree->getForceOn(p, K_val, Dist_Pow);
            p->acc = force / p->mass;

            integrationStack.push(p);
        }

        while (!integrationStack.empty()) {
            ds::Particle* p = integrationStack.top();
            integrationStack.pop();

            p->vel += p->acc * timeStep;
            p->pos += p->vel * timeStep;
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

void printHeader(string title) {
    cout << "\033[1;36m" << string(40, '=') << "\n";
    cout << "  " << title << "\n";
    cout << string(40, '=') << "\033[0m\n";
}

void runner() {
    Simulation sim;
    double k, p;
    int choice;

    printHeader("BARNES-HUT SIMULATOR CONFIG");

    cout << "1) Select Force Constant (k):\n";
    cout << "   [1] Gravitational (G = " << G_CONST << ")\n   [2] Coulomb (k = " << COULOMB_K<<  ")\n   [3] Custom\n>> ";
    cin >> choice;
    if(choice == 1) k = G_CONST;
    else if(choice == 2) k = COULOMB_K;
    else { cout << "Enter k: "; cin >> k; }

    cout << "\n2) Select Distance Power (1/r^n):\n";
    cout << "   [1] Linear (n=1)\n   [2] Inverse Square (n=2)\n   [3] Custom\n>> ";
    cin >> choice;
    if(choice == 1) p = 1.0;
    else if(choice == 2) p = 2.0;
    else { cout << "Enter n: "; cin >> p; }

    cout << "\n3) Input Source:\n";
    cout << "   [1] Read 'random_coordinates.txt'\n   [2] Manual Entry\n>> ";
    cin >> choice;

    try {
        if(choice == 1) {
            sim.initFromFile("random_coordinates.txt", k, p);
        } else {
            int n; cout << "Number of particles: "; cin >> n;
            sim.initFromManual(n, k, p);
        }

        int steps;
        cout << "\n4) Simulation Steps: "; cin >> steps;

        auto start = chrono::high_resolution_clock::now();
        sim.run(steps, "simulation_output.txt");
        auto end = chrono::high_resolution_clock::now();
        
        chrono::duration<double> elapsed = end - start;
        cout << "\033[1;32mTotal Time: " << fixed << setprecision(4) << elapsed.count() << "s\033[0m\n";

    } catch (const exception& e) {
        cerr << "\033[1;31mERROR: " << e.what() << "\033[0m" << endl;
    }
}

int main() {
    runner(); return 0;   
}