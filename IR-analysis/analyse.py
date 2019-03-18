import matplotlib
import matplotlib.pyplot as pyplot
import statistics
import timings
times = timings.times

def extractHighs(timings):
    highTimings = []

    for k in range(len(timings)):
        timing = timings[k]
        
        if k % 2 == 0:
            highTimings.append(timing)

    return highTimings

def extractLows(timings):
    lowTimings = []

    for k in range(len(timings)):
        timing = timings[k]
        
        if k % 2 == 1:
            lowTimings.append(timing)

    return lowTimings

highs = extractHighs(times)[1:]
medianHigh = statistics.median(highs)
lows = extractLows(times)[2:]
CUTOFF = 600
print("MAX LOW", max(lows))
short_lows = [low for low in lows if low < CUTOFF]
long_lows = [low for low in lows if low > CUTOFF]
medianLow = statistics.median(short_lows)
medianLonglow = statistics.median(long_lows)

START_HIGH = times[0]
START_LOW = times[1]
VALUES = []

for k in range(len(times))[2:]:
    if k % 2 == 0:
        VALUES.append(1)
    else:
        timing = times[k]

        if timing > CUTOFF:
            VALUES.append(-1)
        else:
            VALUES.append(0)

pulseStr = ''.join([{0: '0', 1: '1', -1: 'N'}[val] for val in VALUES])
chunks = pulseStr.split('N')
lengths = [len(chunk) for chunk in chunks]
zlengths = [(length - 1) // 2 for length in lengths]
print(zlengths)
splits = ''.join([hex(zlength)[2:] for zlength in zlengths])
print(max(lengths))
#assert max(lengths) < 16

print(f'START HIGH { START_HIGH } START LOW { START_LOW }')
print(f'MEDIAN HIGH { medianHigh } MEDIAN LOW { medianLow } MEDIAN LONG_LOW { medianLonglow }')
print(f'SEQUENCE { VALUES } { len(VALUES) }')
print(f'PULSEMAP { pulseStr }')
print(f'PULSEMAP { chunks }')
print(f"PULSESPT { splits }")

if __name__ == '__main__':
    #plot(*sumTimings(), fill='green')
    print(f'S-LOWS { sorted(lows) }')
    fig, ax = pyplot.subplots()

    # print(f'MEAN {mean} MEDIAN {median} LOW {min(highs)} HIGH {max(highs)}')
    lows = [h / medianHigh for h in extractLows(times)]
    # print(f'TIMES-SCALED { sorted(lows) }')
    # print(f'{lows}')
    ax.plot(highs)
    ax.set_ylim([0, max(highs) * 1.1])
    #ax.axis('off')
    pyplot.show()
