import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results.csv')
df.columns = df.columns.str.strip()

plt.figure(figsize=(10, 6))

plt.plot(df['Number of points'], df['naive'], 'o-', label='Naive Algorithm ($O(n^2)$)', color='red')
plt.plot(df['Number of points'], df['BarnesHut'], 's-', label='Barnes-Hut Algorithm ($O(n \log n)$)', color='blue')

plt.xlabel('Number of Particles ($N$)')
plt.ylabel('Execution Time (seconds)')
plt.title('Performance Comparison: Naive vs. Barnes-Hut')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)

plt.savefig("performance_comparison.png", dpi=300, bbox_inches="tight")
plt.close()