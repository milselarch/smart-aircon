import RPi.GPIO as GPIO
import time

OUT_PIN = 20

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(OUT_PIN, GPIO.OUT)

def send():
    print("LED on")
    GPIO.output(OUT_PIN,GPIO.HIGH)
    time.sleep(0.3)
    print("LED off")
    GPIO.output(OUT_PIN,GPIO.LOW)
    time.sleep(0.8)
    print("LED on")
    GPIO.output(OUT_PIN,GPIO.HIGH)
    time.sleep(0.3)
    print("LED off")
    GPIO.output(OUT_PIN,GPIO.LOW)

while True:
    input('enter to send')
    send()
    print('done')