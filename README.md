### 1. 介绍
#### 1.1. 功能介绍
当手机连接上名片Wi-Fi后会打开个人网站。<br>
进入/admin页面后可以设置Wi-Fi、升级固件、更新网站、配置设备、文件管理。<br>
名片上有两个LED灯可以通过Http协议操作。<br>

#### 1.2. 硬件介绍
原理图和PCB开源地址 -> <a href="https://oshwhub.com/kukela/mybusinesscard" target="_blank">链接</a><br>
USB即插即用。<br>
PCB去嘉立创打样5块钱5个，ESP8266EX在淘宝4块钱就能买到，名片物料成本在10块钱左右。<br>
外观参考了国外大神 George Hilliard 的设计。<br>

#### 1.3. 视频介绍
<a href="https://www.bilibili.com/video/BV18K411P7Zs/" target="_blank">项目介绍</a><br>
<a href="https://www.bilibili.com/video/BV1G54y11733/" target="_blank">功能演示 性能测试 制作过程</a><br>
<a href="https://www.bilibili.com/video/BV1wK4y1Y7S2/" target="_blank">心得体会</a><br>
<a href="https://www.youtube.com/watch?v=RA2ZFGR8FDk" target="_blank">YouTube</a><br>

#### 1.4. 图片展示
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js1.jpg?x-oss-process=image/resize,m_lfit,w_680)<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js2.jpg?x-oss-process=image/resize,m_lfit,w_226)![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js3.jpg?x-oss-process=image/resize,m_lfit,w_226)![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/js4.jpg?x-oss-process=image/resize,m_lfit,w_226)<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/g1.png?x-oss-process=image/resize,m_lfit,w_680)<br>

### 2. 开发工具
1. 嵌入式IDE：Arduino（硬件程序开发）<br>
2. EDA工具：立创EDA（原理图和PCB设计）<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/cardx.png?x-oss-process=image/resize,m_lfit,w_680)<br>
3. 电磁仿真：Ansys HFSS（天线阻抗匹配）<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/zk1.png?x-oss-process=image/resize,m_lfit,w_640)<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/zk2.png?x-oss-process=image/resize,m_lfit,w_640)<br>
4. 3D建模：SketchUP（USB小零件）<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/b.jpg?x-oss-process=image/resize,m_lfit,w_680)<br>
5. Admin页面：使用了HbuilderX开发，NPM，Gulp打包。在命令行输入`gulp`可以把admin页面编译到build/admin.html里面，并且压缩成一行。<br>
6. 文件存储服务：升级更新相关文件存放在阿里云oss<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/up1.png?x-oss-process=image/resize,m_lfit,w_516)<br>
7. 工具：打包spiffs工具在tool目录里面<br>

### 3. 性能测试
#### 3.1. 测试参数表

| 测试序号 | 测试软件版本 |     硬件      | CPU频率 | 晶振  | SPI Flash | Flash频率 |  测试时间  | 测试手机 |
| :-----: | :---------: | :----------: | :-----: | :---: | :-------: | :-------: | :-------: | :-----: |
|   A1    |  2020-9-18  | ESP12-F开发板 |  80MHz  | 26MHz |    4MB    |   40MHz   | 2020-9-18 |  荣耀30  |
|   A2    |  2020-9-18  |  2020-4-11   |  80MHz  | 26MHz |    8MB    |   40MHz   | 2020-9-18 |  荣耀30  |
|   B1    |  2020-9-29  | ESP12-F开发板 | 160MHz  | 26MHz |    4MB    |   80MHz   | 2020-9-29 |  荣耀30  |
|   B2    |  2020-9-29  |   ESP-01S    | 160MHz  | 26MHz |    2MB    |   80MHz   | 2020-9-29 |  荣耀30  |
|   B3    |  2020-9-29  |  2020-4-11   | 160MHz  | 26MHz |    2MB    |   80MHz   | 2020-9-29 |  荣耀30  |
|   B4    |  2020-9-29  |  2020-4-11   | 160MHz  | 26MHz |   16MB    |   80MHz   | 2020-9-29 |  荣耀30  |

#### 3.2. 测试速度表

| 测试序号 |         近程(ms)         | 近程平均(ms) |         远程(ms)          | 远程平均(ms) |
| :-----: | :----------------------: | :---------: | :----------------------: | :---------: |
|   A1    | 1446,1206,1320,1254,990  |   1243.2    | 2261,1631,1656,2063,1785 |   1879.2    |
|   A2    | 1549,1434,1171,1368,1193 |    1343     | 2827,1633,1703,1507,1479 |   1829.8    |
|   B1    | 1550,1750,1669,1033,1249 |   1450.2    | 8889,7086,6086,1703,3103 |   5373.4    |
|   B2    | 1776,1816,1727,1093,1199 |   1522.2    | 1871,1758,1690,1352,1330 |   1600.2    |
|   B3    | 1685,1547,1600,1131,1023 |   1397.2    | 2466,2102,2114,1372,1398 |   1890.4    |
|   B4    | 1714,1587,1880,1455,1578 |   1642.8    | 2097,2015,2018,1625,1620 |    1875     |

#### 3.3. 说明
一般情况下，SPI Flash越小，频率越快，测试速度就越快，当然也可能越不稳定。<br>
由于测试受到手机服务请求、天线射频方向、环境等因素影响，所以以上数据仅供参考。<br>
测试都是在关闭缓存下完成。<br>

##### 3.3.1. A开头的测试
测的是加载到js代码后，网页全部加载完毕的速度。<br>
第1次测的是第一次连接的速度，其余4次测的是在连接情况下，再打开的速度。<br>
尽量保持角度、距离、高度相同进行测试。<br>
近程指距离20厘米之内，远程指距离4米左右，并且隔两堵墙。<br>

##### 3.3.2. B开头
测的是图片加载完毕速度。<br>
前3次测的是第一次连接的速度，其余2次测的是浏览器加载完毕刷新的速度。<br>
从芯片到天线的方向为正，正对着手机并且高度误差在10cm内进行测试。<br>
近程指距离30厘米之内，远程指距离4米左右，并且隔两堵墙。<br>

### 4. 准备
1. 建议去淘宝买一个ESP LINK，可以很方便的调试。<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/MyBusinessCard/esplink.png?x-oss-process=image/resize,m_lfit,w_680)<br>

### 5. 注意
1. ESP LINK 的 RXD接RXD，TXD接TXD，GND需要和名片的GND共地。接口不支持热插拔！！
2. 烧录前保证线路连接正常，然后断掉总电源在接上。如果Arduino报错，建议关掉再打开，或者重启电脑。
3. PCB厚度1.0mm。
4. ESP8266EX最大支持16MB Flash，可用Web主页空间不到15MB。
5. 个人网站文件名称包括路径长度不能超过32字节，建议使用vue-cli或webpack开发。

<br>

****

<br>

你的支持，是我不断贡献的动力！<br>
![](https://kukela-images.oss-cn-shanghai.aliyuncs.com/globle/shoukuan.png?x-oss-process=image/resize,m_lfit,w_680)