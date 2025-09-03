import pandas as pd
import matplotlib.pyplot as plt
import subprocess

import subprocess

subprocess.run(["gcc", "-O0", "-o", "cache_timing", "cache_timing.c", "-march=native"])
subprocess.run(["./cache_timing"])
data =  pd.read_csv("cache_timing.csv")

# Plot histogram
plt.figure(figsize=(10,6))
plt.hist(data['hit'], bins=100, alpha=0.6, label='Cache Hit')
plt.hist(data['miss'], bins=100, alpha=0.6, label='Cache Miss')
plt.xlabel("CPU cycles")
plt.ylabel("Frequency")
plt.title("Cache Hit vs Miss Timing Distribution")
plt.legend()
plt.xlim(0, 10000)
plt.savefig("cache_histogram.png")
plt.show()