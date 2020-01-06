frame(state,fork) and  
(
  int state[5] and int fork[5] and skip;
  fork[1]<==1 and fork[2]<==2 and fork[3]<==1 and fork[4]<==1 and fork[0]<==2 and 
  state[0]:=1 and state[1]:=1 and state[2]:=1 and state[3]:=1 and state[4]:=1;
  (  
    (((((while(true)
    { 
    
      state[0]:=2;   //starvation
	  //第1个哲学家
      while(state[0]!=3)//waiting
      {   
        await(fork[1]=1 AND fork[0]=2); //waiting for releaseing resources
        await(state[1]!=3 AND state[4]!=3);  
        state[0]:=3    //dining 
      };
      state[0]:=1;//thinking
      (fork[1]:=2 and  fork[0]:=1) or (fork[1]:=1 and fork[0]:=2)  //release or occupy resources     
    })
    ||
    (while(true)
    { 
      state[1]:=2; 
	  //第2个哲学家
      while(state[1]!=3)//keep waiting
      {
        await(fork[1]=2 AND fork[2]=1);//等待条件
        await(state[0]!=3 AND state[2]!=3); 
        state[1]:=3    
      };
      state[1]:=1;
     (fork[1]:=1 and fork[2]:=2)or (fork[1]:=2 and fork[2]:=1)  //release or occupy resources    
    }))
    ||
    (while(true)
    { 
    
      state[2]:=2 ;
	  //第3个哲学家
      while(state[2]!=3)//keep waiting
      {
        await(fork[2]=2 AND  fork[3]=1);
        await(state[1]!=3 AND state[3]!=3);
        state[2]:=3    
      };  
      state[2]:=1;
      (fork[2]:=1 and fork[3]:=2)or (fork[2]:=2 and fork[3]:=1)//release or occupy resources
    } ))
   ||
   (while(true)
   { 
   
     state[3]:=2 ;
	 //第4个哲学家
     while(state[3]!=3)
     {
       await(fork[4]=1 AND fork[3]=2);
       await(state[2]!=3 AND state[4]!=3);
       state[3]:=3
     };
     state[3]:=1;
     (fork[4]:=2 and fork[3]:=1) or(fork[4]:=1 and fork[3]:=2)//release or occupy resources
   }))
   ||
   (while(true)
   {  
     state[4]:=2 ;
	 //第5个哲学家
     while(state[4]!=3)//keep waiting
     {
       await(fork[0]=1 AND fork[4]=2);
       await(state[0]!=3 AND state[3]!=3);   
       state[4]:=3     
     };
     state[4]:=1;
     (fork[0]:=2 and fork[4]:=1) or (fork[0]:=1 and fork[4]:=2)//release or occupy resources
   }))
 )
)