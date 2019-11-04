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


/* dst = src^+ */
void MatPlus(Mat* src, Mat* dst)
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


/* dst = src^- */
void MatMinus(Mat* src, Mat* dst)
{
	int row, col;

#ifdef MAT_LEGAL_CHECKING
	if (src->row != dst->row || src->col != (dst->col) + 1) {
		printf("\t\terr check, unmathed matrix for MatMinus\t\t\n");
		printf("\t\tsrcMatShape:\n\t\t\t");
		MatShape(src);
		printf("\t\tdstMatShape:\n\t\t\t");
		MatShape(dst);
		return;
	}
#endif
	for (row = 0; row < src->row; row++) {
		for (col = 0; col < src->col; col++)
			(dst->element[row])[col] = (src->element[row])[col + 1];
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

	for (row = 0; row < src->row; ++row){
		for (col = 0; col < src->col; ++col){
			(src->element[row])[col] = gaussrand(0.f, 0.01f);
		}
	}
	return src;
}

/************************************************************************/
/*                           权值初始化函数操作                         */
/************************************************************************/







///************************************************************************/
///*                        二维矩阵测试主函数                            */
///************************************************************************/
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
//
//	printf("===================================================\n");
//	printf("Sigmoid 激活\n");
//	printf("原矩阵：\n");
//	MatDump(&a);
//	printf("激活后的矩阵：\n");
//	MatDump(MatSigmoid(&a, &act));
//
//
//	printf("===================================================\n");
//	printf("Tanh 激活\n");
//	printf("原矩阵：\n");
//	MatDump(&a);
//	printf("激活后的矩阵：\n");
//	MatDump(MatTanh(&a, &act));
//
//	printf("===================================================\n");
//	printf("Relu 激活\n");
//	printf("原矩阵：\n");
//	MatDump(&a);
//	printf("激活后的矩阵：\n");
//	MatDump(MatRelu(&a, &act));
//
//	printf("===================================================\n");
//	printf("LeakyRelu 激活\n");
//	printf("原矩阵：\n");
//	MatDump(&a);
//	printf("激活后的矩阵：\n");
//	MatDump(MatLeakyRelu(0.1f, &a, &act));
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
//	//MatDump(&Y);
//	MatDump(&Yonehot);
//
//	MatRowSum(&Yonehot, &Y);
//	//MatDump(&Y);
//	MatTrans(&Y, &Ytrans);
//
//	MatRowSum(&Ytrans, &Sum);
//
//	//MatDump(&Sum);
//
//
//
//
//
//	Mat prediction;
//	MatCreate(&prediction, 5, 4);
//	float prop[20] = { 0.8f, 0.7f, 0.3f, 0.2f, 1.0f, 1.6f, 0.8f, 0.1f, 0.6f, 0.7f, 0.9f, 0.6f, 0.1f, 0.9f, 1.2f, 1.3f ,0.8f, 6.f, 1.f, 0.7f};
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
//	return 0;
//}




















/************************************************************************/
/*                       权值初始化测试主函数                           */
/************************************************************************/
int main()
{
	////test Init zero
	//Mat m;
	//MatCreate(&m, 10, 4);
	//MatDump(&m);
	//MatInitZero(&m);
	//MatDump(&m);

	////test gaussrand
	//float mean = 0.f;
	//float stdc = 0.1f;
	//float data = 0;
	//int num = 100;
	//float total = 0.;
	//srand((unsigned int)time(NULL));

	//for (int i = 0; i<num; ++i){
	//	data = gaussrand(mean, stdc);
	//	total += data;
	//	printf("%f\n", data);
	//}

	//printf("real mean =  %f\n", total/num);

	Mat weight;
	MatCreate(&weight, 160, 10);

	MatInitRandomNormalization(&weight);
	MatDump(&weight);

	//char buf[20];
	//printf("%s\n", F2S(0.0, buf));

	return 0;
}