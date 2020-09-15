export default {
	name: 'projectInfo',
	props: {},
	data() {
		return {
			pitem: {},
			swiperOption: {
				slidesPerView: 1,
				spaceBetween: 0,
				loop: true,
				pagination: {
					el: '.swiper-pagination',
					clickable: true
				},
				navigation: {
					nextEl: '.swiper-button-next',
					prevEl: '.swiper-button-prev'
				}
			}
		}
	},
	created: function() {
		var json = this.$Utils.contentJson;
		this.showInfo(json.projects);
	},
	methods: {
		showInfo(projects) {
			this.pitem = projects[this.$route.params.index];
		},
		back() {
			this.$router.go(-1)
		}
	},

}
