#include <u.h>
#include <libc.h>
#include "obj.h"

Ft ft[] =
{
	"1o5",41.73056,122.54444,0,0,
	"3b1",45.46306,69.55583,0,0,
	"abe",40.65306,75.44025,0,0,
	"abi",32.41131,99.68133,0,0,
	"abq",35.04178,106.60639,0,0,
	"abr",45.44975,98.42133,0,0,
	"aby",31.53533,84.19456,0,0,
	"ack",41.25292,70.06072,0,0,
	"act",31.61133,97.22867,0,0,
	"acv",40.97828,124.10744,0,0,
	"acy",39.45664,74.57764,0,0,
	"adm",34.30339,97.01694,0,0,
	"adq",57.75072,152.49167,0,0,
	"agc",40.35467,79.92992,0,0,
	"ags",33.36981,81.96467,0,0,
	"ahn",33.94847,83.32678,0,0,
	"aia",42.05328,102.80325,0,0,
	"akn",58.67753,156.64708,0,0,
	"ako",40.17500,103.21250,0,0,
	"alb",42.74528,73.80139,0,0,
	"ali",27.74056,98.02833,0,0,
	"aln",38.89158,90.04747,0,0,
	"alo",42.55686,92.40011,0,0,
	"als",37.43722,105.86567,0,0,
	"alw",46.09469,118.28697,0,0,
	"ama",35.21953,101.71153,0,0,
	"amg",31.53806,82.50917,0,0,
	"anb",33.58989,85.85569,0,0,
	"anc",61.17489,149.99400,0,0,
	"and",34.49486,82.70917,0,0,
	"anw",42.57722,99.98861,0,0,
	"aoo",40.29631,78.32028,0,0,
	"apa",39.57183,104.84831,0,0,
	"apc",38.21328,122.27961,0,0,
	"apn",45.08017,83.55981,0,0,
	"ara",30.03756,91.88378,0,0,
	"arr",41.77222,88.47222,0,0,
	"art",43.99186,76.02208,0,0,
	"arv",45.92722,89.73139,0,0,
	"ase",39.22458,106.86850,0,0,
	"ast",46.15850,123.87833,0,0,
	"atl",33.63811,84.42783,0,0,
	"atw",44.25778,88.52042,0,0,
	"aty",44.91403,97.15436,0,0,
	"aug",44.31958,69.79700,0,0,
	"aus",30.29844,97.70164,0,0,
	"auw",44.92592,89.62550,0,0,
	"avl",35.43606,82.54197,0,0,
	"avp",41.33875,75.72408,0,0,
	"axn",45.86639,95.39306,0,0,
	"azo",42.23483,85.55203,0,0,
	"baf",42.15764,72.71608,0,0,
	"bbw",41.43472,99.64028,0,0,
	"bce",37.70000,112.15833,0,0,
	"bdl",41.93875,72.68367,0,0,
	"bdr",41.16369,73.12642,0,0,
	"bed",42.46986,71.28953,0,0,
	"beh",42.12856,86.42850,0,0,
	"bet",60.78061,161.83553,0,0,
	"bfd",41.80300,78.64036,0,0,
	"bff",41.87608,103.59817,0,0,
	"bfi",47.53017,122.30072,0,0,
	"bfl",35.43017,119.05200,0,0,
	"bgm",42.20742,75.97933,0,0,
	"bgr",44.80736,68.82864,0,0,
	"bhb",44.44972,68.36203,0,0,
	"bhm",33.56378,86.75453,0,0,
	"bid",41.16806,71.57833,0,0,
	"bie",40.30028,96.75444,0,0,
	"big",63.99506,145.71797,0,0,
	"bih",37.37333,118.36500,0,0,
	"bil",45.80806,108.54028,0,0,
	"bis",46.77700,100.75106,0,0,
	"bjc",39.90789,105.11475,0,0,
	"bji",47.50903,94.93281,0,0,
	"bke",44.83800,117.80864,0,0,
	"bkl",41.51744,81.68347,0,0,
	"bkw",37.78722,81.12433,0,0,
	"bkx",44.30508,96.81331,0,0,
	"blf",37.29569,81.20786,0,0,
	"blh",33.62094,114.71883,0,0,
	"bli",48.79644,122.53725,0,0,
	"blu",39.27500,120.70833,0,0,
	"bmg",39.14447,86.61653,0,0,
	"bmi",40.48050,88.92381,0,0,
	"bna",36.12692,86.68142,0,0,
	"bno",43.59225,118.95394,0,0,
	"boi",43.56511,116.22428,0,0,
	"bos",42.36422,71.00567,0,0,
	"bow",27.94333,81.78417,0,0,
	"bpi",42.57500,110.10833,0,0,
	"bpt",29.95047,94.01917,0,0,
	"brd",46.39769,94.13661,0,0,
	"brl",40.78319,91.12536,0,0,
	"bro",25.90858,97.42347,0,0,
	"brw",71.28586,156.76253,0,0,
	"btl",42.30939,85.24786,0,0,
	"btm",45.95328,112.49569,0,0,
	"btr",30.53261,91.14978,0,0,
	"btv",44.47144,73.15319,0,0,
	"buf",42.94047,78.73242,0,0,
	"bur",34.20058,118.35756,0,0,
	"bvo",36.76239,96.01089,0,0,
	"bvy",42.58414,70.91739,0,0,
	"bwg",36.96447,86.41969,0,0,
	"bwi",39.17503,76.66986,0,0,
	"byi",42.54269,113.77069,0,0,
	"bzn",45.77933,111.15342,0,0,
	"cae",33.94047,81.12011,0,0,
	"cak",40.91706,81.44164,0,0,
	"car",46.87083,68.02083,0,0,
	"ccr",37.98964,122.05586,0,0,
	"cdb",55.20456,162.72181,0,0,
	"cdc",37.70161,113.09725,0,0,
	"cdr",42.83758,103.09494,0,0,
	"cds",34.43333,100.30000,0,0,
	"cdv",60.49233,145.47564,0,0,
	"cdw",40.87522,74.28136,0,0,
	"cec",41.78033,124.23533,0,0,
	"cef",42.19778,72.53056,0,0,
	"cew",30.77972,86.52250,0,0,
	"cez",37.30300,108.62742,0,0,
	"cgf",41.56508,81.48650,0,0,
	"cgi",37.22528,89.57067,0,0,
	"cgx",41.85875,87.60786,0,0,
	"cha",35.03519,85.20383,0,0,
	"cho",38.13850,78.45314,0,0,
	"chs",32.89847,80.04069,0,0,
	"cic",39.79550,121.85733,0,0,
	"cid",41.88456,91.70872,0,0,
	"ciu",46.24778,84.47094,0,0,
	"ckb",39.29544,80.22897,0,0,
	"cle",41.41044,81.84886,0,0,
	"cll",30.58939,96.36414,0,0,
	"clt",35.21442,80.94364,0,0,
	"cmh",39.99511,82.88628,0,0,
	"cmi",40.03961,88.27764,0,0,
	"cmx",47.16850,88.48894,0,0,
	"cnk",39.54944,97.65167,0,0,
	"cnm",32.33711,104.26272,0,0,
	"cnu",37.66778,95.48611,0,0,
	"cod",44.52122,109.02133,0,0,
	"coe",47.77436,116.81806,0,0,
	"con",43.20347,71.50247,0,0,
	"cos",38.80872,104.70969,0,0,
	"cot",28.45528,99.21806,0,0,
	"cou",38.81639,92.21922,0,0,
	"cpr",42.90797,106.46314,0,0,
	"cps",38.57161,90.15953,0,0,
	"crp",27.77006,97.50094,0,0,
	"crq",33.12817,117.27953,0,0,
	"crw",38.37303,81.59339,0,0,
	"csg",32.51619,84.93894,0,0,
	"csm",35.33978,99.20014,0,0,
	"csv",35.95122,85.08503,0,0,
	"ctb",48.61139,112.37917,0,0,
	"cul",38.08944,88.12306,0,0,
	"cvg",39.04789,84.66656,0,0,
	"cwa",44.77844,89.66639,0,0,
	"cwi",41.83108,90.32900,0,0,
	"cxy",40.21689,76.85158,0,0,
	"cys",41.15575,104.81233,0,0,
	"czd",40.86889,100.00389,0,0,
	"czk",45.66667,121.88750,0,0,
	"dab",29.18069,81.05600,0,0,
	"dag",34.85389,116.78611,0,0,
	"dal",32.84700,96.85150,0,0,
	"dan",36.57414,79.33581,0,0,
	"day",39.90100,84.22011,0,0,
	"dbq",42.40294,90.70903,0,0,
	"dca",38.85200,77.03803,0,0,
	"ddc",37.76233,99.96450,0,0,
	"dec",39.83428,88.86811,0,0,
	"den",39.77428,104.87744,0,0,
	"det",42.40922,83.01000,0,0,
	"dfw",32.89658,97.03358,0,0,
	"dhn",31.32117,85.44969,0,0,
	"dht",36.01667,102.55000,0,0,
	"dik",46.79583,102.80000,0,0,
	"dlh",46.84122,92.19039,0,0,
	"dln",45.25000,112.55000,0,0,
	"dls",45.61861,121.16611,0,0,
	"dmn",32.25750,107.71722,0,0,
	"dpa",41.91433,88.24642,0,0,
	"dro",37.15150,107.75314,0,0,
	"drt",29.37094,100.92225,0,0,
	"dsm",41.53494,93.66047,0,0,
	"dtw",42.21522,83.34869,0,0,
	"dug",31.46767,109.60297,0,0,
	"duj",41.17819,78.89894,0,0,
	"dvl",48.11425,98.90833,0,0,
	"dxr",41.37144,73.48264,0,0,
	"dyr",36.00417,89.40417,0,0,
	"ear",40.73133,99.00425,0,0,
	"eat",47.39900,120.20567,0,0,
	"eau",44.86508,91.48408,0,0,
	"ecg",36.26042,76.17494,0,0,
	"eed",34.77000,114.61972,0,0,
	"ege",39.64497,106.91194,0,0,
	"ekm",41.72025,85.99989,0,0,
	"ekn",38.88936,79.85733,0,0,
	"eko",40.82367,115.78956,0,0,
	"eld",33.22078,92.81308,0,0,
	"elm",42.15947,76.89242,0,0,
	"elp",31.80678,106.38189,0,0,
	"ely",39.29969,114.84106,0,0,
	"emp",38.32917,96.19139,0,0,
	"emt",34.08594,118.03367,0,0,
	"ena",60.57308,151.24614,0,0,
	"enn",64.54889,149.07333,0,0,
	"eph",47.30553,119.51194,0,0,
	"eri",42.08167,80.17728,0,0,
	"esc",45.72178,87.09444,0,0,
	"esf",31.39453,92.29550,0,0,
	"eug",44.12189,123.21742,0,0,
	"evv",38.03794,87.53061,0,0,
	"evw",41.35000,111.00833,0,0,
	"ewb",41.67644,70.95831,0,0,
	"ewn",35.07211,77.04381,0,0,
	"ewr",40.69306,74.16869,0,0,
	"eyw",24.55567,81.75975,0,0,
	"fai",64.81628,147.85564,0,0,
	"far",46.91533,96.81461,0,0,
	"fat",36.77661,119.71722,0,0,
	"fay",34.99131,78.88025,0,0,
	"fca",48.31183,114.25411,0,0,
	"fdy",41.01111,83.67500,0,0,
	"fhu",31.58333,110.33333,0,0,
	"fkl",41.37781,79.86058,0,0,
	"flg",35.13844,111.67050,0,0,
	"fll",26.07203,80.15356,0,0,
	"flo",34.18647,79.72358,0,0,
	"fmn",36.74125,108.22931,0,0,
	"fmy",26.58628,81.86347,0,0,
	"fnt",42.96533,83.74358,0,0,
	"fod",42.55150,94.19231,0,0,
	"foe",38.95039,95.66386,0,0,
	"fok",40.84417,72.62833,0,0,
	"frg",40.72869,73.41383,0,0,
	"frm",43.64417,94.41694,0,0,
	"fsd",43.58139,96.74139,0,0,
	"fsm",35.33597,94.36750,0,0,
	"ftw",32.81886,97.36111,0,0,
	"fty",33.77903,84.52144,0,0,
	"ful",33.87200,117.97889,0,0,
	"fwa",40.97819,85.19122,0,0,
	"fyv",36.00503,94.16986,0,0,
	"gad",33.97253,86.08903,0,0,
	"gag",36.29583,99.77500,0,0,
	"gcc",44.34425,105.53994,0,0,
	"gck",37.92658,100.72467,0,0,
	"gcn",35.95236,112.14625,0,0,
	"geg",47.61997,117.53281,0,0,
	"gfk",47.95036,97.17908,0,0,
	"gfl",43.34114,73.61075,0,0,
	"ggg",32.38469,94.71150,0,0,
	"ggw",48.21328,106.61828,0,0,
	"gjt",39.12253,108.52608,0,0,
	"gkn",62.15528,145.45222,0,0,
	"gld",39.37000,101.69794,0,0,
	"glh",33.48247,90.98522,0,0,
	"gls",29.26672,94.86108,0,0,
	"gnv",29.69014,82.27314,0,0,
	"gon",41.32981,72.04686,0,0,
	"gpt",30.40764,89.06989,0,0,
	"grb",44.48792,88.12758,0,0,
	"gri",40.96747,98.30828,0,0,
	"grr",42.88261,85.52394,0,0,
	"gso",36.09636,79.93928,0,0,
	"gsp",34.89625,82.21850,0,0,
	"gst",58.42611,135.70833,0,0,
	"gtf",47.48244,111.36994,0,0,
	"guc",38.53439,106.93611,0,0,
	"gup",35.51142,108.78703,0,0,
	"gvw",38.84361,94.56028,0,0,
	"gwo",33.49469,90.08633,0,0,
	"gyy",41.61625,87.41275,0,0,
	"hbr",34.99167,99.05000,0,0,
	"hez",31.61367,91.29731,0,0,
	"hfd",41.73617,72.65067,0,0,
	"hgr",39.70775,77.73061,0,0,
	"hhr",33.92283,118.33428,0,0,
	"hib",47.38653,92.83858,0,0,
	"hio",45.54056,122.94861,0,0,
	"hky",35.74064,81.39000,0,0,
	"hlg",40.17625,80.64717,0,0,
	"hln",46.60683,111.98189,0,0,
	"hnl",21.32228,157.92425,0,0,
	"hob",32.68761,103.21694,0,0,
	"hom",59.64561,151.47600,0,0,
	"hon",44.38536,98.22875,0,0,
	"hot",34.47794,93.09603,0,0,
	"hou",29.64547,95.27842,0,0,
	"hpn",41.06686,73.70800,0,0,
	"hqm",46.97139,123.93528,0,0,
	"hrl",26.22794,97.65353,0,0,
	"hro",36.26144,93.15456,0,0,
	"hsi",40.60456,98.42714,0,0,
	"hsp",37.95133,79.83411,0,0,
	"hsv",34.64272,86.77558,0,0,
	"htl",44.35000,84.66667,0,0,
	"hts",38.36658,82.55814,0,0,
	"huf",39.45183,87.30889,0,0,
	"hul",46.12417,67.79222,0,0,
	"hum",29.56739,90.66036,0,0,
	"hut",38.06547,97.86031,0,0,
	"hvn",41.26386,72.88747,0,0,
	"hvr",48.54406,109.76128,0,0,
	"hwd",37.65956,122.12172,0,0,
	"hya",41.66864,70.27994,0,0,
	"iad",38.94361,77.45453,0,0,
	"iag",43.10678,78.94553,0,0,
	"iah",29.98267,95.34575,0,0,
	"ict",37.64994,97.43272,0,0,
	"ida",43.51647,112.06719,0,0,
	"ile",31.08578,97.68614,0,0,
	"ilg",39.67844,75.60683,0,0,
	"ilm",34.27044,77.90286,0,0,
	"iml",40.50972,101.61972,0,0,
	"imt",45.81839,88.11442,0,0,
	"ind",39.72436,86.28331,0,0,
	"ink",31.77917,103.20417,0,0,
	"inl",48.56625,93.40292,0,0,
	"int",36.13358,80.22222,0,0,
	"inw",35.02222,110.72222,0,0,
	"ipl",32.83597,115.57450,0,0,
	"ipt",41.24189,76.92206,0,0,
	"irk",40.09244,92.54411,0,0,
	"isn",48.17689,103.63828,0,0,
	"isp",40.79544,73.10008,0,0,
	"ith",42.49019,76.45769,0,0,
	"ito",19.72331,155.05125,0,0,
	"iwd",46.52753,90.13122,0,0,
	"ixd",38.83125,94.88983,0,0,
	"jac",43.60664,110.73736,0,0,
	"jan",32.31103,90.07581,0,0,
	"jax",30.49250,81.68992,0,0,
	"jbr",35.83111,90.64631,0,0,
	"jef",38.59156,92.15697,0,0,
	"jfk",40.64014,73.77831,0,0,
	"jhw",42.15331,79.25828,0,0,
	"jln",37.14944,94.49819,0,0,
	"jms",46.92972,98.67781,0,0,
	"jnu",58.35528,134.57450,0,0,
	"jst",40.31675,78.83475,0,0,
	"jvl",42.61958,89.04022,0,0,
	"jxn",42.25978,84.45944,0,0,
	"koa",19.73561,156.04897,0,0,
	"ktn",55.35611,131.71139,0,0,
	"laf",40.41228,86.93689,0,0,
	"lal",27.98889,82.01472,0,0,
	"lan",42.77858,84.58725,0,0,
	"lar",41.31256,105.67378,0,0,
	"las",36.08278,115.15358,0,0,
	"law",34.56669,98.41606,0,0,
	"lax",33.94242,118.40661,0,0,
	"lbb",33.66358,101.82233,0,0,
	"lbe",40.27692,79.40336,0,0,
	"lbf",41.12742,100.69053,0,0,
	"lbl",37.04369,100.96078,0,0,
	"lch",30.12558,93.22289,0,0,
	"lci",43.57372,71.42164,0,0,
	"leb",43.62756,72.30481,0,0,
	"lex",38.03661,84.60578,0,0,
	"lfk",31.23347,94.75011,0,0,
	"lft",30.20392,91.98789,0,0,
	"lga",40.77725,73.87403,0,0,
	"lgb",33.81758,118.15078,0,0,
	"lhx",38.05167,103.51278,0,0,
	"lic",39.27222,103.66250,0,0,
	"lih",21.98292,159.34436,0,0,
	"lit",34.73008,92.23300,0,0,
	"lmt",42.15786,121.73317,0,0,
	"lnd",42.81528,108.72917,0,0,
	"lnk",40.85078,96.75892,0,0,
	"lnn",41.67917,81.39583,0,0,
	"lnr",43.21000,90.18500,0,0,
	"lns",40.12161,76.29642,0,0,
	"lny",20.78878,156.95422,0,0,
	"lol",40.06806,118.56167,0,0,
	"lou",38.22811,85.66336,0,0,
	"loz",37.08717,84.07686,0,0,
	"lrd",27.54431,99.46103,0,0,
	"lru",32.28933,106.92139,0,0,
	"lse",43.87917,91.25622,0,0,
	"luk",39.10236,84.42003,0,0,
	"lvm",45.70000,110.45000,0,0,
	"lvs",35.65389,105.14222,0,0,
	"lwb",37.85931,80.39867,0,0,
	"lwm",42.71711,71.12392,0,0,
	"lws",46.37461,117.01442,0,0,
	"lwt",47.04919,109.46889,0,0,
	"lyh",37.32672,79.20061,0,0,
	"maf",31.94225,102.20181,0,0,
	"mbl",44.27319,86.24892,0,0,
	"mbs",43.53178,84.08164,0,0,
	"mcb",31.17639,90.46889,0,0,
	"mcg",62.95350,155.60342,0,0,
	"mci",39.29922,94.71772,0,0,
	"mck",40.20636,100.59139,0,0,
	"mcn",32.69358,83.64794,0,0,
	"mco",28.43153,81.32467,0,0,
	"mcw",43.15783,93.33106,0,0,
	"mdh",37.77964,89.24983,0,0,
	"mdt",40.19272,76.76322,0,0,
	"mdw",41.78608,87.75219,0,0,
	"mei",32.33300,88.75117,0,0,
	"mem",35.04989,89.97875,0,0,
	"mfd",40.82136,82.51675,0,0,
	"mfe",26.17608,98.23875,0,0,
	"mfr",42.37225,122.87094,0,0,
	"mgm",32.30019,86.39339,0,0,
	"mgw",39.64289,79.91639,0,0,
	"mhe",43.77483,98.03825,0,0,
	"mhk",39.14097,96.67056,0,0,
	"mhn",42.05000,101.06667,0,0,
	"mht",42.93344,71.43819,0,0,
	"mia",25.79289,80.29050,0,0,
	"mie",40.24192,85.39628,0,0,
	"miv",39.36800,75.07456,0,0,
	"mkc",39.12325,94.59250,0,0,
	"mke",42.94689,87.89692,0,0,
	"mkg",43.16947,86.23817,0,0,
	"mkk",21.15606,157.09906,0,0,
	"mkl",35.59981,88.91556,0,0,
	"mkt",44.22167,93.91850,0,0,
	"mlb",28.10167,80.64042,0,0,
	"mlc",34.88231,95.78319,0,0,
	"mld",42.16667,112.29583,0,0,
	"mli",41.44872,90.50797,0,0,
	"mls",46.42817,105.88525,0,0,
	"mlu",32.51072,92.03756,0,0,
	"mmh",37.62778,118.84861,0,0,
	"mmu",40.79925,74.41528,0,0,
	"mnm",45.12200,87.63642,0,0,
	"mob",30.68828,88.24197,0,0,
	"mod",37.62589,120.95339,0,0,
	"mot",48.25944,101.28081,0,0,
	"mpv",44.20344,72.56278,0,0,
	"mqt",46.53408,87.56156,0,0,
	"mrb",39.40183,77.98486,0,0,
	"mrf",30.36667,104.02083,0,0,
	"mri",61.21619,149.83997,0,0,
	"mry",36.58814,121.84800,0,0,
	"msl",34.74522,87.61019,0,0,
	"msn",43.13875,89.33753,0,0,
	"mso",46.91583,114.08861,0,0,
	"msp",44.88422,93.21486,0,0,
	"mss",44.93581,74.84592,0,0,
	"msv",41.70028,74.79972,0,0,
	"msy",29.99289,90.25636,0,0,
	"mtj",38.50036,107.89456,0,0,
	"mtn",39.32778,76.41583,0,0,
	"mue",20.00436,155.67089,0,0,
	"mvy",41.39278,70.61589,0,0,
	"mwa",37.75372,89.01261,0,0,
	"mwc",43.11092,88.03433,0,0,
	"mwh",47.20781,119.31900,0,0,
	"mwl",32.78306,98.05944,0,0,
	"myl",44.88972,116.10000,0,0,
	"myv",39.09803,121.56847,0,0,
	"new",30.04100,90.02686,0,0,
	"oak",37.72136,122.21964,0,0,
	"ocf",29.17475,82.22464,0,0,
	"odx",41.62222,98.95139,0,0,
	"ofk",41.98544,97.43478,0,0,
	"ogd",41.19642,112.01092,0,0,
	"ogg",20.90186,156.43328,0,0,
	"okc",35.39314,97.60050,0,0,
	"olm",46.97078,122.90156,0,0,
	"olu",41.44719,97.34169,0,0,
	"oma",41.30114,95.89325,0,0,
	"ome",64.51264,165.44211,0,0,
	"onl",42.47083,98.68750,0,0,
	"onp",44.58000,124.05703,0,0,
	"ont",34.05611,117.60306,0,0,
	"ord",41.97989,87.90453,0,0,
	"orf",36.89533,76.20092,0,0,
	"orh",42.26717,71.87592,0,0,
	"orl",28.54519,81.33314,0,0,
	"ort",62.96167,141.92694,0,0,
	"osh",43.98581,88.55683,0,0,
	"osu",40.07500,83.07083,0,0,
	"otg",43.65511,95.57892,0,0,
	"oth",43.41736,124.24275,0,0,
	"otm",41.10683,92.44819,0,0,
	"otz",66.88514,162.59611,0,0,
	"owb",37.74044,87.16628,0,0,
	"owd",42.19069,71.17361,0,0,
	"oxr",34.20081,119.20628,0,0,
	"pae",47.90814,122.28239,0,0,
	"pah",37.06078,88.77369,0,0,
	"paq",61.59444,149.08889,0,0,
	"pbf",34.17442,91.93547,0,0,
	"pbi",26.68286,80.09583,0,0,
	"pdk",33.87506,84.30231,0,0,
	"pdt",45.69467,118.84078,0,0,
	"pdx",45.58911,122.59239,0,0,
	"pfn",30.21208,85.68303,0,0,
	"pga",36.92472,111.44708,0,0,
	"phf",37.13175,76.49331,0,0,
	"phl",39.87025,75.24539,0,0,
	"phx",33.43606,112.00881,0,0,
	"pia",40.66411,89.69069,0,0,
	"pib",31.46697,89.33700,0,0,
	"pie",27.91069,82.68753,0,0,
	"pih",42.91319,112.59422,0,0,
	"pir",44.38269,100.28556,0,0,
	"pit",40.49158,80.23189,0,0,
	"pkb",39.34503,81.43936,0,0,
	"pln",45.57092,84.79667,0,0,
	"pmd",34.62939,118.08364,0,0,
	"pnc",36.73056,97.09944,0,0,
	"pne",40.08183,75.01097,0,0,
	"pns",30.47311,87.18750,0,0,
	"pou",41.62658,73.88417,0,0,
	"pqi",46.68892,68.04533,0,0,
	"prb",35.67292,120.62606,0,0,
	"prc",34.65156,112.42069,0,0,
	"psc",46.26483,119.11792,0,0,
	"psg",56.80200,132.94356,0,0,
	"psp",33.82633,116.50383,0,0,
	"psx",28.72500,96.25000,0,0,
	"ptk",42.66503,83.41808,0,0,
	"ptn",29.71111,91.33833,0,0,
	"pub",38.29097,104.49536,0,0,
	"puw",46.74397,117.10858,0,0,
	"pvd",41.72539,71.42814,0,0,
	"pwa",35.53414,97.64672,0,0,
	"pwm",43.64608,70.30925,0,0,
	"ral",33.95167,117.44389,0,0,
	"rap",44.04453,103.05617,0,0,
	"rbl",40.15139,122.25139,0,0,
	"rdd",40.50911,122.29228,0,0,
	"rdg",40.37889,75.96533,0,0,
	"rdm",44.25422,121.14883,0,0,
	"rdu",35.87197,78.78533,0,0,
	"rfd",42.19622,89.09261,0,0,
	"rhi",45.63089,89.46531,0,0,
	"ric",37.50503,77.31997,0,0,
	"riw",43.06497,108.45789,0,0,
	"rks",41.59569,109.06728,0,0,
	"rmg",34.35072,85.15828,0,0,
	"rno",39.49783,119.76772,0,0,
	"roa",37.32472,79.97628,0,0,
	"roc",43.11881,77.67267,0,0,
	"row",33.30144,104.53003,0,0,
	"rsl",38.86667,98.81667,0,0,
	"rst",43.90886,92.49781,0,0,
	"rwf",44.54583,95.08056,0,0,
	"rwi",35.85483,77.89325,0,0,
	"rwl",41.80417,107.20417,0,0,
	"s80",45.94167,116.12500,0,0,
	"sac",38.51314,121.49225,0,0,
	"saf",35.61706,106.08886,0,0,
	"san",32.73350,117.18669,0,0,
	"sat",29.53339,98.46969,0,0,
	"sav",32.12694,81.20208,0,0,
	"sba",34.42622,119.83939,0,0,
	"sbn",41.70433,86.31531,0,0,
	"sbp",35.23708,120.64139,0,0,
	"sby",38.33950,75.51122,0,0,
	"scc",70.19503,148.46214,0,0,
	"sch",42.85264,73.93000,0,0,
	"sck",37.89425,121.23717,0,0,
	"sdf",38.17786,85.73628,0,0,
	"sea",47.44917,122.30808,0,0,
	"see",32.82619,116.97158,0,0,
	"sff",47.68292,117.32150,0,0,
	"sfo",37.61950,122.37453,0,0,
	"sgf",37.24428,93.38667,0,0,
	"sgy",59.45944,135.31389,0,0,
	"shd",38.26372,78.89669,0,0,
	"shr",44.77411,106.97722,0,0,
	"shv",32.44653,93.82486,0,0,
	"sit",57.04750,135.35983,0,0,
	"siy",41.78158,122.46697,0,0,
	"sjc",37.36147,121.92717,0,0,
	"sjt",31.35819,100.49564,0,0,
	"slc",40.78683,111.96800,0,0,
	"sle",44.90992,123.00133,0,0,
	"slk",44.38525,74.20658,0,0,
	"sln",38.79158,97.65064,0,0,
	"smf",38.69564,121.60019,0,0,
	"smn",45.11667,113.88333,0,0,
	"smo",34.01600,118.45075,0,0,
	"smx",34.89900,120.45644,0,0,
	"sna",33.67564,117.86733,0,0,
	"sns",36.66250,121.60614,0,0,
	"sny",41.10133,102.98481,0,0,
	"sow",34.26528,109.99861,0,0,
	"spg",27.76461,82.62739,0,0,
	"spi",39.84369,89.67711,0,0,
	"sps",33.98500,98.49186,0,0,
	"srq",27.39503,82.55428,0,0,
	"ssi",31.15136,81.39153,0,0,
	"stc",45.54528,94.06167,0,0,
	"stj",39.77131,94.91036,0,0,
	"stl",38.74806,90.36222,0,0,
	"sts",38.50906,122.81178,0,0,
	"sun",43.50417,114.29583,0,0,
	"sus",38.66006,90.64514,0,0,
	"sux",42.40358,96.38364,0,0,
	"svc",32.63231,108.15425,0,0,
	"sve",40.37917,120.57361,0,0,
	"swf",41.50400,74.10525,0,0,
	"syr",43.11222,76.10900,0,0,
	"tad",37.25981,104.33925,0,0,
	"tbn",37.74161,92.14058,0,0,
	"tcc",35.18000,103.60472,0,0,
	"tcl",33.22050,87.61139,0,0,
	"tcs",33.23694,107.27111,0,0,
	"tdo",46.47750,122.80583,0,0,
	"teb",40.85000,74.06125,0,0,
	"tiw",47.26811,122.57686,0,0,
	"tlh",30.39586,84.35042,0,0,
	"toa",33.80336,118.33869,0,0,
	"tol",41.58736,83.80528,0,0,
	"top",39.06925,95.62183,0,0,
	"tpa",27.97517,82.53342,0,0,
	"tph",38.05864,117.08386,0,0,
	"tpl",31.15231,97.40750,0,0,
	"tri",36.47619,82.40625,0,0,
	"trk",39.32081,120.13900,0,0,
	"trm",33.62667,116.16083,0,0,
	"ttd",45.54958,122.39703,0,0,
	"ttn",40.27711,74.81400,0,0,
	"tul",36.19844,95.88767,0,0,
	"tup",34.25933,88.76600,0,0,
	"tus",32.11831,110.94308,0,0,
	"tvc",44.74081,85.58244,0,0,
	"tvf",48.06469,96.18361,0,0,
	"tvl",38.89397,119.99433,0,0,
	"twf",42.48197,114.48497,0,0,
	"txk",33.45358,93.99083,0,0,
	"tyr",32.35406,95.40219,0,0,
	"tys",35.81250,83.99286,0,0,
	"uca",43.14506,75.38425,0,0,
	"ugn",42.42083,87.86667,0,0,
	"uil",47.93417,124.56083,0,0,
	"uin",39.94258,91.19436,0,0,
	"uki",39.12686,123.19994,0,0,
	"vct",28.85247,96.91836,0,0,
	"vdz",61.13447,146.24633,0,0,
	"vel",40.43900,109.51031,0,0,
	"ver",38.94722,92.68861,0,0,
	"vih",38.12778,91.76944,0,0,
	"vis",36.31869,119.39192,0,0,
	"vld",30.78231,83.27683,0,0,
	"vny",34.20981,118.48906,0,0,
	"vrb",27.65481,80.41753,0,0,
	"vtn",42.85694,100.54722,0,0,
	"wjf",34.74097,118.21803,0,0,
	"wmc",40.89722,117.80194,0,0,
	"wrg",56.48486,132.36792,0,0,
	"wrl",43.96558,107.94969,0,0,
	"yak",59.50497,139.65797,0,0,
	"yam",46.4850,84.5100,0,0,
	"yaw",44.6400,63.5000,0,0,
	"ybc",49.1317,68.2083,0,0,
	"ybg",48.3300,70.9967,0,0,
	"ych",47.0066,65.4583,0,0,
	"ycl",47.9833,66.3333,0,0,
	"ydf",49.2116,57.3900,0,0,
	"yfb",63.7583,68.5567,0,0,
	"yfc",45.8700,66.5317,0,0,
	"ygk",44.2250,76.6000,0,0,
	"ygp",48.7767,64.4800,0,0,
	"ygr",47.4250,61.7783,0,0,
	"ygw",55.2817,77.7583,0,0,
	"ygx",56.3600,94.7050,0,0,
	"yhd",49.8300,92.7433,0,0,
	"yhm",43.1717,79.9317,0,0,
	"yhu",45.5183,73.4183,0,0,
	"yhz",44.8817,63.5100,0,0,
	"yip",42.23789,83.53047,0,0,
	"yjt",48.5450,58.5517,0,0,
	"ykf",43.4583,80.3850,0,0,
	"ykl",54.8050,66.8067,0,0,
	"ykm",46.56836,120.54250,0,0,
	"yld",47.8200,83.3533,0,0,
	"ymo",51.2917,80.6033,0,0,
	"ymt",49.7717,74.5283,0,0,
	"ymx",45.6800,74.0383,0,0,
	"yng",41.25778,80.67622,0,0,
	"ynm",49.7600,77.7967,0,0,
	"yow",45.3233,75.6700,0,0,
	"ypq",44.2333,78.3667,0,0,
	"yqa",44.9750,79.3033,0,0,
	"yqb",46.7933,71.3917,0,0,
	"yqg",42.2750,82.9583,0,0,
	"yqi",43.8267,66.0883,0,0,
	"yqk",49.7900,94.3633,0,0,
	"yqm",46.1117,64.6800,0,0,
	"yqt",48.3717,89.3233,0,0,
	"yqx",48.9400,54.5700,0,0,
	"yqy",46.1617,60.0483,0,0,
	"yrb",74.7167,94.9717,0,0,
	"ysb",46.6250,80.7983,0,0,
	"ysc",45.4383,71.6917,0,0,
	"ysj",45.3167,65.8917,0,0,
	"ysn",43.1917,79.1717,0,0,
	"ysu",46.4400,63.8317,0,0,
	"yth",55.8017,97.8633,0,0,
	"ytr",44.1183,77.5283,0,0,
	"yts",48.5700,81.3767,0,0,
	"yul",45.4600,73.7650,0,0,
	"yum",32.65653,114.60519,0,0,
	"yuy",48.2067,78.8333,0,0,
	"yvo",48.0533,77.7833,0,0,
	"yvp",58.0950,68.4233,0,0,
	"yvv",44.7467,81.1067,0,0,
	"ywg",49.9117,97.2433,0,0,
	"ywk",52.9217,66.8650,0,0,
	"yxl",50.1150,91.9050,0,0,
	"yxr",47.6950,79.8500,0,0,
	"yxu",43.0350,81.1533,0,0,
	"yyb",46.3633,79.4233,0,0,
	"yyg",46.2883,63.1300,0,0,
	"yyq",58.7400,94.0667,0,0,
	"yyr",53.3200,60.4267,0,0,
	"yyt",47.6183,52.7533,0,0,
	"yyu",49.4133,82.4683,0,0,
	"yyy",48.6083,68.2083,0,0,
	"yyz",43.6767,79.6317,0,0,
	"yze",45.8850,82.5683,0,0,
	"yzr",43.0000,82.3083,0,0,
	"yzs",54.1950,83.3617,0,0,
	"yzv",50.2250,66.2667,0,0,
	"zum",53.5617,64.1067,0,0,
	"zzv",39.94439,81.89225,0,0,
};
int	nft	= { nelem(ft) };
