from concurrent.futures import ThreadPoolExecutor
import subprocess

def compile_program(src, out):
    subprocess.run(["gcc", "-O0", "-o", out, src, "-march=native"])

def run_program(prog):
    subprocess.run([prog])

compile_program("512.c", "512avx")
compile_program("128.c", "128avx")

with ThreadPoolExecutor() as executor:
    futures = [executor.submit(run_program, "./512avx"),
               executor.submit(run_program, "./128avx")]

    # blocking
    for programs in futures:
        programs.result()

subprocess.run(["python3", "128.py"])
subprocess.run(["python3", "512.py"])