import matplotlib
import matplotlib.pyplot as pyplot
import timings
times = timings.times

plot = pyplot.plot

def getValues():
    values = []
    signal = 1

    for t in times:
        values.append(signal)
        signal = 1 - signal
    
    return values

def sumTimings(timings):
    newTimings = []
    values = []
    signal = 0
    total = 0

    for timing in timings:
        newTimings.append(total)
        newTimings.append(total)
        total += timing

        values.append(0.00 + signal)
        values.append(1.00 - signal)
        signal = 1 - signal
    
    return newTimings, values

#plot(*sumTimings(), fill='green')
fig, ax = pyplot.subplots()
ax.fill_between(*sumTimings(timings.times), zorder=0)
ax.axis('off')
pyplot.show()