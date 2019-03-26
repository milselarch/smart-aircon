url = 'http://192.168.1.89/status'

import requests
from requests.adapters import HTTPAdapter
from requests.exceptions import ConnectionError

github_adapter = HTTPAdapter(max_retries=0)

session = requests.Session()

# Use `github_adapter` for all requests to endpoints that start with this URL
session.mount(url, github_adapter)

try:
    resp = requests.get(url=url, params={})
    data = resp.content
    print(f"DATA {data}")
    # Check the JSON Response Content documentation below
except ConnectionError as ce:
    print(ce)
