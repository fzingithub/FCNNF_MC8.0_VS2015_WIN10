#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 16

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

struct _iobuf {
	char *_ptr;
	int   _cnt;
	char *_base;
	int   _flag;
	int   _file;
	int   _charbuf;
	int   _bufsiz;
	char *_tmpfname;
};
typedef struct _iobuf FILE;

float nums[54880000] = { 0 };
float labels[70000] = { 0 };

int main()
{
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

	fp = fopen("C:/Users/Administrator/Documents/GitHub/MinstHW/DataFeatrue.txt", "r");
	flag = fgets(buf, MAX_LINE, fp);
	while (flag != NULL)
	{
		len = strlen(buf);
		buf[len - 1] = '\0';  /*去掉换行符*/
		//printf("%s %d \n", buf, len - 1);
		nums[i++] = atof(buf);
		flag = fgets(buf, MAX_LINE, fp);
	}
	fclose(fp);

	//for (i = 0; i < 54880000; ++i) {
	//	printf("%d:  %s\n", i, F2S(nums[i], buf));
	//}



	// labels
	fp2 = fopen("C:/Users/Administrator/Documents/GitHub/MinstHW/DataLabel.txt", "r");
	flag = fgets(buf, MAX_LINE, fp2);
	while (flag != NULL)
	{
		len = strlen(buf);
		buf[len - 1] = '\0';  /*去掉换行符*/
		//printf("%s %d \n", buf, len - 1);
		labels[j++] = atof(buf);
		flag = fgets(buf, MAX_LINE, fp2);
	}
	fclose(fp2);

	//for (i = 0; i < 70000; ++i) {
	//	printf("%d:  %s\n", i, F2S(labels[i], buf));
	//}
	return 0;
}
