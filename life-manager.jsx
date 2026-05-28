
import { useState, useEffect, useRef, useCallback } from "react";

// ========================
// CONSTANTS & HELPERS
// ========================
const DAYS_AR = ["الأحد","الاثنين","الثلاثاء","الأربعاء","الخميس","الجمعة","السبت"];
const PERIODS = ["الفجر","الصباح","الظهر","العصر","الليل"];
const ASPECTS = [
  { id:"faith", label:"الإيماني", color:"#a78bfa" },
  { id:"science", label:"العلمي", color:"#60a5fa" },
  { id:"skill", label:"المهاري", color:"#34d399" },
  { id:"culture", label:"الثقافي", color:"#fbbf24" },
  { id:"finance", label:"المالي", color:"#f87171" },
  { id:"sport", label:"الرياضي", color:"#fb923c" },
  { id:"other", label:"أخرى", color:"#94a3b8" },
];
const PRIORITIES = [
  { id:"critical", label:"مهم جدًا", color:"#f87171" },
  { id:"high", label:"مهم", color:"#fb923c" },
  { id:"normal", label:"عادي", color:"#60a5fa" },
  { id:"optional", label:"اختياري", color:"#94a3b8" },
];
const ENERGY = [
  { id:"high", label:"عالية", icon:"⚡" },
  { id:"medium", label:"متوسطة", icon:"🔋" },
  { id:"low", label:"منخفضة", icon:"😴" },
];
const TEMPLATES = [
  { id:"study", label:"يوم دراسة", icon:"📚" },
  { id:"creative", label:"يوم مونتاج", icon:"🎬" },
  { id:"memorize", label:"يوم حفظ", icon:"🧠" },
  { id:"rest", label:"يوم راحة", icon:"☀️" },
  { id:"productive", label:"يوم إنتاجية", icon:"🚀" },
];
const SEASONAL = [
  { id:"ramadan", label:"رمضان", icon:"🌙" },
  { id:"exams", label:"اختبارات", icon:"📝" },
  { id:"vacation", label:"إجازة", icon:"🏖️" },
  { id:"travel", label:"سفر", icon:"✈️" },
];

const uid = () => Math.random().toString(36).slice(2,9);
const today = () => {
  const d = new Date();
  return `${d.getFullYear()}-${String(d.getMonth()+1).padStart(2,"0")}-${String(d.getDate()).padStart(2,"0")}`;
};
const getWeekDays = (dateStr) => {
  const d = new Date(dateStr);
  const day = d.getDay();
  const sun = new Date(d); sun.setDate(d.getDate() - day);
  return Array.from({length:7},(_,i)=>{
    const dd = new Date(sun); dd.setDate(sun.getDate()+i);
    return `${dd.getFullYear()}-${String(dd.getMonth()+1).padStart(2,"0")}-${String(dd.getDate()).padStart(2,"0")}`;
  });
};
const dateLabel = (dateStr) => {
  const d = new Date(dateStr);
  return `${DAYS_AR[d.getDay()]} ${d.getDate()}/${d.getMonth()+1}`;
};
const calcCompletion = (tasks) => {
  if(!tasks||!tasks.length) return 0;
  return Math.round(tasks.filter(t=>t.done).length/tasks.length*100);
};
const completionColor = (pct) => pct >= 80?"#34d399":pct>=50?"#fbbf24":"#f87171";

// ========================
// INITIAL DATA
// ========================
const INIT_TASKS = () => [
  { id:uid(), title:"أذكار الصباح", done:false, priority:"critical", aspect:"faith", period:"الفجر", time:"06:00", duration:15, note:"", subtasks:[], pinned:true, recurring:true, fixed:false },
  { id:uid(), title:"قراءة القرآن", done:false, priority:"critical", aspect:"faith", period:"الصباح", time:"07:00", duration:30, note:"", subtasks:[], pinned:true, recurring:true, fixed:false },
  { id:uid(), title:"مراجعة الدروس", done:false, priority:"high", aspect:"science", period:"الصباح", time:"09:00", duration:60, note:"", subtasks:[{id:uid(),title:"الرياضيات",done:false},{id:uid(),title:"الفيزياء",done:false}], pinned:false, recurring:false, fixed:false },
  { id:uid(), title:"تمرين رياضي", done:false, priority:"normal", aspect:"sport", period:"العصر", time:"17:00", duration:45, note:"", subtasks:[], pinned:false, recurring:true, fixed:false },
  { id:uid(), title:"قراءة كتاب", done:false, priority:"normal", aspect:"culture", period:"الليل", time:"21:00", duration:30, note:"", subtasks:[], pinned:false, recurring:false, fixed:false },
];

const INIT_STATE = () => ({
  tasks: { [today()]: INIT_TASKS() },
  weeklySchedule: {},
  postponed: [],
  habits: [
    { id:uid(), title:"أذكار الصباح", aspect:"faith", time:"06:00" },
    { id:uid(), title:"تمرين رياضي", aspect:"sport", time:"17:00" },
  ],
  reviews: {},
  energy: {},
  notes: {},
  aspects: [...ASPECTS],
  settings: { theme:"dark", notifications:true, notifyBefore:15 },
  seasonal: null,
});

const load = () => { try { return JSON.parse(localStorage.getItem("lm_v2")) || INIT_STATE(); } catch{ return INIT_STATE(); } };
const save = (s) => localStorage.setItem("lm_v2", JSON.stringify(s));

// ========================
// MAIN APP
// ========================
export default function App() {
  const [state, setState] = useState(load);
  const [view, setView] = useState("today"); // today | week | postponed | stats | settings | focus | archive
  const [selectedDate, setSelectedDate] = useState(today());
  const [filterAspect, setFilterAspect] = useState(null);
  const [search, setSearch] = useState("");
  const [showTaskForm, setShowTaskForm] = useState(false);
  const [editTask, setEditTask] = useState(null);
  const [showReview, setShowReview] = useState(false);
  const [showEmergency, setShowEmergency] = useState(false);
  const [focusTask, setFocusTask] = useState(null);
  const [timer, setTimer] = useState(null);
  const [timerRunning, setTimerRunning] = useState(false);
  const timerRef = useRef(null);
  const [dragOver, setDragOver] = useState(null);
  const [dragging, setDragging] = useState(null);
  const [notifPerm, setNotifPerm] = useState("default");

  useEffect(() => { save(state); }, [state]);
  useEffect(() => {
    if("Notification" in window) setNotifPerm(Notification.permission);
  }, []);

  // Timer logic
  useEffect(() => {
    if(timerRunning && timer > 0) {
      timerRef.current = setTimeout(() => setTimer(t=>t-1), 1000);
    } else if(timer === 0 && timerRunning) {
      setTimerRunning(false);
      sendNotif("⏰ انتهى الوقت!", "المؤقت انتهى");
    }
    return () => clearTimeout(timerRef.current);
  }, [timerRunning, timer]);

  const sendNotif = (title, body) => {
    if(state.settings.notifications && Notification.permission === "granted") {
      new Notification(title, { body });
    }
  };

  const requestNotif = async () => {
    if("Notification" in window) {
      const p = await Notification.requestPermission();
      setNotifPerm(p);
    }
  };

  const update = (fn) => setState(s => { const ns = fn({...s}); return ns; });

  // Task CRUD
  const addTask = (task, dateStr=selectedDate) => {
    update(s => {
      const tasks = s.tasks[dateStr] || [];
      s.tasks = {...s.tasks, [dateStr]: [...tasks, {...task, id:uid()}]};
      return s;
    });
  };
  const updateTask = (id, changes, dateStr=selectedDate) => {
    update(s => {
      const tasks = (s.tasks[dateStr]||[]).map(t=>t.id===id?{...t,...changes}:t);
      s.tasks = {...s.tasks, [dateStr]: tasks};
      return s;
    });
  };
  const deleteTask = (id, dateStr=selectedDate) => {
    update(s => {
      s.tasks = {...s.tasks, [dateStr]: (s.tasks[dateStr]||[]).filter(t=>t.id!==id)};
      return s;
    });
  };
  const toggleTask = (id, dateStr=selectedDate) => {
    const t = (state.tasks[dateStr]||[]).find(x=>x.id===id);
    if(t) {
      updateTask(id, {done:!t.done}, dateStr);
      if(!t.done) sendNotif("✅ أحسنت!", `أنجزت: ${t.title}`);
    }
  };
  const postponeTask = (id, dateStr=selectedDate) => {
    const t = (state.tasks[dateStr]||[]).find(x=>x.id===id);
    if(t) {
      update(s => {
        s.postponed = [...(s.postponed||[]), {...t, originalDate:dateStr, postponedAt:new Date().toISOString()}];
        s.tasks = {...s.tasks, [dateStr]: (s.tasks[dateStr]||[]).filter(x=>x.id!==id)};
        return s;
      });
    }
  };
  const reschedulePostponed = (postId, newDate) => {
    update(s => {
      const pt = s.postponed.find(x=>x.id===postId);
      if(!pt) return s;
      const newTask = {...pt, id:uid(), done:false};
      delete newTask.originalDate; delete newTask.postponedAt;
      s.tasks = {...s.tasks, [newDate]: [...(s.tasks[newDate]||[]), newTask]};
      s.postponed = s.postponed.filter(x=>x.id!==postId);
      return s;
    });
  };

  // Emergency mode
  const emergencyMode = () => {
    update(s => {
      const tasks = s.tasks[selectedDate] || [];
      const keep = tasks.filter(t=>t.priority==="critical"||t.pinned||t.fixed);
      const move = tasks.filter(t=>t.priority!=="critical"&&!t.pinned&&!t.fixed&&!t.done);
      s.tasks[selectedDate] = keep;
      s.postponed = [...(s.postponed||[]), ...move.map(t=>({...t, originalDate:selectedDate, postponedAt:new Date().toISOString()}))];
      return s;
    });
    setShowEmergency(false);
  };

  // Copy day
  const copyDay = (fromDate, toDate) => {
    update(s => {
      const tasks = (s.tasks[fromDate]||[]).map(t=>({...t,id:uid(),done:false}));
      s.tasks = {...s.tasks, [toDate]: tasks};
      return s;
    });
  };

  // Drag & drop
  const handleDragStart = (id) => setDragging(id);
  const handleDragOver = (e, id) => { e.preventDefault(); setDragOver(id); };
  const handleDrop = (targetId) => {
    if(!dragging || !targetId || dragging===targetId) { setDragging(null); setDragOver(null); return; }
    update(s => {
      const tasks = [...(s.tasks[selectedDate]||[])];
      const fromIdx = tasks.findIndex(t=>t.id===dragging);
      const toIdx = tasks.findIndex(t=>t.id===targetId);
      const [moved] = tasks.splice(fromIdx,1);
      tasks.splice(toIdx,0,moved);
      s.tasks = {...s.tasks, [selectedDate]: tasks};
      return s;
    });
    setDragging(null); setDragOver(null);
  };

  // Stats
  const getStats = () => {
    const allDates = Object.keys(state.tasks);
    const todayTasks = state.tasks[today()]||[];
    const weekDates = getWeekDays(today());
    const weekTasks = weekDates.flatMap(d=>state.tasks[d]||[]);
    return {
      todayPct: calcCompletion(todayTasks),
      todayDone: todayTasks.filter(t=>t.done).length,
      todayTotal: todayTasks.length,
      weekPct: calcCompletion(weekTasks),
      monthPct: calcCompletion(allDates.slice(-30).flatMap(d=>state.tasks[d]||[])),
      bestWeek: Math.max(...weekDates.map(d=>calcCompletion(state.tasks[d]||[]))),
      postponedCount: (state.postponed||[]).length,
    };
  };

  const stats = getStats();
  const todayTasks = state.tasks[selectedDate]||[];
  const filteredTasks = todayTasks
    .filter(t=>!filterAspect||t.aspect===filterAspect)
    .filter(t=>!search||t.title.includes(search))
    .sort((a,b)=>{
      const ord={critical:0,high:1,normal:2,optional:3};
      return (ord[a.priority]||2)-(ord[b.priority]||2);
    });

  const completion = calcCompletion(todayTasks);

  return (
    <div dir="rtl" style={styles.root(state.settings.theme)}>
      {/* Background */}
      <div style={styles.bg} />
      
      {/* Focus Mode */}
      {focusTask && (
        <FocusMode
          task={focusTask}
          timer={timer} setTimer={setTimer}
          timerRunning={timerRunning} setTimerRunning={setTimerRunning}
          onClose={() => { setFocusTask(null); setTimer(null); setTimerRunning(false); }}
          onDone={() => { toggleTask(focusTask.id); setFocusTask(null); }}
        />
      )}

      {/* Emergency Modal */}
      {showEmergency && (
        <Modal onClose={()=>setShowEmergency(false)} title="⚠️ وضع الطوارئ">
          <p style={{color:"#94a3b8",marginBottom:16,lineHeight:1.7}}>
            سيتم الاحتفاظ بالمهام المهمة جداً والمثبتة فقط، ونقل باقي المهام للمؤجلات.
          </p>
          <div style={{display:"flex",gap:8}}>
            <button style={styles.btnDanger} onClick={emergencyMode}>تفعيل الطوارئ</button>
            <button style={styles.btnGhost} onClick={()=>setShowEmergency(false)}>إلغاء</button>
          </div>
        </Modal>
      )}

      {/* Task Form */}
      {(showTaskForm||editTask) && (
        <TaskForm
          task={editTask}
          aspects={state.aspects}
          onSave={(t) => {
            if(editTask) updateTask(editTask.id, t);
            else addTask(t);
            setShowTaskForm(false); setEditTask(null);
          }}
          onClose={()=>{ setShowTaskForm(false); setEditTask(null); }}
        />
      )}

      {/* Review Modal */}
      {showReview && (
        <ReviewModal
          dateStr={selectedDate}
          review={state.reviews[selectedDate]||{}}
          onSave={(r) => {
            update(s=>{ s.reviews={...s.reviews,[selectedDate]:r}; return s; });
            setShowReview(false);
          }}
          onClose={()=>setShowReview(false)}
        />
      )}

      {/* Header */}
      <header style={styles.header}>
        <div style={styles.headerLeft}>
          <div style={styles.logo}>
            <span style={styles.logoMark}>◈</span>
            <span style={styles.logoText}>مساري</span>
          </div>
          <div style={styles.dateChip}>
            {dateLabel(today())}
          </div>
        </div>
        <div style={styles.headerRight}>
          <button style={styles.iconBtn} onClick={()=>setState(s=>({...s,settings:{...s.settings,theme:s.settings.theme==="dark"?"light":"dark"}}))} title="تبديل الوضع">
            {state.settings.theme==="dark"?"☀️":"🌙"}
          </button>
          <button style={styles.emergencyBtn} onClick={()=>setShowEmergency(true)}>
            ⚡ طوارئ
          </button>
        </div>
      </header>

      {/* Nav */}
      <nav style={styles.nav}>
        {[
          {id:"today",label:"اليوم",icon:"◎"},
          {id:"week",label:"الأسبوع",icon:"▦"},
          {id:"postponed",label:"المؤجلات",icon:"⏳"},
          {id:"stats",label:"الإحصائيات",icon:"◈"},
          {id:"settings",label:"الإعدادات",icon:"⚙"},
        ].map(n=>(
          <button key={n.id} style={styles.navBtn(view===n.id)} onClick={()=>setView(n.id)}>
            <span style={{fontSize:14}}>{n.icon}</span>
            <span>{n.label}</span>
            {n.id==="postponed"&&(state.postponed||[]).length>0&&(
              <span style={styles.badge}>{(state.postponed||[]).length}</span>
            )}
          </button>
        ))}
      </nav>

      {/* Content */}
      <main style={styles.main}>
        
        {/* TODAY VIEW */}
        {view==="today" && (
          <div>
            {/* Quick Stats Bar */}
            <div style={styles.statsBar}>
              <div style={styles.statCard}>
                <div style={{...styles.statNum, color:completionColor(completion)}}>{completion}%</div>
                <div style={styles.statLabel}>الإنجاز</div>
              </div>
              <div style={styles.statCard}>
                <div style={styles.statNum}>{todayTasks.filter(t=>t.done).length}/{todayTasks.length}</div>
                <div style={styles.statLabel}>المهام</div>
              </div>
              <div style={styles.statCard}>
                <div style={{...styles.statNum, color:"#f87171"}}>{(state.postponed||[]).length}</div>
                <div style={styles.statLabel}>مؤجلات</div>
              </div>
              <div style={styles.statCard}>
                <div style={styles.statNum}>{todayTasks.filter(t=>t.pinned&&!t.done).length}</div>
                <div style={styles.statLabel}>مهام أساسية</div>
              </div>
            </div>

            {/* Progress Bar */}
            <div style={styles.progressWrap}>
              <div style={styles.progressBar}>
                <div style={{...styles.progressFill, width:`${completion}%`, background:completionColor(completion)}} />
              </div>
              <span style={{color:"#64748b",fontSize:12}}>{todayTasks.filter(t=>t.done).length} / {todayTasks.length} مهمة</span>
            </div>

            {/* Energy + Review Row */}
            <div style={{display:"flex",gap:8,marginBottom:16,flexWrap:"wrap"}}>
              <div style={styles.card2}>
                <span style={{color:"#64748b",fontSize:12,marginBottom:4,display:"block"}}>مستوى الطاقة</span>
                <div style={{display:"flex",gap:6}}>
                  {ENERGY.map(e=>(
                    <button key={e.id} style={styles.energyBtn(state.energy[selectedDate]===e.id)} onClick={()=>update(s=>({...s,energy:{...s.energy,[selectedDate]:e.id}}))}>
                      {e.icon} {e.label}
                    </button>
                  ))}
                </div>
              </div>
              <button style={styles.card2Btn} onClick={()=>setShowReview(true)}>
                📝 مراجعة اليوم
              </button>
            </div>

            {/* Aspects Filter */}
            <div style={styles.aspectsRow}>
              <button style={styles.aspectChip(filterAspect===null)} onClick={()=>setFilterAspect(null)}>الكل</button>
              {state.aspects.map(a=>(
                <button key={a.id} style={styles.aspectChip(filterAspect===a.id,a.color)} onClick={()=>setFilterAspect(filterAspect===a.id?null:a.id)}>
                  {a.label}
                </button>
              ))}
            </div>

            {/* Search */}
            <div style={styles.searchWrap}>
              <span style={styles.searchIcon}>🔍</span>
              <input style={styles.searchInput} placeholder="بحث في المهام..." value={search} onChange={e=>setSearch(e.target.value)} />
            </div>

            {/* Period Groups */}
            {PERIODS.map(period=>{
              const pts = filteredTasks.filter(t=>t.period===period);
              if(!pts.length) return null;
              return (
                <div key={period} style={styles.periodSection}>
                  <div style={styles.periodHeader}>{period}</div>
                  {pts.map(task=>(
                    <TaskCard
                      key={task.id}
                      task={task}
                      dragging={dragging===task.id}
                      dragOver={dragOver===task.id}
                      onToggle={()=>toggleTask(task.id)}
                      onEdit={()=>setEditTask(task)}
                      onDelete={()=>deleteTask(task.id)}
                      onPostpone={()=>postponeTask(task.id)}
                      onFocus={()=>{ setFocusTask(task); setTimer(task.duration*60); }}
                      onSubtask={(sid)=>{
                        const sub = task.subtasks.map(s=>s.id===sid?{...s,done:!s.done}:s);
                        updateTask(task.id,{subtasks:sub});
                      }}
                      onDragStart={()=>handleDragStart(task.id)}
                      onDragOver={(e)=>handleDragOver(e,task.id)}
                      onDrop={()=>handleDrop(task.id)}
                    />
                  ))}
                </div>
              );
            })}

            {/* Tasks without period */}
            {filteredTasks.filter(t=>!t.period||!PERIODS.includes(t.period)).map(task=>(
              <TaskCard
                key={task.id}
                task={task}
                dragging={dragging===task.id}
                dragOver={dragOver===task.id}
                onToggle={()=>toggleTask(task.id)}
                onEdit={()=>setEditTask(task)}
                onDelete={()=>deleteTask(task.id)}
                onPostpone={()=>postponeTask(task.id)}
                onFocus={()=>{ setFocusTask(task); setTimer(task.duration*60); }}
                onSubtask={(sid)=>{
                  const sub = task.subtasks.map(s=>s.id===sid?{...s,done:!s.done}:s);
                  updateTask(task.id,{subtasks:sub});
                }}
                onDragStart={()=>handleDragStart(task.id)}
                onDragOver={(e)=>handleDragOver(e,task.id)}
                onDrop={()=>handleDrop(task.id)}
              />
            ))}

            {filteredTasks.length===0 && (
              <div style={styles.empty}>
                <div style={{fontSize:48,marginBottom:12}}>✨</div>
                <div style={{color:"#475569"}}>لا توجد مهام {filterAspect?"في هذا الجانب":"اليوم"}</div>
              </div>
            )}

            {/* Add Task Button */}
            <button style={styles.addBtn} onClick={()=>setShowTaskForm(true)}>
              + إضافة مهمة
            </button>

            {/* Postponed reminder */}
            {(state.postponed||[]).length>0 && (
              <div style={styles.postponedBanner} onClick={()=>setView("postponed")}>
                ⏳ لديك {(state.postponed||[]).length} مهمة مؤجلة
              </div>
            )}
          </div>
        )}

        {/* WEEK VIEW */}
        {view==="week" && (
          <WeekView
            state={state}
            selectedDate={selectedDate}
            setSelectedDate={(d)=>{ setSelectedDate(d); setView("today"); }}
            onCopyDay={copyDay}
            onTemplateApply={(template, dateStr) => {
              const templateTasks = TEMPLATES_DATA[template] || [];
              update(s=>{
                s.tasks = {...s.tasks, [dateStr]: templateTasks.map(t=>({...t,id:uid(),done:false}))};
                return s;
              });
            }}
          />
        )}

        {/* POSTPONED VIEW */}
        {view==="postponed" && (
          <PostponedView
            postponed={state.postponed||[]}
            onReschedule={reschedulePostponed}
            onDelete={(id)=>update(s=>({...s,postponed:(s.postponed||[]).filter(x=>x.id!==id)}))}
          />
        )}

        {/* STATS VIEW */}
        {view==="stats" && (
          <StatsView state={state} stats={stats} />
        )}

        {/* SETTINGS VIEW */}
        {view==="settings" && (
          <SettingsView
            state={state}
            notifPerm={notifPerm}
            onRequestNotif={requestNotif}
            onUpdate={(changes)=>update(s=>({...s,...changes}))}
            onSettingsChange={(changes)=>update(s=>({...s,settings:{...s.settings,...changes}}))}
          />
        )}
      </main>
    </div>
  );
}

// ========================
// TASK CARD
// ========================
function TaskCard({ task, onToggle, onEdit, onDelete, onPostpone, onFocus, onSubtask, dragging, dragOver, onDragStart, onDragOver, onDrop }) {
  const [expanded, setExpanded] = useState(false);
  const priority = PRIORITIES.find(p=>p.id===task.priority)||PRIORITIES[2];
  const aspect = ASPECTS.find(a=>a.id===task.aspect);
  const endTime = task.time && task.duration ? (() => {
    const [h,m] = task.time.split(":").map(Number);
    const total = h*60+m+task.duration;
    return `${String(Math.floor(total/60)%24).padStart(2,"0")}:${String(total%60).padStart(2,"0")}`;
  })() : null;

  return (
    <div
      style={{...styles.taskCard(task.done, dragging, dragOver)}}
      draggable
      onDragStart={onDragStart}
      onDragOver={onDragOver}
      onDrop={onDrop}
    >
      {/* Priority stripe */}
      <div style={{...styles.priorityStripe, background:priority.color}} />

      <div style={{flex:1,minWidth:0}}>
        {/* Top row */}
        <div style={{display:"flex",alignItems:"center",gap:8}}>
          <button style={styles.checkBtn(task.done)} onClick={onToggle}>
            {task.done ? "✓" : ""}
          </button>
          <span style={styles.taskTitle(task.done)} onClick={()=>setExpanded(e=>!e)}>
            {task.pinned && <span style={{marginLeft:4}}>📌</span>}
            {task.title}
          </span>
          {task.priority==="critical" && <span style={styles.criticalDot} />}
        </div>

        {/* Meta */}
        <div style={styles.taskMeta}>
          {task.time && <span>🕐 {task.time}{endTime&&` → ${endTime}`}</span>}
          {task.duration && <span>⏱ {task.duration} د</span>}
          {aspect && <span style={{color:aspect.color}}>● {aspect.label}</span>}
        </div>

        {/* Subtasks */}
        {expanded && task.subtasks?.length > 0 && (
          <div style={styles.subtasks}>
            {task.subtasks.map(s=>(
              <div key={s.id} style={styles.subtaskItem} onClick={()=>onSubtask(s.id)}>
                <span style={styles.subCheck(s.done)}>{s.done?"✓":"○"}</span>
                <span style={{textDecoration:s.done?"line-through":"none",color:s.done?"#475569":"#94a3b8",fontSize:13}}>{s.title}</span>
              </div>
            ))}
          </div>
        )}
      </div>

      {/* Actions */}
      <div style={styles.taskActions}>
        <button style={styles.actBtn} onClick={onFocus} title="تركيز">⏱</button>
        <button style={styles.actBtn} onClick={onEdit} title="تعديل">✏</button>
        <button style={styles.actBtn} onClick={onPostpone} title="تأجيل">⏳</button>
        <button style={{...styles.actBtn,color:"#f87171"}} onClick={onDelete} title="حذف">✕</button>
      </div>
    </div>
  );
}

// ========================
// TASK FORM
// ========================
function TaskForm({ task, aspects, onSave, onClose }) {
  const [form, setForm] = useState(task || {
    title:"", priority:"normal", aspect:"other", period:"الصباح",
    time:"", duration:30, note:"", subtasks:[], pinned:false, recurring:false, fixed:false
  });
  const [newSub, setNewSub] = useState("");

  const set = (k,v) => setForm(f=>({...f,[k]:v}));

  return (
    <Modal onClose={onClose} title={task?"تعديل المهمة":"مهمة جديدة"}>
      <div style={styles.formGrid}>
        <div style={styles.formGroup}>
          <label style={styles.label}>عنوان المهمة</label>
          <input style={styles.input} value={form.title} onChange={e=>set("title",e.target.value)} placeholder="اكتب المهمة..." autoFocus />
        </div>
        <div style={styles.formRow}>
          <div style={styles.formGroup}>
            <label style={styles.label}>الأولوية</label>
            <select style={styles.select} value={form.priority} onChange={e=>set("priority",e.target.value)}>
              {PRIORITIES.map(p=><option key={p.id} value={p.id}>{p.label}</option>)}
            </select>
          </div>
          <div style={styles.formGroup}>
            <label style={styles.label}>الجانب</label>
            <select style={styles.select} value={form.aspect} onChange={e=>set("aspect",e.target.value)}>
              {aspects.map(a=><option key={a.id} value={a.id}>{a.label}</option>)}
            </select>
          </div>
        </div>
        <div style={styles.formRow}>
          <div style={styles.formGroup}>
            <label style={styles.label}>الفترة</label>
            <select style={styles.select} value={form.period} onChange={e=>set("period",e.target.value)}>
              {PERIODS.map(p=><option key={p} value={p}>{p}</option>)}
            </select>
          </div>
          <div style={styles.formGroup}>
            <label style={styles.label}>الوقت</label>
            <input style={styles.input} type="time" value={form.time} onChange={e=>set("time",e.target.value)} />
          </div>
          <div style={styles.formGroup}>
            <label style={styles.label}>المدة (د)</label>
            <input style={styles.input} type="number" value={form.duration} onChange={e=>set("duration",+e.target.value)} min={5} step={5} />
          </div>
        </div>
        <div style={styles.formGroup}>
          <label style={styles.label}>ملاحظات</label>
          <textarea style={{...styles.input,height:60,resize:"vertical"}} value={form.note} onChange={e=>set("note",e.target.value)} placeholder="ملاحظات اختيارية..." />
        </div>
        {/* Subtasks */}
        <div style={styles.formGroup}>
          <label style={styles.label}>الخطوات الفرعية</label>
          {(form.subtasks||[]).map((s,i)=>(
            <div key={s.id} style={{display:"flex",gap:6,marginBottom:4}}>
              <span style={{color:"#64748b",flex:1,fontSize:13}}>{s.title}</span>
              <button style={styles.actBtn} onClick={()=>set("subtasks",(form.subtasks||[]).filter((_,j)=>j!==i))}>✕</button>
            </div>
          ))}
          <div style={{display:"flex",gap:6}}>
            <input style={{...styles.input,flex:1}} value={newSub} onChange={e=>setNewSub(e.target.value)} placeholder="خطوة جديدة..." onKeyDown={e=>{ if(e.key==="Enter"&&newSub.trim()){ set("subtasks",[...(form.subtasks||[]),{id:uid(),title:newSub.trim(),done:false}]); setNewSub(""); }}} />
            <button style={styles.btnSmall} onClick={()=>{ if(newSub.trim()){ set("subtasks",[...(form.subtasks||[]),{id:uid(),title:newSub.trim(),done:false}]); setNewSub(""); }}}>+</button>
          </div>
        </div>
        {/* Toggles */}
        <div style={{display:"flex",gap:12,flexWrap:"wrap"}}>
          {[["pinned","📌 مثبتة"],["recurring","🔄 متكررة"],["fixed","🔒 ثابتة"]].map(([k,l])=>(
            <label key={k} style={styles.toggle}>
              <input type="checkbox" checked={!!form[k]} onChange={e=>set(k,e.target.checked)} style={{marginLeft:6}} />
              {l}
            </label>
          ))}
        </div>
      </div>
      <div style={{display:"flex",gap:8,marginTop:16}}>
        <button style={styles.btnPrimary} onClick={()=>form.title.trim()&&onSave(form)}>حفظ</button>
        <button style={styles.btnGhost} onClick={onClose}>إلغاء</button>
      </div>
    </Modal>
  );
}

// ========================
// WEEK VIEW
// ========================
function WeekView({ state, selectedDate, setSelectedDate, onCopyDay, onTemplateApply }) {
  const weekDays = getWeekDays(today());
  const [copyFrom, setCopyFrom] = useState(null);

  return (
    <div>
      <div style={styles.sectionTitle}>الجدول الأسبوعي</div>
      
      {/* Templates */}
      <div style={{marginBottom:16}}>
        <div style={{color:"#64748b",fontSize:12,marginBottom:8}}>قوالب جاهزة</div>
        <div style={{display:"flex",gap:6,flexWrap:"wrap"}}>
          {TEMPLATES.map(t=>(
            <button key={t.id} style={styles.templateChip}>
              {t.icon} {t.label}
            </button>
          ))}
        </div>
      </div>

      {/* Seasonal */}
      <div style={{marginBottom:16}}>
        <div style={{color:"#64748b",fontSize:12,marginBottom:8}}>الوضع الموسمي</div>
        <div style={{display:"flex",gap:6,flexWrap:"wrap"}}>
          {SEASONAL.map(s=>(
            <button key={s.id} style={styles.templateChip}>{s.icon} {s.label}</button>
          ))}
        </div>
      </div>

      {/* Week grid */}
      <div style={styles.weekGrid}>
        {weekDays.map(d=>{
          const tasks = state.tasks[d]||[];
          const pct = calcCompletion(tasks);
          const isToday = d===today();
          return (
            <div key={d} style={styles.weekDayCard(isToday)} onClick={()=>setSelectedDate(d)}>
              <div style={styles.weekDayName(isToday)}>{dateLabel(d).split(" ")[0]}</div>
              <div style={{fontSize:11,color:"#475569",marginBottom:8}}>{d.slice(5)}</div>
              <div style={{...styles.weekPct, color:completionColor(pct)}}>{pct}%</div>
              <div style={styles.weekProgressBar}>
                <div style={{...styles.weekProgressFill,width:`${pct}%`,background:completionColor(pct)}} />
              </div>
              <div style={{fontSize:11,color:"#475569",marginTop:4}}>{tasks.filter(t=>t.done).length}/{tasks.length}</div>
              <button style={styles.copyDayBtn} onClick={e=>{e.stopPropagation();setCopyFrom(d);}}>نسخ</button>
            </div>
          );
        })}
      </div>

      {copyFrom && (
        <Modal onClose={()=>setCopyFrom(null)} title={`نسخ يوم ${dateLabel(copyFrom)}`}>
          <p style={{color:"#94a3b8",marginBottom:12}}>انسخ مهام هذا اليوم إلى:</p>
          <div style={{display:"flex",gap:6,flexWrap:"wrap"}}>
            {weekDays.filter(d=>d!==copyFrom).map(d=>(
              <button key={d} style={styles.btnSmall} onClick={()=>{ onCopyDay(copyFrom,d); setCopyFrom(null); }}>
                {dateLabel(d)}
              </button>
            ))}
          </div>
        </Modal>
      )}
    </div>
  );
}

// ========================
// POSTPONED VIEW
// ========================
function PostponedView({ postponed, onReschedule, onDelete }) {
  const [reschedDate, setReschedDate] = useState(today());

  return (
    <div>
      <div style={styles.sectionTitle}>المهام المؤجلة ({postponed.length})</div>
      {postponed.length===0 && <div style={styles.empty}><div style={{fontSize:48}}>🎉</div><div style={{color:"#475569",marginTop:8}}>لا توجد مهام مؤجلة</div></div>}
      {postponed.map(t=>(
        <div key={t.id} style={styles.postponedCard}>
          <div style={{flex:1}}>
            <div style={{color:"#e2e8f0",fontWeight:600,marginBottom:4}}>{t.title}</div>
            <div style={{fontSize:12,color:"#64748b"}}>من: {t.originalDate} • {PRIORITIES.find(p=>p.id===t.priority)?.label}</div>
          </div>
          <div style={{display:"flex",gap:6,alignItems:"center"}}>
            <input type="date" style={{...styles.input,width:130,fontSize:12}} value={reschedDate} onChange={e=>setReschedDate(e.target.value)} />
            <button style={styles.btnSmall} onClick={()=>onReschedule(t.id,reschedDate)}>جدولة</button>
            <button style={{...styles.actBtn,color:"#f87171"}} onClick={()=>onDelete(t.id)}>✕</button>
          </div>
        </div>
      ))}
    </div>
  );
}

// ========================
// STATS VIEW
// ========================
function StatsView({ state, stats }) {
  return (
    <div>
      <div style={styles.sectionTitle}>الإحصائيات والإنجازات</div>
      
      <div style={{display:"grid",gridTemplateColumns:"repeat(2,1fr)",gap:12,marginBottom:16}}>
        {[
          {label:"إنجاز اليوم",value:`${stats.todayPct}%`,color:completionColor(stats.todayPct)},
          {label:"إنجاز الأسبوع",value:`${stats.weekPct}%`,color:completionColor(stats.weekPct)},
          {label:"إنجاز الشهر",value:`${stats.monthPct}%`,color:completionColor(stats.monthPct)},
          {label:"مؤجلات",value:stats.postponedCount,color:"#f87171"},
        ].map(s=>(
          <div key={s.label} style={styles.bigStatCard}>
            <div style={{...styles.bigStatNum,color:s.color}}>{s.value}</div>
            <div style={{color:"#64748b",fontSize:13}}>{s.label}</div>
          </div>
        ))}
      </div>

      {/* Aspects breakdown */}
      <div style={styles.card}>
        <div style={{color:"#94a3b8",fontSize:13,marginBottom:12}}>الإنجاز حسب الجانب</div>
        {ASPECTS.map(a=>{
          const allTasks = Object.values(state.tasks).flat().filter(t=>t.aspect===a.id);
          const done = allTasks.filter(t=>t.done).length;
          const pct = allTasks.length?Math.round(done/allTasks.length*100):0;
          return (
            <div key={a.id} style={{marginBottom:8}}>
              <div style={{display:"flex",justifyContent:"space-between",marginBottom:3}}>
                <span style={{color:"#94a3b8",fontSize:12}}>{a.label}</span>
                <span style={{color:a.color,fontSize:12}}>{pct}%</span>
              </div>
              <div style={styles.miniProgress}>
                <div style={{height:"100%",width:`${pct}%`,background:a.color,borderRadius:2,transition:"width .5s"}} />
              </div>
            </div>
          );
        })}
      </div>

      {/* Achievements */}
      <div style={{...styles.card,marginTop:12}}>
        <div style={{color:"#94a3b8",fontSize:13,marginBottom:12}}>🏆 الإنجازات</div>
        <div style={{color:"#64748b",fontSize:13}}>
          <div style={{marginBottom:6}}>أفضل إنجاز هذا الأسبوع: <span style={{color:"#34d399"}}>{stats.bestWeek}%</span></div>
          <div style={{marginBottom:6}}>مهام مكتملة اليوم: <span style={{color:"#60a5fa"}}>{stats.todayDone}</span></div>
        </div>
      </div>
    </div>
  );
}

// ========================
// SETTINGS VIEW
// ========================
function SettingsView({ state, notifPerm, onRequestNotif, onUpdate, onSettingsChange }) {
  return (
    <div>
      <div style={styles.sectionTitle}>الإعدادات</div>

      <div style={styles.card}>
        <div style={styles.settingRow}>
          <div>
            <div style={{color:"#e2e8f0",fontSize:14}}>الوضع الليلي</div>
            <div style={{color:"#64748b",fontSize:12}}>تبديل بين الوضع الليلي والنهاري</div>
          </div>
          <button style={styles.toggle2(state.settings.theme==="dark")} onClick={()=>onSettingsChange({theme:state.settings.theme==="dark"?"light":"dark"})}>
            <div style={styles.toggleKnob(state.settings.theme==="dark")} />
          </button>
        </div>

        <div style={styles.settingRow}>
          <div>
            <div style={{color:"#e2e8f0",fontSize:14}}>الإشعارات</div>
            <div style={{color:"#64748b",fontSize:12}}>
              {notifPerm==="granted"?"مفعّلة":"يتطلب الإذن"}
            </div>
          </div>
          {notifPerm!=="granted" ? (
            <button style={styles.btnSmall} onClick={onRequestNotif}>تفعيل</button>
          ) : (
            <button style={styles.toggle2(state.settings.notifications)} onClick={()=>onSettingsChange({notifications:!state.settings.notifications})}>
              <div style={styles.toggleKnob(state.settings.notifications)} />
            </button>
          )}
        </div>

        <div style={styles.settingRow}>
          <div>
            <div style={{color:"#e2e8f0",fontSize:14}}>تنبيه قبل المهمة</div>
          </div>
          <select style={{...styles.select,width:100}} value={state.settings.notifyBefore} onChange={e=>onSettingsChange({notifyBefore:+e.target.value})}>
            {[5,10,15,30].map(n=><option key={n} value={n}>{n} دقيقة</option>)}
          </select>
        </div>
      </div>

      {/* Habits */}
      <div style={{...styles.card,marginTop:12}}>
        <div style={{color:"#94a3b8",fontSize:13,marginBottom:12}}>🔄 العادات والروتين</div>
        {(state.habits||[]).map(h=>(
          <div key={h.id} style={{display:"flex",justifyContent:"space-between",alignItems:"center",marginBottom:8}}>
            <span style={{color:"#94a3b8",fontSize:13}}>{h.title}</span>
            <span style={{color:"#64748b",fontSize:12}}>{h.time}</span>
          </div>
        ))}
        <button style={styles.btnSmall}>+ إضافة عادة</button>
      </div>

      {/* Reset */}
      <div style={{...styles.card,marginTop:12}}>
        <div style={{color:"#94a3b8",fontSize:13,marginBottom:12}}>⚠️ إعادة ضبط</div>
        <button style={styles.btnDanger} onClick={()=>{ if(confirm("هل تريد حذف جميع البيانات؟")){ localStorage.removeItem("lm_v2"); window.location.reload(); }}}>
          حذف جميع البيانات
        </button>
      </div>
    </div>
  );
}

// ========================
// FOCUS MODE
// ========================
function FocusMode({ task, timer, setTimer, timerRunning, setTimerRunning, onClose, onDone }) {
  const fmt = (s) => s!=null ? `${String(Math.floor(s/60)).padStart(2,"0")}:${String(s%60).padStart(2,"0")}` : "--:--";
  const pct = task.duration&&timer!=null ? Math.round((1-timer/(task.duration*60))*100) : 0;

  return (
    <div style={styles.focusOverlay}>
      <div style={styles.focusCard}>
        <button style={{...styles.actBtn,position:"absolute",top:16,left:16}} onClick={onClose}>✕</button>
        <div style={styles.focusTitle}>{task.title}</div>
        <div style={styles.focusTimer}>{fmt(timer)}</div>
        <div style={styles.focusProgress}>
          <div style={{...styles.focusProgressFill,width:`${pct}%`}} />
        </div>
        <div style={{display:"flex",gap:12,justifyContent:"center",marginTop:24}}>
          <button style={styles.btnPrimary} onClick={()=>setTimerRunning(r=>!r)}>
            {timerRunning?"⏸ إيقاف":"▶ بدء"}
          </button>
          <button style={{...styles.btnPrimary,background:"#34d399"}} onClick={onDone}>✓ إنجاز</button>
        </div>
        {task.note&&<div style={{color:"#64748b",fontSize:13,marginTop:16,textAlign:"center"}}>{task.note}</div>}
      </div>
    </div>
  );
}

// ========================
// REVIEW MODAL
// ========================
function ReviewModal({ dateStr, review, onSave, onClose }) {
  const [form, setForm] = useState(review);
  return (
    <Modal onClose={onClose} title={`مراجعة ${dateLabel(dateStr)}`}>
      {[["what_worked","✅ ماذا نجح؟"],["what_failed","❌ لماذا لم يتم الإنجاز؟"],["how_was_day","💭 كيف كان اليوم؟"]].map(([k,l])=>(
        <div key={k} style={styles.formGroup}>
          <label style={styles.label}>{l}</label>
          <textarea style={{...styles.input,height:70,resize:"vertical"}} value={form[k]||""} onChange={e=>setForm(f=>({...f,[k]:e.target.value}))} />
        </div>
      ))}
      <div style={{display:"flex",gap:8,marginTop:12}}>
        <button style={styles.btnPrimary} onClick={()=>onSave(form)}>حفظ</button>
        <button style={styles.btnGhost} onClick={onClose}>إلغاء</button>
      </div>
    </Modal>
  );
}

// ========================
// MODAL WRAPPER
// ========================
function Modal({ children, onClose, title }) {
  return (
    <div style={styles.modalOverlay} onClick={e=>e.target===e.currentTarget&&onClose()}>
      <div style={styles.modalCard}>
        <div style={styles.modalHeader}>
          <div style={styles.modalTitle}>{title}</div>
          <button style={styles.actBtn} onClick={onClose}>✕</button>
        </div>
        {children}
      </div>
    </div>
  );
}

// ========================
// STYLES
// ========================
const DARK = {
  bg:"#0a0f1e", bg2:"#0f172a", bg3:"#1e293b", bg4:"#293548",
  text:"#e2e8f0", text2:"#94a3b8", text3:"#64748b",
  border:"rgba(255,255,255,0.06)", accent:"#6ee7b7", shadow:"rgba(0,0,0,0.4)"
};
const LIGHT = {
  bg:"#f8fafc", bg2:"#ffffff", bg3:"#f1f5f9", bg4:"#e2e8f0",
  text:"#1e293b", text2:"#475569", text3:"#94a3b8",
  border:"rgba(0,0,0,0.08)", accent:"#059669", shadow:"rgba(0,0,0,0.1)"
};

const th = (t) => t==="dark"?DARK:LIGHT;

const styles = {
  root: (t) => ({
    minHeight:"100vh", background:th(t).bg, color:th(t).text,
    fontFamily:"'Tajawal', 'Cairo', 'Segoe UI', sans-serif",
    direction:"rtl", position:"relative", overflowX:"hidden",
    fontSize:15,
  }),
  bg: {
    position:"fixed", inset:0, pointerEvents:"none", zIndex:0,
    background:"radial-gradient(ellipse 80% 50% at 20% -10%, rgba(16,185,129,0.06) 0%, transparent 60%), radial-gradient(ellipse 60% 40% at 80% 110%, rgba(99,102,241,0.05) 0%, transparent 60%)",
  },
  header: {
    display:"flex", alignItems:"center", justifyContent:"space-between",
    padding:"16px 20px", borderBottom:`1px solid rgba(255,255,255,0.06)`,
    position:"sticky", top:0, zIndex:100,
    background:"rgba(10,15,30,0.85)", backdropFilter:"blur(20px)",
  },
  headerLeft: { display:"flex", alignItems:"center", gap:12 },
  headerRight: { display:"flex", alignItems:"center", gap:8 },
  logo: { display:"flex", alignItems:"center", gap:6 },
  logoMark: { fontSize:22, color:"#6ee7b7", lineHeight:1 },
  logoText: { fontSize:20, fontWeight:800, letterSpacing:"-0.5px", color:"#e2e8f0" },
  dateChip: {
    background:"rgba(110,231,183,0.08)", border:"1px solid rgba(110,231,183,0.15)",
    color:"#6ee7b7", padding:"3px 10px", borderRadius:20, fontSize:12
  },
  iconBtn: {
    background:"none", border:"none", cursor:"pointer", fontSize:18, padding:6, borderRadius:8,
    color:"#94a3b8", transition:"all .2s"
  },
  emergencyBtn: {
    background:"rgba(248,113,113,0.1)", border:"1px solid rgba(248,113,113,0.2)",
    color:"#f87171", padding:"5px 12px", borderRadius:8, fontSize:12, cursor:"pointer",
    fontFamily:"inherit", transition:"all .2s"
  },
  nav: {
    display:"flex", gap:2, padding:"8px 16px", overflowX:"auto",
    borderBottom:"1px solid rgba(255,255,255,0.04)",
    background:"rgba(15,23,42,0.6)", backdropFilter:"blur(10px)",
    position:"sticky", top:57, zIndex:99,
  },
  navBtn: (active) => ({
    display:"flex", alignItems:"center", gap:5, padding:"7px 14px", borderRadius:8,
    border:"none", cursor:"pointer", fontFamily:"inherit", fontSize:13, fontWeight:600,
    whiteSpace:"nowrap", transition:"all .2s", position:"relative",
    background: active?"rgba(110,231,183,0.12)":"none",
    color: active?"#6ee7b7":"#64748b",
    borderBottom: active?"2px solid #6ee7b7":"2px solid transparent",
  }),
  badge: {
    background:"#f87171", color:"#fff", fontSize:10, borderRadius:20,
    padding:"1px 5px", marginRight:2, fontWeight:700
  },
  main: {
    maxWidth:700, margin:"0 auto", padding:"20px 16px 80px",
    position:"relative", zIndex:1
  },
  statsBar: {
    display:"grid", gridTemplateColumns:"repeat(4,1fr)", gap:8, marginBottom:16
  },
  statCard: {
    background:"rgba(30,41,59,0.6)", border:"1px solid rgba(255,255,255,0.05)",
    borderRadius:12, padding:"12px 8px", textAlign:"center", backdropFilter:"blur(8px)"
  },
  statNum: { fontSize:22, fontWeight:800, lineHeight:1, marginBottom:3 },
  statLabel: { fontSize:11, color:"#475569" },
  progressWrap: {
    display:"flex", alignItems:"center", gap:10, marginBottom:16
  },
  progressBar: {
    flex:1, height:6, background:"rgba(255,255,255,0.06)", borderRadius:10, overflow:"hidden"
  },
  progressFill: {
    height:"100%", borderRadius:10, transition:"width .6s cubic-bezier(.4,0,.2,1)"
  },
  card2: {
    background:"rgba(30,41,59,0.5)", border:"1px solid rgba(255,255,255,0.05)",
    borderRadius:12, padding:"12px", flex:1, minWidth:0
  },
  card2Btn: {
    background:"rgba(110,231,183,0.08)", border:"1px solid rgba(110,231,183,0.15)",
    color:"#6ee7b7", padding:"12px 16px", borderRadius:12, cursor:"pointer",
    fontFamily:"inherit", fontSize:13, fontWeight:600, whiteSpace:"nowrap"
  },
  energyBtn: (active) => ({
    background: active?"rgba(110,231,183,0.15)":"rgba(255,255,255,0.04)",
    border: active?"1px solid rgba(110,231,183,0.3)":"1px solid rgba(255,255,255,0.06)",
    color: active?"#6ee7b7":"#64748b",
    padding:"3px 8px", borderRadius:6, fontSize:11, cursor:"pointer",
    fontFamily:"inherit", transition:"all .2s"
  }),
  aspectsRow: {
    display:"flex", gap:6, flexWrap:"wrap", marginBottom:12
  },
  aspectChip: (active, color) => ({
    background: active?(color?`${color}22`:"rgba(110,231,183,0.15)"):"rgba(255,255,255,0.04)",
    border: active?`1px solid ${color||"#6ee7b7"}44`:"1px solid rgba(255,255,255,0.06)",
    color: active?(color||"#6ee7b7"):"#64748b",
    padding:"4px 12px", borderRadius:20, fontSize:12, cursor:"pointer",
    fontFamily:"inherit", transition:"all .2s", fontWeight:500
  }),
  searchWrap: {
    display:"flex", alignItems:"center", gap:8,
    background:"rgba(255,255,255,0.04)", border:"1px solid rgba(255,255,255,0.07)",
    borderRadius:10, padding:"8px 12px", marginBottom:16
  },
  searchIcon: { fontSize:14, opacity:.5 },
  searchInput: {
    background:"none", border:"none", outline:"none", color:"#e2e8f0",
    fontFamily:"inherit", fontSize:13, flex:1
  },
  periodSection: { marginBottom:16 },
  periodHeader: {
    fontSize:11, fontWeight:700, color:"#475569", letterSpacing:2,
    textTransform:"uppercase", marginBottom:6, paddingRight:4
  },
  taskCard: (done, dragging, dragOver) => ({
    display:"flex", alignItems:"flex-start", gap:8,
    background: done?"rgba(52,211,153,0.04)":"rgba(30,41,59,0.55)",
    border: dragOver?"1px solid #6ee7b7":"1px solid rgba(255,255,255,0.06)",
    borderRadius:12, padding:"12px", marginBottom:8,
    opacity: done?.6:1, cursor:"grab",
    transition:"all .2s", position:"relative", overflow:"hidden",
    backdropFilter:"blur(8px)",
    transform: dragging?"scale(0.98)":"scale(1)",
    boxShadow: dragOver?"0 0 0 2px rgba(110,231,183,0.2)":"none",
  }),
  priorityStripe: {
    position:"absolute", right:0, top:0, bottom:0, width:3, borderRadius:"0 12px 12px 0"
  },
  checkBtn: (done) => ({
    width:22, height:22, minWidth:22, borderRadius:6,
    border: done?"2px solid #34d399":"2px solid rgba(255,255,255,0.15)",
    background: done?"#34d399":"none",
    color:"#0a0f1e", fontSize:12, fontWeight:700,
    cursor:"pointer", display:"flex", alignItems:"center", justifyContent:"center",
    transition:"all .2s", flexShrink:0, marginTop:2
  }),
  taskTitle: (done) => ({
    flex:1, fontWeight:600, fontSize:14, cursor:"pointer",
    textDecoration: done?"line-through":"none",
    color: done?"#475569":"#e2e8f0",
    transition:"all .2s"
  }),
  criticalDot: {
    width:6, height:6, borderRadius:"50%", background:"#f87171", flexShrink:0
  },
  taskMeta: {
    display:"flex", gap:10, flexWrap:"wrap", marginTop:4, paddingRight:30
  },
  subtasks: { marginTop:8, marginRight:30, borderTop:"1px solid rgba(255,255,255,0.04)", paddingTop:8 },
  subtaskItem: {
    display:"flex", alignItems:"center", gap:6, marginBottom:4,
    cursor:"pointer", padding:"2px 0"
  },
  subCheck: (done) => ({
    color: done?"#34d399":"#475569", fontSize:12
  }),
  taskActions: {
    display:"flex", flexDirection:"column", gap:3, flexShrink:0
  },
  actBtn: {
    background:"none", border:"none", cursor:"pointer", color:"#475569",
    fontSize:13, padding:"3px 5px", borderRadius:4, fontFamily:"inherit",
    transition:"color .2s"
  },
  addBtn: {
    width:"100%", background:"rgba(110,231,183,0.08)", border:"1px dashed rgba(110,231,183,0.2)",
    color:"#6ee7b7", padding:"12px", borderRadius:12, fontSize:14, fontWeight:600,
    cursor:"pointer", fontFamily:"inherit", marginTop:8, transition:"all .2s"
  },
  postponedBanner: {
    marginTop:12, background:"rgba(248,113,113,0.08)", border:"1px solid rgba(248,113,113,0.15)",
    color:"#f87171", padding:"10px 14px", borderRadius:10, fontSize:13,
    cursor:"pointer", textAlign:"center"
  },
  // Week view
  weekGrid: {
    display:"grid", gridTemplateColumns:"repeat(auto-fill,minmax(90px,1fr))", gap:8
  },
  weekDayCard: (isToday) => ({
    background: isToday?"rgba(110,231,183,0.08)":"rgba(30,41,59,0.5)",
    border: isToday?"1px solid rgba(110,231,183,0.25)":"1px solid rgba(255,255,255,0.05)",
    borderRadius:12, padding:"12px 8px", textAlign:"center", cursor:"pointer",
    transition:"all .2s"
  }),
  weekDayName: (isToday) => ({
    fontSize:13, fontWeight:700, color: isToday?"#6ee7b7":"#94a3b8", marginBottom:2
  }),
  weekPct: { fontSize:18, fontWeight:800, margin:"4px 0" },
  weekProgressBar: {
    height:3, background:"rgba(255,255,255,0.06)", borderRadius:10, overflow:"hidden"
  },
  weekProgressFill: { height:"100%", borderRadius:10, transition:"width .5s" },
  copyDayBtn: {
    marginTop:6, background:"rgba(255,255,255,0.06)", border:"none",
    color:"#64748b", padding:"2px 8px", borderRadius:6, fontSize:11, cursor:"pointer", fontFamily:"inherit"
  },
  templateChip: {
    background:"rgba(255,255,255,0.05)", border:"1px solid rgba(255,255,255,0.08)",
    color:"#94a3b8", padding:"5px 12px", borderRadius:8, fontSize:12,
    cursor:"pointer", fontFamily:"inherit", transition:"all .2s"
  },
  // Postponed
  postponedCard: {
    display:"flex", alignItems:"center", gap:8, flexWrap:"wrap",
    background:"rgba(30,41,59,0.5)", border:"1px solid rgba(255,255,255,0.05)",
    borderRadius:12, padding:"12px", marginBottom:8
  },
  // Stats
  bigStatCard: {
    background:"rgba(30,41,59,0.6)", border:"1px solid rgba(255,255,255,0.05)",
    borderRadius:14, padding:"20px 12px", textAlign:"center", backdropFilter:"blur(8px)"
  },
  bigStatNum: { fontSize:32, fontWeight:900, lineHeight:1, marginBottom:4 },
  card: {
    background:"rgba(30,41,59,0.55)", border:"1px solid rgba(255,255,255,0.05)",
    borderRadius:14, padding:"16px", backdropFilter:"blur(8px)"
  },
  miniProgress: {
    height:4, background:"rgba(255,255,255,0.06)", borderRadius:4, overflow:"hidden"
  },
  // Focus
  focusOverlay: {
    position:"fixed", inset:0, background:"rgba(0,0,0,0.92)", zIndex:200,
    display:"flex", alignItems:"center", justifyContent:"center", padding:20,
    backdropFilter:"blur(20px)"
  },
  focusCard: {
    background:"#0f172a", border:"1px solid rgba(110,231,183,0.15)",
    borderRadius:24, padding:"40px 32px", textAlign:"center", maxWidth:380,
    width:"100%", position:"relative"
  },
  focusTitle: {
    fontSize:22, fontWeight:800, color:"#e2e8f0", marginBottom:8, lineHeight:1.4
  },
  focusTimer: {
    fontSize:64, fontWeight:900, color:"#6ee7b7", letterSpacing:"-2px",
    fontVariantNumeric:"tabular-nums", margin:"16px 0"
  },
  focusProgress: {
    height:4, background:"rgba(255,255,255,0.08)", borderRadius:10, overflow:"hidden", margin:"0 8px"
  },
  focusProgressFill: {
    height:"100%", background:"#6ee7b7", borderRadius:10, transition:"width .5s"
  },
  // Modal
  modalOverlay: {
    position:"fixed", inset:0, background:"rgba(0,0,0,0.7)", zIndex:150,
    display:"flex", alignItems:"center", justifyContent:"center", padding:16,
    backdropFilter:"blur(8px)"
  },
  modalCard: {
    background:"#0f172a", border:"1px solid rgba(255,255,255,0.08)",
    borderRadius:20, padding:"20px", maxWidth:520, width:"100%",
    maxHeight:"90vh", overflowY:"auto"
  },
  modalHeader: {
    display:"flex", alignItems:"center", justifyContent:"space-between", marginBottom:16
  },
  modalTitle: { fontSize:17, fontWeight:700, color:"#e2e8f0" },
  // Form
  formGrid: { display:"flex", flexDirection:"column", gap:12 },
  formRow: { display:"flex", gap:8, flexWrap:"wrap" },
  formGroup: { display:"flex", flexDirection:"column", gap:4, flex:1, minWidth:120 },
  label: { fontSize:12, color:"#64748b", fontWeight:600 },
  input: {
    background:"rgba(255,255,255,0.05)", border:"1px solid rgba(255,255,255,0.1)",
    borderRadius:8, padding:"8px 10px", color:"#e2e8f0", fontFamily:"inherit",
    fontSize:13, outline:"none", transition:"border .2s"
  },
  select: {
    background:"rgba(255,255,255,0.05)", border:"1px solid rgba(255,255,255,0.1)",
    borderRadius:8, padding:"8px 10px", color:"#e2e8f0", fontFamily:"inherit",
    fontSize:13, outline:"none"
  },
  toggle: { display:"flex", alignItems:"center", cursor:"pointer", color:"#94a3b8", fontSize:13 },
  // Settings
  settingRow: {
    display:"flex", alignItems:"center", justifyContent:"space-between",
    padding:"12px 0", borderBottom:"1px solid rgba(255,255,255,0.05)"
  },
  toggle2: (active) => ({
    width:44, height:24, borderRadius:12, border:"none", cursor:"pointer",
    background: active?"#6ee7b7":"rgba(255,255,255,0.1)",
    position:"relative", transition:"all .3s", flexShrink:0
  }),
  toggleKnob: (active) => ({
    position:"absolute", top:3, right: active?"3px":"21px",
    width:18, height:18, borderRadius:"50%",
    background: active?"#0f172a":"#475569", transition:"all .3s"
  }),
  // Buttons
  btnPrimary: {
    background:"linear-gradient(135deg,#34d399,#059669)", border:"none",
    color:"#fff", padding:"10px 20px", borderRadius:10, fontSize:14,
    fontWeight:700, cursor:"pointer", fontFamily:"inherit", transition:"all .2s"
  },
  btnSmall: {
    background:"rgba(110,231,183,0.1)", border:"1px solid rgba(110,231,183,0.2)",
    color:"#6ee7b7", padding:"5px 12px", borderRadius:8, fontSize:12,
    cursor:"pointer", fontFamily:"inherit", fontWeight:600
  },
  btnGhost: {
    background:"rgba(255,255,255,0.05)", border:"1px solid rgba(255,255,255,0.1)",
    color:"#94a3b8", padding:"10px 20px", borderRadius:10, fontSize:14,
    cursor:"pointer", fontFamily:"inherit", transition:"all .2s"
  },
  btnDanger: {
    background:"rgba(248,113,113,0.1)", border:"1px solid rgba(248,113,113,0.2)",
    color:"#f87171", padding:"8px 16px", borderRadius:8, fontSize:13,
    cursor:"pointer", fontFamily:"inherit"
  },
  sectionTitle: {
    fontSize:18, fontWeight:800, color:"#e2e8f0", marginBottom:16, letterSpacing:"-0.3px"
  },
  empty: {
    textAlign:"center", padding:"40px 20px", color:"#475569"
  },
};

// Template data
const TEMPLATES_DATA = {
  study: [
    {title:"أذكار الصباح",priority:"critical",aspect:"faith",period:"الفجر",time:"06:00",duration:15,done:false,note:"",subtasks:[],pinned:true,recurring:true,fixed:false},
    {title:"مراجعة المحاضرات",priority:"high",aspect:"science",period:"الصباح",time:"09:00",duration:90,done:false,note:"",subtasks:[],pinned:false,recurring:false,fixed:false},
    {title:"حل التمارين",priority:"high",aspect:"science",period:"الصباح",time:"11:00",duration:60,done:false,note:"",subtasks:[],pinned:false,recurring:false,fixed:false},
    {title:"قراءة",priority:"normal",aspect:"culture",period:"الليل",time:"21:00",duration:30,done:false,note:"",subtasks:[],pinned:false,recurring:false,fixed:false},
  ],
};
