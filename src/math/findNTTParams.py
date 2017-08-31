def checkMillerRabin(n, a):
    r = n-1
    while True:
        x = pow(a, r, n)
        if x == n-1:
            return True
        if x != 1:
            return False
        if r%2 != 0:
            return True
        r = r//2

def isPrime(n, l = [2,3,5,7,11,13,17,19,23,29,31]):
    return n >= 2 and (n in l or all(checkMillerRabin(n, a) for a in l))

def finda(base, a):
    while True:
        if isPrime(a*(2**base) + 1):
            return a
        a += 1

def factorize(n):
    res = []
    x = 2
    while n != 1:
        if n%x == 0:
            res.append(x)
            while n%x == 0:
                n //= x
        x += 1
    return res

def findUnityRoot(p):
    fact = factorize(p-1)
    for w in range(1, p):
        if all(pow(w, (p-1)//f, p) != 1 for f in fact):
            return w

base = int(input())
a = 1
while True:
    a = finda(base, a)
    p = a * (2**base) + 1
    if p > 2**64:
        break
    r = findUnityRoot(p)
    print("a =", a, ", root =", pow(r, a, p))
    a += 1
