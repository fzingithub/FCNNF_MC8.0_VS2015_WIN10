#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <setjmp.h>
#include <math.h>
#include <ctype.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <windows.h>
#include <intrin.h>
#include <io.h>

#pragma comment(lib,"Winmm.lib")

DWORD start_time;
//�Ѽ���Ϣ
void $$Push(char * left, char * right, int priority);

//And Pal Prj Keep Alw Manager
//  0   1   2    3   4       5
//���ݺ�����ڵ�ַ�Ͳ��������̣߳���Ϊ���߳�����Node*���͵Ľ�㣬�����߳�����
void MyCreateThread(unsigned int (WINAPI addr)(LPVOID), LPVOID para, int type);

//��ͣһ���̣߳�֮������̻߳���������
void MyPauseThread(void);

//�������ֻ�������߳��������
void MyWaitForObject(void);

//���MSV�е�ȫ�ֱ�������
//����1��״̬�� ����2��ȫ�ֱ����������
void GetMsvVar(int * sNum, void(*output)(void), int(*isout)(void));

//���MSV�е�ȫ�ֱ�������Ϣ
//����1����ǰor�����±꣬ ��ǰor���ĸ���
void GetExternalVars(int *pCurrentOr, int *pLengthOr, char *pFlagOr, void(*f)(void), char * outFile);

void AddEdge(const char* pVarsInfo, int pStatementNum, int isEmpty);

void UpdateCurrentNode(void);

void SaveCurrentNode(void);

void Draw(void);

void setNodeLength(int type);

//while��ʼʱ���ô˺�����������ͬ��Ϣ���룬���ҵ�ѭ����������Ϣ����whileInfo
//while2IR�в���˺����ĵ���
void whileBegin(unsigned int, unsigned int);

//���ΪwhileNum��while�����ˣ�ɾ����Ӧ��Ϣ
//while2IR�в���˺����ĵ���
void whileEnd(unsigned int, unsigned int);

void setModelOption(int);

jmp_buf $$myJmpbuf;

void addStatement(int);

void GetModelVar(char(*var)[100000], int* l, void(*catVar)(void));

int $$MySetJmp(void)
{
	int a = _setjmp($$myJmpbuf);
	return a;
}

void $$MyLongJmp(void)
{
	printf("********longJmp*********");
	longjmp($$myJmpbuf, 1);
}


void printPathTime(void)
{
	printf("This path cost : %lu ms\n", GetTickCount() - start_time);
	start_time = GetTickCount();
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{

}
jmp_buf j_buf;



int Spe_InterlockedCompareExchange(LPLONG a, LONG b, LONG c) {

	return (int)InterlockedCompareExchange(a, b, c);

}


void* memcpy(void *dest, const void *src, size_t count)
{
	//assert(dest != NULL && src != NULL && count>0);
	char *tmp_dest = (char*)dest;
	char *tmp_src = (char*)src;
	while (count--)//�����Ƿ�����ص���������ж�
		*tmp_dest++ = *tmp_src++;
	return dest;
}

void* memmove(void *dest, const void *src, size_t num)
{
	//assert(dest != NULL && src != NULL && num>0);
	char *tmp_dest = (char*)dest;
	char *tmp_src = (char*)src;
	//�ж�des��src�Ƿ�����ص�
	if (tmp_dest + num < tmp_src || tmp_src + num < tmp_dest)
	{
		while (num--)
			*tmp_dest++ = *tmp_src++;
	}
	else//�����ص�,��ֹ��Ϣ��ʧ,�Ӻ���ǰ��ֵ
	{
		tmp_dest = tmp_dest + num - 1;
		tmp_src = tmp_src + num - 1;
		while (num--)
			*tmp_dest-- = *tmp_src--;
	}
	return dest;
}

void* memset(void *s, int c, size_t n)
{
	unsigned char* p = (unsigned char*)s;

	while (n > 0) {
		*p++ = (unsigned char)c;
		--n;
	}

	return s;
}

void ex()
{
	_beginthreadex(NULL, 1, NULL, NULL, 1, NULL);
	_endthreadex(1);
	strncmp("asdf", "afd", 13);
	int * px = (int*)malloc(2 * sizeof(int));
	realloc(px, 1);
	memset(px, 2, 1);
	memmove(px, px, 1);
	memcmp(NULL, NULL, 10);
	memcpy(NULL,NULL,10);
	free(px);
	char c[10] = "a";
	char b[10] = "a";
	int x = strlen(c);
	x = strcmp(b, c);
	strcpy(c, b);
	strcat(b, c);
	strncat(b, c, 1);
	sprintf(b, "a", x);
	system("IRResult.exe");
	GetExternalVars(NULL, NULL, NULL, NULL, NULL);
	isdigit(c[1]); isupper(c[1]); tolower(c[1]); islower(c[1]); toupper(c[1]); isspace(c[1]);
	isatty(0);
	isalpha(0);
	long *lp = NULL;
	InterlockedCompareExchange(lp, 0, 0);
	strrchr("xxx", 38);
	strcspn("x", "x");
	strspn("x", "x");
	atoi(c); atof(c);
	floor(1.0);
	sqrt(5.0); pow(1.0, 1.0); exp(2.0);
	sin(1.0); asin(1.0); tan(1.0); atan(1.0); atan2(1.0, 1.0);
	cos(1.0); acos(1.0); abs(1); fabs(1.0); fmod(1.0, 1.0); log(1.0); tanh(1.0);
	calloc(1, 1);
	clock(); time(NULL); srand(0); rand();
	getenv(c);
	FILE * fp;
	fp = fopen("1.txt", "r");
	fread(c, 1, 1, fp);
	fprintf(fp, "%d", x);
	fprintf(stdin, "%d", x);
	fprintf(stdout, "%d", x);
	fprintf(stderr, "%d", x);
	fileno(fp);
	fscanf(fp, "%d", &x); sscanf(c, "%s", c);
	fwrite(c, 1, 1, fp);
	fseek(fp, 1, SEEK_SET);
	feof(fp);
	strtok(NULL, "sdfs");
	itoa(1, c, 10);
	rewind(fp);  fflush(fp); rename(c, c); _unlink(c);
	read(0, (void*)NULL, 0);
	tmpnam((char*)NULL);
	write(0, NULL, 0); setjmp(j_buf);
	AddEdge("", 0, 0);
	Draw();
	UpdateCurrentNode();
	SaveCurrentNode();
	if (c[0] != EOF)
		fputs(c, fp);
	fputc(c[1], fp); fgetc(fp); fgets(c, 10, fp); tmpfile();
	getchar(); putchar('a');
	fclose(fp);
	open(c, O_RDONLY | O_BINARY); _close(1); perror(c); strerror(errno); remove(c); _read(1, (void*)c, 1);
	Sleep(1);
	setbuf(fp, c);
	//windows��������
	HANDLE  Handle;
	HANDLE OSSemaphore;
	CRITICAL_SECTION cs;
	TIMECAPS Timecaps;
	va_list OS_Printf_marker;
	//windows API����
	GetSystemInfo(NULL);
	//temp
	time_t timeaaa;
	localtime(&timeaaa);
	CRITICAL_SECTION  a;
	DeleteCriticalSection(&a);

	LPLONG aaa;
	Spe_InterlockedCompareExchange(aaa, 0, 0);
	unsigned char xxxx;
	_byteswap_ushort(xxxx);
	_ReadWriteBarrier();
	int x1;
	_byteswap_ulong(x1);
	_beginthreadex(NULL, 0, NULL, NULL, 0, NULL);
	_endthreadex(0);
	strncmp("", "", 1);
	AreFileApisANSI();
	CloseHandle(NULL);
	CreateFileA(NULL, 0, 0, NULL, 0, 0, NULL);
	CreateFileW(NULL, 0, 0, NULL, 0, 0, NULL);
	CreateFileMappingA(NULL, NULL, 0, 0, 0, NULL);
	CreateFileMappingW(NULL, NULL, 0, 0, 0, NULL);
	CreateMutexW(NULL, NULL, NULL);
	DeleteFileA(NULL);
	DeleteFileW(NULL);
	FlushFileBuffers(NULL);
	FormatMessageA(0, NULL, 0, 0, NULL, 0, NULL);
	FormatMessageW(0, NULL, 0, 0, NULL, 0, NULL);
	FreeLibrary(NULL);
	GetCurrentProcessId();
	GetDiskFreeSpaceA(NULL, NULL, NULL, NULL, NULL);
	GetDiskFreeSpaceW(NULL, NULL, NULL, NULL, NULL);
	GetFileAttributesA(NULL);
	GetFileAttributesW(NULL);
	GET_FILEEX_INFO_LEVELS x2;
	GetFileAttributesExW(NULL, x2, NULL);
	GetFileSize(NULL, NULL);
	GetFullPathNameA(NULL, 0, NULL, NULL);
	GetFullPathNameW(NULL, 0, NULL, NULL);
	GetLastError();
	GetProcAddress(NULL, NULL);
	GetSystemInfo(NULL);
	GetSystemTime(NULL);
	GetSystemTimeAsFileTime(NULL);
	GetTempPathA(0, NULL);
	GetTempPathW(0, NULL);
	GetTickCount();
	GetVersionExA(NULL);
	GetVersionExW(NULL);
	HeapAlloc(NULL, 0, 0);
	HeapCreate(0, 0, 0);
	HeapDestroy(0);
	HeapFree(NULL, 0, NULL);
	HeapReAlloc(NULL, 0, NULL, 0);
	HeapSize(NULL, 0, NULL);
	HeapValidate(NULL, 0, NULL);
	HeapCompact(NULL, 0);
	LoadLibraryA(NULL);
	LoadLibraryW(NULL);
	LocalFree(NULL);
	LockFile(NULL, 0, 0, 0, 0);
	LockFileEx(NULL, 0, 0, 0, 0, 0);
	MapViewOfFile(NULL, 0, 0, 0, 0);
	MultiByteToWideChar(0, 0, 0, 0, NULL, 0);
	QueryPerformanceCounter(NULL);
	ReadFile(NULL, NULL, 0, NULL, NULL);
	SetEndOfFile(NULL);
	SetFilePointer(NULL, 0, NULL, 0);
	Sleep(0);
	SystemTimeToFileTime(NULL, 0);
	UnlockFile(NULL, 0, 0, 0, 0);
	UnlockFileEx(NULL, 0, 0, 0, NULL);
	UnmapViewOfFile(NULL);
	WideCharToMultiByte(0, 0, NULL, 0, NULL, 0, NULL, NULL);
	WriteFile(NULL, NULL, 0, NULL, NULL);
	WaitForSingleObject(NULL, 0);
	WaitForSingleObjectEx(NULL, 0, 0);
	OutputDebugStringA(NULL);
	OutputDebugStringW(NULL);
	GetProcessHeap();
	FlushViewOfFile(NULL, 0);


	EnterCriticalSection(&cs);

	LeaveCriticalSection(&cs);
	InitializeCriticalSection(&cs);
	WaitForSingleObject(OSSemaphore, 1);
	ReleaseSemaphore(OSSemaphore, 1, NULL);
	CreateSemaphore(NULL, 1, 1, NULL);
	SetThreadAffinityMask(Handle, 1);
	GetCurrentThreadId();
	CreateThread(NULL, 0, ClientThread, NULL, 0, NULL);
	SuspendThread(Handle);
	ResumeThread(Handle);
	SetThreadPriority(Handle, THREAD_PRIORITY_HIGHEST);
	SetThreadPriorityBoost(Handle, TRUE);
	CloseHandle(Handle);

	CreateEvent(NULL, 0, 0, NULL);
	ResetEvent(Handle);

	timeSetEvent(10, 1, 0, 1, 1);
	timeKillEvent(0);
	timeEndPeriod(0);
	timeGetDevCaps(&Timecaps, 0);
	timeBeginPeriod(0);

	SetPriorityClass(Handle, THREAD_PRIORITY_HIGHEST);
	GetTickCount();

	vprintf(c, OS_Printf_marker);

	MyCreateThread(NULL, NULL, 1);
	MyPauseThread();
	MyWaitForObject();
	setNodeLength(1);

	$$Push("", "", 1);
	GetMsvVar(NULL, NULL, NULL);

	addStatement(0);
	GetModelVar(NULL, NULL, NULL);
	whileBegin(0, 0);
	whileEnd(0, 0);
	setModelOption(0);

	exit(0);
}

void ex1()
{
	longjmp(j_buf, 0);
}

int $$tempLen = 0;
char $$varsInfo[100000];//�����ñ�����ֵƴ�������Ļ���

void $$addChar(char ch)
{
	$$varsInfo[$$tempLen] = ch;
	$$tempLen++;
	$$varsInfo[$$tempLen] = 0;
}

void $$addInt(int num)
{
	char temp[10] = { 0 };
	sprintf(temp, "%d", num);
	strcat($$varsInfo, temp);
	$$tempLen += strlen(temp);
	$$varsInfo[$$tempLen] = 0;
}

void $$addFloat(float num)
{
	char temp[30] = { 0 };
	sprintf(temp, "%f", num);
	strcat($$varsInfo, temp);
	$$tempLen += strlen(temp);
	$$varsInfo[$$tempLen] = 0;
}

void $$addString(char* str)
{
	strcat($$varsInfo, str);
	$$tempLen += strlen(str);
	$$varsInfo[$$tempLen] = 0;
}
void sayhello()
{
	printf("hello MSVL\n");
}
