#ifndef MCTYPES_H_
#define MCTYPES_H_


enum RETURNTYPE{
	UNKNOWNTYPE = 0,  // unknown type
	VOIDTYPE = 1,     // void type
	BOOLTYPE = 2,     // bool type
	CHARTYPE = 3,     // char type
	UCHARTYPE = 4,    // unsigned char type
	SHORTTYPE = 5,    // short type
	USHORTTYPE = 6,   // unsigned short type
	INTTYPE = 7,      // int type
	UINTTYPE = 8,     // unsigned int type
	LONGTYPE = 9,     // long type
	ULONGTYPE = 10,   // unsigned long type
	FLOATTYPE = 11,   // float type
	DOUBLETYPE = 12,  // double type
	LLONGTYPE = 13,   // long long type
	ULLONGTYPE = 14,  // unsigned long long type
	STRUCT_NAME_TYPE = 15, // struct name type
	STRUCTTYPE = 16,  // struct type
	POINTERTYPE = 27, // pointer type
	FUNCPTYPE = 18,   // function pointer type

	STRTYPE = 19,     // string type
	FILETYPE = 20,    // file type
	LISTTYPE = 21,    // list type

	CRITICAL_SECTION_TYPE = 23, // windows type
	TIMECAPS_TYPE = 24,         // windows type

	ARITHMETICTYPE = 100
};


enum SymbolType{
	UNDEFINE = 0,
	SYMBOLVOID = 1,
	SYMBOLBOOL = 3,
	SYMBOLCHAR = 5,
	SYMBOLUCHAR = 7,
	SYMBOLSHORT = 9,
	SYMBOLUSHORT = 11,
	SYMBOLINT = 13,
	SYMBOLUINT = 15,
	SYMBOLLONG = 17,
	SYMBOLULONG = 19,
	SYMBOLLLONG = 21,
	SYMBOLULLONG = 23,
	SYMBOLFLOAT = 25,
	SYMBOLDOUBLE = 27,
	SYMBOLSTRUCT = 29,

	SYMBOLLIST = 31,  // List
	SYMBOLFUNCP = 33, // 指向函数的函数指针
	SYMBOLFILE = 35,
	SYMBOLSTR = 37,

	SYMBOLNULL = 100,
	SYMBOLPOINTER = 102,

	SYMBOLCRITICALSECTION = 120, // windows数据类型
	SYMBOLTIMECAPS = 121,        // windows数据类型
	IDENTIFIER = 200           // identifier or variable name

};

enum NODETYPE{
	NEXT_STA,		// 必须有下一个状态,执行
	PREVIOUS_STA,
	W_NEXT_STA,		// 可以有下一个状态,也可以没有.如果有,就执行
	MORE_STA,		// 还有更多程序要执行的标志
	CHOP_STA,
	SKIP_STA,
	ALWAYS_STA,		// 一直执行
	SOMETIMES_STA,
	LENGTH_STA,

	IF_STA,
	IF_ELSE_STA,
	FOR_TIMES_STA,
	FOR_STA,
	FOR_STA_CON,

	WHILE_DO_STA,
	DO_WHILE_STA,
	WHEN_DO_STA,
	REPEAT_UNTIL_STA,

	DECLARE_STA,
	FRAME_STA,
	PARALLEL_STA,	// 并行语句
	PROJECTION_STA,	// 投影语句

	FINAL_STA,		// 结束语句
	HALT_STA,		// 暂停语句,即当满足条件是终止执行
	KEEP_STA,		// 除了终态,一直执行

	AWAIT_STA,		// 等待语句
	IMPLY_STA,		// 蕴涵
	IFF_STA,		// 当且仅当

	DISPLAY_STA,
	REQUEST_STA,

	TYPE_CAST_STA,	// 强制类型转换
	FUNCTION_STA,	// 函数调用
	EXT_FUNCTION_STA,	// 外部函数调用
	FUNCTION_DEFINE_STA,// 无返回值的函数定义

	STRUCT_DEFINE_STA,	// 结构体定义语句
	STRUCT_DEC,			// 结构体类型变量的声明

	PREFIX_STA,
	EXIST_STA,		// 局部变量声明
	RETURN_STA,

	IDENT_EXP,
	STR_EXP,
	INTEGER_EXP,
	ARRAY_EXP,
	FLOATLITERAL_EXP,
	CHARLITERAL_EXP,
	ADDRESS_EXP,
	ADDR_VAR,		// 地址变量
	ADDR_VAR_LIST,	// 地址变量偏移

	PARAMETER_EXP,
	FORMAL_PARAMETER_EXP,
	ACTUAL_PARAMETER_EXP,
	ADDRESS_PARAMETER_EXP,
	ARRAY_PARAMETER_EXP,
	POINTER_ARRAY_PARAMETER_EXP,
	STRUCT_ARRAY_PARAMETER_EXP,
	TRUE_EXP,
	FALSE_EXP,
	IF_ELSE_EXP,

	ADD_EXP,
	SUB_EXP,
	MUL_EXP,
	DIV_EXP,
	MOD_EXP,
	LST_EXP,	// 左移
	RST_EXP,	// 右移
	BAN_EXP,	// 按位与
	BOR_EXP,	// 按位或
	XOR_EXP,	// 异或
	BNE_EXP,	// 按位非
	CON_EXP,
	MINUS_EXP,

	LT_EXP,
	GT_EXP,
	LE_EXP,
	GE_EXP,
	NE_EXP,
	NEGATION_EXP,

	AND_EXP,
	OR_EXP,

	EMPTY_EXP,

	EQU_EXP,

	ASS_EQU_EXP,	// <==
	EX_ASS_EXP,		// 由:=得到的<==其节点类型

	UNITASSIGN_EXP,	// :=

	ARRAY_DECLARE_STA,	/// node type for array declaration 数组变量的声明

	CHOPSTAR_STA,	// 新定义的CHOP_STAR
	FIN_EXP,

	FOPEN_EXP,
	FPUTS_EXP,

	EMPTYNULL_EXP,	// 仿真empty节点的下一节点

	FALSE_REDU_EXP, // 建模化简失败节点

	FRAME_FREE_STA,
	UNFRAME_STA,
	NULL_EXP,
	STRUCT_LIST_ASSVALUE_STA,	// 结构体初始化列表{{1,"he"},{2,"hello"}}
	UNSIGN_DECLARATION_STA,		// unsigned或者signed定义语句

	FUNCP_PARAMETER_EXP,	// 参数类型是函数指针
	ESCAPECHARNULL_EXP,		// 转义字符 '\0'
	STRUCT_ADDRESS_EXP,
	STRUCT_VALUE_EXP,
	STRUCTP_LIST_EXP,
	STRUCT_NAME,
	SYSTEM_STA,
	DLL_CALL_STA,
	NEG_EXPONENT_EXP,
	POS_EXPONENT_EXP,
	STATE_OUTPUT_STA,
	FUNC_RETURN_TYPE,
	FUNC_DEC_STA,		// 函数声明
	ARRAY_CAST_EXP,

	GLOBAL_INITSET_STA, // 新加一个节点，后期IR转换的时候，遇见此结点类型，就将全局符号表变为0
	PROJECTION_PLUS_STA,
	LIST_FUNC_STA,
	POINTER_EXP,
	TYPE,
	ARRAY_DEC,
	IDENT_DEC,
	DEC_VAR_LIST,
	POINTER_TYPE,
	ARROW_EXP,
	COMMA_EXP,
	DOT_EXP,
	SIZEOF_EXP,
	UNARY_ADD_EXP,
	UNARY_SUB_EXP,
	CAST_TYPE,
	ELEMENT_LIST,
	ELEMENT,
	OPTION_EXP,
	THREAD_FUNC,
	CREATE_THREAD,
	NODELENGTH,
	LABEL_STA,
	CAL_PATH_TIME,
	EXTERN_VAR_DEC,		//多文件
	EXTERN_FUNC_DEC,	//多文件
	GENERATE_EDGE_STA,	//在模型图中生成一条新的边
	DRAW_STA,			//生成dot文件
	FINDSAMENODE_STA	//建模时找相同节点
};

#endif