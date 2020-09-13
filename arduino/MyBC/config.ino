// 网络相关
static String MSSID = "aMyCard";
static String MPSK = "";
const char *myHostname = "esp8266";

// 网页相关
static const char TEXT_PLAIN[] PROGMEM = "text/plain;charset=UTF-8";
static const char adminHTML[] PROGMEM = "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1,minimum-scale=1,maximum-scale=1,user-scalable=no'><title>admin</title><style>body{padding:0;margin:0;color:#303133}.jj{margin:0 20px}.t1{position:relative;height:60px;margin-top:20px}.t1,.t2{display:flex;align-items:center}.t1:first-of-type{margin-top:0}.t1 h1{margin:0;flex:1;color:#303133}.t2{height:30px;margin-top:10px}.t2.c{height:50px}.t2>.t{font-size:16px;color:#303133;flex:1}.t2>input{width:44px}.dc{height:44px;line-height:44px;border-radius:4px;padding:0 12px;color:#303133;font-size:16px;background:#ebeef5;border:1px solid #e4e7ed}.rbtn{cursor:pointer;overflow:hidden;display:inline-block;text-decoration:none;margin-left:10px}.rbtn:hover{background:#e4e7ed;border-color:#ccc;text-decoration:none}.rbtn input{position:absolute;font-size:100px;right:0;top:0;opacity:0;cursor:pointer}.item{position:relative;display:flex;min-height:30px;margin:10px 20px 0 20px}.item>div{font-size:14px;color:#303133;line-height:30px}.item>input,.item>select{flex:1}table{width:100%;border-collapse:collapse}#state,td{font-size:14px;color:#303133;line-height:28px}thead td{font-weight:700}tr td:first-letter{padding-left:20px}tr td:last-child{padding-left:20px}table .drak{background-color:#ebeef5}#state{margin:20px;display:none}</style></head><body><div class='t1 jj'><h1>配置</h1></div><div class='t2 jj'><div class='t'>选择信道</div></div><div class='item'><select id='channel' class='dc' name='channel' onchange='cChnage()'><option value='1'>1 ---------- 2.412GHz</option><option value='2'>2 ---------- 2.417Ghz</option><option value='3'>3 ---------- 2.422Ghz</option><option value='4'>4 ---------- 2.427Ghz</option><option value='5'>5 ---------- 2.432Ghz</option><option value='6'>6 ---------- 2.437Ghz</option><option value='7'>7 ---------- 2.442Ghz</option><option value='8'>8 ---------- 2.447Ghz</option><option value='9'>9 ---------- 2.452Ghz</option><option value='10'>10 -------- 2.457Ghz</option><option value='11'>11 -------- 2.462Ghz</option><option value='12'>12 -------- 2.467Ghz</option><option value='13'>13 -------- 2.472Ghz</option><option value='14'>14 -------- 2.484Ghz</option></select></div><div class='t2 c jj'><div class='t'>开启缓存</div><input id='cache' type='checkbox' onchange='caChange()'></div><div class='t2 jj'><div class='t'>首页地址</div></div><div class='item'><input id='homeUrl' class='dc' type='text' placeholder='请输入网站首页地址' value=''> <button class='dc rbtn' onclick='huChange()'>确 定</button></div><div class='t1 jj'><h1>文件管理</h1><form id='fileForm' method='POST' action='/update' enctype='multipart/form-data' target='mf'><a href='javascript:;' class='dc rbtn'>覆盖上传 <input id='file' type='file' onchange='fileInputChange()' webkitdirectory multiple='multiple'></a></form><iframe id='mf' name='mf' style='display:none'></iframe></div><div id='state' onclick='stateClick()'></div><div class='item'><div>总容量：<span id='cap'></span>；已用容量：<span id='sy'></span></div></div><table><thead class='drak'><tr><td>名称</td><td>大小</td></tr></thead><tbody id='body'></tbody></table><script>var httpRequest=new XMLHttpRequest,mf=getDomById(\"mf\"),state=getDomById(\"state\"),channel=getDomById(\"channel\"),cache=getDomById(\"cache\"),homeUrl=getDomById(\"homeUrl\");function getInfo(e){requestGet(\"/info\",function(){var t;4==httpRequest.readyState&&200==httpRequest.status&&(t=httpRequest.responseText,refreshPage(JSON.parse(t)),e())})}function getConfig(){requestGet(\"/config\",function(){var t,e;4==httpRequest.readyState&&200==httpRequest.status&&(t=httpRequest.responseText,e=JSON.parse(t),channel.value=e.channel,cache.checked=e.cache,homeUrl.value=e.homeUrl)})}function refreshPage(t){getDomById(\"cap\").innerText=bytes2CapStr(t.totalBytes,1);for(var e=t.fList,n=\"\",a=0;a<e.length;a++){var o=e[a];n+=\"<tr\",a%2&&(n+=\" class='drak'\"),n+=\">\",n+=\"<td>\"+o.fn+\"</td>\",n+=\"<td>\"+bytes2CapStr(o.fs,1)+\"</td>\",n+=\"</tr>\"}getDomById(\"body\").innerHTML=n,getDomById(\"sy\").innerText=bytes2CapStr(t.usedBytes,1)}function cChnage(){putConfig(\"c=\"+channel.value)}function caChange(){putConfig(\"ca=\"+cache.checked)}function huChange(){putConfig(\"hu=\"+homeUrl.value)}function putConfig(t){requestGet(\"/putConfig?\"+t,function(){4==httpRequest.readyState&&httpRequest.status})}function fileInputChange(){showState(\"正在格式化...\"),requestGet(\"/erase\",function(){if(4==httpRequest.readyState)if(200==httpRequest.status){showState(\"正在上传...\");for(var t=getDomById(\"file\"),e=t.files,n=\"\",a=0;a<e.length;a++)n+=e[a].size,a!=e.length-1&&(n+=\",\");t.name=n,getDomById(\"fileForm\").submit()}else showState(\"格式化错误...\")})}function stateClick(){state.style.display=\"none\"}function reload(){var t=(mf.contentDocument||mf.contentWindow.document).getElementsByTagName(\"body\")[0];if(-1!=t.innerHTML.toString().indexOf(\"成功\"))return window.location.reload(),void showState(\"上传成功...\");showState(t.innerHTML)}function bytes2CapStr(t,e,n){switch(null==n&&(n=2),1==e&&t<1024&&(e=0),e){case 0:return t+\" B\";case 1:return(t/1024).toFixed(n)+\" KB\";case 2:return(t/1024/1024).toFixed(n)+\" MB\"}}function requestGet(t,e){httpRequest.open(\"GET\",t,!0),httpRequest.onreadystatechange=e,httpRequest.send()}function showState(t,e){state.style.display=\"block\",state.innerHTML=t}function getDomById(t){return document.getElementById(t)}getInfo(function(){getConfig()}),mf.attachEvent?mf.attachEvent(\"onload\",reload):mf.onload=reload;</script></body></html>";

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
static uint8_t homeUrlEndAddr = 32;

// 缓存相关
static uint8_t changeAddr = 33;
