import dis
import sys
import time
import c_pi
import cython_pi
from dj import DJ

def compute_pi(iterations):
    delta = 1.0 / iterations
    inside = 0.0
    x = 0.0
    while x < 1.0:
        ## print()
        ## print('x =', x)
        y = 0.0
        while y < 1.0:
            ## print('  y = ', y)
            if x*x + y*y < 1.0:
                inside = inside + 1.0
            y = y + delta
        x = x + delta
    total = iterations * iterations
    return inside / total * 4.0

def sum_numbers(n):
    tot = 0
    i = 0
    while i < n:
        tot += i
        i += 1
    return tot


def run(name, fn, iterations):
    a = time.time()
    res = fn(iterations)
    b = time.time()
    t = b - a
    print('%-10s %s = %.6f    t = %.2f secs' % (name, fn.__name__, res, t))

def bench_pi():
    N = 2000
    run('Python', compute_pi, N)
    #run('Cython', cython_pi.compute_pi, N)
    run('C', c_pi.compute_pi, N)
    run('C', c_pi.compute_pi2, N)

def bench_sum():
    N = 10000000
    run('Python', sum_numbers, N)
    run('C', c_pi.sum_numbers, N)
    run('C', c_pi.sum_numbers2, N)

if __name__ == '__main__':
    bench_pi()
    #bench_sum()

    #DJ.compile(sum_numbers)
    #DJ.compile(compute_pi)
