import pandas as pd
import matplotlib.pyplot as plt
import subprocess

import subprocess

subprocess.run(["gcc", "-O0", "-o", "cache_timing", "cache_timing.c", "-march=native"])
subprocess.run(["./cache_timing"])
data =  pd.read_csv("avx2_timing.csv")

import matplotlib.pyplot as plt

# Load CSV
data = pd.read_csv("avx2_timing.csv")

# Histogram of cycle counts
plt.figure(figsize=(10,6))
plt.hist(data['avx2_cycles'], bins=100, alpha=0.7, color='blue')
plt.xlabel("CPU cycles")
plt.ylabel("Frequency")
plt.title("AVX2 Operation Timing Distribution")
plt.xlim(0, 500)  # shrink scale if you only care about small cycle counts
plt.savefig("avx2_histogram.png")
plt.show()