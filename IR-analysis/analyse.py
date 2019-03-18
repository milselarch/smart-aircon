import matplotlib
import matplotlib.pyplot as pyplot
import statistics
import extract
import timings

times = timings.times
extractor = extract.Extractor()

highs = extractor.extractHighs(times)[1:]
medianHigh = statistics.median(highs)
lows = extractor.extractLows(times)[2:]
CUTOFF = 600
short_lows = [low for low in lows if low < CUTOFF]
long_lows = [low for low in lows if low > CUTOFF]
medianLow = statistics.median(short_lows)
medianLonglow = statistics.median(long_lows)

startHigh, startLow, splits, length = extractor.extractPulses(times)
#print(f'START HIGH { START_HIGH } START LOW { START_LOW }')
#print(f'MEDIAN HIGH { medianHigh } MEDIAN LOW { medianLow } MEDIAN LONG_LOW { medianLonglow }')
#print(f'SEQUENCE { VALUES } { len(VALUES) }')
#print(f'PULSEMAP { pulseStr }')
#print(f'PULSEMAP { chunks }')
print(f"PULSESPT { splits }")

if __name__ == '__main__':
    #plot(*sumTimings(), fill='green')
    print(f'S-LOWS { sorted(lows) }')
    fig, ax = pyplot.subplots()

    # print(f'MEAN {mean} MEDIAN {median} LOW {min(highs)} HIGH {max(highs)}')
    lows = [h / medianHigh for h in extractor.extractLows(times)]
    # print(f'TIMES-SCALED { sorted(lows) }')
    # print(f'{lows}')
    ax.plot(highs)
    ax.set_ylim([0, max(highs) * 1.1])
    #ax.axis('off')
    pyplot.show()
