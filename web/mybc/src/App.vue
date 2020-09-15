<template>
	<div id="app">
		<router-view></router-view>
	</div>
</template>

<script>
	export default {
		name: 'App'
	}
</script>

<style lang="scss">
	@import './components/base/scss/base.scss';

	#app {
		font-family: Avenir, Helvetica, Arial, sans-serif;
		-webkit-font-smoothing: antialiased;
		-moz-osx-font-smoothing: grayscale;
	}

	html,
	body {
		width: 100%;
		margin: 0;
		background: $page-bg;
	}

	.mui-content {
		background: $page-bg;
	}

	input {
		-webkit-user-select: auto;
	}
</style>
