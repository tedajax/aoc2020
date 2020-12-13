from itertools import count
from sympy.ntheory.modular import crt


with open('input_13.txt') as f:
    ls = [line.strip() for line in f.readlines()]

earliest = int(ls[0])
bus_times = [(int(x), -i) for i, x in enumerate(ls[1].split(',')) if x != 'x']
busses, times = zip(*bus_times)

print(times)


# Part one
print(next((time - earliest)*bus
           for time in count(earliest) for bus in busses
           if time % bus == 0))


# Part two
print(crt(busses, times)[0])
