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
	SYMBOLFUNCP = 33, // ָ�����ĺ���ָ��
	SYMBOLFILE = 35,
	SYMBOLSTR = 37,

	SYMBOLNULL = 100,
	SYMBOLPOINTER = 102,

	SYMBOLCRITICALSECTION = 120, // windows��������
	SYMBOLTIMECAPS = 121,        // windows��������
	IDENTIFIER = 200           // identifier or variable name

};

enum NODETYPE{
	NEXT_STA,		// ��������һ��״̬,ִ��
	PREVIOUS_STA,
	W_NEXT_STA,		// ��������һ��״̬,Ҳ����û��.�����,��ִ��
	MORE_STA,		// ���и������Ҫִ�еı�־
	CHOP_STA,
	SKIP_STA,
	ALWAYS_STA,		// һֱִ��
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
	PARALLEL_STA,	// �������
	PROJECTION_STA,	// ͶӰ���

	FINAL_STA,		// �������
	HALT_STA,		// ��ͣ���,����������������ִֹ��
	KEEP_STA,		// ������̬,һֱִ��

	AWAIT_STA,		// �ȴ����
	IMPLY_STA,		// �̺�
	IFF_STA,		// ���ҽ���

	DISPLAY_STA,
	REQUEST_STA,

	TYPE_CAST_STA,	// ǿ������ת��
	FUNCTION_STA,	// ��������
	EXT_FUNCTION_STA,	// �ⲿ��������
	FUNCTION_DEFINE_STA,// �޷���ֵ�ĺ�������

	STRUCT_DEFINE_STA,	// �ṹ�嶨�����
	STRUCT_DEC,			// �ṹ�����ͱ���������

	PREFIX_STA,
	EXIST_STA,		// �ֲ���������
	RETURN_STA,

	IDENT_EXP,
	STR_EXP,
	INTEGER_EXP,
	ARRAY_EXP,
	FLOATLITERAL_EXP,
	CHARLITERAL_EXP,
	ADDRESS_EXP,
	ADDR_VAR,		// ��ַ����
	ADDR_VAR_LIST,	// ��ַ����ƫ��

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
	LST_EXP,	// ����
	RST_EXP,	// ����
	BAN_EXP,	// ��λ��
	BOR_EXP,	// ��λ��
	XOR_EXP,	// ���
	BNE_EXP,	// ��λ��
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
	EX_ASS_EXP,		// ��:=�õ���<==��ڵ�����

	UNITASSIGN_EXP,	// :=

	ARRAY_DECLARE_STA,	/// node type for array declaration �������������

	CHOPSTAR_STA,	// �¶����CHOP_STAR
	FIN_EXP,

	FOPEN_EXP,
	FPUTS_EXP,

	EMPTYNULL_EXP,	// ����empty�ڵ����һ�ڵ�

	FALSE_REDU_EXP, // ��ģ����ʧ�ܽڵ�

	FRAME_FREE_STA,
	UNFRAME_STA,
	NULL_EXP,
	STRUCT_LIST_ASSVALUE_STA,	// �ṹ���ʼ���б�{{1,"he"},{2,"hello"}}
	UNSIGN_DECLARATION_STA,		// unsigned����signed�������

	FUNCP_PARAMETER_EXP,	// ���������Ǻ���ָ��
	ESCAPECHARNULL_EXP,		// ת���ַ� '\0'
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
	FUNC_DEC_STA,		// ��������
	ARRAY_CAST_EXP,

	GLOBAL_INITSET_STA, // �¼�һ���ڵ㣬����IRת����ʱ�������˽�����ͣ��ͽ�ȫ�ַ��ű��Ϊ0
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
	EXTERN_VAR_DEC,		//���ļ�
	EXTERN_FUNC_DEC,	//���ļ�
	GENERATE_EDGE_STA,	//��ģ��ͼ������һ���µı�
	DRAW_STA,			//����dot�ļ�
	FINDSAMENODE_STA	//��ģʱ����ͬ�ڵ�
};

#endif