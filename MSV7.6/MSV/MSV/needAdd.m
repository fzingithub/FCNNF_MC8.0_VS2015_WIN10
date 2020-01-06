/*在每个分支也就是每个if...else语句前调用changeState()函数，如果current等于len说明出现新的节点，增加节点，最后将current加1，代表又遇到一个分支*/
function changeState()
{
	if(current=length) then
	{
		length:=length+1;
		flag[length-1]:=0
	}
	else
	{
		skip
	};
	current:=current+1
};

pathNum<==1 /*and length <== 0 and current <== 0 and firstTime<==0 and flag1<==0 and flag2<==0*/ and skip;
while(flag2=0)
{
	flag1:=0;
	while(length>0 AND flag1=0)
	{
		if(flag[length-1]=1) then//删除已经遍历过两边的节点
		{
			length:=length-1
		}
		else //遍历最后一个左分支的另一个分支
		{
			flag[length-1]:=1;
			flag1:=1
		}
	};
	if((length=0 AND flag[0]=1) OR (current=0 AND firstTime=1)) then
	{
		flag2:=1
	}
	else
	{
		skip
	}
	//在这里添加主程序
}