#if 0
	shc Version 3.8.9, Generic Script Compiler
	Copyright (c) 1994-2012 Francisco Rosales <frosal@fi.upm.es>

	shc -r -v -T -f firecast_init_manual_IMERG.sh 
#endif

static  char data [] = 
#define      msg1_z	42
#define      msg1	((&data[4]))
	"\153\331\246\317\265\264\211\047\142\070\276\040\070\326\046\331"
	"\315\242\240\370\212\145\103\207\102\050\147\310\311\301\063\037"
	"\143\204\010\104\160\155\101\173\011\377\036\367\374\211\020\053"
	"\144\236\106\371"
#define      chk2_z	19
#define      chk2	((&data[55]))
	"\135\144\164\067\014\141\223\035\377\357\366\247\364\227\221\134"
	"\345\265\200\156\235\003\131"
#define      text_z	4267
#define      text	((&data[439]))
	"\251\136\077\265\316\330\007\034\372\147\267\145\101\135\064\121"
	"\210\231\360\317\222\340\233\326\075\377\113\227\317\242\154\170"
	"\001\253\056\320\204\065\354\176\235\243\344\336\001\030\057\211"
	"\261\037\131\104\000\364\032\075\363\146\325\302\011\101\072\012"
	"\355\150\333\161\236\307\360\074\153\324\032\154\354\112\366\236"
	"\152\117\342\152\103\375\250\067\144\176\372\155\277\065\170\255"
	"\236\123\036\074\033\016\171\206\342\223\363\317\336\352\156\110"
	"\071\121\263\175\117\133\265\263\331\257\040\231\344\230\106\202"
	"\354\145\277\007\164\070\216\127\314\201\047\252\153\225\362\245"
	"\347\246\043\067\001\330\352\333\207\013\165\154\244\274\356\220"
	"\041\256\227\226\346\046\355\263\247\024\135\023\252\120\271\222"
	"\366\334\311\370\264\263\323\074\277\111\250\143\005\226\363\047"
	"\104\213\275\053\261\253\336\131\277\074\155\152\214\046\374\202"
	"\002\305\172\267\171\116\363\070\227\233\233\234\062\217\304\167"
	"\032\201\242\314\054\201\046\354\275\223\127\111\271\123\314\274"
	"\031\106\163\222\225\146\312\054\002\145\311\064\365\215\253\017"
	"\017\116\334\074\317\002\051\215\225\200\327\117\324\243\013\355"
	"\352\177\177\177\346\111\254\350\257\165\034\244\003\310\264\023"
	"\026\221\120\346\223\171\163\051\372\112\170\317\356\204\274\330"
	"\004\073\130\352\205\004\322\065\172\357\332\176\270\217\222\316"
	"\040\342\264\263\134\050\335\127\163\125\046\141\332\342\072\337"
	"\036\223\311\244\227\234\332\022\214\264\220\104\103\042\023\144"
	"\004\310\027\141\360\364\270\144\112\336\306\045\266\117\314\307"
	"\005\237\163\017\364\222\234\137\072\202\176\172\213\070\240\106"
	"\136\107\334\167\321\125\361\047\270\034\304\355\342\227\227\270"
	"\117\372\054\103\241\101\273\072\011\372\343\350\077\312\310\164"
	"\071\320\211\104\160\271\246\177\323\377\265\120\307\076\350\062"
	"\312\332\222\050\104\223\271\004\102\311\006\237\105\160\076\250"
	"\110\277\065\240\231\233\105\261\161\036\052\001\017\071\031\037"
	"\220\162\314\034\333\312\227\076\273\021\010\165\052\000\300\205"
	"\063\120\076\127\260\173\077\310\042\144\223\377\046\322\102\131"
	"\125\122\165\110\226\026\100\004\203\034\060\313\163\106\011\071"
	"\157\024\006\322\047\215\207\127\227\300\053\230\170\254\144\237"
	"\306\237\047\340\133\015\133\047\143\372\255\177\335\347\265\206"
	"\160\122\113\153\235\172\143\357\225\355\004\141\146\104\072\135"
	"\343\337\350\003\233\024\150\112\117\025\106\235\334\272\173\012"
	"\202\154\261\300\335\365\131\173\006\004\174\321\246\174\304\255"
	"\347\153\055\054\245\047\262\052\070\253\063\365\331\174\062\044"
	"\314\071\155\053\243\113\127\206\261\202\256\245\263\364\064\071"
	"\173\101\275\260\003\172\264\272\057\252\141\003\371\215\033\201"
	"\226\035\333\365\161\327\036\130\340\217\116\044\077\154\274\355"
	"\013\255\214\256\146\217\134\355\130\317\365\122\024\067\125\200"
	"\151\011\223\151\127\210\051\352\227\076\200\343\115\315\353\343"
	"\216\110\301\302\122\122\331\105\222\356\376\272\215\210\333\344"
	"\053\377\056\351\113\021\124\077\140\123\305\105\076\066\314\250"
	"\170\057\272\350\137\216\231\142\225\050\056\254\052\251\311\062"
	"\063\052\121\146\222\273\055\211\027\051\223\043\067\224\373\231"
	"\312\127\173\176\232\317\052\360\350\131\147\052\076\013\217\033"
	"\260\341\150\162\177\130\110\321\005\377\254\256\362\165\375\004"
	"\356\346\134\120\134\247\034\205\045\274\331\223\310\217\251\100"
	"\232\141\273\110\101\165\365\121\165\212\211\356\231\164\010\246"
	"\116\261\344\013\315\335\065\307\122\132\352\225\236\130\313\234"
	"\224\256\003\065\017\116\253\225\210\226\276\035\040\357\362\164"
	"\247\153\032\336\243\376\204\271\313\171\242\213\017\051\106\147"
	"\322\262\270\233\046\250\117\267\240\215\301\261\056\365\272\334"
	"\313\134\061\366\235\050\070\131\302\040\335\253\341\355\276\320"
	"\131\043\336\034\307\025\347\302\365\245\375\310\061\224\117\060"
	"\306\030\372\200\004\241\303\017\200\210\232\305\350\210\256\377"
	"\335\125\160\023\063\261\331\007\275\043\306\061\255\102\104\172"
	"\311\330\204\202\362\257\363\367\037\017\246\261\271\071\163\242"
	"\335\343\121\152\273\101\010\221\347\051\072\101\335\314\074\104"
	"\206\361\127\100\070\244\100\373\211\027\014\320\363\245\265\050"
	"\024\310\133\114\335\254\227\241\117\124\233\021\122\273\025\147"
	"\363\311\161\332\076\210\174\370\176\044\233\163\245\170\240\003"
	"\213\354\245\370\131\342\357\304\002\123\014\075\277\201\303\073"
	"\071\064\027\220\265\302\065\044\260\372\072\313\163\265\226\027"
	"\336\375\327\175\324\026\012\071\221\306\316\076\147\375\072\150"
	"\313\151\167\250\004\340\251\064\025\223\042\361\116\221\230\322"
	"\304\371\374\011\360\335\061\101\226\142\203\012\075\275\154\346"
	"\304\036\024\326\303\267\070\124\032\321\352\377\363\020\241\264"
	"\122\062\303\064\261\252\370\301\073\300\226\316\100\313\373\124"
	"\202\043\216\266\210\163\054\341\075\227\240\033\153\224\035\125"
	"\337\257\002\001\232\216\205\207\347\317\137\044\110\026\163\245"
	"\231\142\070\161\334\120\212\215\140\254\347\176\323\312\277\133"
	"\365\053\324\006\026\347\325\051\022\120\366\055\271\322\025\176"
	"\006\224\314\367\041\346\346\035\345\153\221\001\144\133\050\070"
	"\120\257\203\023\235\341\122\072\313\217\017\344\367\020\330\172"
	"\233\161\264\045\121\000\236\247\037\325\160\377\051\150\037\332"
	"\125\034\275\364\130\232\007\261\152\213\233\012\231\235\127\355"
	"\055\273\063\103\162\054\154\243\106\251\152\267\126\217\313\374"
	"\003\064\025\023\300\312\373\106\310\016\360\020\157\254\053\026"
	"\011\245\121\355\210\156\001\100\300\261\216\071\344\143\062\175"
	"\177\220\127\155\063\300\320\265\063\231\075\337\017\156\354\326"
	"\101\234\154\271\073\015\112\253\233\300\053\065\316\056\323\016"
	"\067\300\041\220\035\313\176\264\005\031\050\074\126\011\270\212"
	"\361\076\150\030\064\263\220\150\215\027\140\254\174\140\076\316"
	"\355\157\147\222\357\257\143\375\316\061\331\102\015\236\317\340"
	"\051\236\122\110\342\051\120\257\037\170\215\350\111\342\316\323"
	"\350\247\005\275\312\370\220\134\202\272\074\075\023\173\242\270"
	"\351\036\152\002\243\172\246\147\043\027\220\331\042\356\200\173"
	"\215\106\026\043\004\034\365\044\147\110\200\216\062\030\275\333"
	"\062\360\143\011\017\033\172\250\173\100\003\222\044\002\120\262"
	"\363\243\072\126\103\341\007\125\014\304\273\327\012\316\002\373"
	"\012\347\113\124\331\261\224\224\016\162\230\153\263\056\202\076"
	"\014\214\364\252\202\170\166\015\326\313\017\215\374\055\027\242"
	"\135\156\026\150\243\344\123\037\022\135\172\055\347\306\130\216"
	"\305\120\015\224\057\137\117\343\027\074\313\117\150\046\332\130"
	"\212\117\253\204\126\032\212\061\024\242\343\175\246\367\371\036"
	"\062\052\127\244\107\352\046\377\060\313\117\004\370\061\043\177"
	"\323\053\306\111\165\220\256\375\130\324\362\312\006\246\172\163"
	"\053\237\137\133\254\046\017\124\164\310\054\340\301\157\212\376"
	"\023\122\140\165\211\063\335\376\205\254\236\351\037\140\307\066"
	"\254\033\064\047\235\041\250\033\226\242\277\121\365\275\001\337"
	"\277\210\053\047\174\363\360\306\073\046\234\026\312\012\162\214"
	"\315\133\300\221\327\345\044\336\365\113\006\105\060\132\354\301"
	"\301\100\351\031\301\010\262\163\317\160\167\016\134\175\332\214"
	"\047\052\240\100\252\253\062\342\045\232\122\351\157\076\172\031"
	"\174\226\000\063\363\160\126\241\061\015\250\277\375\230\264\171"
	"\166\254\065\066\021\045\124\143\025\316\340\236\205\042\021\321"
	"\230\105\251\202\270\326\164\205\062\112\165\110\013\217\347\357"
	"\247\375\330\167\162\015\276\347\045\167\342\104\256\337\167\160"
	"\133\100\123\075\223\031\170\333\012\111\164\346\134\340\276\030"
	"\206\257\125\376\302\072\210\236\060\030\010\241\350\037\134\227"
	"\212\350\031\077\363\053\163\311\200\073\305\047\336\175\260\266"
	"\106\135\371\040\342\266\217\265\360\120\021\347\101\310\107\330"
	"\345\353\100\063\130\320\102\011\066\027\353\003\304\110\356\211"
	"\051\113\210\115\116\103\334\351\032\070\156\165\230\337\334\061"
	"\346\251\263\270\272\113\332\337\220\316\226\200\133\207\076\132"
	"\311\155\176\336\164\040\344\064\246\123\174\266\246\026\245\211"
	"\340\322\243\344\253\303\130\107\154\013\051\077\205\362\337\201"
	"\205\102\216\166\161\177\107\323\236\103\142\217\167\265\316\057"
	"\370\271\162\323\256\103\356\341\005\335\347\035\170\103\166\371"
	"\340\204\154\003\377\252\317\317\153\320\013\307\243\330\051\062"
	"\041\352\124\070\117\110\104\140\177\072\133\055\011\335\273\050"
	"\155\144\030\312\171\113\137\105\313\014\225\253\267\023\212\213"
	"\132\251\135\320\332\051\315\356\147\304\343\110\262\112\011\156"
	"\217\126\351\234\216\304\044\221\324\127\122\003\265\207\345\235"
	"\257\077\367\143\225\235\273\030\303\106\320\257\353\166\037\140"
	"\321\270\264\372\220\177\376\263\175\320\034\306\216\205\252\126"
	"\056\223\317\043\075\032\333\064\102\354\015\351\211\321\013\154"
	"\344\177\344\353\331\052\323\225\312\115\062\117\241\311\236\154"
	"\246\042\171\051\042\031\167\244\232\302\141\256\242\370\145\377"
	"\046\154\243\131\157\040\305\164\154\065\060\252\350\207\146\327"
	"\337\252\012\144\016\157\224\254\222\253\015\130\147\260\334\107"
	"\077\100\251\373\363\235\367\303\304\133\271\111\310\014\000\233"
	"\040\270\260\224\015\377\357\340\375\240\233\376\131\367\304\224"
	"\203\351\201\312\235\166\137\166\132\032\072\001\363\075\257\206"
	"\177\203\120\175\204\240\153\363\162\207\237\067\211\257\111\361"
	"\331\021\074\137\306\345\056\246\254\101\073\160\076\176\377\205"
	"\171\226\262\374\365\112\317\047\157\265\213\011\247\063\071\107"
	"\151\134\010\313\175\237\176\261\367\377\017\000\036\374\356\112"
	"\063\067\354\047\276\252\356\171\346\324\176\257\066\267\161\010"
	"\207\236\300\317\275\236\343\266\336\235\152\132\020\135\371\027"
	"\264\343\312\115\353\212\025\051\262\054\202\005\330\324\233\044"
	"\056\017\106\260\250\125\015\000\201\107\235\217\232\157\100\221"
	"\306\273\265\251\155\115\366\226\315\155\026\157\343\370\120\174"
	"\314\147\377\145\367\010\044\051\323\205\020\305\176\077\275\143"
	"\047\266\061\267\141\052\122\147\254\314\215\226\255\053\317\111"
	"\265\173\052\132\016\233\256\122\122\361\074\314\137\171\270\331"
	"\370\201\262\334\331\306\270\172\220\275\070\351\326\344\127\361"
	"\040\133\074\120\006\071\027\200\120\352\342\246\223\353\322\022"
	"\210\205\346\277\216\064\351\165\113\332\032\320\002\213\342\213"
	"\364\313\073\272\272\057\145\241\151\373\073\153\210\337\307\034"
	"\227\347\070\013\377\040\124\013\265\137\237\017\265\274\012\124"
	"\041\073\336\226\261\317\337\207\106\307\263\065\114\211\364\110"
	"\165\040\355\351\050\035\055\027\100\247\014\125\167\377\175\073"
	"\337\342\144\375\332\025\233\274\200\171\077\321\152\350\313\032"
	"\303\056\234\161\374\261\113\062\006\025\352\270\000\052\063\276"
	"\030\011\371\327\065\230\304\213\101\240\166\146\024\155\055\050"
	"\142\204\163\073\201\320\072\162\175\017\275\336\263\350\022\163"
	"\246\361\230\030\346\320\073\211\351\326\221\026\366\104\313\224"
	"\023\266\173\155\047\050\366\045\276\346\136\127\242\276\377\315"
	"\367\213\145\154\264\304\262\064\346\227\032\003\113\252\227\275"
	"\161\106\230\152\024\325\137\305\306\243\310\173\050\024\136\321"
	"\002\032\264\355\335\050\147\220\025\057\065\336\343\171\364\304"
	"\042\242\170\002\136\246\110\042\066\177\242\157\353\306\124\147"
	"\354\307\336\001\036\331\042\313\143\157\226\070\027\135\017\127"
	"\332\247\273\152\044\136\056\006\231\101\302\254\055\112\130\053"
	"\155\223\152\013\250\207\232\003\336\200\355\274\324\265\030\351"
	"\367\111\165\133\023\051\223\243\244\227\104\260\313\313\177\350"
	"\376\067\202\106\351\275\254\172\114\113\202\035\237\061\101\225"
	"\322\246\211\207\366\031\351\111\305\360\100\160\067\224\237\005"
	"\241\377\257\037\122\377\325\032\120\031\061\263\071\212\167\303"
	"\050\300\007\013\366\344\025\270\351\310\367\275\300\031\035\350"
	"\220\061\113\245\252\354\126\247\157\206\022\035\055\216\021\200"
	"\343\116\331\212\111\065\250\110\031\025\115\310\000\045\360\234"
	"\050\050\062\351\172\053\161\265\351\343\053\002\161\233\221\054"
	"\111\023\065\052\037\300\201\065\330\200\077\325\311\316\154\331"
	"\263\033\332\055\005\353\031\236\074\363\161\060\074\013\213\074"
	"\007\311\006\313\341\222\060\362\051\012\327\344\351\214\072\316"
	"\222\133\051\365\035\027\144\260\200\074\175\335\015\344\245\151"
	"\166\332\116\366\150\130\335\054\054\236\164\104\015\310\173\123"
	"\141\276\300\050\052\324\270\111\140\177\366\314\266\372\126\140"
	"\113\104\204\151\067\037\206\250\313\234\246\075\076\340\266\322"
	"\364\216\375\267\301\314\037\316\062\300\000\203\310\320\251\362"
	"\177\031\333\232\217\076\022\165\020\102\230\304\021\365\273\030"
	"\107\032\262\215\065\277\206\332\173\107\200\235\246\173\230\033"
	"\354\137\146\301\016\207\242\010\132\275\376\263\323\057\126\321"
	"\111\131\063\155\141\000\375\026\044\265\114\134\005\226\307\154"
	"\024\174\153\337\141\302\025\042\132\330\054\156\156\164\211\213"
	"\072\221\130\345\332\221\167\225\302\350\173\152\257\244\064\114"
	"\153\034\241\353\354\221\005\264\270\354\365\156\276\030\344\327"
	"\075\145\053\154\115\247\045\241\151\253\225\125\327\300\007\216"
	"\011\122\122\342\163\017\233\226\143\250\177\355\101\155\130\344"
	"\006\160\277\222\213\354\007\113\154\153\354\315\111\353\033\246"
	"\216\037\333\077\323\072\067\056\210\201\160\230\270\314\262\024"
	"\165\251\045\304\141\173\326\014\252\233\004\100\333\322\041\153"
	"\344\150\174\312\230\057\101\315\312\005\217\113\117\177\233\055"
	"\125\133\246\063\202\142\071\306\347\200\155\162\256\055\070\027"
	"\325\202\355\250\213\245\145\035\352\146\004\133\241\025\313\301"
	"\167\322\004\136\141\171\336\275\004\010\134\157\122\121\150\173"
	"\215\140\237\006\131\172\031\053\102\227\277\014\122\251\234\042"
	"\017\135\076\005\277\074\215\016\206\122\223\070\242\113\112\352"
	"\013\262\346\112\151\013\140\021\052\123\353\047\370\032\163\215"
	"\154\113\163\217\255\004\213\105\325\020\116\246\031\355\041\011"
	"\226\355\257\074\064\147\302\071\350\351\116\102\224\273\352\163"
	"\007\063\166\144\067\062\317\304\222\375\271\004\375\066\060\254"
	"\305\214\243\232\373\117\307\033\344\301\230\325\256\214\247\200"
	"\240\201\022\276\302\355\116\333\177\124\026\322\237\071\234\314"
	"\231\362\064\342\245\271\235\027\322\366\202\127\344\244\213\317"
	"\367\353\301\304\345\320\026\263\344\254\124\352\210\317\144\371"
	"\053\211\177\074\305\377\042\075\244\065\341\116\075\377\046\212"
	"\274\210\112\313\022\252\152\051\217\342\137\041\021\306\307\221"
	"\053\123\324\116\375\140\326\004\250\266\176\312\110\254\136\160"
	"\337\246\211\252\120\026\003\371\271\166\315\266\211\237\000\234"
	"\234\241\224\033\274\323\370\005\141\257\277\110\005\124\002\363"
	"\117\302\141\367\103\207\135\256\162\345\010\121\204\306\177\275"
	"\145\302\331\004\037\244\014\110\147\203\271\363\156\055\043\331"
	"\304\142\235\013\355\055\044\265\170\050\361\045\045\001\322\032"
	"\373\345\176\257\133\250\331\032\137\014\162\335\366\101\112\357"
	"\336\137\011\176\313\257\177\366\263\041\345\073\240\255\321\056"
	"\373\012\342\245\225\151\116\317\134\013\031\127\227\161\104\345"
	"\234\031\247\151\234\302\155\273\141\046\030\376\076\153\232\303"
	"\215\213\220\337\365\305\001\046\346\276\270\253\302\072\003\025"
	"\001\041\036\074\325\266\317\316\203\216\366\015\370\361\361\034"
	"\326\264\343\022\336\141\210\013\273\333\373\056\260\133\337\232"
	"\270\325\040\023\214\342\025\066\146\325\314\205\004\274\325\165"
	"\033\063\013\004\203\374\017\217\347\025\302\225\125\300\107\122"
	"\155\032\164\143\004\255\020\377\210\217\136\354\061\261\150\135"
	"\165\341\067\330\271\347\006\245\234\143\234\212\127\233\125\145"
	"\335\002\306\011\017\112\332\017\120\371\277\155\046\266\301\105"
	"\235\065\330\204\261\210\377\236\021\245\132\037\030\167\307\065"
	"\162\332\116\063\261\203\154\337\242\021\374\173\201\053\144\251"
	"\237\165\014\043\176\352\050\041\354\240\260\303\040\322\062\255"
	"\213\336\134\231\377\143\102\004\127\014\202\213\063\220\177\151"
	"\302\027\052\360\020\136\241\200\356\376\266\062\163\130\326\357"
	"\110\051\212\245\206\367\351\276\027\220\230\325\342\034\134\211"
	"\307\022\370\245\171\340\054\236\257\054\114\104\317\303\040\334"
	"\247\007\260\355\024\223\114\052\016\277\101\146\076\361\117\321"
	"\354\262\247\212\355\207\033\101\126\102\335\005\210\240\237\133"
	"\121\361\165\270\303\257\061\003\016\356\246\306\146\260\000\110"
	"\335\275\032\276\136\331\340\142\050\342\107\261\171\175\126\250"
	"\027\321\031\175\231\312\234\367\011\023\000\367\167\124\010\221"
	"\136\254\056\367\251\352\044\070\107\322\125\057\037\345\365\256"
	"\061\150\265\120\316\205\371\236\201\163\216\213\065\156\232\242"
	"\023\021\170\343\221\035\043\374\053\066\123\275\210\114\171\370"
	"\363\317\352\157\106\357\111\353\264\247\253\361\053\161\013\143"
	"\334\345\026\234\032\135\335\320\071\216\052\117\140\051\325\223"
	"\137\150\024\050\032\057\217\170\326\233\011\322\012\213\171\247"
	"\230\120\023\230\230\162\205\334\137\155\126\165\077\110\373\277"
	"\055\115\116\003\214\127\370\021\031\017\042\070\266\323\047\267"
	"\145\105\342\331\311\007\120\043\154\107\266\242\276\231\063\356"
	"\343\104\002\276\011\017\126\355\164\304\120\153\023\340\236\230"
	"\102\041\227\134\241\220\110\314\230\243\243\354\313\112\144\024"
	"\054\210\054\234\212\074\312\230\206\347\332\067\247\045\300\225"
	"\031\270\340\201\112\146\326\216\205\062\312\323\272\044\011\236"
	"\012\336\102\275\016\140\002\210\250\164\333\257\077\223\316\071"
	"\237\322\172\243\022\264\340\126\155\150\017\364\167\346\117\371"
	"\346\247\365\153\134\002\215\102\077\201\234\130\332\223\273\234"
	"\205\371\250\175\163\134\155\004\130\066\362\246\175\262\037\141"
	"\034\342\357\340\315\247\066\141\272\177\361\113\366\340\153\052"
	"\160\242\304\144\231\064\275\001\216\374\103\153\066\277\144\371"
	"\345\220\317\365\303\364\374\107\040\243\007\123\042\353\140\125"
	"\060\155\062\107\042\053\334\351\135\307\024\124\345\370\075\213"
	"\215\035\337\232\161\364\232\037\230\323\053\265\150\037\246\277"
	"\310\061\153\353\303\226\004\320\201\377\163\167\263\042\100\154"
	"\366\032\127\056\347\251\111\212\241\364\132\367\352\365\123\147"
	"\265\030\144\130\214\157\027\052\362\370\314\251\257\062\145\142"
	"\303\252\260\000\151\356\360\362\314\161\146\063\234\346\036\244"
	"\327\356\147\144\247\054\071\326\007\171\126\054\126\123\172\243"
	"\166\261\111\112\273\232\342\165\301\344\365\224\130\055\077\307"
	"\030\026\164\017\214\144\257\024\127\272\204\034\353\130\224\001"
	"\207\233\262\143\142\346\366\123\205\036\170\114\167\031\110\346"
	"\216\227\204\014\100\373\256\362\074\153\223\312\111\156\074\231"
	"\107\220\170\323\017\061\070\214\175\374\317\144\276\105\162\132"
	"\331\313\124\150\372\202\012\034\131\143\216\351\232\047\113\010"
	"\376\303\123\340\150\042\330\174\052\221\272\067\140\221\246\314"
	"\117\355\244\020\000\176\142\232\001\072\171\315\132\333\054\273"
	"\115\113\052\311\250\075\106\270\365\230\007\176\134\246\336\100"
	"\353\312\374\202\046\105\057\045\224\011\070\143\176\356\373\376"
	"\005\376\027\155\122\304\333\341\105\210\132\210\310\060\135\377"
	"\116\324\201\351\267\026\076\172\361\303\133\016\167\234\125\314"
	"\340\067\123\014\117\327\145\126\010\052\355\013\063\253\333\155"
	"\155\227\246\123\052\073\057\112\325\003\137\014\164\175\141\151"
	"\270\031\264\300\045\271\242\255\077\103\165\113\006\167\204\361"
	"\140\251\305\317\364\342\072\351\151\234\262\032\271\306\316\027"
	"\204\336\102\357\022\206\066\021\142\375\046\345\302\224\151\063"
	"\352\134\344\076\271\277\333\337\364\305\054\247\042\164\156\322"
	"\260\175\357\036\047\146\116\274\162\031\230\310\210\264\074\145"
	"\046\146\120\017\330\151\345\301\001\004\337\224\316\203\053\153"
	"\136\075\367\022\316\073\126\360\116\272\365\026\322\126\007\307"
	"\017\153\022\355\062\067\256\063\074\216\307\013\022\362\166\160"
	"\060\155\203\376\251\331\357\370\224\344\016\147\073\026\056\112"
	"\202\100\070\264\170\347\347\264\166\256\277\210\241\065\371\321"
	"\243\174\320\114\126\277\104\353\244\123\122\337\152\200\052\354"
	"\300\143\240\070\112\210\355\300\066\255\111\330\343\102\251\206"
	"\277\172\323\025\071\027\001\336\153\123\276\325\323\350\301\224"
	"\114\142\315\227\352\273\130\041\150\241\371\113\344\243\322\243"
	"\035\245\271\127\274\272\066\050\015\364\375\341\335\277\165\051"
	"\042\103\300\015\376\030\057\147\272\051\262\236\314\204\102\352"
	"\051\373\102\346\266\170\016\304\155\014\245\112\314\033\163\357"
	"\137\064\374\135\114\054\304\007\125\167\245\041\374\350\014\045"
	"\344\117\014\232\307\033\137\064\050\005\176\365\040\362\344\200"
	"\047\341\335\164\015\242\173\142\032\041\204\026\011\221\074\355"
	"\340\110\210\247\144\350\334\214\355\133\201\016\116\146\216\165"
	"\107\153\351\125\016\144\267\050\205\074\076\217\315\173\175\255"
	"\303\006\125\050\356\061\265\333\215\067\351\333\235\167\120\345"
	"\343\071\072\362\236\361\032\044\056\131\263\373\324\060\251\230"
	"\066\376\300\044\060\166\000\275\255\351\231\112\141\351\057\104"
	"\043\151\067\302\133\121\346\211\253\231\205\177\312\056\030\000"
	"\054\331\045\134\117\045\032\375\017\263\110\160\235\167\265\301"
	"\341\354\203\075\076\151\307\351\003\114\151\315\172\202\316\247"
	"\133\363\004\253\031\036\250\050\322\360\231\157\150\117\060\112"
	"\074\264\210\173\035\117\144\040\233\316\356\026\121\274\276\255"
	"\260\302\131\311\341\002\362\263\362\214\043\133\333\123\246\030"
	"\010\056\223\045\175\370\106\031\307\064\060\030\361\356\305\241"
	"\260\036\153\221\040\135\105\023\352\150\157\305\274\026\336\304"
	"\104\161\352\302\151\061\334\060\146\014\111\127\373\017\370\254"
	"\056\144\076\117\301\203\142\253\353\322\161\250\350\120\154\055"
	"\301\127\357\053\210\314\134\356\331\245\105\324\264\076\201\342"
	"\242\277\061\144\102\224\020\056\147\202\327\117\322\103\175\223"
	"\233\155\276\043\071\033\022\023\300\127\347\164\226\151\127\070"
	"\050\211\235\153\035\255\231\205\057\160\325\001\264\122\225\120"
	"\277\124\163\371\157\205\014\057\335\364\244\163\135\374\254\205"
	"\205\112\360\243\367\212\050\047\373\375\051\260\117\276\000\017"
	"\023\164\010\202\372\024\262\327\010\127\113\145\123\370\353\331"
	"\102\333\175\072\146\245\142\141\243\213\021\363\111\022\002\134"
	"\206\013\337\200\040\221\127\051\351\243\216\074\233\172\026\336"
	"\125\223\031\274\071\173\035\334\006\057\320\120\101\323\255\307"
	"\336\215\107\376\036\237\047\010\103\266\105\337\061\133\276\206"
	"\356\327\102\050\122\140\004\131\217\325\252\321\250\127\230\206"
	"\344\340\205\003\200\255\013\303\144\120\242\225\254\140\034\233"
	"\070\137\303\212\277\310\344\117\235\216\040\105\346\271\314\312"
	"\231\121\316\032\377\331\335\143\052\200\371\326\341\025\161\031"
	"\165\065\244\065\375\210\204\233\027\245\340\375\136\255\307\370"
	"\377\226\022\376\160\360\142\232\161\133\161\122\161\343\153\346"
	"\030\020\033\026\230\240\261\257\106\221\255\245\077\165\235\076"
	"\013\260\074\173\241\237\026\022\373\207\145\154\153\320\123\203"
	"\340\157\232\171\020\113\051\126\335\326\373\034\113\231\132\127"
	"\112\227\322\353\066\350\376\062\160\143\237\333\064\363\137\024"
	"\142\372\216\163\105\270\312\042\216\305\077\332\137\232\061\251"
	"\061\004\225\150\355\223\233\135\366\072\071\052\055\231\077\220"
	"\223\316\003\330\206\315\373\025\223\073\360\362\325\042\234\007"
	"\046\061\160\023\304\013\161\273\106\252\346\163\104\046\003\327"
	"\364\007\260\173\325\253\220\150\347\201\133\275\243"
#define      pswd_z	256
#define      pswd	((&data[5641]))
	"\050\065\335\355\100\116\250\207\371\216\372\075\264\376\025\251"
	"\005\305\044\333\161\265\103\130\066\237\025\331\226\332\243\277"
	"\017\200\254\120\317\124\327\310\343\322\006\230\320\033\101\326"
	"\340\273\361\372\360\377\215\130\262\061\020\167\360\226\261\227"
	"\047\226\245\270\272\263\134\155\351\233\254\042\350\040\026\177"
	"\334\010\172\315\010\010\045\272\071\066\062\052\314\343\301\364"
	"\172\146\254\064\031\010\242\003\244\116\045\214\157\074\014\113"
	"\104\206\031\114\217\076\007\310\164\071\362\101\035\263\065\227"
	"\032\341\314\063\352\157\067\216\275\134\033\055\230\050\171\335"
	"\257\222\051\076\320\060\006\105\152\371\206\207\254\274\037\306"
	"\236\354\372\210\133\062\027\031\216\063\106\047\133\277\004\012"
	"\121\056\110\042\137\117\150\311\110\357\121\365\253\161\274\111"
	"\135\266\322\271\351\352\322\167\035\031\237\171\331\243\203\052"
	"\322\314\115\061\034\265\373\145\244\114\132\120\275\026\232\033"
	"\315\154\324\266\127\247\056\164\301\315\356\232\161\161\305\103"
	"\076\022\165\132\307\160\300\154\274\032\274\172\061\127\225\377"
	"\303\152\266\033\022\344\217\323\262\175\155\044\357\063\150\056"
	"\105\335\211\015\115\111\172\012\143\066\205\225\215\033\224\121"
	"\205\113\154\227\060\374\153\342\172\331\007\152\014\157\230\122"
	"\115\145\261\121\032\365\252\120\224\300\051\053\232\314\352\251"
	"\115\227\372\034\354\321\345\317\243\353\147\164\007\250\112\347"
	"\016\374\071\050\361\344\171\206\244\242\262\076\157\235\350\275"
	"\064\342\331\040"
#define      rlax_z	1
#define      rlax	((&data[5948]))
	"\124"
#define      xecc_z	15
#define      xecc	((&data[5951]))
	"\314\262\273\120\145\343\122\322\235\341\374\374\074\072\332\211"
	"\026\000"
#define      date_z	1
#define      date	((&data[5967]))
	"\134"
#define      shll_z	10
#define      shll	((&data[5970]))
	"\005\267\063\025\241\370\053\133\100\070\206\161"
#define      opts_z	1
#define      opts	((&data[5980]))
	"\350"
#define      inlo_z	3
#define      inlo	((&data[5981]))
	"\146\156\075"
#define      tst2_z	19
#define      tst2	((&data[5988]))
	"\177\017\144\130\032\147\220\225\336\034\077\022\257\341\117\364"
	"\110\160\167\204\202\027\217\060\030"
#define      lsto_z	1
#define      lsto	((&data[6009]))
	"\102"
#define      tst1_z	22
#define      tst1	((&data[6012]))
	"\164\167\034\011\004\071\342\210\356\225\367\332\243\060\146\153"
	"\245\221\025\053\061\155\347\027\123"
#define      chk1_z	22
#define      chk1	((&data[6038]))
	"\342\274\153\067\200\321\064\257\263\153\054\241\276\133\262\253"
	"\122\362\244\333\035\254\051\032\257\231\154"
#define      msg2_z	19
#define      msg2	((&data[6066]))
	"\066\220\200\021\355\251\363\204\113\352\262\352\135\201\236\341"
	"\176\307\002\352\225\251\046\022\377\041"/* End of data[] */;
#define      hide_z	4096
#define DEBUGEXEC	0	/* Define as 1 to debug execvp calls */
#define TRACEABLE	1	/* Define as 1 to enable ptrace the executable */

/* rtc.c */

#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* 'Alleged RC4' */

static unsigned char stte[256], indx, jndx, kndx;

/*
 * Reset arc4 stte. 
 */
void stte_0(void)
{
	indx = jndx = kndx = 0;
	do {
		stte[indx] = indx;
	} while (++indx);
}

/*
 * Set key. Can be used more than once. 
 */
void key(void * str, int len)
{
	unsigned char tmp, * ptr = (unsigned char *)str;
	while (len > 0) {
		do {
			tmp = stte[indx];
			kndx += tmp;
			kndx += ptr[(int)indx % len];
			stte[indx] = stte[kndx];
			stte[kndx] = tmp;
		} while (++indx);
		ptr += 256;
		len -= 256;
	}
}

/*
 * Crypt data. 
 */
void arc4(void * str, int len)
{
	unsigned char tmp, * ptr = (unsigned char *)str;
	while (len > 0) {
		indx++;
		tmp = stte[indx];
		jndx += tmp;
		stte[indx] = stte[jndx];
		stte[jndx] = tmp;
		tmp += stte[indx];
		*ptr ^= stte[tmp];
		ptr++;
		len--;
	}
}

/* End of ARC4 */

/*
 * Key with file invariants. 
 */
int key_with_file(char * file)
{
	struct stat statf[1];
	struct stat control[1];

	if (stat(file, statf) < 0)
		return -1;

	/* Turn on stable fields */
	memset(control, 0, sizeof(control));
	control->st_ino = statf->st_ino;
	control->st_dev = statf->st_dev;
	control->st_rdev = statf->st_rdev;
	control->st_uid = statf->st_uid;
	control->st_gid = statf->st_gid;
	control->st_size = statf->st_size;
	control->st_mtime = statf->st_mtime;
	control->st_ctime = statf->st_ctime;
	key(control, sizeof(control));
	return 0;
}

#if DEBUGEXEC
void debugexec(char * sh11, int argc, char ** argv)
{
	int i;
	fprintf(stderr, "shll=%s\n", sh11 ? sh11 : "<null>");
	fprintf(stderr, "argc=%d\n", argc);
	if (!argv) {
		fprintf(stderr, "argv=<null>\n");
	} else { 
		for (i = 0; i <= argc ; i++)
			fprintf(stderr, "argv[%d]=%.60s\n", i, argv[i] ? argv[i] : "<null>");
	}
}
#endif /* DEBUGEXEC */

void rmarg(char ** argv, char * arg)
{
	for (; argv && *argv && *argv != arg; argv++);
	for (; argv && *argv; argv++)
		*argv = argv[1];
}

int chkenv(int argc)
{
	char buff[512];
	unsigned long mask, m;
	int l, a, c;
	char * string;
	extern char ** environ;

	mask  = (unsigned long)&chkenv;
	mask ^= (unsigned long)getpid() * ~mask;
	sprintf(buff, "x%lx", mask);
	string = getenv(buff);
#if DEBUGEXEC
	fprintf(stderr, "getenv(%s)=%s\n", buff, string ? string : "<null>");
#endif
	l = strlen(buff);
	if (!string) {
		/* 1st */
		sprintf(&buff[l], "=%lu %d", mask, argc);
		putenv(strdup(buff));
		return 0;
	}
	c = sscanf(string, "%lu %d%c", &m, &a, buff);
	if (c == 2 && m == mask) {
		/* 3rd */
		rmarg(environ, &string[-l - 1]);
		return 1 + (argc - a);
	}
	return -1;
}

#if !TRACEABLE

#define _LINUX_SOURCE_COMPAT
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#if !defined(PTRACE_ATTACH) && defined(PT_ATTACH)
#	define PTRACE_ATTACH	PT_ATTACH
#endif
void untraceable(char * argv0)
{
	char proc[80];
	int pid, mine;

	switch(pid = fork()) {
	case  0:
		pid = getppid();
		/* For problematic SunOS ptrace */
#if defined(__FreeBSD__)
		sprintf(proc, "/proc/%d/mem", (int)pid);
#else
		sprintf(proc, "/proc/%d/as",  (int)pid);
#endif
		close(0);
		mine = !open(proc, O_RDWR|O_EXCL);
		if (!mine && errno != EBUSY)
			mine = !ptrace(PTRACE_ATTACH, pid, 0, 0);
		if (mine) {
			kill(pid, SIGCONT);
		} else {
			perror(argv0);
			kill(pid, SIGKILL);
		}
		_exit(mine);
	case -1:
		break;
	default:
		if (pid == waitpid(pid, 0, 0))
			return;
	}
	perror(argv0);
	_exit(1);
}
#endif /* !TRACEABLE */

char * xsh(int argc, char ** argv)
{
	char * scrpt;
	int ret, i, j;
	char ** varg;
	char * me = getenv("_");
	if (me == NULL) { me = argv[0]; }

	stte_0();
	 key(pswd, pswd_z);
	arc4(msg1, msg1_z);
	arc4(date, date_z);
	if (date[0] && (atoll(date)<time(NULL)))
		return msg1;
	arc4(shll, shll_z);
	arc4(inlo, inlo_z);
	arc4(xecc, xecc_z);
	arc4(lsto, lsto_z);
	arc4(tst1, tst1_z);
	 key(tst1, tst1_z);
	arc4(chk1, chk1_z);
	if ((chk1_z != tst1_z) || memcmp(tst1, chk1, tst1_z))
		return tst1;
	ret = chkenv(argc);
	arc4(msg2, msg2_z);
	if (ret < 0)
		return msg2;
	varg = (char **)calloc(argc + 10, sizeof(char *));
	if (!varg)
		return 0;
	if (ret) {
		arc4(rlax, rlax_z);
		if (!rlax[0] && key_with_file(shll))
			return shll;
		arc4(opts, opts_z);
		arc4(text, text_z);
		arc4(tst2, tst2_z);
		 key(tst2, tst2_z);
		arc4(chk2, chk2_z);
		if ((chk2_z != tst2_z) || memcmp(tst2, chk2, tst2_z))
			return tst2;
		/* Prepend hide_z spaces to script text to hide it. */
		scrpt = malloc(hide_z + text_z);
		if (!scrpt)
			return 0;
		memset(scrpt, (int) ' ', hide_z);
		memcpy(&scrpt[hide_z], text, text_z);
	} else {			/* Reexecute */
		if (*xecc) {
			scrpt = malloc(512);
			if (!scrpt)
				return 0;
			sprintf(scrpt, xecc, me);
		} else {
			scrpt = me;
		}
	}
	j = 0;
	varg[j++] = argv[0];		/* My own name at execution */
	if (ret && *opts)
		varg[j++] = opts;	/* Options on 1st line of code */
	if (*inlo)
		varg[j++] = inlo;	/* Option introducing inline code */
	varg[j++] = scrpt;		/* The script itself */
	if (*lsto)
		varg[j++] = lsto;	/* Option meaning last option */
	i = (ret > 1) ? ret : 0;	/* Args numbering correction */
	while (i < argc)
		varg[j++] = argv[i++];	/* Main run-time arguments */
	varg[j] = 0;			/* NULL terminated array */
#if DEBUGEXEC
	debugexec(shll, j, varg);
#endif
	execvp(shll, varg);
	return shll;
}

int main(int argc, char ** argv)
{
#if DEBUGEXEC
	debugexec("main", argc, argv);
#endif
#if !TRACEABLE
	untraceable(argv[0]);
#endif
	argv[1] = xsh(argc, argv);
	fprintf(stderr, "%s%s%s: %s\n", argv[0],
		errno ? ": " : "",
		errno ? strerror(errno) : "",
		argv[1] ? argv[1] : "<null>"
	);
	return 1;
}
