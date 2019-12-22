#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAT_LEGAL_CHECKING
//#undef MAT_LEGAL_CHECKING

typedef struct {
	int row, col;
	float** element;
}Mat;



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
			if (data[0] == '-'){
				printf(" %s\t", F2S((mat->element[row])[col], str));
			}
			else{
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


/* dst = src1 x src2 */
Mat* MatMul(Mat* src1, Mat* src2, Mat* dst)
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
			temp = 0.0f;
			for (i = 0; i < src1->col; i++) {
				temp += (src1->element[row])[i] * (src2->element[i])[col];
			}
			(dst->element[row])[col] = temp;
		}
	}

	return dst;
}


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


/* dst = num * src 矩阵数乘运算 */
Mat* MatNumMul(float num,Mat* src, Mat* dst)
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


/*全0阵*/
Mat* MatZeros(Mat* mat)
{
	int row, col;

	for (row = 0; row < mat->row; row++){
		for (col = 0; col < mat->col; col++){
			(mat->element[row])[col] = 0.0f;
		}
	}

	return mat;
}


/*全1阵*/
Mat* MatOnes(Mat* mat)
{
	int row, col;

	for (row = 0; row < mat->row; row++){
		for (col = 0; col < mat->col; col++){
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
	for (i = 0; i < min(float(mat->row), float(mat->col)); i++){
		(mat->element[i])[i] = 1.0f;
	}

	return mat;
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

	for (row = 0; row < src->row; row++){
		temp = 0;
		for (col = 0; col < src->col; col++){
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

	for (row = 0; row < src->row; row++){
		temp = (src->element[row])[0];
		for (col = 1; col < src->col; col++){
			if ((src->element[row])[col] > temp){
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

	for (row = 0; row < src->row; row++){
		for (col = 0; col < src->col; col++){
			(dst->element[row])[col] = (src->element[row])[col] * (src->element[row])[col];
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

	for (row = 0; row < src->row; row++){
		for (col = 0; col < src->col; col++){
			(dst->element[row])[col] = (exp(float((src->element[row])[col])));
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


	for (row = 0; row < src->row; row++){
		for (col = 0; col < src->col; col++){
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

	for (row = 0; row < src->row; row++){
		if (absolute((vector->element[row])[0]) < 0.000001){
			printf("err check, Divisor vector has zero.\n");
			return NULL;
		}
	}
#endif


	for (row = 0; row < src->row; row++){
		for (col = 0; col < src->col; col++){
			(dst->element[row])[col] = (src->element[row])[col]/(vector->element[row])[0];
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
	for (row = 0; row < src->row; row++){
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col + 1] = (src->element[row])[col];
	}
}



/* dst = src^+    row+1*/
void MatPlusRow(Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row+1 != dst->row || (src->col) != dst->col) {
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
	for (row = 0; row < src->row; row++){
		for (col = 0; col < src->col; col++)
			(dst->element[row+1])[col] = (src->element[row])[col];
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
	return 1 / (1 + exp(-z));
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
			(dst->element[row])[col] = tanh((src->element[row])[col]);
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
		for (col = 0; col < src->col; col++){
			for (i = 0; i < src->col; i++){
				if (i == col){
					(dst->element[row])[col] += (src->element[row])[i] * (1 - (src->element[row])[col]);
				}
				else{
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
		for (col = 0; col < src->col; col++){
			if ((src->element[row])[col]>0){
				(dst->element[row])[col] = 1.f;
			}
			else{
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
		for (col = 0; col < src->col; col++){
			if ((src->element[row])[col]>0){
				(dst->element[row])[col] = 1.f;
			}
			else{
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
	float loss=0.f;
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

	for (row = 0; row < src->row; ++row){
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
			loss += -1*(dst->element[row])[col]*log((src->element[row])[col]);
	}

	loss = loss / (src->row);
	
	return loss;
}


//对激活值求导数
Mat * MSEDerivative(Mat *ActiMat, Mat *DerivativeActiMat, Mat One_hotMat){

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


Mat * CrossEntropyDerivative(Mat *ActiMat, Mat *DerivativeActiMat, Mat One_hotMat){
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
			if (equal((ActiMat->element[row])[col], 0.f)==1){
				(DerivativeActiMat->element[row])[col] = - (One_hotMat.element[row])[col] * 10000000000;
			}
			else{
				(DerivativeActiMat->element[row])[col] = - (One_hotMat.element[row])[col] / (ActiMat->element[row])[col];
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
	float V1=0., V2=0., S=0.;
	int phase = 0;
	int count = 0;
	float X;


	if (phase == 0) {
		while (count == 0 || (S >= 1 || S == 0)){
			float U1 = (float)(rand() % 10000) / 10000.f;
			float U2 = (float)(rand() % 10000) / 10000.f;


			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
			count += 1;
		} ;

		float temp_S_1 = log(S);
		X = V1 * sqrt(-2 * temp_S_1 / S);
	}
	else
	{
		float temp_S_2 = log(S);
		X = V2 * sqrt(-2 * temp_S_2 / S);
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
	for (row = 0; row < src->row; ++row){
		for (col = 0; col < src->col; ++col){
			(src->element[row])[col] = gaussrand(0.f, 0.1f);  // mean stdc
		}
	}
	//set bias row 0
	//for (col = 0; col < src->col; ++col){
	//	(src->element[0])[col] = 0.f;
	//}
	return src;
}


Mat* MatInitXavier(Mat *src)
{
	srand((unsigned int)time(NULL));  // set randon seed

	int row, col;
	//weight
	for (row = 0; row < src->row; ++row){
		for (col = 0; col < src->col; ++col){
			(src->element[row])[col] = gaussrand(0.f, 0.1f) * sqrt(1.f/src->row);  // mean stdc
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
	for (row = 0; row < src->row; ++row){
		for (col = 0; col < src->col; ++col){
			(src->element[row])[col] = gaussrand(0.f, 0.9f) * sqrt(2.f / src->row);  // mean stdc
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
//用户输入参数列表
//输入样本的数量：			N_sample.
//单一样本的维度：			D_sample.
//样本真值：				Xval = [], length = N_sample * D_sample.
//样本标签：				Yval = [], length = N_sample.
//权值初始化方式			Style_initWeight. 
						  //0  -> 全0初始化
						  //1  -> 随机初始化
						  //2  -> Xavier初始化
						  //3  -> 何凯明初始化


//神经网络隐藏层层数:		N_hidden.
//各层神经元个数 :			N_layerNeuron[i], i =0(输入层),1,...,N_hidden,N_hidden+1(输出层).
//各层激活函数：			NStr_ActiFsHidden[i], i=0,1,...,N_hidden,N_hidden+1(输出层).
								//0  -> no activation
								//1  -> sigmoid
								//2  -> tanh
								//3  -> relu
								//4  -> leaky relu
								//5  -> softmax (output layer)

//输出层维度（分类类别数）：N_out
//输出层损失函数：			Nstr_LossF 
								//0  -> MSE
								//1  -> CE

//伪填充
int ParaPassedIn(void){
	return 0;
}

//建立长度为 length 的整形数组，并传入值。
int* intVal2List(int length, int *src, int* dst){
	dst = (int*)malloc(length*sizeof(int));
	for (int i = 0; i < length; ++i){
		dst[i] = src[i];
	}
	return dst;
}
/************************************************************************/
/*                     用户输入参数列表及传入函数                       */
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


// malloc并初始化激活值矩阵空间
Mat* SpaceCreateActi(Mat* P_ActiMat, int N_sample, int N_hidden, int* N_layerNeuron){
	P_ActiMat = (Mat*)malloc((N_hidden + 2)*sizeof(Mat));

	for (int i = 0; i < N_hidden + 2; ++i){
		(P_ActiMat[i]).row = N_sample;
		(P_ActiMat[i]).col = N_layerNeuron[i];
		MatCreate(&(P_ActiMat[i]), N_sample, N_layerNeuron[i]);
		MatInitZero(&(P_ActiMat[i]));
		//printf("%d %d\n", (P_ActiMat[i]).row, (P_ActiMat[i]).col);
		//MatDump(&(P_ActiMat[i]));
	}

	return P_ActiMat;
}


// malloc并初始化激活值Plus矩阵空间
Mat* SpaceCreateActiPlus(Mat* P_ActiMatPlus, int N_sample, int N_hidden, int* N_layerNeuron){
	P_ActiMatPlus = (Mat*)malloc((N_hidden + 1)*sizeof(Mat)); //输出层不需要Plus

	for (int i = 0; i < N_hidden + 1; ++i){
		(P_ActiMatPlus[i]).row = N_sample;
		(P_ActiMatPlus[i]).col = N_layerNeuron[i] + 1;
		MatCreate(&(P_ActiMatPlus[i]), N_sample, N_layerNeuron[i] + 1);
		MatInitZero(&(P_ActiMatPlus[i]));
		//printf("%d %d\n", (P_ActiMatPlus[i]).row, (P_ActiMatPlus[i]).col);
		//MatDump(&(P_ActiMatPlus[i]));
	}

	return P_ActiMatPlus;
}


// malloc并初始化求和值矩阵空间
Mat* SpaceCreateSum(Mat* P_SumMat, int N_sample, int N_hidden, int* N_layerNeuron){
	P_SumMat = (Mat*)malloc((N_hidden + 2)*sizeof(Mat));

	(P_SumMat[0]).row = 0; //输入层求和矩阵无意义
	(P_SumMat[0]).col = 0;
	for (int i = 1; i < N_hidden + 2; ++i){
		(P_SumMat[i]).row = N_sample;
		(P_SumMat[i]).col = N_layerNeuron[i];
		MatCreate(&(P_SumMat[i]), N_sample, N_layerNeuron[i]);
		MatInitZero(&(P_SumMat[i]));
		//printf("%d %d\n", (P_SumMat[i]).row, (P_SumMat[i]).col);
		//MatDump(&(P_SumMat[i]));
	}
	return P_SumMat;
}


// malloc并初始化激活函数对求和值导数矩阵
Mat* SpaceCreateActiFunDerivation(Mat* P_ActiFunDerivation, int N_sample, int N_hidden, int* N_layerNeuron){
	P_ActiFunDerivation = (Mat*)malloc((N_hidden + 2)*sizeof(Mat));

	(P_ActiFunDerivation[0]).row = 0; //输入层求和矩阵无意义
	(P_ActiFunDerivation[0]).col = 0;
	for (int i = 1; i < N_hidden + 2; ++i){
		(P_ActiFunDerivation[i]).row = N_sample;
		(P_ActiFunDerivation[i]).col = N_layerNeuron[i];
		MatCreate(&(P_ActiFunDerivation[i]), N_sample, N_layerNeuron[i]);
		MatInitZero(&(P_ActiFunDerivation[i]));
		//printf("%d %d\n", (P_ActiFunDerivation[i]).row, (P_ActiFunDerivation[i]).col);
		//MatDump(&(P_ActiFunDerivation[i]));
	}
	return P_ActiFunDerivation;
}


// malloc并初始化神经网络权值矩阵空间
Mat* SpaceCreateWeight(Mat* P_WeightMat, int N_hidden, int* N_layerNeuron){
	P_WeightMat = (Mat*)malloc((N_hidden + 2)*sizeof(Mat));

	(P_WeightMat[0]).row = 0; //输入层权值矩阵无意义
	(P_WeightMat[0]).col = 0; 
	for (int i = 1; i < N_hidden + 2; ++i){
		(P_WeightMat[i]).row = N_layerNeuron[i - 1];
		(P_WeightMat[i]).col = N_layerNeuron[i];
		MatCreate(&(P_WeightMat[i]), N_layerNeuron[i - 1], N_layerNeuron[i]);
		MatInitZero(&(P_WeightMat[i]));
		//printf("%d %d\n", (P_ActiMat[i]).row, (P_ActiMat[i]).col);
		//MatDump(&(P_ActiMat[i]));
	}

	return P_WeightMat;
}



// malloc并初始化神经网络权值偏置矩阵空间
Mat* SpaceCreateWeightBias(Mat* P_WeightBiasMat, int N_hidden, int* N_layerNeuron){
	P_WeightBiasMat = (Mat*)malloc((N_hidden + 2)*sizeof(Mat));

	(P_WeightBiasMat[0]).row = 0; //输入层权值矩阵无意义
	(P_WeightBiasMat[0]).col = 0;
	for (int i = 1; i < N_hidden + 2; ++i){
		(P_WeightBiasMat[i]).row = N_layerNeuron[i - 1]+1;
		(P_WeightBiasMat[i]).col = N_layerNeuron[i];
		MatCreate(&(P_WeightBiasMat[i]), N_layerNeuron[i - 1]+1, N_layerNeuron[i]);
		MatInitZero(&(P_WeightBiasMat[i]));
		//printf("%d %d\n", (P_WeightBiasMat[i]).row, (P_WeightBiasMat[i]).col);
		//MatDump(&(P_WeightBiasMat[i]));
	}

	return P_WeightBiasMat;
}



// malloc并初始化反向传播中间变量空间
Mat* SpaceCreateDelta(Mat* P_DeltaMat, int N_sample, int N_hidden, int* N_layerNeuron){
	P_DeltaMat = (Mat*)malloc((N_hidden + 2)*sizeof(Mat));

	(P_DeltaMat[0]).row = 0; //输入层权值矩阵无意义
	(P_DeltaMat[0]).col = 0;
	for (int i = 1; i < N_hidden + 2; ++i){
		(P_DeltaMat[i]).row = N_sample;
		(P_DeltaMat[i]).col = N_layerNeuron[i];
		MatCreate(&(P_DeltaMat[i]), N_sample, N_layerNeuron[i]);
		MatInitZero(&(P_DeltaMat[i]));
		//printf("%d %d\n", (P_DeltaMat[i]).row, (P_DeltaMat[i]).col);
		//MatDump(&(P_DeltaMat[i]));
	}

	return P_DeltaMat;
}



//malloc并初始化权值偏置矩阵导数变量矩阵
Mat* SpaceCreateNablaWeightBias(Mat* P_NablaWbMat, int N_hidden, int* N_layerNeuron){
	P_NablaWbMat = (Mat*)malloc((N_hidden + 2)*sizeof(Mat));

	(P_NablaWbMat[0]).row = 0; //输入层权值矩阵无意义
	(P_NablaWbMat[0]).col = 0;
	for (int i = 1; i < N_hidden + 2; ++i){
		(P_NablaWbMat[i]).row = N_layerNeuron[i - 1] + 1;
		(P_NablaWbMat[i]).col = N_layerNeuron[i];
		MatCreate(&(P_NablaWbMat[i]), N_layerNeuron[i - 1] + 1, N_layerNeuron[i]);
		MatInitZero(&(P_NablaWbMat[i]));
		//printf("%d %d\n", (P_WeightBiasMat[i]).row, (P_WeightBiasMat[i]).col);
		//MatDump(&(P_WeightBiasMat[i]));
	}

	return P_NablaWbMat;
}
/************************************************************************/
/*                      神经网络的运算所需空间变量                      */
/************************************************************************/

























/************************************************************************/
/*                             初始化神经网络                           */
/************************************************************************/
/*初始化神经网络参数*/
//输入矩阵 输出矩阵 权值权值矩阵

void WeightInit_ChooseWay(Mat *Weight, int Style_initWeight){
	if (Style_initWeight == 0){
		MatInitZero(Weight);
	}
	else if (Style_initWeight == 1){
		MatInitRandomNormalization(Weight);
	}
	else if (Style_initWeight == 2){
		MatInitXavier(Weight);
	}
	else if (Style_initWeight == 3){
		MatInitHe(Weight);
	}
	else{
		printf("error for WeightInit_ChooseWay, please check Style_initWeight variable!\n");
	}

}


int NNinit(Mat *P_ActiMat, Mat *P_ActiMatPlus, Mat *Mat_Y, Mat * Mat_oneHot, Mat *P_WeightMat, Mat *P_WeightBiasMat, int N_out, int N_hidden, float *Xval, float *Yval, int Style_initWeight){

	MatSetVal(&P_ActiMat[0], Xval);
	MatPlusCol(&P_ActiMat[0], &P_ActiMatPlus[0]);
	MatSetVal(Mat_Y, Yval);
	OneHot(Mat_Y, N_out, Mat_oneHot);



	//权值何凯明初始化（后期扩展可选常用的几种初始化方式）
	for (int i = 1; i < N_hidden + 2; ++i){
		WeightInit_ChooseWay(&P_WeightMat[i], Style_initWeight);
		MatPlusRow(&P_WeightMat[i], &P_WeightBiasMat[i]);
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


Mat* *MatActivate(Mat *src, Mat *dst, int way){
	if (way == 0){
		MatNoneActi(src, dst);
	}
	else if (way == 1){
		MatSigmoid(src, dst);
	}
	else if (way == 2){
		MatTanh(src, dst);
	}
	else if (way == 3){
		MatRelu(src, dst);
	}
	else if (way == 4){
		MatLeakyRelu(0.2f, src, dst); //leak = 0.2
	}
	else if (way == 5){
		MatSoftmax(src, dst);
	}
	else{
		printf("error for MatActivate, please check ActiFsHidden  variable!\n");
	}
	return NULL;
}


float LossFunction(Mat *src, Mat *dst, int Nstr_LossF){
	if (Nstr_LossF == 0){
		return MSE(src, dst);
	}
	else if (Nstr_LossF == 1){
		return CrossEntropy(src, dst);
	}
	else{
		printf("error for Nstr_LossF, please check loss function variable!\n");
		return -1;
	}
}

//神经网络向前传播， 返回loss scalar
float NNforward(Mat *P_ActiMat, Mat *P_ActiMatPlus, Mat *P_SumMat, Mat *P_WeightBiasMat, Mat Mat_oneHot, int N_hidden, int *NStr_ActiFsHidden, int Nstr_LossF){

	// 向前传播
	for (int i = 0; i < N_hidden+1; ++i){
		MatMul(&P_ActiMatPlus[i], &P_WeightBiasMat[i + 1], &P_SumMat[i + 1]);
		MatActivate(&P_SumMat[i + 1], &P_ActiMat[i + 1], NStr_ActiFsHidden[i + 1]);
		if (i != N_hidden){
			MatPlusCol(&P_ActiMat[i + 1], &P_ActiMatPlus[i + 1]);
		}
		//MatDump(&P_ActiMat[i + 1]);
	}
	//计算loss
	return LossFunction(&P_ActiMat[N_hidden + 1], &Mat_oneHot, Nstr_LossF);
}

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


Mat * ActiFunDerivation(Mat Mat_Sum, Mat* Mat_ActiFunDerivation, int option){
	if (option == 0){
		return MatDerivationNoneActi(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 1){
		return MatDerivationSigmoid(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 2){
		return MatDerivationTanh(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 3){
		return MatDerivationRelu(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 4){
		return MatDerivationLeakyRelu(0.1f, &Mat_Sum, Mat_ActiFunDerivation);
	}
	else if (option == 5){
		return MatDerivationSoftmax(&Mat_Sum, Mat_ActiFunDerivation);
	}
	else{
		printf("error for ActiFunDerivation, please check ActiFsHidden  variable!\n");
	}
	return NULL;
}

Mat * LossFunDerivation(Mat *ActiMat, Mat *DerivativeActiMat, Mat One_hotMat, int option){
	if (option == 0){
		return MSEDerivative(ActiMat, DerivativeActiMat, One_hotMat);
	}
	else if (option == 1){
		return CrossEntropyDerivative(ActiMat, DerivativeActiMat, One_hotMat);
	}
	else{
		printf("error for LossFunDerivation, please check Nstr_LossF  variable!\n");
	}
	return NULL;
}

Mat * NNOuputLayerBackward(int N_hidden, int N_sample, int* N_layerNeuron, int *NStr_ActiFsHidden, 
	int Nstr_LossF, Mat* P_NablaWbMat, Mat* P_ActiMatPlus, Mat* P_SumMat, Mat* P_DeltaMat, Mat* P_ActiMat, Mat* P_ActiFunDerivation, Mat Mat_oneHot){
	
	if (NStr_ActiFsHidden[N_hidden+1] == 5 && Nstr_LossF == 1){//softmax+crossentropy
		MatSub(&P_ActiMat[N_hidden+1], &Mat_oneHot, &P_DeltaMat[N_hidden + 1]);
		//MatDump(&P_ActiMat[N_hidden + 1]);
		//MatDump(&Mat_oneHot);
		//MatDump(&P_DeltaMat[N_hidden + 1]);
	}
	else{
		Mat tempMat;
		MatCreate(&tempMat, N_sample, N_layerNeuron[N_hidden + 1]);

		LossFunDerivation(&P_ActiMat[N_hidden + 1], &tempMat, Mat_oneHot, Nstr_LossF);

		ActiFunDerivation(P_SumMat[N_hidden + 1], &P_ActiFunDerivation[N_hidden+1], NStr_ActiFsHidden[N_hidden + 1]);

		MatProduct(&P_ActiMat[N_hidden + 1], &P_ActiFunDerivation[N_hidden + 1], &P_DeltaMat[N_hidden + 1]);
		
		MatDelete(&tempMat);
		//MatDump(&P_DeltaMat[N_hidden + 1]);
	}

	Mat ActiPlusTrans;
	MatCreate(&ActiPlusTrans, N_layerNeuron[N_hidden]+1, N_sample);
	MatTrans(&P_ActiMatPlus[N_hidden], &ActiPlusTrans);
	MatMul(&ActiPlusTrans, &P_DeltaMat[N_hidden + 1], &P_NablaWbMat[N_hidden + 1]);
	//MatDump(&P_NablaWbMat[N_hidden + 1]);
	MatNumMul(1.f / P_SumMat[1].row, &P_NablaWbMat[N_hidden + 1], &P_NablaWbMat[N_hidden + 1]);
	//MatDump(&P_NablaWbMat[N_hidden + 1]);

	return NULL;

}

Mat * NNBackward(int N_hidden, int N_sample, int* N_layerNeuron, int *NStr_ActiFsHidden, int Nstr_LossF, Mat* P_NablaWbMat, Mat* P_SumMat, 
	Mat* P_DeltaMat, Mat* P_ActiFunDerivation, Mat* P_ActiMat, Mat* P_ActiMatPlus, Mat Mat_oneHot, Mat* P_WeightMat){

	//printf("NN Start to backward......\n");

	//输出层反向传播
	NNOuputLayerBackward(N_hidden, N_sample, N_layerNeuron, NStr_ActiFsHidden, Nstr_LossF, P_NablaWbMat, P_ActiMatPlus, P_SumMat, P_DeltaMat, 
P_ActiMat, P_ActiFunDerivation, Mat_oneHot);

	//MatDump(&P_NablaWbMat[N_hidden + 1]);
	//隐藏层反向传播
	for (int i = N_hidden; i > 0; --i){
		Mat tempTransW;
		Mat ActiFuncMat;
		Mat tempMulMat;
		Mat tempProdMat;
		Mat tempTransActi;

		MatCreate(&tempTransW, P_WeightMat[i+1].col, P_WeightMat[i+1].row);
		MatCreate(&ActiFuncMat, P_SumMat[i].row, P_SumMat[i].col);
		MatCreate(&tempMulMat, P_DeltaMat[i + 1].row, P_WeightMat[i+1].row);
		MatCreate(&tempProdMat, P_SumMat[i].row, P_SumMat[i].col);
		MatCreate(&tempTransActi, P_ActiMatPlus[i - 1].col, P_ActiMatPlus[i - 1].row);

		MatTrans(&P_WeightMat[i+1], &tempTransW);
		ActiFunDerivation(P_SumMat[i], &ActiFuncMat, i);

		MatMul(&P_DeltaMat[i + 1], &tempTransW, &tempMulMat);
		MatProduct(&tempMulMat, &ActiFuncMat, &P_DeltaMat[i]);

		MatTrans(&P_ActiMatPlus[i - 1], &tempTransActi);
		MatMul(&tempTransActi, &P_DeltaMat[i], &P_NablaWbMat[i]);
		//MatDump(&P_NablaWbMat[i]);
		MatNumMul(1.f / N_sample, &P_NablaWbMat[i], &P_NablaWbMat[i]);
		//MatDump(&P_NablaWbMat[i]);

		MatDelete(&tempTransW);
		MatDelete(&ActiFuncMat);
		MatDelete(&tempMulMat);
		MatDelete(&tempProdMat);
		MatDelete(&tempTransActi);
		//break;
	}
	return NULL;
}

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
Mat * BGD(Mat *P_WeightBiasMat, Mat *P_NablaWbMat, int N_hidden, float alpha){
	Mat temp;
	for (int i = 1; i <= N_hidden + 1; ++i){
		//MatDump(&P_WeightBiasMat[i]);
		MatCreate(&temp, P_NablaWbMat[i].row, P_NablaWbMat[i].col);
		MatNumMul(alpha, &P_NablaWbMat[i], &temp);
		MatSub(&P_WeightBiasMat[i],&temp , &P_WeightBiasMat[i]);
		MatDelete(&temp);
	}
	return P_WeightBiasMat;
}



/************************************************************************/
/*                          神经网络优化算法                          */
/************************************************************************/






int main(){


	/*用户自定义参数输入*/
	int N_sample = 16; //样本数量
	int D_sample = 4;  //样本维度
	int N_out = 2;   //二分类

	float Xval[] = {
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 1.f, 1.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 1.f, 1.f, 0.f,
		0.f, 1.f, 1.f, 1.f,
		1.f, 0.f, 0.f, 0.f,
		1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 1.f, 0.f,
		1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 0.f, 0.f,
		1.f, 1.f, 0.f, 1.f,
		1.f, 1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f }; //样本真值

	//float Xval[] = {
	//	0.4f, 0.4f, 0.4f, 0.4f,
	//	0.4f, 0.4f, 0.4f, 0.6f,
	//	0.4f, 0.4f, 0.6f, 0.4f,
	//	0.4f, 0.4f, 0.6f, 0.6f,
	//	0.4f, 0.6f, 0.4f, 0.4f,
	//	0.4f, 0.6f, 0.4f, 0.6f,
	//	0.4f, 0.6f, 0.6f, 0.4f,
	//	0.4f, 0.6f, 0.6f, 0.6f,
	//	0.6f, 0.4f, 0.4f, 0.4f,
	//	0.6f, 0.4f, 0.4f, 0.6f,
	//	0.6f, 0.4f, 0.6f, 0.4f,
	//	0.6f, 0.4f, 0.6f, 0.6f,
	//	0.6f, 0.6f, 0.4f, 0.4f,
	//	0.6f, 0.6f, 0.4f, 0.6f,
	//	0.6f, 0.6f, 0.6f, 0.4f,
	//	0.6f, 0.6f, 0.6f, 0.6f }; //样本真值

	float Yval[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };  //样本标签二分类

	int N_hidden = 3;//神经网络隐藏层层数

	int *N_layerNeuron = NULL; // 各层神经元个数  0(输入层),1,...,N_hidden,N_hidden+1(输出层).
	int Nval[] = { 4, 5, 6, 5, 2 };// 各隐藏层神经元个数真值

	N_layerNeuron = intVal2List(N_hidden + 2, Nval, N_layerNeuron);
	////测试传入正确性
	//for (int i = 0; i < N_hidden + 2; ++i){
	//	printf("%d\n", N_layerNeuron[i]);
	//}

	int *Nstr_ActiFsHidden = NULL;// 各层激活函数使用；
	int Aval[] = { 0, 3, 3, 3, 5 };// 各层激活函数使用真值；注意映射关系。

	Nstr_ActiFsHidden = intVal2List(N_hidden + 2, Aval, Nstr_ActiFsHidden);

	////测试传入正确性
	//for (int i = 0; i < N_hidden + 2; ++i){
	//	printf("%d\n", NStr_ActiFsHidden[i]);
	//}

	int Nstr_LossF = 1;//use CrossEntropy Loss function

	int Style_initWeight = 3;   //kaiming initialization














	/*构建神经网络所需空间并初始化所需参数*/
	//神经网络所需空间变量
	Mat* P_ActiMat=NULL;			//神经网络激活值矩阵
	Mat* P_ActiMatPlus = NULL;		//神经网络激活值矩阵加偏置列	
	Mat* P_SumMat = NULL;			//神经网络求和矩阵				
	Mat* P_WeightMat = NULL;		//神经网络权值矩阵		
	Mat* P_WeightBiasMat = NULL;	//神经网络权值偏置矩阵	
	Mat Mat_oneHot;					//训练数据标签	
	MatCreate(&Mat_oneHot, N_sample, N_out);
	Mat Mat_Y;
	MatCreate(&Mat_Y, N_sample, 1);
	Mat* P_DeltaMat = NULL;			//反向传播中间变量矩阵	
	Mat* P_NablaWbMat = NULL;		//反向传播权值偏置导数矩阵
	Mat* P_ActiFunDerivation = NULL; //激活函数对求和值求导数矩阵

	//用户输入参数
	//样本数量			N_sample			int
	//隐藏层层数		N_hidden			int
	//各层神经元个数	N_layerNeuron		int*
	//各层激活函数		NStr_ActiFsHidden	int*


	P_ActiMat = SpaceCreateActi(P_ActiMat, N_sample, N_hidden, N_layerNeuron);

	//MatDump(&(P_ActiMat[0]));

	P_ActiMatPlus = SpaceCreateActiPlus(P_ActiMatPlus, N_sample, N_hidden, N_layerNeuron);

	//MatDump(&(P_ActiMatPlus[0]));

	P_SumMat = SpaceCreateSum(P_SumMat, N_sample, N_hidden, N_layerNeuron);

	//MatDump(&P_SumMat[0]);    //P_SumMat[0] 无意义

	P_WeightMat = SpaceCreateWeight(P_WeightMat, N_hidden, N_layerNeuron);

	//MatDump(&P_WeightMat[1]);

	P_WeightBiasMat = SpaceCreateWeightBias(P_WeightBiasMat, N_hidden, N_layerNeuron);

	//MatDump(&P_WeightBiasMat[1]);

	P_DeltaMat = SpaceCreateDelta(P_DeltaMat, N_sample, N_hidden, N_layerNeuron);

	//MatDump(&(P_DeltaMat[1]));


	P_NablaWbMat = SpaceCreateNablaWeightBias(P_NablaWbMat, N_hidden, N_layerNeuron);

	P_ActiFunDerivation = SpaceCreateActiFunDerivation(P_ActiFunDerivation, N_sample, N_hidden, N_layerNeuron);
	//printf("Nabla\n");
	//MatDump(&(P_NablaWbMat[1]));








	/*初始化神经网络参数*/
	//输入矩阵 输出矩阵 权值权值矩阵
	NNinit(P_ActiMat, P_ActiMatPlus, &Mat_Y, &Mat_oneHot, P_WeightMat, P_WeightBiasMat, N_out, N_hidden, Xval, Yval, Style_initWeight);

	//MatDump(&P_ActiMat[0]);
	//MatDump(&P_ActiMatPlus[0]);
	//MatDump(&Mat_Y);
	//MatDump(&Mat_oneHot);
	//MatDump(&P_WeightMat[1]);
	//MatDump(&P_WeightBiasMat[1]);




	
	for (int i = 0; i <= 100000; ++i){
		float loss = 0.f;
		/*神经网络前项传播*/
		loss = NNforward(P_ActiMat, P_ActiMatPlus, P_SumMat, P_WeightBiasMat, Mat_oneHot, N_hidden, Nstr_ActiFsHidden, Nstr_LossF);
		if (i == 0){
			MatDump(&P_ActiMat[N_hidden + 1]);
			MatDump(&Mat_oneHot);
		}
		if (i % 2000 == 0){
			printf("第%d次训练：%f\n", i,loss);
			//MatDump(&P_ActiMat[N_hidden + 1]);
			//MatDump(&Mat_oneHot);
			//printf("%f\n", CrossEntropy(&P_ActiMat[N_hidden + 1], &Mat_oneHot));
			/*for (int i = 0; i < N_hidden + 2; ++i){
				printf("Weight Bias:\n");
				MatDump(&P_WeightBiasMat[i]);*/
				//printf("Weight Bias Nabla:\n");
				//MatDump(&P_NablaWbMat[i]);
			//}
		}
		
		//printf("%d\n", isinf(loss));

		NNBackward(N_hidden, N_sample, N_layerNeuron, Nstr_ActiFsHidden, Nstr_LossF, P_NablaWbMat, P_SumMat, P_DeltaMat, P_ActiFunDerivation, P_ActiMat, P_ActiMatPlus, Mat_oneHot, P_WeightMat);



		BGD(P_WeightBiasMat, P_NablaWbMat, N_hidden, 0.1f);

	}
	MatDump(&P_ActiMat[N_hidden + 1]);
	MatDump(&Mat_oneHot);
	return 0;
}


















/************************************************************************/
/*                        二维矩阵测试主函数                            */
/************************************************************************/
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
//	printf("矩阵 a1 = a+a:\n");
//	MatDump(MatAdd(&a, &a, &a1));
//	printf("矩阵 b1 = b-b:\n");
//	MatDump(MatSub(&b, &b, &b1));
//	printf("矩阵 c = bxa:\n");
//	MatDump(MatMul(&b, &a, &c));
//
//	printf("矩阵 b = a转置:\n");
//	MatDump(MatTrans(&a, &b));
//
//	printf("矩阵复制 a1 = a.\n");
//	MatCopy(&a, &a1);
//	MatDump(&a1);
//
//
//	Mat x;
//	Mat y;
//	Mat z;
//	MatCreate(&x, 2, 3);
//	MatCreate(&y, 2, 4);
//	MatCreate(&z, 2, 2);
//	MatDump(MatSetVal(&x, val2));
//	printf("y = x^+\n");
//	MatPlus(&x, &y);
//	MatDump(&y);
//
//	printf("z = x^-\n");
//	MatMinus(&x, &z);
//	MatDump(&z);
//
//
//	MatCreate(&e, 2, 1);
//	printf("每一行元素求和 e = sum(a)\n");
//	MatDump(MatSum(&a, &e));
//
//	printf("每一行元素求最大值 e = MatMax(a)\n");
//	MatDump(MatMax(&a, &e));
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
//	MatDump(&d);
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
