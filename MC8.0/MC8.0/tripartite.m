function P1(){
	frame(m11,m12,m13,m21,m22,m23,M1,M2,time1,time2,flag1,flag2,result) and (
		char m11[15]<=="sign1(1,c)",m12[15]<=="nil",m13[15]<=="nil",m21[15]<=="sign1(2,c)" and
		char m22[15]<=="nil",m23[15]<=="nil" ,result[15]<=="nil" and int flag1<==0,flag2<==0,time1,time2 and skip;
		( ( SendStr(c12,m11) ;  SendStr(c13,m11)) or skip);
		time1:=clock();
		time2:=time1;
		while(time2-time1<50 AND flag1=0)
		{
			GetStr(c21,m12);
			GetStr(c31,m12);
			if(strcmp(m12,"sign2(1,c)")=0 AND strcmp(m13,"sign3(1,c)")=0) then
			{
				flag1:=1;
				( ( SendStr(c12,m21);SendStr(c13,m21) ) or skip);
				time1:=clock();
				time2:=time1;
				while(time2-time1<50 AND flag2=0)
				{
					GetStr(c21,m12);
					GetStr(c31,m12);
					if(strcmp(m22,"sign2(2,c)")=0 AND strcmp(m23,"sign3(2,c)")=0) then
					{
						flag2:=1
					}
					else
					{
						time2:=clock()
					}
				};
				if(flag2=0)then
				{
					SendStr(c1t,"sign1(3,M1)");
					output("wait\n") and skip;
					ReceiveStr(ct1,result);
					cont1:="signed"
				}
				else
				{
					opt1:=1;
					cont1:="signed"
				}
			}
			else
			{
				time2:=clock()
			}
		};

		if(flag1=0) then      				//fails to complie M1
		{
			time1:=clock();
			time2:=time1;
			while(time2-time1<500 AND flag1=0)  //waits for an meassage from TTP, different computers have different timeouts
			{
				GetStr(ct1,result);
				if(strcmp(result,"Sttp(M1)")=0) then
				{			
					flag1:=1;
					cont1:="signed"
				}
				else
				{
					time2:=clock()
				}			
			}		
		}
		else
		{
			skip
		}
	)
};

function P2(){
	frame(m11,m12,m13,m21,m22,m23,M1,M2,time1,time2,flag1,flag2,result) and (
		char m11[15]<=="nil",m12[15]<=="sign2(1,c)",m13[15]<=="nil",m21[15]<=="nil" and
		char m22[15]<=="sign2(2,c)",m23[15]<=="nil" ,result[15]<=="nil" and int flag1<==0,flag2<==0,time1,time2 and skip;
		( ( SendStr(c21,m12) ;  SendStr(c23,m12)) or skip);
		time1:=clock();
		time2:=time1;
		while(time2-time1<50 AND flag1=0)
		{
			GetStr(c12,m11);
			GetStr(c32,m13);
			if(strcmp(m11,"sign1(1,c)")=0 AND strcmp(m13,"sign3(1,c)")=0) then
			{
				flag1:=1;
				( ( SendStr(c21,m22);SendStr(c23,m22) ) or skip);
				time1:=clock();
				time2:=time1;
				while(time2-time1<50 AND flag2=0)
				{
					GetStr(c12,m21);
					GetStr(c32,m23);
					if(strcmp(m21,"sign1(2,c)")=0 AND strcmp(m23,"sign3(2,c)")=0) then
					{
						flag2:=1
					}
					else
					{
						time2:=clock()
					}
				};
				if(flag2=0)then
				{
					SendStr(c2t,"sign2(3,M1)");
					output("wait\n") and skip;
					ReceiveStr(ct2,result);
					cont2:="signed"
				}
				else
				{
					opt2:=1;
					cont2:="signed"
				}
			}
			else
			{
				time2:=clock()
			}
		};

		if(flag1=0) then      				//fails to complie M1
		{
			time1:=clock();
			time2:=time1;
			while(time2-time1<500 AND flag1=0)  //waits for an meassage from TTP, different computers have different timeouts
			{
				GetStr(ct2,result);
				if(strcmp(result,"Sttp(M1)")=0) then
				{			
					flag1:=1;
					cont2:="signed"
				}
				else
				{
					time2:=clock()
				}			
			}		
		}
		else
		{
			skip
		}
	)
};

function P3(){
	frame(m11,m12,m13,m21,m22,m23,M1,M2,time1,time2,flag1,flag2,result) and (
		char m11[15]<=="nil",m12[15]<=="nil",m13[15]<=="sign3(1,c)",m21[15]<=="nil" and
		char m22[15]<=="nil",m23[15]<=="sign3(2,c)" ,result[15]<=="nil" and int flag1<==0,flag2<==0,time1,time2 and skip;
		( ( SendStr(c31,m13) ;  SendStr(c32,m13)) or skip);
		time1:=clock();
		time2:=time1;
		while(time2-time1<50 AND flag1=0)
		{
			GetStr(c13,m11);
			GetStr(c23,m12);
			if(strcmp(m11,"sign1(1,c)")=0 AND strcmp(m12,"sign2(1,c)")=0) then
			{
				flag1:=1;
				( ( SendStr(c31,m23);SendStr(c32,m23) ) or skip);
				time1:=clock();
				time2:=time1;
				while(time2-time1<50 AND flag2=0)
				{
					GetStr(c13,m21);
					GetStr(c23,m22);
					if(strcmp(m21,"sign1(2,c)")=0 AND strcmp(m22,"sign2(2,c)")=0) then
					{
						flag2:=1
					}
					else
					{
						time2:=clock()
					}
				};
				if(flag2=0)then
				{
					SendStr(c3t,"sign3(3,M1)");
					output("wait\n") and skip;
					ReceiveStr(ct3,result);
					cont3:="signed"
				}
				else
				{
					opt3:=1;
					cont3:="signed"
				}
			}
			else
			{
				time2:=clock()
			}
		};

		if(flag1=0) then      				//fails to complie M1
		{
			time1:=clock();
			time2:=time1;
			while(time2-time1<500 AND flag1=0)  //waits for an meassage from TTP, different computers have different timeouts
			{
				GetStr(ct3,result);
				if(strcmp(result,"Sttp(M1)")=0) then
				{			
					flag1:=1;
					cont3:="signed"
				}
				else
				{
					time2:=clock()
				}			
			}		
		}
		else
		{
			skip
		}
	)
};

function TTP(){
	frame(reqA,reqB,reqC,flag,time0) and (
		char reqA[15]<=="nil",reqB[15]<=="nil",reqC[15]<=="nil" and int flag<==0,time0<==1 and skip;
		while(time0<100 AND flag=0)
		{
			GetStr(c1t,reqA);
			GetStr(c2t,reqB);
			GetStr(c3t,reqC);
			output(reqA) and skip;
			if(strcmp(reqA,"sign1(3,M1)")=0 OR strcmp(reqB,"sign2(3,M1)")=0 OR strcmp(reqC,"sign3(3,M1)")=0) then
			{
				SendStr(ct1,"Sttp(M1)");
				SendStr(ct2,"Sttp(M1)");
				SendStr(ct3,"Sttp(M1)");
				output("send over \n") and skip;
				flag:=1;
				opt:=0
			}
			else
			{
				time0:=time0+1
			}
		}	
	)
};

frame(c12,c13,c21,c23,c31,c32,c1t,c2t,c3t,ct1,ct2,ct3,cont1,cont2,cont3,opt1,opt2,opt3,opt) and (
	Chn c12[5],c13[5],c21[5],c23[5],c31[5],c32[5] ,c1t[5],c2t[5],c3t[5],ct1[5],ct2[5],ct3[5]
	and char cont1[15]<=="nil",cont2[15]<=="nil",cont3[15]<=="nil" and int opt1,opt2,opt3,opt and skip;
	P1()|| P2() || P3() || TTP()
)