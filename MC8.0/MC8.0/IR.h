#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED

#include <llvm/ADT/SmallVector.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

#include <llvm/ExecutionEngine/RuntimeDyld.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Bitcode/BitcodeWriterPass.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <llvm-c/BitWriter.h>
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
	Type *type;//��������
	string name;//������
};

class IRSymbol
{
public:
	explicit IRSymbol(AllocaInst *inst, bool sign) :isSigned(sign), inst(inst), value(nullptr){}
	explicit IRSymbol(Value *value, bool sign) : isSigned(sign), inst(nullptr), value(value){}

	void SetIsSigned(bool sign){
		isSigned = sign;
	}

	bool GetIsSigned(){
		return isSigned;
	}

	void SetAllocaInstVar(AllocaInst* InstVar){
		inst = InstVar;
	}

	Value* GetAllocaInstVar(){
		return inst;
	}

	Value* GetGlobalVar(){
		return value;
	}

	Value* GetLocalVar(){
		return inst;
	}

private:
	bool isSigned;
	AllocaInst *inst;
	Value *value;
};

class IR
{
public:
	IR();
	~IR(){}

	/**
	* ����m_IRTree,����ÿ����㣬����Stmt2IR����
	* @param �����������m_IRTree
	* @return void
	*/
	void Trslt2IR(CSyntaxTree *m_GlbVarTree, CSyntaxNode *function_tree, CSyntaxTree *IRTree);

	void __InitWinStruct();
	void __InitWinApiMap();

	/**add by yubin 2015-4-7
	* ��ÿ�����ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void Stmt2IR(CSyntaxNode *pTree);

	/*
	*���ݽṹ�嶨���ģ���declare���õ���Ӧ��llvm type
	*@param module (�����ṹ���ģ��) pTree(�������)
	*@return ��������Ӧ��llvm type
	*/
	Type* GetType(CSyntaxNode *pTree);

	//���ݴ���Ļ������ͺ��﷨��
	VarInfo __GetPointerInfo(Type *type, CSyntaxNode *pTree);

	//���ݴ���Ļ������ͺ��﷨��������Array_Declare_STA�����
	VarInfo __GetArrayInfo(Type *type, CSyntaxNode *pTree);

	//���ݴ���Ļ������ͺ��﷨�����������������
	VarInfo __GetVarInfo(Type *type, CSyntaxNode *pTree);

	//���ݴ���Ļ������ͺ��﷨�������������β�
	VarInfo __GetFuncParaInfo(CSyntaxNode *pTree);

	/**
	* ������ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Declr2IR(CSyntaxNode *pTree, bool sign);

	//�첽ͨ����ʹ�ã��������﷨��������Ԫ�ظ�����һ,��Ϊ��һ��Ԫ������������
	bool __AsyncSubscript(CSyntaxNode *pTree);

	//�첽ͨ����ʹ�ã������������õ���Ԫ�ظ�ֵ,�ṹ���һ��Ԫ�ش泤��,�ڶ���Ԫ��Ϊ0
	void __AsyncAss(Value *);

	//ȫ�ֱ�������
	void __GlbSetZero();

	/*
	*Ϊ�ۼ����͸���ֵ�������顢�ṹ��
	*@param   value(ֵ)  right(Ҫ����ֵ)
	*/
	void __AssAggregateTy(Value *value, CSyntaxNode *right);

	/*
	*Ϊ�ۼ����͸���ֵ�������顢�ṹ��
	*@param   value(ֵ)  right(Ҫ����ֵ)
	*/
	void __AssStructTy(Value *value, CSyntaxNode *right);

	/**
	* ����Ԫ���������������Ƿ�Ϸ�����ת��Ϊ�Ϸ�����
	* @param ��ֵ ��ֵ���� ��ֵ ��ֵ����
	* @return ����ת��������ͣ�����������prsnt_sign��
	*/
	Type* __CheckType(Value *&LValue, bool LSigned, Value *&RValue, bool RSigned);
	Type* __CheckType(Type* LType, bool LSigned, Type* RType, bool RSigned);
	
	/**
	* ��ֵ���ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Ass2IR(CSyntaxNode *pTree);

	/**add by yubin 2015-4-7
	* ˳�����ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Chop2IR(CSyntaxNode *pTree);

	/**
	* ���ʽת�ɶ�Ӧ��IR����
	* @param pTree(���������﷨��) load(�Ƿ��ȡ����ֵ(ȡ���������ȡ���ĵ�ַ))
	* @return ת��֮��Ľ��
	*/
	Value* __Expr2IR(CSyntaxNode *pTree, bool load);

	/**
	*ȡ�ñ��ʽ�������ֵ
	*@param pTree(�������ڵ��﷨��)
	*@return ת�����ֵ
	*/
	Value* __Array2IR(CSyntaxNode *pTree, bool load);

	/**
	*ȡ�ñ��ʽ�нṹ���ֵ
	*@param pTree(�ṹ�����ڵ��﷨��) StructP(����������� ->��true��.(false))
	*@return ת�����ֵ
	*/
	Value* __Struct2IR(CSyntaxNode *pTree, bool StructP);

	/**
	*ȡ�ñ��ʽ�нṹ���Ա��ֵ,�ݹ鴦��ṹ���Ա���ǽṹ������
	*@param pTree(�������ڵ��﷨��) value(����ṹ��ָ��,���շ��ؽṹ���Ա��ַ)
	*@return
	*/
	Value* __StructEle2IR(CSyntaxNode *pTree, Value *value, string sname = "");

	void __WhetherOutput(CSyntaxNode *pTree);

	//��IR������һ��function $$IsOutPut()
	void __CreateIsOutput();

	/**
	* ������ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __StateOut2IR(CSyntaxNode *pTree);

	/**
	* �Բ�ͬ�Ļ������ͽ������
	* @param ������ı���
	* @return void
	*/
	void __TypeOutput(Constant *putsFunc, Value *outPutVar);

	//Ϊ�˼�С������Ĺ�ģ����ȫ�ֱ����������Ϊһ������
	void __CreateGlbOutFunc();

	/**
	* �����ԼӲ���ת�ɶ�Ӧ��IR����
	* @param ������ı���
	* @return void
	*/
	void __AddOne2IR(Value *alloc);

	//��һ����ַ�е����ݶ�ȡ���������ݵ�ַ������Ϊ�������ָ�룬�в�ͬ�Ķ�ȡ����
	Value* __Load2IR(Value *alloc);

	/*
	������ֵתΪ��Ӧ���͵ĵ�ַ
	*/
	Value* __AddVar2IR(CSyntaxNode *pTree);

	Value* __AddVarList2IR(CSyntaxNode *pTree);

	Value* __ValueList2IR(CSyntaxNode *pTree);

	/**
	* �ӷ�����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __Add2IR(CSyntaxNode *pTree);

	/**
	* ��������ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __Sub2IR(CSyntaxNode *pTree);

	/**
	* �˷�����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __Mul2IR(CSyntaxNode *pTree);

	/**
	* ��������ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __Div2IR(CSyntaxNode *pTree);

	/**
	* ȡ�����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __Mod2IR(CSyntaxNode *pTree);

	void __StructTree2IR(CSyntaxNode *sTree);
	/**
	* while���ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __While2IR(CSyntaxNode *pTree);

	/**
	* if���ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __If2IR(CSyntaxNode *pTree);

	/**
	* ��if����while����е��������ʽתΪIR����
	* @param ������������﷨��
	* @return void
	*/
	Value* __Cond2IR(CSyntaxNode *pTree);

	//��Ŀ����� xxx := if a>b then x else y
	Value* __ThreeOp2IR(CSyntaxNode *pTree);
	
	/**
	* ����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __EQ2IR(CSyntaxNode *pTree);

	/**
	* ������ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __NE2IR(CSyntaxNode *pTree);

	/**
	* ����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __GT2IR(CSyntaxNode *pTree);

	/**
	* С��ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __LT2IR(CSyntaxNode *pTree);

	/**
	* ���ڵ���ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __GE2IR(CSyntaxNode *pTree);

	/**
	* С�ڵ���ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __LE2IR(CSyntaxNode *pTree);

	/**
	* ��ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __Neg2IR(CSyntaxNode *pTree);

	/**
	* ��ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __And2IR(CSyntaxNode *pTree);

	Value* __BAnd2IR(CSyntaxNode *pTree);

	/**
	* ��ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __Or2IR(CSyntaxNode *pTree);

	Value* __BOr2IR(CSyntaxNode *pTree);

	Value* __BXOr2IR(CSyntaxNode *pTree);

	Value* __BNeg2IR(CSyntaxNode *pTree);

	/**
	* ����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __Shl2IR(CSyntaxNode *pTree);

	/**
	* ����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value* __Shr2IR(CSyntaxNode *pTree);

	/**
	*����ǿ��ת��
	*@param ��ת����ֵ ת������ ԭֵ�Ƿ��з���
	*@return ת�����ֵ
	*/
	Value* __Cast2IR(Value *value, Type *type, bool sign);

	/**
	* �����������תΪIR����
	* @param ������������﷨��
	* @return ת�����IR����
	*/
	Value* __Call2IR(CSyntaxNode *pTree);

	string __GetFunctionName(CSyntaxNode *pTree, bool &isWinAPI);

	Function* __GetFunction(CSyntaxNode *pTree, string name, bool &load);

	Type* __getFunctionType(CSyntaxNode *pTree);

	/**
	* �����������תΪIR����
	* @param ������������﷨��
	* @return ת�����IR����
	*/
	void func2IR(CSyntaxNode *pTree);

	void TraverseFuncTree(CSyntaxNode *pTree);

	/**
	* ����ȫ�ֱ���
	* @param pTrlee(ȫ�ֱ�����)
	*/
	void Global2IR(CSyntaxNode *pTrlee);

	//����or��ʱ��Ԥ�Ƚ�һЩ����������ű�
	void OrGlobal2IR();

	/**
	*����ȫ�ֱ�����ʼ��
	* @param pTrlee(ȫ�ֱ�����) sign(�Ƿ��з��ţ�
	)
	*/
	void __GlobalInit(CSyntaxNode *pTrlee, bool sign);

	/**
	* ������ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __OutType(Constant* putsFunc, map<string, IRSymbol *> tempSTable);

	//bool InstIRSymbol(string name, AllocaInst* InstVar, bool sign);

	/**
	* ��ȡ������ŵ�ַ
	* @param name(������)
	* @return ������ŵ�ַ
	*/
	Value* __STableSearch(string name, bool outputerror = true);

	//mode : 0 stdin�� 1 stdout��2 stderr
	Value* __GetStdIO(int mode);

	bool InstIRSymbol(string name, AllocaInst* InstVar, bool sign);


	//������
	void __PrintIRPrsnt(string str);

	/**
	* input����ת�ɶ�Ӧ��IR����
	* @param �﷨��
	* @return void
	*/
	void __Input2IR(CSyntaxNode *pTree);

	/**
	* output����ת�ɶ�Ӧ��IR����
	* @param ������ı���
	* @return void
	*/
	void __Output2IR(CSyntaxNode *pTree);

	/**
	* return���ת�ɶ�Ӧ��IR����
	* @param ������ı���
	* @return void
	*/
	void __Return2IR(CSyntaxNode *pTree);

	Value* __IRSizeof(CSyntaxNode *pTree);

	void __MyCreateThread(CSyntaxNode *pTree);

	//������ļ�,����extern_tree
	void __FuncFile(CSyntaxTree *m_GlbVarTree, CSyntaxNode *function_tree);

	//����extern_tree
	void __ExTree2IR(CSyntaxNode *eTree);

	//yangkai  ��ģ��ͼ������һ���µı�
	void __GenerateEdge(CSyntaxNode *pTree);

	//yangkai �õ�������ֵ��ƴ�ӳ�һ���ַ���
	void __GetVarsValue();

	//���ֲ�������ֵƴ�ӵ��ַ���
	void __GetLocalVar(CSyntaxNode * pTree);

	//yangkai ƴ��ÿ��������ֵ
	void __CatValue(Value* value);

	//yangkai ����dot�ļ�
	void __Draw();

	/*
	*���ݽṹ�����Ƶõ���Ӧ��IRStruct
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
	static map<string,IRStruct*> m_IRStruct; // �ṹ������IRStruct��ӳ��
	static bool prsnt_sign;                  // ��ǰValue���Ƿ��з��ţ�ÿ����������ʱ��Ӧ��ʱ�ı�����ֵ

	static Type *m_Int1Ty;
	static Type *m_Int8Ty;
	static Type *m_Int16Ty;
	static Type *m_Int32Ty;
	static Type *m_Int64Ty;
	static Type *m_Int128Ty;
	static Type *m_FloatTy;
	static Type *m_DoubleTy;

private:
	LLVMContext *m_context;
	SMDiagnostic *m_smd;
	Module *m_module;
	IRBuilder<> *m_builder;
	DataLayout *m_dataLayout;
	map<string,IRSymbol*> m_IRSTable;     // ���ű�
	map<string,IRSymbol*> m_GlobalSTable;
	map<string,BasicBlock*> m_IRLabel;    // �ַ�������ǩ��ӳ��
	map<string,string> m_WinApiMap;
	map<string,CSyntaxNode*> m_Funcs;
	set<string> m_FuncNames;
	BasicBlock *rightEnd = nullptr;       // ���뺯����ʱ��ǵñ���һ�£����񱸷ݷ��ű�һ��
	//Value *IRTemp = nullptr;              // IR�е�void*���͵ı���,����ǿ������ת��֮��Ĵ洢
	Value *m_StNum = nullptr;             // ״̬��
	bool m_InFunc;                        // ����Ƿ��ں�����
};

#endif