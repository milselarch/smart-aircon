import requests
import json

url = 'http://192.168.1.89/set-status'

params = dict(
    data=json.dumps({'apple': 2})
)

resp = requests.get(url=url, params=params)
data = resp.content
print(f"DATA { data }")
# Check the JSON Response Content documentation below
