from picamera import PiCamera
from time import sleep

camera = PiCamera()

camera.start_preview()
input("waiting")
camera.stop_preview()