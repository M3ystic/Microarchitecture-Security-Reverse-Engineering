import pandas as pd
import matplotlib.pyplot as plt
import subprocess

# Compile the AVX-512 C program
subprocess.run(["gcc", "-O0", "-o", "cache_timing", "cache_timing.c", "-march=native"])

# Run the compiled program
subprocess.run(["./cache_timing"])

# Load CSV
data = pd.read_csv("avx512_timing.csv")

# Histogram of cycle counts
plt.figure(figsize=(10,6))
plt.hist(data['avx512_cycles'], bins=100, alpha=0.7, color='purple')
plt.xlabel("CPU cycles")
plt.ylabel("Frequency")
plt.title("AVX-512 Operation Timing Distribution")
plt.xlim(0, 500)  # shrink scale if you only care about small cycle counts
plt.savefig("avx512_histogram.png")
plt.show()
