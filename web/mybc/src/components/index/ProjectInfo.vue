<template>
	<div class="content">
		<swiper class="swiper" :options="swiperOption">
			<swiper-slide v-for="item in pitem.img" :key="item"><img :src="item" /></swiper-slide>
			<div class="swiper-pagination" slot="pagination"></div>
			<div class="swiper-button-next swiper-button-white" slot="button-next"></div>
			<div class="swiper-button-prev swiper-button-white" slot="button-prev"></div>
		</swiper>

		<div class="item">
			<h1>{{ pitem.title }}</h1>
			<div class="info">{{ pitem.info }}</div>
			<div class="info">{{ pitem.date }}</div>
		</div>
		<div class="item">
			<button class="btn fw" v-on:click="back">返 回</button>
		</div>
		<div class="info footer"></div>
	</div>
</template>

<script src="./js/projectInfo.js" />

<style scoped lang="scss">
@import './scss/projectInfo.scss';
</style>
