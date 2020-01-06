
function xmemcpy(int *des, int *src, int num)
{
    frame(i) and (
    int i<==0 and skip;
    while(i<num)
    {
	des[i]:=src[i];
	i:=i+1
    }
    )
};
function mymemcmp(int *buf1, int *buf2, int count,int RValue)
{
    frame(i,resFlag) and(
    int i<==0 and skip;
    int resFlag<==0 and skip;
    while((i<count) AND (resFlag=0))
    {
	if(buf1[i]=buf2[i]) then
	{
	     i:=i+1
	}
	else 
	{
	   if(buf1[i]>buf2[i]) then
           {
         resFlag:=1;
	     RValue:=1
        }
        else
	   {
	     resFlag:=1;
	     RValue:=-1
	   }
	}
	
    };
    if(resFlag=0) then
    {
	RValue:=0
    } 
else
{
  skip
}
    )
};

frame(iStep, i, a, b,success,return1,continue1) and
(
    int success<==0 and skip;
	int iStep<==0 and skip;
	int continue1<==0 and skip;
	int return1<==0 and skip;
	int a[15,4] and skip;
	int b[15] and skip;
	while(iStep<15 AND return1=0)
	{
		continue1<==0 and skip;
		int i<==0 and skip;
		if(a[iStep,0]+a[iStep,1]+a[iStep,2]+a[iStep,3]=4) then
		{
		    success:=1;
			output("success!!!\n") and skip;
			return1:=1
		}
		else
		{
		    skip
		};
		if(return1=0) then
		{
		    i:=0;
			while(i<iStep AND return1=0)
			{
			    //if(extern memcmp(a[i],a[iStep], sizeof(a[i]),RValue)=0) then
				if(extern mymemcmp(a[i],a[iStep], 4,RValue)=0) then
				{
				return1:=1
				}
				else
				{
				skip
				};
				if(return1=0) then//修改
				{
				i:=i+1
				}
				else
				{
				skip
				}
			};
			if(return1=0) then
			{
			if (a[iStep,1] != a[iStep,3] AND (a[iStep,2] = a[iStep,1] OR a[iStep,0] = a[iStep,1])) then
		    {
			    return1:=1//不能带
		    }
			else
			{
			    skip
			};
			if(return1=0) then
			{
			    b[iStep] := 1 or b[iStep] := -1 or b[iStep] := 2 or b[iStep] := 0;
				xmemcpy(a[iStep + 1], a[iStep], 16);
				a[iStep + 1,3] := 1 - a[iStep + 1,3];
				if(b[iStep]=-1) then
				{
				    iStep:=iStep+1;
					continue1:=1
				}
				else
				{
				skip
				};
				if(continue1=0) then
				{
						int j<==b[iStep] and skip;
						if (a[iStep,j] = a[iStep,3]) then
						{
							a[iStep + 1,j] := a[iStep + 1,3];
							iStep:=iStep+1;
				            continue1:=1
					}
					else
					{
							return1:=1
					}
				}
				else
				{ 
				skip
				}
			}
			else
			{
			    skip
			}
		}
		else
		{
		    skip
		}
		   
		}
		   
		else
		{
		skip
		}
		   
	}
)



