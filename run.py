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


def run(name, fn, iterations):
    a = time.time()
    res = fn(iterations)
    b = time.time()
    t = b - a
    print('%-10s %s = %.6f    t = %.2f secs' % (name, fn.__name__, res, t))

N = 2000
def main():
    run('Python', compute_pi, N)
    run('Cython', cython_pi.compute_pi, N)
    run('C', c_pi.compute_pi, N)

if __name__ == '__main__':
    main()
    ## print('Python')
    ## print(compute_pi(2))
    ## print()
    ## print('C')
    ## print(c_pi.compute_pi(2))
