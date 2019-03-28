<template>
    <Page>
        <ActionBar title="Smart Aircon"/>

        <GridLayout
          columns="*"
          rows="120, 25, 25, 25, 25, 25, 25, 80, 50, 50"
          width="300"
          height="600"
          backgroundColor="white"
        >
          <GridLayout
            id="button_layout"
            columns="100, 100, 100"
            rows="50" width="300"
            height="600"
            row="0" col="0"
            backgroundColor="white"
          >
            <Button 
              id="on_button" text="ON" @tap="turnOn" 
              col="0" row="0" v-bind:class="{
                active: currentState === ON_STATE
              }"
            />
            <Button 
              id="auto_button" text="AUTO" @tap="turnAuto"
              col="1" row="0" v-bind:class="{
                active: currentState === AUTO_STATE
              }"
            />
            <Button 
              id="off_button" text="OFF" @tap="turnOff"
              col="2" row="0" v-bind:class="{
                active: currentState === OFF_STATE
              }"
            />
          </GridLayout>
          
          <Label col="0" row="1">
            <FormattedString>
              <Span text="Target temperature: " />
              <Span :text="targetTemp" />
            </FormattedString>
          </Label>
          <Slider 
            v-model="targetTemp" minValue=16 maxValue=31
            col="0" row="2" @valueChange="changeTemp"
          />
          
          <Label col="0" row="3" >
            <FormattedString>
              <Span text="Fan speed: " />
              <Span :text="fanspeed" />
            </FormattedString>
          </Label>
          <Slider 
            v-model="fanspeed" minValue=0 maxValue=3
            col="0" row="4" @valueChange="changefanspeed"
          />
          
          <Label col="0" row="5" >
            <FormattedString>
              <Span text="Max temperature: " />
              <Span :text="maxTemp" />
            </FormattedString>
          </Label>
          <Slider 
            v-model="maxTemp" minValue=0 maxValue=30
            col="0" row="6" @valueChange="changeMaxTemp"
          />
        
          <GridLayout
            id="status_layout"
            columns="300"
            rows="50" width="300"
            height="600"
            row="7" col="0"
            backgroundColor="white"
          >
            <TextField 
              v-model="address" col="0" row="0" 
              @returnPress="changeAddress"
            />
          </GridLayout>

          <Label :text="status" col="0" row="8" textWrap=true />
          <ActivityIndicator row="9" :busy="isPolling" width="100" height="100" class="activity-indicator">
          </ActivityIndicator>

        </GridLayout>
    </Page>
</template>

<script>
  const timerModule = require("tns-core-modules/timer");
  const axios = require('axios');
  const http = require('http')

  const OFF_STATE = 0;
  const ON_STATE = 1;
  const AUTO_STATE = 2;

  function sleepAsync (ms) {
    return new Promise(
      resolve => timerModule.setTimeout(resolve, ms)
    )
  }

  export default {
    computed: {
      status: function () {
        if (this.connected === true) { 
          return this.currentTemp + "℃"
        } else {
          return "Disconnected"
        }
      }
    },

    data() {
      return {
        ON_STATE: ON_STATE,
        OFF_STATE: OFF_STATE,
        AUTO_STATE: AUTO_STATE,

        response: null,
        destroyed: false,
        connected: false,
        isPolling: false,

        currentState: null,
        currentTemp: null,
        currentTargetTemp: null,
        currentFanspeed: null,
        currentMaxtemp: null,

        address: '192.168.1.89',
        targetTemp: 25,
        maxTemp: 28,
        fanspeed: 1
      }
    },

    methods: {
      async fetchStatus() {
        const self = this;
        let response;
        
        try {
          self.isPolling = true
          response = await axios.get(`http://${self.address}/status`)
          // console.log(`RESPONSE ${JSON.stringify(response)}`)
        } catch (error) {
          // console.log(`AXIOS ERROR ${error}`)
          self.connected = false;
          return false
        } finally {
          self.isPolling = false
        }
      
        const data = response.data
        if (data === "") {
          self.connected = false;
          return false
        }

        self.currentState = data.state
        self.currentTemp = data.temp
        self.currentFanspeed = data.fanspeed
        self.currentTargetTemp = data.targetTemp
        self.currentMaxtemp = data.maxTemp
        self.connected = true;
        // self.response = JSON.stringify(response)
        return true
      },

      async changeAirconSettings(state) {
        while (this.isPolling === true) {
          await sleepAsync(100);
        }

        const self = this;
        const endPoint = `http://${self.address}/set-status`
        this.isPolling = true
        let response;

        try {
          response = await axios.post(endPoint, {
            state: state,
            temp: self.targetTemp,
            fanspeed: self.fanspeed,
            maxTemp: self.maxTemp,
            force: true
          })

          console.log("POSTED")
          self.currentState = state
          return true

        } catch (error) {
          console.log(`SEND STATE ERR ${error}`)
          return false

        } finally {
          self.isPolling = false
        }
      },

      changeTemp () {
        this.$store.commit("setTargetTemp", this.targetTemp)
        console.log("SETTER")
      },
      changefanspeed () {
        this.$store.commit("setFanspeed", this.fanspeed)
        console.log("SETTER")
      },
      changeMaxTemp () {
        this.$store.commit("setMaxTemp", this.maxTemp)
        console.log("SETTER")
      },
      changeAddress () {
        this.$store.commit("setAddress", this.address)
        console.log("SETTER")
      },

      async turnOn () {
        await this.changeAirconSettings(ON_STATE)
      },
      async turnOff () {
        await this.changeAirconSettings(OFF_STATE)
      },
      async turnAuto () {
        await this.changeAirconSettings(AUTO_STATE)
      }
    },

    created () {
      this.targetTemp = this.$store.getters.targetTemp
      this.fanspeed = this.$store.getters.fanspeed
      this.maxTemp = this.$store.getters.maxTemp
      this.address = this.$store.getters.address
      /*
      this.fanspeed = this.$store.getters['fanspeed']
      this.maxTemp = this.$store.getters['maxTemp']
      this.address = this.$store.getters['address']
      */
    },

    mounted () {
      const self = this
      console.log("MOI")

      this.$store.commit("load")
      timerModule.setTimeout(async () => {
        // fetech aircon status every second
        while (!self.destroyed) {
          await self.fetchStatus()
          await sleepAsync(5000);
        }
      }, 0)
    },
    
    beforeDestroy () {
      this.destroyed = true
    }
  }
</script>

<style scoped lang="scss">
  GridLayout #status_layout {
    padding-top: 20;
  }

  GridLayout #button_layout {
    padding-top: 20;
  }

  Label, TextField {
    text-align: center;
  }

  ActionBar {
    background-color: #53ba82;
    color: #ffffff;
  }

  Button {
    font-weight: 200;

    &#on_button {
      background-color: #1bbe36;
      color: rgb(241, 239, 239);
    }
    &#off_button {
      background-color: #c03927;
      color: rgb(241, 239, 239);
    }
    &#auto_button {
      background-color: #ac9514;
      color: rgb(241, 239, 239);
    }

    &.active {
      font-weight: normal;
    }
  }

  GridLayout#button_layout {
    margin-top: 100px;
  }
  Button#set_button {
    background-color: #4f10b4;
    color: rgb(241, 239, 239);
  }

  .message {
    vertical-align: center;
    text-align: center;
    font-size: 20;
    color: #333333;
  }
</style>
