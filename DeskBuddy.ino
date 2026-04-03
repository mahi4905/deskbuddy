// ==================================================
// DESKBUDDY - EDISON SCIENCE CORNER - ESCLABS
// ESP32 S3 Mini | SH1106 OLED | MAX4466 Mic
// Full Featured Version
// ==================================================

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "time.h"
#include <math.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

// ==================================================
// PIN DEFINITIONS
// ==================================================
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define SDA_PIN       8
#define SCL_PIN       9
#define TOUCH_PIN     7
#define MIC_PIN       4

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==================================================
// BITMAPS
// ==================================================
const unsigned char bmp_clear[] PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x01,0x80,0x00,0x00,0x01,0x80,0x00,
  0x00,0x00,0x00,0x00,0x01,0x03,0xc0,0x80,0x00,0x0f,0xf0,0x00,0x00,0x3f,0xfc,0x00,
  0x00,0x7f,0xfe,0x00,0x00,0xff,0xff,0x00,0x06,0xff,0xff,0x60,0x06,0xff,0xff,0x60,
  0x06,0xff,0xff,0x60,0x00,0xff,0xff,0x00,0x3e,0xff,0xff,0x7c,0x3e,0xff,0xff,0x7c,
  0x3e,0xff,0xff,0x7c,0x00,0xff,0xff,0x00,0x06,0xff,0xff,0x60,0x06,0xff,0xff,0x60,
  0x06,0xff,0xff,0x60,0x00,0xff,0xff,0x00,0x00,0x7f,0xfe,0x00,0x00,0x3f,0xfc,0x00,
  0x01,0x0f,0xf0,0x80,0x00,0x03,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,
  0x00,0x01,0x80,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
const unsigned char bmp_clouds[] PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xe0,0x00,
  0x00,0x0f,0xf8,0x00,0x00,0x1f,0xfc,0x00,0x00,0x3f,0xfe,0x00,0x00,0x3f,0xff,0x00,
  0x00,0x7f,0xff,0x80,0x00,0xff,0xff,0xc0,0x00,0xff,0xff,0xe0,0x01,0xff,0xff,0xf0,
  0x03,0xff,0xff,0xf8,0x07,0xff,0xff,0xfc,0x07,0xff,0xff,0xfc,0x0f,0xff,0xff,0xfe,
  0x0f,0xff,0xff,0xfe,0x1f,0xff,0xff,0xff,0x1f,0xff,0xff,0xff,0x1f,0xff,0xff,0xff,
  0x1f,0xff,0xff,0xff,0x1f,0xff,0xff,0xff,0x1f,0xff,0xff,0xff,0x0f,0xff,0xff,0xfe,
  0x07,0xff,0xff,0xfc,0x03,0xff,0xff,0xf8,0x00,0xff,0xff,0xe0,0x00,0x3f,0xff,0x80,
  0x00,0x0f,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
const unsigned char bmp_rain[] PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x03,0xe0,0x00,0x00,0x0f,0xf8,0x00,0x00,0x1f,0xfc,0x00,
  0x00,0x3f,0xfe,0x00,0x00,0x7f,0xff,0x80,0x00,0xff,0xff,0xc0,0x01,0xff,0xff,0xf0,
  0x03,0xff,0xff,0xf8,0x07,0xff,0xff,0xfc,0x0f,0xff,0xff,0xfe,0x1f,0xff,0xff,0xff,
  0x1f,0xff,0xff,0xff,0x1f,0xff,0xff,0xff,0x1f,0xff,0xff,0xff,0x0f,0xff,0xff,0xfe,
  0x07,0xff,0xff,0xfc,0x03,0xff,0xff,0xf8,0x00,0xff,0xff,0xe0,0x00,0x3f,0xff,0x80,
  0x00,0x0f,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x0c,0x00,0x00,0x60,0x0c,0x00,
  0x00,0xe0,0x1c,0x00,0x00,0xc0,0x18,0x00,0x03,0x80,0x70,0x00,0x03,0x80,0x70,0x00,
  0x03,0x00,0x60,0x00,0x02,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
const unsigned char mini_sun[]   PROGMEM = {
  0x00,0x00,0x01,0x80,0x00,0x00,0x10,0x08,0x04,0x20,0x03,0xc0,0x27,0xe4,0x07,0xe0,
  0x07,0xe0,0x27,0xe4,0x03,0xc0,0x04,0x20,0x10,0x08,0x00,0x00,0x01,0x80,0x00,0x00
};
const unsigned char mini_cloud[] PROGMEM = {
  0x00,0x00,0x00,0x00,0x01,0xc0,0x07,0xe0,0x0f,0xf0,0x1f,0xf8,0x1f,0xf8,0x3f,0xfc,
  0x3f,0xfc,0x7f,0xfe,0x3f,0xfe,0x1f,0xfc,0x0f,0xf0,0x00,0x00,0x00,0x00,0x00,0x00
};
const unsigned char mini_rain[]  PROGMEM = {
  0x00,0x00,0x00,0x00,0x01,0xc0,0x07,0xe0,0x0f,0xf0,0x1f,0xf8,0x1f,0xf8,0x3f,0xfc,
  0x3f,0xfc,0x7f,0xfe,0x3f,0xfe,0x1f,0xfc,0x00,0x00,0x44,0x44,0x22,0x22,0x11,0x11
};
const unsigned char bmp_tiny_drop[] PROGMEM = { 0x10,0x38,0x7c,0xfe,0xfe,0x7c,0x38,0x00 };
const unsigned char bmp_heart[]  PROGMEM = {
  0x00,0x00,0x0c,0x60,0x1e,0xf0,0x3f,0xf8,0x7f,0xfc,0x7f,0xfc,0x7f,0xfc,0x3f,0xf8,
  0x1f,0xf0,0x0f,0xe0,0x07,0xc0,0x03,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
const unsigned char bmp_zzz[]    PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x00,0x0c,0x00,0x18,0x00,0x30,0x00,0x7e,
  0x00,0x00,0x3c,0x00,0x0c,0x00,0x18,0x00,0x30,0x00,0x7c,0x00,0x00,0x00,0x00,0x00
};
const unsigned char bmp_anger[]  PROGMEM = {
  0x00,0x00,0x11,0x10,0x2a,0x90,0x44,0x40,0x80,0x20,0x80,0x20,0x44,0x40,0x2a,0x90,
  0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// ==================================================
// MOODS
// ==================================================
#define MOOD_NORMAL     0
#define MOOD_HAPPY      1
#define MOOD_SURPRISED  2
#define MOOD_SLEEPY     3
#define MOOD_ANGRY      4
#define MOOD_SAD        5
#define MOOD_EXCITED    6
#define MOOD_LOVE       7
#define MOOD_SUSPICIOUS 8
int currentMood = MOOD_NORMAL;

// ==================================================
// EYE PHYSICS
// ==================================================
struct Eye {
  float x,y,w,h,targetX,targetY,targetW,targetH;
  float pupilX,pupilY,targetPupilX,targetPupilY;
  float velX,velY,velW,velH,pVelX,pVelY;
  float k=0.12,d=0.60,pk=0.08,pd=0.50;
  bool blinking; unsigned long lastBlink,nextBlinkTime;
  void init(float _x,float _y,float _w,float _h){
    x=targetX=_x; y=targetY=_y; w=targetW=_w; h=targetH=_h;
    pupilX=targetPupilX=pupilY=targetPupilY=0;
    nextBlinkTime=millis()+random(1000,4000);
  }
  void update(){
    velX=(velX+(targetX-x)*k)*d; velY=(velY+(targetY-y)*k)*d;
    velW=(velW+(targetW-w)*k)*d; velH=(velH+(targetH-h)*k)*d;
    x+=velX; y+=velY; w+=velW; h+=velH;
    pVelX=(pVelX+(targetPupilX-pupilX)*pk)*pd;
    pVelY=(pVelY+(targetPupilY-pupilY)*pk)*pd;
    pupilX+=pVelX; pupilY+=pVelY;
  }
};
Eye leftEye, rightEye;
unsigned long lastSaccade=0, saccadeInterval=3000;
float breathVal=0;

// ==================================================
// PAGE CONSTANTS
// ==================================================
#define PAGE_EYES     0
#define PAGE_CLOCK    1
#define PAGE_WEATHER  2
#define PAGE_POMO     3
#define PAGE_WATER    4
#define PAGE_ANALOG   5
#define PAGE_MOVIE    6
#define PAGE_SUN      10
#define PAGE_WITTY    11
#define TOTAL_MAIN    7

// ==================================================
// GLOBAL STATE
// ==================================================
int  currentPage   = 0;
bool highBrightness= true;

// Touch
int  tapCounter=0; unsigned long lastTapTime=0;
bool lastPinState=false;
unsigned long pressStartTime=0;
bool isLongPressHandled=false;
const unsigned long LONG_PRESS_TIME  = 800;
const unsigned long DOUBLE_TAP_DELAY = 300;
unsigned long lastPageSwitch=0;
const unsigned long PAGE_INTERVAL = 8000;

// Weather
float temperature=0,feelsLike=0,tempMin=0,tempMax=0;
int   humidity=0; float windSpeed=0;
String weatherMain="Loading", weatherDesc="Please wait...";
long  sunriseTime=0, sunsetTime=0;
unsigned long lastWeatherUpdate=0;
struct ForecastDay{ String dayName; int temp; String iconType; };
ForecastDay fcast[3];

// Quote
String currentQuote="Connecting...", currentAuthor="";
int quoteScrollX=SCREEN_WIDTH;
unsigned long lastQuoteScroll=0, lastQuoteUpdate=0;
int lastQuoteHour=-1;

// Movie
String movieTitle="", movieDirector="", movieYear="", movieRating="";
int movieTitleSX=SCREEN_WIDTH, movieDirSX=SCREEN_WIDTH;
unsigned long lastMovieScroll=0, lastMovieUpdate=0;
int lastMovieDay=-1;

// Mic
#define MIC_SAMPLES 10
#define SILENCE_THRESH  200
#define MEDIUM_THRESH   900
#define LOUD_THRESH    2600
#define SPIKE_THRESH   1600
int  micBuf[MIC_SAMPLES]; int micIdx=0;
int  micAvg=0, micPeak=0, micPrev=0;
unsigned long lastMicSample=0;
bool micMuted=false;
unsigned long moodHoldUntil=0;
int  heldMood=MOOD_NORMAL;

// Sleep
unsigned long lastActivityTime=0;
const unsigned long SLEEP_MOOD_MS = 120000;
const unsigned long SLEEP_OFF_MS  = 300000;
bool displayOff=false;

// Water
int  waterCount=0, waterGoal=8;
bool showWaterPopup=false;
unsigned long lastWaterReminder=0, waterPopupShown=0;
const unsigned long WATER_INTERVAL    = 3600000;
const unsigned long WATER_POPUP_SHOW  = 12000;

// Pomodoro
bool pomoLocked=false, pomoRunning=false;
int  pomoSession=0, pomoCount=0;
unsigned long pomoStart=0, pomoElapsed=0;
const unsigned long POMO_WORK  = 1500000;
const unsigned long POMO_SHORT =  300000;
const unsigned long POMO_LONG  =  900000;
bool pomoFlashing=false; unsigned long pomoFlashStart=0;

// Config
String wifiSsid,wifiPass,apiKey,city,countryCode,tzString,tmdbKey,movieGenre;
float  bodyWeight=70.0;
const char* ntpServer="pool.ntp.org";

// Portal
#define CONFIG_AP_SSID "DeskBuddy-Setup"
#define CONFIG_AP_PASS "12345678"
#define CONFIG_HOLD_MS 3000
Preferences prefs; WebServer configServer(80); bool inConfigMode=false;

// ==================================================
// CONFIG PORTAL
// ==================================================
void loadConfig(){
  prefs.begin("deskbuddy",true);
  wifiSsid    = prefs.getString("ssid","");
  wifiPass    = prefs.getString("pass","");
  apiKey      = prefs.getString("apikey","452d284a8c71cb7125cb932f7a2afa27");
  city        = prefs.getString("city","Mysuru");
  countryCode = prefs.getString("country","IN");
  tzString    = prefs.getString("tz","IST-5:30");
  tmdbKey     = prefs.getString("tmdbkey","1ca147506caad53c2085e5a394f01a11");
  movieGenre  = prefs.getString("genre","28");
  bodyWeight  = prefs.getFloat("weight",70.0);
  prefs.end();
  if(wifiSsid.isEmpty()){ wifiSsid="edison science corner"; wifiPass="eeeeeeee"; }
  waterGoal = max(6,(int)(bodyWeight*0.033/0.25));
}

void saveConfig(String s,String p,String ak,String cy,String ct,
                String tz,String tk,String mg,float wt){
  prefs.begin("deskbuddy",false);
  prefs.putString("ssid",s); prefs.putString("pass",p);
  prefs.putString("apikey",ak); prefs.putString("city",cy);
  prefs.putString("country",ct); prefs.putString("tz",tz);
  prefs.putString("tmdbkey",tk); prefs.putString("genre",mg);
  prefs.putFloat("weight",wt);
  prefs.end();
}

void handleConfigRoot(){
  prefs.begin("deskbuddy",true);
  String ss=prefs.getString("ssid",""),ak=prefs.getString("apikey","");
  String cy=prefs.getString("city","Mysuru"),ct=prefs.getString("country","IN");
  String tz=prefs.getString("tz","IST-5:30"),tk=prefs.getString("tmdbkey","");
  String mg=prefs.getString("genre","28"); float wt=prefs.getFloat("weight",70.0);
  prefs.end();

  String html=R"raw(<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>DeskBuddy</title>
<style>
body{font-family:sans-serif;max-width:440px;margin:30px auto;padding:20px;
     background:#0c1929;color:#e8f4fc;}
h1{color:#5ba3f5;} h3{color:#5ba3f5;margin:16px 0 4px;}
input,select{width:100%;padding:10px;margin:4px 0;border:1px solid #2d4a6f;
  border-radius:6px;box-sizing:border-box;background:#1a2d47;color:#e8f4fc;}
input:focus,select:focus{outline:none;border-color:#5ba3f5;}
button{width:100%;padding:12px;background:#3498db;color:#fff;border:none;
  border-radius:6px;font-size:16px;cursor:pointer;margin-top:14px;}
label{display:block;margin-top:10px;color:#8ab4e8;font-size:13px;}
.sec{margin-top:16px;padding-top:14px;border-top:1px solid #1e3a5f;}
</style></head><body><h1>DeskBuddy Setup</h1>
<form action="/save" method="POST">
<h3>WiFi</h3>
<label>SSID</label><input name="ssid" value=")raw";
  html+=ss; html+=R"raw(">
<label>Password</label><input name="pass" type="password" placeholder="leave blank to keep current">
<div class="sec"><h3>Weather</h3>
<label>OpenWeatherMap API Key</label><input name="apikey" value=")raw";
  html+=ak; html+=R"raw(">
<label>City</label><input name="city" value=")raw";
  html+=cy; html+=R"raw(">
<label>Country Code (IN/US/GB...)</label><input name="country" value=")raw";
  html+=ct; html+=R"raw(">
</div><div class="sec"><h3>Time</h3>
<label>Timezone (e.g. IST-5:30 / EST5EDT)</label><input name="tz" value=")raw";
  html+=tz; html+=R"raw(">
</div><div class="sec"><h3>Movies</h3>
<label>TMDB API Key</label><input name="tmdbkey" value=")raw";
  html+=tk; html+=R"raw(">
<label>Preferred Genre</label><select name="genre">)raw";
  String genres[][2]={{"28","Action"},{"35","Comedy"},{"18","Drama"},
    {"27","Horror"},{"878","Sci-Fi"},{"53","Thriller"},{"12","Adventure"},{"16","Animation"}};
  for(auto& g:genres){
    html+="<option value=\""+g[0]+"\"";
    if(mg==g[0]) html+=" selected";
    html+=">"+g[1]+"</option>";
  }
  html+=R"raw(</select></div>
<div class="sec"><h3>Health</h3>
<label>Body Weight (kg) — sets daily water goal</label>
<input name="weight" type="number" step="0.1" value=")raw";
  html+=String(wt,1); html+=R"raw(">
</div><button type="submit">Save &amp; Reboot</button>
</form></body></html>)raw";
  configServer.send(200,"text/html",html);
}

void handleConfigSave(){
  String s=configServer.arg("ssid");
  if(s.isEmpty()){configServer.send(400,"text/plain","SSID required");return;}
  String p=configServer.arg("pass"),ak=configServer.arg("apikey");
  String cy=configServer.arg("city"),ct=configServer.arg("country");
  String tz=configServer.arg("tz"),tk=configServer.arg("tmdbkey");
  String mg=configServer.arg("genre");
  float wt=configServer.arg("weight").toFloat();
  if(wt<30||wt>300) wt=70.0;
  if(p.isEmpty()){prefs.begin("deskbuddy",true);p=prefs.getString("pass","");prefs.end();}
  saveConfig(s,p,ak,cy,ct,tz,tk,mg,wt);
  configServer.send(200,"text/html",
    "<html><body style='background:#0c1929;color:#e8f4fc;font-family:sans-serif;padding:40px'>"
    "<h2 style='color:#5ba3f5'>Saved!</h2><p>Rebooting in 2s...</p></body></html>");
  delay(2000); ESP.restart();
}

void startConfigPortal(){
  inConfigMode=true;
  WiFi.mode(WIFI_AP); WiFi.softAP(CONFIG_AP_SSID,CONFIG_AP_PASS);
  configServer.on("/",handleConfigRoot);
  configServer.on("/save",HTTP_POST,handleConfigSave);
  configServer.begin();
  display.clearDisplay(); display.setFont(NULL); display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.print("Config Mode\n\nWiFi:\nDeskBuddy-Setup\nPass: 12345678\n\nOpen:\n192.168.4.1");
  display.display();
}

// ==================================================
// NETWORK
// ==================================================
const unsigned char* getBigIcon(String w){
  if(w=="Clear") return bmp_clear;
  if(w=="Clouds") return bmp_clouds;
  if(w=="Rain"||w=="Drizzle") return bmp_rain;
  return bmp_clouds;
}
const unsigned char* getMiniIcon(String w){
  if(w=="Clear") return mini_sun;
  if(w=="Rain"||w=="Drizzle"||w=="Thunderstorm") return mini_rain;
  return mini_cloud;
}

void getWeatherAndForecast(){
  if(WiFi.status()!=WL_CONNECTED) return;
  HTTPClient http;
  String url="http://api.openweathermap.org/data/2.5/weather?q="+city+","+
             countryCode+"&appid="+apiKey+"&units=metric";
  http.begin(url);
  if(http.GET()==200){
    JSONVar o=JSON.parse(http.getString());
    if(JSON.typeof(o)!="undefined"){
      temperature=double(o["main"]["temp"]);
      feelsLike  =double(o["main"]["feels_like"]);
      tempMin    =double(o["main"]["temp_min"]);
      tempMax    =double(o["main"]["temp_max"]);
      humidity   =int(o["main"]["humidity"]);
      windSpeed  =double(o["wind"]["speed"]);
      sunriseTime=long(o["sys"]["sunrise"]);
      sunsetTime =long(o["sys"]["sunset"]);
      weatherMain=(const char*)o["weather"][0]["main"];
      weatherDesc=(const char*)o["weather"][0]["description"];
      weatherDesc[0]=toupper(weatherDesc[0]);
    }
  }
  http.end();
  url="http://api.openweathermap.org/data/2.5/forecast?q="+city+","+
      countryCode+"&appid="+apiKey+"&units=metric";
  http.begin(url);
  if(http.GET()==200){
    JSONVar fo=JSON.parse(http.getString());
    if(JSON.typeof(fo)!="undefined"){
      struct tm t; getLocalTime(&t);
      const char* days[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
      int idx[3]={7,15,23};
      for(int i=0;i<3;i++){
        fcast[i].temp    =(int)double(fo["list"][idx[i]]["main"]["temp"]);
        fcast[i].iconType=(const char*)fo["list"][idx[i]]["weather"][0]["main"];
        fcast[i].dayName =days[(t.tm_wday+i+1)%7];
      }
    }
  }
  http.end();
  lastWeatherUpdate=millis();
}

void getQuote(){
  if(WiFi.status()!=WL_CONNECTED) return;
  HTTPClient http;
  http.begin("https://zenquotes.io/api/random");
  if(http.GET()==200){
    JSONVar o=JSON.parse(http.getString());
    if(JSON.typeof(o)!="undefined"){
      currentQuote =(const char*)o[0]["q"];
      currentAuthor=(const char*)o[0]["a"];
      quoteScrollX =SCREEN_WIDTH;
    }
  }
  http.end();
}

void getMovie(){
  if(WiFi.status()!=WL_CONNECTED) return;
  HTTPClient http;
  String url="https://api.themoviedb.org/3/discover/movie?api_key="+tmdbKey+
             "&with_genres="+movieGenre+"&sort_by=vote_average.desc"+
             "&vote_count.gte=500&page="+String(random(1,8));
  http.begin(url);
  if(http.GET()==200){
    JSONVar o=JSON.parse(http.getString());
    if(JSON.typeof(o)!="undefined"){
      int idx=random(0,20);
      int mid=int(o["results"][idx]["id"]);
      movieTitle  =(const char*)o["results"][idx]["title"];
      float rt    =double(o["results"][idx]["vote_average"]);
      String rd   =(const char*)o["results"][idx]["release_date"];
      movieYear   =rd.substring(0,4);
      movieRating =String(rt,1);
      http.end();
      // Get director
      http.begin("https://api.themoviedb.org/3/movie/"+String(mid)+
                 "/credits?api_key="+tmdbKey);
      if(http.GET()==200){
        JSONVar cr=JSON.parse(http.getString());
        if(JSON.typeof(cr)!="undefined"){
          JSONVar crew=cr["crew"];
          for(int i=0;i<40;i++){
            if(String((const char*)crew[i]["job"])=="Director"){
              movieDirector=(const char*)crew[i]["name"]; break;
            }
          }
        }
      }
    }
  }
  http.end();
  movieTitleSX=SCREEN_WIDTH; movieDirSX=SCREEN_WIDTH;
}

// ==================================================
// MICROPHONE
// ==================================================
void updateMic(){
  if(millis()-lastMicSample<50) return;
  lastMicSample=millis();
  int raw=analogRead(MIC_PIN);
  micBuf[micIdx]= raw; micIdx=(micIdx+1)%MIC_SAMPLES;
  int s=0; for(int i=0;i<MIC_SAMPLES;i++) s+=micBuf[i]; micAvg=s/MIC_SAMPLES;
  int spike=abs(raw-micPrev); micPeak=max(micPeak-15,spike); micPrev=raw;
}

void updateMoodFromMic(){
  if(micMuted) return;
  unsigned long now=millis();
  int newMood=MOOD_NORMAL;
  if(micPeak>SPIKE_THRESH){
    newMood=MOOD_SURPRISED; heldMood=MOOD_SURPRISED; moodHoldUntil=now+2000;
  } else if(micAvg>LOUD_THRESH){
    newMood=MOOD_ANGRY;
  } else if(micPeak>800&&micAvg>500){
    newMood=MOOD_EXCITED;
  } else if(micAvg<SILENCE_THRESH){
    newMood=(now-lastActivityTime>SLEEP_MOOD_MS)?MOOD_SLEEPY:MOOD_NORMAL;
  } else {
    newMood=MOOD_NORMAL;
  }
  if(now<moodHoldUntil) newMood=heldMood;
  else heldMood=MOOD_NORMAL;
  currentMood=newMood;
}

// ==================================================
// TOUCH
// ==================================================
void handleTouch(){
  bool cur=digitalRead(TOUCH_PIN);
  unsigned long now=millis();

  if(cur&&!lastPinState){
    pressStartTime=now; isLongPressHandled=false;
    lastActivityTime=now;
    if(displayOff){
      displayOff=false; display.setContrast(highBrightness?255:1);
      currentMood=MOOD_SURPRISED;
    }
  } else if(cur&&lastPinState){
    if(now-pressStartTime>LONG_PRESS_TIME&&!isLongPressHandled){
      isLongPressHandled=true; lastPageSwitch=now;
      if(currentPage==PAGE_EYES){ micMuted=!micMuted; if(!micMuted) currentMood=MOOD_SURPRISED; }
      else if(currentPage==PAGE_CLOCK)  currentPage=PAGE_SUN;
      else if(currentPage==PAGE_WEATHER) currentPage=PAGE_WITTY;
      else if(currentPage==PAGE_POMO){
        if(!pomoLocked){ pomoLocked=true; pomoRunning=false; pomoSession=pomoCount=0; pomoElapsed=0; }
        else { pomoLocked=false; pomoRunning=false; }
      }
      else if(currentPage==PAGE_SUN)   currentPage=PAGE_CLOCK;
      else if(currentPage==PAGE_WITTY) currentPage=PAGE_WEATHER;
    }
  } else if(!cur&&lastPinState){
    if(now-pressStartTime<LONG_PRESS_TIME&&!isLongPressHandled){
      tapCounter++; lastTapTime=now; lastActivityTime=now;
    }
  }
  lastPinState=cur;

  if(tapCounter>0&&now-lastTapTime>DOUBLE_TAP_DELAY){
    lastPageSwitch=now;
    // Water popup intercept
    if(showWaterPopup){
      if(tapCounter==1){ waterCount=min(waterCount+1,waterGoal); showWaterPopup=false; }
      else { lastWaterReminder=now-WATER_INTERVAL+1800000; showWaterPopup=false; }
    }
    // Pomodoro locked mode
    else if(pomoLocked){
      if(tapCounter==1){
        if(!pomoRunning){ pomoRunning=true; pomoStart=now-pomoElapsed; }
        else{ pomoElapsed=now-pomoStart; pomoRunning=false; }
      } else { pomoRunning=false; pomoElapsed=0; }
    }
    // Normal navigation
    else {
      if(tapCounter>=2){
        highBrightness=!highBrightness;
        display.setContrast(highBrightness?255:1);
      } else {
        if(currentPage==PAGE_SUN||currentPage==PAGE_WITTY){
          currentPage=(currentPage==PAGE_SUN)?PAGE_CLOCK:PAGE_WEATHER;
        } else if(currentPage==PAGE_MOVIE){
          getMovie(); // tap = new movie
        } else if(currentPage==PAGE_WATER){
          waterCount=min(waterCount+1,waterGoal);
        } else {
          currentPage++; if(currentPage>=TOTAL_MAIN) currentPage=0;
          if(currentPage!=PAGE_POMO){ pomoLocked=false; pomoRunning=false; }
        }
      }
    }
    tapCounter=0;
  }
}

// ==================================================
// EYE DRAWING
// ==================================================
void drawEyelidMask(float x,float y,float w,float h,int mood,bool isLeft){
  int ix=x,iy=y,iw=w,ih=h;
  if(mood==MOOD_ANGRY){
    if(isLeft) for(int i=0;i<16;i++) display.drawLine(ix,iy+i,ix+iw,iy-6+i,SH110X_BLACK);
    else       for(int i=0;i<16;i++) display.drawLine(ix,iy-6+i,ix+iw,iy+i,SH110X_BLACK);
  } else if(mood==MOOD_SAD){
    if(isLeft) for(int i=0;i<16;i++) display.drawLine(ix,iy-6+i,ix+iw,iy+i,SH110X_BLACK);
    else       for(int i=0;i<16;i++) display.drawLine(ix,iy+i,ix+iw,iy-6+i,SH110X_BLACK);
  } else if(mood==MOOD_HAPPY||mood==MOOD_LOVE||mood==MOOD_EXCITED){
    display.fillRect(ix,iy+ih-12,iw,14,SH110X_BLACK);
    display.fillCircle(ix+iw/2,iy+ih+6,iw/1.3,SH110X_BLACK);
  } else if(mood==MOOD_SLEEPY){
    display.fillRect(ix,iy,iw,ih/2+2,SH110X_BLACK);
  } else if(mood==MOOD_SUSPICIOUS){
    if(isLeft) display.fillRect(ix,iy,iw,ih/2-2,SH110X_BLACK);
    else       display.fillRect(ix,iy+ih-8,iw,8,SH110X_BLACK);
  }
}

void drawUltraProEye(Eye& e,bool isLeft){
  int ix=e.x,iy=e.y,iw=e.w,ih=e.h;
  int r=(iw<20)?3:8;
  display.fillRoundRect(ix,iy,iw,ih,r,SH110X_WHITE);
  int cx=ix+iw/2,cy=iy+ih/2,pw=iw/2.2,ph=ih/2.2;
  int px=constrain(cx+(int)e.pupilX-pw/2,ix,ix+iw-pw);
  int py=constrain(cy+(int)e.pupilY-ph/2,iy,iy+ih-ph);
  display.fillRoundRect(px,py,pw,ph,r/2,SH110X_BLACK);
  if(iw>15&&ih>15) display.fillCircle(px+pw-4,py+4,2,SH110X_WHITE);
  drawEyelidMask(e.x,e.y,e.w,e.h,currentMood,isLeft);
}

void drawMutedEyes(){
  display.drawLine(18,32,54,32,SH110X_WHITE);
  display.drawLine(18,33,54,33,SH110X_WHITE);
  display.drawLine(74,32,110,32,SH110X_WHITE);
  display.drawLine(74,33,110,33,SH110X_WHITE);
  display.setFont(NULL); display.setCursor(110,2); display.print("M");
}

void updatePhysicsAndMood(){
  unsigned long now=millis();
  breathVal=sin(now/800.0)*1.5;
  if(now>leftEye.nextBlinkTime){
    leftEye.blinking=rightEye.blinking=true;
    leftEye.lastBlink=now;
    leftEye.nextBlinkTime=now+random(2000,6000);
  }
  if(leftEye.blinking){
    leftEye.targetH=rightEye.targetH=2;
    if(now-leftEye.lastBlink>120) leftEye.blinking=rightEye.blinking=false;
  }
  if(!leftEye.blinking&&now-lastSaccade>saccadeInterval){
    lastSaccade=now; saccadeInterval=random(500,3000);
    int dir=random(0,10);
    float lx=0,ly=0;
    if(dir==4){lx=-6;ly=-4;} else if(dir==5){lx=6;ly=-4;}
    else if(dir==6){lx=-6;ly=4;} else if(dir==7){lx=6;ly=4;}
    else if(dir==8){lx=8;ly=0;} else if(dir==9){lx=-8;ly=0;}
    leftEye.targetPupilX=rightEye.targetPupilX=lx;
    leftEye.targetPupilY=rightEye.targetPupilY=ly;
    leftEye.targetX=18+lx*0.3; leftEye.targetY=14+ly*0.3;
    rightEye.targetX=74+lx*0.3; rightEye.targetY=14+ly*0.3;
  }
  if(!leftEye.blinking){
    float bW=36,bH=36+breathVal;
    switch(currentMood){
      case MOOD_NORMAL:
        leftEye.targetW=rightEye.targetW=bW;
        leftEye.targetH=rightEye.targetH=bH; break;
      case MOOD_HAPPY: case MOOD_LOVE:
        leftEye.targetW=rightEye.targetW=40;
        leftEye.targetH=rightEye.targetH=32; break;
      case MOOD_SURPRISED:
        leftEye.targetW=rightEye.targetW=30;
        leftEye.targetH=rightEye.targetH=45;
        leftEye.targetPupilX+=random(-1,2); break;
      case MOOD_SLEEPY:
        leftEye.targetW=rightEye.targetW=38;
        leftEye.targetH=rightEye.targetH=30; break;
      case MOOD_ANGRY:
        leftEye.targetW=rightEye.targetW=34;
        leftEye.targetH=rightEye.targetH=32; break;
      case MOOD_SAD:
        leftEye.targetW=rightEye.targetW=34;
        leftEye.targetH=rightEye.targetH=40; break;
      case MOOD_EXCITED:
        leftEye.targetW=rightEye.targetW=40;
        leftEye.targetH=rightEye.targetH=38;
        leftEye.targetPupilX+=random(-2,3);
        rightEye.targetPupilX+=random(-2,3); break;
      case MOOD_SUSPICIOUS:
        leftEye.targetW=36; leftEye.targetH=20;
        rightEye.targetW=36; rightEye.targetH=42; break;
    }
  }
  leftEye.update(); rightEye.update();
}

void drawPageDots(int pg){
  int sp=10, sx=(SCREEN_WIDTH-(TOTAL_MAIN*sp))/2;
  for(int i=0;i<TOTAL_MAIN;i++){
    int dx=sx+i*sp+4;
    if(i==pg) display.fillCircle(dx,62,2,SH110X_WHITE);
    else       display.drawCircle(dx,62,2,SH110X_WHITE);
  }
}

// ==================================================
// PAGE DRAWING
// ==================================================

// PAGE 0 - EYES
void drawEmoPage(){
  if(micMuted){ drawMutedEyes(); return; }
  updatePhysicsAndMood();
  if(currentMood==MOOD_LOVE)    display.drawBitmap(56,0,bmp_heart,16,16,SH110X_WHITE);
  else if(currentMood==MOOD_SLEEPY) display.drawBitmap(110,0,bmp_zzz,16,16,SH110X_WHITE);
  else if(currentMood==MOOD_ANGRY)  display.drawBitmap(56,0,bmp_anger,16,16,SH110X_WHITE);
  drawUltraProEye(leftEye,true);
  drawUltraProEye(rightEye,false);
}

// PAGE 1 - CLOCK + QUOTE
void drawClock(){
  struct tm t; display.setTextColor(SH110X_WHITE);
  if(!getLocalTime(&t)){
    display.setFont(NULL); display.setCursor(30,30); display.print("Syncing..."); return;
  }
  String ampm=(t.tm_hour>=12)?"PM":"AM";
  int h12=t.tm_hour%12; if(!h12) h12=12;
  display.setFont(NULL);
  // AM/PM top left
  display.setCursor(0,0); display.print(ampm);
  // Temperature top right
  String tStr=String((int)temperature)+"\xf8""C";
  display.setCursor(SCREEN_WIDTH-tStr.length()*6,0);
  display.print(tStr);
  // Large time
  display.setFont(&FreeSansBold18pt7b);
  char ts[6]; sprintf(ts,"%02d:%02d",h12,t.tm_min);
  int16_t x1,y1; uint16_t w,h2;
  display.getTextBounds(ts,0,0,&x1,&y1,&w,&h2);
  display.setCursor((SCREEN_WIDTH-w)/2,40);
  display.print(ts);
  // Date
  display.setFont(&FreeSans9pt7b);
  char ds[20]; strftime(ds,20,"%a, %b %d",&t);
  display.getTextBounds(ds,0,0,&x1,&y1,&w,&h2);
  display.setCursor((SCREEN_WIDTH-w)/2,53);
  display.print(ds);
  // Scrolling quote
  display.setFont(NULL);
  String q="\""+currentQuote+"\" - "+currentAuthor+"   ";
  display.setCursor(quoteScrollX,58);
  display.print(q);
  int qw=q.length()*6;
  if(quoteScrollX+qw<SCREEN_WIDTH){ display.setCursor(quoteScrollX+qw+8,58); display.print(q); }
  drawPageDots(PAGE_CLOCK);
}

// PAGE 2 - WEATHER MINIMAL
void drawWeatherCard(){
  if(WiFi.status()!=WL_CONNECTED){
    display.setFont(NULL); display.setCursor(0,0); display.print("No WiFi"); return;
  }
  display.setTextColor(SH110X_WHITE);
  // City top left
  display.setFont(&FreeSansBold9pt7b);
  String c=city; c.toUpperCase();
  if(c.length()>9) c=c.substring(0,8)+".";
  display.setCursor(0,12); display.print(c);
  // Icon top right
  display.drawBitmap(96,0,getBigIcon(weatherMain),32,32,SH110X_WHITE);
  // Thermometer (simple)
  display.drawLine(18,18,18,40,SH110X_WHITE);
  display.drawLine(17,18,19,18,SH110X_WHITE);
  display.fillCircle(18,44,5,SH110X_WHITE);
  display.fillRect(16,36,5,10,SH110X_WHITE);
  // Big temp
  display.setFont(&FreeSansBold18pt7b);
  String ts=String((int)temperature);
  int16_t x1,y1; uint16_t w,h2;
  display.getTextBounds(ts.c_str(),0,0,&x1,&y1,&w,&h2);
  display.setCursor(32,46); display.print(ts);
  display.fillCircle(32+w+6,28,3,SH110X_WHITE);
  // Min/Max
  display.setFont(NULL);
  display.setCursor(32,50);
  display.print(String((int)tempMin)+"/"+String((int)tempMax)+"\xf8""C");
  // Description
  display.setCursor(0,57); display.print(weatherDesc);
  drawPageDots(PAGE_WEATHER);
}

// PAGE 3 - POMODORO
void drawPomodoro(){
  display.setFont(NULL); display.setTextColor(SH110X_WHITE);
  if(!pomoLocked){
    display.setCursor(30,8);  display.print("POMODORO");
    display.setCursor(14,24); display.print("Long press to enter");
    display.setCursor(20,38); display.print("25min focus timer");
    display.setCursor(14,50); display.print("Tap:start Dbl:reset");
    drawPageDots(PAGE_POMO); return;
  }
  unsigned long now=millis();
  unsigned long sesLen=(pomoCount>0&&pomoCount%4==0&&pomoSession%2!=0)?POMO_LONG:
                       (pomoSession%2==0)?POMO_WORK:POMO_SHORT;
  unsigned long elapsed=pomoRunning?(now-pomoStart):pomoElapsed;
  if(pomoRunning&&elapsed>=sesLen){
    pomoFlashing=true; pomoFlashStart=now;
    pomoSession++; if(pomoSession%2==0) pomoCount++;
    pomoElapsed=0; pomoRunning=false;
    currentMood=(pomoSession%2==0)?MOOD_SUSPICIOUS:MOOD_HAPPY;
  }
  if(pomoFlashing){
    if((now-pomoFlashStart)%300<150) display.fillRect(0,0,128,64,SH110X_WHITE);
    if(now-pomoFlashStart>2000) pomoFlashing=false;
    return;
  }
  bool isFocus=(pomoSession%2==0);
  unsigned long rem=elapsed<sesLen?(sesLen-elapsed):0;
  // Header
  display.fillRect(0,0,128,14,SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(isFocus?40:28,3); display.print(isFocus?"FOCUS":"BREAK");
  // Tomato markers
  display.setCursor(100,3);
  for(int i=0;i<min(pomoCount%4,4);i++) display.print("o");
  display.setTextColor(SH110X_WHITE);
  // Timer
  display.setFont(&FreeSansBold18pt7b);
  char ts[6]; sprintf(ts,"%02lu:%02lu",rem/60000,(rem%60000)/1000);
  int16_t x1,y1; uint16_t w,h2;
  display.getTextBounds(ts,0,0,&x1,&y1,&w,&h2);
  display.setCursor((SCREEN_WIDTH-w)/2,44); display.print(ts);
  // Progress bar
  display.setFont(NULL);
  float prog=1.0-(float)rem/sesLen;
  display.drawRect(4,50,120,6,SH110X_WHITE);
  display.fillRect(4,50,(int)(prog*120),6,SH110X_WHITE);
  display.setCursor(pomoRunning?34:20,57);
  display.print(pomoRunning?"tap=pause":"tap=start  dbl=rst");
}

// PAGE 4 - WATER TRACKER
void drawWaterPage(){
  display.setFont(NULL); display.setTextColor(SH110X_WHITE);
  display.setCursor(32,2); display.print("HYDRATION");
  display.drawLine(0,11,128,11,SH110X_WHITE);
  // Bottle
  int bx=8,by=14,bw=30,bh=44;
  display.drawRect(bx+9,by,bw-18,7,SH110X_WHITE);   // neck
  display.drawRect(bx,by+7,bw,bh,SH110X_WHITE);      // body
  int fillH=(int)((float)waterCount/waterGoal*bh);
  if(fillH>0){
    display.fillRect(bx+1,by+7+bh-fillH,bw-2,fillH,SH110X_WHITE);
    for(int wy=by+7+bh-fillH;wy<by+7+bh;wy+=4)
      display.drawLine(bx+2,wy,bx+bw-3,wy,SH110X_BLACK);
  }
  // Percentage inside bottle
  int pct=(int)((float)waterCount/waterGoal*100);
  display.setTextColor(fillH>16?SH110X_BLACK:SH110X_WHITE);
  display.setCursor(bx+3,by+bh-4); display.print(String(pct)+"%");
  display.setTextColor(SH110X_WHITE);
  // Info right side
  display.setFont(&FreeSansBold9pt7b);
  display.setCursor(52,30);
  display.print(String(waterCount)+"/"+String(waterGoal));
  display.setFont(NULL);
  display.setCursor(52,36); display.print("glasses");
  display.setCursor(52,48);
  display.print(String(waterCount*0.25,1)+"/"+String(waterGoal*0.25,1)+"L");
  display.setCursor(36,58); display.print("tap = log a glass");
  drawPageDots(PAGE_WATER);
}

// PAGE 5 - ANALOG CLOCK
void drawAnalogClock(){
  struct tm t; display.setTextColor(SH110X_WHITE);
  if(!getLocalTime(&t)){ display.setFont(NULL); display.setCursor(30,30); display.print("Syncing..."); return; }
  int cx=44,cy=32,r=28;
  display.drawCircle(cx,cy,r,SH110X_WHITE);
  display.drawCircle(cx,cy,r-1,SH110X_WHITE);
  // Hour ticks
  for(int i=0;i<12;i++){
    float a=i*30.0*PI/180.0;
    display.drawLine(cx+(int)((r-3)*sin(a)),cy-(int)((r-3)*cos(a)),
                     cx+(int)((r-7)*sin(a)),cy-(int)((r-7)*cos(a)),SH110X_WHITE);
  }
  // Hands
  float ha=((t.tm_hour%12)+t.tm_min/60.0)*30.0*PI/180.0;
  float ma=t.tm_min*6.0*PI/180.0;
  float sa=t.tm_sec*6.0*PI/180.0;
  display.drawLine(cx,cy,cx+(int)(15*sin(ha)),cy-(int)(15*cos(ha)),SH110X_WHITE);
  display.drawLine(cx,cy,cx+(int)(22*sin(ma)),cy-(int)(22*cos(ma)),SH110X_WHITE);
  display.drawLine(cx,cy,cx+(int)(25*sin(sa)),cy-(int)(25*cos(sa)),SH110X_WHITE);
  display.fillCircle(cx,cy,2,SH110X_WHITE);
  // Digital right
  display.setFont(&FreeSansBold9pt7b);
  char ts[6]; sprintf(ts,"%02d:%02d",t.tm_hour,t.tm_min);
  display.setCursor(86,26); display.print(ts);
  display.setFont(NULL);
  char ds[12]; strftime(ds,12,"%a %d %b",&t);
  display.setCursor(82,34); display.print(ds);
  // Seconds bar
  display.drawRect(82,46,42,6,SH110X_WHITE);
  display.fillRect(82,46,(int)(t.tm_sec/59.0*42),6,SH110X_WHITE);
  drawPageDots(PAGE_ANALOG);
}

// PAGE 6 - MOVIE
void drawMoviePage(){
  display.setFont(NULL); display.setTextColor(SH110X_WHITE);
  display.fillRect(0,0,128,12,SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(18,2); display.print("WATCH TONIGHT");
  display.setTextColor(SH110X_WHITE);
  if(movieTitle.isEmpty()){
    display.setCursor(20,30); display.print("Loading..."); drawPageDots(PAGE_MOVIE); return;
  }
  // Title scroll
  int tw=movieTitle.length()*6;
  if(tw>SCREEN_WIDTH){
    display.setCursor(movieTitleSX,24); display.print(movieTitle);
    if(movieTitleSX+tw<SCREEN_WIDTH){ display.setCursor(movieTitleSX+tw+10,24); display.print(movieTitle); }
  } else {
    display.setCursor((SCREEN_WIDTH-tw)/2,24); display.print(movieTitle);
  }
  // Director scroll
  String dirStr="Dir: "+movieDirector;
  int dw=dirStr.length()*6;
  if(dw>SCREEN_WIDTH){
    display.setCursor(movieDirSX,36); display.print(dirStr);
    if(movieDirSX+dw<SCREEN_WIDTH){ display.setCursor(movieDirSX+dw+10,36); display.print(dirStr); }
  } else {
    display.setCursor((SCREEN_WIDTH-dw)/2,36); display.print(dirStr);
  }
  // Rating & year
  display.setFont(&FreeSans9pt7b);
  String info=movieYear+"  * "+movieRating;
  int16_t x1,y1; uint16_t w,h2;
  display.getTextBounds(info.c_str(),0,0,&x1,&y1,&w,&h2);
  display.setCursor((SCREEN_WIDTH-w)/2,52); display.print(info);
  display.setFont(NULL);
  display.setCursor(24,58); display.print("tap = next movie");
  drawPageDots(PAGE_MOVIE);
}

// PAGE 10 - SUN TRACKER
void drawSunTracker(){
  struct tm t; getLocalTime(&t);
  time_t now; time(&now);
  display.setFont(NULL); display.setTextColor(SH110X_WHITE);
  display.fillRect(0,0,128,12,SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(28,2); display.print("SUN TRACKER");
  display.setTextColor(SH110X_WHITE);
  bool isNight=(now<sunriseTime||now>sunsetTime);
  // Arc
  int acx=64,acy=54,ar=34;
  for(int deg=0;deg<=180;deg+=3){
    float rad=deg*PI/180.0;
    display.drawPixel(acx-(int)(ar*cos(rad)),acy-(int)(ar*sin(rad)),SH110X_WHITE);
  }
  display.drawLine(acx-ar-4,acy,acx+ar+4,acy,SH110X_WHITE);
  // Sun/Moon on arc
  float prog=0.5;
  if(!isNight&&sunsetTime>sunriseTime)
    prog=constrain((float)(now-sunriseTime)/(sunsetTime-sunriseTime),0.0,1.0);
  float sunRad=(180.0-prog*180.0)*PI/180.0;
  int sx=acx-(int)(ar*cos(sunRad));
  int sy=acy-(int)(ar*sin(sunRad));
  if(isNight){
    display.drawCircle(sx,sy,4,SH110X_WHITE);
    display.fillCircle(sx+2,sy-1,3,SH110X_BLACK);
  } else {
    display.fillCircle(sx,sy,4,SH110X_WHITE);
    display.drawLine(sx-7,sy,sx-5,sy,SH110X_WHITE);
    display.drawLine(sx+5,sy,sx+7,sy,SH110X_WHITE);
    display.drawLine(sx,sy-7,sx,sy-5,SH110X_WHITE);
  }
  // Times
  struct tm* sr=localtime(&sunriseTime); char srS[6]; sprintf(srS,"%02d:%02d",sr->tm_hour,sr->tm_min);
  struct tm* ss=localtime(&sunsetTime);  char ssS[6]; sprintf(ssS,"%02d:%02d",ss->tm_hour,ss->tm_min);
  display.setCursor(2,58); display.print(srS);
  display.setCursor(96,58); display.print(ssS);
  // Daylight remaining
  long rem=sunsetTime-now;
  display.setCursor(14,18);
  if(!isNight&&rem>0){
    char rs[16]; sprintf(rs,"%dh %dm left",(int)(rem/3600),(int)((rem%3600)/60));
    display.print(rs);
  } else {
    display.print(isNight?"Good night! :)":"Sunset very soon");
  }
  display.setCursor(28,57); // overwritten by sunrise but ok
}

// PAGE 11 - WITTY WEATHER
void drawWittyWeather(){
  display.setFont(NULL); display.setTextColor(SH110X_WHITE);
  display.fillRect(0,0,128,12,SH110X_WHITE);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(20,2); display.print("HOW'S OUTSIDE?");
  display.setTextColor(SH110X_WHITE);
  int temp=(int)temperature;
  String lines[3];
  if(weatherMain=="Thunderstorm"){
    lines[0]="Sky is having"; lines[1]="a moment out"; lines[2]="there. Stay in!";
  } else if(weatherMain=="Rain"||weatherMain=="Drizzle"){
    if(temp<15){lines[0]="Cold + rain ="; lines[1]="perfect excuse"; lines[2]="to stay inside.";}
    else       {lines[0]="Sky is crying"; lines[1]="again. You know"; lines[2]="what to do.";}
  } else if(weatherMain=="Snow"){
    lines[0]="It is snowing!"; lines[1]="Everything looks"; lines[2]="pretty. Stay warm.";
  } else if(weatherMain=="Mist"||weatherMain=="Fog"){
    lines[0]="Spooky fog day."; lines[1]="Cannot see much."; lines[2]="Drive carefully!";
  } else if(weatherMain=="Clear"){
    if(temp>38)     {lines[0]="You will melt."; lines[1]="Seriously stay"; lines[2]="inside today.";}
    else if(temp>32){lines[0]="Sun being extra"; lines[1]="today. Sunscreen"; lines[2]="is not optional.";}
    else if(temp>22){lines[0]="Actually perfect"; lines[1]="outside. Go touch"; lines[2]="some grass.";}
    else if(temp>15){lines[0]="Lovely outside!"; lines[1]="Light jacket and"; lines[2]="you are all set.";}
    else            {lines[0]="Clear but cold."; lines[1]="Sun lied to you"; lines[2]="today. Stay warm.";}
  } else if(weatherMain=="Clouds"){
    if(temp>25){lines[0]="Cloudy but warm."; lines[1]="Not bad honestly."; lines[2]="Could be worse.";}
    else       {lines[0]="Sun called in"; lines[1]="sick. Clouds on"; lines[2]="duty today.";}
  } else if(temp>35){
    lines[0]="Basically a"; lines[1]="sauna out there."; lines[2]="Hydrate or die.";
  } else if(temp<5){
    lines[0]="Blanket weather."; lines[1]="You are welcome"; lines[2]="to stay in bed.";
  } else {
    lines[0]="Weather is just"; lines[1]="being weather."; lines[2]="Nothing special.";
  }
  for(int i=0;i<3;i++){
    int lx=(SCREEN_WIDTH-(int)(lines[i].length()*6))/2;
    display.setCursor(lx,18+i*13); display.print(lines[i]);
  }
  display.setCursor(24,58); display.print("tap to go back");
}

// WATER POPUP OVERLAY
void drawWaterPopup(){
  display.fillRect(8,6,112,52,SH110X_BLACK);
  display.drawRect(8,6,112,52,SH110X_WHITE);
  display.drawRect(9,7,110,50,SH110X_WHITE);
  display.setFont(NULL); display.setTextColor(SH110X_WHITE);
  display.setCursor(26,11); display.print("DRINK WATER!");
  display.drawBitmap(16,22,bmp_tiny_drop,8,8,SH110X_WHITE);
  int pct=(int)((float)waterCount/waterGoal*100);
  display.setCursor(28,24); display.print(String(waterCount)+"/"+String(waterGoal)+" glasses");
  display.setCursor(28,34); display.print(String(pct)+"% of daily goal");
  display.drawLine(8,44,120,44,SH110X_WHITE);
  display.setCursor(14,47); display.print("tap=logged");
  display.setCursor(72,47); display.print("dbl=later");
}

// ==================================================
// BOOT ANIMATION
// ==================================================
void playBootAnimation(){
  display.setTextColor(SH110X_WHITE);
  int cx=64,cy=32;
  for(int r=0;r<80;r+=4){ display.clearDisplay(); display.fillCircle(cx,cy,r,SH110X_WHITE); display.display(); delay(8); }
  for(int r=0;r<80;r+=4){ display.clearDisplay(); display.fillCircle(cx,cy,80,SH110X_WHITE); display.fillCircle(cx,cy,r,SH110X_BLACK); display.display(); delay(8); }
  display.clearDisplay();
  display.setFont(&FreeSansBold9pt7b);
  String bt="DeskBuddy";
  int16_t x1,y1; uint16_t w,h2;
  display.getTextBounds(bt.c_str(),0,0,&x1,&y1,&w,&h2);
  display.setCursor((SCREEN_WIDTH-w)/2,30); display.print(bt);
  display.setFont(NULL); display.setCursor(22,42); display.print("by ESCLabs");
  display.display(); delay(2000);
}

// ==================================================
// SETUP
// ==================================================
void setup(){
  Wire.begin(SDA_PIN,SCL_PIN);
  pinMode(TOUCH_PIN,INPUT);
  analogReadResolution(12);
  display.begin(0x3C,true);
  display.setTextColor(SH110X_WHITE);

  bool forceConfig=false;
  for(unsigned long t=millis();millis()-t<CONFIG_HOLD_MS;){
    if(digitalRead(TOUCH_PIN)){forceConfig=true;break;} delay(80);
  }
  loadConfig();
  if(forceConfig){startConfigPortal();return;}

  leftEye.init(18,14,36,36); rightEye.init(74,14,36,36);
  playBootAnimation();

  display.clearDisplay(); display.setFont(NULL);
  display.setCursor(10,28); display.print("Connecting WiFi...");
  display.display();

  WiFi.begin(wifiSsid.c_str(),wifiPass.c_str());
  unsigned long ws=millis();
  while(WiFi.status()!=WL_CONNECTED&&millis()-ws<15000) delay(200);
  if(WiFi.status()!=WL_CONNECTED){startConfigPortal();return;}

  configTime(0,0,ntpServer);
  setenv("TZ",tzString.c_str(),1); tzset();
  delay(1000);

  getWeatherAndForecast();
  getQuote();
  getMovie();

  lastActivityTime=lastWaterReminder=lastPageSwitch=millis();
}

// ==================================================
// LOOP
// ==================================================
void loop(){
  if(inConfigMode){configServer.handleClient();return;}

  unsigned long now=millis();
  updateMic();
  handleTouch();

  // Weather every 10 min
  if(now-lastWeatherUpdate>600000) getWeatherAndForecast();

  // Quote at noon and midnight
  struct tm t; getLocalTime(&t);
  if((t.tm_hour==0||t.tm_hour==12)&&t.tm_hour!=lastQuoteHour){
    getQuote(); lastQuoteHour=t.tm_hour;
  }

  // Movie at midnight
  if(t.tm_hour==0&&t.tm_min==0&&t.tm_mday!=lastMovieDay){
    getMovie(); lastMovieDay=t.tm_mday;
  }

  // Water reminder (not during focus session)
  bool pomoFocus=pomoLocked&&pomoRunning&&pomoSession%2==0;
  if(!pomoFocus&&!showWaterPopup&&waterCount<waterGoal&&
     now-lastWaterReminder>WATER_INTERVAL){
    showWaterPopup=true; waterPopupShown=now; lastWaterReminder=now;
  }
  if(showWaterPopup&&now-waterPopupShown>WATER_POPUP_SHOW) showWaterPopup=false;

  // Auto page rotation
  if(currentPage<TOTAL_MAIN&&!pomoLocked&&!showWaterPopup&&
     now-lastPageSwitch>PAGE_INTERVAL){
    currentPage++; if(currentPage>=TOTAL_MAIN) currentPage=0;
    lastPageSwitch=now; lastSaccade=0;
  }

  // Mood updates
  if(currentPage==PAGE_EYES&&!micMuted) updateMoodFromMic();
  if(pomoLocked&&pomoRunning) currentMood=(pomoSession%2==0)?MOOD_SUSPICIOUS:MOOD_HAPPY;

  // Sleep
  if(!displayOff){
    if(now-lastActivityTime>SLEEP_OFF_MS){ display.setContrast(0); displayOff=true; return; }
    if(now-lastActivityTime>SLEEP_MOOD_MS&&currentPage==PAGE_EYES) currentMood=MOOD_SLEEPY;
  }

  // Quote scroll
  if(now-lastQuoteScroll>60){
    quoteScrollX--;
    String q="\""+currentQuote+"\" - "+currentAuthor+"   ";
    if(quoteScrollX<-(int)(q.length()*6)) quoteScrollX=SCREEN_WIDTH;
    lastQuoteScroll=now;
  }

  // Movie scroll
  if(now-lastMovieScroll>80){
    int tw=movieTitle.length()*6, dw=(movieDirector.length()+5)*6;
    if(tw>SCREEN_WIDTH){ movieTitleSX--; if(movieTitleSX<-tw) movieTitleSX=SCREEN_WIDTH; }
    if(dw>SCREEN_WIDTH){ movieDirSX--;   if(movieDirSX<-dw)   movieDirSX=SCREEN_WIDTH;  }
    lastMovieScroll=now;
  }

  // Draw
  display.clearDisplay();
  if(showWaterPopup){
    // draw current page behind popup
    switch(currentPage){
      case PAGE_CLOCK:   drawClock();       break;
      case PAGE_WEATHER: drawWeatherCard(); break;
      case PAGE_EYES:    drawEmoPage();     break;
      default: break;
    }
    drawWaterPopup();
  } else {
    switch(currentPage){
      case PAGE_EYES:    drawEmoPage();       break;
      case PAGE_CLOCK:   drawClock();         break;
      case PAGE_WEATHER: drawWeatherCard();   break;
      case PAGE_POMO:    drawPomodoro();      break;
      case PAGE_WATER:   drawWaterPage();     break;
      case PAGE_ANALOG:  drawAnalogClock();   break;
      case PAGE_MOVIE:   drawMoviePage();     break;
      case PAGE_SUN:     drawSunTracker();    break;
      case PAGE_WITTY:   drawWittyWeather();  break;
    }
  }
  display.display();
}
