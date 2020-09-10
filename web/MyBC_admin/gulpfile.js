const gulp = require('gulp'),
	t2 = require('through2'),
	$ = require('gulp-load-plugins')();


gulp.task('default', function() {
	return build("index");
});

gulp.task('update', function() {
	return build("update");
});

function build(dir) {
	return gulp.src('./' + dir + '/*.html')
	.pipe($.usemin({
		html: [$.htmlMinifier({
			collapseWhitespace: true,
			quoteCharacter: "'",
		})],
		inlinejs: [$.uglify()],
		inlinecss: [$.cleanCss(), 'concat']
	}))
	.pipe($.replace('"', '\\"'))
	.pipe(gulp.dest('build/'));
}

//在管道中 打印
function t2Log(file, enc, cb) {
	//t2.obj(t2Log)
	console.log(file.path);
	this.push(file); // 似乎需要push一下，不然后续的pipe不会处理这个文件
	return cb();
};