import dis
import sys
import time
import c_pi
import cython_pi

def compute_pi(iterations):
    delta = 1.0 / iterations
    inside = 0.0
    x = 0.0
    while x < 1:
        ## print()
        ## print('x =', x)
        y = 0.0
        while y < 1:
            ## print('  y = ', y)
            if x*x + y*y < 1:
                inside = inside + 1
            y = y + delta
        x = x + delta
    total = iterations * iterations
    return inside / total * 4

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
    run('Cython', cython_pi.compute_pi, N)
    run('C', c_pi.compute_pi, N)

def bench_sum():
    N = 10000000
    run('Python', sum_numbers, N)
    run('C', c_pi.sum_numbers, N)
    run('C', c_pi.sum_numbers2, N)

if __name__ == '__main__':
    #bench_pi()
    bench_sum()

    #dis.dis(compute_pi)
    #import pdb;pdb.set_trace()
    #print(c_pi.sum_numbers2(4))
