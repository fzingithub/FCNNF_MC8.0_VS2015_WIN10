/*��ÿ����֧Ҳ����ÿ��if...else���ǰ����changeState()���������current����len˵�������µĽڵ㣬���ӽڵ㣬���current��1������������һ����֧*/
function changeState()
{
	if(current=length) then
	{
		length:=length+1;
		flag[length-1]:=0;
		SaveCurrentNode()
	}
	else
	{
		skip
	};

	if(flag[current]=1)then
	{
		UpdateCurrentNode() and skip
	};
	current:=current+1
};

pathNum<==1 /*and length <== 0 and current <== 0 and firstTime<==0 and flag1<==0 and flag2<==0*/ and skip;
while(flag2=0)
{
	flag1:=0;
	while(length>0 AND flag1=0)
	{
		if(flag[length-1]=1) then//ɾ���Ѿ����������ߵĽڵ�
		{
			length:=length-1
		}
		else //�������һ�����֧����һ����֧
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
	//���������������
}