frame(c1,c2,turn,A) and 
(
   int c1<==1 and int c2<==1 and int turn<==1 and int A<==0 and skip;
   (
     (
      while(true)
      {
         c1:=0;skip;
         while(!(c2=1))
         {
            if(turn=2)then
            { 
              c1:=1;
              await(turn=1);
              c1:=0 
            }
            else{skip}
         };
         A:=1;
         c1:=1;
         turn:=2
      } 
     )
     ||
     (
      while(true)
      {
	  
         c2:=0;skip;
         while(!(c1=1))
         {
            if(turn=1)then
            { 
              c2:=1;
              await(turn=2);
              c2:=0 
            }else{skip}
            
         };
         A:=2;
         c2:=1;
         turn:=1
      } 
     )
    )
  )
    