</
define p1:main_A[0,0]+main_A[0,1]+main_A[0,2]+main_A[0,3]=main_A[1,0]+main_A[1,1]+main_A[1,2]+main_A[1,3];
define p2:main_A[1,0]+main_A[1,1]+main_A[1,2]+main_A[1,3]=main_A[2,0]+main_A[2,1]+main_A[2,2]+main_A[2,3];
define p3:main_A[2,0]+main_A[2,1]+main_A[2,2]+main_A[2,3]=main_A[3,0]+main_A[3,1]+main_A[3,2]+main_A[3,3];

define p4:main_A[0,0]+main_A[1,0]+main_A[2,0]+main_A[3,0]=main_A[0,1]+main_A[1,1]+main_A[2,1]+main_A[3,1];
define p5:main_A[0,1]+main_A[1,1]+main_A[2,1]+main_A[3,1]=main_A[0,2]+main_A[1,2]+main_A[2,2]+main_A[3,2];
define p6:main_A[0,2]+main_A[1,2]+main_A[2,2]+main_A[3,2]=main_A[0,3]+main_A[1,3]+main_A[2,3]+main_A[3,3];

define p7:main_A[0,0]+main_A[1,1]+main_A[2,2]+main_A[3,3]=main_A[0,3]+main_A[1,2]+main_A[2,1]+main_A[3,0];

define p8:main_A[0,0]+main_A[0,1]+main_A[0,2]+main_A[0,3]=main_A[0,0]+main_A[1,0]+main_A[2,0]+main_A[3,0];

define p9:main_A[0,0]+main_A[0,1]+main_A[0,2]+main_A[0,3]=main_A[0,0]+main_A[1,1]+main_A[2,2]+main_A[3,3];
fin(p1 and p2 and p3 and p4 and p5 and p6 and p7 and p8 and p9)
/>
     frame(main_A,main_i,main_j,main_B,main_temp) and (
     int main_A[4,4] and skip;
     int main_i<==0,main_j<==0 and skip;
     main_i:=0;
     
     while(main_i<4)
     {
         main_j:=0;
         
         while(main_j<4)
         {
             if((main_i % 2)=0) then 
             {
                 if(main_i=0) then 
                 {
                     main_A[main_i,main_j]:=main_i*4+main_j+1
                 }
                 else
                 {
                     main_A[main_i,main_j]:=(main_i+1)*4-main_j
                 }
                 
             }
             else
             {
                 if(main_i=1) then 
                 {
                     main_A[main_i,main_j]:=(main_i+1)*4-main_j
                 }
                 else
                 {
                     main_A[main_i,main_j]:=main_i*4+main_j+1
                 }
             };
             main_j:=main_j+1
             
         };
         main_i:=main_i+1
         
     };
     int main_B[2,4] and skip;
     main_i:=0;
     main_j:=0;
     main_i:=0;
     
     while(main_i<2)
     {
         main_j<==0 and skip;
         
         while(main_j<4)
         {
             main_B[main_i,main_j]:=0;
             main_j:=main_j+1
             
         };
         main_i:=main_i+1
         
     };
     main_B[0,0]:=1  or  main_B[0,1]:=1 ;
     main_B[1,0]:=1  or  main_B[1,1]:=1 ;
     main_i:=0;
     main_j:=0;
     main_i:=0;
     
     while(main_i<2)
     {
         if(main_B[main_i,0]=1) then 
         {
             main_B[main_i,3]:=1
             
         }
         else 
         {
              skip 
         };
         if(main_B[main_i,1]=1) then 
         {
             main_B[main_i,2]:=1
             
         }
         else 
         {
              skip 
         };
         main_i:=main_i+1
         
     };
     main_i:=0;
     main_j:=0;
     int main_temp and skip;
     while(main_i<2)
     {
         main_j:=0;
         
         while(main_j<4)
         {
             if(main_B[main_i,main_j]=0) then 
             {
                 main_temp:=main_A[main_i,main_j];
                 main_A[main_i,main_j]:=main_A[4-main_i-1,main_j];
                 main_A[4-main_i-1,main_j]:=main_temp
                 
             }
             else 
             {
                  skip 
             };
             main_j:=main_j+1
             
         };
         main_i:=main_i+1
     };
     main_i:=0;
     
     while(main_i<4)
     {
         main_j:=0;
         
         while(main_j<4)
         {
             output (main_A[main_i,main_j],"   ","   ") and skip;
             main_j:=main_j+1
             
         };
         output ("\n") and skip;
         main_i:=main_i+1
         
     }
     )
