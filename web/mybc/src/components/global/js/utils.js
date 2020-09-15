export default {
	contentJson: require('../../../assets/res/content.json')
	// getJson(url, bc) {
	// 	var request = new XMLHttpRequest;
	// 	request.open("get", url);
	// 	request.send(null);
	// 	request.onload = function() {
	// 		if (200 == request.status) {
	// 			console.log(request.responseText);
	// 			var json = JSON.parse(request.responseText);
	// 			if (null == json) return;
	// 			bc(json);
	// 		}
	// 	}
	// },
}
