import Vue from 'vue'
import VueRouter from 'vue-router'

import Index from '@/components/index/Index.vue'
import ProjectInfo from '@/components/index/ProjectInfo.vue'

Vue.use(VueRouter)

export default new VueRouter({
	mode: 'history',
	routes: [{
		path: '/',
		name: 'Index',
		component: Index
	}, {
		path: '/projectInfo',
		name: 'ProjectInfo',
		component: ProjectInfo
	}]
})
