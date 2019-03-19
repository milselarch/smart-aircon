import RPi.GPIO as GPIO
import time

timings = [10305693, 35579, 15997, 5254, 11596, 5247, 11575, 5266, 3097, 5273, 3123, 5259, 3123, 5261, 11799, 5030, 3116, 5295, 3359, 5035, 11579, 5218, 11801, 5025, 3368, 5028, 11827, 4987, 3359, 5023, 3373, 5035, 11827, 5006, 11808, 4987, 3390, 4987, 11825, 4992, 11811, 5040, 3385, 4994, 3399, 4997, 11820, 5025, 3385, 4975, 3395, 5016, 11799, 5030, 3387, 4973, 3378, 5009, 3404, 4994, 3387, 5030, 3356, 5018, 3366, 5042, 3342, 5056, 3306, 5042, 3407, 4971, 3392, 5037, 3392, 4997, 3356, 4770, 3659, 4920, 3435, 5037, 3361, 4754, 3659, 4987, 3414, 4990, 11787, 5028, 3364, 5002, 3392, 4928, 11935, 5004, 3366, 5018, 11835, 5002, 11765, 5049, 11782, 4994, 3411, 5013, 3373, 5013, 3390, 4751, 3612, 4785, 3604, 4796, 3626, 5004, 3399, 4968, 11837, 4999, 11835, 5013, 3340, 4754, 3652, 5021, 3376, 4837, 3557, 5006, 3364, 4959, 11863, 5040, 11796, 4947, 3449, 4830, 3550, 4765, 3659, 4987, 3378, 4777, 3631, 4985, 3399, 4742, 3650, 4732, 3669, 4975, 3390, 4749, 3662, 4744, 3633, 4997, 3402, 4978, 3404,4730, 3688, 4720, 3664, 4763, 3614, 4739, 3659, 4746, 3626, 4746, 3678, 4711, 3662, 4770, 3600, 4799, 3612, 4744, 3621, 4799, 3602, 4751, 3657, 4746, 3633, 4768, 3614, 4756, 12075, 4782, 3621, 4751, 3623, 4754, 3666, 4749, 3604, 4796, 3607, 4742, 3662, 4751, 3628, 4765, 3631, 4746, 3626, 4775, 3657, 4715, 12075, 4773, 3650, 4706, 12078, 4758, 3662, 4713, 3669, 4732, 12111, 4742, 12066, 4725, 12097, 4732]

stimings = []
total = 0
for timing in timings:
    total += timing
    stimings.append(total / 10e6)

value = 1
OUT_PIN = 20

GPIO.setmode(GPIO.BCM)
GPIO.setup(OUT_PIN, GPIO.OUT)

def send():
    start = time.time()
    now = start
    for index in range(len(stimings)):
        while now - start < stimings[index]:
            GPIO.output(OUT_PIN, (1 + index) % 2)
            now = time.time()
        

while True:
    input('enter to send')
    send()
    print('done')