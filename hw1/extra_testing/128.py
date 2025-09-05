import pandas as pd
import matplotlib.pyplot as plt

# --- Load the CSV ---
df = pd.read_csv("avx_timing_50ms.csv")

# --- Overlaid histograms of pre vs post ---
plt.figure(figsize=(10,6))
plt.hist(df['pre_cycles'], bins=100, alpha=0.6, label='Pre-sleep (50ms) AVX mul')
plt.hist(df['post_cycles'], bins=100, alpha=0.6, label='Post-sleep AVX mul')
plt.xlabel("CPU cycles")
plt.ylabel("Frequency")
plt.title("AVX multiply timing: before vs after 50 ms sleep")
plt.legend()
plt.xlim(0, 600)  # adjust if needed
plt.tight_layout()
plt.savefig("avx_wait_50ms_pre_post.png", dpi=300)
plt.show()

# --- Histogram of absolute difference ---
plt.figure(figsize=(10,6))
plt.hist(df['delta_cycles'], bins=100, alpha=0.7, label='|post - pre| cycles')
plt.xlabel("Absolute difference in cycles")
plt.ylabel("Frequency")
plt.title("AVX multiply: absolute timing difference after 50 ms sleep")
plt.legend()
plt.xlim(0, 200)  # adjust as needed
plt.tight_layout()
plt.savefig("avx_wait_50ms_delta.png", dpi=300)
plt.show()
