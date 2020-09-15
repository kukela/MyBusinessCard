### 1. 介绍
当手机连接上名片Wi-Fi后会打开个人主页，进入/admin页面后可以设置Wi-Fi、升级固件、更新网站、配置设备、文件管理，名片上有两个LED灯可以通过Http协议操作。<br>
PCB去嘉立创打样5块钱5个，ESP8266EX在淘宝4块钱就能买到，名片物料成本在10块钱左右。<br>
外观参考了国外大神 George Hilliard 的设计。<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js1.jpg?x-oss-process=image/resize,m_lfit,w_680)<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js2.jpg?x-oss-process=image/resize,m_lfit,w_226)![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js3.jpg?x-oss-process=image/resize,m_lfit,w_226)![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js4.jpg?x-oss-process=image/resize,m_lfit,w_226)<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/g1.png?x-oss-process=image/resize,m_lfit,w_680)<br>

### 2. 开发工具
IDE用的是Arduino。<br>
原理图和PCB开源地址 -> <a href="https://oshwhub.com/kukela/mybusinesscard" target="_blank">链接</a>（最新版本会在9月20号左右放出）<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/cardx.png?x-oss-process=image/resize,m_lfit,w_680)<br>
阻抗匹配用的是HFSS仿真<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/zk1.png?x-oss-process=image/resize,m_lfit,w_640)<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/zk2.png?x-oss-process=image/resize,m_lfit,w_640)<br>
USB小零件使用SketchUP制作。<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/b.jpg?x-oss-process=image/resize,m_lfit,w_680)<br>
admin页面使用了NPM，Gulp。在命令行输入`gulp`可以把admin页面编译到build/admin.html里面，并且压缩成一行。<br>
升级更新相关文件存放在阿里云oss<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/up1.png?x-oss-process=image/resize,m_lfit,w_680)<br>
打包spiffs工具在tool目录里面<br>

### 3. 准备
1. 建议去淘宝买一个ESP LINK，可以很方便的调试。<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/esplink.png?x-oss-process=image/resize,m_lfit,w_680)<br>

### 4. 注意
1. ESP LINK 的 RXD接RXD，TXD接TXD，GND需要和名片的GND共地。接口不支持热插拔！！
2. PCB厚度1.0mm。
3. ESP8266EX最大支持16MB Flash，可用Web主页空间不到15MB。
4. 个人网站文件名称和路径长度不能超过32字节，建议使用vue-cli或webpack开发。

<br>

****

<br>

你的支持，是我不断贡献的动力！<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/globle/shoukuan.png?x-oss-process=image/resize,m_lfit,w_680)