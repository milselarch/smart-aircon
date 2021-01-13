const { SmartThings } = require("smartthings-node");
const prettyjson = require('prettyjson')

const {SmartThingsClient, BearerTokenAuthenticator} = require('@smartthings/core-sdk')
const { stat } = require('fs')

const token = '6ac2836d-8956-4dbc-bee0-0d3931c77b3d'
const client = new SmartThingsClient(
    new BearerTokenAuthenticator(token)
)

const st = new SmartThings(String(token));
client.devices.list().then(devices => {
    console.log(`Found ${JSON.stringify(devices)} devices`)
})

const device = "34c271dc-71ff-ff60-91ff-49de3dc5f8ee"
client.devices.list([device, 'capabilities']).then(cap => {
    console.log(`\nCAP DESCRIPTION ${JSON.stringify(cap)}`)
})

/*
client.devices.get(device).then((result) => {
    console.log(`DEVICE DESCRIPTION ${JSON.stringify(result)}`)
})

client.devices.list([device, 'capabilities']).then(cap => {
    console.log(`\nCAP DESCRIPTION ${JSON.stringify(cap)}`)
})

client.devices.getStatus(device).then((status) => {
    console.log(`\STATS DESCRIPTION ${JSON.stringify(status)}`)
})
*/

st.devices.getDeviceCapabilityStatus(
    device, "main", token
)

st.devices.listDevicesByCapability('switch').then(deviceList => {
  console.log('LIST CAPS', deviceList);
})

st.devices.executeDeviceCommand(device, [{
    capability: "switch",
    arguments: ['on']
}], token)