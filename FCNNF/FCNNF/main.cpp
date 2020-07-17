﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAT_LEGAL_CHECKING
#undef MAT_LEGAL_CHECKING

typedef struct {
	int row, col;      // rowNum and columnNum [int]
	float** element;   // element, two dimensions
}Mat;

typedef struct {
	Mat ActiMat;               // active value Matrix without bias column
	Mat ActiMatPlus;           // active value Matrix with bias column
	Mat SumMat;                // sum value Matrix 
	Mat WeightMat;             // weight value Matrix without bias row
	Mat WeightBiasMat;         // weight value Matrix with bias row
	Mat DeltaMat;              // backtrack temporary variable Matrix
	Mat NablaWbMat;            // gradient Matrix for weight with bias 
	Mat ActiFunDerivationMat;  // active Function Dervation Matrix

	int NeuronNum;             // number of neuron [int]
	int AcitFuncNum;           // active function [int]
}FCLayer;


typedef struct {
	int sampleCapacity;             // number of current samples [int]
	int SampleDimensionNum;    // dimensions(features) of sample [int]
	int HiddenLayerNum;        // number of hidden layer [int]
	int WeightInitWayNum;      // weight initialization mode [int]

	FCLayer *Layer;            // layers of FCNN
	Mat OnehotMat;             // onehot code Matrix

	int ClassificationNum;     // Number of categories classified [int]
	int LossFuncNum;           // loss function [int]
}FCNN;



typedef struct {
	int CompleteSampleNum;		// number of all samples [int]
	int TrainSampleNum;			// number of training samples [int]
	int TestSampleNum;			// number of test samples [int]
								//int ValidationNum;			// number of validation samples [int]
	int SampleDimensionNum;    // dimensions(features) of sample [int]
	int HiddenLayerNum;        // number of hidden layer [int]
	int WeightInitWayNum;      // weight initialization mode [int]
	float *XValArray;          // samples features value [float*]
	float *YValArray;          // samples labels value [float*]
	int *NeuronNumArray;       // numbers of every layers Neuron [int*]  
	int *ActiFuncNumArray;     // activate functions of every layers [int*]
	int ClassificationNum;     // Number of categories classified [int]
	int LossFuncNum;           // loss function [int]
	int BatchSize;			   // batch size for optimization 
}Custom;


typedef struct {
	Mat CompleteFeatureDataSet;	// complete featrue Mat for FCNN
	Mat CompleteLabelDataSet;	// complete label Mat without onehot
	Mat CompleteTrainFeature;	// complete featrue Mat for FCNN training
	Mat CompleteTrainLabel;		// complete label Mat without onehot
	Mat *BatchTrainFeature;		// batch featrue Mat for FCNN training [three dimensions]
	Mat *BatchTrainLabel;		// batch label Mat without onehot [three dimensions]
	Mat *BatchTrainLabelOneHot; // batch label Mat with onehot [three dimensions]
	Mat TestFeature;			// featrue Mat for FCNN test
	Mat TestLabel;				// label Mat without onehot
	Mat TestLabelOneHot;        // label Mat with onehot
								//Mat ValidationFeature;	// featrue Mat for FCNN Validation
								//Mat ValidationLabel;		// label Mat withoutone


	int CompleteSampleNum;		// number of all samples [int]
	int TrainSampleNum;			// number of training samples [int]
	int TestSampleNum;			// number of test samples [int]
								//int ValidationNum;			// number of validation samples [int]
	int SampleDimensionNum;    // dimensions(features) of sample [int]
	int ClassificationNum;     // Number of categories classified [int]
	int BatchSize;				// batch size for dataset
	int BatchNum;				// number of batch
	int remainder;				// the last batch size
}DataSet;

/************************************************************************/
/*                            辅助函数                                  */
/************************************************************************/
float absolute(float a)
{
	if (a < 0)
	{
		return -a;
	}
	else
	{
		return a;
	}
}


float min(float a, float b)
{
	if (a > b)
	{
		return b;
	}
	else
	{
		return a;
	}
}


int equal(float a, float b)
{
	if (absolute(a - b)<0.00000001)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


char* F2S(float f, char* str)
{
	char str1[40];
	int j = 0, k, i;
	float d;

	d = absolute(f);
	i = (int)d;//浮点数的整数部分
	while (i > 0)
	{
		str1[j++] = i % 10 + '0';
		i = i / 10;
	}
	for (k = 0; k < j; k++)
	{
		str[k] = str1[j - 1 - k];//被提取的整数部分正序存放到另一个数组
	}

	if ((int)d == 0)
	{
		str[j++] = '0';
	}

	str[j++] = '.';

	d = d - (int)d;//小数部分提取
				   //printf("小数部分%f\n", d);
	for (i = 0; i < 4; i++)
	{
		d = d * 10;
		str[j++] = (int)d + '0';
		//printf("%c", str[j - 1]);
		d = d - (int)d;

	}
	/*while (str[--j] == '0');*/
	str[j] = '\0';

	/*printf("%c\n", str[0]);*/

	//处理负数
	if (f < 0)
	{
		j = 0;
		while (str[j] != '\0')
		{
			++j;
		}
		str[j + 1] = '\0';
		while (j > 0)
		{
			str[j] = str[j - 1];
			--j;
		}
		str[0] = '-';
	}
	return str;
}
/************************************************************************/
/*                            辅助函数                                  */
/************************************************************************/






/************************************************************************/
/*                           二维矩阵操作                               */
/************************************************************************/
Mat* MatCreate(Mat* mat, int row, int col)
{
	int i;

#ifdef MAT_LEGAL_CHECKING
	if (row <= 0 || col <= 0) {
		printf("err check, unmatch matrix for MatCreate\n");
		printf("\t\trow:\n\t\t\t");
		printf("%d\n", row);
		printf("\t\tcol:\n\t\t\t");
		printf("%d\n", col);
		return NULL;
	}
#endif


	mat->element = (float**)malloc(row * sizeof(float*));
	if (mat->element == NULL) {
		printf("mat create fail!\n");
		return NULL;
	}
	for (i = 0; i < row; i++) {
		mat->element[i] = (float*)malloc(col * sizeof(float));
		if (mat->element[i] == NULL) {
			int j;
			printf("mat create fail!\n");
			for (j = 0; j < i; j++)
				free(mat->element[j]);
			free(mat->element);
			return NULL;
		}
	}

	mat->row = row;
	mat->col = col;

	return mat;
}


void MatDelete(Mat* mat)
{
	int i;

	for (i = 0; i < mat->row; i++)
		free(mat->element[i]);
	free(mat->element);
}


Mat* MatSetVal(Mat* mat, float* val)
{
	int row, col;

	for (row = 0; row < mat->row; row++) {
		for (col = 0; col < mat->col; col++) {
			(mat->element[row])[col] = val[col + row * mat->col];
		}
	}

	return mat;
}


void MatShape(const Mat* mat)
{
#ifdef MAT_LEGAL_CHECKING
	if (mat == NULL) {
		printf("err check for MatShape\n");
		return;
	}
#endif

	printf("Mat %dx%d:\n", mat->row, mat->col);
}


void MatDump(const Mat* mat)
{
	int row, col;
	char str[40];
	char * data;

#ifdef MAT_LEGAL_CHECKING
	if (mat == NULL) {
		printf("err check for MatDump\n");
		return;
	}
#endif

	printf("Mat %dx%d:\n", mat->row, mat->col);
	for (row = 0; row < mat->row; row++) {
		for (col = 0; col < mat->col; col++) {
			data = F2S((mat->element[row])[col], str);
			if (data[0] == '-') {
				printf(" %s\t", F2S((mat->element[row])[col], str));
			}
			else {
				printf("  %s\t", F2S((mat->element[row])[col], str));
			}

		}
		printf("\n");
	}
	printf("\n");
	//for (row = 0; row < mat->row; row++) {
	//	for (col = 0; col < mat->col; col++) {
	//		data = (mat->element[row])[col];
	//		if (data<0){
	//			printf("%.4f\t", data);
	//		}
	//		else{
	//			printf(" %.4f\t", data);
	//		}

	//	}
	//	printf("\n");
	//}
	//printf("\n");
}

/*全0阵*/
Mat* MatZeros(Mat* mat)
{
	int row, col;

	for (row = 0; row < mat->row; row++) {
		for (col = 0; col < mat->col; col++) {
			(mat->element[row])[col] = 0.0f;
		}
	}

	return mat;
}



/*全1阵*/
Mat* MatOnes(Mat* mat)
{
	int row, col;

	for (row = 0; row < mat->row; row++) {
		for (col = 0; col < mat->col; col++) {
			(mat->element[row])[col] = 1.0f;
		}
	}

	return mat;
}


/*对角1矩阵*/
Mat* MatEye(Mat* mat)
{
	int i;

	MatZeros(mat);
	for (i = 0; i < min(float(mat->row), float(mat->col)); i++) {
		(mat->element[i])[i] = 1.0f;
	}

	return mat;
}


/* dst = src1 + src2 */
Mat* MatAdd(Mat* src1, Mat* src2, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (!(src1->row == src2->row && src2->row == dst->row && src1->col == src2->col && src2->col == dst->col)) {
		printf("\t\terr check, unmatch matrix for MatAdd\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src1);
		printf("\t\t\t");
		MatShape(src2);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src1->row; row++) {
		for (col = 0; col < src1->col; col++) {
			(dst->element[row])[col] = (src1->element[row])[col] + (src2->element[row])[col];
		}
	}

	return dst;
}


/* dst = src1 - src2 */
Mat* MatSub(Mat* src1, Mat* src2, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (!(src1->row == src2->row && src2->row == dst->row && src1->col == src2->col && src2->col == dst->col)) {
		printf("\t\terr check, unmatch matrix for MatSub\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src1);
		printf("\t\t\t");
		MatShape(src2);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src1->row; row++) {
		for (col = 0; col < src1->col; col++) {
			(dst->element[row])[col] = (src1->element[row])[col] - (src2->element[row])[col];
		}
	}

	return dst;
}


/* dst = src1 x src2  稀疏寄存器缓存优化*/
Mat* MatMul(Mat* src1, Mat* src2, Mat* dst)
{
	int row, col;
	int i;

#ifdef MAT_LEGAL_CHECKING
	if (src1->col != src2->row || src1->row != dst->row || src2->col != dst->col) {
		printf("\t\terr check, unmatch matrix for MatMul\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src1);
		printf("\t\t\t");
		MatShape(src2);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif
	MatZeros(dst);
	float temp1 = 0.f;
	float temp2 = 0.f;
	for (row = 0; row < src1->row; row++) {
		for (col = 0; col < src1->col; col++) {
			temp1 = (src1->element[row])[col];
			if (equal(temp1, 0) == 0) {
				for (i = 0; i < src2->col; i++) {
					temp2 = (src2->element[col])[i];
					if (equal(temp2, 0) == 0) {
						(dst->element[row])[i] += temp1 * temp2;
					}
				}
			}
		}
	}

	return dst;
}


/* dst = src1 x src2 原始 */
Mat* MatMul2(Mat* src1, Mat* src2, Mat* dst)
{
	int row, col;
	int i;
	float temp;

#ifdef MAT_LEGAL_CHECKING
	if (src1->col != src2->row || src1->row != dst->row || src2->col != dst->col) {
		printf("\t\terr check, unmatch matrix for MatMul\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src1);
		printf("\t\t\t");
		MatShape(src2);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < dst->row; row++) {
		for (col = 0; col < dst->col; col++) {
			for (i = 0; i < src1->col; i++) {
				(dst->element[row])[col] += (src1->element[row])[i] * (src2->element[i])[col];
			}	
		}
	}

	return dst;
}
////2.1 子矩阵乘法 C=A'*B
//void SMblock_MultCAOB(Mat *rawCAOB, Mat rawA, Mat rawB, int si, int sj, int sk, int subm, int subn, int subp, int T, int S) {
//	int i, j, k;
//	for (j = 0; j < subn; j++) { //列号
//		for (i = 0; i < subm; i++) { //行号
//			for (k = 0; k < subp; k++) { //并行
//										 //printf("子块乘：C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",sj * T + j,sk * S + k,si*S + i,sj * T + j,si * T + i,sk*S + k);
//										 //C[j * p + k]+= A[i*m + j] * B[i * p + k];  //参考
//				(rawCAOB->element[sj * T + j])[sk * T + k] += (rawA.element[si*S + i])[sj * T + j] * (rawB.element[si*S + i])[sk * T + k];
//
//			}
//		}
//	}
//}
//
//
///* dst = src1 x src2 缓存优化*/
//Mat* MatMul2(Mat* src1, Mat* src2, Mat* dst)
//{
//
//#ifdef MAT_LEGAL_CHECKING
//	if (src1->col != src2->row || src1->row != dst->row || src2->col != dst->col) {
//		printf("\t\terr check, unmatch matrix for MatMul\n");
//		printf("\t\tsrcMatShape:\n\t\t\t");
//		MatShape(src1);
//		printf("\t\t\t");
//		MatShape(src2);
//		printf("\t\tdstMatShape:\n\t\t\t");
//		MatShape(dst);
//		return NULL;
//	}
//#endif
//
//	int rawm = src1->row;
//	int rawn = src2->col;
//	int rawp = src2->col;
//
//	// 稍后计算
//	int S = 1;  //块矩阵的行
//	int T = 2;  //块矩阵的列
//
//	//分块后子矩阵的个数h*l,A矩阵分为S*T的子矩阵，B矩阵分为T*S的子矩阵
//	static int col_M = 1;
//	static int row_N = 1;
//
//	//矩阵A、B分块后，不能全分块时，最后一行和最后一列的子矩阵的大小
//	static int col_last = 1;
//	static int row_last = 1;
//
//	//====================================================================================
//	//将矩阵rawA[rawm][rawn]分为C_M*R_N个大小为S*T的子块，ceil(x)函数返回不小于x的最小整数
//	if (rawm % S == 0) {
//		col_M = rawm / S;
//	}
//	else {
//		col_M = rawm / S + 1;
//	}
//	//AC_M = ceil((double) rawm / (double) S); //矩阵A分块后的行数
//	row_N = ceil((double)rawn / (double)T); //矩阵A分块后的列数，即矩阵B分块后的行数
//	col_last = rawm - (col_M - 1) * S;//最后一行
//	row_last = rawn - (row_N - 1) * T;//最后一列
//	printf("%d\n", row_N);
//	printf("%d\n", col_M);
//	printf("%d\n", row_last);
//	printf("%d\n", col_last);
//	//====================================================================================
//	//MatDump(dst);
//	int i, j, k;
//	int count = 0;//循环计数
//				  //循环 的顺序可根据需要更换，不会影响计算的结果
//	for (j = 0; j < row_N; j++) {
//		for (i = 0; i < col_M; i++) {
//			for (k = 0; k < row_N; k++) {
//								printf("\t 第%d层循环：  ",++count);
//				//				printf(" 分块乘法：C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",j,k,i,j,i,k);
//				int mblk = S, nblk = T, pblk = T;//默认当前参与运算的两个子矩阵块的大小，必须每次循环重新赋初值
//												 //计算当前子块的大小为mblk*nblk
//				if ((i == col_M - 1)) {
//					mblk = col_last;
//				}
//				if (j == row_N - 1) {
//					nblk = row_last;
//				}
//				if (k == row_N - 1) {
//					pblk = row_last;
//				}
//				
//				//分块矩阵乘法C=A'*B
//				//SMblock_MultCAOB(i, j, k, mblk, nblk, mblk);
//				SMblock_MultCAOB(dst, *src1, *src2, i, j, k, mblk, nblk, pblk, T, S);
//			}
//		}
//	}
//	
//	return dst;
//}


/* dst = src1 * src2 */   // Hadamard product
Mat* MatProduct(Mat* src1, Mat* src2, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (!(src1->row == src2->row && src2->row == dst->row && src1->col == src2->col && src2->col == dst->col)) {
		printf("\t\terr check, unmatch matrix for MatAdd\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src1);
		printf("\t\t\t");
		MatShape(src2);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src1->row; row++) {
		for (col = 0; col < src1->col; col++) {
			(dst->element[row])[col] = (src1->element[row])[col] * (src2->element[row])[col];
		}
	}

	return dst;
}


/* dst = src1 / src2 */   
Mat* MatDiv(Mat* src1, Mat* src2, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (!(src1->row == src2->row && src2->row == dst->row && src1->col == src2->col && src2->col == dst->col)) {
		printf("\t\terr check, unmatch matrix for MatDiv\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src1);
		printf("\t\t\t");
		MatShape(src2);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src1->row; row++) {
		for (col = 0; col < src1->col; col++) {
			(dst->element[row])[col] = (src1->element[row])[col] / (src2->element[row])[col];
		}
	}

	return dst;
}


/* dst = num * src 矩阵数乘运算 */
Mat* MatNumMul(float num, Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatNumMul\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = num * (src->element[row])[col];
	}

	return dst;
}


/* dst = num + src 矩阵数加运算 */
Mat* MatNumAdd(float num, Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatNumAdd\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = num + (src->element[row])[col];
	}

	return dst;
}


/* dst = src^T */
Mat* MatTrans(Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->col || src->col != dst->row) {
		printf("err check, unmatch matrix for MatTranspose\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < dst->row; row++) {
		for (col = 0; col < dst->col; col++) {
			(dst->element[row])[col] = (src->element[col])[row];
		}
	}

	return dst;
}






/*dst = sum(src)  src 矩阵的每一行相加*/
Mat* MatRowSum(Mat* src, Mat* dst)
{
	int row, col;
	float temp;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || dst->col != 1) {
		printf("err check, unmatch matrix for MatSum\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		temp = 0;
		for (col = 0; col < src->col; col++) {
			temp += (src->element[row])[col];
			//printf("%f\t", temp);
		}
		(dst->element[row])[0] = temp;
		//printf("%f\n", temp);
	}

	return dst;
}


/*dst = MatMax(src)  src 找出矩阵的每一行最大值*/
Mat* MatRowMax(Mat* src, Mat* dst)
{
	int row, col;
	float temp;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || dst->col != 1) {
		printf("err check, unmatch matrix for MatMax\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		temp = (src->element[row])[0];
		for (col = 1; col < src->col; col++) {
			if ((src->element[row])[col] > temp) {
				temp = (src->element[row])[col];
			}
		}
		(dst->element[row])[0] = temp;
	}

	return dst;
}

/* dst = src * src (Hadamard product)*/
Mat* MatSquare(Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("err check, unmatch matrix for MatSquare\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++) {
			(dst->element[row])[col] = (src->element[row])[col] * (src->element[row])[col];
		}
	}

	return dst;
}


/* dst = Sqrt(src) */
Mat* MatSqrt(Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("err check, unmatch matrix for MatSqrt\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++) {
			(dst->element[row])[col] = sqrt(float((src->element[row])[col]));
		}
	}

	return dst;
}


/*dst = MatExp(src)  指数作用*/
Mat* MatExp(Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("err check, unmatch matrix for MatExp\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++) {
			(dst->element[row])[col] = (float)(exp(float((src->element[row])[col])));
		}
	}

	return dst;
}




/* dst = src - vector   矩阵减向量 行减*/
Mat* MatVectorSub(Mat* src, Mat* vector, Mat *dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col || vector->row != src->row || vector->col != 1) {
		printf("err check, unmatch matrix for MatVectorSub\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tvectorShape:\n\t\t\t");
		MatShape(vector);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif


	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++) {
			(dst->element[row])[col] = (src->element[row])[col] - (vector->element[row])[0];
		}
	}

	return dst;

}


/* dst = src / vector   矩阵除向量 行除*/
Mat* MatVectorDiv(Mat* src, Mat* vector, Mat *dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col || vector->row != src->row || vector->col != 1) {
		printf("err check, unmatch matrix for MatVectorSub\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tvectorShape:\n\t\t\t");
		MatShape(vector);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}

	for (row = 0; row < src->row; row++) {
		if (absolute((vector->element[row])[0]) < 0.000001) {
			printf("err check, Divisor vector has zero.\n");
			return NULL;
		}
	}
#endif


	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++) {
			(dst->element[row])[col] = (src->element[row])[col] / (vector->element[row])[0];
		}
	}

	return dst;

}


/* dst = src 内存拷贝 */
void MatCopy(Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatCopy\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = (src->element[row])[col];
	}
}


/* dst = src^+   col+1*/
void MatPlusCol(Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || (src->col) + 1 != dst->col) {
		printf("\t\terr check, unmathed matrix for MatPlus\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return;
	}
#endif
	for (row = 0, col = 0; row < dst->row; row++) {
		(dst->element[row])[col] = 1;
	}
	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col + 1] = (src->element[row])[col];
	}
}



/* dst = src^+    row+1*/
void MatPlusRow(Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row + 1 != dst->row || (src->col) != dst->col) {
		printf("\t\terr check, unmathed matrix for MatPlus\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return;
	}
#endif
	for (row = 0, col = 0; col < dst->col; col++) {
		(dst->element[row])[col] = 0.f;
	}
	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row + 1])[col] = (src->element[row])[col];
	}
}


/************************************************************************/
/*                           二维矩阵操作                               */
/************************************************************************/
















/************************************************************************/
/*                           激活函数操作                               */
/************************************************************************/
float sigmoid(float z)
{
	return (float)(1 / (1 + exp(-z)));
}


//直接调用 math.h 里函数
//float tanh(float z){
//	return (exp(z) - exp(-z)) / (exp(z) + exp(-z));
//}
//


float relu(float z)
{
	return z > 0 ? z : 0;
}


float leakyRelu(float z, float a)
{
	return z < 0 ? a : z;
}


Mat* MatSoftmax(Mat *src, Mat *dst)
{
#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatSofmax\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	Mat tempV;
	MatCreate(&tempV, src->row, 1);   //存临时的最大行值以及求和行值向量

	MatRowMax(src, &tempV);   //求最大行值向量

	MatVectorSub(src, &tempV, dst);   //矩阵向量相减

	MatExp(dst, dst);   //矩阵求对数

	MatRowSum(dst, &tempV);   //求行求和向量

	MatVectorDiv(dst, &tempV, dst);   //矩阵向量相除

	MatDelete(&tempV);

	return dst;
}


Mat* MatNoneActi(Mat *src, Mat *dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatNoneActi\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = (src->element[row])[col];
	}

	return dst;
}


Mat* MatSigmoid(Mat *src, Mat *dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatSigmoid\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = sigmoid((src->element[row])[col]);
	}

	return dst;
}


Mat* MatTanh(Mat *src, Mat *dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatTanh\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = (float)tanh((src->element[row])[col]);
	}

	return dst;
}


Mat* MatRelu(Mat *src, Mat *dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatRelu\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = relu((src->element[row])[col]);
	}

	return dst;
}


Mat* MatLeakyRelu(float a, Mat *src, Mat *dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatLeakyRelu\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = leakyRelu((src->element[row])[col], a);
	}

	return dst;
}


//Derivative
Mat* MatDerivationSoftmax(Mat *src, Mat *dst)
{
	int row, col, i;
#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatDerivationSofmax\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	MatSoftmax(src, src);
	MatZeros(dst);

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++) {
			for (i = 0; i < src->col; i++) {
				if (i == col) {
					(dst->element[row])[col] += (src->element[row])[i] * (1 - (src->element[row])[col]);
				}
				else {
					(dst->element[row])[col] += -(src->element[row])[i] * (src->element[row])[col];
				}
				//printf("%f\n", (dst->element[row])[col]);
			}
			//printf("sum=%f\n", (dst->element[row])[col]);
		}
	}


	return dst;
}


Mat* MatDerivationNoneActi(Mat *src, Mat *dst)
{	//不激活导数值为1
	int row, col;
#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatDerivationNoneActi\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = 1.f;
	}

	return dst;
}


Mat* MatDerivationSigmoid(Mat *src, Mat *dst)
{
#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatDerivationSigmoid\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif
	Mat temp1Mat;
	Mat temp2Mat;
	MatCreate(&temp1Mat, src->row, src->col);
	MatCreate(&temp2Mat, src->row, src->col);

	MatSigmoid(src, &temp1Mat);
	//MatDump(&temp1Mat);
	MatNumMul(-1.f, &temp1Mat, &temp2Mat);
	//MatDump(&temp2Mat);
	MatNumAdd(1.f, &temp2Mat, &temp2Mat);
	//MatDump(&temp2Mat);
	MatProduct(&temp1Mat, &temp2Mat, dst);
	//MatDump(dst);
	MatDelete(&temp1Mat);
	MatDelete(&temp2Mat);
	return dst;
}


Mat* MatDerivationTanh(Mat *src, Mat *dst)
{
#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatDerivationTanh\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	Mat tempMat;
	MatCreate(&tempMat, src->row, src->col);

	MatTanh(src, &tempMat);
	//MatDump(&tempMat);
	MatSquare(&tempMat, &tempMat);
	//MatDump(&tempMat);
	MatNumMul(-1.f, &tempMat, &tempMat);
	//MatDump(&tempMat);
	MatNumAdd(1.f, &tempMat, dst);

	MatDelete(&tempMat);
	return dst;
}


Mat* MatDerivationRelu(Mat *src, Mat *dst)
{
	int row, col;
#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatDerivationRelu\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif
	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++) {
			if ((src->element[row])[col]>0) {
				(dst->element[row])[col] = 1.f;
			}
			else {
				(dst->element[row])[col] = 0.f;
			}
		}

	}

	return dst;
}


Mat* MatDerivationLeakyRelu(float a, Mat *src, Mat *dst)
{
	int row, col;
#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for MatDerivationLeakyRelu\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif
	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++) {
			if ((src->element[row])[col]>0) {
				(dst->element[row])[col] = 1.f;
			}
			else {
				(dst->element[row])[col] = a;
			}
		}

	}

	return dst;
}
/************************************************************************/
/*                           激活函数操作                               */
/************************************************************************/














/************************************************************************/
/*                           损失函数操作                               */
/************************************************************************/
Mat* OneHot(Mat *src, int k, Mat *dst)
{
	int row;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != 1 || dst->col != k) {
		printf("\t\terr check, unmathed matrix for Onehot\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tThe number of class:\n\t\t\t");
		printf("%d\n", k);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return NULL;
	}
#endif

	MatZeros(dst);

	for (row = 0; row < dst->row; row++) {
		(dst->element[row])[int((src->element[row])[0])] = 1.0f;
	}

	return dst;
}

/*均方误差*/
float MSE(Mat *src, Mat *dst)
{
	int row;
	float loss = 0.f;
	Mat sub_square_mat;
	Mat sum_row_mat;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for Loss Function MSE\t\t\n");
		printf("\t\tPredictoinMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tOneHotMatShape:\n\t\t\t");
		MatShape(dst);
		return -1.;     // 参数检查不过关返回 -1；
	}
#endif

	MatCreate(&sub_square_mat, src->row, src->col);
	MatCreate(&sum_row_mat, src->row, 1);

	MatSub(src, dst, &sub_square_mat);
	//MatDump(&sub_square_mat);
	MatSquare(&sub_square_mat, &sub_square_mat);
	//MatDump(&sub_square_mat);
	MatRowSum(&sub_square_mat, &sum_row_mat);
	//MatDump(&sum_row_mat);

	for (row = 0; row < src->row; ++row) {
		//printf("%f\t", (sum_row_mat.element[row])[0]);
		loss = loss + (sum_row_mat.element[row])[0];
	}
	loss = loss / float(src->row);

	MatDelete(&sub_square_mat);
	MatDelete(&sum_row_mat);
	return loss;
}


float CrossEntropy(Mat *src, Mat *dst)
{
	int row, col;
	float loss = 0.f;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != dst->col) {
		printf("\t\terr check, unmathed matrix for Loss Function CrossEntropy\t\t\n");
		printf("\t\tPredictoinMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tOneHotMatShape:\n\t\t\t");
		MatShape(dst);
		return -1.;     // 参数检查不过关返回 -1；
	}
#endif

	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			loss += (float)(-1 * (dst->element[row])[col] * log((src->element[row])[col]));
	}

	loss = loss / (src->row);

	return loss;
}


//对激活值求导数
Mat * MSEDerivative(Mat *ActiMat, Mat *DerivativeActiMat, Mat One_hotMat) {

#ifdef MAT_LEGAL_CHECKING
	if (ActiMat->row != DerivativeActiMat->row || ActiMat->col != DerivativeActiMat->col || ActiMat->row != One_hotMat.row || ActiMat->col != One_hotMat.col) {
		printf("\t\terr check, unmathed matrix for Loss Function MSEDerivative\t\t\n");
		printf("\t\tActiMatShape:\n\t\t\t");
		MatShape(ActiMat);
		printf("\t\tDerivativeActiMatShape:\n\t\t\t");
		MatShape(DerivativeActiMat);
		printf("\t\tOne_hotMatShape:\n\t\t\t");
		MatShape(&One_hotMat);
		return NULL;     // 参数检查不过关返回 -1；
	}
#endif

	//partial L /partial z = z - y 
	return MatSub(ActiMat, &One_hotMat, DerivativeActiMat);

}


Mat * CrossEntropyDerivative(Mat *ActiMat, Mat *DerivativeActiMat, Mat One_hotMat) {
	int row, col;
#ifdef MAT_LEGAL_CHECKING
	if (ActiMat->row != DerivativeActiMat->row || ActiMat->col != DerivativeActiMat->col || ActiMat->row != One_hotMat.row || ActiMat->col != One_hotMat.col) {
		printf("\t\terr check, unmathed matrix for Loss Function CrossEntropyDerivative\t\t\n");
		printf("\t\tActiMatShape:\n\t\t\t");
		MatShape(ActiMat);
		printf("\t\tDerivativeActiMatShape:\n\t\t\t");
		MatShape(DerivativeActiMat);
		printf("\t\tOne_hotMatShape:\n\t\t\t");
		MatShape(&One_hotMat);
		return NULL;     // 参数检查不过关返回 -1；
	}
#endif

	//partial L /partial z = -(y / z)
	for (row = 0; row < ActiMat->row; row++) {
		for (col = 0; col < ActiMat->col; col++)
			//div 0 error 
			if (equal((ActiMat->element[row])[col], 0.f) == 1) {
				(DerivativeActiMat->element[row])[col] = -(One_hotMat.element[row])[col] * 10000000000;
			}
			else {
				(DerivativeActiMat->element[row])[col] = -(One_hotMat.element[row])[col] / (ActiMat->element[row])[col];
			}
	}

	return DerivativeActiMat;

}
/************************************************************************/
/*                           损失函数操作                               */
/************************************************************************/














/************************************************************************/
/*                           权值初始化函数操作                         */
/************************************************************************/
float gaussrand_NORMAL() {
	float V1 = 0., V2 = 0., S = 0.;
	int phase = 0;
	int count = 0;
	float X;


	if (phase == 0) {
		while (count == 0 || (S >= 1 || S == 0)) {
			float U1 = (float)(rand() % 10000) / 10000.f;
			float U2 = (float)(rand() % 10000) / 10000.f;


			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
			count += 1;
		};

		float temp_S_1 = (float)log(S);
		X = V1 * (float)sqrt(-2 * temp_S_1 / S);
	}
	else
	{
		float temp_S_2 = (float)log(S);
		X = V2 * (float)sqrt(-2 * temp_S_2 / S);
	}



	phase = 1 - phase;


	return X;
}


float gaussrand(float mean, float stdc) {
	return mean + gaussrand_NORMAL() * stdc;
}


// 全0初始化 
Mat* MatInitZero(Mat *src)
{
	MatZeros(src);
	return src;
}



/* 初始化成0均值 0.01方差的标准化数据*/
Mat* MatInitRandomNormalization(Mat *src)
{
	srand((unsigned int)time(NULL));  // set randon seed

	int row, col;

	//weight
	for (row = 0; row < src->row; ++row) {
		for (col = 0; col < src->col; ++col) {
			(src->element[row])[col] = gaussrand(0.f, 0.1f);  // mean stdc
		}
	}
	return src;
}


Mat* MatInitXavier(Mat *src)
{
	srand((unsigned int)time(NULL));  // set randon seed

	int row, col;
	//weight
	for (row = 0; row < src->row; ++row) {
		for (col = 0; col < src->col; ++col) {
			(src->element[row])[col] = gaussrand(0.f, 0.1f) * (float)sqrt(1.f / src->row);  // mean stdc
		}
	}
	//set bias row 0
	//for (col = 0; col < src->col; ++col){
	//	(src->element[0])[col] = 0.f;
	//}
	return src;
}


Mat* MatInitHe(Mat *src)
{
	//srand((unsigned int)time(NULL));  // set randon seed
	srand(19950826);

	int row, col;
	//weight
	for (row = 0; row < src->row; ++row) {
		for (col = 0; col < src->col; ++col) {
			(src->element[row])[col] = gaussrand(0.f, 0.9f) * (float)sqrt(2.f / src->row);  // mean stdc
		}
	}
	//set bias row 0
	//for (col = 0; col < src->col; ++col){
	//	(src->element[0])[col] = 0.f;
	//}
	return src;
}
/************************************************************************/
/*                           权值初始化函数操作                         */
/************************************************************************/























/************************************************************************/
/*                     用户输入参数列表及传入函数                       */
/************************************************************************/
//typedef struct{
//	int CompleteSampleNum;		// number of all samples [int]
//	int TrainSampleNum;			// number of training samples [int]
//	int TestSampleNum;			// number of test samples [int]
//	int ValidationNum;			// number of validation samples [int]
//	int SampleDimensionNum;    // dimensions(features) of sample [int]
//	int HiddenLayerNum;        // number of hidden layer [int]
//	int WeightInitWayNum;      // weight initialization mode [int]
//	float *XValArray;          // samples features value [float*]
//	float *YValArray;          // samples labels value [float*]
//	int *NeuronNumArray;       // numbers of every layers Neuron [int*]  
//	int *ActiFuncNumArray;     // activate functions of every layers [int*]
//	int ClassificationNum;     // Number of categories classified [int]
//	int LossFuncNum;           // loss function [int]
//	int BatchSize;			   // batch size for optimization 
//}Custom;


int InitCustom(Custom *userDefine) {
	userDefine->CompleteSampleNum = -1;
	userDefine->TrainSampleNum = -1;
	userDefine->TestSampleNum = -1;
	//userDefine->ValidationNum = -1;
	userDefine->SampleDimensionNum = -1;
	userDefine->HiddenLayerNum = -1;
	userDefine->WeightInitWayNum = -1;
	userDefine->XValArray = NULL;
	userDefine->YValArray = NULL;
	userDefine->NeuronNumArray = NULL;
	userDefine->ActiFuncNumArray = NULL;
	userDefine->ClassificationNum = -1;
	userDefine->LossFuncNum = -1;
	userDefine->BatchSize = -1;

	return 0;
}

void DumpFloatArray(float* array, int n) {
	char str[40];
	for (int i = 0; i < n; ++i) {
		printf("%s", F2S(array[i], str));
		printf("\t");
	}
	printf("\n");
}

void DumpIntArray(int* array, int n) {
	for (int i = 0; i < n; ++i) {
		printf("%d\t", array[i]);
	}
	printf("\n");
}

int DumpCustom(Custom UserDefine) {
	printf("==================================================================== Custom Dump =====================================================================\n");
	if (UserDefine.CompleteSampleNum == -1) {
		printf("\t\t\tCustom parameter 'TrainSampleNum' uninitialized!!!\n");
		return -1;
	}
	printf("CompleteSampleNum:\t%d\n", UserDefine.CompleteSampleNum);

	if (UserDefine.TrainSampleNum == -1) {
		printf("\t\t\tCustom parameter 'TrainSampleNum' uninitialized!!!\n");
		return -1;
	}
	printf("TrainSampleNum:\t\t%d\n", UserDefine.TrainSampleNum);

	if (UserDefine.TestSampleNum == -1) {
		printf("\t\t\tCustom parameter 'TestSampleNum' uninitialized!!!\n");
		return -1;
	}
	printf("TestSampleNum:\t\t%d\n", UserDefine.TestSampleNum);

	//if (UserDefine.ValidationNum == -1){
	//	printf("\t\t\tCustom parameter 'ValidationNum' uninitialized!!!\n");
	//	return -1;
	//}
	//printf("ValidationNum:\t\t%d\n", UserDefine.ValidationNum);

	if (UserDefine.SampleDimensionNum == -1) {
		printf("\t\t\tCustom parameter 'SampleDimensionNum' uninitialized!!!\n");
		return -1;
	}
	printf("SampleDimensionNum:\t%d\n", UserDefine.SampleDimensionNum);

	if (UserDefine.HiddenLayerNum == -1) {
		printf("\t\t\tCustom parameter 'HiddenLayerNum' uninitialized!!!\n");
		return -1;
	}
	printf("HiddenLayerNum:\t\t%d\n", UserDefine.HiddenLayerNum);

	if (UserDefine.WeightInitWayNum == -1) {
		printf("\t\t\tCustom parameter 'WeightInitWayNum' uninitialized!!!\n");
		return -1;
	}
	printf("WeightInitWayNum:\t%d\n", UserDefine.WeightInitWayNum);

	if (UserDefine.ClassificationNum == -1) {
		printf("\t\t\tCustom parameter 'ClassificationNum' uninitialized!!!\n");
		return -1;
	}
	printf("ClassificationNum:\t%d\n", UserDefine.ClassificationNum);

	if (UserDefine.LossFuncNum == -1) {
		printf("\t\t\tCustom parameter 'LossFuncNum' uninitialized!!!\n");
		return -1;
	}
	printf("LossFuncNum:\t\t%d\n", UserDefine.LossFuncNum);

	if (UserDefine.BatchSize == -1) {
		printf("\t\t\tCustom parameter 'BatchSize' uninitialized!!!\n");
		return -1;
	}
	printf("BatchSize:\t\t%d\n", UserDefine.BatchSize);


	if (UserDefine.XValArray == NULL) {
		printf("\t\t\tCustom parameter 'XValArray' uninitialized!!!\n");
		return -1;
	}
	printf("XValArray:(length = %d)\t\n", UserDefine.CompleteSampleNum*UserDefine.SampleDimensionNum);
	//DumpFloatArray(UserDefine.XValArray, UserDefine.CompleteSampleNum*UserDefine.SampleDimensionNum);

	if (UserDefine.YValArray == NULL) {
		printf("\t\t\tCustom parameter 'YValArray' uninitialized!!!\n");
		return -1;
	}
	printf("YValArray:(length = %d)\t\n", UserDefine.CompleteSampleNum);
	//DumpFloatArray(UserDefine.YValArray, UserDefine.CompleteSampleNum);

	if (UserDefine.NeuronNumArray == NULL) {
		printf("\t\t\tCustom parameter 'NeuronNumArray' uninitialized!!!\n");
		return -1;
	}
	printf("NeuronNumArray:\t\n");
	DumpIntArray(UserDefine.NeuronNumArray, UserDefine.HiddenLayerNum + 2);

	if (UserDefine.ActiFuncNumArray == NULL) {
		printf("\t\t\tCustom parameter 'ActiFuncNumArray' uninitialized!!!\n");
		return -1;
	}
	printf("ActiFuncNumArray:(include output layer Acti-Function)\t\n");
	DumpIntArray(UserDefine.ActiFuncNumArray, UserDefine.HiddenLayerNum + 2);

	printf("================================================================= Custom Dump finish =================================================================\n");

	return 0;
}

void LoadParaFromCustom(Custom userDefine, DataSet *dataSet, FCNN *fcnn) {
	// Custom ==>> DataSet
	dataSet->BatchSize = userDefine.BatchSize;
	dataSet->CompleteSampleNum = userDefine.CompleteSampleNum;
	dataSet->SampleDimensionNum = userDefine.SampleDimensionNum;
	dataSet->TrainSampleNum = userDefine.TrainSampleNum;
	dataSet->TestSampleNum = userDefine.TestSampleNum;
	dataSet->ClassificationNum = userDefine.ClassificationNum;

	// Custom ==>> FCNN
	fcnn->sampleCapacity = userDefine.BatchSize; //代表当前神经网络前向传播时的数据样本的个数
	fcnn->SampleDimensionNum = userDefine.SampleDimensionNum;
	fcnn->HiddenLayerNum = userDefine.HiddenLayerNum;
	fcnn->WeightInitWayNum = userDefine.WeightInitWayNum;
	fcnn->ClassificationNum = userDefine.ClassificationNum;
	fcnn->LossFuncNum = userDefine.LossFuncNum;
}

/************************************************************************/
/*                     用户输入参数列表及传入函数                       */
/************************************************************************/










/************************************************************************/
/*                         神经网络数据集构建                           */
/************************************************************************/
//typedef struct{
//	Mat CompleteFeatureDataSet;	// complete featrue Mat for FCNN
//	Mat CompleteLabelDataSet;	// complete label Mat without onehot
//	Mat CompleteTrainFeature;	// complete featrue Mat for FCNN training
//	Mat CompleteTrainLabel;		// complete label Mat without onehot
//	Mat *BatchTrainFeature;		// batch featrue Mat for FCNN training [three dimensions]
//	Mat *BatchTrainLabel;		// batch label Mat without onehot [three dimensions]
//	Mat TestFeature;			// featrue Mat for FCNN test
//	Mat TestLabel;				// label Mat without onehot
//	//Mat ValidationFeature;		// featrue Mat for FCNN Validation
//	//Mat ValidationLabel;		// label Mat withoutone
//
//	int CompleteSampleNum;		// number of all samples [int]
//	int TrainSampleNum;			// number of training samples [int]
//	int TestSampleNum;			// number of test samples [int]
//	//int ValidationNum;			// number of validation samples [int]
//	int SampleDimensionNum;    // dimensions(features) of sample [int]
//	int BatchSize;				// batch size for dataset
//}DataSet;

int InitDataSet(DataSet *dataSet) {
	dataSet->CompleteFeatureDataSet.element = NULL;
	dataSet->CompleteLabelDataSet.element = NULL;
	dataSet->CompleteTrainFeature.element = NULL;
	dataSet->CompleteTrainLabel.element = NULL;

	dataSet->BatchTrainFeature = NULL;
	dataSet->BatchTrainLabel = NULL;
	dataSet->BatchTrainLabelOneHot = NULL;
	dataSet->TestFeature.element = NULL;
	dataSet->TestLabel.element = NULL;
	dataSet->TestLabelOneHot.element = NULL;
	//dataSet->ValidationFeature.element = NULL;
	//dataSet->ValidationLabel.element = NULL;

	dataSet->CompleteSampleNum = -1;
	dataSet->TrainSampleNum = -1;
	dataSet->TestSampleNum = -1;
	dataSet->SampleDimensionNum = -1;
	dataSet->BatchSize = -1;
	dataSet->BatchNum = -1;
	dataSet->remainder = -1;
	dataSet->ClassificationNum = -1;

	return 0;
}

void CalculateAndLoadDataSetPara(DataSet *dataSet) {
	int batchNum = 0;
	int remainder = 0;
	batchNum = dataSet->TrainSampleNum / dataSet->BatchSize;
	remainder = dataSet->TrainSampleNum % dataSet->BatchSize;
	if (remainder != 0) {
		batchNum = batchNum + 1;
	}
	dataSet->BatchNum = batchNum;
	dataSet->remainder = remainder;
}

void CreateDataSetSpace(DataSet *dataSet) {
	// init Complete dataset
	MatCreate(&dataSet->CompleteFeatureDataSet, dataSet->CompleteSampleNum, dataSet->SampleDimensionNum);
	MatZeros(&dataSet->CompleteFeatureDataSet);

	MatCreate(&dataSet->CompleteLabelDataSet, dataSet->CompleteSampleNum, 1);
	MatZeros(&dataSet->CompleteLabelDataSet);

	// init Training dataset
	MatCreate(&dataSet->CompleteTrainFeature, dataSet->TrainSampleNum, dataSet->SampleDimensionNum);
	MatZeros(&dataSet->CompleteTrainFeature);


	MatCreate(&dataSet->CompleteTrainLabel, dataSet->TrainSampleNum, 1);
	MatZeros(&dataSet->CompleteTrainLabel);

	// init train batch 

	//printf("%d\t%d\n", dataSet->BatchNum, dataSet->remainder);
	dataSet->BatchTrainFeature = (Mat*)malloc(dataSet->BatchNum * sizeof(Mat));
	dataSet->BatchTrainLabel = (Mat*)malloc(dataSet->BatchNum * sizeof(Mat));
	dataSet->BatchTrainLabelOneHot = (Mat*)malloc(dataSet->BatchNum * sizeof(Mat));

	for (int i = 0; i < dataSet->BatchNum; ++i) {
		if (i == dataSet->BatchNum - 1 && dataSet->remainder != 0) {
			MatCreate(&(dataSet->BatchTrainFeature)[i], dataSet->remainder, dataSet->SampleDimensionNum);
			MatZeros(&(dataSet->BatchTrainFeature)[i]);

			MatCreate(&(dataSet->BatchTrainLabel)[i], dataSet->remainder, 1);
			MatZeros(&(dataSet->BatchTrainLabel)[i]);

			MatCreate(&(dataSet->BatchTrainLabelOneHot)[i], dataSet->remainder, dataSet->ClassificationNum);
			//printf("%d\n", dataSet->ClassificationNum);
			MatZeros(&(dataSet->BatchTrainLabelOneHot)[i]);
		}
		else {
			MatCreate(&(dataSet->BatchTrainFeature)[i], dataSet->BatchSize, dataSet->SampleDimensionNum);
			MatZeros(&(dataSet->BatchTrainFeature)[i]);

			MatCreate(&(dataSet->BatchTrainLabel)[i], dataSet->BatchSize, 1);
			MatZeros(&(dataSet->BatchTrainLabel)[i]);

			MatCreate(&(dataSet->BatchTrainLabelOneHot)[i], dataSet->BatchSize, dataSet->ClassificationNum);
			MatZeros(&(dataSet->BatchTrainLabelOneHot)[i]);
		}

	}



	// init Training dataset
	MatCreate(&dataSet->TestFeature, dataSet->TestSampleNum, dataSet->SampleDimensionNum);
	MatZeros(&dataSet->TestFeature);

	MatCreate(&dataSet->TestLabel, dataSet->TestSampleNum, 1);
	MatZeros(&dataSet->TestLabel);

	MatCreate(&dataSet->TestLabelOneHot, dataSet->TestSampleNum, dataSet->ClassificationNum);
	MatZeros(&dataSet->TestLabelOneHot);
}



int DataLoading(Custom userDefine, DataSet *dataSet) {
	// complete data loading
	MatSetVal(&dataSet->CompleteFeatureDataSet, userDefine.XValArray);
	MatSetVal(&dataSet->CompleteLabelDataSet, userDefine.YValArray);

	// complete train data loading
	MatSetVal(&dataSet->CompleteTrainFeature, userDefine.XValArray);
	MatSetVal(&dataSet->CompleteTrainLabel, userDefine.YValArray);

	// init batch train data loading and onehot coding
	for (int i = 0; i < dataSet->BatchNum; ++i) {
		MatSetVal(&dataSet->BatchTrainFeature[i], &userDefine.XValArray[i*dataSet->BatchSize*dataSet->SampleDimensionNum]);
		MatSetVal(&dataSet->BatchTrainLabel[i], &userDefine.YValArray[i*dataSet->BatchSize]);
		OneHot(&dataSet->BatchTrainLabel[i], dataSet->ClassificationNum, &dataSet->BatchTrainLabelOneHot[i]);
	}


	//test data loading and onehot coding
	MatSetVal(&dataSet->TestFeature, &userDefine.XValArray[dataSet->TrainSampleNum*dataSet->SampleDimensionNum]);
	MatSetVal(&dataSet->TestLabel, &userDefine.YValArray[dataSet->TrainSampleNum]);
	OneHot(&dataSet->TestLabel, dataSet->ClassificationNum, &dataSet->TestLabelOneHot);

	return 0;
}


void DatasetConstruction(Custom userDefine, DataSet *dataSet) {
	CalculateAndLoadDataSetPara(dataSet);
	CreateDataSetSpace(dataSet);
	DataLoading(userDefine, dataSet);
}
/************************************************************************/
/*                         神经网络数据集构建                           */
/************************************************************************/














/************************************************************************/
/*                      神经网络的运算所需空间变量                      */
/************************************************************************/
//神经网络激活值矩阵			P_ActiMat			Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//神经网络激活值矩阵加偏置列	P_ActiMatPlus		Mat*		列表索引i = 0(输入层), 1, ..., N hidden
//加偏置列偏置列全部置1.输出层不需要Plus
//神经网络求和矩阵				P_SumMat			Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//神经网络权值矩阵				P_WeightMat			Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//神经网络权值偏置矩阵			P_WeightBiasMat		Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//训练数据标签					Mat_oneHot			Mat			row = N_sample col = N_out
//反向传播中间变量矩阵			P_DeltaMat			Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//权值偏置矩阵导数变量矩阵		P_NablaWbMat		Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//激活函数对求和值导数矩阵      P_ActiFunDerivation	Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义

//初始化FCNN结构体
int InitFCNN(FCNN *fcnn) {
	fcnn->sampleCapacity = -1;
	fcnn->SampleDimensionNum = -1;
	fcnn->HiddenLayerNum = -1;
	fcnn->WeightInitWayNum = -1;
	fcnn->Layer = NULL;
	fcnn->OnehotMat.element = NULL;
	fcnn->ClassificationNum = -1;
	fcnn->LossFuncNum = -1;

	return 0;
}


//初始化FCLayer结构体
int InitFCLayer(FCLayer *fclayer) {
	fclayer->ActiMat.element = NULL;
	fclayer->ActiMatPlus.element = NULL;
	fclayer->SumMat.element = NULL;
	fclayer->WeightMat.element = NULL;
	fclayer->WeightBiasMat.element = NULL;
	fclayer->DeltaMat.element = NULL;
	fclayer->NablaWbMat.element = NULL;
	fclayer->ActiFunDerivationMat.element = NULL;

	fclayer->NeuronNum = -1;
	fclayer->AcitFuncNum = -1;

	return 0;
}

//创建网络层空间
FCLayer * SpaceCreateFCLayer(FCNN *fcnn) {
	fcnn->Layer = (FCLayer*)malloc((fcnn->HiddenLayerNum + 2) * sizeof(FCLayer));
	return fcnn->Layer;
}


// malloc并初始化激活值矩阵空间
void SpaceCreateActi(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatCreate(&fcnn->Layer[i].ActiMat, fcnn->sampleCapacity, fcnn->Layer[i].NeuronNum);
		MatZeros(&fcnn->Layer[i].ActiMat);
	}
}

// free激活值矩阵空间
void SpaceDeleteActi(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatDelete(&fcnn->Layer[i].ActiMat);
	}
}

// malloc并初始化激活值Plus矩阵空间
void SpaceCreateActiPlus(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatCreate(&fcnn->Layer[i].ActiMatPlus, fcnn->sampleCapacity, fcnn->Layer[i].NeuronNum + 1);
		MatZeros(&fcnn->Layer[i].ActiMatPlus);
	}
}

// free激活值Plus矩阵空间
void SpaceDeleteActiPlus(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatDelete(&fcnn->Layer[i].ActiMatPlus);
	}
}

// malloc并初始化求和值矩阵空间
void SpaceCreateSum(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatCreate(&fcnn->Layer[i].SumMat, fcnn->sampleCapacity, fcnn->Layer[i].NeuronNum);
		MatZeros(&fcnn->Layer[i].SumMat);
	}
}

// free求和值矩阵空间
void SpaceDeleteSum(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatDelete(&fcnn->Layer[i].SumMat);
	}
}

// malloc并初始化激活函数对求和值导数矩阵
void SpaceCreateActiFunDerivation(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatCreate(&fcnn->Layer[i].ActiFunDerivationMat, fcnn->sampleCapacity, fcnn->Layer[i].NeuronNum);
		MatZeros(&fcnn->Layer[i].ActiFunDerivationMat);
	}
}

// free激活函数对求和值导数矩阵
void SpaceDeleteActiFunDerivation(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatDelete(&fcnn->Layer[i].ActiFunDerivationMat);
	}
}


// malloc并初始化反向传播中间变量空间
void SpaceCreateDelta(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatCreate(&fcnn->Layer[i].DeltaMat, fcnn->sampleCapacity, fcnn->Layer[i].NeuronNum);
		MatZeros(&fcnn->Layer[i].DeltaMat);
	}
}

// malloc并初始化反向传播中间变量空间
void SpaceDeleteDelta(FCNN *fcnn) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatDelete(&fcnn->Layer[i].DeltaMat);
	}
}

//malloc并初始化FCNN OneHotMat
void SpaceCreateFCNNOneHotMat(FCNN *fcnn) {
	MatCreate(&fcnn->OnehotMat, fcnn->sampleCapacity, fcnn->ClassificationNum);
}

//free FCNN OneHotMat
void SpaceDeleteFCNNOneHotMat(FCNN *fcnn) {
	MatDelete(&fcnn->OnehotMat);
}



// malloc并初始化神经网络权值矩阵空间
void SpaceCreateWeight(FCNN *fcnn) {

	fcnn->Layer[0].WeightMat.row = 0;
	fcnn->Layer[0].WeightMat.col = 0;
	for (int i = 1; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatCreate(&fcnn->Layer[i].WeightMat, fcnn->Layer[i - 1].NeuronNum, fcnn->Layer[i].NeuronNum);
		MatZeros(&fcnn->Layer[i].WeightMat);
	}
}


// malloc并初始化神经网络权值偏置矩阵空间
void SpaceCreateWeightBias(FCNN *fcnn) {

	fcnn->Layer[0].WeightBiasMat.row = 0;
	fcnn->Layer[0].WeightBiasMat.col = 0;
	for (int i = 1; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatCreate(&fcnn->Layer[i].WeightBiasMat, fcnn->Layer[i - 1].NeuronNum + 1, fcnn->Layer[i].NeuronNum);
		MatZeros(&fcnn->Layer[i].WeightBiasMat);
	}
}

//malloc并初始化权值偏置矩阵导数变量矩阵
void SpaceCreateNablaWeightBias(FCNN *fcnn) {

	fcnn->Layer[0].NablaWbMat.row = 0;
	fcnn->Layer[0].NablaWbMat.col = 0;
	for (int i = 1; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatCreate(&fcnn->Layer[i].NablaWbMat, fcnn->Layer[i - 1].NeuronNum + 1, fcnn->Layer[i].NeuronNum);
		MatZeros(&fcnn->Layer[i].NablaWbMat);
	}
}



void CreateNNOperationSpace(FCNN *fcnn) {
	SpaceCreateActi(fcnn);
	SpaceCreateActiPlus(fcnn);
	SpaceCreateSum(fcnn);
	SpaceCreateActiFunDerivation(fcnn);
	SpaceCreateDelta(fcnn);

	SpaceCreateFCNNOneHotMat(fcnn);
}

void DeleteNNOperationSpace(FCNN *fcnn) {
	SpaceDeleteActi(fcnn);
	SpaceDeleteActiPlus(fcnn);
	SpaceDeleteSum(fcnn);
	SpaceDeleteActiFunDerivation(fcnn);
	SpaceDeleteDelta(fcnn);

	SpaceDeleteFCNNOneHotMat(fcnn);
}



void CreateNNParaSpace(FCNN *fcnn) {
	SpaceCreateWeight(fcnn);
	SpaceCreateWeightBias(fcnn);
	SpaceCreateNablaWeightBias(fcnn);
}

void DoadinPara2FCLayer(FCNN *fcnn, Custom userDefine) {
	for (int i = 0; i < fcnn->HiddenLayerNum + 2; ++i) {
		fcnn->Layer[i].AcitFuncNum = userDefine.ActiFuncNumArray[i];
		fcnn->Layer[i].NeuronNum = userDefine.NeuronNumArray[i];
	}
}



int CreateNNSpaceAndLoadinPara2FCLayer(FCNN *fcnn, Custom userDefine) {
	fcnn->Layer = SpaceCreateFCLayer(fcnn);

	DoadinPara2FCLayer(fcnn, userDefine);

	CreateNNOperationSpace(fcnn);
	CreateNNParaSpace(fcnn);

	return 0;
}






/************************************************************************/
/*                      神经网络的运算所需空间变量                      */
/************************************************************************/

























/************************************************************************/
/*                             初始化权值                           */
/************************************************************************/
/*神经网络权值初始化*/
//输入矩阵 输出矩阵 权值矩阵

void WeightInit_ChooseWay(Mat *Weight, int Style_initWeight) {
	if (Style_initWeight == 0) {
		MatInitZero(Weight);
	}
	else if (Style_initWeight == 1) {
		MatInitRandomNormalization(Weight);
	}
	else if (Style_initWeight == 2) {
		MatInitXavier(Weight);
	}
	else if (Style_initWeight == 3) {
		MatInitHe(Weight);
	}
	else {
		printf("error for WeightInit_ChooseWay, please check Style_initWeight variable!\n");
	}

}


int NNWeightinit(FCNN *fcnn) {

	//权值何凯明初始化（后期扩展可选常用的几种初始化方式）
	for (int i = 1; i < fcnn->HiddenLayerNum + 2; ++i) {
		WeightInit_ChooseWay(&fcnn->Layer[i].WeightMat, fcnn->WeightInitWayNum);
		MatPlusRow(&fcnn->Layer[i].WeightMat, &fcnn->Layer[i].WeightBiasMat);
	}

	return 0;
}
/************************************************************************/
/*                             初始化神经网络                           */
/************************************************************************/



















/************************************************************************/
/*                           神经网络前向传播                           */
/************************************************************************/
//所需参数
//神经网络隐藏层层数:			N_hidden.
//神经网络激活值矩阵			P_ActiMat		Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//神经网络激活值矩阵加偏置列	P_ActiMatPlus	Mat*		列表索引i = 0(输入层), 1, ..., N hidden
//加偏置列偏置列全部置1.输出层不需要Plus
//神经网络求和矩阵				P_SumMat		Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//神经网络权值偏置矩阵			P_WeightBiasMat	Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//训练数据标签					Mat_oneHot		Mat			row = N_sample col = N_out


Mat* MatActivate(Mat *src, Mat *dst, int way) {
	if (way == 0) {
		MatNoneActi(src, dst);
	}
	else if (way == 1) {
		MatSigmoid(src, dst);
	}
	else if (way == 2) {
		MatTanh(src, dst);
	}
	else if (way == 3) {
		MatRelu(src, dst);
	}
	else if (way == 4) {
		MatLeakyRelu(0.2f, src, dst); //leak = 0.2
	}
	else if (way == 5) {
		MatSoftmax(src, dst);
	}
	else {
		printf("error for MatActivate, please check ActiFsHidden  variable!\n");
	}
	return NULL;
}


float LossFunction(Mat *src, Mat *dst, int Nstr_LossF) {
	if (Nstr_LossF == 0) {
		return MSE(src, dst);
	}
	else if (Nstr_LossF == 1) {
		return CrossEntropy(src, dst);
	}
	else {
		printf("error for Nstr_LossF, please check loss function variable!\n");
		return -1;
	}
}

//神经网络向前传播， 返回loss scalar
float NNforward(Mat featureMat, Mat labelMatOneHot, FCNN *fcnn) {

#ifdef MAT_LEGAL_CHECKING
	if (featureMat.row != labelMatOneHot.row) {
		printf("\t\terr check, mismatching matrix for NNforward\t\t\n");
		printf("\t\tfeatureMatShape:\n\t\t\t");
		MatShape(&featureMat);
		printf("\t\tlabelMatMatShape:\n\t\t\t");
		MatShape(&labelMatOneHot);
		return -1.f;
	}
#endif
	//MatDump(&featureMat);
	if (featureMat.row != fcnn->sampleCapacity) {// 处理小批量的余数
		fcnn->sampleCapacity = featureMat.row;

		DeleteNNOperationSpace(fcnn);
		CreateNNOperationSpace(fcnn);
	}



	MatCopy(&featureMat, &fcnn->Layer[0].ActiMat);
	MatPlusCol(&fcnn->Layer[0].ActiMat, &fcnn->Layer[0].ActiMatPlus);

	MatCopy(&labelMatOneHot, &fcnn->OnehotMat);

	// 向前传播
	for (int i = 0; i < fcnn->HiddenLayerNum + 1; ++i) {
		MatMul(&fcnn->Layer[i].ActiMatPlus, &fcnn->Layer[i + 1].WeightBiasMat, &fcnn->Layer[i + 1].SumMat);
		MatActivate(&fcnn->Layer[i + 1].SumMat, &fcnn->Layer[i + 1].ActiMat, fcnn->Layer[i + 1].AcitFuncNum);
		if (i != fcnn->HiddenLayerNum) {
			MatPlusCol(&fcnn->Layer[i + 1].ActiMat, &fcnn->Layer[i + 1].ActiMatPlus);
		}
	}
	//计算loss
	//MatDump(&OneHotMat);
	//MatDump(&fcnn->Layer[fcnn->HiddenLayerNum + 1].ActiMat);

	float loss = -1.f;
	loss = LossFunction(&fcnn->Layer[fcnn->HiddenLayerNum + 1].ActiMat, &fcnn->OnehotMat, fcnn->LossFuncNum);

	return loss;
}

//float NNforward(Mat *P_ActiMat, Mat *P_ActiMatPlus, Mat *P_SumMat, Mat *P_WeightBiasMat, Mat Mat_oneHot, int N_hidden, int *NStr_ActiFsHidden, int Nstr_LossF){
//
//	// 向前传播
//	for (int i = 0; i < N_hidden+1; ++i){
//		MatMul(&P_ActiMatPlus[i], &P_WeightBiasMat[i + 1], &P_SumMat[i + 1]);
//		MatActivate(&P_SumMat[i + 1], &P_ActiMat[i + 1], NStr_ActiFsHidden[i + 1]);
//		if (i != N_hidden){
//			MatPlusCol(&P_ActiMat[i + 1], &P_ActiMatPlus[i + 1]);
//		}
//		//MatDump(&P_ActiMat[i + 1]);
//	}
//	//计算loss
//	return LossFunction(&P_ActiMat[N_hidden + 1], &Mat_oneHot, Nstr_LossF);
//}

/************************************************************************/
/*                           神经网络前向传播                           */
/************************************************************************/






/************************************************************************/
/*                           神经网络反向传播                           */
/************************************************************************/
//所需参数
//神经网络隐藏层层数:			N_hidden.
//输入样本的数量：				N_sample.
//各层神经元个数 :				N_layerNeuron[i],			i =0(输入层),1,...,N_hidden,N_hidden+1(输出层).
//权值偏置矩阵导数变量矩阵		P_NablaWbMat	Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//神经网络求和矩阵				P_SumMat		Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//神经网络激活值矩阵			P_ActiMat		Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//神经网络激活值矩阵加偏置列	P_ActiMatPlus	Mat*		列表索引i = 0(输入层), 1, ..., N hidden
//各层激活函数：				NStr_ActiFsHidden[i], i=0,1,...,N_hidden,N_hidden+1(输出层).
//0  -> no activation
//1  -> sigmoid
//2  -> tanh
//3  -> relu
//4  -> leaky relu
//5  -> softmax (output layer)
//输出层损失函数：				Nstr_LossF 
//0  -> MSE
//1  -> CE
//反向传播中间变量矩阵			P_DeltaMat		Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//训练数据标签					Mat_oneHot		Mat			row = N_sample col = N_out
//激活函数对求和值导数矩阵      P_ActiFunDerivation	Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义


Mat * ActiFunDerivation(Mat Mat_Sum, Mat* Mat_ActiFunDerivation, int option) {
	if (option == 0) {
		return MatDerivationNoneActi(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 1) {
		return MatDerivationSigmoid(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 2) {
		return MatDerivationTanh(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 3) {
		return MatDerivationRelu(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 4) {
		return MatDerivationLeakyRelu(0.1f, &Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 5) {
		return MatDerivationSoftmax(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else {
		printf("error for ActiFunDerivation, please check ActiFsHidden  variable!\n");
	}
	return NULL;
}

Mat * LossFunDerivation(Mat *ActiMat, Mat *DerivativeActiMat, Mat One_hotMat, int option) {
	if (option == 0) {
		return MSEDerivative(ActiMat, DerivativeActiMat, One_hotMat);
	}
	else if (option == 1) {
		return CrossEntropyDerivative(ActiMat, DerivativeActiMat, One_hotMat);
	}
	else {
		printf("error for LossFunDerivation, please check Nstr_LossF  variable!\n");
	}
	return NULL;
}

Mat * NNOuputLayerBackward(FCNN *fcnn) {

	if (fcnn->Layer[fcnn->HiddenLayerNum + 1].AcitFuncNum == 5 && fcnn->LossFuncNum == 1) {//softmax+crossentropy
		MatSub(&fcnn->Layer[fcnn->HiddenLayerNum + 1].ActiMat, &fcnn->OnehotMat, &fcnn->Layer[fcnn->HiddenLayerNum + 1].DeltaMat);
	}
	else {
		Mat tempMat;
		MatCreate(&tempMat, fcnn->sampleCapacity, fcnn->Layer[fcnn->HiddenLayerNum + 1].NeuronNum);

		LossFunDerivation(&fcnn->Layer[fcnn->HiddenLayerNum + 1].ActiMat, &tempMat, fcnn->OnehotMat, fcnn->LossFuncNum);

		ActiFunDerivation(fcnn->Layer[fcnn->HiddenLayerNum + 1].SumMat, &fcnn->Layer[fcnn->HiddenLayerNum + 1].ActiFunDerivationMat, fcnn->Layer[fcnn->HiddenLayerNum + 1].AcitFuncNum);

		MatProduct(&fcnn->Layer[fcnn->HiddenLayerNum + 1].ActiMat, &fcnn->Layer[fcnn->HiddenLayerNum + 1].ActiFunDerivationMat, &fcnn->Layer[fcnn->HiddenLayerNum + 1].DeltaMat);

		MatDelete(&tempMat);
		//MatDump(&P_DeltaMat[N_hidden + 1]);
	}

	Mat ActiPlusTrans;
	MatCreate(&ActiPlusTrans, fcnn->Layer[fcnn->HiddenLayerNum].NeuronNum + 1, fcnn->sampleCapacity);
	MatTrans(&fcnn->Layer[fcnn->HiddenLayerNum].ActiMatPlus, &ActiPlusTrans);
	MatMul(&ActiPlusTrans, &fcnn->Layer[fcnn->HiddenLayerNum + 1].DeltaMat, &fcnn->Layer[fcnn->HiddenLayerNum + 1].NablaWbMat);
	//MatDump(&P_NablaWbMat[N_hidden + 1]);
	MatNumMul(1.f / fcnn->sampleCapacity, &fcnn->Layer[fcnn->HiddenLayerNum + 1].NablaWbMat, &fcnn->Layer[fcnn->HiddenLayerNum + 1].NablaWbMat);
	//MatDump(&P_NablaWbMat[N_hidden + 1]);

	MatDelete(&ActiPlusTrans);
	return NULL;

}



//Mat * NNOuputLayerBackward(int N_hidden, int N_sample, int* N_layerNeuron, int *NStr_ActiFsHidden, 
//	int Nstr_LossF, Mat* P_NablaWbMat, Mat* P_ActiMatPlus, Mat* P_SumMat, Mat* P_DeltaMat, Mat* P_ActiMat, Mat* P_ActiFunDerivation, Mat Mat_oneHot){
//	
//	if (NStr_ActiFsHidden[N_hidden+1] == 5 && Nstr_LossF == 1){//softmax+crossentropy
//		MatSub(&P_ActiMat[N_hidden+1], &Mat_oneHot, &P_DeltaMat[N_hidden + 1]);
//		//MatDump(&P_ActiMat[N_hidden + 1]);
//		//MatDump(&Mat_oneHot);
//		//MatDump(&P_DeltaMat[N_hidden + 1]);
//	}
//	else{
//		Mat tempMat;
//		MatCreate(&tempMat, N_sample, N_layerNeuron[N_hidden + 1]);
//
//		LossFunDerivation(&P_ActiMat[N_hidden + 1], &tempMat, Mat_oneHot, Nstr_LossF);
//
//		ActiFunDerivation(P_SumMat[N_hidden + 1], &P_ActiFunDerivation[N_hidden+1], NStr_ActiFsHidden[N_hidden + 1]);
//
//		MatProduct(&P_ActiMat[N_hidden + 1], &P_ActiFunDerivation[N_hidden + 1], &P_DeltaMat[N_hidden + 1]);
//		
//		MatDelete(&tempMat);
//		//MatDump(&P_DeltaMat[N_hidden + 1]);
//	}
//
//	Mat ActiPlusTrans;
//	MatCreate(&ActiPlusTrans, N_layerNeuron[N_hidden]+1, N_sample);
//	MatTrans(&P_ActiMatPlus[N_hidden], &ActiPlusTrans);
//	MatMul(&ActiPlusTrans, &P_DeltaMat[N_hidden + 1], &P_NablaWbMat[N_hidden + 1]);
//	//MatDump(&P_NablaWbMat[N_hidden + 1]);
//	MatNumMul(1.f / P_SumMat[1].row, &P_NablaWbMat[N_hidden + 1], &P_NablaWbMat[N_hidden + 1]);
//	//MatDump(&P_NablaWbMat[N_hidden + 1]);
//
//	return NULL;
//
//}


Mat * NNBackward(FCNN *fcnn) {

	//printf("NN Start to backward......\n");

	//输出层反向传播
	NNOuputLayerBackward(fcnn);

	//MatDump(&fcnn->Layer[fcnn->HiddenLayerNum+1].NablaWbMat);
	//隐藏层反向传播
	for (int i = fcnn->HiddenLayerNum; i > 0; --i) {
		Mat tempTransW;
		Mat ActiFuncMat;
		Mat tempMulMat;
		Mat tempProdMat;
		Mat tempTransActi;

		MatCreate(&tempTransW, fcnn->Layer[i + 1].WeightMat.col, fcnn->Layer[i + 1].WeightMat.row);
		MatCreate(&ActiFuncMat, fcnn->Layer[i].SumMat.row, fcnn->Layer[i].SumMat.col);
		MatCreate(&tempMulMat, fcnn->Layer[i + 1].DeltaMat.row, fcnn->Layer[i + 1].WeightMat.row);
		MatCreate(&tempProdMat, fcnn->Layer[i].SumMat.row, fcnn->Layer[i].SumMat.col);
		MatCreate(&tempTransActi, fcnn->Layer[i - 1].ActiMatPlus.col, fcnn->Layer[i - 1].ActiMatPlus.row);

		MatTrans(&fcnn->Layer[i + 1].WeightMat, &tempTransW);
		ActiFunDerivation(fcnn->Layer[i].SumMat, &ActiFuncMat, fcnn->Layer[i].AcitFuncNum);

		MatMul(&fcnn->Layer[i + 1].DeltaMat, &tempTransW, &tempMulMat);
		MatProduct(&tempMulMat, &ActiFuncMat, &fcnn->Layer[i].DeltaMat);

		//MatDump(&fcnn->Layer[i].NablaWbMat);
		MatTrans(&fcnn->Layer[i - 1].ActiMatPlus, &tempTransActi);
		MatMul(&tempTransActi, &fcnn->Layer[i].DeltaMat, &fcnn->Layer[i].NablaWbMat);
		/*MatDump(&fcnn->Layer[i].NablaWbMat);*/
		MatNumMul(1.f / fcnn->sampleCapacity, &fcnn->Layer[i].NablaWbMat, &fcnn->Layer[i].NablaWbMat);
		//MatDump(&fcnn->Layer[i].NablaWbMat);

		MatDelete(&tempTransW);
		MatDelete(&ActiFuncMat);
		MatDelete(&tempMulMat);
		MatDelete(&tempProdMat);
		MatDelete(&tempTransActi);
		//break;
	}
	return NULL;
}

//Mat * NNBackward(int N_hidden, int N_sample, int* N_layerNeuron, int *NStr_ActiFsHidden, int Nstr_LossF, Mat* P_NablaWbMat, Mat* P_SumMat, 
//	Mat* P_DeltaMat, Mat* P_ActiFunDerivation, Mat* P_ActiMat, Mat* P_ActiMatPlus, Mat Mat_oneHot, Mat* P_WeightMat){
//
//	//printf("NN Start to backward......\n");
//
//	//输出层反向传播
//	NNOuputLayerBackward(N_hidden, N_sample, N_layerNeuron, NStr_ActiFsHidden, Nstr_LossF, P_NablaWbMat, P_ActiMatPlus, P_SumMat, P_DeltaMat, 
//P_ActiMat, P_ActiFunDerivation, Mat_oneHot);
//
//	//MatDump(&P_NablaWbMat[N_hidden + 1]);
//	//隐藏层反向传播
//	for (int i = N_hidden; i > 0; --i){
//		Mat tempTransW;
//		Mat ActiFuncMat;
//		Mat tempMulMat;
//		Mat tempProdMat;
//		Mat tempTransActi;
//
//		MatCreate(&tempTransW, P_WeightMat[i+1].col, P_WeightMat[i+1].row);
//		MatCreate(&ActiFuncMat, P_SumMat[i].row, P_SumMat[i].col);
//		MatCreate(&tempMulMat, P_DeltaMat[i + 1].row, P_WeightMat[i+1].row);
//		MatCreate(&tempProdMat, P_SumMat[i].row, P_SumMat[i].col);
//		MatCreate(&tempTransActi, P_ActiMatPlus[i - 1].col, P_ActiMatPlus[i - 1].row);
//
//		MatTrans(&P_WeightMat[i+1], &tempTransW);
//		ActiFunDerivation(P_SumMat[i], &ActiFuncMat, i);
//
//		MatMul(&P_DeltaMat[i + 1], &tempTransW, &tempMulMat);
//		MatProduct(&tempMulMat, &ActiFuncMat, &P_DeltaMat[i]);
//
//		MatTrans(&P_ActiMatPlus[i - 1], &tempTransActi);
//		MatMul(&tempTransActi, &P_DeltaMat[i], &P_NablaWbMat[i]);
//		//MatDump(&P_NablaWbMat[i]);
//		MatNumMul(1.f / N_sample, &P_NablaWbMat[i], &P_NablaWbMat[i]);
//		//MatDump(&P_NablaWbMat[i]);
//
//		MatDelete(&tempTransW);
//		MatDelete(&ActiFuncMat);
//		MatDelete(&tempMulMat);
//		MatDelete(&tempProdMat);
//		MatDelete(&tempTransActi);
//		//break;
//	}
//	return NULL;
//}

/************************************************************************/
/*                           神经网络反向传播                           */
/************************************************************************/


















/************************************************************************/
/*                           神经网络优化算法                           */
/************************************************************************/
//所需参数
//神经网络隐藏层层数:			N_hidden.
//权值偏置矩阵导数变量矩阵		P_NablaWbMat	Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义
//神经网络权值偏置矩阵			P_WeightBiasMat	Mat*		列表索引i = 0(输入层), 1, ..., N hidden，N hidden + 1(输出层)
//i = 0 时输入层求和矩阵行列置零无实际含义


//批量梯度下降
void  MBGD(FCNN *fcnn, float alpha) {
	Mat temp;
	for (int i = 1; i <= fcnn->HiddenLayerNum + 1; ++i) {
		//MatDump(&P_WeightBiasMat[i]);
		MatCreate(&temp, fcnn->Layer[i].NablaWbMat.row, fcnn->Layer[i].NablaWbMat.col);
		MatNumMul(alpha, &fcnn->Layer[i].NablaWbMat, &temp);
		MatSub(&fcnn->Layer[i].WeightBiasMat, &temp, &fcnn->Layer[i].WeightBiasMat);
		MatDelete(&temp);
	}
}


// Adam优化器
typedef struct {
	float beta1;
	float beta2;
	float eta;
	float epsilon;

	int time;

	Mat *v;
	Mat *hat_v;
	Mat *s;
	Mat *hat_s;
	Mat *hat_g;
}AdamPara;



void SpaceCreateAdamPara(FCNN *fcnn, AdamPara *adamPara) {

	adamPara->v = (Mat *)malloc((fcnn->HiddenLayerNum + 2) * sizeof(Mat));
	adamPara->hat_v = (Mat *)malloc((fcnn->HiddenLayerNum + 2) * sizeof(Mat));
	adamPara->s = (Mat *)malloc((fcnn->HiddenLayerNum + 2) * sizeof(Mat));
	adamPara->hat_s = (Mat *)malloc((fcnn->HiddenLayerNum + 2) * sizeof(Mat));
	adamPara->hat_g = (Mat *)malloc((fcnn->HiddenLayerNum + 2) * sizeof(Mat));



	adamPara->v[0].row = 0;
	adamPara->v[0].col = 0;
	adamPara->hat_v[0].row = 0;
	adamPara->hat_v[0].col = 0;
	adamPara->s[0].row = 0;
	adamPara->s[0].col = 0;
	adamPara->hat_s[0].row = 0;
	adamPara->hat_s[0].col = 0;
	adamPara->hat_g[0].row = 0;
	adamPara->hat_g[0].col = 0;

	for (int i = 1; i < fcnn->HiddenLayerNum + 2; ++i) {
		MatCreate(&adamPara->v[i], fcnn->Layer[i - 1].NeuronNum + 1, fcnn->Layer[i].NeuronNum);
		MatZeros(&adamPara->v[i]);
		MatCreate(&adamPara->hat_v[i], fcnn->Layer[i - 1].NeuronNum + 1, fcnn->Layer[i].NeuronNum);
		MatZeros(&adamPara->hat_v[i]);
		MatCreate(&adamPara->s[i], fcnn->Layer[i - 1].NeuronNum + 1, fcnn->Layer[i].NeuronNum);
		MatZeros(&adamPara->s[i]);
		MatCreate(&adamPara->hat_s[i], fcnn->Layer[i - 1].NeuronNum + 1, fcnn->Layer[i].NeuronNum);
		MatZeros(&adamPara->hat_s[i]);
		MatCreate(&adamPara->hat_g[i], fcnn->Layer[i - 1].NeuronNum + 1, fcnn->Layer[i].NeuronNum);
		MatZeros(&adamPara->hat_g[i]);
	}
}

void initAdam(FCNN fcnn, AdamPara *adamPara) {
	adamPara->beta1 = 0.9;
	adamPara->beta2 = 0.999;
	adamPara->eta = 0.0001;
	adamPara->epsilon = 0.00000008;
	SpaceCreateAdamPara(&fcnn, adamPara);

	adamPara->time = 1;
}

// Adam
void Adam(FCNN *fcnn, AdamPara *adamPara){
	//Mat temp;
	for (int i = 1; i <= fcnn->HiddenLayerNum + 1; ++i) {
		// formula (1)
		MatNumMul(adamPara->beta1, &adamPara->v[i], &adamPara->v[i]);
		MatNumMul(1-adamPara->beta1, &fcnn->Layer[i].NablaWbMat, &adamPara->hat_g[i]);
		MatAdd(&adamPara->v[i], &adamPara->hat_g[i], &adamPara->v[i]);


		// formula (2)
		MatNumMul(adamPara->beta2, &adamPara->s[i], &adamPara->s[i]);
		MatSquare(&adamPara->hat_g[i], &adamPara->hat_g[i]);
		MatNumMul(1 - adamPara->beta2, &adamPara->hat_g[i], &adamPara->hat_g[i]);
		MatAdd(&adamPara->s[i], &adamPara->hat_g[i], &adamPara->s[i]);

		// formula (3)(4)
		MatNumMul((1 / (1 - pow(adamPara->beta1, adamPara->time))), &adamPara->v[i], &adamPara->hat_v[i]);
		MatNumMul((1 / (1 - pow(adamPara->beta2, adamPara->time))), &adamPara->s[i], &adamPara->hat_s[i]);

		// formula (5)
		MatNumMul(adamPara->eta, &adamPara->hat_v[i], &adamPara->hat_v[i]);
		MatSqrt(&adamPara->hat_s[i], &adamPara->hat_s[i]);
		MatNumAdd(adamPara->epsilon, &adamPara->hat_s[i], &adamPara->hat_s[i]);
		MatDiv(&adamPara->hat_v[i], &adamPara->hat_s[i], &adamPara->hat_g[i]);

		MatSub(&fcnn->Layer[i].WeightBiasMat, &adamPara->hat_g[i], &fcnn->Layer[i].WeightBiasMat);
	}

	adamPara->time = adamPara->time + 1;
}
/************************************************************************/
/*                          神经网络优化算法                            */
/************************************************************************/







/************************************************************************/
/*                             准确率测试                               */
/************************************************************************/
int judge_max(float arr[], int n)
{
	int index = 0;
	int i;
	float max = arr[0];
	for (i = 0; i < n; i++)
	{	
		if (arr[i] > max)
		{
			max = arr[i];
			index = i;
		}
	}
	return index;
}


float testAcc(FCNN fcnn, DataSet dataset) {
	//MatDump(&fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat);
	Mat MatCompare;
	char buf[20];
	int i = 0;
	MatCreate(&MatCompare, dataset.TestLabel.row, 1);
	MatZeros(&MatCompare);

	for (i = 0; i < dataset.TestLabel.row; ++i) {
		(MatCompare.element[i])[0] = (float)judge_max(fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat.element[i], fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat.col);
		//DumpFloatArray(fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat.element[i], fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat.col);
		//printf("%d\n", judge_max(fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat.element[i], fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat.col));
		//printf("%s\n", F2S((MatCompare.element[i])[0], buf));
	}

	//MatShape(&MatCompare);
	//MatShape(&dataset.TestLabel);
	int correctNum=0;
	for (i = 0; i < dataset.TestLabel.row; ++i) {
		if (equal((MatCompare.element[i])[0], (dataset.TestLabel.element[i])[0])) {
			correctNum++;
		}
	}
	/*printf("%d\n", correctNum);*/

	return (float)correctNum/(float)dataset.TestLabel.row;
}

/************************************************************************/
/*                             准确率测试                               */
/************************************************************************/






/************************************************************************/
/*                           MinstHW数据加载                            */
/************************************************************************/
#define MAX_LINE 16


//struct _iobuf {
//	char *_ptr;
//	int   _cnt;
//	char *_base;
//	int   _flag;
//	int   _file;
//	int   _charbuf;
//	int   _bufsiz;
//	char *_tmpfname;
//};
//typedef struct _iobuf FILE;


float Xval[54880000] = { 0 };
float Yval[70000] = { 0 };

void MinstHWDataLoading() {
	char buf[MAX_LINE];  /*缓冲区*/
	FILE *fp = NULL;     /*文件指针*/
	FILE *fp2 = NULL;
	int len;             /*行字符个数*/
	int i = 0;
	int j = 0;
	char * flag;

	//if ((fp = fopen("C:/Users/Administrator/Documents/GitHub/MinstHW/DataFeatrue.txt", "r")) == NULL)
	//{
	//	perror("fail to read");
	//	return 0;
	//}

	fp = fopen("../../DataSet/MinstHandWriting/Transformation/MinstHW-70000/DataFeatrue.msd", "r");
	flag = fgets(buf, MAX_LINE, fp);
	while (flag != NULL)
	{
		len = strlen(buf);
		buf[len - 1] = '\0';  /*去掉换行符*/
		//printf("%s %d \n", buf, len - 1);
		Xval[i++] = (float)atof(buf);
		flag = fgets(buf, MAX_LINE, fp);
	}
	fclose(fp);

	//for (i = 0; i < 54880000; ++i) {
	//	printf("%d:  %s\n", i, F2S(nums[i], buf));
	//}



	// labels
	fp2 = fopen("../../DataSet/MinstHandWriting/Transformation/MinstHW-70000/DataLabel.msd", "r");
	flag = fgets(buf, MAX_LINE, fp2);
	while (flag != NULL)
	{
		len = strlen(buf);
		buf[len - 1] = '\0';  /*去掉换行符*/
		//printf("%s %d \n", buf, len - 1);
		Yval[j++] = (float)atof(buf);
		flag = fgets(buf, MAX_LINE, fp2);
	}
	fclose(fp2);
}

/************************************************************************/
/*                           MinstHW数据加载                            */
/************************************************************************/





//int main() {
//
//
//	/*数据集导入*/
//	char buf3[20];
//	char buf4[20];
//	MinstHWDataLoading();
//	//for (int i = 0; i < 70000; ++i) {
//	//	printf("%d, %s, %s\n", i, F2S(Xval[i], buf3), F2S(Yval[i],buf4));
//	//}
//
//
//	int NueronNumArray[] = { 784, 512, 256 ,10 };
//	int ActiFuncNumArray[] = { 0, 3, 3, 5 };// 各层激活函数使用真值；注意映射关系。 最后一层表示输出层激活函数SoftMax
//	char buf1[40];
//	char buf2[40];
//
//
//
//	/*用户自定义参数输入*/
//	Custom userDefine;
//	DataSet dataSet;
//	FCNN fcnn;
//	FCLayer fclayer;
//
//	InitCustom(&userDefine);   // custom initial
//	InitDataSet(&dataSet);
//	InitFCNN(&fcnn);
//	InitFCLayer(&fclayer);
//
//	userDefine.CompleteSampleNum = 70000;
//	userDefine.TrainSampleNum = 60000;
//	userDefine.TestSampleNum = 10000;
//	//userDefine.ValidationNum = 0;
//	userDefine.SampleDimensionNum = 784;
//	userDefine.HiddenLayerNum = 2;
//	userDefine.ClassificationNum = 10;
//	userDefine.LossFuncNum = 1; // CE
//	userDefine.WeightInitWayNum = 3; // KaiMing
//	userDefine.BatchSize = 200;
//	userDefine.XValArray = Xval;
//	userDefine.YValArray = Yval;
//	userDefine.NeuronNumArray = NueronNumArray;
//	userDefine.ActiFuncNumArray = ActiFuncNumArray;
//
//	DumpCustom(userDefine); // print
//
//							// 从Custon传入参数到各个结构体
//	LoadParaFromCustom(userDefine, &dataSet, &fcnn);
//
//
//	DatasetConstruction(userDefine, &dataSet);
//
//	//MatDump(&dataSet.CompleteFeatureDataSet);
//	//MatDump(&dataSet.CompleteLabelDataSet);
//
//	//MatDump(&dataSet.CompleteTrainFeature);
//	//MatDump(&dataSet.CompleteTrainLabel);
//
//	//MatDump(&dataSet.TestFeature);
//	//MatDump(&dataSet.TestLabel);
//
//
//	//MatDump(&dataSet.CompleteFeatureDataSet);
//	//MatDump(&dataSet.CompleteLabelDataSet);
//
//	//MatDump(&dataSet.CompleteTrainFeature);
//	//MatDump(&dataSet.CompleteTrainLabel);
//
//	//for (int i = 0; i < batchNum; ++i){
//	//	MatDump(&(dataSet.BatchTrainFeature)[i]);
//	//	MatDump(&(dataSet.BatchTrainLabel)[i]);
//	//}
//	//MatDump(&dataSet.TestFeature);
//	//MatDump(&dataSet.TestLabel);
//
//
//	//printf("Change the platform succesfully!\n");
//
//
//
//
//	CreateNNSpaceAndLoadinPara2FCLayer(&fcnn, userDefine);
//	////printf("%d\n", fcnn.Layer[4].NeuronNum);
//
//
//	NNWeightinit(&fcnn);
//
//
//
//
//	//MatDump(&dataSet.TestFeature);
//	//MatDump(&dataSet.TestLabelOneHot);
//
//	
//
//	/*训练*/
//	float loss = 0.f;
//	float losstest = 0.f;
//	int trainOperationNum = 6;
//
//	for (int i = 0; i < trainOperationNum; ++i) {
//		for (int j = 0; j < dataSet.BatchNum; ++j) {
//
//			/*神经网络前项传播*/
//
//			loss = NNforward(dataSet.BatchTrainFeature[j], dataSet.BatchTrainLabelOneHot[j], &fcnn);
//
//			NNBackward(&fcnn);
//			MBGD(&fcnn, 0.1f);
//
//			if ((j+1) % 200 == 0) {
//				printf("epoch %d/%d iteration %d/%d loss=%s\n", i + 1, trainOperationNum, j + 1, dataSet.BatchNum, F2S(loss, buf1));
//			}	
//			//break;
//		}
//		//test
//		losstest = NNforward(dataSet.TestFeature, dataSet.TestLabelOneHot, &fcnn);
//		printf("====== epoch %d/%d  testloss=%s  acc=%s ======\n", i + 1, trainOperationNum, F2S(losstest, buf2), F2S(testAcc(fcnn, dataSet), buf1));
//		//break;
//	}
//
//	//MatDump(&fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat);
//	//losstest = NNforward(dataSet.TestFeature, dataSet.TestLabelOneHot, &fcnn);
//	//MatDump(&fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat);
//
//}


int main() {
	clock_t start, finish; // 定义变量
	double time;
	start = clock();
	/*数据集导入*/
	char buf3[20];
	char buf4[20];
	MinstHWDataLoading();
	//for (int i = 0; i < 70000; ++i) {
	//	printf("%d, %s, %s\n", i, F2S(Xval[i], buf3), F2S(Yval[i],buf4));
	//}


	int NueronNumArray[] = { 784, 512, 256 ,10 };
	int ActiFuncNumArray[] = { 0, 3, 3, 5 };// 各层激活函数使用真值；注意映射关系。 最后一层表示输出层激活函数SoftMax
	char buf1[40];
	char buf2[40];



	/*用户自定义参数输入*/
	Custom userDefine;
	DataSet dataSet;
	FCNN fcnn;
	FCLayer fclayer;

	InitCustom(&userDefine);   // custom initial
	InitDataSet(&dataSet);
	InitFCNN(&fcnn);
	InitFCLayer(&fclayer);

	userDefine.CompleteSampleNum = 70000;
	userDefine.TrainSampleNum = 60000;
	userDefine.TestSampleNum = 10000;
	userDefine.SampleDimensionNum = 784;
	userDefine.HiddenLayerNum = 2;
	userDefine.ClassificationNum = 10;
	userDefine.LossFuncNum = 1; // CE
	userDefine.WeightInitWayNum = 3; // KaiMing
	userDefine.BatchSize = 200;
	userDefine.XValArray = Xval;
	userDefine.YValArray = Yval;
	userDefine.NeuronNumArray = NueronNumArray;
	userDefine.ActiFuncNumArray = ActiFuncNumArray;

	DumpCustom(userDefine); // print

							// 从Custon传入参数到各个结构体
	LoadParaFromCustom(userDefine, &dataSet, &fcnn);


	DatasetConstruction(userDefine, &dataSet);





	CreateNNSpaceAndLoadinPara2FCLayer(&fcnn, userDefine);
	////printf("%d\n", fcnn.Layer[4].NeuronNum);


	NNWeightinit(&fcnn);



	AdamPara adamPara;
	initAdam(fcnn, &adamPara);
	//MatShape(&adamPara.hat_g[1]);


	/*训练*/
	float loss = 0.f;
	float losstest = 0.f;
	int trainOperationNum = 1;

	for (int i = 0; i < trainOperationNum; ++i) {
		for (int j = 0; j < dataSet.BatchNum; ++j) {

			/*神经网络前项传播*/

			loss = NNforward(dataSet.BatchTrainFeature[j], dataSet.BatchTrainLabelOneHot[j], &fcnn);

			NNBackward(&fcnn);
			Adam(&fcnn, &adamPara);

			if ((j+1) % 30 == 0) {
				printf("epoch %d/%d iteration %d/%d loss=%s\n", i + 1, trainOperationNum, j + 1, dataSet.BatchNum, F2S(loss, buf1));
			}	
			//break;
		}
		//test
		losstest = NNforward(dataSet.TestFeature, dataSet.TestLabelOneHot, &fcnn);
		printf("====== epoch %d/%d  testloss=%s  acc=%s ======\n", i + 1, trainOperationNum, F2S(losstest, buf2), F2S(testAcc(fcnn, dataSet), buf1));
		//break;
	}

	/*MatDump(&fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat);
	losstest = NNforward(dataSet.TestFeature, dataSet.TestLabelOneHot, &fcnn);
	MatDump(&fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat);*/

	finish = clock();
	time = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("time :%f seconds\n", time);


// 普通 446s
// 稀疏单判断 177s
// 稀疏两判断 1511s
}













// demo
//int main() {
//
//
//	/*用户自定义相关数据录入*/
//	float Xval[] = {
//		0.f, 0.f, 0.f, 0.f, 0.f,
//		0.f, 0.f, 0.f, 0.f, 1.f,
//		0.f, 0.f, 0.f, 1.f, 0.f,
//		0.f, 0.f, 0.f, 1.f, 1.f,
//		0.f, 0.f, 1.f, 0.f, 0.f,
//		0.f, 0.f, 1.f, 0.f, 1.f,
//		0.f, 0.f, 1.f, 1.f, 0.f,
//		0.f, 0.f, 1.f, 1.f, 1.f,
//		0.f, 1.f, 0.f, 0.f, 0.f,
//		0.f, 1.f, 0.f, 0.f, 1.f,
//		0.f, 1.f, 0.f, 1.f, 0.f,
//		0.f, 1.f, 0.f, 1.f, 1.f,
//		0.f, 1.f, 1.f, 0.f, 0.f,
//		0.f, 1.f, 1.f, 0.f, 1.f,
//		0.f, 1.f, 1.f, 1.f, 0.f,
//		0.f, 1.f, 1.f, 1.f, 1.f,
//		1.f, 0.f, 0.f, 0.f, 0.f,
//		1.f, 0.f, 0.f, 0.f, 1.f,
//		1.f, 0.f, 0.f, 1.f, 0.f,
//		1.f, 0.f, 0.f, 1.f, 1.f,
//		1.f, 0.f, 1.f, 0.f, 0.f,
//		1.f, 0.f, 1.f, 0.f, 1.f,
//		1.f, 0.f, 1.f, 1.f, 0.f,
//		1.f, 0.f, 1.f, 1.f, 1.f,
//		1.f, 1.f, 0.f, 0.f, 0.f,
//		1.f, 1.f, 0.f, 0.f, 1.f,
//		1.f, 1.f, 0.f, 1.f, 0.f,
//		1.f, 1.f, 0.f, 1.f, 1.f,
//		1.f, 1.f, 1.f, 0.f, 0.f,
//		1.f, 1.f, 1.f, 0.f, 1.f,
//		1.f, 1.f, 1.f, 1.f, 0.f,
//		1.f, 1.f, 1.f, 1.f, 1.f }; //样本真值
//
//
//	float Yval[] = { 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f };  //样本标签二分类
//	int NueronNumArray[] = { 5, 6, 8, 6 ,2 };
//	int ActiFuncNumArray[] = { 0, 3, 3, 3, 5 };// 各层激活函数使用真值；注意映射关系。 最后一层表示输出层激活函数SoftMax
//	char buf1[40];
//	char buf2[40];
//
//
//
//	/*用户自定义参数输入*/
//	Custom userDefine;
//	DataSet dataSet;
//	FCNN fcnn;
//	FCLayer fclayer;
//
//	InitCustom(&userDefine);   // custom initial
//	InitDataSet(&dataSet);
//	InitFCNN(&fcnn);
//	InitFCLayer(&fclayer);
//
//	userDefine.CompleteSampleNum = 32;
//	userDefine.TrainSampleNum = 24;
//	userDefine.TestSampleNum = 7;
//	//userDefine.ValidationNum = 0;
//	userDefine.SampleDimensionNum = 5;
//	userDefine.HiddenLayerNum = 3;
//	userDefine.ClassificationNum = 2;
//	userDefine.LossFuncNum = 1; // CE
//	userDefine.WeightInitWayNum = 3; // KaiMing
//	userDefine.BatchSize = 5;
//	userDefine.XValArray = Xval;
//	userDefine.YValArray = Yval;
//	userDefine.NeuronNumArray = NueronNumArray;
//	userDefine.ActiFuncNumArray = ActiFuncNumArray;
//
//	DumpCustom(userDefine); // print
//
//							// 从Custon传入参数到各个结构体
//	LoadParaFromCustom(userDefine, &dataSet, &fcnn);
//
//
//	DatasetConstruction(userDefine, &dataSet);
//
//	//MatDump(&dataSet.CompleteFeatureDataSet);
//	//MatDump(&dataSet.CompleteLabelDataSet);
//
//	//MatDump(&dataSet.CompleteTrainFeature);
//	//MatDump(&dataSet.CompleteTrainLabel);
//
//	//MatDump(&dataSet.TestFeature);
//	//MatDump(&dataSet.TestLabel);
//
//
//	//MatDump(&dataSet.CompleteFeatureDataSet);
//	//MatDump(&dataSet.CompleteLabelDataSet);
//
//	//MatDump(&dataSet.CompleteTrainFeature);
//	//MatDump(&dataSet.CompleteTrainLabel);
//
//	//for (int i = 0; i < batchNum; ++i){
//	//	MatDump(&(dataSet.BatchTrainFeature)[i]);
//	//	MatDump(&(dataSet.BatchTrainLabel)[i]);
//	//}
//	//MatDump(&dataSet.TestFeature);
//	//MatDump(&dataSet.TestLabel);
//
//
//	//printf("Change the platform succesfully!\n");
//
//
//
//
//	CreateNNSpaceAndLoadinPara2FCLayer(&fcnn, userDefine);
//	////printf("%d\n", fcnn.Layer[4].NeuronNum);
//
//
//	NNWeightinit(&fcnn);
//
//
//
//
//	MatDump(&dataSet.TestFeature);
//	MatDump(&dataSet.TestLabelOneHot);
//
//
//	/*训练*/
//	float loss = 0.f;
//	float losstest = 0.f;
//	int trainOperationNum = 100000;
//	for (int i = 0; i < trainOperationNum; ++i) {
//		for (int j = 0; j < dataSet.BatchNum; ++j) {
//
//			/*神经网络前项传播*/
//			if (i % 10 == 0) {
//				losstest = NNforward(dataSet.TestFeature, dataSet.TestLabelOneHot, &fcnn);
//			}
//
//			loss = NNforward(dataSet.BatchTrainFeature[j], dataSet.BatchTrainLabelOneHot[j], &fcnn);
//
//			NNBackward(&fcnn);
//			MBGD(&fcnn, 0.1f);
//
//			if (i % 10 == 0) {
//				printf("epoch %d/%d iteration %d/%d loss=%s  testloss=%s\n", i + 1, trainOperationNum, j + 1, dataSet.BatchNum, F2S(loss, buf1), F2S(losstest, buf2));
//			}
//			else {
//				printf("epoch %d/%d iteration %d/%d loss=%s\n", i + 1, trainOperationNum, j + 1, dataSet.BatchNum, F2S(loss, buf1));
//
//			}
//
//
//		}
//	}
//
//	MatDump(&fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat);
//	losstest = NNforward(dataSet.TestFeature, dataSet.TestLabelOneHot, &fcnn);
//	MatDump(&fcnn.Layer[fcnn.HiddenLayerNum + 1].ActiMat);
//
//}





//int main(){
//
//
//	/*用户自定义参数输入*/
//	int N_sample = 16; //样本数量
//	int D_sample = 4;  //样本维度
//	int N_out = 2;   //二分类
//
//	float Xval[] = {
//		0.f, 0.f, 0.f, 0.f,
//		0.f, 0.f, 0.f, 1.f,
//		0.f, 0.f, 1.f, 0.f,
//		0.f, 0.f, 1.f, 1.f,
//		0.f, 1.f, 0.f, 0.f,
//		0.f, 1.f, 0.f, 1.f,
//		0.f, 1.f, 1.f, 0.f,
//		0.f, 1.f, 1.f, 1.f,
//		1.f, 0.f, 0.f, 0.f,
//		1.f, 0.f, 0.f, 1.f,
//		1.f, 0.f, 1.f, 0.f,
//		1.f, 0.f, 1.f, 1.f,
//		1.f, 1.f, 0.f, 0.f,
//		1.f, 1.f, 0.f, 1.f,
//		1.f, 1.f, 1.f, 0.f,
//		1.f, 1.f, 1.f, 1.f }; //样本真值
//
//	//float Xval[] = {
//	//	0.4f, 0.4f, 0.4f, 0.4f,
//	//	0.4f, 0.4f, 0.4f, 0.6f,
//	//	0.4f, 0.4f, 0.6f, 0.4f,
//	//	0.4f, 0.4f, 0.6f, 0.6f,
//	//	0.4f, 0.6f, 0.4f, 0.4f,
//	//	0.4f, 0.6f, 0.4f, 0.6f,
//	//	0.4f, 0.6f, 0.6f, 0.4f,
//	//	0.4f, 0.6f, 0.6f, 0.6f,
//	//	0.6f, 0.4f, 0.4f, 0.4f,
//	//	0.6f, 0.4f, 0.4f, 0.6f,
//	//	0.6f, 0.4f, 0.6f, 0.4f,
//	//	0.6f, 0.4f, 0.6f, 0.6f,
//	//	0.6f, 0.6f, 0.4f, 0.4f,
//	//	0.6f, 0.6f, 0.4f, 0.6f,
//	//	0.6f, 0.6f, 0.6f, 0.4f,
//	//	0.6f, 0.6f, 0.6f, 0.6f }; //样本真值
//
//	float Yval[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };  //样本标签二分类
//
//	int N_hidden = 3;//神经网络隐藏层层数
//
//	int *N_layerNeuron = NULL; // 各层神经元个数  0(输入层),1,...,N_hidden,N_hidden+1(输出层).
//	int Nval[] = { 4, 5, 6, 5, 2 };// 各隐藏层神经元个数真值
//
//	N_layerNeuron = intVal2List(N_hidden + 2, Nval, N_layerNeuron);
//	////测试传入正确性
//	//for (int i = 0; i < N_hidden + 2; ++i){
//	//	printf("%d\n", N_layerNeuron[i]);
//	//}
//
//	int *Nstr_ActiFsHidden = NULL;// 各层激活函数使用；
//	int Aval[] = { 0, 3, 3, 3, 5 };// 各层激活函数使用真值；注意映射关系。
//
//	Nstr_ActiFsHidden = intVal2List(N_hidden + 2, Aval, Nstr_ActiFsHidden);
//
//	////测试传入正确性
//	//for (int i = 0; i < N_hidden + 2; ++i){
//	//	printf("%d\n", NStr_ActiFsHidden[i]);
//	//}
//
//	int Nstr_LossF = 1;//use CrossEntropy Loss function
//
//	int Style_initWeight = 3;   //kaiming initialization
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	/*构建神经网络所需空间并初始化所需参数*/
//	//神经网络所需空间变量
//	Mat* P_ActiMat=NULL;			//神经网络激活值矩阵
//	Mat* P_ActiMatPlus = NULL;		//神经网络激活值矩阵加偏置列	
//	Mat* P_SumMat = NULL;			//神经网络求和矩阵				
//	Mat* P_WeightMat = NULL;		//神经网络权值矩阵		
//	Mat* P_WeightBiasMat = NULL;	//神经网络权值偏置矩阵	
//	Mat Mat_oneHot;					//训练数据标签	
//	MatCreate(&Mat_oneHot, N_sample, N_out);
//	Mat Mat_Y;
//	MatCreate(&Mat_Y, N_sample, 1);
//	Mat* P_DeltaMat = NULL;			//反向传播中间变量矩阵	
//	Mat* P_NablaWbMat = NULL;		//反向传播权值偏置导数矩阵
//	Mat* P_ActiFunDerivation = NULL; //激活函数对求和值求导数矩阵
//
//	//用户输入参数
//	//样本数量			N_sample			int
//	//隐藏层层数		N_hidden			int
//	//各层神经元个数	N_layerNeuron		int*
//	//各层激活函数		NStr_ActiFsHidden	int*
//
//
//	P_ActiMat = SpaceCreateActi(P_ActiMat, N_sample, N_hidden, N_layerNeuron);
//
//	//MatDump(&(P_ActiMat[0]));
//
//	P_ActiMatPlus = SpaceCreateActiPlus(P_ActiMatPlus, N_sample, N_hidden, N_layerNeuron);
//
//	//MatDump(&(P_ActiMatPlus[0]));
//
//	P_SumMat = SpaceCreateSum(P_SumMat, N_sample, N_hidden, N_layerNeuron);
//
//	//MatDump(&P_SumMat[0]);    //P_SumMat[0] 无意义
//
//	P_WeightMat = SpaceCreateWeight(P_WeightMat, N_hidden, N_layerNeuron);
//
//	//MatDump(&P_WeightMat[1]);
//
//	P_WeightBiasMat = SpaceCreateWeightBias(P_WeightBiasMat, N_hidden, N_layerNeuron);
//
//	//MatDump(&P_WeightBiasMat[1]);
//
//	P_DeltaMat = SpaceCreateDelta(P_DeltaMat, N_sample, N_hidden, N_layerNeuron);
//
//	//MatDump(&(P_DeltaMat[1]));
//
//
//	P_NablaWbMat = SpaceCreateNablaWeightBias(P_NablaWbMat, N_hidden, N_layerNeuron);
//
//	P_ActiFunDerivation = SpaceCreateActiFunDerivation(P_ActiFunDerivation, N_sample, N_hidden, N_layerNeuron);
//	//printf("Nabla\n");
//	//MatDump(&(P_NablaWbMat[1]));
//
//
//
//
//
//
//
//
//	/*初始化神经网络参数*/
//	//输入矩阵 输出矩阵 权值权值矩阵
//	NNinit(P_ActiMat, P_ActiMatPlus, &Mat_Y, &Mat_oneHot, P_WeightMat, P_WeightBiasMat, N_out, N_hidden, Xval, Yval, Style_initWeight);
//
//	//MatDump(&P_ActiMat[0]);
//	//MatDump(&P_ActiMatPlus[0]);
//	//MatDump(&Mat_Y);
//	//MatDump(&Mat_oneHot);
//	//MatDump(&P_WeightMat[1]);
//	//MatDump(&P_WeightBiasMat[1]);
//
//
//
//
//	
//	for (int i = 0; i <= 100000; ++i){
//		float loss = 0.f;
//		/*神经网络前项传播*/
//		loss = NNforward(P_ActiMat, P_ActiMatPlus, P_SumMat, P_WeightBiasMat, Mat_oneHot, N_hidden, Nstr_ActiFsHidden, Nstr_LossF);
//		if (i == 0){
//			MatDump(&P_ActiMat[N_hidden + 1]);
//			MatDump(&Mat_oneHot);
//		}
//		if (i % 2000 == 0){
//			printf("第%d次训练：%f\n", i,loss);
//			//MatDump(&P_ActiMat[N_hidden + 1]);
//			//MatDump(&Mat_oneHot);
//			//printf("%f\n", CrossEntropy(&P_ActiMat[N_hidden + 1], &Mat_oneHot));
//			/*for (int i = 0; i < N_hidden + 2; ++i){
//				printf("Weight Bias:\n");
//				MatDump(&P_WeightBiasMat[i]);*/
//				//printf("Weight Bias Nabla:\n");
//				//MatDump(&P_NablaWbMat[i]);
//			//}
//		}
//		
//		//printf("%d\n", isinf(loss));
//
//		NNBackward(N_hidden, N_sample, N_layerNeuron, Nstr_ActiFsHidden, Nstr_LossF, P_NablaWbMat, P_SumMat, P_DeltaMat, P_ActiFunDerivation, P_ActiMat, P_ActiMatPlus, Mat_oneHot, P_WeightMat);
//
//
//
//		BGD(P_WeightBiasMat, P_NablaWbMat, N_hidden, 0.1f);
//
//	}
//	MatDump(&P_ActiMat[N_hidden + 1]);
//	MatDump(&Mat_oneHot);
//	return 0;
//}


















/************************************************************************/
/*                        二维矩阵测试主函数                            */
/************************************************************************/
//int main(void)
//{
//	Mat a;
//	Mat a1;
//	float val[] = { 3, 2, 2, 3, 3, 1, 3, 3, 4, 4, 3, 3, 2, 3, 3, 4 ,3, 4, 3, 3 };
//	Mat b;
//	float val2[] = { 1, 2, 3, 1, 1, 2, 4, 1, 1, 2, 4, 3, 1, 4, 4, 1, 1, 2, 4, 3, 1, 4, 4, 1,  4, 3, 1, 4, 4, 1 };
//	Mat c;
//
//
//	MatCreate(&a, 4, 5);
//	printf("a= ");
//	MatDump(MatSetVal(&a, val));
//	MatCreate(&b, 5, 6);
//	printf("b= ");
//	MatDump(MatSetVal(&b, val2));
//	MatCreate(&c, 4, 6);
//	MatZeros(&c);
//
//	printf("矩阵 c = bxa:\n");
//	MatDump(MatMul(&a, &b, &c));
//
//}
//int main(void)
//{
//	Mat a;
//	Mat a1;
//	float val[] = {
//		1.45, 3.92, 0.983,
//		5.4, 7.665, 0.876,
//	};
//	Mat b;
//	Mat b1;
//	float val2[] = {
//		3, 6,
//		8, 1,
//		9, 2
//	};
//	Mat c;
//	Mat d;
//	Mat e;
//	float val3[] = {
//		3, 2, -3,
//		10, -3, 2,
//		-3, 5, 9,
//	};
//
//	MatCreate(&a, 2, 3);
//	MatCreate(&a1, 2, 3);
//	printf("a= ");
//	MatDump(MatSetVal(&a, val));
//	MatCreate(&b, 3, 2);
//	MatCreate(&b1, 3, 2);
//	printf("b= ");
//	MatDump(MatSetVal(&b, val2));
//	MatCreate(&c, 3, 3);
//	MatCreate(&d, 3, 3);
//	printf("d= ");
//	MatDump(MatSetVal(&d, val3));
//
//	//printf("矩阵 a1 = a+a:\n");
//	//MatDump(MatAdd(&a, &a, &a1));
//	//printf("矩阵 b1 = b-b:\n");
//	//MatDump(MatSub(&b, &b, &b1));
//	printf("矩阵 c = bxa:\n");
//	MatDump(MatMul(&b, &a, &c));
//
//	printf("矩阵 c = bxa:\n");
//	MatDump(MatMul2(&b, &a, &c));
//
//	//printf("矩阵 b = a转置:\n");
//	//MatDump(MatTrans(&a, &b));
//
//	//printf("矩阵复制 a1 = a.\n");
//	//MatCopy(&a, &a1);
//	//MatDump(&a1);
//
//
//	/*Mat x;
//	Mat y;
//	Mat z;
//	MatCreate(&x, 2, 3);
//	MatCreate(&y, 2, 4);
//	MatCreate(&z, 2, 2);
//	MatDump(MatSetVal(&x, val2));
//
//	printf("MatExp a1 = Matexp(a)\n");
//	MatDump(MatExp(&a, &a1));
//
//
//	printf("a1 = 2.0*a\n");
//	MatDump(MatNumMul(2.0, &a, &a1));
//
//	printf("a1 = 3.0+a\n");
//	MatDump(MatNumAdd(3.0, &a, &a1));
//
//
//	printf("MatOnes, MatZeros, MatEye\n");
//	MatDump(MatOnes(&x));
//	MatDump(MatZeros(&x));
//	MatDump(MatEye(&x));
//
//	Mat f;
//	float valf[] = {7, 8, 9};
//	MatCreate(&f, 3, 1);
//	MatSetVal(&f, valf);
//	printf("MatVectorSub \n");
//	MatDump(&d);
//	MatDump(MatVectorSub(&d, &f, &d));
//	MatDump(&d);*/
//
//
//	return 0;
//}






/************************************************************************/
/*                        激活函数测试主函数                            */
/************************************************************************/
//int main() {
//	//float z = 1.6;
//
//	//printf("%f\n", sigmoid(z));
//	//printf("%f\n", tanh(z));
//	//printf("%f\n", relu(z));
//	//printf("%f\n", leakyRelu(z,0.1));
//
//
//	Mat a;
//	Mat b;
//	Mat act;
//	float val[] = {
//		-0.2f, 1.3f, 0.f,
//		-1.5f, 2.6f, 3.3f,
//		0.5f, -2.5f, 6.f
//	};
//	MatCreate(&b, 3, 1);
//	MatCreate(&a, 3, 3);
//	MatCreate(&act, 3, 3);
//	MatSetVal(&a, val);
//
//	printf("Softmax 激活\n");
//	printf("原矩阵：\n");
//	MatDump(&a);
//	printf("激活后的矩阵：\n");
//	MatDump(MatSoftmax(&a, &act));
//	printf("行求和矩阵：\n");
//	MatDump(MatRowSum(&act, &b));
//	printf("求导后的矩阵：\n");
//	MatDump(MatDerivationSoftmax(&a, &act));
//
//	printf("===================================================\n");
//	printf("Sigmoid 激活\n");
//	printf("原矩阵：\n");
//	MatDump(&a);
//	printf("激活后的矩阵：\n");
//	MatDump(MatSigmoid(&a, &act));
//	printf("求导后的矩阵：\n");
//	MatDump(MatDerivationSigmoid(&a, &act));
//
//	printf("===================================================\n");
//	printf("Tanh 激活\n");
//	printf("原矩阵：\n");
//	MatDump(&a);
//	printf("激活后的矩阵：\n");
//	MatDump(MatTanh(&a, &act));
//	printf("求导后的矩阵：\n");
//	MatDump(MatDerivationTanh(&a, &act));
//
//	printf("===================================================\n");
//	printf("Relu 激活\n");
//	printf("原矩阵：\n");
//	MatDump(&a);
//	printf("激活后的矩阵：\n");
//	MatDump(MatRelu(&a, &act));
//	printf("求导后的矩阵：\n");
//	MatDump(MatDerivationRelu(&a, &act));
//
//	printf("===================================================\n");
//	printf("LeakyRelu 激活\n");
//	printf("原矩阵：\n");
//	MatDump(&a);
//	printf("激活后的矩阵：\n");
//	MatDump(MatLeakyRelu(0.2f, &a, &act));
//	printf("求导后的矩阵：\n");
//	MatDump(MatDerivationLeakyRelu(0.2f, &a, &act));
//
//	return 0;
//}








/************************************************************************/
/*                       损失函数测试主函数                             */
/************************************************************************/
//int main()
//{
//	Mat Y;
//	Mat Yonehot;
//	Mat Ytrans;
//	Mat Sum;
//	Mat ActiMatDerivative;
//	MatCreate(&ActiMatDerivative, 5, 4);
//
//	float val[5] = { 0, 3, 1, 2, 3};
//
//	MatCreate(&Y, 5, 1);
//	MatCreate(&Ytrans, 1, 5);
//	MatCreate(&Yonehot, 5, 4);
//	MatSetVal(&Y, val);
//	MatCreate(&Sum, 1, 1);
//
//	OneHot(&Y, 4, &Yonehot);
//
//	MatDump(&Yonehot);
//
//	//MatRowSum(&Yonehot, &Y);
//	//MatTrans(&Y, &Ytrans);
//
//	//MatRowSum(&Ytrans, &Sum);
//
//	//MatDump(&Sum);
//
//
//
//
//
//	Mat prediction;
//	MatCreate(&prediction, 5, 4);
//	float prop[20] = { 0.0f, 1.7f, 0.0f, 0.0f, 1.0f, 1.6f, 0.8f, 0.1f, 0.6f, 0.7f, 0.9f, 0.6f, 0.1f, 0.9f, 1.2f, 1.3f ,0.8f, 6.f, 1.f, 0.7f};
//	
//	MatSetVal(&prediction, prop);
//	MatDump(&prediction);
//
//	MatSoftmax(&prediction, &prediction);
//	MatDump(&prediction);
//
//	float MSEloss = 0.f;
//	MSEloss = MSE(&prediction, &Yonehot);
//
//	printf("MSE loss = %f\n", MSEloss);
//
//	float CEloss = 0.f;
//	CEloss = CrossEntropy(&prediction, &Yonehot);
//
//	printf("CE loss = %f\n", CEloss);
//
//	printf("MSEDerivative \n");
//	MatDump(MSEDerivative(&prediction, &ActiMatDerivative, Yonehot));
//
//	printf("CrossEntropyDerivative \n");
//	MatDump(CrossEntropyDerivative(&prediction, &ActiMatDerivative, Yonehot));
//
//
//	return 0;
//}




















/************************************************************************/
/*                       权值初始化测试主函数                           */
/************************************************************************/
//int main()
//{
//	////test Init zero
//	//Mat m;
//	//MatCreate(&m, 10, 4);
//	//MatDump(&m);
//	//MatInitZero(&m);
//	//MatDump(&m);
//
//	////test gaussrand
//	//float mean = 0.f;
//	//float stdc = 0.1f;
//	//float data = 0;
//	//int num = 100;
//	//float total = 0.;
//	//srand((unsigned int)time(NULL));
//
//	//for (int i = 0; i<num; ++i){
//	//	data = gaussrand(mean, stdc);
//	//	total += data;
//	//	printf("%f\n", data);
//	//}
//
//	//printf("real mean =  %f\n", total/num);
//
//	Mat weight;
//	MatCreate(&weight, 16, 10);
//
//	MatInitRandomNormalization(&weight);
//	printf("Random Initial:\n");
//	MatDump(&weight);
//
//	printf("Xavier Initial:\n");
//	MatInitXavier(&weight);
//	MatDump(&weight);
//
//
//	printf("He Initial:\n");
//	MatInitHe(&weight);
//	MatDump(&weight);
//	return 0;
//}