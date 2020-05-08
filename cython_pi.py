def compute_pi(iterations):
    delta = 1.0 / iterations
    inside = 0.0
    x = 0.0
    while x < 1:
        y = 0.0
        while y < 1:
            if x*x + y*y < 1:
                inside = inside + 1
            y = y + delta
        x = x + delta
    total = iterations * iterations
    return inside / total * 4
