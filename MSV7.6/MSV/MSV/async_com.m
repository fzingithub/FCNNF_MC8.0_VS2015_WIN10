//元素0的value和aux分别代表最大长度和当前长度
struct Chn {
int value and
int aux
};

int $$x and skip;

function SendInt(Chn *channel,int x)
{
	await(channel[0].value>channel[0].aux+1);
	channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=x
	
};

//通道的移位操作,返回值为首元素
function $$myShift(Chn *channel, int RValue)
{
frame(i,x,y) and
(
	int i,x and skip;
	x:=channel[1].value;
	i:=1;
	while(i<=channel[0].aux)
	{
		channel[i].value:=channel[i+1].value;
		i:=i+1
	};
	channel[channel[0].aux+1].value:=0;
	RValue:=x
)
};

function ReceiveInt(Chn *channel,int *y)
{
	await(channel[0].aux!=0);
	channel[0].aux:=channel[0].aux-1 and  *y:= (extern  $$myShift(channel))
	
};

function PutInt(Chn *channel,int x)
{
	if(channel[0].value>channel[0].aux+1) then
	{
		channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=x
	}else
	{skip}
};

function GetInt(Chn *channel,int *y)
{
	if(channel[0].aux!=0) then
	{
		channel[0].aux:=channel[0].aux-1 and  *y:= (extern  $$myShift(channel))
	}else
	{skip}
};

function SendChar(Chn *channel,char x)
{
	await(channel[0].value>channel[0].aux+1);
	channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x
	
};

function ReceiveChar(Chn *channel,char *y)
{
	await(channel[0].aux!=0);
	channel[0].aux:=channel[0].aux-1 and  *y:= (char)(extern  $$myShift(channel))
	
};

function PutChar(Chn *channel,char x)
{
	if(channel[0].value>channel[0].aux+1) then
	{
		channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x
	}else
	{skip}
};

function GetChar(Chn *channel,char *y)
{
	if(channel[0].aux!=0) then
	{
		channel[0].aux:=channel[0].aux-1 and  *y:= (char)(extern  $$myShift(channel))
	}else
	{skip}
};

function SendFloat(Chn *channel,float x)
{
	await(channel[0].value>channel[0].aux+1);
	channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x
	
};

function ReceiveFloat(Chn *channel,float *y)
{
	await(channel[0].aux!=0);
	channel[0].aux:=channel[0].aux-1 and  *y:= (float)(extern  $$myShift(channel))
	
};

function PutFloat(Chn *channel,float x)
{
	if(channel[0].value>channel[0].aux+1) then
	{
		channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x
	}else
	{skip}
};

function GetFloat(Chn *channel,float *y)
{
	if(channel[0].aux!=0) then
	{
		channel[0].aux:=channel[0].aux-1 and  *y:= (float)(extern  $$myShift(channel))
	}else
	{skip}
};

function $$Mymemcpy(char *dst,char *src)
{
while(*src!='\0')
{
	*dst := *src;
	src:=src+1 and dst:=dst+1
};
*dst := '\0'
};

function SendStr(Chn *channel,char *x)
{
	await(channel[0].value>channel[0].aux+1);
	channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x
	
};

function ReceiveStr(Chn *channel,char *y)
{
	await(channel[0].aux!=0);
	channel[0].aux:=channel[0].aux-1 and  extern $$Mymemcpy(y, (char*)(extern  $$myShift(channel)) )
	
};

function PutStr(Chn *channel,char *x)
{
	if(channel[0].value>channel[0].aux+1) then
	{
		channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x
	}else
	{skip}
};

function GetStr(Chn *channel,char *y)
{
	if(channel[0].aux!=0) then
	{
		channel[0].aux:=channel[0].aux-1 and  extern $$Mymemcpy(y, (char*)(extern  $$myShift(channel)) )
	}else
	{skip}
}