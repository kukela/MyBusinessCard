// 网络相关
static String MSSID = "aMyCard";
static String MPSK = "";

// 网页相关
static const char TEXT_PLAIN[] PROGMEM = "text/plain;charset=UTF-8";
static const char APP_JSON[] PROGMEM = "application/json";
static const char adminHTML[] PROGMEM = "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1,minimum-scale=1,maximum-scale=1,user-scalable=no'><title>admin</title><style>body{padding:0;max-width:800px;margin:0 auto;color:#303133}.jj{margin:0 20px}.t1{position:relative;height:60px;margin-top:20px}.t1,.t2{display:flex;align-items:center}.t1:first-of-type{margin-top:0}.t1 h1{margin:0;flex:1;color:#303133}.t2{min-height:30px;margin-top:10px}.t2.c{min-height:50px}.t2>.t{font-size:16px;line-height:24px;color:#303133;flex:1}.t2>input{width:44px}.dc{height:44px;line-height:44px;border-radius:4px;padding:0 12px;color:#303133;font-size:16px;background:#ebeef5;border:1px solid #e4e7ed}.dc.pt1{margin-top:20px}.rbtn{margin-left:10px;min-width:78px}.dc:hover:not([disabled]){background:#e4e7ed;border-color:#ccc;text-decoration:none}button[disabled]{color:#909399}.rl{overflow:hidden;text-decoration:none;cursor:pointer;display:block}input[type=file]{position:absolute;width:0;opacity:0;display:inline-block}.item{position:relative;display:flex;min-height:30px;margin:10px 20px 0 20px}.item.v{flex-direction:column}.item>div{font-size:14px;color:#303133;line-height:30px}.item>input,.item>select{flex:1}.item .info{font-size:14px;color:#606266;line-height:22px}table{width:100%;border-collapse:collapse}#state,td{font-size:14px;color:#303133;line-height:28px}thead td{font-weight:700}tr td:first-letter{padding-left:20px}tr td:last-child{padding-left:20px}table .drak{background-color:#ebeef5}#state{margin:20px;display:none}</style></head><body><div class='t1 jj'><h1>Wi-Fi 设置</h1></div><div class='t2 jj'><div class='t'>Wi-Fi 名称</div></div><div class='item'><select id='ssid' class='dc' onchange='WCBtnReset()'></select><button id='scan' class='dc rbtn' onclick='reqScanSsid()'>刷 新</button></div><div class='t2 jj'><div class='t'>Wi-Fi 密码</div></div><div class='item'><input id='pwd' class='dc' type='password' placeholder='请输入 Wi-Fi 密码' value='' onchange='WCBtnReset()'></div><div class='item v'><div class='info'>当设备在同一局域网内，可以通过 <a href='http://hello.local'>hello.local</a> 访问（安卓手机不支持），或者内网IP地址 <a id='hip'></a> 访问，效果和直接连名片Wi-Fi是一样的。</div><button id='wificonn' class='dc pt1' onclick='connSsid()'></button></div><div class='t1 jj'><h1>更新</h1></div><div class='t2 c jj'><div id='ver' class='t'></div><button id='vup' class='dc rbtn' onclick='reqUpdate(0)'></button></div><div class='t2 c jj'><div id='wver' class='t'></div><button id='wup' class='dc rbtn' onclick='reqUpdate(1)'></button></div><div class='item v'><div class='info'>不推荐在直连名片Wi-Fi的情况下升级固件，因为升级过程会导致Wi-Fi断开，从而不能获取升级反馈。</div></div><div class='t1 jj'><h1>配置</h1></div><div class='t2 jj'><div class='t'>选择信道</div></div><div class='item'><select id='channel' class='dc' onchange='cChnage()'><option value='1'>1 ---------- 2.412GHz</option><option value='2'>2 ---------- 2.417Ghz</option><option value='3'>3 ---------- 2.422Ghz</option><option value='4'>4 ---------- 2.427Ghz</option><option value='5'>5 ---------- 2.432Ghz</option><option value='6'>6 ---------- 2.437Ghz</option><option value='7'>7 ---------- 2.442Ghz</option><option value='8'>8 ---------- 2.447Ghz</option><option value='9'>9 ---------- 2.452Ghz</option><option value='10'>10 -------- 2.457Ghz</option><option value='11'>11 -------- 2.462Ghz</option><option value='12'>12 -------- 2.467Ghz</option><option value='13'>13 -------- 2.472Ghz</option><option value='14'>14 -------- 2.484Ghz</option></select></div><div class='t2 c jj'><div class='t'>开启缓存</div><input id='cache' type='checkbox' onchange='caChange()'></div><div class='t2 jj'><div class='t'>首页地址</div></div><div class='item'><input id='homeUrl' class='dc' type='text' placeholder='请输入网站首页地址' value=''> <button class='dc rbtn' onclick='huChange()'>确 定</button></div><div class='item v'><button class='dc pt1' onclick='resetConf()'>重置所有配置</button></div><div class='t1 jj'><h1>文件管理</h1><form id='fileForm' method='POST' action='/update' enctype='multipart/form-data' target='mf'><label class='dc rl' for='file'>覆盖上传</label><input id='file' type='file' name='file' onchange='fileInputChange()' webkitdirectory multiple='multiple'></form><iframe id='mf' name='mf' style='display:none'></iframe></div><div id='state'></div><div class='item'><div>总容量：<span id='cap'></span>；已用：<span id='sy'></span></div></div><table><thead class='drak'><tr><td>名称</td><td>大小</td></tr></thead><tbody id='body'></tbody></table><script>function requestGet(e,t){var n=new XMLHttpRequest;n.timeout=1e4,n.open(\"GET\",e,!0),n.onreadystatechange=t,n.send()}function getDomById(e){return document.getElementById(e)}function req2Json(e){try{var t=e.responseText;return JSON.parse(t)}catch(e){return{}}}function isReqError(e){return isReqError2(e)||200!=e.status}function isReqError2(e){return 4!=e.readyState}function bytes2CapStr(e,t,n){switch(null==n&&(n=2),1==t&&e<1024&&(t=0),t){case 0:return e+\" B\";case 1:return(e/1024).toFixed(n)+\" KB\";case 2:return(e/1024/1024).toFixed(n)+\" MB\"}}function WCBtnType(e){var t=\"\";switch(wifistate=\"\",e){case 1:t=\"连接中...\";break;case 2:t=\"连接失败！点击重试\";break;case 3:t=\"连接成功\",wifistate=\"3\";break;default:t=\"连 接\"}wificonn.innerText=t,refreshUpBtns()}function WCBtnReset(){setWifiConnBtn(!1),WCBtnType(0)}function setWifiConnBtn(e){disDom(wificonn,e)}function disDom(e,t){t?e.setAttribute(\"disabled\",\"\"):e.removeAttribute(\"disabled\")}function disUpdateDoms(e){for(var t=[ssid,pwd,scan,vup,wup],n=0;n<t.length;n++)disDom(t[n],e)}function refreshUpBtns(){var e=\"3\"!=wifistate;disDom(vup,e),disDom(wup,e),e||(ver==nver&&disDom(vup,!0),wver==nwver&&disDom(wup,!0))}function UpBtnType(e,t,n){var i=\"\";switch(e){case 1:i=\"升级中...\";break;case 2:i=\"升级失败\";break;case 3:i=\"升级成功\";break;case 4:i=n+\"%\";break;default:i=0==t?\"固件升级\":\"网站更新\"}0==t?vup.innerText=i:wup.innerText=i}function defSetTimeout(e){setTimeout(e,1e3)}var mf=getDomById(\"mf\"),state=getDomById(\"state\"),channel=getDomById(\"channel\"),cache=getDomById(\"cache\"),homeUrl=getDomById(\"homeUrl\"),ssid=getDomById(\"ssid\"),pwd=getDomById(\"pwd\"),scan=getDomById(\"scan\"),wificonn=getDomById(\"wificonn\"),hip=getDomById(\"hip\"),vup=getDomById(\"vup\"),wup=getDomById(\"wup\"),ssidV=\"\",wifistate=\"\",ver=\"\",nver=\"\",wver=\"\",nwver=\"\";function reqScanSsid(e){setWifiConnBtn(!0),ssid.innerHTML=\"\",requestGet(\"/wifiscan\",function(){isReqError(this)||(refreshSsid(req2Json(this)),reqWificonf())})}function refreshSsid(e){e=e.reverse();for(var t=\"<option value=''>请选择 Wi-Fi</option>\",n=0;n<e.length;n++){var i=e[n];t+=\"<option value='\",t+=i.ssid,t+=\"'>\",t+=i.ssid+\"&emsp;\"+i.rssi,t+=\"</option>\"}ssid.innerHTML=t,ssid.value=ssidV,setWifiConnBtn(!1)}function reqWificonf(){requestGet(\"/wificonf\",function(){if(!isReqError(this)){var e=req2Json(this);if(ssid.value=ssidV=e.ssid,pwd.value=e.pwd,0<e.ip.length)WCBtnType(3),setWifiConnBtn(!0),hip.setAttribute(\"href\",\"http://\"+e.ip),hip.innerHTML=e.ip;else{if(0==ssidV.length)return;WCBtnType(2),setWifiConnBtn(!1)}}})}function connSsid(){WCBtnType(1),setWifiConnBtn(!0),requestGet(\"/wificonn?s=\"+ssid.value+\"&p=\"+pwd.value,function(){isReqError(this)||reqWificonnSt()})}function reqWificonnSt(){defSetTimeout(function(){requestGet(\"/wificonn?st=1\",function(){if(!isReqError2(this)){var e=this.responseText;if(\"4\"==e)return WCBtnType(2),void setWifiConnBtn(!1);(\"3\"!=e?reqWificonnSt:reqWificonf)()}})})}function reqVersion(){requestGet(\"/version\",function(){var e;isReqError(this)||(e=req2Json(this),ver=e.ver,wver=e.wver,refreshVersion())}),requestGet(\"/nVersion\",function(){var e;isReqError(this)||(e=req2Json(this),nver=e.nv,nwver=e.nw,refreshVersion())})}function refreshVersion(){var e=\"固件版本：\"+ver;e+=\"<br>\",e+=\"最新版本：\"+nver,getDomById(\"ver\").innerHTML=e,e=\"网站版本：\"+wver,e+=\"<br>\",e+=\"最新版本：\"+nwver,getDomById(\"wver\").innerHTML=e}function reqUpdate(e){requestGet(\"/update?t=\"+e,function(){isReqError(this)||(disUpdateDoms(!0),reqProgress(e),UpBtnType(1,e))})}function reqProgress(t){defSetTimeout(function(){requestGet(\"/progress\",function(){if(!isReqError2(this)){var e=this.responseText;if(\"100\"==e)return UpBtnType(3,t),disUpdateDoms(!1),void location.reload();if(\"255\"==e)return UpBtnType(2,t),void disUpdateDoms(!1);0<e.length&&UpBtnType(4,t,e),reqProgress(t)}})})}function reqConfig(){requestGet(\"/config\",function(){var e;isReqError(this)||(e=req2Json(this),channel.value=e.channel,cache.checked=e.cache,homeUrl.value=e.homeUrl)})}function cChnage(){putConfig(\"c=\"+channel.value)}function caChange(){putConfig(\"ca=\"+cache.checked)}function huChange(){putConfig(\"hu=\"+homeUrl.value)}function putConfig(e){requestGet(\"/putConfig?\"+e,function(){})}function resetConf(){requestGet(\"/is?v=1\")}function reqInfo(){requestGet(\"/info\",function(){isReqError(this)||refreshPage(req2Json(this))})}function refreshPage(e){getDomById(\"cap\").innerText=bytes2CapStr(e.totalBytes,1);for(var t=e.fList,n=\"\",i=0;i<t.length;i++){var r=t[i];n+=\"<tr\",i%2&&(n+=\" class='drak'\"),n+=\">\",n+=\"<td>\"+r.fn+\"</td>\",n+=\"<td>\"+bytes2CapStr(r.fs,1)+\"</td>\",n+=\"</tr>\"}getDomById(\"body\").innerHTML=n,getDomById(\"sy\").innerText=bytes2CapStr(e.usedBytes,1)}function fileInputChange(){showState(\"正在格式化...\"),requestGet(\"/is?v=0\",function(){if(isReqError(this))showState(\"格式化错误...\");else{showState(\"正在上传...\");for(var e=getDomById(\"file\"),t=e.files,n=\"\",i=0;i<t.length;i++)n+=t[i].size,i!=t.length-1&&(n+=\",\");e.name=n,getDomById(\"fileForm\").submit()}})}function reload(){var e=(mf.contentDocument||mf.contentWindow.document).getElementsByTagName(\"body\")[0];if(-1!=e.innerHTML.toString().indexOf(\"成功\"))return window.location.reload(),void showState(\"上传成功...\");showState(e.innerHTML)}function showState(e,t){state.style.display=\"block\",state.innerHTML=e}WCBtnReset(),UpBtnType(0,0),UpBtnType(0,1),reqScanSsid(),reqVersion(),reqConfig(),reqInfo(),mf.attachEvent?mf.attachEvent(\"onload\",reload):mf.onload=reload;</script></body></html>";

// 文件相关
Dir dir;
File file;
uint8_t maxPathLength;// 最大文件路径长度

// LED 相关
Ticker ticker;
uint8_t tickerIndex = 0;
bool isLightLed1 = false;
bool isLightLed2 = false;
bool isWLLed = false;

// 信道相关
static uint8_t channelAddr = 0;

// 首页地址相关
char homeUrl[32] = "";
static uint8_t homeUrlStartAddr = 1;

// 缓存相关
static uint8_t changeAddr = 33;

// Wi-Fi相关
static uint8_t ssidAddr = 34;
static uint8_t pwdAddr = 66;

// 更新相关
uint16_t ver = 1000;
char wver[7] = "";
static uint8_t progressAddr = 131;
