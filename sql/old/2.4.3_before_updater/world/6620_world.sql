#set some missing CREATURE_TYPEFLAGS_MOUNTED_COMBAT
update creature_template SET type_flags = type_flags & 2048 WHERE entry IN (16163,19364,19795,19806,21102,21315,21719,21979,21998,22006,22018,22059,22084,22102,22197,22274,22331,22433,23030,23330,23440,22966,16126,22251,22967,23188,23321,23340,23342,23344,23345,23346,23348,23387,23441,23580,25063,24976,28132,29202,25302,25264,23539,23556,23557,23739,23792,23844,23851,23856,23857,24132,24142,24169,24237,24238,24387,24447,24533,24538,24646,24647,24849,25286,25289,25326,25364,25414,25420,25421,25440,25446,25447,25743,25802,25808,25819,25862,25968,26047,26103,26115,26128,26585,26931,26967,27074,27106,27107,27108,27109,27110,27155,27156,27161,27206,27214,27220,27224,27225,27229,27268,27476,27492,27504,27521,27544,27549,27550,27579,27673,27678,27693,27748,27758,27759,27843,27859,27860,27873,28006,28063,28164,28171,28229,28264,28530,28611,28683,28689,28737,28801,28818,28863,28865,28898,28909,28936,28945,28946,28964,29000,29101,29106,29173,29199,29200,29204,29228,29329,29333,29344,29456,29458,29498,29584,29585,29592,29639,29724,29734,29738,29775,29836,29842,29872,29982,29986,30187,30188,30224,30263,30265,30595,30609,30639,30675,30677,30703,30704,30739,30740,30925,30934,31003,31044,31045,31046,31063,31084,31088,31089,31094,31314,31428,31676,32307,32308,32309,32358,32438,32492,32573,32777,33285,33305,33306,33382,33383,33384,33429,33438,33447,33448,33501,33518,33520,33521,33522,33550,33558,33559,33561,33562,33564,33698,33707,33738,33739,33740,33743,33744,33745,33746,33747,33748,33749,33780,33785,34104,34127,34179,35344,35326,35325,35327,35314,35323,35587,36167,35330,35331,35332,35328,35329,37119,37928,37965,37967,37996,38194,38493,38505,39092,27124,36792,38472,38995,37845,37542,37658,38497,39091,39259,39271,39273,39675,39678,39819,39836,39837,39910,40704);