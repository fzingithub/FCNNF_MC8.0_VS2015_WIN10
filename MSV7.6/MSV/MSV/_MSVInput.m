struct Mat {
int row,col and 
float **element 
};
 function absolute ( float a,float RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     if(a<0) then 
     {
         return<==1 and RValue:=-a;
         skip
     }
     else
     {
         return<==1 and RValue:=a;
         skip
     }
     )
     }; 
  function min ( float a,float b,float RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     if(a>b) then 
     {
         return<==1 and RValue:=b;
         skip
     }
     else
     {
         return<==1 and RValue:=a;
         skip
     }
     )
     }; 
  function equal ( float a,float b,int RValue )
 {
     frame(equal_1_temp$_1,return) and ( 
     int return<==0 and skip;
     float equal_1_temp$_1 and skip;
     equal_1_temp$_1:=absolute(a-b,RValue);
     if(equal_1_temp$_1<0.00000001) then 
     {
         return<==1 and RValue:=1;
         skip
     }
     else
     {
         return<==1 and RValue:=0;
         skip
     }
     )
     }; 
  function F2S ( float f,char *str,char* RValue )
 {
     frame(F2S_str1,F2S_j,F2S_k,F2S_i,F2S_d,return) and ( 
     int return<==0 and skip;
     char F2S_str1[40] and skip;
     int F2S_j<==0,F2S_k,F2S_i and skip;
     float F2S_d and skip;
     F2S_d:=absolute(f,RValue);
     F2S_i:=(int)F2S_d;
     while( (F2S_i>0) )
     {
         F2S_str1[F2S_j]:=F2S_i % 10+'0';
         F2S_j:=F2S_j+1;
         F2S_i:=F2S_i/ 10
     };
     F2S_k:=0;
     
     while( (F2S_k<F2S_j) )
     {
         str[F2S_k]:=F2S_str1[F2S_j-1-F2S_k];
         F2S_k:=F2S_k+1
         
     };
     if((int)F2S_d=0) then 
     {
         str[F2S_j]:='0';
         F2S_j:=F2S_j+1
         
     }
     else 
     {
          skip 
     };
     str[F2S_j]:='.';
     F2S_j:=F2S_j+1;
     F2S_d:=F2S_d-(int)F2S_d;
     F2S_i:=0;
     
     while( (F2S_i<4) )
     {
         F2S_d:=F2S_d*10;
         str[F2S_j]:=(int)F2S_d+'0';
         F2S_j:=F2S_j+1;
         F2S_d:=F2S_d-(int)F2S_d;
         F2S_i:=F2S_i+1
         
     };
     str[F2S_j]:='\0';
     if(f<0) then 
     {
         F2S_j:=0;
         while( (str[F2S_j]!='\0') )
         {
             F2S_j:=F2S_j+1
         };
         str[F2S_j+1]:='\0';
         while( (F2S_j>0) )
         {
             str[F2S_j]:=str[F2S_j-1];
             F2S_j:=F2S_j-1
         };
         str[0]:='-'
         
     }
     else 
     {
          skip 
     };
     return<==1 and RValue:=str;
     skip
     )
     }; 
  function MatCreate ( Mat *mat,int row,int col,Mat* RValue )
 {
     frame(MatCreate_i,MatCreate_3_j,return) and ( 
     int return<==0 and skip;
     int MatCreate_i and skip;
     if(row<=0 OR col<=0) then 
     {
         output ("err check, unmatch matrix for MatCreate\n") and skip;
         output ("\t\trow:\n\t\t\t") and skip;
         output (row,"\n","\n") and skip;
         output ("\t\tcol:\n\t\t\t") and skip;
         output (col,"\n","\n") and skip;
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         mat->element:=(float **)malloc(row*sizeof(float *));
         if(mat->element=NULL) then 
         {
             output ("mat create fail!\n") and skip;
             return<==1 and RValue:=NULL;
             skip
             
         }
         else 
         {
              skip 
         };
         if(return=0)   then 
         {
             MatCreate_i:=0;
             
             while( return=0 AND   (MatCreate_i<row) )
             {
                 mat->element[MatCreate_i]:=(float *)malloc(col*sizeof(float));
                 if(mat->element[MatCreate_i]=NULL) then 
                 {
                     int MatCreate_3_j and skip;
                     output ("mat create fail!\n") and skip;
                     MatCreate_3_j:=0;
                     
                     while( (MatCreate_3_j<MatCreate_i) )
                     {
                         free(mat->element[MatCreate_3_j]) and skip;
                         MatCreate_3_j:=MatCreate_3_j+1
                         
                     };
                     free(mat->element) and skip;
                     return<==1 and RValue:=NULL;
                     skip
                     
                 }
                 else 
                 {
                      skip 
                 };
                 if(return=0)   then 
                 {
                     MatCreate_i:=MatCreate_i+1
                 }
                 else
                 {
                     skip
                 }
                 
             };
             if(return=0)   then 
             {
                 mat->row:=row;
                 mat->col:=col;
                 return<==1 and RValue:=mat;
                 skip
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
     )
     }; 
  function MatDelete ( Mat *mat )
 {
     frame(MatDelete_i) and ( 
     int MatDelete_i and skip;
     MatDelete_i:=0;
     
     while( (MatDelete_i<mat->row) )
     {
         free(mat->element[MatDelete_i]) and skip;
         MatDelete_i:=MatDelete_i+1
         
     };
     free(mat->element) and skip
     )
     }; 
  function MatSetVal ( Mat *mat,float *val,Mat* RValue )
 {
     frame(MatSetVal_row,MatSetVal_col,return) and ( 
     int return<==0 and skip;
     int MatSetVal_row,MatSetVal_col and skip;
     MatSetVal_row:=0;
     
     while( (MatSetVal_row<mat->row) )
     {
         MatSetVal_col:=0;
         
         while( (MatSetVal_col<mat->col) )
         {
             (mat->element[MatSetVal_row])[MatSetVal_col]:=val[MatSetVal_col+MatSetVal_row*mat->col];
             MatSetVal_col:=MatSetVal_col+1
             
         };
         MatSetVal_row:=MatSetVal_row+1
         
     };
     return<==1 and RValue:=mat;
     skip
     )
     }; 
  function MatShape ( Mat *mat )
 {
     frame(return) and ( 
     int return<==0 and skip;
     if(mat=NULL) then 
     {
         output ("err check for MatShape\n") and skip;
          return<==1 and skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         output ("Mat ",mat->row,"x",mat->col,":\n") and skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatDump ( Mat *mat )
 {
     frame(MatDump_row,MatDump_col,MatDump_str,MatDump_data,return) and ( 
     int return<==0 and skip;
     int MatDump_row,MatDump_col and skip;
     char MatDump_str[40] and skip;
     char *MatDump_data and skip;
     if(mat=NULL) then 
     {
         output ("err check for MatDump\n") and skip;
          return<==1 and skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         output ("Mat ",mat->row,"x",mat->col,":\n") and skip;
         MatDump_row:=0;
         
         while( (MatDump_row<mat->row) )
         {
             MatDump_col:=0;
             
             while( (MatDump_col<mat->col) )
             {
                 MatDump_data:=F2S((mat->element[MatDump_row])[MatDump_col],MatDump_str,RValue);
                 if(MatDump_data[0]='-') then 
                 {
                     output (" ",F2S((mat->element[MatDump_row])[MatDump_col],MatDump_str,RValue),"\t") and skip
                     
                 }
                 else
                 {
                     output ("  ",F2S((mat->element[MatDump_row])[MatDump_col],MatDump_str,RValue),"\t") and skip
                 };
                 MatDump_col:=MatDump_col+1
                 
             };
             output ("\n") and skip;
             MatDump_row:=MatDump_row+1
             
         };
         output ("\n") and skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatAdd ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatAdd_row,MatAdd_col,return) and ( 
     int return<==0 and skip;
     int MatAdd_row,MatAdd_col and skip;
     if(!(src1->row=src2->row AND src2->row=dst->row AND src1->col=src2->col AND src2->col=dst->col)) then 
     {
         output ("\t\terr check, unmatch matrix for MatAdd\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src1);
         output ("\t\t\t") and skip;
         MatShape(src2);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatAdd_row:=0;
         
         while( (MatAdd_row<src1->row) )
         {
             MatAdd_col:=0;
             
             while( (MatAdd_col<src1->col) )
             {
                 (dst->element[MatAdd_row])[MatAdd_col]:=(src1->element[MatAdd_row])[MatAdd_col]+(src2->element[MatAdd_row])[MatAdd_col];
                 MatAdd_col:=MatAdd_col+1
                 
             };
             MatAdd_row:=MatAdd_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatSub ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatSub_row,MatSub_col,return) and ( 
     int return<==0 and skip;
     int MatSub_row,MatSub_col and skip;
     if(!(src1->row=src2->row AND src2->row=dst->row AND src1->col=src2->col AND src2->col=dst->col)) then 
     {
         output ("\t\terr check, unmatch matrix for MatSub\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src1);
         output ("\t\t\t") and skip;
         MatShape(src2);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatSub_row:=0;
         
         while( (MatSub_row<src1->row) )
         {
             MatSub_col:=0;
             
             while( (MatSub_col<src1->col) )
             {
                 (dst->element[MatSub_row])[MatSub_col]:=(src1->element[MatSub_row])[MatSub_col]-(src2->element[MatSub_row])[MatSub_col];
                 MatSub_col:=MatSub_col+1
                 
             };
             MatSub_row:=MatSub_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatMul ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatMul_row,MatMul_col,MatMul_i,MatMul_temp,return) and ( 
     int return<==0 and skip;
     int MatMul_row,MatMul_col and skip;
     int MatMul_i and skip;
     float MatMul_temp and skip;
     if(src1->col!=src2->row OR src1->row!=dst->row OR src2->col!=dst->col) then 
     {
         output ("\t\terr check, unmatch matrix for MatMul\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src1);
         output ("\t\t\t") and skip;
         MatShape(src2);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatMul_row:=0;
         
         while( (MatMul_row<dst->row) )
         {
             MatMul_col:=0;
             
             while( (MatMul_col<dst->col) )
             {
                 MatMul_temp:=0.0;
                 MatMul_i:=0;
                 
                 while( (MatMul_i<src1->col) )
                 {
                     MatMul_temp:=MatMul_temp+(src1->element[MatMul_row])[MatMul_i]*(src2->element[MatMul_i])[MatMul_col];
                     MatMul_i:=MatMul_i+1
                     
                 };
                 (dst->element[MatMul_row])[MatMul_col]:=MatMul_temp;
                 MatMul_col:=MatMul_col+1
                 
             };
             MatMul_row:=MatMul_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatProduct ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatProduct_row,MatProduct_col,return) and ( 
     int return<==0 and skip;
     int MatProduct_row,MatProduct_col and skip;
     if(!(src1->row=src2->row AND src2->row=dst->row AND src1->col=src2->col AND src2->col=dst->col)) then 
     {
         output ("\t\terr check, unmatch matrix for MatAdd\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src1);
         output ("\t\t\t") and skip;
         MatShape(src2);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatProduct_row:=0;
         
         while( (MatProduct_row<src1->row) )
         {
             MatProduct_col:=0;
             
             while( (MatProduct_col<src1->col) )
             {
                 (dst->element[MatProduct_row])[MatProduct_col]:=(src1->element[MatProduct_row])[MatProduct_col]*(src2->element[MatProduct_row])[MatProduct_col];
                 MatProduct_col:=MatProduct_col+1
                 
             };
             MatProduct_row:=MatProduct_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatNumMul ( float num,Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatNumMul_row,MatNumMul_col,return) and ( 
     int return<==0 and skip;
     int MatNumMul_row,MatNumMul_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatNumMul\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatNumMul_row:=0;
         
         while( (MatNumMul_row<src->row) )
         {
             MatNumMul_col:=0;
             
             while( (MatNumMul_col<src->col) )
             {
                 (dst->element[MatNumMul_row])[MatNumMul_col]:=num*(src->element[MatNumMul_row])[MatNumMul_col];
                 MatNumMul_col:=MatNumMul_col+1
                 
             };
             MatNumMul_row:=MatNumMul_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatNumAdd ( float num,Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatNumAdd_row,MatNumAdd_col,return) and ( 
     int return<==0 and skip;
     int MatNumAdd_row,MatNumAdd_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatNumAdd\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatNumAdd_row:=0;
         
         while( (MatNumAdd_row<src->row) )
         {
             MatNumAdd_col:=0;
             
             while( (MatNumAdd_col<src->col) )
             {
                 (dst->element[MatNumAdd_row])[MatNumAdd_col]:=num+(src->element[MatNumAdd_row])[MatNumAdd_col];
                 MatNumAdd_col:=MatNumAdd_col+1
                 
             };
             MatNumAdd_row:=MatNumAdd_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatTrans ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatTrans_row,MatTrans_col,return) and ( 
     int return<==0 and skip;
     int MatTrans_row,MatTrans_col and skip;
     if(src->row!=dst->col OR src->col!=dst->row) then 
     {
         output ("err check, unmatch matrix for MatTranspose\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatTrans_row:=0;
         
         while( (MatTrans_row<dst->row) )
         {
             MatTrans_col:=0;
             
             while( (MatTrans_col<dst->col) )
             {
                 (dst->element[MatTrans_row])[MatTrans_col]:=(src->element[MatTrans_col])[MatTrans_row];
                 MatTrans_col:=MatTrans_col+1
                 
             };
             MatTrans_row:=MatTrans_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatZeros ( Mat *mat,Mat* RValue )
 {
     frame(MatZeros_row,MatZeros_col,return) and ( 
     int return<==0 and skip;
     int MatZeros_row,MatZeros_col and skip;
     MatZeros_row:=0;
     
     while( (MatZeros_row<mat->row) )
     {
         MatZeros_col:=0;
         
         while( (MatZeros_col<mat->col) )
         {
             (mat->element[MatZeros_row])[MatZeros_col]:=0.0;
             MatZeros_col:=MatZeros_col+1
             
         };
         MatZeros_row:=MatZeros_row+1
         
     };
     return<==1 and RValue:=mat;
     skip
     )
     }; 
  function MatOnes ( Mat *mat,Mat* RValue )
 {
     frame(MatOnes_row,MatOnes_col,return) and ( 
     int return<==0 and skip;
     int MatOnes_row,MatOnes_col and skip;
     MatOnes_row:=0;
     
     while( (MatOnes_row<mat->row) )
     {
         MatOnes_col:=0;
         
         while( (MatOnes_col<mat->col) )
         {
             (mat->element[MatOnes_row])[MatOnes_col]:=1.0;
             MatOnes_col:=MatOnes_col+1
             
         };
         MatOnes_row:=MatOnes_row+1
         
     };
     return<==1 and RValue:=mat;
     skip
     )
     }; 
  function MatEye ( Mat *mat,Mat* RValue )
 {
     frame(MatEye_i,return) and ( 
     int return<==0 and skip;
     int MatEye_i and skip;
     MatZeros(mat,RValue);
     MatEye_i:=0;
     
     while( (MatEye_i<min((float)(mat->row),(float)(mat->col),RValue)) )
     {
         (mat->element[MatEye_i])[MatEye_i]:=1.0;
         MatEye_i:=MatEye_i+1
         
     };
     return<==1 and RValue:=mat;
     skip
     )
     }; 
  function MatRowSum ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatRowSum_row,MatRowSum_col,MatRowSum_temp,return) and ( 
     int return<==0 and skip;
     int MatRowSum_row,MatRowSum_col and skip;
     float MatRowSum_temp and skip;
     if(src->row!=dst->row OR dst->col!=1) then 
     {
         output ("err check, unmatch matrix for MatSum\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatRowSum_row:=0;
         
         while( (MatRowSum_row<src->row) )
         {
             MatRowSum_temp:=0;
             MatRowSum_col:=0;
             
             while( (MatRowSum_col<src->col) )
             {
                 MatRowSum_temp:=MatRowSum_temp+(src->element[MatRowSum_row])[MatRowSum_col];
                 MatRowSum_col:=MatRowSum_col+1
                 
             };
             (dst->element[MatRowSum_row])[0]:=MatRowSum_temp;
             MatRowSum_row:=MatRowSum_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatRowMax ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatRowMax_row,MatRowMax_col,MatRowMax_temp,return) and ( 
     int return<==0 and skip;
     int MatRowMax_row,MatRowMax_col and skip;
     float MatRowMax_temp and skip;
     if(src->row!=dst->row OR dst->col!=1) then 
     {
         output ("err check, unmatch matrix for MatMax\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatRowMax_row:=0;
         
         while( (MatRowMax_row<src->row) )
         {
             MatRowMax_temp:=(src->element[MatRowMax_row])[0];
             MatRowMax_col:=1;
             
             while( (MatRowMax_col<src->col) )
             {
                 if((src->element[MatRowMax_row])[MatRowMax_col]>MatRowMax_temp) then 
                 {
                     MatRowMax_temp:=(src->element[MatRowMax_row])[MatRowMax_col]
                     
                 }
                 else 
                 {
                      skip 
                 };
                 MatRowMax_col:=MatRowMax_col+1
                 
             };
             (dst->element[MatRowMax_row])[0]:=MatRowMax_temp;
             MatRowMax_row:=MatRowMax_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatSquare ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatSquare_row,MatSquare_col,return) and ( 
     int return<==0 and skip;
     int MatSquare_row,MatSquare_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("err check, unmatch matrix for MatSquare\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatSquare_row:=0;
         
         while( (MatSquare_row<src->row) )
         {
             MatSquare_col:=0;
             
             while( (MatSquare_col<src->col) )
             {
                 (dst->element[MatSquare_row])[MatSquare_col]:=(src->element[MatSquare_row])[MatSquare_col]*(src->element[MatSquare_row])[MatSquare_col];
                 MatSquare_col:=MatSquare_col+1
                 
             };
             MatSquare_row:=MatSquare_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatExp ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatExp_row,MatExp_col,return) and ( 
     int return<==0 and skip;
     int MatExp_row,MatExp_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("err check, unmatch matrix for MatExp\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatExp_row:=0;
         
         while( (MatExp_row<src->row) )
         {
             MatExp_col:=0;
             
             while( (MatExp_col<src->col) )
             {
                 (dst->element[MatExp_row])[MatExp_col]:=(exp((float)((src->element[MatExp_row])[MatExp_col])));
                 MatExp_col:=MatExp_col+1
                 
             };
             MatExp_row:=MatExp_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatVectorSub ( Mat *src,Mat *vector,Mat *dst,Mat* RValue )
 {
     frame(MatVectorSub_row,MatVectorSub_col,return) and ( 
     int return<==0 and skip;
     int MatVectorSub_row,MatVectorSub_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col OR vector->row!=src->row OR vector->col!=1) then 
     {
         output ("err check, unmatch matrix for MatVectorSub\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tvectorShape:\n\t\t\t") and skip;
         MatShape(vector);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatVectorSub_row:=0;
         
         while( (MatVectorSub_row<src->row) )
         {
             MatVectorSub_col:=0;
             
             while( (MatVectorSub_col<src->col) )
             {
                 (dst->element[MatVectorSub_row])[MatVectorSub_col]:=(src->element[MatVectorSub_row])[MatVectorSub_col]-(vector->element[MatVectorSub_row])[0];
                 MatVectorSub_col:=MatVectorSub_col+1
                 
             };
             MatVectorSub_row:=MatVectorSub_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatVectorDiv ( Mat *src,Mat *vector,Mat *dst,Mat* RValue )
 {
     frame(MatVectorDiv_row,MatVectorDiv_col,MatVectorDiv_2_temp$_1,return) and ( 
     int return<==0 and skip;
     int MatVectorDiv_row,MatVectorDiv_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col OR vector->row!=src->row OR vector->col!=1) then 
     {
         output ("err check, unmatch matrix for MatVectorSub\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tvectorShape:\n\t\t\t") and skip;
         MatShape(vector);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatVectorDiv_row:=0;
         
         while( return=0 AND   (MatVectorDiv_row<src->row) )
         {
             float MatVectorDiv_2_temp$_1 and skip;
             MatVectorDiv_2_temp$_1:=absolute((vector->element[MatVectorDiv_row])[0],RValue);
             if(MatVectorDiv_2_temp$_1<0.000001) then 
             {
                 output ("err check, Divisor vector has zero.\n") and skip;
                 return<==1 and RValue:=NULL;
                 skip
                 
             }
             else 
             {
                  skip 
             };
             if(return=0)   then 
             {
                 MatVectorDiv_row:=MatVectorDiv_row+1
             }
             else
             {
                 skip
             }
             
         };
         if(return=0)   then 
         {
             MatVectorDiv_row:=0;
             
             while( (MatVectorDiv_row<src->row) )
             {
                 MatVectorDiv_col:=0;
                 
                 while( (MatVectorDiv_col<src->col) )
                 {
                     (dst->element[MatVectorDiv_row])[MatVectorDiv_col]:=(src->element[MatVectorDiv_row])[MatVectorDiv_col]/ (vector->element[MatVectorDiv_row])[0];
                     MatVectorDiv_col:=MatVectorDiv_col+1
                     
                 };
                 MatVectorDiv_row:=MatVectorDiv_row+1
                 
             };
             return<==1 and RValue:=dst;
             skip
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
     )
     }; 
  function MatCopy ( Mat *src,Mat *dst )
 {
     frame(MatCopy_row,MatCopy_col,return) and ( 
     int return<==0 and skip;
     int MatCopy_row,MatCopy_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatCopy\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
          return<==1 and skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatCopy_row:=0;
         
         while( (MatCopy_row<src->row) )
         {
             MatCopy_col:=0;
             
             while( (MatCopy_col<src->col) )
             {
                 (dst->element[MatCopy_row])[MatCopy_col]:=(src->element[MatCopy_row])[MatCopy_col];
                 MatCopy_col:=MatCopy_col+1
                 
             };
             MatCopy_row:=MatCopy_row+1
             
         }
     }
     else
     {
         skip
     }
     )
     }; 
  function MatPlusCol ( Mat *src,Mat *dst )
 {
     frame(MatPlusCol_row,MatPlusCol_col,return) and ( 
     int return<==0 and skip;
     int MatPlusCol_row,MatPlusCol_col and skip;
     if(src->row!=dst->row OR (src->col)+1!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatPlus\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
          return<==1 and skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatPlusCol_row:=0 and MatPlusCol_col:=0;
         while( (MatPlusCol_row<dst->row) )
         {
             (dst->element[MatPlusCol_row])[MatPlusCol_col]:=1;
             MatPlusCol_row:=MatPlusCol_row+1
             
         };
         MatPlusCol_row:=0;
         
         while( (MatPlusCol_row<src->row) )
         {
             MatPlusCol_col:=0;
             
             while( (MatPlusCol_col<src->col) )
             {
                 (dst->element[MatPlusCol_row])[MatPlusCol_col+1]:=(src->element[MatPlusCol_row])[MatPlusCol_col];
                 MatPlusCol_col:=MatPlusCol_col+1
                 
             };
             MatPlusCol_row:=MatPlusCol_row+1
             
         }
     }
     else
     {
         skip
     }
     )
     }; 
  function MatPlusRow ( Mat *src,Mat *dst )
 {
     frame(MatPlusRow_row,MatPlusRow_col,return) and ( 
     int return<==0 and skip;
     int MatPlusRow_row,MatPlusRow_col and skip;
     if(src->row+1!=dst->row OR (src->col)!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatPlus\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
          return<==1 and skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatPlusRow_row:=0 and MatPlusRow_col:=0;
         while( (MatPlusRow_col<dst->col) )
         {
             (dst->element[MatPlusRow_row])[MatPlusRow_col]:=0.0;
             MatPlusRow_col:=MatPlusRow_col+1
             
         };
         MatPlusRow_row:=0;
         
         while( (MatPlusRow_row<src->row) )
         {
             MatPlusRow_col:=0;
             
             while( (MatPlusRow_col<src->col) )
             {
                 (dst->element[MatPlusRow_row+1])[MatPlusRow_col]:=(src->element[MatPlusRow_row])[MatPlusRow_col];
                 MatPlusRow_col:=MatPlusRow_col+1
                 
             };
             MatPlusRow_row:=MatPlusRow_row+1
             
         }
     }
     else
     {
         skip
     }
     )
     }; 
  function sigmoid ( float z,float RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     return<==1 and RValue:=1/ (1+exp(-z));
     skip
     )
     }; 
  function relu ( float z,float RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     return<==1 and RValue:=( if(z>0) then z else 0);
     skip
     )
     }; 
  function leakyRelu ( float z,float a,float RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     return<==1 and RValue:=( if(z<0) then a else z);
     skip
     )
     }; 
  function MatSoftmax ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatSoftmax_tempV,return) and ( 
     int return<==0 and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatSofmax\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         Mat MatSoftmax_tempV and skip;
         MatCreate(&MatSoftmax_tempV,src->row,1,RValue);
         MatRowMax(src,&MatSoftmax_tempV,RValue);
         MatVectorSub(src,&MatSoftmax_tempV,dst,RValue);
         MatExp(dst,dst,RValue);
         MatRowSum(dst,&MatSoftmax_tempV,RValue);
         MatVectorDiv(dst,&MatSoftmax_tempV,dst,RValue);
         MatDelete(&MatSoftmax_tempV);
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatSigmoid ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatSigmoid_row,MatSigmoid_col,return) and ( 
     int return<==0 and skip;
     int MatSigmoid_row,MatSigmoid_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatSigmoid\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatSigmoid_row:=0;
         
         while( (MatSigmoid_row<src->row) )
         {
             MatSigmoid_col:=0;
             
             while( (MatSigmoid_col<src->col) )
             {
                 (dst->element[MatSigmoid_row])[MatSigmoid_col]:=sigmoid((src->element[MatSigmoid_row])[MatSigmoid_col],RValue);
                 MatSigmoid_col:=MatSigmoid_col+1
                 
             };
             MatSigmoid_row:=MatSigmoid_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatTanh ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatTanh_row,MatTanh_col,return) and ( 
     int return<==0 and skip;
     int MatTanh_row,MatTanh_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatTanh\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatTanh_row:=0;
         
         while( (MatTanh_row<src->row) )
         {
             MatTanh_col:=0;
             
             while( (MatTanh_col<src->col) )
             {
                 (dst->element[MatTanh_row])[MatTanh_col]:=tanh((src->element[MatTanh_row])[MatTanh_col]);
                 MatTanh_col:=MatTanh_col+1
                 
             };
             MatTanh_row:=MatTanh_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatRelu ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatRelu_row,MatRelu_col,return) and ( 
     int return<==0 and skip;
     int MatRelu_row,MatRelu_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatRelu\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatRelu_row:=0;
         
         while( (MatRelu_row<src->row) )
         {
             MatRelu_col:=0;
             
             while( (MatRelu_col<src->col) )
             {
                 (dst->element[MatRelu_row])[MatRelu_col]:=relu((src->element[MatRelu_row])[MatRelu_col],RValue);
                 MatRelu_col:=MatRelu_col+1
                 
             };
             MatRelu_row:=MatRelu_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MatLeakyRelu ( float a,Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatLeakyRelu_row,MatLeakyRelu_col,return) and ( 
     int return<==0 and skip;
     int MatLeakyRelu_row,MatLeakyRelu_col and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for MatLeakyRelu\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatLeakyRelu_row:=0;
         
         while( (MatLeakyRelu_row<src->row) )
         {
             MatLeakyRelu_col:=0;
             
             while( (MatLeakyRelu_col<src->col) )
             {
                 (dst->element[MatLeakyRelu_row])[MatLeakyRelu_col]:=leakyRelu((src->element[MatLeakyRelu_row])[MatLeakyRelu_col],a,RValue);
                 MatLeakyRelu_col:=MatLeakyRelu_col+1
                 
             };
             MatLeakyRelu_row:=MatLeakyRelu_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function OneHot ( Mat *src,int k,Mat *dst,Mat* RValue )
 {
     frame(OneHot_row,return) and ( 
     int return<==0 and skip;
     int OneHot_row and skip;
     if(src->row!=dst->row OR src->col!=1 OR dst->col!=k) then 
     {
         output ("\t\terr check, unmathed matrix for Onehot\t\t\n") and skip;
         output ("\t\tsrcMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tThe number of class:\n\t\t\t") and skip;
         output (k,"\n","\n") and skip;
         output ("\t\tdstMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=NULL;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatZeros(dst,RValue);
         OneHot_row:=0;
         
         while( (OneHot_row<dst->row) )
         {
             (dst->element[OneHot_row])[(int)((src->element[OneHot_row])[0])]:=1.0;
             OneHot_row:=OneHot_row+1
             
         };
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function MSE ( Mat *src,Mat *dst,float RValue )
 {
     frame(MSE_row,MSE_loss,MSE_sub_square_mat,MSE_sum_row_mat,return) and ( 
     int return<==0 and skip;
     int MSE_row and skip;
     float MSE_loss<==0.0 and skip;
     Mat MSE_sub_square_mat and skip;
     Mat MSE_sum_row_mat and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for Loss Function MSE\t\t\n") and skip;
         output ("\t\tPredictoinMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tOneHotMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=-1.0;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         MatCreate(&MSE_sub_square_mat,src->row,src->col,RValue);
         MatCreate(&MSE_sum_row_mat,src->row,1,RValue);
         MatSub(src,dst,&MSE_sub_square_mat,RValue);
         MatSquare(&MSE_sub_square_mat,&MSE_sub_square_mat,RValue);
         MatRowSum(&MSE_sub_square_mat,&MSE_sum_row_mat,RValue);
         MSE_row:=0;
         
         while( (MSE_row<src->row) )
         {
             MSE_loss:=MSE_loss+(MSE_sum_row_mat.element[MSE_row])[0];
             MSE_row:=MSE_row+1
             
         };
         MSE_loss:=MSE_loss/ (float)(src->row);
         MatDelete(&MSE_sub_square_mat);
         MatDelete(&MSE_sum_row_mat);
         return<==1 and RValue:=MSE_loss;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function CrossEntropy ( Mat *src,Mat *dst,float RValue )
 {
     frame(CrossEntropy_row,CrossEntropy_col,CrossEntropy_loss,return) and ( 
     int return<==0 and skip;
     int CrossEntropy_row,CrossEntropy_col and skip;
     float CrossEntropy_loss<==0.0 and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for Loss Function CrossEntropy\t\t\n") and skip;
         output ("\t\tPredictoinMatShape:\n\t\t\t") and skip;
         MatShape(src);
         output ("\t\tOneHotMatShape:\n\t\t\t") and skip;
         MatShape(dst);
         return<==1 and RValue:=-1.0;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         CrossEntropy_row:=0;
         
         while( (CrossEntropy_row<src->row) )
         {
             CrossEntropy_col:=0;
             
             while( (CrossEntropy_col<src->col) )
             {
                 CrossEntropy_loss:=CrossEntropy_loss+-1*(dst->element[CrossEntropy_row])[CrossEntropy_col]*log((src->element[CrossEntropy_row])[CrossEntropy_col]);
                 CrossEntropy_col:=CrossEntropy_col+1
                 
             };
             CrossEntropy_row:=CrossEntropy_row+1
             
         };
         CrossEntropy_loss:=CrossEntropy_loss/ (src->row);
         return<==1 and RValue:=CrossEntropy_loss;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function gaussrand_NORMAL ( float  RValue )
 {
     frame(gaussrand_NORMAL_V1,gaussrand_NORMAL_V2,gaussrand_NORMAL_S,gaussrand_NORMAL_phase,gaussrand_NORMAL_count,gaussrand_NORMAL_X,gaussrand_NORMAL_1_2_U1,gaussrand_NORMAL_1_2_U2,gaussrand_NORMAL_1_temp_S_1,gaussrand_NORMAL_3_temp_S_2,return) and ( 
     int return<==0 and skip;
     float gaussrand_NORMAL_V1<==0.0,gaussrand_NORMAL_V2<==0.0,gaussrand_NORMAL_S<==0.0 and skip;
     int gaussrand_NORMAL_phase<==0 and skip;
     int gaussrand_NORMAL_count<==0 and skip;
     float gaussrand_NORMAL_X and skip;
     if(gaussrand_NORMAL_phase=0) then 
     {
         while( (gaussrand_NORMAL_count=0 OR (gaussrand_NORMAL_S>=1 OR gaussrand_NORMAL_S=0)) )
         {
             float gaussrand_NORMAL_1_2_U1<==(float)(rand() % 10000)/ 10000.0 and skip;
             float gaussrand_NORMAL_1_2_U2<==(float)(rand() % 10000)/ 10000.0 and skip;
             gaussrand_NORMAL_V1:=2*gaussrand_NORMAL_1_2_U1-1;
             gaussrand_NORMAL_V2:=2*gaussrand_NORMAL_1_2_U2-1;
             gaussrand_NORMAL_S:=gaussrand_NORMAL_V1*gaussrand_NORMAL_V1+gaussrand_NORMAL_V2*gaussrand_NORMAL_V2;
             gaussrand_NORMAL_count:=gaussrand_NORMAL_count+1
         };
         float gaussrand_NORMAL_1_temp_S_1 and skip;
         gaussrand_NORMAL_1_temp_S_1:=log(gaussrand_NORMAL_S);
         gaussrand_NORMAL_X:=gaussrand_NORMAL_V1*sqrt(-2*gaussrand_NORMAL_1_temp_S_1/ gaussrand_NORMAL_S)
         
     }
     else
     {
         float gaussrand_NORMAL_3_temp_S_2 and skip;
         gaussrand_NORMAL_3_temp_S_2:=log(gaussrand_NORMAL_S);
         gaussrand_NORMAL_X:=gaussrand_NORMAL_V2*sqrt(-2*gaussrand_NORMAL_3_temp_S_2/ gaussrand_NORMAL_S)
     };
     gaussrand_NORMAL_phase:=1-gaussrand_NORMAL_phase;
     return<==1 and RValue:=gaussrand_NORMAL_X;
     skip
     )
     }; 
  function gaussrand ( float mean,float stdc,float RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     return<==1 and RValue:=mean+gaussrand_NORMAL(RValue)*stdc;
     skip
     )
     }; 
  function MatInitZero ( Mat *src,Mat* RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     MatZeros(src,RValue);
     return<==1 and RValue:=src;
     skip
     )
     }; 
  function MatInitRandomNormalization ( Mat *src,Mat* RValue )
 {
     frame(MatInitRandomNormalization_temp$_1,MatInitRandomNormalization_row,MatInitRandomNormalization_col,return) and ( 
     int return<==0 and skip;
     int MatInitRandomNormalization_temp$_1 and skip;
     MatInitRandomNormalization_temp$_1:=time(NULL);
     srand((unsigned int)MatInitRandomNormalization_temp$_1) and skip;
     int MatInitRandomNormalization_row,MatInitRandomNormalization_col and skip;
     MatInitRandomNormalization_row:=0;
     
     while( (MatInitRandomNormalization_row<src->row) )
     {
         MatInitRandomNormalization_col:=0;
         
         while( (MatInitRandomNormalization_col<src->col) )
         {
             (src->element[MatInitRandomNormalization_row])[MatInitRandomNormalization_col]:=gaussrand(0.0,0.1,RValue);
             MatInitRandomNormalization_col:=MatInitRandomNormalization_col+1
             
         };
         MatInitRandomNormalization_row:=MatInitRandomNormalization_row+1
         
     };
     return<==1 and RValue:=src;
     skip
     )
     }; 
  function MatInitXavier ( Mat *src,Mat* RValue )
 {
     frame(MatInitXavier_temp$_1,MatInitXavier_row,MatInitXavier_col,return) and ( 
     int return<==0 and skip;
     int MatInitXavier_temp$_1 and skip;
     MatInitXavier_temp$_1:=time(NULL);
     srand((unsigned int)MatInitXavier_temp$_1) and skip;
     int MatInitXavier_row,MatInitXavier_col and skip;
     MatInitXavier_row:=0;
     
     while( (MatInitXavier_row<src->row) )
     {
         MatInitXavier_col:=0;
         
         while( (MatInitXavier_col<src->col) )
         {
             (src->element[MatInitXavier_row])[MatInitXavier_col]:=gaussrand(0.0,0.1,RValue)*sqrt(1.0/ src->row);
             MatInitXavier_col:=MatInitXavier_col+1
             
         };
         MatInitXavier_row:=MatInitXavier_row+1
         
     };
     return<==1 and RValue:=src;
     skip
     )
     }; 
  function MatInitHe ( Mat *src,Mat* RValue )
 {
     frame(MatInitHe_temp$_1,MatInitHe_row,MatInitHe_col,return) and ( 
     int return<==0 and skip;
     int MatInitHe_temp$_1 and skip;
     MatInitHe_temp$_1:=time(NULL);
     srand((unsigned int)MatInitHe_temp$_1) and skip;
     int MatInitHe_row,MatInitHe_col and skip;
     MatInitHe_row:=0;
     
     while( (MatInitHe_row<src->row) )
     {
         MatInitHe_col:=0;
         
         while( (MatInitHe_col<src->col) )
         {
             (src->element[MatInitHe_row])[MatInitHe_col]:=gaussrand(0.0,0.1,RValue)*sqrt(2.0/ src->row);
             MatInitHe_col:=MatInitHe_col+1
             
         };
         MatInitHe_row:=MatInitHe_row+1
         
     };
     return<==1 and RValue:=src;
     skip
     )
     }; 
  function ParaPassedIn ( int  RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     return<==1 and RValue:=0;
     skip
     )
     }; 
  function intVal2List ( int length,int *src,int *dst,int* RValue )
 {
     frame(intVal2List_i,return) and ( 
     int return<==0 and skip;
     dst:=(int *)malloc(length*sizeof(int));
     int intVal2List_i<==0 and skip;
     
     while( (intVal2List_i<length) )
     {
         dst[intVal2List_i]:=src[intVal2List_i];
         intVal2List_i:=intVal2List_i+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function SpaceCreateActi ( Mat *P_ActiMat,int N_sample,int N_hidden,int *N_layerNeuron,Mat* RValue )
 {
     frame(SpaceCreateActi_i,return) and ( 
     int return<==0 and skip;
     P_ActiMat:=(Mat *)malloc((N_hidden+2)*sizeof(Mat));
     int SpaceCreateActi_i<==0 and skip;
     
     while( (SpaceCreateActi_i<N_hidden+2) )
     {
         (P_ActiMat[SpaceCreateActi_i]).row:=N_sample;
         (P_ActiMat[SpaceCreateActi_i]).col:=N_layerNeuron[SpaceCreateActi_i];
         MatCreate(&(P_ActiMat[SpaceCreateActi_i]),N_sample,N_layerNeuron[SpaceCreateActi_i],RValue);
         MatInitZero(&(P_ActiMat[SpaceCreateActi_i]),RValue);
         SpaceCreateActi_i:=SpaceCreateActi_i+1
         
     };
     return<==1 and RValue:=P_ActiMat;
     skip
     )
     }; 
  function SpaceCreateActiPlus ( Mat *P_ActiMatPlus,int N_sample,int N_hidden,int *N_layerNeuron,Mat* RValue )
 {
     frame(SpaceCreateActiPlus_i,return) and ( 
     int return<==0 and skip;
     P_ActiMatPlus:=(Mat *)malloc((N_hidden+1)*sizeof(Mat));
     int SpaceCreateActiPlus_i<==0 and skip;
     
     while( (SpaceCreateActiPlus_i<N_hidden+1) )
     {
         (P_ActiMatPlus[SpaceCreateActiPlus_i]).row:=N_sample;
         (P_ActiMatPlus[SpaceCreateActiPlus_i]).col:=N_layerNeuron[SpaceCreateActiPlus_i]+1;
         MatCreate(&(P_ActiMatPlus[SpaceCreateActiPlus_i]),N_sample,N_layerNeuron[SpaceCreateActiPlus_i]+1,RValue);
         MatInitZero(&(P_ActiMatPlus[SpaceCreateActiPlus_i]),RValue);
         SpaceCreateActiPlus_i:=SpaceCreateActiPlus_i+1
         
     };
     return<==1 and RValue:=P_ActiMatPlus;
     skip
     )
     }; 
  function SpaceCreateSum ( Mat *P_SumMat,int N_sample,int N_hidden,int *N_layerNeuron,Mat* RValue )
 {
     frame(SpaceCreateSum_i,return) and ( 
     int return<==0 and skip;
     P_SumMat:=(Mat *)malloc((N_hidden+2)*sizeof(Mat));
     (P_SumMat[0]).row:=0;
     (P_SumMat[0]).col:=0;
     int SpaceCreateSum_i<==1 and skip;
     
     while( (SpaceCreateSum_i<N_hidden+2) )
     {
         (P_SumMat[SpaceCreateSum_i]).row:=N_sample;
         (P_SumMat[SpaceCreateSum_i]).col:=N_layerNeuron[SpaceCreateSum_i];
         MatCreate(&(P_SumMat[SpaceCreateSum_i]),N_sample,N_layerNeuron[SpaceCreateSum_i],RValue);
         MatInitZero(&(P_SumMat[SpaceCreateSum_i]),RValue);
         SpaceCreateSum_i:=SpaceCreateSum_i+1
         
     };
     return<==1 and RValue:=P_SumMat;
     skip
     )
     }; 
  function SpaceCreateWeight ( Mat *P_WeightMat,int N_hidden,int *N_layerNeuron,Mat* RValue )
 {
     frame(SpaceCreateWeight_i,return) and ( 
     int return<==0 and skip;
     P_WeightMat:=(Mat *)malloc((N_hidden+2)*sizeof(Mat));
     (P_WeightMat[0]).row:=0;
     (P_WeightMat[0]).col:=0;
     int SpaceCreateWeight_i<==1 and skip;
     
     while( (SpaceCreateWeight_i<N_hidden+2) )
     {
         (P_WeightMat[SpaceCreateWeight_i]).row:=N_layerNeuron[SpaceCreateWeight_i-1];
         (P_WeightMat[SpaceCreateWeight_i]).col:=N_layerNeuron[SpaceCreateWeight_i];
         MatCreate(&(P_WeightMat[SpaceCreateWeight_i]),N_layerNeuron[SpaceCreateWeight_i-1],N_layerNeuron[SpaceCreateWeight_i],RValue);
         MatInitZero(&(P_WeightMat[SpaceCreateWeight_i]),RValue);
         SpaceCreateWeight_i:=SpaceCreateWeight_i+1
         
     };
     return<==1 and RValue:=P_WeightMat;
     skip
     )
     }; 
  function SpaceCreateWeightBias ( Mat *P_WeightBiasMat,int N_hidden,int *N_layerNeuron,Mat* RValue )
 {
     frame(SpaceCreateWeightBias_i,return) and ( 
     int return<==0 and skip;
     P_WeightBiasMat:=(Mat *)malloc((N_hidden+2)*sizeof(Mat));
     (P_WeightBiasMat[0]).row:=0;
     (P_WeightBiasMat[0]).col:=0;
     int SpaceCreateWeightBias_i<==1 and skip;
     
     while( (SpaceCreateWeightBias_i<N_hidden+2) )
     {
         (P_WeightBiasMat[SpaceCreateWeightBias_i]).row:=N_layerNeuron[SpaceCreateWeightBias_i-1]+1;
         (P_WeightBiasMat[SpaceCreateWeightBias_i]).col:=N_layerNeuron[SpaceCreateWeightBias_i];
         MatCreate(&(P_WeightBiasMat[SpaceCreateWeightBias_i]),N_layerNeuron[SpaceCreateWeightBias_i-1]+1,N_layerNeuron[SpaceCreateWeightBias_i],RValue);
         MatInitZero(&(P_WeightBiasMat[SpaceCreateWeightBias_i]),RValue);
         SpaceCreateWeightBias_i:=SpaceCreateWeightBias_i+1
         
     };
     return<==1 and RValue:=P_WeightBiasMat;
     skip
     )
     }; 
  function SpaceCreateDelta ( Mat *P_DeltaMat,int N_sample,int N_hidden,int *N_layerNeuron,Mat* RValue )
 {
     frame(SpaceCreateDelta_i,return) and ( 
     int return<==0 and skip;
     P_DeltaMat:=(Mat *)malloc((N_hidden+2)*sizeof(Mat));
     (P_DeltaMat[0]).row:=0;
     (P_DeltaMat[0]).col:=0;
     int SpaceCreateDelta_i<==1 and skip;
     
     while( (SpaceCreateDelta_i<N_hidden+2) )
     {
         (P_DeltaMat[SpaceCreateDelta_i]).row:=N_sample;
         (P_DeltaMat[SpaceCreateDelta_i]).col:=N_layerNeuron[SpaceCreateDelta_i];
         MatCreate(&(P_DeltaMat[SpaceCreateDelta_i]),N_sample,N_layerNeuron[SpaceCreateDelta_i],RValue);
         MatInitZero(&(P_DeltaMat[SpaceCreateDelta_i]),RValue);
         SpaceCreateDelta_i:=SpaceCreateDelta_i+1
         
     };
     return<==1 and RValue:=P_DeltaMat;
     skip
     )
     }; 
  function WeightInit_ChooseWay ( Mat *Weight,int Style_initWeight )
 {
     if(Style_initWeight=0) then 
     {
         MatInitZero(Weight,RValue)
     }
     else
     {
         if(Style_initWeight=1) then 
         {
             MatInitRandomNormalization(Weight,RValue)
         }
         else
         {
             if(Style_initWeight=2) then 
             {
                 MatInitXavier(Weight,RValue)
             }
             else
             {
                 if(Style_initWeight=3) then 
                 {
                     MatInitHe(Weight,RValue)
                 }
                 else
                 {
                     output ("error for WeightInit_ChooseWay, please check Style_initWeight variable!\n") and skip
                 }
             }
         }
     }
     
 };
 function NNinit ( Mat *P_ActiMat,Mat *P_ActiMatPlus,Mat *Mat_Y,Mat *Mat_oneHot,Mat *P_WeightMat,Mat *P_WeightBiasMat,int N_out,int N_hidden,float *Xval,float *Yval,int Style_initWeight,int RValue )
 {
     frame(NNinit_i,return) and ( 
     int return<==0 and skip;
     MatSetVal(&P_ActiMat[0],Xval,RValue);
     MatPlusCol(&P_ActiMat[0],&P_ActiMatPlus[0]);
     MatSetVal(Mat_Y,Yval,RValue);
     OneHot(Mat_Y,N_out,Mat_oneHot,RValue);
     int NNinit_i<==1 and skip;
     
     while( (NNinit_i<N_hidden+2) )
     {
         WeightInit_ChooseWay(&P_WeightMat[NNinit_i],Style_initWeight);
         MatPlusRow(&P_WeightMat[NNinit_i],&P_WeightBiasMat[NNinit_i]);
         NNinit_i:=NNinit_i+1
         
     };
     return<==1 and RValue:=0;
     skip
     )
     }; 
  function NNforward ( Mat *P_ActiMat,Mat *P_ActiMatPlus,Mat *P_SumMat,Mat *P_WeightMatBias,Mat Mat_oneHot,float RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     return<==1 and RValue:=0;
     skip
     )
     }; 
  function main ( int  RValue )
 {
     frame(main_N_sample,main_D_sample,main_N_out,main_Xval,main_Yval,main_N_hidden,main_N_layerNeuron,main_Nval,main_NStr_ActiFsHidden,main_Aval,main_Nstr_LossF,main_Style_initWeight,main_P_ActiMat,main_P_ActiMatPlus,main_P_SumMat,main_P_WeightMat,main_P_WeightBiasMat,main_Mat_oneHot,main_Mat_Y,main_P_DeltaMat) and (
     int main_N_sample<==16 and skip;
     int main_D_sample<==4 and skip;
     int main_N_out<==2 and skip;
     float main_Xval[64]<=={0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,0,1,1,1,1} and skip;
     float main_Yval[16]<=={0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0} and skip;
     int main_N_hidden<==3 and skip;
     int *main_N_layerNeuron<==NULL and skip;
     int main_Nval[5]<=={4,6,10,8,2} and skip;
     main_N_layerNeuron:=intVal2List(main_N_hidden+2,main_Nval,main_N_layerNeuron,RValue);
     int *main_NStr_ActiFsHidden<==NULL and skip;
     int main_Aval[5]<=={0,3,3,3,5} and skip;
     main_NStr_ActiFsHidden:=intVal2List(main_N_hidden+2,main_Aval,main_NStr_ActiFsHidden,RValue);
     int main_Nstr_LossF<==1 and skip;
     int main_Style_initWeight<==3 and skip;
     Mat *main_P_ActiMat<==NULL and skip;
     Mat *main_P_ActiMatPlus<==NULL and skip;
     Mat *main_P_SumMat<==NULL and skip;
     Mat *main_P_WeightMat<==NULL and skip;
     Mat *main_P_WeightBiasMat<==NULL and skip;
     Mat main_Mat_oneHot and skip;
     MatCreate(&main_Mat_oneHot,main_N_sample,main_N_out,RValue);
     Mat main_Mat_Y and skip;
     MatCreate(&main_Mat_Y,main_N_sample,1,RValue);
     Mat *main_P_DeltaMat<==NULL and skip;
     main_P_ActiMat:=SpaceCreateActi(main_P_ActiMat,main_N_sample,main_N_hidden,main_N_layerNeuron,RValue);
     main_P_ActiMatPlus:=SpaceCreateActiPlus(main_P_ActiMatPlus,main_N_sample,main_N_hidden,main_N_layerNeuron,RValue);
     main_P_SumMat:=SpaceCreateSum(main_P_SumMat,main_N_sample,main_N_hidden,main_N_layerNeuron,RValue);
     main_P_WeightMat:=SpaceCreateWeight(main_P_WeightMat,main_N_hidden,main_N_layerNeuron,RValue);
     main_P_WeightBiasMat:=SpaceCreateWeightBias(main_P_WeightBiasMat,main_N_hidden,main_N_layerNeuron,RValue);
     main_P_DeltaMat:=SpaceCreateDelta(main_P_DeltaMat,main_N_sample,main_N_hidden,main_N_layerNeuron,RValue);
     NNinit(main_P_ActiMat,main_P_ActiMatPlus,&main_Mat_Y,&main_Mat_oneHot,main_P_WeightMat,main_P_WeightBiasMat,main_N_out,main_N_hidden,main_Xval,main_Yval,main_Style_initWeight,RValue);
     MatDump(&main_P_ActiMat[0]);
     MatDump(&main_P_ActiMatPlus[0]);
     MatDump(&main_Mat_Y);
     MatDump(&main_Mat_oneHot);
     MatDump(&main_P_WeightMat[1]);
     MatDump(&main_P_WeightBiasMat[1])
     )
 };
  main(RValue)
