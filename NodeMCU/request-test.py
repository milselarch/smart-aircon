import requests


def start():
    requests.get('http://192.168.1.82')


while True:
    input("Press enter to send >>>")
    start()
