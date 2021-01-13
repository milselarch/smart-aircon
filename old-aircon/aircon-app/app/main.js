import Vue from 'nativescript-vue'
import App from './components/App'
import VueDevtools from 'nativescript-vue-devtools'
import bindVueToVuex from './store'

if (TNS_ENV !== 'production') {
  Vue.use(VueDevtools)
}
// Prints Vue logs when --env.production is *NOT* set while building
Vue.config.silent = (TNS_ENV === 'production')
const store = bindVueToVuex(Vue)

new Vue({
  store,
  render: h => h('frame', [h(App)])
}).$start()
