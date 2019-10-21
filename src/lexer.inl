
#define CODE_PRELEXER	\
	"\tvector<int> state_stack;\n" \
	"\tvector<symb_t> symbol_stack;\n" \
	"\tint next_type = -1;\n" \
	"\tsymb_t next_symb;\n" \
	"\tstring ready_chars;\n" \
	"\tstate_stack.push_back(0);\n" \
	"\tint curpos_line = 1;\n" \
	"\tint curpos_col = 1;\n" \
	"\n" \
	"\twhile(1)\n" \
	"\t{\n" \
	"\t\tint LINE = curpos_line;\n" \
	"\t\tint COLUMN = curpos_col;\n" \
	"\n" \
	"\t\twhile(next_type<0)\n" \
	"\t\t{\n" \
	"\t\t\tint ls = 0, lpos=0;\n" \
	"\t\t\tint best_match=-1, best_len=0;\n" \
	"\t\t\twhile(1)\n" \
	"\t\t\t{\n" \
	"\t\t\t\tint cc = 0;\n" \
	"\t\t\t\tif(lpos>=(int)ready_chars.size())\n" \
	"\t\t\t\t{\n" \
	"\t\t\t\t\tcc = GetChar();\n" \
	"\t\t\t\t\tif(cc<0 || cc>=128) cc=128-1;\n" \
	"\t\t\t\t\tready_chars.push_back(cc);\n" \
	"\n" \
	"\t\t\t\t\tcurpos_col++;\n" \
	"\t\t\t\t\tif(cc=='\\n') curpos_line++, curpos_col=1;\n" \
	"\t\t\t\t}\n" \
	"\t\t\t\telse\n" \
	"\t\t\t\t\tcc = ready_chars[lpos];\n" \
	"\t\t\t\tlpos++;\n" \
	"\t\t\t\tls = LEXER[cc][ls];\n" \
	"\t\t\t\tif(ls<0) break;\n" \
	"\t\t\t\tif(MATCH[ls]>=0) best_match=MATCH[ls], best_len=ready_chars.size();\n" \
	"\t\t\t\tif(cc==0) break;\n" \
	"\t\t\t}\n" \
	"\t\t\tstring st(ready_chars.begin(),ready_chars.begin()+best_len);\n" \
	"\t\t\tready_chars.erase(ready_chars.begin(),ready_chars.begin()+best_len);\n" \
	"\t\t\tif(best_len<=0) best_match=-1;\n" \
	"\t\t\tswitch(best_match)\n" \
	"\t\t\t{\n" \
	"#define COMMENT\tnext_type = -1; st.clear();\n" \

#define CODE_LEXER	\
	"\t\t\tcase   -1: Error(\"lexical error\"); return symb_t();\n" \
	"\t\t\tcase    0: { next_type = 26;  next_symb=atoi(st.c_str())&0xFF; } break;\n" \
	"\t\t\tcase    1: { next_type = 26;  next_symb=st.c_str()[1]; } break;\n" \
	"\t\t\tcase    2: { next_type = 0;  next_symb=bf_ident(st.c_str()); } break;\n" \
	"\t\t\tcase    3: { next_type = 28;  COMMENT } break;\n" \
	"\t\t\tcase    4: { next_type = 28;  COMMENT } break;\n" \
	"\t\t\tcase    5: { next_type = 28;  COMMENT } break;\n" \
	"\t\t\tdefault  : { next_type = MATCH[ls]-7;  printf(\"!\"); } break;\n" \

#define CODE_POSTLEXER	\
	"#undef COMMENT\n" \
	"\t\t\t}\n" \
	"\t\t}\n" \

#define CODE_PREPARSER	\
	"\t\tint ss = ACTION[state_stack.back()][next_type];\n" \
	"\t\tif(!ss) { Error(\"syntax error\"); printf(\"%%s\\n\",SYMB[next_type]); return symb_t(); }\n" \
	"\t\tif(ss>0)\n" \
	"\t\t{\n" \
	"\t\t\tstate_stack.push_back(ss-1);\n" \
	"\t\t\tsymbol_stack.push_back(next_symb);\n" \
	"\t\t\tnext_type = -1;\n" \
	"\t\t}\n" \
	"\t\telse\n" \
	"\t\t{\n" \
	"\t\t\tint nr = -1, nst;\n" \
	"\t\t\tsymb_t ns;\n" \
	"#define S0\t\tns\n" \
	"#define S(n)\tsymbol_stack[symbol_stack.size()-(nr-(n)+1)]\n" \
	"#define SN\t\tnr\n" \
	"\t\t\tswitch(~ss)\n" \
	"\t\t\t{\n" \

#define CODE_POSTPARSER	\
	"\t\t\tdefault: Error(\"internal error\"); return symb_t();\n" \
	"\t\t\t}\n" \
	"#undef S\n" \
	"\n" \
	"\t\t\tstate_stack.erase(state_stack.end()-nr,state_stack.end());\n" \
	"\t\t\tsymbol_stack.erase(symbol_stack.end()-nr,symbol_stack.end());\n" \
	"\t\t\tsymbol_stack.push_back(ns);\n" \
	"\t\t\tif((~ss)==0) return symbol_stack.back();\n" \
	"\t\t\tss = GOTO[state_stack.back()][nst];\n" \
	"\t\t\tif(ss<0) { Error(\"internal error\"); return symb_t(); }\n" \
	"\t\t\tstate_stack.push_back(ss);\n" \
	"\t\t}\n" \
	"\t}\n" \

