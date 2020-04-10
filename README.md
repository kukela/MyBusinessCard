### 1. 介绍
当手机连接上名片Wi-Fi后会打开个人主页，进入文件管理系统后可以修改个人主页，名片上面有两个LED灯可以在Web页面操作。<br>
PCB去嘉立创打样5块钱5个，ESP8266EX在淘宝4块钱就能买到，名片物料成本在10块钱左右。<br>
外观参考了国外大神 George Hilliard 的设计。<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js1.jpg?x-oss-process=image/resize,m_lfit,w_680)<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js2.jpg?x-oss-process=image/resize,m_lfit,w_340)![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js3.jpg?x-oss-process=image/resize,m_lfit,w_340)

### 2. 开发工具
IDE用的是Arduino。<br>
原理图和PCB开源地址 -> <a href="https://oshwhub.com/kukela/mybusinesscard" target="_blank">链接</a><br>
USB小零件使用SketchUP制作。<br>
Web使用了NPM，Gulp。在命令行输入Gulp可以把admin页面编译到build/admin.html里面，并且压缩成一行。<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/cardx.png?x-oss-process=image/resize,m_lfit,w_680)<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/a.jpg?x-oss-process=image/resize,m_lfit,w_680)

### 3. 准备
1. 建议去淘宝买一个ESP LINK，可以很方便的调试。<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/esplink.png?x-oss-process=image/resize,m_lfit,w_680)

### 4. 注意
1. ESP LINK 的 RXD接RXD，TXD接TXD，GND需要和名片的GND共地。接口不支持热插拔！！
2. PCB厚度1.2mm。

<br>

****

<br>

你的支持，是我不断贡献的动力！<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/globle/shoukuan.png?x-oss-process=image/resize,m_lfit,w_680)