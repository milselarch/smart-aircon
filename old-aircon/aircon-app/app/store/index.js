import Vuex from 'vuex';
import * as ApplicationSettings from "application-settings";
import localStorage from "nativescript-localstorage";

const NSVuexPersistent = store => {
    // Init hook.
    let storageStr = localStorage.getItem('ns-vuex-persistent');
    console.log("STORE STRING", storageStr)

    if (storageStr) {
        store.replaceState(JSON.parse(storageStr))
    }
    
    store.subscribe((mutation, state) => {
        // Suscribe hook.
        localStorage.setItem('ns-vuex-persistent', JSON.stringify(state));
        console.log("SET STORE STRING", JSON.stringify(state))
    })
};

const storeSettings = {
    state: {
        targetTemp: 25,
        fanspeed: 2,
        maxTemp: 28,
        address: '192.168.1.89'
    },

    mutations: {
        load (state) {
            if (ApplicationSettings.getString("store")) {
                this.replaceState(
                    Object.assign(state, JSON.parse(ApplicationSettings.getString("store")))
                );
            }
        },

        setTargetTemp(state, targetTemp) {
            state.targetTemp = targetTemp
        },
        setFanspeed(state, fanspeed) {
            state.fanspeed = fanspeed
        },
        setMaxTemp(state, maxTemp) {
            state.maxTemp = maxTemp
        },
        setAddress(state, address) {
            state.address = address
        }
    },

    getters: {
        targetTemp: state => state.targetTemp,
        fanspeed: state => state.fanspeed,
        maxTemp: state => state.maxTemp,
        address: state => state.address
    },

    plugins: [NSVuexPersistent]
}

function bindVueToVuex(Vue) {
    Vue.use(Vuex)
    const store = new Vuex.Store(storeSettings);
    Vue.prototype.$store = store;
    return store
}

export default bindVueToVuex;