import Vue from 'vue'
import App from './App.vue'
import VueResource from 'vue-resource'
import router from './router/router.js'
import {
	Swiper as SwiperClass,
	Pagination,
	Navigation
} from 'swiper/core'
import getAwesomeSwiper from 'vue-awesome-swiper/dist/exporter'
import 'swiper/swiper-bundle.css'

// 自定义组件
import Utils from '@/components/global/js/utils.js'

Vue.use(VueResource)
SwiperClass.use([Pagination, Navigation])
Vue.use(getAwesomeSwiper(SwiperClass))

Vue.config.productionTip = false

// 自定义组件
Vue.prototype.$Utils = Utils

new Vue({
	el: '#app',
	router,
	render: h => h(App),
	comments: {}
})
