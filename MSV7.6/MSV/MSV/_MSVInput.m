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
     while( (str[(F2S_j-1)]='0') )
     {
         F2S_j:=F2S_j-1
     };
     str[(F2S_j+1)]:='\0';
     F2S_j:=F2S_j+1;
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
     frame(MatCreate_i,MatCreate_2_j,return) and ( 
     int return<==0 and skip;
     int MatCreate_i and skip;
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
                 int MatCreate_2_j and skip;
                 output ("mat create fail!\n") and skip;
                 MatCreate_2_j:=0;
                 
                 while( (MatCreate_2_j<MatCreate_i) )
                 {
                     free(mat->element[MatCreate_2_j]) and skip;
                     MatCreate_2_j:=MatCreate_2_j+1
                     
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
     frame(MatDump_row,MatDump_col,MatDump_str,return) and ( 
     int return<==0 and skip;
     int MatDump_row,MatDump_col and skip;
     char MatDump_str[40] and skip;
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
                 output (F2S((mat->element[MatDump_row])[MatDump_col],MatDump_str,RValue),"\t","\t") and skip;
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
     
     while( (MatEye_i<min(mat->row,mat->col,RValue)) )
     {
         (mat->element[MatEye_i])[MatEye_i]:=1.0;
         MatEye_i:=MatEye_i+1
         
     };
     return<==1 and RValue:=mat;
     skip
     )
     }; 
  function MatSum ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatSum_row,MatSum_col,MatSum_temp,return) and ( 
     int return<==0 and skip;
     int MatSum_row,MatSum_col and skip;
     float MatSum_temp and skip;
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
         MatSum_row:=0;
         
         while( (MatSum_row<src->row) )
         {
             MatSum_temp:=0;
             MatSum_col:=0;
             
             while( (MatSum_col<src->col) )
             {
                 MatSum_temp:=MatSum_temp+(src->element[MatSum_row])[MatSum_col];
                 MatSum_col:=MatSum_col+1
                 
             };
             (dst->element[MatSum_row])[0]:=MatSum_temp;
             MatSum_row:=MatSum_row+1
             
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
  function MatMax ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatMax_row,MatMax_col,MatMax_temp,return) and ( 
     int return<==0 and skip;
     int MatMax_row,MatMax_col and skip;
     float MatMax_temp and skip;
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
         MatMax_row:=0;
         
         while( (MatMax_row<src->row) )
         {
             MatMax_temp:=(src->element[MatMax_row])[0];
             MatMax_col:=1;
             
             while( (MatMax_col<src->col) )
             {
                 if((src->element[MatMax_row])[MatMax_col]>MatMax_temp) then 
                 {
                     MatMax_temp:=(src->element[MatMax_row])[MatMax_col]
                     
                 }
                 else 
                 {
                      skip 
                 };
                 MatMax_col:=MatMax_col+1
                 
             };
             (dst->element[MatMax_row])[0]:=MatMax_temp;
             MatMax_row:=MatMax_row+1
             
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
  function MatPlus ( Mat *src,Mat *dst )
 {
     frame(MatPlus_row,MatPlus_col,return) and ( 
     int return<==0 and skip;
     int MatPlus_row,MatPlus_col and skip;
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
         MatPlus_row:=0 and MatPlus_col:=0;
         while( (MatPlus_row<dst->row) )
         {
             (dst->element[MatPlus_row])[MatPlus_col]:=1;
             MatPlus_row:=MatPlus_row+1
             
         };
         MatPlus_row:=0;
         
         while( (MatPlus_row<src->row) )
         {
             MatPlus_col:=0;
             
             while( (MatPlus_col<src->col) )
             {
                 (dst->element[MatPlus_row])[MatPlus_col+1]:=(src->element[MatPlus_row])[MatPlus_col];
                 MatPlus_col:=MatPlus_col+1
                 
             };
             MatPlus_row:=MatPlus_row+1
             
         }
     }
     else
     {
         skip
     }
     )
     }; 
  function MatMinus ( Mat *src,Mat *dst )
 {
     frame(MatMinus_row,MatMinus_col,return) and ( 
     int return<==0 and skip;
     int MatMinus_row,MatMinus_col and skip;
     if(src->row!=dst->row OR src->col!=(dst->col)+1) then 
     {
         output ("\t\terr check, unmathed matrix for MatMinus\t\t\n") and skip;
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
         MatMinus_row:=0;
         
         while( (MatMinus_row<src->row) )
         {
             MatMinus_col:=0;
             
             while( (MatMinus_col<src->col) )
             {
                 (dst->element[MatMinus_row])[MatMinus_col]:=(src->element[MatMinus_row])[MatMinus_col+1];
                 MatMinus_col:=MatMinus_col+1
                 
             };
             MatMinus_row:=MatMinus_row+1
             
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
     return<==1 and RValue:=( if(z>0) then z else a);
     skip
     )
     }; 
  function MatSoftmax ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatSoftmax_tempV,return) and ( 
     int return<==0 and skip;
     if(src->row!=dst->row OR src->col!=dst->col) then 
     {
         output ("\t\terr check, unmathed matrix for Matsofmax\t\t\n") and skip;
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
         MatMax(src,&MatSoftmax_tempV,RValue);
         MatVectorSub(src,&MatSoftmax_tempV,dst,RValue);
         MatExp(dst,dst,RValue);
         MatSum(dst,&MatSoftmax_tempV,RValue);
         MatVectorDiv(dst,&MatSoftmax_tempV,dst,RValue);
         return<==1 and RValue:=dst;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function main ( int  RValue )
 {
     frame(main_a,main_b,main_val,main_temp$_1,main_temp$_2,return) and (
     int return<==0 and skip;
     Mat main_a and skip;
     Mat main_b and skip;
     float main_val[9]<=={1.45,3.92,0.983,5.4,7.65,0.876,1.3,9.86,9.56} and skip;
     MatCreate(&main_b,3,1,RValue);
     MatCreate(&main_a,3,3,RValue);
     MatSetVal(&main_a,main_val,RValue);
     output ("原矩阵：\n") and skip;
     MatDump(&main_a);
     output ("激活后的矩阵：\n") and skip;
     Mat* main_temp$_1 and skip;
     main_temp$_1:=MatSoftmax(&main_a,&main_a,RValue);
     MatDump(main_temp$_1);
     output ("行求和矩阵：\n") and skip;
     Mat* main_temp$_2 and skip;
     main_temp$_2:=MatSum(&main_a,&main_b,RValue);
     MatDump(main_temp$_2);
     return<==1 and RValue:=0;
     skip
     )
 };
  main(RValue)
