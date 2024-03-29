/* See LICENSE file for copyright and license details. */

/* Audio */
#include <X11/X.h>
#include <X11/XF86keysym.h>

/* Add somewhere in your constants definition section */
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+1%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-1%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };

/* If you use amixer, use this instead. Thanks go to DaniOrt3ga. */
// static const char *upvol[] = { "/usr/bin/amixer", "set", "Master", "5%+", NULL };
// static const char *downvol[] = { "/usr/bin/amixer", "set", "Master", "5%-", NULL };
// static const char *mutevol[] = { "/usr/bin/amixerl", "set", "Master", "toggle", NULL };

/* To use light add this to the constant definition section. Thanks Hritik14. */
static const char *light_up[] = {"/usr/bin/light", "-A", "5", NULL};
static const char *light_down[] = {"/usr/bin/light", "-U", "5", NULL};

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
// static const char *fonts[]          = { "JetBrains Mono Medium:size=14" };
static const char *fonts[]          = { "SF Pro Display:style=Black:size=16" };
static const char dmenufont[]       = "monospace:size=20";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#34182e";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"alacritty",   "--class", "spterm",   "--config-file", "/home/agent/.config/alacritty/scratchpad.yml", NULL };
const char *spcmd2[] = {"alacritty",   "--class", "spfm",     "--config-file", "/home/agent/.config/alacritty/scratchpad.yml", NULL };
const char *spcmd3[] = {"thunar",      NULL };
const char *spcmd4[] = {"speedcrunch", NULL };

static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"spthunar",    spcmd3},
	{"spcalc",      spcmd4},
};

/* tagging */
// static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tags[] = { "One", "Chrome", "VSCode", "Four", "Five", "Six", "Seven", "Eight",  "Nine" };
// static const char *tags[] = { "", "", "", "", "", "", "",  "", "" };
// static const char *tags[] = { "📖", "🌎", "🐛", "🖌","ext" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance    title       tags mask     isfloating   monitor */
	// { "Sublime_text",       NULL,       NULL,       1,            0,           -1 },
	// { "Google-chrome",      NULL,       NULL,       2,            0,           -1 },
	// { "jetbrains-clion",    NULL,       NULL,       8,            0,           -1 },
    { NULL,		           "spterm",	NULL,		SPTAG(0),	  1,           -1 },
    { NULL,		           "spfm",		NULL,		SPTAG(1),	  1,           -1 },
    { NULL,		           "thunar",	NULL,		SPTAG(2),	  1,           -1 },
    { NULL,		       "speedcrunch",   NULL,		SPTAG(3),	  1,           -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
//static const char *sublime[]  = { "subl", NULL };
static const char *vscode[]  = { "code", NULL };
static const char *chrome[]  =  { "google-chrome-stable", NULL };
static const char *brave[]  =  { "firefox", NULL };
// static const char *clion[]   =  { "clion", NULL };
//static const char *thunar[]  =  { "thunar", NULL };
// static const char *ranger[]  =  { "alacritty", "-e", "ranger", NULL };
static const char *flameshot[]  = { "flameshot", "gui", NULL };





static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_grave,      spawn,          {.v = brave } },
	{ MODKEY,                       XK_F7,     spawn,          {.v = vscode } },
	{ MODKEY,                       XK_grave,  spawn,          {.v = chrome } },
	{ MODKEY,                       XK_u,      shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_o,      shiftview,      {.i = +1 } },
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ Mod1Mask,                     XK_Tab,    focusstack,     {.i = -1 } },
	{ 0,                            0x1008ffb2,focusstack,     {.i = -1 } }, //F20 // F19 = 0x1008ffa9
 // { MODKEY,                       XK_s,      incnmaster,     {.i = +1 } },
 // { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_w,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY,            			XK_y,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY,            			XK_i,	   togglescratch,  {.ui = 1 } },
    { MODKEY,            			XK_p,	   togglescratch,  {.ui = 2 } },
    { MODKEY,            			XK_n,	   togglescratch,  {.ui = 3 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_grave,                  1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_F7,                     2)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },


    { MODKEY,                       XK_Down,   spawn, {.v = downvol } },
    { MODKEY,                       XK_Home,   spawn, {.v = mutevol } },
    { MODKEY,                       XK_Up,     spawn, {.v = upvol   } },

    { 0,                            XF86XK_AudioLowerVolume,     spawn, {.v = downvol } },
	{ 0,                            XF86XK_AudioMute, 			 spawn, {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume,     spawn, {.v = upvol   } },
	{ 0,					        XF86XK_MonBrightnessUp,		 spawn,	{.v = light_up} },
	{ 0,					        XF86XK_MonBrightnessDown,	 spawn,	{.v = light_down} },
	{ 0,					        XK_Print,	 				 spawn,	{.v = flameshot} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, 0, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },{ ClkWinTitle,          0,              Button3,        zoom,           {0} },
	{ ClkStatusText,        0,              Button3,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ 0,                    0,              Button2,        focusstack,     {.i = -1 } },
	{ 0,                    0,              Button2,        focusstack,     {.i = +1 } },
};

