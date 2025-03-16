import subprocess

print("Uploading filesystem before compilation/build...")

subprocess.run(['pio', 'run','--target', 'clean'], check=True)
subprocess.run(['pio', 'run', '--target', 'uploadfs'], check=True)
print("Uploading HTML successful ✅")