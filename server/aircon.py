import requests

BASE_URL = 'https://api.smartthings.com/v1/devices'


class Aircon(object):
    def __init__(self, device, token):
        self.device = device
        self.token = token

    def get(self, *args, **kwargs):
        headers = {'Authorization': f'Bearer {self.token}'}
        return requests.get(*args, headers=headers, **kwargs)

    def post(self, *args, **kwargs):
        headers = {'Authorization': f'Bearer {self.token}'}
        return requests.post(*args, headers=headers, **kwargs)

    def get_status(self):
        response = self.get(
            f'{BASE_URL}/{self.device}/components/main/status'
        )

        json_response = response.json()
        return json_response




