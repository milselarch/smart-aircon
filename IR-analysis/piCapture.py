import RPi.GPIO as GPIO
import extract
import time

extractor = extract.Extractor()
INPUT_WIRE = 16

GPIO.setmode(GPIO.BCM)
GPIO.setup(INPUT_WIRE, GPIO.IN)
inc = 0

def capture(output=True, inverted=True):
    start = None
    recordings = []
    prevTime = 0
    prevValue = GPIO.input(INPUT_WIRE)
    #print("PREVVALUE", prevValue)
    now = -1
    inc = 0

    while (now - prevTime < 1) or len(recordings) == 0:
        now = time.time()

        if start is None:
            prevTime = now
            start = now

        value = GPIO.input(INPUT_WIRE)

        if value != prevValue:
            recordings.append([prevValue, now - prevTime, inc])
            prevValue = value
            prevTime = now
            inc += 1

        start = now

        if len(recordings) == 0:
            time.sleep(0.001)
    
    if inverted:
        values = [1 - recording[0] for recording in recordings]
    else:
        values = recordings

    timings = [int(recording[1] * 1e6) for recording in recordings][1:]
    values = values[1:]

    if output:
        for recording in recordings:
            print(recording)

    return values, timings, len(timings)

def getPulses():
    values, timings, length = capture(False)
    startHigh, startLow, pulses, l = extractor.extractPulses(timings)
    return startHigh, startLow, pulses, length

while True:
    inVal = input("press enter to start capture ")
    print('capturing...')
    startHigh, startLow, pulses, length = getPulses()
    print(inVal)
    print(length, pulses)

    #print(f'LENGTH FAIL {length}')
