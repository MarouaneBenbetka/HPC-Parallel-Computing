import random
from concurrent.futures import ThreadPoolExecutor


def sample(n):
    n_inside_circle = 0
    for i in range(n):
        x = random.random()
        y = random.random()
        if x**2 + y**2 < 1.0:
            n_inside_circle += 1
    pi = 4.0 * (n_inside_circle / n)
    return pi


with ThreadPoolExecutor(4) as executor:
    results = executor.map(sample, [10**3, 10**4, 10**5, 10**6], chunksize=1)
    for result in results:
        print(result)
