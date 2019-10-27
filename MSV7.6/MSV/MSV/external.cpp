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
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <windows.h>
#include <intrin.h>
#include <io.h>
#include <intrin.h>
#include <iostream>
#pragma comment(lib,"Winmm.lib")

using namespace std;
DWORD start_time;
//搜集信息
void $$Push(char * left, char * right, int priority);

//And Pal Prj Keep Alw Manager
//  0   1   2    3   4       5
//根据函数入口地址和参数创建线程，并为此线程生成Node*类型的结点，加入线程树中
void MyCreateThread(unsigned int (WINAPI addr)(LPVOID), LPVOID para, int type);

//暂停一个线程，之后调度线程会阻塞掉它
void MyPauseThread();

//这个函数只有主调线程最后会调用
void MyWaitForObject();

//获得MSV中的全局变量的信
//参数1：状态数 参数2：全局变量输出函数
void GetMsvVar(int * sNum, void(*output)(), bool(*isout)());

//获得MSV中的全局变量的信息
//参数1：当前or语句的下标， 当前or语句的个数
void GetExternalVars(int *pCurrentOr, int *pLengthOr, char *pFlagOr, void(*f)(),char * outFile);

void AddEdge(const char* pVarsInfo, int pStatementNum, int isEmpty);

void UpdateCurrentNode();

void SaveCurrentNode();

void Draw();

void setNodeLength(int type);

//while开始时调用此函数，若有相同信息存入，则找到循环，否则将信息存入whileInfo
//while2IR中插入此函数的调用
void whileBegin(unsigned int, unsigned int);

//编号为whileNum的while结束了，删除对应信息
//while2IR中插入此函数的调用
void whileEnd(unsigned int, unsigned int);

void setModelOption(bool );

jmp_buf $$myJmpbuf;

void addStatement(int );

void GetModelVar(char (*var)[100000], int* l, void(*catVar)());

int $$MySetJmp()
{
	
	int a = setjmp($$myJmpbuf);

	return 1;
}

void $$MyLongJmp()
{
	printf("********longJmp*********");
	longjmp($$myJmpbuf, 1);
}


void printPathTime()
{
	printf("This path cost : %lu ms\n", GetTickCount() - start_time);
	start_time = GetTickCount();
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	
}
jmp_buf j_buf;



int Spe_InterlockedCompareExchange(LPLONG a, LONG b, LONG c){

	return (int)InterlockedCompareExchange(a, b, c);

}

void geterror() {
	DWORD x = GetLastError();
	LPVOID buf;
	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS
		| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&buf,
		0,
		NULL)) {
		printf("%s\n", buf);
		fflush(stderr);
		LocalFree(buf);
	}


}

typedef struct SPE_SYSTEM_INFO {
	unsigned long replica;
	DWORD dwPageSize;
	LPVOID lpMinimumApplicationAddress;
	LPVOID lpMaximumApplicationAddress;
	DWORD_PTR dwActiveProcessorMask;
	DWORD dwNumberOfProcessors;
	DWORD dwProcessorType;
	DWORD dwAllocationGranularity;
	WORD wProcessorLevel;
	WORD wProcessorRevision;
} SPE_SYSTEM_INFO;

void Spe_GetSystemInfo(SPE_SYSTEM_INFO* x) {
	_SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);
	x->replica = 9;
	x->dwPageSize = sys_info.dwPageSize;
	x->lpMinimumApplicationAddress = sys_info.lpMinimumApplicationAddress;
	x->lpMaximumApplicationAddress = sys_info.lpMaximumApplicationAddress;
	x->dwActiveProcessorMask = sys_info.dwActiveProcessorMask;
	x->dwNumberOfProcessors = sys_info.dwNumberOfProcessors;
	x->dwProcessorType = sys_info.dwProcessorType;
	x->dwAllocationGranularity = sys_info.dwAllocationGranularity;
	x->wProcessorLevel = sys_info.wProcessorLevel;
	x->wProcessorRevision = sys_info.wProcessorRevision;
}
void Spe_GetVersionExA(OSVERSIONINFOA * b) {
	OSVERSIONINFOA x;
	GetVersionExA(&x);
	b->dwMajorVersion = x.dwMajorVersion;
	b->dwBuildNumber = x.dwBuildNumber;
	b->dwMinorVersion = x.dwMinorVersion;
	b->dwOSVersionInfoSize = x.dwOSVersionInfoSize;
	b->dwPlatformId = x.dwPlatformId;
}


void ex()
{
	_beginthreadex(NULL,1,NULL,NULL,1,NULL);
	_endthreadex(1);
	strncmp("asdf","afd",13);
	_ReadWriteBarrier();
	int * px = (int*)malloc(2*sizeof(int));
	realloc(px,1);
	memset(px,2,1);
	memmove(px,px,1);
	free(px);
	char c[10] = "a";
	char b[10] = "a";
	int x = strlen(c);
	x = strcmp(b,c);
	strcpy(c,b);
	strcat(b,c);
	strncat(b, c, 1);
	sprintf(b,"a",x);
	system("IRResult.exe");
	GetExternalVars(NULL, NULL, NULL, NULL,NULL);
	isdigit(c[1]); isupper(c[1]); tolower(c[1]); islower(c[1]); toupper(c[1]); isspace(c[1]);
	isatty(0); 
	isalpha(0);
	long *lp = NULL;
	InterlockedCompareExchange(lp,0,0);
	strrchr("xxx",38);
	strcspn("x","x");
	strspn("x","x");
	atoi(c); atof(c);
	floor(1.0);
	sqrt(5.0); pow(1.0,1.0); exp(2.0);
	sin(1.0); asin(1.0); tan(1.0); atan(1.0); atan2(1.0,1.0);
	cos(1.0); acos(1.0); abs(1); fabs(1.0); fmod(1.0, 1.0); log(1.0); log10(1.0);
	calloc(1,1);
	clock(); time(NULL); srand(0); rand();
	getenv(c);
	FILE * fp;
	fp = fopen("1.txt","r");
	fread(c,1,1,fp);
	fprintf(fp,"%d",x);
	fprintf(stdin,"%d",x);
	fprintf(stdout,"%d",x);
	fprintf(stderr,"%d",x);
    fileno(fp);
	fscanf(fp,"%d",&x); sscanf(c,"%s",c);
	fwrite(c,1,1,fp);
	fseek(fp, 1, 0);
	feof(fp);
	strtok(NULL,"sdfs");
	itoa(1,c,10);
	rewind(fp);  fflush(fp); rename(c, c); _unlink(c);
	read(0, (void*)NULL, 0); 
	tmpnam((char*)NULL);
	write(0, NULL, 0); setjmp(j_buf); 
	memcmp(NULL, NULL, 10);
	AddEdge("", 0, 0);
	Draw();
	UpdateCurrentNode();
	SaveCurrentNode();
if(c[0]!=EOF)	
	fputs(c,fp);
	fputc(c[1],fp); fgetc(fp);fgets(c,10,fp); tmpfile();
    getchar(); putchar('a');
	fclose(fp); 
	open(c, O_RDONLY|O_BINARY); _close(1); perror(c); strerror(errno); remove(c); _read(1,(void*) c, 1);
	Sleep(1);
	setbuf(fp, c);
	//windows数据类型
	HANDLE  Handle;
	HANDLE OSSemaphore;
    CRITICAL_SECTION cs;
	TIMECAPS Timecaps;
	va_list OS_Printf_marker;
	//windows API函数

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
	SetThreadPriorityBoost(Handle,true);
	CloseHandle(Handle);
	
	CreateEvent(NULL,0,0,NULL);
	ResetEvent(Handle);
	
	timeSetEvent(10, 1, 0, 1, 1);
	timeKillEvent(0);
	timeEndPeriod(0);
	timeGetDevCaps(&Timecaps, 0);
	timeBeginPeriod(0);

	va_start(OS_Printf_marker,c);
	va_end(OS_Printf_marker);

	SetPriorityClass(Handle, THREAD_PRIORITY_HIGHEST);
	GetTickCount();

	vprintf(c, OS_Printf_marker);

	MyCreateThread(NULL,NULL,1);
	MyPauseThread();
	MyWaitForObject();
	setNodeLength(1);
	
	$$Push("","",1);
	GetMsvVar(NULL,NULL,NULL);

	addStatement(0);
	GetModelVar(NULL,NULL,NULL);
	whileBegin(0,0);
	whileEnd(0,0);
	setModelOption(0);
	Spe_GetSystemInfo(NULL);
	Spe_GetVersionExA(NULL);
	
	char *tepppppa = "";
	strtod(tepppppa,NULL);
	
	exit(0);
}

void ex1()
{
	longjmp(j_buf, 0);
}

void *memcpy(void *dest, const void *src, size_t count)

{
	//assert(dest != NULL && src != NULL && count>0);
	char *tmp_dest = (char*)dest;
	char *tmp_src = (char*)src;
	while( count--)//不对是否存在重叠区域进行判断
		*tmp_dest++ = *tmp_src++;
	return dest;
}

void * memmove(void *dest, const void *src,int num)
{
	if (dest == NULL || src == NULL)
		return 0;
	//assert(dest != NULL && src != NULL && num>0);
	char *tmp_dest = (char*)dest;
	char *tmp_src = (char*)src;
	//判断des和src是否存在重叠
	if(tmp_dest+num < tmp_src || tmp_src+num < tmp_dest)
	{
		while(num--)
			*tmp_dest++ = *tmp_src++;
	}
	else//存在重叠,防止信息丢失,从后向前赋值
	{
		tmp_dest = tmp_dest + num - 1;
		tmp_src = tmp_src + num -1;
		while(num--)
			*tmp_dest-- = *tmp_src--;
	}
	return dest;
}
void* memset(void* s, int c, size_t n)
{
    unsigned char* p = (unsigned char*) s;

    while (n > 0) {
    	*p++ = (unsigned char) c;
   		--n;
    }

    return s;
}
int $$tempLen= 0;
char $$varsInfo[100000];//将所用变量及值拼接起来的缓存
void $$addChar(char ch)
{
	$$varsInfo[$$tempLen] = ch;
	$$tempLen++;
	$$varsInfo[$$tempLen] = '\0';
	
}

void $$addInt(int num)
{
	char temp[10] = { '\0' };
	sprintf(temp, "%d", num);
	strcat($$varsInfo, temp);
	$$tempLen += strlen(temp);
	$$varsInfo[$$tempLen] = '\0';
}

void $$addFloat(float num)
{
	char temp[30] = {'\0'};
	sprintf(temp, "%f", num);
	strcat($$varsInfo, temp);
	$$tempLen += strlen(temp);
	$$varsInfo[$$tempLen] = '\0';

}

void $$addString(char* str)
{
	strcat($$varsInfo, str);
	$$tempLen += strlen(str);
	$$varsInfo[$$tempLen] = '\0';
}