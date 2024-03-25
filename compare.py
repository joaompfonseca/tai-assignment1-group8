import os


# Function to get the size of a file in bits
def get_size(f):
    return os.path.getsize(f) * 8


# Get number of characters in original file
file = open("example/chry.txt", "r")
text = file.read()
file.close()
original = len(text)

# If results directory does not exist, create it
if not os.path.exists("results"):
    os.makedirs("results")

# Open file to write results
file = open("results/other_comp.csv", "w")

# Get compression ratios for different compression algorithms
for i in range(1, 10):
    x = i / 10
    fgzip = "example/chry_" + str(x) + ".gz"
    fbzip2 = "example/chry_" + str(x) + ".bz2"
    ftarxz = "example/chry_" + str(x) + ".tar.xz"
    # Get size of compressed files
    size_gzip = get_size(fgzip)
    size_bzip2 = get_size(fbzip2)
    size_tarxz = get_size(ftarxz)
    # Calculate compression ratio
    ratio_gzip = size_gzip / original
    ratio_bzip2 = size_bzip2 / original
    ratio_tarxz = size_tarxz / original
    # Write results to file
    file.write(str(x) + "," + str(ratio_gzip) + "," + str(ratio_bzip2) + "," + str(ratio_tarxz) + "\n")

file.close()
