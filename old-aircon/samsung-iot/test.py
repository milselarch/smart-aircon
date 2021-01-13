import aiohttp
import asyncio
import pysmartthings

from config import token

async def test():
    async with aiohttp.ClientSession() as session:
        api = pysmartthings.SmartThings(session, token)
        locations = await api.locations()
        print(len(locations))

        location = locations[0]
        print(location.name)
        print(location.location_id)

        devices = await api.devices()
        print(len(devices))

        device = devices[0]
        print(device.device_id)
        print(device.name)
        print(device.label)
        print(device.capabilities)

        

if __name__ == '__main__':
    asyncio.run(test())
