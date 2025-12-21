### Barnes-Hut Simulation
An optimized N-body physics simulation that reduces computational complexity from $O(N^2)$ to $O(N \log N)$ using a recursive **QuadTree** data structure. By approximating distant clusters of particles as single centers of mass (using the Multipole Acceptance Criterion), the system can efficiently simulate thousands of interacting bodies in real-time.

![alt text](image.png)