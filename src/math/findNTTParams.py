from math import sqrt
def isPrime(n):
    if n <= 1 or pow(2, n, n) != 2:
        return False
    for i in range(2, int(sqrt(n))):
        if n%i == 0:
            return False
    return True

def finda(base, a):
    while True:
        if isPrime(a*(2**base) + 1):
            return a
        a += 1

def checkUnityRoot(w, p):
    x = w
    for i in range(p-2):
        if x == 1:
            return False
        x = (x*w)%p
    return True

def findUnityRoot(p):
    for i in range(1, p):
        if checkUnityRoot(i, p):
            return i

base = int(input())
a = 1
while True:
    a = finda(base, a)
    p = a * (2**base) + 1
    r = findUnityRoot(p)
    print("a =", a, ", root =", pow(r, a, p))
    a += 1
