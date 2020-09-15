<template>
	<div class="content">
		<div class="item head">
			<img id="icon" :src="ud.icon"/>
			<div>
				<h1>{{ud.name}}</h1>
				<div class="info">{{ud.info}}</div>
			</div>
		</div>
		<div class="item">
			<h1>个人简介</h1>
			<div class="info" v-html="ud.ir"></div>
		</div>
		<div class="item">
			<h1>个人作品</h1>
			<div id="p-list">
				<div class="p-item" v-on:click="pItemClick(index)" v-for="(item, index) in ud.projects" :key="index">
					<img class="icon" :src="item.icon" />
					<div>
						<div class="title">{{item.title}}</div>
						<div class="info">{{item.info}}</div>
						<div class="info date">{{item.date}}</div>
					</div>
					<img class="more" src="../../assets/icon/more.svg" />
				</div>
			</div>
		</div>
		<div class="item">
			<h1>玩一玩</h1>
			<form class="info">
				<input type="checkbox" v-model="led.led1" v-on:change="ledChange('led1')" />LED1
				<input type="checkbox" v-model="led.led2" v-on:change="ledChange('led2')" />LED2
				<input type="radio" value="wl" v-model="led.ledm" v-on:change="ledChange('wl')" />交替闪烁
				<input type="radio" value="off" v-model="led.ledm" v-on:change="ledChange('off')" />关闭
			</form>
		</div>
		<div class="item footer"></div>
	</div>
</template>

<script src="./js/index.js" />

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped lang="scss">
	@import './scss/index.scss';
</style>
