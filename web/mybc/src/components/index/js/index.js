export default {
	name: 'index',
	props: {},
	data() {
		return {
			ud: {},
			led: {
				led1: false,
				led2: false,
				ledm: 'off'
			}
		}
	},
	created: function() {
		this.showInfo(this.$Utils.contentJson);
	},
	methods: {
		showInfo(json) {
			this.ud = json;
		},
		pItemClick(index) {
			this.$router.push({
				name: 'ProjectInfo',
				params: {
					index: index
				}
			})
			// console.log(this.ud.projects[index].title);
		},
		ledChange(tag) {
			var led = this.led;
			switch (tag) {
				case "led1":
				case "led2":
					led.ledm = "";
					break;
				case "wl":
					led.led1 = true;
					led.led2 = true;
					break;
				case "off":
					led.led1 = false;
					led.led2 = false;
					break;
			}
			if (led.led1 == false && led.led2 == false) {
				led.ledm = "off";
			}
			var type = [];
			type[0] = led.led1 == true ? 1 : 0;
			type[1] = led.led2 == true ? 1 : 0;
			type[2] = led.ledm == "wl" ? 1 : 0;
			var url = '/led?type=' + type.join("");
			this.$http.get(url).then({});
		}
	}
}
