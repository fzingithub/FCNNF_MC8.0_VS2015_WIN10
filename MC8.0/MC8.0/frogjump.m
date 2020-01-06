

     frame(stone,main_1_i,main_1_n,main_1_original_empty_pos,main_1_3_can_shift,nm_1$,break_1$,switch_1,return_1,break_1,success) and (
	 int success<==0 and skip;
	 int stone[5]<=={1,1,0,2,2} and skip;
     int break_1<==0 and skip;
     int return_1<==0 and skip;
     int main_1_i and skip;
     main_1_i:=0;
     

     int main_1_n<==0 and skip;
     
     while( return_1=0 AND  main_1_n<20)
     {
         int main_1_original_empty_pos and skip;
         
         if(stone[0]=2 AND stone[1]=2 AND stone[2]=0 AND stone[3]=1 AND stone[4]=1) then 
         {
              success:=1;
			  output("success!!!") and skip;
             return_1<==1 and skip;
             skip
             
         }
         else
         {
             main_1_i:=3 or main_1_i:=0 or main_1_i:=1 or main_1_i:=2 or main_1_i:=4;
             int main_1_3_can_shift<==0 and skip;
             break_1<==0 and skip;
             main_1_original_empty_pos:=0;
             
             while( break_1=0 AND  main_1_original_empty_pos<5)
             {
                 if(stone[main_1_original_empty_pos]=0) then 
                 {
                     break_1<==1 and skip
                  }
                 else 
                 {
                      skip 
                 };
                 if(break_1=0)   then
                 {
                     main_1_original_empty_pos:=main_1_original_empty_pos+1
                 }
                 else
                 {
                     skip
                 }
                 
             };
             break_1<==0 and skip;
             int switch_1 and skip;
             int break_1$ and skip;
             break_1$<==0 and skip;
              switch_1<==0 and skip;
              int nm_1$ and skip;
             nm_1$ := stone[main_1_i];
             if (nm_1$=1 OR (switch_1=1 AND break_1$=0 AND return_1=0) ) then 
             {
                 switch_1<==1 and skip;
                 if(main_1_original_empty_pos>main_1_i AND main_1_original_empty_pos<=main_1_i+2) then 
                 {
                     main_1_3_can_shift:=1
                 }
                 else 
                 {
                      skip 
                 };
                 break_1$<==1 and skip
                  
             }
             else
             {
                 skip
             };
             if (nm_1$=2 OR (switch_1=1 AND break_1$=0 AND return_1=0) ) then 
             {
                 switch_1<==1 and skip;
                 if(main_1_original_empty_pos<main_1_i AND main_1_original_empty_pos>=main_1_i-2) then 
                 {
                     main_1_3_can_shift:=1
                 }
                 else 
                 {
                      skip 
                 };
                 break_1$<==1 and skip
                  
             }
             else
             {
                 skip
             };
             if (nm_1$=0 OR (switch_1=1 AND break_1$=0 AND return_1=0) ) then 
             {
                 switch_1<==1 and skip
                 
             }
             else
             {
                 skip
             };
             if(break_1$=0 AND return_1=0) then 
             {
                 main_1_3_can_shift:=0;
                 break_1$<==1 and skip
                  
             }
             else
             {
                  skip
             };
             if(main_1_3_can_shift) then 
             {
                 
                 stone[main_1_original_empty_pos]:=stone[main_1_i];
                 stone[main_1_i]:=0
             }
             else
             {
                 return_1<==1 and skip;
                 skip
             }
         };
         if(return_1=0)  then
         {
             if(return_1=0)  then
             {
                 main_1_n:=main_1_n+1
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
         
     };
     if(return_1=0)   then 
     {
         return_1<==1 and skip;
         skip
     }
     else
     {
         skip
     }
     )

