
import subprocess

print("Uploading filesystem before compilation/build...")

uploadHtmlCommandString = "pio run --target uploadfs"
uploadHtmlCommandArray = uploadHtmlCommandString.split()

try:
    subprocess.run(uploadHtmlCommandArray, check=True)
    print("Uploading HTML successful ✅")
except subprocess.CalledProcessError as e:
    print("Uploading HTML failed ❌")
    print(f"Error: {e}")
    exit(1)