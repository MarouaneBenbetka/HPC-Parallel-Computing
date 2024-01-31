from concurrent.futures import ThreadPoolExecutor


def square(x):
    return (x*x)


with ThreadPoolExecutor(3) as executor:
    results = executor.map(square, [1, 2, 3, 4, 5, 6], chunksize=2)
    for result in results:
        print(result)

print("Exiting Main Thread")
