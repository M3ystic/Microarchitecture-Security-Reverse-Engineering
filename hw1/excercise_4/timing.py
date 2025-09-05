import pandas as pd
import matplotlib.pyplot as plt
import subprocess

# Compile the AVX-512 C program
subprocess.run(["gcc", "-O0", "-o", "avx512_timing", "avx512_timing.c", "-march=native"])

# Run the compiled program
subprocess.run(['./avx512_timing'])

df = pd.read_csv("avx512_timing.csv")

# --- Overlaid histograms of pre vs post ---
plt.figure(figsize=(10,6))
plt.hist(df['pre_cycles'],  bins=100, alpha=0.6, label='Pre-sleep (100ms) AVX-512 mul')
plt.hist(df['post_cycles'], bins=100, alpha=0.6, label='Post-sleep AVX-512 mul')
plt.xlabel("CPU cycles")
plt.ylabel("Frequency")
plt.title("AVX-512 multiply timing: before vs after 100 ms sleep")
plt.legend()
plt.xlim(0, 600)  # adjust if needed for your machine
plt.tight_layout()
plt.savefig("avx512_wait_pre_post.png", dpi=300)
plt.show()

# --- Histogram of absolute difference ---
plt.figure(figsize=(10,6))
plt.hist(df['delta_cycles'], bins=100, alpha=0.7, label='|post - pre| cycles')
plt.xlabel("Absolute difference in cycles")
plt.ylabel("Frequency")
plt.title("AVX-512 multiply: absolute timing difference after 100 ms sleep")
plt.legend()
plt.xlim(0, 200)  # adjust as needed
plt.tight_layout()
plt.savefig("avx512_wait_delta.png", dpi=300)
plt.show()

