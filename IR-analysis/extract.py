import matplotlib
import matplotlib.pyplot as pyplot
import statistics

class Extractor(object):
    def extractHighs(self, timings):
        highTimings = []

        for k in range(len(timings)):
            timing = timings[k]
            
            if k % 2 == 0:
                highTimings.append(timing)

        return highTimings

    def extractLows(self, timings):
        lowTimings = []

        for k in range(len(timings)):
            timing = timings[k]
            
            if k % 2 == 1:
                lowTimings.append(timing)

        return lowTimings
    
        
    def extractPulses(self, times, cutoff=600):
        startHigh = times[0]
        startLow = times[1]
        pulses = []

        for k in range(len(times))[2:]:
            if k % 2 == 0:
                pulses.append(1)
            else:
                timing = times[k]

                if timing > cutoff:
                    pulses.append(-1)
                else:
                    pulses.append(0)

        pulseStr = ''.join([{0: '0', 1: '1', -1: 'N'}[val] for val in pulses])
        chunks = pulseStr.split('N')
        lengths = [len(chunk) for chunk in chunks]
        zlengths = [(length - 1) // 2 for length in lengths]
        splits = ''.join([hex(zlength)[2:] for zlength in zlengths])
        return startHigh, startLow, splits, len(times)
