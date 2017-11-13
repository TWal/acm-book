from sys import argv

def readFile(filename):
    with open(filename, 'r') as f:
        return f.read()

with open(argv[1], 'r') as f:
    content = f.readlines()

output = ""

for line in content:
    if line[0:10] == "#include <" and line[-2:] == ">\n":
        output += readFile("../src/" + line[10:-2])
    else:
        output += line


with open(argv[2], 'w') as f:
    f.write(output)
