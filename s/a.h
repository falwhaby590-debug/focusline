<!DOCTYPE html>
<html lang="ar" dir="rtl">
<head>
<meta charset="UTF-8"/>
<meta name="viewport" content="width=device-width,initial-scale=1.0"/>
<link rel="manifest" href="manifest.json">
<meta name="theme-color" content="#0a0f1e">
<link rel="apple-touch-icon" href="icon-192.png">
<title>مساري - نظام إدارة الحياة</title>
<meta charset="UTF-8"/>
<meta name="viewport" content="width=device-width,initial-scale=1.0"/>
<title>مساري</title>
<link href="https://fonts.googleapis.com/css2?family=Tajawal:wght@400;500;700;800;900&display=swap" rel="stylesheet"/>
<style>
*,*::before,*::after{box-sizing:border-box;margin:0;padding:0}
body{font-family:'Tajawal',sans-serif;direction:rtl;min-height:100vh;-webkit-font-smoothing:antialiased}
body.dark{background:#0a0a0e;color:#eeeef2}
body.light{background:#f2f4f7;color:#111114}
input,select,textarea,button{font-family:'Tajawal',sans-serif}
::-webkit-scrollbar{width:4px;height:4px}
::-webkit-scrollbar-thumb{background:rgba(128,128,128,.2);border-radius:99px}
input[type=time]::-webkit-calendar-picker-indicator,
input[type=date]::-webkit-calendar-picker-indicator{filter:invert(.5);cursor:pointer}
.tcard{transition:transform .15s}
.tcard:hover{transform:translateY(-1px)}
button:focus-visible{outline:2px solid #4ade80;outline-offset:2px}
input:focus,textarea:focus,select:focus{border-color:rgba(128,128,128,.35)!important}

/* ── layout ── */
#header{display:flex;align-items:center;justify-content:space-between;padding:13px 18px;position:sticky;top:0;z-index:100;backdrop-filter:blur(18px)}
body.dark  #header{background:rgba(10,10,14,.9);border-bottom:1px solid rgba(255,255,255,.07)}
body.light #header{background:rgba(242,244,247,.92);border-bottom:1px solid rgba(0,0,0,.08)}
#nav{display:flex;gap:0;padding:0 10px;overflow-x:auto;position:sticky;top:56px;z-index:99;scrollbar-width:none;-ms-overflow-style:none}
#nav::-webkit-scrollbar{display:none}
body.dark  #nav{background:rgba(13,13,18,.88);border-bottom:1px solid rgba(255,255,255,.06)}
body.light #nav{background:rgba(235,237,242,.95);border-bottom:1px solid rgba(0,0,0,.07)}
#main{max-width:680px;margin:0 auto;padding:16px 13px 90px}

/* ── cards ── */
.card{border-radius:14px;padding:14px;margin-bottom:10px}
body.dark  .card{background:#131318;border:1px solid rgba(255,255,255,.07)}
body.light .card{background:#fff;border:1px solid rgba(0,0,0,.09)}
.card2{border-radius:14px;padding:13px}
body.dark  .card2{background:#1a1a22;border:1px solid rgba(255,255,255,.06)}
body.light .card2{background:#f8f9fb;border:1px solid rgba(0,0,0,.08)}

/* ── buttons ── */
.btn-g{background:linear-gradient(135deg,#4ade80,#16a34a);border:none;color:#000;padding:10px 22px;border-radius:10px;font-size:14px;font-weight:800;cursor:pointer;width:100%}
.btn-ghost{border-radius:9px;padding:8px 18px;font-size:13px;cursor:pointer;font-weight:600}
body.dark  .btn-ghost{background:#1a1a22;border:1px solid rgba(255,255,255,.1);color:#ccc}
body.light .btn-ghost{background:#ececf0;border:1px solid rgba(0,0,0,.1);color:#444}
.btn-danger{background:rgba(248,113,113,.12);border:1px solid rgba(248,113,113,.3);color:#f87171;padding:9px 18px;border-radius:9px;font-size:13px;font-weight:700;cursor:pointer}

/* ── nav buttons ── */
.nb{display:flex;align-items:center;gap:5px;padding:9px 13px;border:none;cursor:pointer;font-size:13px;font-weight:700;white-space:nowrap;border-bottom:2px solid transparent;background:none;transition:all .18s}
body.dark  .nb{color:rgba(255,255,255,.32)}
body.light .nb{color:rgba(0,0,0,.38)}
.nb.active{border-bottom:2px solid #4ade80}
body.dark  .nb.active{color:#4ade80;background:rgba(74,222,128,.08)}
body.light .nb.active{color:#16a34a;background:rgba(22,163,74,.07);border-bottom-color:#16a34a}

/* ── inputs ── */
.inp{border-radius:9px;padding:9px 11px;font-size:13px;outline:none;width:100%}
body.dark  .inp{background:#1a1a22;border:1px solid rgba(255,255,255,.11);color:#eeeef2}
body.light .inp{background:#f0f1f5;border:1px solid rgba(0,0,0,.12);color:#111114}
.sel{border-radius:9px;padding:9px 10px;font-size:13px;outline:none}
body.dark  .sel{background:#1a1a22;border:1px solid rgba(255,255,255,.11);color:#eeeef2}
body.light .sel{background:#f0f1f5;border:1px solid rgba(0,0,0,.12);color:#111114}

/* ── task card ── */
.tcard{display:flex;align-items:flex-start;gap:8px;border-radius:12px;padding:11px;margin-bottom:6px;position:relative;overflow:hidden}
body.dark  .tcard{background:#131318;border:1px solid rgba(255,255,255,.07)}
body.light .tcard{background:#fff;border:1px solid rgba(0,0,0,.09)}
.tcard.done{opacity:.55}
.tcard.done .ttitle{text-decoration:line-through;color:#64748b!important}

/* ── modal ── */
.modal-ov{position:fixed;inset:0;background:rgba(0,0,0,.72);z-index:400;display:flex;align-items:flex-end;justify-content:center;backdrop-filter:blur(8px)}
.modal-box{border-radius:18px 18px 0 0;width:100%;max-width:540px;max-height:88vh;overflow-y:auto}
body.dark  .modal-box{background:#131318;border:1px solid rgba(255,255,255,.08)}
body.light .modal-box{background:#fff;border:1px solid rgba(0,0,0,.1)}
.modal-hdr{display:flex;justify-content:space-between;align-items:center;padding:14px 18px;position:sticky;top:0}
body.dark  .modal-hdr{background:#131318;border-bottom:1px solid rgba(255,255,255,.07)}
body.light .modal-hdr{background:#fff;border-bottom:1px solid rgba(0,0,0,.07)}
.modal-body{padding:18px;display:flex;flex-direction:column;gap:13px}

/* ── focus ── */
.focus-ov{position:fixed;inset:0;background:rgba(0,0,0,.95);z-index:500;display:flex;align-items:center;justify-content:center;padding:20px;backdrop-filter:blur(24px)}
.focus-box{background:#0f0f14;border:1px solid rgba(74,222,128,.15);border-radius:22px;padding:38px 28px;text-align:center;max-width:360px;width:100%;position:relative}

/* ── toast ── */
.toast{position:fixed;bottom:22px;left:50%;transform:translateX(-50%);color:#000;font-weight:700;font-size:13px;padding:9px 22px;border-radius:12px;z-index:999;pointer-events:none;white-space:nowrap;box-shadow:0 4px 24px rgba(0,0,0,.3);animation:slideUp .22s ease}
@keyframes slideUp{from{opacity:0;transform:translateX(-50%) translateY(8px)}to{opacity:1;transform:translateX(-50%) translateY(0)}}

/* ── misc ── */
.sec-title{font-size:17px;font-weight:800;margin-bottom:14px;letter-spacing:-.3px}
.muted{color:rgba(128,128,128,.7)}
body.light .muted{color:rgba(0,0,0,.42)}
.badge{background:#f87171;color:#fff;font-size:10px;border-radius:99px;padding:1px 6px;font-weight:700}
.chip{padding:4px 11px;border-radius:99px;font-size:12px;cursor:pointer;font-weight:700;border:1px solid;transition:all .15s}
.prog-track{height:5px;border-radius:99px;overflow:hidden}
body.dark  .prog-track{background:rgba(255,255,255,.07)}
body.light .prog-track{background:rgba(0,0,0,.08)}
.sep{border:none;border-bottom:1px solid}
body.dark  .sep{border-color:rgba(255,255,255,.06)}
body.light .sep{border-color:rgba(0,0,0,.07)}
.srow{display:flex;align-items:center;justify-content:space-between;padding:11px 0;border-bottom:1px solid}
body.dark  .srow{border-color:rgba(255,255,255,.06)}
body.light .srow{border-color:rgba(0,0,0,.07)}
.toggle-wrap{width:46px;height:26px;border-radius:13px;position:relative;cursor:pointer;transition:background .3s;flex-shrink:0}
.toggle-knob{position:absolute;top:3px;width:20px;height:20px;border-radius:50%;transition:right .3s;box-shadow:0 1px 3px rgba(0,0,0,.3)}
.empty-state{text-align:center;padding:40px 0;color:rgba(128,128,128,.6)}
@media(max-width:480px){#header{padding:11px 13px}#main{padding:13px 10px 85px}.focus-box{padding:28px 18px}}
</style>
</head>
<body class="dark">

<div id="header">
  <div style="display:flex;align-items:center;gap:11px">
    <div style="display:flex;align-items:center;gap:6px">
      <span id="acc-icon" style="font-size:21px;line-height:1">◈</span>
      <span style="font-size:19px;font-weight:900;letter-spacing:-.5px">مساري</span>
    </div>
    <span id="today-chip" style="padding:3px 10px;border-radius:99px;font-size:12px;font-weight:700"></span>
  </div>
  <div style="display:flex;align-items:center;gap:7px">
    <button onclick="toggleTheme()" style="background:none;border:none;cursor:pointer;font-size:19px;padding:4px 7px;border-radius:8px" id="theme-btn">☀️</button>
    <button onclick="showEmergency()" class="btn-danger" style="padding:5px 12px;font-size:12px">⚡ طوارئ</button>
  </div>
</div>

<nav id="nav">
  <button class="nb active" data-view="today"     onclick="switchView('today')">    <span>◎</span><span>اليوم</span></button>
  <button class="nb"        data-view="week"      onclick="switchView('week')">     <span>▦</span><span>الأسبوع</span></button>
  <button class="nb"        data-view="habits"    onclick="switchView('habits')">   <span>🔄</span><span>العادات</span></button>
  <button class="nb"        data-view="postponed" onclick="switchView('postponed')"><span>⏳</span><span>المؤجلات</span><span class="badge" id="post-badge" style="display:none">0</span></button>
  <button class="nb"        data-view="stats"     onclick="switchView('stats')">    <span>◈</span><span>الإحصائيات</span></button>
  <button class="nb"        data-view="settings"  onclick="switchView('settings')"> <span>⚙</span><span>الإعدادات</span></button>
</nav>

<div id="main"></div>
<div id="toast-wrap"></div>

<script>
// ══════════ CONSTANTS ══════════
var DAYS=["الأحد","الاثنين","الثلاثاء","الأربعاء","الخميس","الجمعة","السبت"];
var PERIODS=["الفجر","الصباح","الظهر","العصر","الليل"];
var ASPECTS=[
  {id:"faith",  label:"الإيماني", color:"#a78bfa"},
  {id:"science",label:"العلمي",   color:"#60a5fa"},
  {id:"skill",  label:"المهاري",  color:"#34d399"},
  {id:"culture",label:"الثقافي",  color:"#fbbf24"},
  {id:"finance",label:"المالي",   color:"#f87171"},
  {id:"sport",  label:"الرياضي",  color:"#fb923c"},
  {id:"other",  label:"أخرى",     color:"#94a3b8"}
];
var PRIS=[
  {id:"critical", label:"مهم جدًا",color:"#f87171"},
  {id:"high",     label:"مهم",      color:"#fb923c"},
  {id:"normal",   label:"عادي",     color:"#60a5fa"},
  {id:"optional", label:"اختياري", color:"#94a3b8"}
];
var ENERGY=[{id:"high",label:"عالية",icon:"⚡"},{id:"medium",label:"متوسطة",icon:"🔋"},{id:"low",label:"منخفضة",icon:"😴"}];

// ══════════ HELPERS ══════════
function uid(){return Math.random().toString(36).slice(2,10)}
function todayStr(){var d=new Date();return d.getFullYear()+"-"+pad(d.getMonth()+1)+"-"+pad(d.getDate())}
function pad(n){return String(n).padStart(2,"0")}
function addDays(s,n){var d=new Date(s);d.setDate(d.getDate()+n);return d.getFullYear()+"-"+pad(d.getMonth()+1)+"-"+pad(d.getDate())}
function dateLabel(s){var d=new Date(s+"T00:00:00");return DAYS[d.getDay()]+" "+d.getDate()+"/"+(d.getMonth()+1)}
function calcPct(tasks){if(!tasks||!tasks.length)return 0;return Math.round(tasks.filter(function(t){return t.done}).length/tasks.length*100)}
function pctColor(p){return p>=75?"#4ade80":p>=40?"#facc15":"#f87171"}
function endTime(s,dur){if(!s||!dur)return"";var p=s.split(":").map(Number),t=p[0]*60+p[1]+dur;return pad(Math.floor(t/60)%24)+":"+pad(t%60)}
function fmtTimer(s){return pad(Math.floor(s/60))+":"+pad(s%60)}
function isLight(){return document.body.classList.contains("light")}
function acc(){return isLight()?"#16a34a":"#4ade80"}
function weekStart(s){var d=new Date(s+"T00:00:00");d.setDate(d.getDate()-d.getDay());return d.getFullYear()+"-"+pad(d.getMonth()+1)+"-"+pad(d.getDate())}

// ══════════ STATE ══════════
var ST;
try{ST=JSON.parse(localStorage.getItem("masari_v6"))||null;}catch(e){ST=null;}
if(!ST){
  ST={
    tasks:{},
    postponed:[],
    habits:[
      {id:uid(),title:"أذكار الصباح",  aspect:"faith",  time:"06:00",duration:15,period:"الفجر", priority:"critical",streak:0,log:{}},
      {id:uid(),title:"قراءة القرآن",   aspect:"faith",  time:"07:00",duration:30,period:"الصباح",priority:"critical",streak:0,log:{}},
      {id:uid(),title:"تمرين رياضي",   aspect:"sport",  time:"17:00",duration:45,period:"العصر", priority:"normal",  streak:0,log:{}},
      {id:uid(),title:"قراءة كتاب",    aspect:"culture",time:"21:00",duration:30,period:"الليل", priority:"normal",  streak:0,log:{}}
    ],
    reviews:{},energy:{},
    settings:{theme:"dark",notifs:true,notifyBefore:15}
  };
  // inject habits for today
  injectHabitsForDate(todayStr());
}

function saveState(){localStorage.setItem("masari_v6",JSON.stringify(ST))}

// inject habits as tasks into a given date (skip if already present)
function injectHabitsForDate(dateStr){
  if(!ST.tasks[dateStr]) ST.tasks[dateStr]=[];
  ST.habits.forEach(function(h){
    var exists=ST.tasks[dateStr].some(function(t){return t.habitId===h.id});
    if(!exists){
      ST.tasks[dateStr].push({
        id:uid(), title:h.title, done:false, priority:h.priority,
        aspect:h.aspect, period:h.period, time:h.time, duration:h.duration,
        note:"", subtasks:[], pinned:true, isHabit:true, habitId:h.id
      });
    }
  });
}

// call inject when switching to any date
function ensureDate(dateStr){
  injectHabitsForDate(dateStr);
}

// ══════════ UI STATE ══════════
var currentView="today";
var selDate=todayStr();
var filterAsp=null;
var searchQ="";
var focusInterval=null;

// ══════════ BOOT ══════════
applyTheme();
ensureDate(selDate);
renderHeader();
renderAll();

function applyTheme(){
  var t=ST.settings.theme;
  document.body.className=t;
  var btn=document.getElementById("theme-btn");
  if(btn) btn.textContent=t==="dark"?"☀️":"🌙";
  var icon=document.getElementById("acc-icon");
  if(icon) icon.style.color=acc();
}

function renderHeader(){
  var chip=document.getElementById("today-chip");
  if(chip){
    chip.textContent=dateLabel(todayStr());
    chip.style.background=acc()+"18";
    chip.style.border="1px solid "+acc()+"33";
    chip.style.color=acc();
  }
  var badge=document.getElementById("post-badge");
  if(badge){
    var n=(ST.postponed||[]).length;
    badge.style.display=n>0?"inline":"none";
    badge.textContent=n;
  }
}

function switchView(v){
  currentView=v;
  document.querySelectorAll(".nb").forEach(function(b){
    b.classList.toggle("active",b.getAttribute("data-view")===v);
  });
  if(v==="today") ensureDate(selDate);
  renderAll();
}

function renderAll(){
  renderHeader();
  var main=document.getElementById("main");
  if(!main) return;
  main.innerHTML="";
  if(currentView==="today")     main.appendChild(buildToday());
  else if(currentView==="week") main.appendChild(buildWeek());
  else if(currentView==="habits")    main.appendChild(buildHabits());
  else if(currentView==="postponed") main.appendChild(buildPostponed());
  else if(currentView==="stats")     main.appendChild(buildStats());
  else if(currentView==="settings")  main.appendChild(buildSettings());
}

// ══════════ TODAY ══════════
function buildToday(){
  var wrap=mk("div");
  var tasks=ST.tasks[selDate]||[];
  var priOrd={critical:0,high:1,normal:2,optional:3};
  var filtered=tasks
    .filter(function(t){return(!filterAsp||t.aspect===filterAsp)&&(!searchQ||t.title.indexOf(searchQ)>=0)})
    .sort(function(a,b){return(priOrd[a.priority]||2)-(priOrd[b.priority]||2)});
  var pct=calcPct(tasks);
  var done=tasks.filter(function(t){return t.done}).length;

  // ── date nav ──
  var dnav=mk("div","display:flex;align-items:center;justify-content:space-between;margin-bottom:14px");
  var prev=mkBtn("‹","background:none;border:1px solid rgba(128,128,128,.2);border-radius:8px;width:32px;height:32px;cursor:pointer;font-size:18px;",function(){
    selDate=addDays(selDate,-1);ensureDate(selDate);filterAsp=null;renderAll();
  });
  var dateEl=mk("div","font-size:15px;font-weight:800;text-align:center");
  dateEl.textContent=dateLabel(selDate)+(selDate===todayStr()?" · اليوم":"");
  dateEl.style.color=selDate===todayStr()?acc():"";
  var next=mkBtn("›","background:none;border:1px solid rgba(128,128,128,.2);border-radius:8px;width:32px;height:32px;cursor:pointer;font-size:18px;",function(){
    selDate=addDays(selDate,1);ensureDate(selDate);filterAsp=null;renderAll();
  });
  dnav.appendChild(prev);dnav.appendChild(dateEl);dnav.appendChild(next);
  wrap.appendChild(dnav);

  // ── stats bar ──
  var bar=mk("div","display:grid;grid-template-columns:repeat(4,1fr);gap:7px;margin-bottom:13px");
  [{label:"الإنجاز",v:pct+"%",c:pctColor(pct)},{label:"المهام",v:done+"/"+tasks.length,c:""},{label:"مؤجلات",v:(ST.postponed||[]).length,c:"#f87171"},{label:"عادات",v:tasks.filter(function(t){return t.isHabit&&t.done}).length+"/"+tasks.filter(function(t){return t.isHabit}).length,c:acc()}].forEach(function(s){
    var c=mk("div","text-align:center;padding:11px 7px;border-radius:12px");
    c.className="card2";
    var v=mk("div","font-size:19px;font-weight:900;line-height:1;margin-bottom:2px");v.textContent=s.v;if(s.c)v.style.color=s.c;
    var l=mk("div","font-size:10px");l.textContent=s.label;l.className="muted";
    c.appendChild(v);c.appendChild(l);bar.appendChild(c);
  });
  wrap.appendChild(bar);

  // ── progress ──
  var pw=mk("div","display:flex;align-items:center;gap:10px;margin-bottom:13px");
  var pt=mk("div","flex:1");pt.className="prog-track";
  var pf=mk("div","height:100%;border-radius:99px;transition:width .6s");pf.style.width=pct+"%";pf.style.background=pctColor(pct);
  pt.appendChild(pf);
  var pl=mk("span","font-size:12px;white-space:nowrap");pl.textContent=done+" / "+tasks.length;pl.className="muted";
  pw.appendChild(pt);pw.appendChild(pl);wrap.appendChild(pw);

  // ── energy + review ──
  var topR=mk("div","display:flex;gap:8px;margin-bottom:13px;flex-wrap:wrap");
  var ec=mk("div","flex:1;min-width:0");ec.className="card2";
  var elb=mk("div","font-size:10px;font-weight:700;letter-spacing:1px;margin-bottom:7px");elb.textContent="مستوى الطاقة";elb.className="muted";
  ec.appendChild(elb);
  var er=mk("div","display:flex;gap:5px;flex-wrap:wrap");
  ENERGY.forEach(function(e){
    var active=ST.energy[selDate]===e.id;var a=acc();
    var b=mkBtn(e.icon+" "+e.label,"padding:4px 9px;border-radius:7px;font-size:11px;cursor:pointer;font-weight:700;transition:all .18s;",function(){
      ST.energy[selDate]=e.id;saveState();renderAll();
    });
    b.style.background=active?a+"22":"transparent";
    b.style.border=active?"1px solid "+a+"55":"1px solid rgba(128,128,128,.2)";
    b.style.color=active?a:"";
    er.appendChild(b);
  });
  ec.appendChild(er);topR.appendChild(ec);
  var rb=mkBtn("📝 مراجعة","padding:10px 13px;border-radius:12px;cursor:pointer;font-size:13px;font-weight:700;white-space:nowrap;",showReviewModal);
  rb.style.background=acc()+"15";rb.style.border="1px solid "+acc()+"33";rb.style.color=acc();
  topR.appendChild(rb);wrap.appendChild(topR);

  // ── aspect filter ──
  var ar=mk("div","display:flex;gap:5px;flex-wrap:wrap;margin-bottom:11px");
  var allB=mkBtn("الكل","",function(){filterAsp=null;renderAll();});
  allB.className="chip";
  allB.style.background=!filterAsp?acc()+"20":"transparent";
  allB.style.borderColor=!filterAsp?acc()+"55":"rgba(128,128,128,.2)";
  allB.style.color=!filterAsp?acc():"rgba(128,128,128,.6)";
  ar.appendChild(allB);
  ASPECTS.forEach(function(a){
    var active=filterAsp===a.id;
    var b=mkBtn(a.label,"",function(){filterAsp=filterAsp===a.id?null:a.id;renderAll();});
    b.className="chip";
    b.style.background=active?a.color+"20":"transparent";
    b.style.borderColor=active?a.color+"55":"rgba(128,128,128,.2)";
    b.style.color=active?a.color:"rgba(128,128,128,.6)";
    ar.appendChild(b);
  });
  wrap.appendChild(ar);

  // ── search ──
  var sw=mk("div","display:flex;align-items:center;gap:8px;padding:8px 11px;border-radius:10px;margin-bottom:13px");
  sw.className="card2";
  sw.appendChild(mk("span","font-size:13px;opacity:.45",["🔍"]));
  var si=mk("input","background:none;border:none;outline:none;font-size:13px;flex:1;width:100%");
  si.type="text";si.placeholder="بحث في المهام...";si.value=searchQ;si.style.color=isLight()?"#111":"#eee";
  si.oninput=function(){searchQ=this.value;renderAll();};
  sw.appendChild(si);wrap.appendChild(sw);

  // ── tasks by period ──
  var hasTasks=false;
  PERIODS.forEach(function(period){
    var pts=filtered.filter(function(t){return t.period===period});
    if(!pts.length)return;
    hasTasks=true;
    var sec=mk("div","margin-bottom:13px");
    var pl=mk("div","font-size:10px;font-weight:700;letter-spacing:2px;margin-bottom:6px");
    pl.textContent=period;pl.className="muted";
    sec.appendChild(pl);
    pts.forEach(function(t){sec.appendChild(buildTaskCard(t));});
    wrap.appendChild(sec);
  });
  filtered.filter(function(t){return!t.period||PERIODS.indexOf(t.period)<0}).forEach(function(t){
    hasTasks=true;wrap.appendChild(buildTaskCard(t));
  });

  if(!hasTasks){
    var em=mk("div");em.className="empty-state";
    em.innerHTML="<div style='font-size:44px;margin-bottom:9px'>✨</div><div style='font-size:14px;font-weight:600'>لا توجد مهام اليوم</div>";
    wrap.appendChild(em);
  }

  // ── add task ──
  var a=acc();
  var ab=mkBtn("+ إضافة مهمة","width:100%;padding:12px;border-radius:12px;font-size:14px;font-weight:700;cursor:pointer;margin-top:5px;transition:all .18s;",function(){showTaskForm(null);});
  ab.style.background=a+"10";ab.style.border="1px dashed "+a+"44";ab.style.color=a;
  wrap.appendChild(ab);

  if((ST.postponed||[]).length>0){
    var bn=mkBtn("⏳ لديك "+ST.postponed.length+" مهمة مؤجلة","width:100%;margin-top:9px;padding:10px;border-radius:10px;font-size:13px;cursor:pointer;",function(){switchView("postponed");});
    bn.style.background="rgba(248,113,113,.08)";bn.style.border="1px solid rgba(248,113,113,.2)";bn.style.color="#f87171";
    wrap.appendChild(bn);
  }
  return wrap;
}

// ══════════ TASK CARD ══════════
function buildTaskCard(task){
  var pri=PRIS.find(function(p){return p.id===task.priority})||PRIS[2];
  var asp=ASPECTS.find(function(a){return a.id===task.aspect});
  var card=mk("div");card.className="tcard"+(task.done?" done":"");
  card.setAttribute("data-id",task.id);

  // priority stripe
  var stripe=mk("div","position:absolute;right:0;top:0;bottom:0;width:3px;border-radius:0 12px 12px 0");
  stripe.style.background=pri.color;card.appendChild(stripe);

  // checkbox
  var chk=mk("div","width:22px;height:22px;min-width:22px;border-radius:6px;display:flex;align-items:center;justify-content:center;cursor:pointer;font-size:12px;font-weight:800;flex-shrink:0;margin-top:1px;transition:all .2s");
  chk.style.border=task.done?"2px solid "+acc():"2px solid rgba(128,128,128,.3)";
  chk.style.background=task.done?acc():"transparent";
  chk.style.color=task.done?"#000":"";
  chk.textContent=task.done?"✓":"";
  chk.onclick=function(){toggleTask(task.id);};
  card.appendChild(chk);

  // content
  var content=mk("div","flex:1;min-width:0");
  var tr=mk("div","display:flex;align-items:center;gap:7px");
  var title=mk("span","cursor:pointer;font-weight:700;font-size:14px;line-height:1.4");
  title.className="ttitle";
  if(task.isHabit) title.textContent="🔄 "+task.title;
  else if(task.pinned) title.textContent="📌 "+task.title;
  else title.textContent=task.title;
  title.onclick=function(){
    var sub=card.querySelector(".subtasks");
    if(sub) sub.style.display=sub.style.display==="none"?"block":"none";
  };
  tr.appendChild(title);
  if(task.priority==="critical"){var dot=mk("div","width:5px;height:5px;border-radius:50%;background:#f87171;flex-shrink:0;margin-top:6px");tr.appendChild(dot);}
  content.appendChild(tr);

  // meta
  var meta=[];
  if(task.time){var et=endTime(task.time,task.duration);meta.push({t:"🕐 "+task.time+(et?" → "+et:""),c:""});}
  if(task.duration) meta.push({t:"⏱ "+task.duration+"د",c:""});
  if(asp) meta.push({t:"● "+asp.label,c:asp.color});
  if(meta.length){
    var mr=mk("div","display:flex;gap:9px;flex-wrap:wrap;margin-top:4px;font-size:11px");
    meta.forEach(function(m){var s=mk("span");s.textContent=m.t;s.className="muted";if(m.c)s.style.color=m.c;mr.appendChild(s);});
    content.appendChild(mr);
  }

  // subtasks
  if(task.subtasks&&task.subtasks.length){
    var sw=mk("div","margin-top:7px;padding-top:7px");sw.className="subtasks sep";sw.style.display="none";
    task.subtasks.forEach(function(s){
      var sr=mk("div","display:flex;align-items:center;gap:6px;margin-bottom:4px;cursor:pointer");
      var sc=mk("span","font-size:12px");sc.textContent=s.done?"✓":"○";sc.style.color=s.done?acc():"rgba(128,128,128,.5)";
      var st=mk("span","font-size:12px");st.textContent=s.title;st.className="muted";
      st.style.textDecoration=s.done?"line-through":"none";
      sr.appendChild(sc);sr.appendChild(st);
      sr.onclick=(function(sid){return function(){toggleSubtask(task.id,sid);};})(s.id);
      sw.appendChild(sr);
    });
    content.appendChild(sw);
  }
  card.appendChild(content);

  // actions
  var acts=mk("div","display:flex;flex-direction:column;gap:2px;flex-shrink:0");
  function actBtn(ic,fn){
    var b=mkBtn(ic,"background:none;border:none;cursor:pointer;font-size:13px;padding:3px 5px;border-radius:5px;",fn);
    b.className="muted";return b;
  }
  acts.appendChild(actBtn("⏱",function(){openFocus(task);}));
  acts.appendChild(actBtn("✏",function(){showTaskForm(task);}));
  acts.appendChild(actBtn("⏳",function(){postponeTask(task.id);}));
  acts.appendChild(actBtn("↗",function(){showMoveModal(task.id);}));
  var db=actBtn("✕",function(){deleteTask(task.id);});db.style.color="#f87171";
  acts.appendChild(db);
  card.appendChild(acts);
  return card;
}

// ══════════ HABITS VIEW ══════════
function buildHabits(){
  var wrap=mk("div");
  var th=mk("div");th.className="sec-title";th.textContent="العادات والروتين 🔄";wrap.appendChild(th);

  var info=mk("div","font-size:12px;margin-bottom:16px;padding:10px 13px;border-radius:10px;line-height:1.7");
  info.className="card2 muted";
  info.textContent="كل عادة تُضاف تلقائياً كمهمة في بداية كل يوم.";
  wrap.appendChild(info);

  if(!(ST.habits||[]).length){
    var em=mk("div");em.className="empty-state";em.innerHTML="<div style='font-size:40px;margin-bottom:8px'>🔄</div><div>لا توجد عادات بعد</div>";wrap.appendChild(em);
  }

  // habit list
  var habitList=mk("div");wrap.appendChild(habitList);

  function renderHabits(){
    habitList.innerHTML="";
    var td=todayStr();
    (ST.habits||[]).forEach(function(h){
      var asp=ASPECTS.find(function(a){return a.id===h.aspect});
      var doneToday=!!h.log[td];
      var c=mk("div");c.className="card";

      var top=mk("div","display:flex;align-items:center;gap:10px;margin-bottom:7px");
      // done toggle
      var chk=mk("div","width:24px;height:24px;min-width:24px;border-radius:7px;display:flex;align-items:center;justify-content:center;cursor:pointer;font-size:13px;font-weight:800;flex-shrink:0;transition:all .2s");
      chk.style.border=doneToday?"2px solid "+acc():"2px solid rgba(128,128,128,.3)";
      chk.style.background=doneToday?acc():"transparent";
      chk.style.color=doneToday?"#000":"";
      chk.textContent=doneToday?"✓":"";
      chk.onclick=(function(hid){return function(){toggleHabit(hid);};})(h.id);
      top.appendChild(chk);

      var info2=mk("div","flex:1;min-width:0");
      var tn=mk("div","font-size:14px;font-weight:700");tn.textContent=h.title;
      if(h.streak>1){
        var sk=mk("span","font-size:11px;margin-right:6px;padding:2px 7px;border-radius:99px;font-weight:700");
        sk.textContent="🔥 "+h.streak+" يوم";sk.style.background="#f97316"+"20";sk.style.color="#f97316";
        tn.appendChild(sk);
      }
      info2.appendChild(tn);
      var meta=mk("div","font-size:11px;margin-top:3px;display:flex;gap:8px");meta.className="muted";
      if(h.time) meta.appendChild(mkSpan("🕐 "+h.time));
      if(h.duration) meta.appendChild(mkSpan("⏱ "+h.duration+"د"));
      if(asp){var as2=mkSpan("● "+asp.label);as2.style.color=asp.color;meta.appendChild(as2);}
      info2.appendChild(meta);
      top.appendChild(info2);

      // del btn
      var db=mkBtn("✕","background:none;border:none;cursor:pointer;font-size:13px;color:#f87171;",function(){
        if(!window.confirm("حذف هذه العادة؟")) return;
        ST.habits=ST.habits.filter(function(x){return x.id!==h.id});
        saveState();renderHabits();
      });
      top.appendChild(db);
      c.appendChild(top);

      // week log
      var wk=mk("div","display:flex;gap:4px;margin-top:2px");
      var ws=weekStart(td);
      for(var i=0;i<7;i++){
        var dd=addDays(ws,i);
        var done2=!!h.log[dd];
        var sq=mk("div","width:26px;height:26px;border-radius:6px;display:flex;align-items:center;justify-content:center;font-size:10px;cursor:default");
        sq.title=dateLabel(dd);
        sq.style.background=done2?acc()+"30":"rgba(128,128,128,.1)";
        sq.style.border="1px solid "+(done2?acc()+"44":"rgba(128,128,128,.15)");
        sq.textContent=done2?"✓":DAYS[new Date(dd+"T00:00:00").getDay()].charAt(0);
        sq.style.color=done2?acc():"rgba(128,128,128,.4)";
        sq.style.fontWeight=done2?"700":"400";
        wk.appendChild(sq);
      }
      c.appendChild(wk);
      habitList.appendChild(c);
    });

    // add habit form
    var formWrap=mk("div","margin-top:4px");
    var addBtn2=mkBtn("+ إضافة عادة","width:100%;padding:11px;border-radius:12px;font-size:13px;font-weight:700;cursor:pointer;transition:all .18s;",function(){
      formWrap.innerHTML="";
      var a2=acc();
      var ti=mk("input");ti.type="text";ti.className="inp";ti.placeholder="اسم العادة...";ti.style.marginBottom="8px";
      var r2=mk("div","display:flex;gap:7px;margin-bottom:8px;flex-wrap:wrap");
      var asel=mk("select");asel.className="sel";asel.style.flex="1";asel.style.minWidth="110px";
      ASPECTS.forEach(function(a3){var o=mk("option");o.value=a3.id;o.textContent=a3.label;asel.appendChild(o);});
      var psel=mk("select");psel.className="sel";psel.style.flex="1";psel.style.minWidth="110px";
      PRIS.forEach(function(p){var o=mk("option");o.value=p.id;o.textContent=p.label;psel.appendChild(o);});
      r2.appendChild(asel);r2.appendChild(psel);
      var r3=mk("div","display:flex;gap:7px;margin-bottom:10px;flex-wrap:wrap");
      var perSel=mk("select");perSel.className="sel";perSel.style.flex="1";
      PERIODS.forEach(function(p){var o=mk("option");o.value=p;o.textContent=p;perSel.appendChild(o);});
      var tInp=mk("input");tInp.type="time";tInp.className="inp";tInp.style.width="100px";tInp.value="06:00";
      var dInp=mk("input");dInp.type="number";dInp.className="inp";dInp.style.width="75px";dInp.min="5";dInp.step="5";dInp.value="30";
      r3.appendChild(perSel);r3.appendChild(tInp);r3.appendChild(dInp);
      var brow=mk("div","display:flex;gap:7px");
      var sv=mkBtn("إضافة","padding:9px 18px;border-radius:9px;font-size:13px;font-weight:700;cursor:pointer;border:none;color:#000;",function(){
        var ttl=ti.value.trim();if(!ttl)return;
        var newHabit={id:uid(),title:ttl,aspect:asel.value,priority:psel.value,period:perSel.value,time:tInp.value||"06:00",duration:+dInp.value||30,streak:0,log:{}};
        ST.habits.push(newHabit);
        // inject into today
        if(!ST.tasks[todayStr()]) ST.tasks[todayStr()]=[];
        ST.tasks[todayStr()].push({id:uid(),title:ttl,done:false,priority:psel.value,aspect:asel.value,period:perSel.value,time:tInp.value||"06:00",duration:+dInp.value||30,note:"",subtasks:[],pinned:true,isHabit:true,habitId:newHabit.id});
        saveState();toast("تمت الإضافة ✓");renderHabits();
      });
      sv.style.background="linear-gradient(135deg,"+a2+",#059669)";
      var cn=mkBtn("إلغاء","padding:9px 18px;border-radius:9px;font-size:13px;cursor:pointer;",function(){renderHabits();});
      cn.className="btn-ghost";
      brow.appendChild(sv);brow.appendChild(cn);
      formWrap.appendChild(ti);formWrap.appendChild(r2);formWrap.appendChild(r3);formWrap.appendChild(brow);
      setTimeout(function(){ti.focus();},50);
    });
    var a4=acc();
    addBtn2.style.background=a4+"12";addBtn2.style.border="1px dashed "+a4+"44";addBtn2.style.color=a4;
    formWrap.appendChild(addBtn2);
    habitList.appendChild(formWrap);
  }
  renderHabits();
  return wrap;
}

// ══════════ WEEK ══════════
function buildWeek(){
  var wrap=mk("div");
  var th=mk("div");th.className="sec-title";th.textContent="الجدول الأسبوعي";wrap.appendChild(th);
  var days=[];var ws=weekStart(selDate);
  for(var i=0;i<7;i++) days.push(addDays(ws,i));
  var grid=mk("div","display:grid;grid-template-columns:repeat(auto-fill,minmax(88px,1fr));gap:7px;margin-top:10px");
  days.forEach(function(d){
    var tasks=ST.tasks[d]||[];var pct=calcPct(tasks);var isT=d===todayStr();var a=acc();
    var card=mk("div","text-align:center;padding:10px 7px;cursor:pointer;transition:all .2s;border-radius:12px;border:1px solid");
    card.style.borderColor=isT?a+"44":"rgba(128,128,128,.15)";
    card.style.background=isT?a+"10":"";
    var dn=mk("div","font-size:11px;font-weight:800;margin-bottom:2px");dn.textContent=dateLabel(d).split(" ")[0];dn.style.color=isT?a:"";
    var dd2=mk("div","font-size:10px;margin-bottom:6px");dd2.textContent=d.slice(5);dd2.className="muted";
    var pp=mk("div","font-size:17px;font-weight:900;margin-bottom:3px");pp.textContent=pct+"%";pp.style.color=pctColor(pct);
    var pt=mk("div","height:3px;border-radius:99px;overflow:hidden;margin-bottom:3px");pt.className="prog-track";
    var pff=mk("div","height:100%;border-radius:99px");pff.style.width=pct+"%";pff.style.background=pctColor(pct);pt.appendChild(pff);
    var cnt=mk("div","font-size:10px");cnt.textContent=tasks.filter(function(x){return x.done}).length+"/"+tasks.length;cnt.className="muted";
    var cb=mkBtn("نسخ","font-size:10px;padding:2px 8px;border-radius:6px;cursor:pointer;margin-top:5px;display:block;width:100%;background:rgba(128,128,128,.1);border:1px solid rgba(128,128,128,.15);",function(e){e.stopPropagation();showCopyModal(d,days);});
    cb.className="muted";
    card.appendChild(dn);card.appendChild(dd2);card.appendChild(pp);card.appendChild(pt);card.appendChild(cnt);card.appendChild(cb);
    card.onclick=function(){selDate=d;ensureDate(d);filterAsp=null;switchView("today");};
    grid.appendChild(card);
  });
  wrap.appendChild(grid);
  return wrap;
}

// ══════════ POSTPONED ══════════
function buildPostponed(){
  var wrap=mk("div");
  var th=mk("div");th.className="sec-title";th.textContent="المهام المؤجلة ("+(ST.postponed||[]).length+")";wrap.appendChild(th);
  if(!(ST.postponed||[]).length){
    var em=mk("div");em.className="empty-state";em.innerHTML="<div style='font-size:44px'>🎉</div><div style='margin-top:8px;font-size:14px;font-weight:600'>لا توجد مهام مؤجلة</div>";wrap.appendChild(em);return wrap;
  }
  ST.postponed.forEach(function(task){
    var c=mk("div");c.className="card";
    var tn=mk("div","font-weight:700;font-size:14px;margin-bottom:4px");tn.textContent=task.title;c.appendChild(tn);
    var mt=mk("div","font-size:11px;margin-bottom:9px");mt.className="muted";
    mt.textContent="من: "+(task.originalDate||"")+" • "+((PRIS.find(function(p){return p.id===task.priority})||{}).label||"");c.appendChild(mt);
    var row=mk("div","display:flex;gap:6px;align-items:center;flex-wrap:wrap");
    var di=mk("input");di.type="date";di.className="inp";di.style.width="130px";di.style.fontSize="12px";di.value=todayStr();
    var a=acc();
    var sb=mkBtn("جدولة","padding:6px 13px;border-radius:8px;font-size:12px;cursor:pointer;font-weight:700;",function(){reschedule(task.id,di.value);});
    sb.style.background=a+"20";sb.style.border="1px solid "+a+"44";sb.style.color=a;
    var db=mkBtn("✕","background:none;border:none;cursor:pointer;font-size:13px;color:#f87171;",function(){ST.postponed=ST.postponed.filter(function(x){return x.id!==task.id});saveState();renderAll();});
    row.appendChild(di);row.appendChild(sb);row.appendChild(db);c.appendChild(row);wrap.appendChild(c);
  });
  return wrap;
}

// ══════════ STATS ══════════
function buildStats(){
  var wrap=mk("div");
  var th=mk("div");th.className="sec-title";th.textContent="الإحصائيات";wrap.appendChild(th);
  var ws=weekStart(todayStr());var days=[];for(var i=0;i<7;i++) days.push(addDays(ws,i));
  var wt=days.reduce(function(a,d){return a.concat(ST.tasks[d]||[]);},[]);
  var allDates=Object.keys(ST.tasks);
  var mt=allDates.slice(-30).reduce(function(a,d){return a.concat(ST.tasks[d]||[]);},[]);
  var td2=ST.tasks[selDate]||[];
  var items=[
    {label:"إنجاز اليوم",v:calcPct(td2)+"%",c:pctColor(calcPct(td2))},
    {label:"إنجاز الأسبوع",v:calcPct(wt)+"%",c:pctColor(calcPct(wt))},
    {label:"إنجاز الشهر",v:calcPct(mt)+"%",c:pctColor(calcPct(mt))},
    {label:"مؤجلات",v:(ST.postponed||[]).length,c:"#f87171"}
  ];
  var grid=mk("div","display:grid;grid-template-columns:repeat(2,1fr);gap:9px;margin-bottom:11px");
  items.forEach(function(s){
    var c=mk("div","text-align:center;padding:18px 12px;border-radius:14px");c.className="card";
    var v=mk("div","font-size:30px;font-weight:900;line-height:1;margin-bottom:4px");v.textContent=s.v;v.style.color=s.c;
    var l=mk("div","font-size:12px");l.textContent=s.label;l.className="muted";
    c.appendChild(v);c.appendChild(l);grid.appendChild(c);
  });
  wrap.appendChild(grid);

  var ac=mk("div");ac.className="card";
  var ah=mk("div","font-size:10px;font-weight:700;letter-spacing:1px;margin-bottom:13px");ah.textContent="الإنجاز حسب الجانب";ah.className="muted";ac.appendChild(ah);
  ASPECTS.forEach(function(a){
    var all=Object.values(ST.tasks).reduce(function(arr,ts){return arr.concat(ts);},[]).filter(function(x){return x.aspect===a.id});
    var p=all.length?Math.round(all.filter(function(x){return x.done}).length/all.length*100):0;
    var row=mk("div","margin-bottom:10px");
    var rl=mk("div","display:flex;justify-content:space-between;margin-bottom:4px");
    var lb=mk("span","font-size:12px");lb.textContent=a.label;lb.className="muted";
    var pv=mk("span","font-size:12px;font-weight:700");pv.textContent=p+"%";pv.style.color=a.color;
    rl.appendChild(lb);rl.appendChild(pv);
    var pt=mk("div","height:4px;border-radius:99px;overflow:hidden");pt.className="prog-track";
    var pf=mk("div","height:100%;border-radius:99px;transition:width .5s");pf.style.width=p+"%";pf.style.background=a.color;
    pt.appendChild(pf);row.appendChild(rl);row.appendChild(pt);ac.appendChild(row);
  });
  wrap.appendChild(ac);

  // habits streak
  var hc=mk("div");hc.className="card";hc.style.marginTop="0";
  var hh=mk("div","font-size:10px;font-weight:700;letter-spacing:1px;margin-bottom:13px");hh.textContent="🔥 سلاسل العادات";hh.className="muted";hc.appendChild(hh);
  (ST.habits||[]).forEach(function(h){
    var row=mk("div","display:flex;align-items:center;justify-content:space-between;padding:7px 0;border-bottom:1px solid rgba(128,128,128,.08)");
    var lt=mk("span","font-size:13px;font-weight:600");lt.textContent=h.title;
    var rt=mk("span","font-size:13px;font-weight:700");rt.textContent="🔥 "+h.streak+" يوم";rt.style.color="#f97316";
    row.appendChild(lt);row.appendChild(rt);hc.appendChild(row);
  });
  wrap.appendChild(hc);
  return wrap;
}

// ══════════ SETTINGS ══════════
function buildSettings(){
  var wrap=mk("div");
  var th=mk("div");th.className="sec-title";th.textContent="الإعدادات";wrap.appendChild(th);
  var a=acc();

  var app=mk("div");app.className="card";
  var ah=mk("div","font-size:10px;font-weight:700;letter-spacing:1px;margin-bottom:13px");ah.textContent="المظهر والإشعارات";ah.className="muted";app.appendChild(ah);

  // theme
  app.appendChild(buildSRow("الوضع الليلي","تبديل بين الليلي والنهاري",buildToggle(ST.settings.theme==="dark",function(v){ST.settings.theme=v?"dark":"light";saveState();applyTheme();renderAll();})));
  // notifs
  var notifRight;
  if(typeof Notification!=="undefined"&&Notification.permission==="granted"){
    notifRight=buildToggle(ST.settings.notifs,function(v){ST.settings.notifs=v;saveState();renderAll();});
  } else {
    var nb2=mkBtn("تفعيل","padding:6px 13px;border-radius:8px;font-size:12px;cursor:pointer;font-weight:700;",function(){if(typeof Notification!=="undefined")Notification.requestPermission().then(function(){renderAll();});});
    nb2.style.background=a+"20";nb2.style.border="1px solid "+a+"44";nb2.style.color=a;notifRight=nb2;
  }
  app.appendChild(buildSRow("الإشعارات",(typeof Notification!=="undefined"&&Notification.permission==="granted")?"مفعّلة ✓":"يتطلب إذن",notifRight));
  var sel2=mk("select");sel2.className="sel";sel2.style.width="110px";
  [5,10,15,30].forEach(function(n){var o=mk("option");o.value=n;o.textContent=n+" دقيقة";if(n===ST.settings.notifyBefore)o.selected=true;sel2.appendChild(o);});
  sel2.onchange=function(){ST.settings.notifyBefore=+this.value;saveState();};
  app.appendChild(buildSRow("تنبيه قبل المهمة","",sel2));
  wrap.appendChild(app);

  // danger
  var dc=mk("div");dc.className="card";dc.style.marginTop="0";
  var dh=mk("div","font-size:10px;font-weight:700;letter-spacing:1px;margin-bottom:9px;color:#f87171");dh.textContent="⚠️ منطقة الخطر";dc.appendChild(dh);
  var dp=mk("div","font-size:13px;margin-bottom:11px;line-height:1.7");dp.textContent="حذف جميع البيانات بشكل دائم.";dp.className="muted";dc.appendChild(dp);
  var db2=mkBtn("🗑️ حذف جميع البيانات","","function");db2.className="btn-danger";
  db2.onclick=function(){if(window.confirm("هل أنت متأكد؟ سيتم حذف جميع البيانات نهائيًا!")){localStorage.removeItem("masari_v6");window.location.reload();}};
  dc.appendChild(db2);wrap.appendChild(dc);
  return wrap;
}

function buildSRow(title,sub,right){
  var row=mk("div");row.className="srow";
  var lt=mk("div");
  var tl=mk("div","font-size:14px;font-weight:600");tl.textContent=title;lt.appendChild(tl);
  if(sub){var sl=mk("div","font-size:11px;margin-top:2px");sl.textContent=sub;sl.className="muted";lt.appendChild(sl);}
  row.appendChild(lt);row.appendChild(right);return row;
}
function buildToggle(active,onChange){
  var a=acc();
  var wrap2=mk("div");wrap2.className="toggle-wrap";
  wrap2.style.background=active?a:"rgba(128,128,128,.2)";
  var knob=mk("div");knob.className="toggle-knob";
  knob.style.background=active?"#000":"#fff";
  knob.style.right=active?"3px":"23px";
  wrap2.appendChild(knob);
  wrap2.onclick=function(){onChange(!active);};
  return wrap2;
}

// ══════════ MODALS ══════════
function showModal(content){
  closeModal();
  var ov=mk("div");ov.className="modal-ov";ov.id="app-modal";
  ov.onclick=function(e){if(e.target===ov)closeModal();};
  var box=mk("div");box.className="modal-box";box.appendChild(content);
  ov.appendChild(box);document.body.appendChild(ov);
}
function closeModal(){var m=document.getElementById("app-modal");if(m)m.remove();}

function modalHdr(title){
  var row=mk("div");row.className="modal-hdr";
  var x=mkBtn("✕","background:none;border:none;cursor:pointer;font-size:19px;line-height:1;",closeModal);x.className="muted";
  var h=mk("div","font-size:15px;font-weight:800");h.textContent=title;
  row.appendChild(x);row.appendChild(h);return row;
}

function showTaskForm(task){
  var isEdit=!!task;
  var form=task?JSON.parse(JSON.stringify(task)):{title:"",priority:"normal",aspect:"other",period:"الصباح",time:"",duration:30,note:"",subtasks:[],pinned:false,recurring:false};
  var a=acc();var wrap2=mk("div");
  wrap2.appendChild(modalHdr(isEdit?"✏️ تعديل المهمة":"✨ مهمة جديدة"));
  var body=mk("div");body.className="modal-body";

  var ti=mk("input");ti.type="text";ti.className="inp";ti.placeholder="اكتب المهمة...";ti.value=form.title;
  ti.oninput=function(){form.title=this.value;};body.appendChild(ti);

  var r1=mk("div","display:flex;gap:7px;flex-wrap:wrap");
  var ps=mk("select");ps.className="sel";ps.style.flex="1";ps.style.minWidth="110px";
  PRIS.forEach(function(p){var o=mk("option");o.value=p.id;o.textContent=p.label;if(p.id===form.priority)o.selected=true;ps.appendChild(o);});
  ps.onchange=function(){form.priority=this.value;};
  var as2=mk("select");as2.className="sel";as2.style.flex="1";as2.style.minWidth="110px";
  ASPECTS.forEach(function(a3){var o=mk("option");o.value=a3.id;o.textContent=a3.label;if(a3.id===form.aspect)o.selected=true;as2.appendChild(o);});
  as2.onchange=function(){form.aspect=this.value;};
  r1.appendChild(ps);r1.appendChild(as2);body.appendChild(r1);

  var r2=mk("div","display:flex;gap:7px;flex-wrap:wrap");
  var per=mk("select");per.className="sel";per.style.flex="1";per.style.minWidth="90px";
  PERIODS.forEach(function(p){var o=mk("option");o.value=p;o.textContent=p;if(p===form.period)o.selected=true;per.appendChild(o);});
  per.onchange=function(){form.period=this.value;};
  var tInp=mk("input");tInp.type="time";tInp.className="inp";tInp.style.flex="1";tInp.value=form.time;tInp.onchange=function(){form.time=this.value;};
  var dInp=mk("input");dInp.type="number";dInp.className="inp";dInp.style.width="75px";dInp.min="5";dInp.step="5";dInp.value=form.duration;dInp.onchange=function(){form.duration=+this.value;};
  r2.appendChild(per);r2.appendChild(tInp);r2.appendChild(dInp);body.appendChild(r2);

  var note2=mk("textarea");note2.className="inp";note2.rows=2;note2.placeholder="ملاحظات...";note2.style.resize="vertical";note2.value=form.note||"";
  note2.oninput=function(){form.note=this.value;};body.appendChild(note2);

  // subtasks
  var subLbl=mk("div","font-size:11px;font-weight:700;letter-spacing:1px");subLbl.textContent="الخطوات الفرعية";subLbl.className="muted";body.appendChild(subLbl);
  var subList=mk("div");body.appendChild(subList);
  function renderSubs(){
    subList.innerHTML="";
    (form.subtasks||[]).forEach(function(s,i){
      var sr=mk("div","display:flex;align-items:center;gap:6px;margin-bottom:5px");
      var st2=mk("span","flex:1;font-size:12px");st2.textContent="• "+s.title;st2.className="muted";
      var db3=mkBtn("✕","background:none;border:none;cursor:pointer;font-size:12px;color:#f87171;",function(){form.subtasks.splice(i,1);renderSubs();});
      sr.appendChild(st2);sr.appendChild(db3);subList.appendChild(sr);
    });
    var sr2=mk("div","display:flex;gap:6px");
    var si2=mk("input");si2.type="text";si2.className="inp";si2.style.flex="1";si2.placeholder="خطوة جديدة...";
    var addS=mkBtn("+","border:none;padding:0 13px;border-radius:8px;cursor:pointer;font-weight:900;font-size:15px;height:35px;color:#000;",function(){
      if(si2.value.trim()){form.subtasks.push({id:uid(),title:si2.value.trim(),done:false});renderSubs();}
    });
    addS.style.background=a;
    si2.onkeydown=function(e){if(e.key==="Enter"&&si2.value.trim()){form.subtasks.push({id:uid(),title:si2.value.trim(),done:false});renderSubs();}};
    sr2.appendChild(si2);sr2.appendChild(addS);subList.appendChild(sr2);
  }
  renderSubs();

  var brow=mk("div","display:flex;gap:7px");
  var sv2=mkBtn("حفظ","","");sv2.className="btn-g";sv2.style.width="auto";sv2.style.padding="10px 22px";
  sv2.onclick=function(){
    if(!form.title.trim()) return;
    if(isEdit){ST.tasks[selDate]=(ST.tasks[selDate]||[]).map(function(x){return x.id===task.id?Object.assign({},x,form):x;});}
    else{if(!ST.tasks[selDate])ST.tasks[selDate]=[];ST.tasks[selDate].push(Object.assign({},form,{id:uid(),done:false}));}
    saveState();closeModal();toast(isEdit?"تم التعديل ✓":"تمت الإضافة ✓");renderAll();
  };
  var cn=mkBtn("إلغاء","","");cn.className="btn-ghost";cn.onclick=closeModal;
  brow.appendChild(sv2);brow.appendChild(cn);body.appendChild(brow);
  wrap2.appendChild(body);showModal(wrap2);
  setTimeout(function(){var i=document.querySelector("#app-modal input[type=text]");if(i)i.focus();},60);
}

function showMoveModal(taskId){
  var wrap2=mk("div");wrap2.appendChild(modalHdr("↗ نقل المهمة إلى"));
  var body=mk("div");body.className="modal-body";
  var di=mk("input");di.type="date";di.className="inp";di.value=addDays(selDate,1);body.appendChild(di);
  var mv=mkBtn("نقل المهمة","","");mv.className="btn-g";
  mv.onclick=function(){
    var task=(ST.tasks[selDate]||[]).find(function(t){return t.id===taskId});
    if(!task) return;
    var nd=di.value;
    if(!ST.tasks[nd]) ST.tasks[nd]=[];
    ST.tasks[nd].push(Object.assign({},task,{id:uid(),done:false}));
    ST.tasks[selDate]=(ST.tasks[selDate]||[]).filter(function(t){return t.id!==taskId});
    saveState();closeModal();toast("تم النقل ✓");renderAll();
  };
  body.appendChild(mv);wrap2.appendChild(body);showModal(wrap2);
}

function showReviewModal(){
  var rev=ST.reviews[selDate]||{};var vals=Object.assign({},rev);
  var wrap2=mk("div");wrap2.appendChild(modalHdr("📝 مراجعة "+dateLabel(selDate)));
  var body=mk("div");body.className="modal-body";
  [["what_worked","✅ ماذا نجح؟"],["what_failed","❌ لماذا لم يكتمل؟"],["how_was_day","💭 كيف كان اليوم؟"]].forEach(function(kl){
    var lb=mk("div","font-size:11px;font-weight:700;margin-bottom:4px");lb.textContent=kl[1];lb.className="muted";
    var ta=mk("textarea");ta.className="inp";ta.rows=3;ta.style.resize="vertical";ta.value=vals[kl[0]]||"";
    ta.oninput=(function(k,el){return function(){vals[k]=el.value;};})(kl[0],ta);
    body.appendChild(lb);body.appendChild(ta);
  });
  var brow=mk("div","display:flex;gap:7px");
  var sv=mkBtn("حفظ","","");sv.className="btn-g";sv.style.width="auto";sv.style.padding="10px 22px";
  sv.onclick=function(){ST.reviews[selDate]=vals;saveState();closeModal();toast("تم الحفظ ✓");};
  var cn=mkBtn("إلغاء","","");cn.className="btn-ghost";cn.onclick=closeModal;
  brow.appendChild(sv);brow.appendChild(cn);body.appendChild(brow);
  wrap2.appendChild(body);showModal(wrap2);
}

function showEmergency(){
  var wrap2=mk("div");wrap2.appendChild(modalHdr("⚡ وضع الطوارئ"));
  var body=mk("div");body.className="modal-body";
  var p=mk("div","font-size:13px;line-height:1.8");p.textContent="سيتم الاحتفاظ بالمهام المهمة جداً والمثبتة فقط، وتأجيل الباقي.";p.className="muted";body.appendChild(p);
  var brow=mk("div","display:flex;gap:7px");
  var go=mkBtn("تفعيل الطوارئ","","");go.className="btn-danger";
  go.onclick=function(){
    var tasks=ST.tasks[selDate]||[];
    var keep=tasks.filter(function(t){return t.priority==="critical"||t.pinned;});
    var move=tasks.filter(function(t){return t.priority!=="critical"&&!t.pinned&&!t.done;});
    ST.tasks[selDate]=keep;
    move.forEach(function(t){ST.postponed.push(Object.assign({},t,{originalDate:selDate}));});
    saveState();closeModal();toast("تم تفعيل الطوارئ");renderAll();
  };
  var cn=mkBtn("إلغاء","","");cn.className="btn-ghost";cn.onclick=closeModal;
  brow.appendChild(go);brow.appendChild(cn);body.appendChild(brow);
  wrap2.appendChild(body);showModal(wrap2);
}

function showCopyModal(fromDate,days){
  var wrap2=mk("div");wrap2.appendChild(modalHdr("نسخ "+dateLabel(fromDate)));
  var body=mk("div");body.className="modal-body";
  var p=mk("div","font-size:12px;margin-bottom:4px");p.textContent="نسخ مهام هذا اليوم إلى:";p.className="muted";body.appendChild(p);
  var gr=mk("div","display:flex;gap:6px;flex-wrap:wrap");
  days.filter(function(d){return d!==fromDate;}).forEach(function(d){
    var b=mkBtn(dateLabel(d),"padding:7px 11px;border-radius:8px;font-size:12px;cursor:pointer;font-weight:600;background:rgba(128,128,128,.1);border:1px solid rgba(128,128,128,.15);","");
    b.className="muted";
    b.onclick=function(){ST.tasks[d]=(ST.tasks[fromDate]||[]).map(function(t){return Object.assign({},t,{id:uid(),done:false});});saveState();closeModal();toast("تم النسخ ✓");renderAll();};
    gr.appendChild(b);
  });
  body.appendChild(gr);wrap2.appendChild(body);showModal(wrap2);
}

// ══════════ FOCUS ══════════
function openFocus(task){
  if(focusInterval) clearInterval(focusInterval);
  var timer=task.duration*60;var running=false;
  var ov=mk("div");ov.className="focus-ov";ov.id="focus-modal";
  var box=mk("div");box.className="focus-box";
  var xb=mkBtn("✕","position:absolute;top:13px;left:13px;background:none;border:none;cursor:pointer;font-size:18px;",function(){clearInterval(focusInterval);ov.remove();});
  xb.className="muted";
  var tn=mk("div","font-size:20px;font-weight:800;color:#eee;margin-bottom:7px;line-height:1.4");tn.textContent=task.title;
  var te=mk("div","font-size:58px;font-weight:900;color:#4ade80;letter-spacing:-2px;margin:14px 0;font-variant-numeric:tabular-nums");te.textContent=fmtTimer(timer);
  var pb=mk("div","height:4px;border-radius:99px;overflow:hidden;margin:0 8px;background:rgba(255,255,255,.08)");
  var pf2=mk("div","height:100%;background:#4ade80;border-radius:99px;width:0%;transition:width .5s");pb.appendChild(pf2);
  var brow=mk("div","display:flex;gap:11px;justify-content:center;margin-top:22px");
  var pl=mkBtn("▶ بدء","padding:10px 22px;border-radius:10px;font-size:14px;font-weight:700;cursor:pointer;background:rgba(74,222,128,.15);border:1px solid rgba(74,222,128,.3);color:#4ade80;",function(){
    running=!running;pl.textContent=running?"⏸ إيقاف":"▶ بدء";
    if(running){focusInterval=setInterval(function(){if(timer<=0){clearInterval(focusInterval);running=false;return;}timer--;te.textContent=fmtTimer(timer);pf2.style.width=Math.round((1-timer/(task.duration*60))*100)+"%";},1000);}
    else clearInterval(focusInterval);
  });
  var dn2=mkBtn("✓ إنجاز","","");dn2.className="btn-g";dn2.style.width="auto";dn2.style.padding="10px 22px";
  dn2.onclick=function(){clearInterval(focusInterval);toggleTask(task.id);ov.remove();};
  brow.appendChild(pl);brow.appendChild(dn2);
  box.appendChild(xb);box.appendChild(tn);box.appendChild(te);box.appendChild(pb);box.appendChild(brow);
  ov.appendChild(box);document.body.appendChild(ov);
}

// ══════════ TASK OPS ══════════
function toggleTask(id){
  ST.tasks[selDate]=(ST.tasks[selDate]||[]).map(function(t){return t.id===id?Object.assign({},t,{done:!t.done}):t;});
  // if habit, update streak
  var task=(ST.tasks[selDate]||[]).find(function(t){return t.id===id});
  if(task&&task.isHabit&&task.habitId){
    var h=ST.habits.find(function(h2){return h2.id===task.habitId});
    if(h){
      var td3=selDate;
      h.log[td3]=task.done;
      if(!task.done) delete h.log[td3]; // was done, now undone
      // recalc streak
      var streak=0,d2=todayStr();while(h.log[d2]){streak++;d2=addDays(d2,-1);}
      h.streak=streak;
    }
  }
  saveState();renderAll();
}
function toggleSubtask(taskId,subId){
  ST.tasks[selDate]=(ST.tasks[selDate]||[]).map(function(t){
    if(t.id!==taskId) return t;
    return Object.assign({},t,{subtasks:(t.subtasks||[]).map(function(s){return s.id===subId?Object.assign({},s,{done:!s.done}):s;})});
  });
  saveState();renderAll();
}
function deleteTask(id){
  ST.tasks[selDate]=(ST.tasks[selDate]||[]).filter(function(t){return t.id!==id;});
  saveState();toast("تم الحذف");renderAll();
}
function postponeTask(id){
  var task=(ST.tasks[selDate]||[]).find(function(t){return t.id===id;});
  if(!task) return;
  ST.postponed.push(Object.assign({},task,{originalDate:selDate}));
  ST.tasks[selDate]=ST.tasks[selDate].filter(function(t){return t.id!==id;});
  saveState();toast("تم التأجيل");renderAll();
}
function reschedule(id,newDate){
  var pt=(ST.postponed||[]).find(function(x){return x.id===id;});
  if(!pt) return;
  var nt=Object.assign({},pt,{id:uid(),done:false});delete nt.originalDate;
  if(!ST.tasks[newDate]) ST.tasks[newDate]=[];
  ST.tasks[newDate].push(nt);
  ST.postponed=ST.postponed.filter(function(x){return x.id!==id;});
  saveState();toast("تمت الجدولة ✓");renderAll();
}
function toggleHabit(hid){
  var td4=todayStr();
  var h=ST.habits.find(function(x){return x.id===hid;});
  if(!h) return;
  var wasDone=!!h.log[td4];
  h.log[td4]=!wasDone;
  if(wasDone) delete h.log[td4];
  // streak
  var streak=0,d3=td4;while(h.log[d3]){streak++;d3=addDays(d3,-1);}h.streak=streak;
  // sync task
  if(ST.tasks[td4]){
    ST.tasks[td4]=ST.tasks[td4].map(function(t){
      if(t.habitId===hid) return Object.assign({},t,{done:!wasDone});
      return t;
    });
  }
  saveState();renderAll();
}
function toggleTheme(){ST.settings.theme=ST.settings.theme==="dark"?"light":"dark";saveState();applyTheme();renderAll();}

// ══════════ TOAST ══════════
function toast(msg,type){
  var tw=document.getElementById("toast-wrap");if(!tw) return;
  tw.innerHTML="";
  var t=mk("div");t.className="toast";t.textContent=msg;
  t.style.background=type==="err"?"#f87171":"#4ade80";
  tw.appendChild(t);
  setTimeout(function(){if(tw.contains(t)) tw.removeChild(t);},2400);
}

// ══════════ DOM HELPERS ══════════
function mk(tag,css,children){
  var e=document.createElement(tag);
  if(css) e.style.cssText=css;
  if(children) children.forEach(function(c){if(typeof c==="string"||typeof c==="number"){e.appendChild(document.createTextNode(c));}});
  return e;
}
function mkBtn(text,css,fn){var b=mk("button",css);b.textContent=text;if(fn) b.onclick=fn;return b;}
function mkSpan(text){var s=document.createElement("span");s.textContent=text;return s;}
</script>
</body>
</html>
