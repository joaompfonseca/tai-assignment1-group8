import os
import pandas as pd

# Function to get the size of a file in bits
def get_size(f):
    return os.path.getsize(f) * 8

dict = {}

# If results directory does not exist, create it
if not os.path.exists("results"):
    os.makedirs("results")

# Open file to write results
file = open("results/other_comp.csv", "w")

# Write header to file
file.write("mutate,pred,gzip,bzip2,tarxz\n")

# Get compression ratios for different compression algorithms
for i in range(1, 10):
    x = i / 10
    original = "example/chry_" + str(x) + ".txt"
    fgzip = "example/chry_" + str(x) + ".gz"
    fbzip2 = "example/chry_" + str(x) + ".bz2"
    ftarxz = "example/chry_" + str(x) + ".tar.xz"
    pred = "results/mutate_" + str(x) + ".csv"

    # Get size of original file
    original = get_size(original)

    # Get size of predicted compression
    data = pd.read_csv(pred)
    size_pred = data["totalb"][0]

    # Get size of compressed files
    size_gzip = get_size(fgzip)
    size_bzip2 = get_size(fbzip2)
    size_tarxz = get_size(ftarxz)

    # Calculate compression ratio
    ratio_pred = ((original - size_pred) / original) * 100
    ratio_gzip = ((original - size_gzip) / original) * 100
    ratio_bzip2 = ((original - size_bzip2) / original) * 100
    ratio_tarxz = ((original - size_tarxz) / original) * 100

    dict[x] = ratio_pred

    # Write results to file
    file.write(str(x) + "," + str(ratio_pred) + "," + str(ratio_gzip) + "," + str(ratio_bzip2) + "," + str(ratio_tarxz) + "\n")

file.close()

# Open file to write results
file = open("results/original_comp.csv", "w")

file.write("pred,gzip,bzip2,tarxz\n")

# Get size of predicted compression
data = pd.read_csv("results/default.csv")
size_pred = data["totalb"][0]

# Get size of compressed files
size_gzip = get_size("example/chry.gz")
size_bzip2 = get_size("example/chry.bz2")
size_tarxz = get_size("example/chry.tar.xz")

# Get size of original file
original = get_size("example/chry.txt")

# Calculate compression ratio
ratio_pred = ((original - size_pred) / original) * 100
ratio_gzip = ((original - size_gzip) / original) * 100
ratio_bzip2 = ((original - size_bzip2) / original) * 100
ratio_tarxz = ((original - size_tarxz) / original) * 100

dict[0] = ratio_pred

# Write results to file
file.write(str(ratio_pred) + "," + str(ratio_gzip) + "," + str(ratio_bzip2) + "," + str(ratio_tarxz) + "\n")

file.close()

# Compare between predicted original and mutated files
file = open("results/compare.csv", "w")

file.write("pred,mutate_0.1,mutate_0.2,mutate_0.3,mutate_0.4,mutate_0.5,mutate_0.6,mutate_0.7,mutate_0.8,mutate_0.9\n")

file.write(str(dict[0]) + "," + str(dict[0.1]) + "," + str(dict[0.2]) + "," + str(dict[0.3]) + "," + str(dict[0.4]) + "," + str(dict[0.5]) + "," + str(dict[0.6]) + "," + str(dict[0.7]) + "," + str(dict[0.8]) + "," + str(dict[0.9]) + "\n")

file.close()
