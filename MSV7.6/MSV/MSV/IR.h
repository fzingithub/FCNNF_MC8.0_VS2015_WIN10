#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED

#include "llvm/ADT/SmallVector.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

#include "llvm/ExecutionEngine/RuntimeDyld.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Bitcode/BitcodeWriterPass.h"
#include "llvm/Bitcode/BitstreamWriter.h"
#include "llvm-c/BitWriter.h"
#include <llvm/Support/SourceMgr.h>

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <cmath>

#include "IRStruct.h"
#include "SyntaxTree.h"
#include "SyntaxNode.h"
#include "Setting.h"

using namespace llvm;
using namespace std;

struct Asss_Struct{
	string label;
	vector<string> left;
	vector<string> right;
	int priority;
};

struct VarInfo{
	Type *type;//变量类型
	string name;//变量名
};

class IRSymbol
{
public:
	IRSymbol(AllocaInst* InstVar, bool sign) :IsSigned(sign), AllocaInstVar(InstVar), GlobalValue(nullptr){}
	IRSymbol(Value* GlobalVar, bool sign) : IsSigned(sign), AllocaInstVar(nullptr), GlobalValue(GlobalVar){}
	~IRSymbol(){};
	void SetIsSigned(bool sign){
		IsSigned = sign;
	}
	bool GetIsSigned(){
		return IsSigned;
	}
	void SetAllocaInstVar(AllocaInst* InstVar){
		AllocaInstVar = InstVar;
	}
	Value* GetAllocaInstVar(){
		return AllocaInstVar;
	}
	Value* GetGlobalVar(){
		return GlobalValue;
	}
	Value* GetLocalVar(){
		return AllocaInstVar;
	}
private:
	bool IsSigned;
	AllocaInst *AllocaInstVar;
	Value *GlobalValue;
};

class IR
{
public:
	IR();
	~IR(){}

	/**
	* 遍历m_IRTree,遇到每个结点，调用Stmt2IR函数
	* @param 传入待分析的m_IRTree
	* @return void
	*/
	void Trslt2IR(CSyntaxTree *m_GlbVarTree, CSyntaxNode *function_tree, CSyntaxTree *IRTree);

	void __InitWinStruct();
	void __InitWinApiMap();

	/**add by yubin 2015-4-7
	* 将每个结点转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void Stmt2IR(CSyntaxNode *pTree);

	/*
	*根据结构体定义的模块和declare树得到对应的llvm type
	*@param module (声明结构体的模块) pTree(声明语句)
	*@return 与声明对应的llvm type
	*/
	Type* GetType(CSyntaxNode *pTree);

	//根据传入的基础类型和语法树
	VarInfo __GetPointerInfo(Type *type, CSyntaxNode *pTree);

	//根据传入的基础类型和语法树，处理Array_Declare_STA的情况
	VarInfo __GetArrayInfo(Type *type, CSyntaxNode *pTree);

	//根据传入的基础类型和语法树，处理变量的声明
	VarInfo __GetVarInfo(Type *type, CSyntaxNode *pTree);

	//根据传入的基础类型和语法树，处理函数的形参
	VarInfo __GetFuncParaInfo(CSyntaxNode *pTree);

	/**
	* 将声明转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __Declr2IR(CSyntaxNode *pTree, bool sign);

	//异步通信中使用，将声明语法树中数组元素个数加一,因为第一个元素有特殊作用
	bool __AsyncSubscript(CSyntaxNode *pTree);

	//异步通信中使用，给有特殊作用的首元素赋值,结构体第一个元素存长度,第二个元素为0
	void __AsyncAss(Value *);

	//全局变量清零
	void __GlbSetZero();

	/*
	*为聚集类型赋初值，如数组、结构体
	*@param   value(值)  right(要赋的值)
	*/
	void __AssAggregateTy(Value *value, CSyntaxNode *right);

	/*
	*为聚集类型赋初值，如数组、结构体
	*@param   value(值)  right(要赋的值)
	*/
	void __AssStructTy(Value *value, CSyntaxNode *right);

	/**
	* 检查二元操作操作数类型是否合法，并转换为合法类型
	* @param 左值 左值符号 右值 右值符号
	* @return 二者转换后的类型，符号体现在prsnt_sign中
	*/
	Type* __CheckType(Value *&LValue, bool LSigned, Value *&RValue, bool RSigned);
	Type* __CheckType(Type* LType, bool LSigned, Type* RType, bool RSigned);
	
	/**
	* 赋值语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __Ass2IR(CSyntaxNode *pTree);

	/**add by yubin 2015-4-7
	* 顺序语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __Chop2IR(CSyntaxNode *pTree);

	/**
	* 表达式转成对应的IR代码
	* @param pTree(待分析的语法树) load(是否读取最终值(取这个数还是取它的地址))
	* @return 转换之后的结果
	*/
	Value* __Expr2IR(CSyntaxNode *pTree, bool load);

	/**
	*取得表达式中数组的值
	*@param pTree(数组所在的语法树)
	*@return 转换后的值
	*/
	Value* __Array2IR(CSyntaxNode *pTree, bool load);

	/**
	*取得表达式中结构体的值
	*@param pTree(结构体所在的语法树) StructP(代表操作符是 ->（true）.(false))
	*@return 转换后的值
	*/
	Value* __Struct2IR(CSyntaxNode *pTree, bool StructP);

	/**
	*取得表达式中结构体成员的值,递归处理结构体成员还是结构体的情况
	*@param pTree(数组所在的语法树) value(传入结构体指针,最终返回结构体成员地址)
	*@return
	*/
	Value* __StructEle2IR(CSyntaxNode *pTree, Value *value, string sname = "");

	void __WhetherOutput(CSyntaxNode *pTree);

	//在IR中生成一个function $$IsOutPut()
	void __CreateIsOutput();

	/**
	* 输出语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __StateOut2IR(CSyntaxNode *pTree);

	/**
	* 对不同的基本类型进行输出
	* @param 待输出的变量
	* @return void
	*/
	void __TypeOutput(Constant *putsFunc, Value *outPutVar);

	//为了减小输出语句的规模，将全局变量的输出变为一个函数
	void __CreateGlbOutFunc();

	/**
	* 变量自加操作转成对应的IR代码
	* @param 待处理的变量
	* @return void
	*/
	void __AddOne2IR(Value *alloc);

	//将一个地址中的数据读取出来，根据地址的类型为数组或者指针，有不同的读取方法
	Value* __Load2IR(Value *alloc);

	/*
	将变量值转为对应类型的地址
	*/
	Value* __AddVar2IR(CSyntaxNode *pTree);

	Value* __AddVarList2IR(CSyntaxNode *pTree);

	Value* __ValueList2IR(CSyntaxNode *pTree);

	/**
	* 加法操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __Add2IR(CSyntaxNode *pTree);

	/**
	* 减法操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __Sub2IR(CSyntaxNode *pTree);

	/**
	* 乘法操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __Mul2IR(CSyntaxNode *pTree);

	/**
	* 除法操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __Div2IR(CSyntaxNode *pTree);

	/**
	* 取余操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __Mod2IR(CSyntaxNode *pTree);

	void __StructTree2IR(CSyntaxNode *sTree);
	/**
	* while语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __While2IR(CSyntaxNode *pTree);

	/**
	* if语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __If2IR(CSyntaxNode *pTree);

	/**
	* 将if语句和while语句中的条件表达式转为IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	Value* __Cond2IR(CSyntaxNode *pTree);

	//三目运算符 xxx := if a>b then x else y
	Value* __ThreeOp2IR(CSyntaxNode *pTree);
	
	/**
	* 等于转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __EQ2IR(CSyntaxNode *pTree);

	/**
	* 不等于转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __NE2IR(CSyntaxNode *pTree);

	/**
	* 大于转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __GT2IR(CSyntaxNode *pTree);

	/**
	* 小于转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __LT2IR(CSyntaxNode *pTree);

	/**
	* 大于等于转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __GE2IR(CSyntaxNode *pTree);

	/**
	* 小于等于转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __LE2IR(CSyntaxNode *pTree);

	/**
	* 非转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __Neg2IR(CSyntaxNode *pTree);

	/**
	* 与转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __And2IR(CSyntaxNode *pTree);

	Value* __BAnd2IR(CSyntaxNode *pTree);

	/**
	* 或转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __Or2IR(CSyntaxNode *pTree);

	Value* __BOr2IR(CSyntaxNode *pTree);

	Value* __BXOr2IR(CSyntaxNode *pTree);

	Value* __BNeg2IR(CSyntaxNode *pTree);

	/**
	* 左移转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __Shl2IR(CSyntaxNode *pTree);

	/**
	* 右移转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value* __Shr2IR(CSyntaxNode *pTree);

	/**
	*类型强制转换
	*@param 待转换的值 转换类型 原值是否有符号
	*@return 转换后的值
	*/
	Value* __Cast2IR(Value *value, Type *type, bool sign);

	/**
	* 函数调用语句转为IR代码
	* @param 传入待分析的语法树
	* @return 转换后的IR代码
	*/
	Value* __Call2IR(CSyntaxNode *pTree);

	string __GetFunctionName(CSyntaxNode *pTree, bool &isWinAPI);

	Function* __GetFunction(CSyntaxNode *pTree, string name, bool &load);

	Type* __getFunctionType(CSyntaxNode *pTree);

	/**
	* 函数定义语句转为IR代码
	* @param 传入待分析的语法树
	* @return 转换后的IR代码
	*/
	void func2IR(CSyntaxNode *pTree);

	void TraverseFuncTree(CSyntaxNode *pTree);

	/**
	* 处理全局变量
	* @param pTrlee(全局变量树)
	*/
	void Global2IR(CSyntaxNode *pTrlee);

	//处理or的时候，预先将一些变量加入符号表
	void OrGlobal2IR();

	/**
	*处理全局变量初始化
	* @param pTrlee(全局变量树) sign(是否有符号）
	)
	*/
	void __GlobalInit(CSyntaxNode *pTrlee, bool sign);

	/**
	* 输出语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __OutType(Constant* putsFunc, map<string, IRSymbol *> tempSTable);

	//bool InstIRSymbol(string name, AllocaInst* InstVar, bool sign);

	/**
	* 获取变量存放地址
	* @param name(变量名)
	* @return 变量存放地址
	*/
	Value* __STableSearch(string name, bool outputerror = true);

	//mode : 0 stdin、 1 stdout、2 stderr
	Value* __GetStdIO(int mode);

	bool InstIRSymbol(string name, AllocaInst* InstVar, bool sign);


	//测试用
	void __PrintIRPrsnt(string str);

	//调用$$开头的内部写死的函数时，替换为真正的函数名
	string __GetRealFunctionName(string name);

	/**
	* input操作转成对应的IR代码
	* @param 语法树
	* @return void
	*/
	void __Input2IR(CSyntaxNode *pTree);

	/**
	* output操作转成对应的IR代码
	* @param 待处理的变量
	* @return void
	*/
	void __Output2IR(CSyntaxNode *pTree);

	/**
	* return语句转成对应的IR代码
	* @param 待处理的变量
	* @return void
	*/
	void __Return2IR(CSyntaxNode *pTree);

	Value* __IRSizeof(CSyntaxNode *pTree);

	void __MyCreateThread(CSyntaxNode *pTree);

	//处理多文件,处理extern_tree
	void __FuncFile(CSyntaxTree *m_GlbVarTree, CSyntaxNode *function_tree);

	//处理extern_tree
	void __ExTree2IR(CSyntaxNode *eTree);

	//yangkai  在模型图中生成一条新的边
	void __GenerateEdge(CSyntaxNode *pTree);

	//yangkai 得到变量的值并拼接成一个字符串
	void __GetVarsValue();

	//将局部变量的值拼接到字符串
	void __GetLocalVar(CSyntaxNode * pTree);

	//yangkai 拼接每个变量的值
	void __CatValue(Value* value);

	//yangkai 生成dot文件
	void __Draw();

	/*
	*根据结构体名称得到对应的IRStruct
	*@param name
	*@return IRStruct*
	*/
	static IRStruct* GetIRStruct(string name);

	DataLayout* getDataLayout(){
		return m_dataLayout;
	}

	int IR::CalculateAlign(Type *type);
	int IR::CalculateSize(Type *type);
public:
	static map<string,StructType*> udef;
	static map<string,IRStruct*> m_IRStruct; // 结构体名到IRStruct的映射
	static bool prsnt_sign;                  // 当前Value的是否有符号，每当进行运算时都应及时改变它的值
private:
	LLVMContext *m_context;
	SMDiagnostic *m_smd;
	Module *m_module;
	IRBuilder<> *m_builder;
	DataLayout *m_dataLayout;
	map<string,IRSymbol*> m_IRSTable;     // 符号表
	map<string,IRSymbol*> m_GlobalSTable;
	map<string,BasicBlock*> m_IRLabel;    // 字符串到标签的映射
	map<string,string> m_WinApiMap;
	map<string,CSyntaxNode*> m_Funcs;
	set<string> m_FuncNames;
	BasicBlock *rightEnd = nullptr;       // 翻译函数的时候记得备份一下，就像备份符号表一样
	//Value *IRTemp = nullptr;              // IR中的void*类型的变量,用于强制类型转换之后的存储
	Value *m_StNum = nullptr;             // 状态数
	bool m_InFunc;                        // 标记是否在函数中
};

#endif