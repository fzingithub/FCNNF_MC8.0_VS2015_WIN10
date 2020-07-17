frame(Xval,Yval) and (
struct Mat {
int row,col and 
float **element 
};
struct FCLayer {
Mat ActiMat and 
Mat ActiMatPlus and 
Mat SumMat and 
Mat WeightMat and 
Mat WeightBiasMat and 
Mat DeltaMat and 
Mat NablaWbMat and 
Mat ActiFunDerivationMat and 
int NeuronNum and 
int AcitFuncNum 
};
struct FCNN {
int sampleCapacity and 
int SampleDimensionNum and 
int HiddenLayerNum and 
int WeightInitWayNum and 
FCLayer *Layer and 
Mat OnehotMat and 
int ClassificationNum and 
int LossFuncNum 
};
struct Custom {
int CompleteSampleNum and 
int TrainSampleNum and 
int TestSampleNum and 
int SampleDimensionNum and 
int HiddenLayerNum and 
int WeightInitWayNum and 
float *XValArray and 
float *YValArray and 
int *NeuronNumArray and 
int *ActiFuncNumArray and 
int ClassificationNum and 
int LossFuncNum and 
int BatchSize 
};
struct DataSet {
Mat CompleteFeatureDataSet and 
Mat CompleteLabelDataSet and 
Mat CompleteTrainFeature and 
Mat CompleteTrainLabel and 
Mat *BatchTrainFeature and 
Mat *BatchTrainLabel and 
Mat *BatchTrainLabelOneHot and 
Mat TestFeature and 
Mat TestLabel and 
Mat TestLabelOneHot and 
int CompleteSampleNum and 
int TrainSampleNum and 
int TestSampleNum and 
int SampleDimensionNum and 
int ClassificationNum and 
int BatchSize and 
int BatchNum and 
int remainder 
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
     output ("Mat ",mat->row,"x",mat->col,":\n") and skip
     
 };
 function MatDump ( Mat *mat )
 {
     frame(MatDump_row,MatDump_col,MatDump_str,MatDump_data) and ( 
     int MatDump_row,MatDump_col and skip;
     char MatDump_str[40] and skip;
     char *MatDump_data and skip;
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
  function MatAdd ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatAdd_row,MatAdd_col,return) and ( 
     int return<==0 and skip;
     int MatAdd_row,MatAdd_col and skip;
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
     )
     }; 
  function MatSub ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatSub_row,MatSub_col,return) and ( 
     int return<==0 and skip;
     int MatSub_row,MatSub_col and skip;
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
     )
     }; 
  function MatMul ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatMul_row,MatMul_col,MatMul_i,MatMul_temp,MatMul_1_temp$_1,return) and ( 
     int return<==0 and skip;
     int MatMul_row,MatMul_col and skip;
     int MatMul_i and skip;
     MatZeros(dst,RValue);
     float MatMul_temp<==0.0 and skip;
     MatMul_row:=0;
     
     while( (MatMul_row<src1->row) )
     {
         MatMul_col:=0;
         
         while( (MatMul_col<src1->col) )
         {
             MatMul_temp:=(src1->element[MatMul_row])[MatMul_col];
             int MatMul_1_temp$_1 and skip;
             MatMul_1_temp$_1:=equal(MatMul_temp,0,RValue);
             if(MatMul_1_temp$_1=0) then 
             {
                 MatMul_i:=0;
                 
                 while( (MatMul_i<src2->col) )
                 {
                     (dst->element[MatMul_row])[MatMul_i]:=(dst->element[MatMul_row])[MatMul_i]+MatMul_temp*(src2->element[MatMul_col])[MatMul_i];
                     MatMul_i:=MatMul_i+1
                     
                 }
                 
             }
             else 
             {
                  skip 
             };
             MatMul_col:=MatMul_col+1
             
         };
         MatMul_row:=MatMul_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatMul1 ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatMul1_row,MatMul1_col,MatMul1_i,MatMul1_temp,return) and ( 
     int return<==0 and skip;
     int MatMul1_row,MatMul1_col and skip;
     int MatMul1_i and skip;
     float MatMul1_temp and skip;
     MatMul1_row:=0;
     
     while( (MatMul1_row<dst->row) )
     {
         MatMul1_col:=0;
         
         while( (MatMul1_col<dst->col) )
         {
             MatMul1_temp:=0.0;
             MatMul1_i:=0;
             
             while( (MatMul1_i<src1->col) )
             {
                 MatMul1_temp:=MatMul1_temp+(src1->element[MatMul1_row])[MatMul1_i]*(src2->element[MatMul1_i])[MatMul1_col];
                 MatMul1_i:=MatMul1_i+1
                 
             };
             (dst->element[MatMul1_row])[MatMul1_col]:=MatMul1_temp;
             MatMul1_col:=MatMul1_col+1
             
         };
         MatMul1_row:=MatMul1_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatProduct ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatProduct_row,MatProduct_col,return) and ( 
     int return<==0 and skip;
     int MatProduct_row,MatProduct_col and skip;
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
     )
     }; 
  function MatDiv ( Mat *src1,Mat *src2,Mat *dst,Mat* RValue )
 {
     frame(MatDiv_row,MatDiv_col,return) and ( 
     int return<==0 and skip;
     int MatDiv_row,MatDiv_col and skip;
     MatDiv_row:=0;
     
     while( (MatDiv_row<src1->row) )
     {
         MatDiv_col:=0;
         
         while( (MatDiv_col<src1->col) )
         {
             (dst->element[MatDiv_row])[MatDiv_col]:=(src1->element[MatDiv_row])[MatDiv_col]/ (src2->element[MatDiv_row])[MatDiv_col];
             MatDiv_col:=MatDiv_col+1
             
         };
         MatDiv_row:=MatDiv_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatNumMul ( float num,Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatNumMul_row,MatNumMul_col,return) and ( 
     int return<==0 and skip;
     int MatNumMul_row,MatNumMul_col and skip;
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
     )
     }; 
  function MatNumAdd ( float num,Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatNumAdd_row,MatNumAdd_col,return) and ( 
     int return<==0 and skip;
     int MatNumAdd_row,MatNumAdd_col and skip;
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
     )
     }; 
  function MatTrans ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatTrans_row,MatTrans_col,return) and ( 
     int return<==0 and skip;
     int MatTrans_row,MatTrans_col and skip;
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
     )
     }; 
  function MatRowSum ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatRowSum_row,MatRowSum_col,MatRowSum_temp,return) and ( 
     int return<==0 and skip;
     int MatRowSum_row,MatRowSum_col and skip;
     float MatRowSum_temp and skip;
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
     )
     }; 
  function MatRowMax ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatRowMax_row,MatRowMax_col,MatRowMax_temp,return) and ( 
     int return<==0 and skip;
     int MatRowMax_row,MatRowMax_col and skip;
     float MatRowMax_temp and skip;
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
     )
     }; 
  function MatSquare ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatSquare_row,MatSquare_col,return) and ( 
     int return<==0 and skip;
     int MatSquare_row,MatSquare_col and skip;
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
     )
     }; 
  function MatSqrt ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatSqrt_row,MatSqrt_col,return) and ( 
     int return<==0 and skip;
     int MatSqrt_row,MatSqrt_col and skip;
     MatSqrt_row:=0;
     
     while( (MatSqrt_row<src->row) )
     {
         MatSqrt_col:=0;
         
         while( (MatSqrt_col<src->col) )
         {
             (dst->element[MatSqrt_row])[MatSqrt_col]:=sqrt((float)((src->element[MatSqrt_row])[MatSqrt_col]));
             MatSqrt_col:=MatSqrt_col+1
             
         };
         MatSqrt_row:=MatSqrt_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatExp ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatExp_row,MatExp_col,return) and ( 
     int return<==0 and skip;
     int MatExp_row,MatExp_col and skip;
     MatExp_row:=0;
     
     while( (MatExp_row<src->row) )
     {
         MatExp_col:=0;
         
         while( (MatExp_col<src->col) )
         {
             (dst->element[MatExp_row])[MatExp_col]:=(float)(exp((float)((src->element[MatExp_row])[MatExp_col])));
             MatExp_col:=MatExp_col+1
             
         };
         MatExp_row:=MatExp_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatVectorSub ( Mat *src,Mat *vector,Mat *dst,Mat* RValue )
 {
     frame(MatVectorSub_row,MatVectorSub_col,return) and ( 
     int return<==0 and skip;
     int MatVectorSub_row,MatVectorSub_col and skip;
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
     )
     }; 
  function MatVectorDiv ( Mat *src,Mat *vector,Mat *dst,Mat* RValue )
 {
     frame(MatVectorDiv_row,MatVectorDiv_col,return) and ( 
     int return<==0 and skip;
     int MatVectorDiv_row,MatVectorDiv_col and skip;
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
     )
     }; 
  function MatCopy ( Mat *src,Mat *dst )
 {
     frame(MatCopy_row,MatCopy_col) and ( 
     int MatCopy_row,MatCopy_col and skip;
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
     )
     }; 
  function MatPlusCol ( Mat *src,Mat *dst )
 {
     frame(MatPlusCol_row,MatPlusCol_col) and ( 
     int MatPlusCol_row,MatPlusCol_col and skip;
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
     )
     }; 
  function MatPlusRow ( Mat *src,Mat *dst )
 {
     frame(MatPlusRow_row,MatPlusRow_col) and ( 
     int MatPlusRow_row,MatPlusRow_col and skip;
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
     )
     }; 
  function sigmoid ( float z,float RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     return<==1 and RValue:=(float)(1/ (1+exp(-z)));
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
     )
     }; 
  function MatNoneActi ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatNoneActi_row,MatNoneActi_col,return) and ( 
     int return<==0 and skip;
     int MatNoneActi_row,MatNoneActi_col and skip;
     MatNoneActi_row:=0;
     
     while( (MatNoneActi_row<src->row) )
     {
         MatNoneActi_col:=0;
         
         while( (MatNoneActi_col<src->col) )
         {
             (dst->element[MatNoneActi_row])[MatNoneActi_col]:=(src->element[MatNoneActi_row])[MatNoneActi_col];
             MatNoneActi_col:=MatNoneActi_col+1
             
         };
         MatNoneActi_row:=MatNoneActi_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatSigmoid ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatSigmoid_row,MatSigmoid_col,return) and ( 
     int return<==0 and skip;
     int MatSigmoid_row,MatSigmoid_col and skip;
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
     )
     }; 
  function MatTanh ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatTanh_row,MatTanh_col,return) and ( 
     int return<==0 and skip;
     int MatTanh_row,MatTanh_col and skip;
     MatTanh_row:=0;
     
     while( (MatTanh_row<src->row) )
     {
         MatTanh_col:=0;
         
         while( (MatTanh_col<src->col) )
         {
             (dst->element[MatTanh_row])[MatTanh_col]:=(float)tanh((src->element[MatTanh_row])[MatTanh_col]);
             MatTanh_col:=MatTanh_col+1
             
         };
         MatTanh_row:=MatTanh_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatRelu ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatRelu_row,MatRelu_col,return) and ( 
     int return<==0 and skip;
     int MatRelu_row,MatRelu_col and skip;
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
     )
     }; 
  function MatLeakyRelu ( float a,Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatLeakyRelu_row,MatLeakyRelu_col,return) and ( 
     int return<==0 and skip;
     int MatLeakyRelu_row,MatLeakyRelu_col and skip;
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
     )
     }; 
  function MatDerivationSoftmax ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatDerivationSoftmax_row,MatDerivationSoftmax_col,MatDerivationSoftmax_i,return) and ( 
     int return<==0 and skip;
     int MatDerivationSoftmax_row,MatDerivationSoftmax_col,MatDerivationSoftmax_i and skip;
     MatSoftmax(src,src,RValue);
     MatZeros(dst,RValue);
     MatDerivationSoftmax_row:=0;
     
     while( (MatDerivationSoftmax_row<src->row) )
     {
         MatDerivationSoftmax_col:=0;
         
         while( (MatDerivationSoftmax_col<src->col) )
         {
             MatDerivationSoftmax_i:=0;
             
             while( (MatDerivationSoftmax_i<src->col) )
             {
                 if(MatDerivationSoftmax_i=MatDerivationSoftmax_col) then 
                 {
                     (dst->element[MatDerivationSoftmax_row])[MatDerivationSoftmax_col]:=(dst->element[MatDerivationSoftmax_row])[MatDerivationSoftmax_col]+(src->element[MatDerivationSoftmax_row])[MatDerivationSoftmax_i]*(1-(src->element[MatDerivationSoftmax_row])[MatDerivationSoftmax_col])
                     
                 }
                 else
                 {
                     (dst->element[MatDerivationSoftmax_row])[MatDerivationSoftmax_col]:=(dst->element[MatDerivationSoftmax_row])[MatDerivationSoftmax_col]+-(src->element[MatDerivationSoftmax_row])[MatDerivationSoftmax_i]*(src->element[MatDerivationSoftmax_row])[MatDerivationSoftmax_col]
                 };
                 MatDerivationSoftmax_i:=MatDerivationSoftmax_i+1
                 
             };
             MatDerivationSoftmax_col:=MatDerivationSoftmax_col+1
             
         };
         MatDerivationSoftmax_row:=MatDerivationSoftmax_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatDerivationNoneActi ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatDerivationNoneActi_row,MatDerivationNoneActi_col,return) and ( 
     int return<==0 and skip;
     int MatDerivationNoneActi_row,MatDerivationNoneActi_col and skip;
     MatDerivationNoneActi_row:=0;
     
     while( (MatDerivationNoneActi_row<src->row) )
     {
         MatDerivationNoneActi_col:=0;
         
         while( (MatDerivationNoneActi_col<src->col) )
         {
             (dst->element[MatDerivationNoneActi_row])[MatDerivationNoneActi_col]:=1.0;
             MatDerivationNoneActi_col:=MatDerivationNoneActi_col+1
             
         };
         MatDerivationNoneActi_row:=MatDerivationNoneActi_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatDerivationSigmoid ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatDerivationSigmoid_temp1Mat,MatDerivationSigmoid_temp2Mat,return) and ( 
     int return<==0 and skip;
     Mat MatDerivationSigmoid_temp1Mat and skip;
     Mat MatDerivationSigmoid_temp2Mat and skip;
     MatCreate(&MatDerivationSigmoid_temp1Mat,src->row,src->col,RValue);
     MatCreate(&MatDerivationSigmoid_temp2Mat,src->row,src->col,RValue);
     MatSigmoid(src,&MatDerivationSigmoid_temp1Mat,RValue);
     MatNumMul(-1.0,&MatDerivationSigmoid_temp1Mat,&MatDerivationSigmoid_temp2Mat,RValue);
     MatNumAdd(1.0,&MatDerivationSigmoid_temp2Mat,&MatDerivationSigmoid_temp2Mat,RValue);
     MatProduct(&MatDerivationSigmoid_temp1Mat,&MatDerivationSigmoid_temp2Mat,dst,RValue);
     MatDelete(&MatDerivationSigmoid_temp1Mat);
     MatDelete(&MatDerivationSigmoid_temp2Mat);
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatDerivationTanh ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatDerivationTanh_tempMat,return) and ( 
     int return<==0 and skip;
     Mat MatDerivationTanh_tempMat and skip;
     MatCreate(&MatDerivationTanh_tempMat,src->row,src->col,RValue);
     MatTanh(src,&MatDerivationTanh_tempMat,RValue);
     MatSquare(&MatDerivationTanh_tempMat,&MatDerivationTanh_tempMat,RValue);
     MatNumMul(-1.0,&MatDerivationTanh_tempMat,&MatDerivationTanh_tempMat,RValue);
     MatNumAdd(1.0,&MatDerivationTanh_tempMat,dst,RValue);
     MatDelete(&MatDerivationTanh_tempMat);
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatDerivationRelu ( Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatDerivationRelu_row,MatDerivationRelu_col,return) and ( 
     int return<==0 and skip;
     int MatDerivationRelu_row,MatDerivationRelu_col and skip;
     MatDerivationRelu_row:=0;
     
     while( (MatDerivationRelu_row<src->row) )
     {
         MatDerivationRelu_col:=0;
         
         while( (MatDerivationRelu_col<src->col) )
         {
             if((src->element[MatDerivationRelu_row])[MatDerivationRelu_col]>0) then 
             {
                 (dst->element[MatDerivationRelu_row])[MatDerivationRelu_col]:=1.0
                 
             }
             else
             {
                 (dst->element[MatDerivationRelu_row])[MatDerivationRelu_col]:=0.0
             };
             MatDerivationRelu_col:=MatDerivationRelu_col+1
             
         };
         MatDerivationRelu_row:=MatDerivationRelu_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function MatDerivationLeakyRelu ( float a,Mat *src,Mat *dst,Mat* RValue )
 {
     frame(MatDerivationLeakyRelu_row,MatDerivationLeakyRelu_col,return) and ( 
     int return<==0 and skip;
     int MatDerivationLeakyRelu_row,MatDerivationLeakyRelu_col and skip;
     MatDerivationLeakyRelu_row:=0;
     
     while( (MatDerivationLeakyRelu_row<src->row) )
     {
         MatDerivationLeakyRelu_col:=0;
         
         while( (MatDerivationLeakyRelu_col<src->col) )
         {
             if((src->element[MatDerivationLeakyRelu_row])[MatDerivationLeakyRelu_col]>0) then 
             {
                 (dst->element[MatDerivationLeakyRelu_row])[MatDerivationLeakyRelu_col]:=1.0
                 
             }
             else
             {
                 (dst->element[MatDerivationLeakyRelu_row])[MatDerivationLeakyRelu_col]:=a
             };
             MatDerivationLeakyRelu_col:=MatDerivationLeakyRelu_col+1
             
         };
         MatDerivationLeakyRelu_row:=MatDerivationLeakyRelu_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
     )
     }; 
  function OneHot ( Mat *src,int k,Mat *dst,Mat* RValue )
 {
     frame(OneHot_row,return) and ( 
     int return<==0 and skip;
     int OneHot_row and skip;
     MatZeros(dst,RValue);
     OneHot_row:=0;
     
     while( (OneHot_row<dst->row) )
     {
         (dst->element[OneHot_row])[(int)((src->element[OneHot_row])[0])]:=1.0;
         OneHot_row:=OneHot_row+1
         
     };
     return<==1 and RValue:=dst;
     skip
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
     )
     }; 
  function CrossEntropy ( Mat *src,Mat *dst,float RValue )
 {
     frame(CrossEntropy_row,CrossEntropy_col,CrossEntropy_loss,return) and ( 
     int return<==0 and skip;
     int CrossEntropy_row,CrossEntropy_col and skip;
     float CrossEntropy_loss<==0.0 and skip;
     CrossEntropy_row:=0;
     
     while( (CrossEntropy_row<src->row) )
     {
         CrossEntropy_col:=0;
         
         while( (CrossEntropy_col<src->col) )
         {
             CrossEntropy_loss:=CrossEntropy_loss+(float)(-1*(dst->element[CrossEntropy_row])[CrossEntropy_col]*log((src->element[CrossEntropy_row])[CrossEntropy_col]));
             CrossEntropy_col:=CrossEntropy_col+1
             
         };
         CrossEntropy_row:=CrossEntropy_row+1
         
     };
     CrossEntropy_loss:=CrossEntropy_loss/ (src->row);
     return<==1 and RValue:=CrossEntropy_loss;
     skip
     )
     }; 
  function MSEDerivative ( Mat *ActiMat,Mat *DerivativeActiMat,Mat One_hotMat,Mat* RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     return<==1 and RValue:=MatSub(ActiMat,&One_hotMat,DerivativeActiMat,RValue);
     skip
     )
     }; 
  function CrossEntropyDerivative ( Mat *ActiMat,Mat *DerivativeActiMat,Mat One_hotMat,Mat* RValue )
 {
     frame(CrossEntropyDerivative_row,CrossEntropyDerivative_col,CrossEntropyDerivative_1_2_temp$_1,return) and ( 
     int return<==0 and skip;
     int CrossEntropyDerivative_row,CrossEntropyDerivative_col and skip;
     CrossEntropyDerivative_row:=0;
     
     while( (CrossEntropyDerivative_row<ActiMat->row) )
     {
         CrossEntropyDerivative_col:=0;
         
         while( (CrossEntropyDerivative_col<ActiMat->col) )
         {
             int CrossEntropyDerivative_1_2_temp$_1 and skip;
             CrossEntropyDerivative_1_2_temp$_1:=equal((ActiMat->element[CrossEntropyDerivative_row])[CrossEntropyDerivative_col],0.0,RValue);
             if(CrossEntropyDerivative_1_2_temp$_1=1) then 
             {
                 (DerivativeActiMat->element[CrossEntropyDerivative_row])[CrossEntropyDerivative_col]:=-(One_hotMat.element[CrossEntropyDerivative_row])[CrossEntropyDerivative_col]*10000000000
                 
             }
             else
             {
                 (DerivativeActiMat->element[CrossEntropyDerivative_row])[CrossEntropyDerivative_col]:=-(One_hotMat.element[CrossEntropyDerivative_row])[CrossEntropyDerivative_col]/ (ActiMat->element[CrossEntropyDerivative_row])[CrossEntropyDerivative_col]
             };
             CrossEntropyDerivative_col:=CrossEntropyDerivative_col+1
             
         };
         CrossEntropyDerivative_row:=CrossEntropyDerivative_row+1
         
     };
     return<==1 and RValue:=DerivativeActiMat;
     skip
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
         gaussrand_NORMAL_1_temp_S_1:=(float)log(gaussrand_NORMAL_S);
         gaussrand_NORMAL_X:=gaussrand_NORMAL_V1*(float)sqrt(-2*gaussrand_NORMAL_1_temp_S_1/ gaussrand_NORMAL_S)
         
     }
     else
     {
         float gaussrand_NORMAL_3_temp_S_2 and skip;
         gaussrand_NORMAL_3_temp_S_2:=(float)log(gaussrand_NORMAL_S);
         gaussrand_NORMAL_X:=gaussrand_NORMAL_V2*(float)sqrt(-2*gaussrand_NORMAL_3_temp_S_2/ gaussrand_NORMAL_S)
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
             (src->element[MatInitXavier_row])[MatInitXavier_col]:=gaussrand(0.0,0.1,RValue)*(float)sqrt(1.0/ src->row);
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
     frame(MatInitHe_row,MatInitHe_col,return) and ( 
     int return<==0 and skip;
     srand(19950826) and skip;
     int MatInitHe_row,MatInitHe_col and skip;
     MatInitHe_row:=0;
     
     while( (MatInitHe_row<src->row) )
     {
         MatInitHe_col:=0;
         
         while( (MatInitHe_col<src->col) )
         {
             (src->element[MatInitHe_row])[MatInitHe_col]:=gaussrand(0.0,0.9,RValue)*(float)sqrt(2.0/ src->row);
             MatInitHe_col:=MatInitHe_col+1
             
         };
         MatInitHe_row:=MatInitHe_row+1
         
     };
     return<==1 and RValue:=src;
     skip
     )
     }; 
  function InitCustom ( Custom *userDefine,int RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     userDefine->CompleteSampleNum:=-1;
     userDefine->TrainSampleNum:=-1;
     userDefine->TestSampleNum:=-1;
     userDefine->SampleDimensionNum:=-1;
     userDefine->HiddenLayerNum:=-1;
     userDefine->WeightInitWayNum:=-1;
     userDefine->XValArray:=NULL;
     userDefine->YValArray:=NULL;
     userDefine->NeuronNumArray:=NULL;
     userDefine->ActiFuncNumArray:=NULL;
     userDefine->ClassificationNum:=-1;
     userDefine->LossFuncNum:=-1;
     userDefine->BatchSize:=-1;
     return<==1 and RValue:=0;
     skip
     )
     }; 
  function DumpFloatArray ( float *array,int n )
 {
     frame(DumpFloatArray_str,DumpFloatArray_i) and ( 
     char DumpFloatArray_str[40] and skip;
     int DumpFloatArray_i<==0 and skip;
     
     while( (DumpFloatArray_i<n) )
     {
         output (F2S(array[DumpFloatArray_i],DumpFloatArray_str,RValue)) and skip;
         output ("\t") and skip;
         DumpFloatArray_i:=DumpFloatArray_i+1
         
     };
     output ("\n") and skip
     )
     }; 
  function DumpIntArray ( int *array,int n )
 {
     frame(DumpIntArray_i) and ( 
     int DumpIntArray_i<==0 and skip;
     
     while( (DumpIntArray_i<n) )
     {
         output (array[DumpIntArray_i],"\t","\t") and skip;
         DumpIntArray_i:=DumpIntArray_i+1
         
     };
     output ("\n") and skip
     )
     }; 
  function DumpCustom ( Custom UserDefine,int RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     output ("==================================================================== Custom Dump =====================================================================\n") and skip;
     if(UserDefine.CompleteSampleNum=-1) then 
     {
         output ("\t\t\tCustom parameter 'TrainSampleNum' uninitialized!!!\n") and skip;
         return<==1 and RValue:=-1;
         skip
         
     }
     else 
     {
          skip 
     };
     if(return=0)   then 
     {
         output ("CompleteSampleNum:\t",UserDefine.CompleteSampleNum,"\n") and skip;
         if(UserDefine.TrainSampleNum=-1) then 
         {
             output ("\t\t\tCustom parameter 'TrainSampleNum' uninitialized!!!\n") and skip;
             return<==1 and RValue:=-1;
             skip
             
         }
         else 
         {
              skip 
         };
         if(return=0)   then 
         {
             output ("TrainSampleNum:\t\t",UserDefine.TrainSampleNum,"\n") and skip;
             if(UserDefine.TestSampleNum=-1) then 
             {
                 output ("\t\t\tCustom parameter 'TestSampleNum' uninitialized!!!\n") and skip;
                 return<==1 and RValue:=-1;
                 skip
                 
             }
             else 
             {
                  skip 
             };
             if(return=0)   then 
             {
                 output ("TestSampleNum:\t\t",UserDefine.TestSampleNum,"\n") and skip;
                 if(UserDefine.SampleDimensionNum=-1) then 
                 {
                     output ("\t\t\tCustom parameter 'SampleDimensionNum' uninitialized!!!\n") and skip;
                     return<==1 and RValue:=-1;
                     skip
                     
                 }
                 else 
                 {
                      skip 
                 };
                 if(return=0)   then 
                 {
                     output ("SampleDimensionNum:\t",UserDefine.SampleDimensionNum,"\n") and skip;
                     if(UserDefine.HiddenLayerNum=-1) then 
                     {
                         output ("\t\t\tCustom parameter 'HiddenLayerNum' uninitialized!!!\n") and skip;
                         return<==1 and RValue:=-1;
                         skip
                         
                     }
                     else 
                     {
                          skip 
                     };
                     if(return=0)   then 
                     {
                         output ("HiddenLayerNum:\t\t",UserDefine.HiddenLayerNum,"\n") and skip;
                         if(UserDefine.WeightInitWayNum=-1) then 
                         {
                             output ("\t\t\tCustom parameter 'WeightInitWayNum' uninitialized!!!\n") and skip;
                             return<==1 and RValue:=-1;
                             skip
                             
                         }
                         else 
                         {
                              skip 
                         };
                         if(return=0)   then 
                         {
                             output ("WeightInitWayNum:\t",UserDefine.WeightInitWayNum,"\n") and skip;
                             if(UserDefine.ClassificationNum=-1) then 
                             {
                                 output ("\t\t\tCustom parameter 'ClassificationNum' uninitialized!!!\n") and skip;
                                 return<==1 and RValue:=-1;
                                 skip
                                 
                             }
                             else 
                             {
                                  skip 
                             };
                             if(return=0)   then 
                             {
                                 output ("ClassificationNum:\t",UserDefine.ClassificationNum,"\n") and skip;
                                 if(UserDefine.LossFuncNum=-1) then 
                                 {
                                     output ("\t\t\tCustom parameter 'LossFuncNum' uninitialized!!!\n") and skip;
                                     return<==1 and RValue:=-1;
                                     skip
                                     
                                 }
                                 else 
                                 {
                                      skip 
                                 };
                                 if(return=0)   then 
                                 {
                                     output ("LossFuncNum:\t\t",UserDefine.LossFuncNum,"\n") and skip;
                                     if(UserDefine.BatchSize=-1) then 
                                     {
                                         output ("\t\t\tCustom parameter 'BatchSize' uninitialized!!!\n") and skip;
                                         return<==1 and RValue:=-1;
                                         skip
                                         
                                     }
                                     else 
                                     {
                                          skip 
                                     };
                                     if(return=0)   then 
                                     {
                                         output ("BatchSize:\t\t",UserDefine.BatchSize,"\n") and skip;
                                         if(UserDefine.XValArray=NULL) then 
                                         {
                                             output ("\t\t\tCustom parameter 'XValArray' uninitialized!!!\n") and skip;
                                             return<==1 and RValue:=-1;
                                             skip
                                             
                                         }
                                         else 
                                         {
                                              skip 
                                         };
                                         if(return=0)   then 
                                         {
                                             output ("XValArray:(length = ",UserDefine.CompleteSampleNum*UserDefine.SampleDimensionNum,")\t\n") and skip;
                                             if(UserDefine.YValArray=NULL) then 
                                             {
                                                 output ("\t\t\tCustom parameter 'YValArray' uninitialized!!!\n") and skip;
                                                 return<==1 and RValue:=-1;
                                                 skip
                                                 
                                             }
                                             else 
                                             {
                                                  skip 
                                             };
                                             if(return=0)   then 
                                             {
                                                 output ("YValArray:(length = ",UserDefine.CompleteSampleNum,")\t\n") and skip;
                                                 if(UserDefine.NeuronNumArray=NULL) then 
                                                 {
                                                     output ("\t\t\tCustom parameter 'NeuronNumArray' uninitialized!!!\n") and skip;
                                                     return<==1 and RValue:=-1;
                                                     skip
                                                     
                                                 }
                                                 else 
                                                 {
                                                      skip 
                                                 };
                                                 if(return=0)   then 
                                                 {
                                                     output ("NeuronNumArray:\t\n") and skip;
                                                     DumpIntArray(UserDefine.NeuronNumArray,UserDefine.HiddenLayerNum+2);
                                                     if(UserDefine.ActiFuncNumArray=NULL) then 
                                                     {
                                                         output ("\t\t\tCustom parameter 'ActiFuncNumArray' uninitialized!!!\n") and skip;
                                                         return<==1 and RValue:=-1;
                                                         skip
                                                         
                                                     }
                                                     else 
                                                     {
                                                          skip 
                                                     };
                                                     if(return=0)   then 
                                                     {
                                                         output ("ActiFuncNumArray:(include output layer Acti-Function)\t\n") and skip;
                                                         DumpIntArray(UserDefine.ActiFuncNumArray,UserDefine.HiddenLayerNum+2);
                                                         output ("================================================================= Custom Dump finish =================================================================\n") and skip;
                                                         return<==1 and RValue:=0;
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
  function LoadParaFromCustom ( Custom userDefine,DataSet *dataSet,FCNN *fcnn )
 {
     dataSet->BatchSize:=userDefine.BatchSize;
     dataSet->CompleteSampleNum:=userDefine.CompleteSampleNum;
     dataSet->SampleDimensionNum:=userDefine.SampleDimensionNum;
     dataSet->TrainSampleNum:=userDefine.TrainSampleNum;
     dataSet->TestSampleNum:=userDefine.TestSampleNum;
     dataSet->ClassificationNum:=userDefine.ClassificationNum;
     fcnn->sampleCapacity:=userDefine.BatchSize;
     fcnn->SampleDimensionNum:=userDefine.SampleDimensionNum;
     fcnn->HiddenLayerNum:=userDefine.HiddenLayerNum;
     fcnn->WeightInitWayNum:=userDefine.WeightInitWayNum;
     fcnn->ClassificationNum:=userDefine.ClassificationNum;
     fcnn->LossFuncNum:=userDefine.LossFuncNum
     
 };
 function InitDataSet ( DataSet *dataSet,int RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     dataSet->CompleteFeatureDataSet.element:=NULL;
     dataSet->CompleteLabelDataSet.element:=NULL;
     dataSet->CompleteTrainFeature.element:=NULL;
     dataSet->CompleteTrainLabel.element:=NULL;
     dataSet->BatchTrainFeature:=NULL;
     dataSet->BatchTrainLabel:=NULL;
     dataSet->BatchTrainLabelOneHot:=NULL;
     dataSet->TestFeature.element:=NULL;
     dataSet->TestLabel.element:=NULL;
     dataSet->TestLabelOneHot.element:=NULL;
     dataSet->CompleteSampleNum:=-1;
     dataSet->TrainSampleNum:=-1;
     dataSet->TestSampleNum:=-1;
     dataSet->SampleDimensionNum:=-1;
     dataSet->BatchSize:=-1;
     dataSet->BatchNum:=-1;
     dataSet->remainder:=-1;
     dataSet->ClassificationNum:=-1;
     return<==1 and RValue:=0;
     skip
     )
     }; 
  function CalculateAndLoadDataSetPara ( DataSet *dataSet )
 {
     frame(CalculateAndLoadDataSetPara_batchNum,CalculateAndLoadDataSetPara_remainder) and ( 
     int CalculateAndLoadDataSetPara_batchNum<==0 and skip;
     int CalculateAndLoadDataSetPara_remainder<==0 and skip;
     CalculateAndLoadDataSetPara_batchNum:=dataSet->TrainSampleNum/ dataSet->BatchSize;
     CalculateAndLoadDataSetPara_remainder:=dataSet->TrainSampleNum % dataSet->BatchSize;
     if(CalculateAndLoadDataSetPara_remainder!=0) then 
     {
         CalculateAndLoadDataSetPara_batchNum:=CalculateAndLoadDataSetPara_batchNum+1
         
     }
     else 
     {
          skip 
     };
     dataSet->BatchNum:=CalculateAndLoadDataSetPara_batchNum;
     dataSet->remainder:=CalculateAndLoadDataSetPara_remainder
     )
     }; 
  function CreateDataSetSpace ( DataSet *dataSet )
 {
     frame(CreateDataSetSpace_i) and ( 
     MatCreate(&dataSet->CompleteFeatureDataSet,dataSet->CompleteSampleNum,dataSet->SampleDimensionNum,RValue);
     MatZeros(&dataSet->CompleteFeatureDataSet,RValue);
     MatCreate(&dataSet->CompleteLabelDataSet,dataSet->CompleteSampleNum,1,RValue);
     MatZeros(&dataSet->CompleteLabelDataSet,RValue);
     MatCreate(&dataSet->CompleteTrainFeature,dataSet->TrainSampleNum,dataSet->SampleDimensionNum,RValue);
     MatZeros(&dataSet->CompleteTrainFeature,RValue);
     MatCreate(&dataSet->CompleteTrainLabel,dataSet->TrainSampleNum,1,RValue);
     MatZeros(&dataSet->CompleteTrainLabel,RValue);
     dataSet->BatchTrainFeature:=(Mat *)malloc(dataSet->BatchNum*sizeof(Mat));
     dataSet->BatchTrainLabel:=(Mat *)malloc(dataSet->BatchNum*sizeof(Mat));
     dataSet->BatchTrainLabelOneHot:=(Mat *)malloc(dataSet->BatchNum*sizeof(Mat));
     int CreateDataSetSpace_i<==0 and skip;
     
     while( (CreateDataSetSpace_i<dataSet->BatchNum) )
     {
         if(CreateDataSetSpace_i=dataSet->BatchNum-1 AND dataSet->remainder!=0) then 
         {
             MatCreate(&(dataSet->BatchTrainFeature)[CreateDataSetSpace_i],dataSet->remainder,dataSet->SampleDimensionNum,RValue);
             MatZeros(&(dataSet->BatchTrainFeature)[CreateDataSetSpace_i],RValue);
             MatCreate(&(dataSet->BatchTrainLabel)[CreateDataSetSpace_i],dataSet->remainder,1,RValue);
             MatZeros(&(dataSet->BatchTrainLabel)[CreateDataSetSpace_i],RValue);
             MatCreate(&(dataSet->BatchTrainLabelOneHot)[CreateDataSetSpace_i],dataSet->remainder,dataSet->ClassificationNum,RValue);
             MatZeros(&(dataSet->BatchTrainLabelOneHot)[CreateDataSetSpace_i],RValue)
             
         }
         else
         {
             MatCreate(&(dataSet->BatchTrainFeature)[CreateDataSetSpace_i],dataSet->BatchSize,dataSet->SampleDimensionNum,RValue);
             MatZeros(&(dataSet->BatchTrainFeature)[CreateDataSetSpace_i],RValue);
             MatCreate(&(dataSet->BatchTrainLabel)[CreateDataSetSpace_i],dataSet->BatchSize,1,RValue);
             MatZeros(&(dataSet->BatchTrainLabel)[CreateDataSetSpace_i],RValue);
             MatCreate(&(dataSet->BatchTrainLabelOneHot)[CreateDataSetSpace_i],dataSet->BatchSize,dataSet->ClassificationNum,RValue);
             MatZeros(&(dataSet->BatchTrainLabelOneHot)[CreateDataSetSpace_i],RValue)
         };
         CreateDataSetSpace_i:=CreateDataSetSpace_i+1
         
     };
     MatCreate(&dataSet->TestFeature,dataSet->TestSampleNum,dataSet->SampleDimensionNum,RValue);
     MatZeros(&dataSet->TestFeature,RValue);
     MatCreate(&dataSet->TestLabel,dataSet->TestSampleNum,1,RValue);
     MatZeros(&dataSet->TestLabel,RValue);
     MatCreate(&dataSet->TestLabelOneHot,dataSet->TestSampleNum,dataSet->ClassificationNum,RValue);
     MatZeros(&dataSet->TestLabelOneHot,RValue)
     )
     }; 
  function DataLoading ( Custom userDefine,DataSet *dataSet,int RValue )
 {
     frame(DataLoading_i,return) and ( 
     int return<==0 and skip;
     MatSetVal(&dataSet->CompleteFeatureDataSet,userDefine.XValArray,RValue);
     MatSetVal(&dataSet->CompleteLabelDataSet,userDefine.YValArray,RValue);
     MatSetVal(&dataSet->CompleteTrainFeature,userDefine.XValArray,RValue);
     MatSetVal(&dataSet->CompleteTrainLabel,userDefine.YValArray,RValue);
     int DataLoading_i<==0 and skip;
     
     while( (DataLoading_i<dataSet->BatchNum) )
     {
         MatSetVal(&dataSet->BatchTrainFeature[DataLoading_i],&userDefine.XValArray[DataLoading_i*dataSet->BatchSize*dataSet->SampleDimensionNum],RValue);
         MatSetVal(&dataSet->BatchTrainLabel[DataLoading_i],&userDefine.YValArray[DataLoading_i*dataSet->BatchSize],RValue);
         OneHot(&dataSet->BatchTrainLabel[DataLoading_i],dataSet->ClassificationNum,&dataSet->BatchTrainLabelOneHot[DataLoading_i],RValue);
         DataLoading_i:=DataLoading_i+1
         
     };
     MatSetVal(&dataSet->TestFeature,&userDefine.XValArray[dataSet->TrainSampleNum*dataSet->SampleDimensionNum],RValue);
     MatSetVal(&dataSet->TestLabel,&userDefine.YValArray[dataSet->TrainSampleNum],RValue);
     OneHot(&dataSet->TestLabel,dataSet->ClassificationNum,&dataSet->TestLabelOneHot,RValue);
     return<==1 and RValue:=0;
     skip
     )
     }; 
  function DatasetConstruction ( Custom userDefine,DataSet *dataSet )
 {
     CalculateAndLoadDataSetPara(dataSet);
     CreateDataSetSpace(dataSet);
     DataLoading(userDefine,dataSet,RValue)
     
 };
 function InitFCNN ( FCNN *fcnn,int RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     fcnn->sampleCapacity:=-1;
     fcnn->SampleDimensionNum:=-1;
     fcnn->HiddenLayerNum:=-1;
     fcnn->WeightInitWayNum:=-1;
     fcnn->Layer:=NULL;
     fcnn->OnehotMat.element:=NULL;
     fcnn->ClassificationNum:=-1;
     fcnn->LossFuncNum:=-1;
     return<==1 and RValue:=0;
     skip
     )
     }; 
  function InitFCLayer ( FCLayer *fclayer,int RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     fclayer->ActiMat.element:=NULL;
     fclayer->ActiMatPlus.element:=NULL;
     fclayer->SumMat.element:=NULL;
     fclayer->WeightMat.element:=NULL;
     fclayer->WeightBiasMat.element:=NULL;
     fclayer->DeltaMat.element:=NULL;
     fclayer->NablaWbMat.element:=NULL;
     fclayer->ActiFunDerivationMat.element:=NULL;
     fclayer->NeuronNum:=-1;
     fclayer->AcitFuncNum:=-1;
     return<==1 and RValue:=0;
     skip
     )
     }; 
  function SpaceCreateFCLayer ( FCNN *fcnn,FCLayer* RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     fcnn->Layer:=(FCLayer *)malloc((fcnn->HiddenLayerNum+2)*sizeof(FCLayer));
     return<==1 and RValue:=fcnn->Layer;
     skip
     )
     }; 
  function SpaceCreateActi ( FCNN *fcnn )
 {
     frame(SpaceCreateActi_i) and ( 
     int SpaceCreateActi_i<==0 and skip;
     
     while( (SpaceCreateActi_i<fcnn->HiddenLayerNum+2) )
     {
         MatCreate(&fcnn->Layer[SpaceCreateActi_i].ActiMat,fcnn->sampleCapacity,fcnn->Layer[SpaceCreateActi_i].NeuronNum,RValue);
         MatZeros(&fcnn->Layer[SpaceCreateActi_i].ActiMat,RValue);
         SpaceCreateActi_i:=SpaceCreateActi_i+1
         
     }
     )
     }; 
  function SpaceDeleteActi ( FCNN *fcnn )
 {
     frame(SpaceDeleteActi_i) and ( 
     int SpaceDeleteActi_i<==0 and skip;
     
     while( (SpaceDeleteActi_i<fcnn->HiddenLayerNum+2) )
     {
         MatDelete(&fcnn->Layer[SpaceDeleteActi_i].ActiMat);
         SpaceDeleteActi_i:=SpaceDeleteActi_i+1
         
     }
     )
     }; 
  function SpaceCreateActiPlus ( FCNN *fcnn )
 {
     frame(SpaceCreateActiPlus_i) and ( 
     int SpaceCreateActiPlus_i<==0 and skip;
     
     while( (SpaceCreateActiPlus_i<fcnn->HiddenLayerNum+2) )
     {
         MatCreate(&fcnn->Layer[SpaceCreateActiPlus_i].ActiMatPlus,fcnn->sampleCapacity,fcnn->Layer[SpaceCreateActiPlus_i].NeuronNum+1,RValue);
         MatZeros(&fcnn->Layer[SpaceCreateActiPlus_i].ActiMatPlus,RValue);
         SpaceCreateActiPlus_i:=SpaceCreateActiPlus_i+1
         
     }
     )
     }; 
  function SpaceDeleteActiPlus ( FCNN *fcnn )
 {
     frame(SpaceDeleteActiPlus_i) and ( 
     int SpaceDeleteActiPlus_i<==0 and skip;
     
     while( (SpaceDeleteActiPlus_i<fcnn->HiddenLayerNum+2) )
     {
         MatDelete(&fcnn->Layer[SpaceDeleteActiPlus_i].ActiMatPlus);
         SpaceDeleteActiPlus_i:=SpaceDeleteActiPlus_i+1
         
     }
     )
     }; 
  function SpaceCreateSum ( FCNN *fcnn )
 {
     frame(SpaceCreateSum_i) and ( 
     int SpaceCreateSum_i<==0 and skip;
     
     while( (SpaceCreateSum_i<fcnn->HiddenLayerNum+2) )
     {
         MatCreate(&fcnn->Layer[SpaceCreateSum_i].SumMat,fcnn->sampleCapacity,fcnn->Layer[SpaceCreateSum_i].NeuronNum,RValue);
         MatZeros(&fcnn->Layer[SpaceCreateSum_i].SumMat,RValue);
         SpaceCreateSum_i:=SpaceCreateSum_i+1
         
     }
     )
     }; 
  function SpaceDeleteSum ( FCNN *fcnn )
 {
     frame(SpaceDeleteSum_i) and ( 
     int SpaceDeleteSum_i<==0 and skip;
     
     while( (SpaceDeleteSum_i<fcnn->HiddenLayerNum+2) )
     {
         MatDelete(&fcnn->Layer[SpaceDeleteSum_i].SumMat);
         SpaceDeleteSum_i:=SpaceDeleteSum_i+1
         
     }
     )
     }; 
  function SpaceCreateActiFunDerivation ( FCNN *fcnn )
 {
     frame(SpaceCreateActiFunDerivation_i) and ( 
     int SpaceCreateActiFunDerivation_i<==0 and skip;
     
     while( (SpaceCreateActiFunDerivation_i<fcnn->HiddenLayerNum+2) )
     {
         MatCreate(&fcnn->Layer[SpaceCreateActiFunDerivation_i].ActiFunDerivationMat,fcnn->sampleCapacity,fcnn->Layer[SpaceCreateActiFunDerivation_i].NeuronNum,RValue);
         MatZeros(&fcnn->Layer[SpaceCreateActiFunDerivation_i].ActiFunDerivationMat,RValue);
         SpaceCreateActiFunDerivation_i:=SpaceCreateActiFunDerivation_i+1
         
     }
     )
     }; 
  function SpaceDeleteActiFunDerivation ( FCNN *fcnn )
 {
     frame(SpaceDeleteActiFunDerivation_i) and ( 
     int SpaceDeleteActiFunDerivation_i<==0 and skip;
     
     while( (SpaceDeleteActiFunDerivation_i<fcnn->HiddenLayerNum+2) )
     {
         MatDelete(&fcnn->Layer[SpaceDeleteActiFunDerivation_i].ActiFunDerivationMat);
         SpaceDeleteActiFunDerivation_i:=SpaceDeleteActiFunDerivation_i+1
         
     }
     )
     }; 
  function SpaceCreateDelta ( FCNN *fcnn )
 {
     frame(SpaceCreateDelta_i) and ( 
     int SpaceCreateDelta_i<==0 and skip;
     
     while( (SpaceCreateDelta_i<fcnn->HiddenLayerNum+2) )
     {
         MatCreate(&fcnn->Layer[SpaceCreateDelta_i].DeltaMat,fcnn->sampleCapacity,fcnn->Layer[SpaceCreateDelta_i].NeuronNum,RValue);
         MatZeros(&fcnn->Layer[SpaceCreateDelta_i].DeltaMat,RValue);
         SpaceCreateDelta_i:=SpaceCreateDelta_i+1
         
     }
     )
     }; 
  function SpaceDeleteDelta ( FCNN *fcnn )
 {
     frame(SpaceDeleteDelta_i) and ( 
     int SpaceDeleteDelta_i<==0 and skip;
     
     while( (SpaceDeleteDelta_i<fcnn->HiddenLayerNum+2) )
     {
         MatDelete(&fcnn->Layer[SpaceDeleteDelta_i].DeltaMat);
         SpaceDeleteDelta_i:=SpaceDeleteDelta_i+1
         
     }
     )
     }; 
  function SpaceCreateFCNNOneHotMat ( FCNN *fcnn )
 {
     MatCreate(&fcnn->OnehotMat,fcnn->sampleCapacity,fcnn->ClassificationNum,RValue)
     
 };
 function SpaceDeleteFCNNOneHotMat ( FCNN *fcnn )
 {
     MatDelete(&fcnn->OnehotMat)
     
 };
 function SpaceCreateWeight ( FCNN *fcnn )
 {
     frame(SpaceCreateWeight_i) and ( 
     fcnn->Layer[0].WeightMat.row:=0;
     fcnn->Layer[0].WeightMat.col:=0;
     int SpaceCreateWeight_i<==1 and skip;
     
     while( (SpaceCreateWeight_i<fcnn->HiddenLayerNum+2) )
     {
         MatCreate(&fcnn->Layer[SpaceCreateWeight_i].WeightMat,fcnn->Layer[SpaceCreateWeight_i-1].NeuronNum,fcnn->Layer[SpaceCreateWeight_i].NeuronNum,RValue);
         MatZeros(&fcnn->Layer[SpaceCreateWeight_i].WeightMat,RValue);
         SpaceCreateWeight_i:=SpaceCreateWeight_i+1
         
     }
     )
     }; 
  function SpaceCreateWeightBias ( FCNN *fcnn )
 {
     frame(SpaceCreateWeightBias_i) and ( 
     fcnn->Layer[0].WeightBiasMat.row:=0;
     fcnn->Layer[0].WeightBiasMat.col:=0;
     int SpaceCreateWeightBias_i<==1 and skip;
     
     while( (SpaceCreateWeightBias_i<fcnn->HiddenLayerNum+2) )
     {
         MatCreate(&fcnn->Layer[SpaceCreateWeightBias_i].WeightBiasMat,fcnn->Layer[SpaceCreateWeightBias_i-1].NeuronNum+1,fcnn->Layer[SpaceCreateWeightBias_i].NeuronNum,RValue);
         MatZeros(&fcnn->Layer[SpaceCreateWeightBias_i].WeightBiasMat,RValue);
         SpaceCreateWeightBias_i:=SpaceCreateWeightBias_i+1
         
     }
     )
     }; 
  function SpaceCreateNablaWeightBias ( FCNN *fcnn )
 {
     frame(SpaceCreateNablaWeightBias_i) and ( 
     fcnn->Layer[0].NablaWbMat.row:=0;
     fcnn->Layer[0].NablaWbMat.col:=0;
     int SpaceCreateNablaWeightBias_i<==1 and skip;
     
     while( (SpaceCreateNablaWeightBias_i<fcnn->HiddenLayerNum+2) )
     {
         MatCreate(&fcnn->Layer[SpaceCreateNablaWeightBias_i].NablaWbMat,fcnn->Layer[SpaceCreateNablaWeightBias_i-1].NeuronNum+1,fcnn->Layer[SpaceCreateNablaWeightBias_i].NeuronNum,RValue);
         MatZeros(&fcnn->Layer[SpaceCreateNablaWeightBias_i].NablaWbMat,RValue);
         SpaceCreateNablaWeightBias_i:=SpaceCreateNablaWeightBias_i+1
         
     }
     )
     }; 
  function CreateNNOperationSpace ( FCNN *fcnn )
 {
     SpaceCreateActi(fcnn);
     SpaceCreateActiPlus(fcnn);
     SpaceCreateSum(fcnn);
     SpaceCreateActiFunDerivation(fcnn);
     SpaceCreateDelta(fcnn);
     SpaceCreateFCNNOneHotMat(fcnn)
     
 };
 function DeleteNNOperationSpace ( FCNN *fcnn )
 {
     SpaceDeleteActi(fcnn);
     SpaceDeleteActiPlus(fcnn);
     SpaceDeleteSum(fcnn);
     SpaceDeleteActiFunDerivation(fcnn);
     SpaceDeleteDelta(fcnn);
     SpaceDeleteFCNNOneHotMat(fcnn)
     
 };
 function CreateNNParaSpace ( FCNN *fcnn )
 {
     SpaceCreateWeight(fcnn);
     SpaceCreateWeightBias(fcnn);
     SpaceCreateNablaWeightBias(fcnn)
     
 };
 function DoadinPara2FCLayer ( FCNN *fcnn,Custom userDefine )
 {
     frame(DoadinPara2FCLayer_i) and ( 
     int DoadinPara2FCLayer_i<==0 and skip;
     
     while( (DoadinPara2FCLayer_i<fcnn->HiddenLayerNum+2) )
     {
         fcnn->Layer[DoadinPara2FCLayer_i].AcitFuncNum:=userDefine.ActiFuncNumArray[DoadinPara2FCLayer_i];
         fcnn->Layer[DoadinPara2FCLayer_i].NeuronNum:=userDefine.NeuronNumArray[DoadinPara2FCLayer_i];
         DoadinPara2FCLayer_i:=DoadinPara2FCLayer_i+1
         
     }
     )
     }; 
  function CreateNNSpaceAndLoadinPara2FCLayer ( FCNN *fcnn,Custom userDefine,int RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     fcnn->Layer:=SpaceCreateFCLayer(fcnn,RValue);
     DoadinPara2FCLayer(fcnn,userDefine);
     CreateNNOperationSpace(fcnn);
     CreateNNParaSpace(fcnn);
     return<==1 and RValue:=0;
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
 function NNWeightinit ( FCNN *fcnn,int RValue )
 {
     frame(NNWeightinit_i,return) and ( 
     int return<==0 and skip;
     int NNWeightinit_i<==1 and skip;
     
     while( (NNWeightinit_i<fcnn->HiddenLayerNum+2) )
     {
         WeightInit_ChooseWay(&fcnn->Layer[NNWeightinit_i].WeightMat,fcnn->WeightInitWayNum);
         MatPlusRow(&fcnn->Layer[NNWeightinit_i].WeightMat,&fcnn->Layer[NNWeightinit_i].WeightBiasMat);
         NNWeightinit_i:=NNWeightinit_i+1
         
     };
     return<==1 and RValue:=0;
     skip
     )
     }; 
  function MatActivate ( Mat *src,Mat *dst,int way,Mat* RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     if(way=0) then 
     {
         MatNoneActi(src,dst,RValue)
         
     }
     else
     {
         if(way=1) then 
         {
             MatSigmoid(src,dst,RValue)
         }
         else
         {
             if(way=2) then 
             {
                 MatTanh(src,dst,RValue)
             }
             else
             {
                 if(way=3) then 
                 {
                     MatRelu(src,dst,RValue)
                 }
                 else
                 {
                     if(way=4) then 
                     {
                         MatLeakyRelu(0.2,src,dst,RValue)
                     }
                     else
                     {
                         if(way=5) then 
                         {
                             MatSoftmax(src,dst,RValue)
                         }
                         else
                         {
                             output ("error for MatActivate, please check ActiFsHidden  variable!\n") and skip
                         }
                     }
                 }
             }
         }
     };
     return<==1 and RValue:=NULL;
     skip
     )
     }; 
  function LossFunction ( Mat *src,Mat *dst,int Nstr_LossF,float RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     if(Nstr_LossF=0) then 
     {
         return<==1 and RValue:=MSE(src,dst,RValue);
         skip
     }
     else
     {
         if(Nstr_LossF=1) then 
         {
             return<==1 and RValue:=CrossEntropy(src,dst,RValue);
             skip
         }
         else
         {
 output ("error for Nstr_LossF, please check loss function variable!\n") and skip;
 return<==1 and RValue:=-1;
 skip
 }
 }
 )
 }; 
  function NNforward ( Mat featureMat,Mat labelMatOneHot,FCNN *fcnn,float RValue )
 {
     frame(NNforward_i,NNforward_loss,return) and ( 
     int return<==0 and skip;
     if(featureMat.row!=fcnn->sampleCapacity) then 
     {
         fcnn->sampleCapacity:=featureMat.row;
         DeleteNNOperationSpace(fcnn);
         CreateNNOperationSpace(fcnn)
         
     }
     else 
     {
          skip 
     };
     MatCopy(&featureMat,&fcnn->Layer[0].ActiMat);
     MatPlusCol(&fcnn->Layer[0].ActiMat,&fcnn->Layer[0].ActiMatPlus);
     MatCopy(&labelMatOneHot,&fcnn->OnehotMat);
     int NNforward_i<==0 and skip;
     
     while( (NNforward_i<fcnn->HiddenLayerNum+1) )
     {
         MatMul(&fcnn->Layer[NNforward_i].ActiMatPlus,&fcnn->Layer[NNforward_i+1].WeightBiasMat,&fcnn->Layer[NNforward_i+1].SumMat,RValue);
         MatActivate(&fcnn->Layer[NNforward_i+1].SumMat,&fcnn->Layer[NNforward_i+1].ActiMat,fcnn->Layer[NNforward_i+1].AcitFuncNum,RValue);
         if(NNforward_i!=fcnn->HiddenLayerNum) then 
         {
             MatPlusCol(&fcnn->Layer[NNforward_i+1].ActiMat,&fcnn->Layer[NNforward_i+1].ActiMatPlus)
             
         }
         else 
         {
              skip 
         };
         NNforward_i:=NNforward_i+1
         
     };
     float NNforward_loss<==-1.0 and skip;
     NNforward_loss:=LossFunction(&fcnn->Layer[fcnn->HiddenLayerNum+1].ActiMat,&fcnn->OnehotMat,fcnn->LossFuncNum,RValue);
     return<==1 and RValue:=NNforward_loss;
     skip
     )
     }; 
  function ActiFunDerivation ( Mat Mat_Sum,Mat *Mat_ActiFunDerivation,int option,Mat* RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     if(option=0) then 
     {
         return<==1 and RValue:=MatDerivationNoneActi(&Mat_Sum,Mat_ActiFunDerivation,RValue);
         skip
         
     }
     else
     {
         if(option=1) then 
         {
             return<==1 and RValue:=MatDerivationSigmoid(&Mat_Sum,Mat_ActiFunDerivation,RValue);
             skip
         }
         else
         {
             if(option=2) then 
             {
                 return<==1 and RValue:=MatDerivationTanh(&Mat_Sum,Mat_ActiFunDerivation,RValue);
                 skip
             }
             else
             {
                 if(option=3) then 
                 {
                     return<==1 and RValue:=MatDerivationRelu(&Mat_Sum,Mat_ActiFunDerivation,RValue);
                     skip
                 }
                 else
                 {
                     if(option=4) then 
                     {
                         return<==1 and RValue:=MatDerivationLeakyRelu(0.1,&Mat_Sum,Mat_ActiFunDerivation,RValue);
                         skip
                     }
                     else
                     {
                         if(option=5) then 
                         {
                             return<==1 and RValue:=MatDerivationSoftmax(&Mat_Sum,Mat_ActiFunDerivation,RValue);
                             skip
                         }
                         else
                         {
                             output ("error for ActiFunDerivation, please check ActiFsHidden  variable!\n") and skip
                         }
                     }
                 }
             }
         }
     };
     if(return=0)  then
     {
         if(return=0)  then
         {
             return<==1 and RValue:=NULL;
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
  function LossFunDerivation ( Mat *ActiMat,Mat *DerivativeActiMat,Mat One_hotMat,int option,Mat* RValue )
 {
     frame(return) and ( 
     int return<==0 and skip;
     if(option=0) then 
     {
         return<==1 and RValue:=MSEDerivative(ActiMat,DerivativeActiMat,One_hotMat,RValue);
         skip
         
     }
     else
     {
         if(option=1) then 
         {
             return<==1 and RValue:=CrossEntropyDerivative(ActiMat,DerivativeActiMat,One_hotMat,RValue);
             skip
         }
         else
         {
             output ("error for LossFunDerivation, please check Nstr_LossF  variable!\n") and skip
         }
     };
     if(return=0)  then
     {
         if(return=0)  then
         {
             return<==1 and RValue:=NULL;
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
  function NNOuputLayerBackward ( FCNN *fcnn,Mat* RValue )
 {
     frame(NNOuputLayerBackward_2_tempMat,NNOuputLayerBackward_ActiPlusTrans,return) and ( 
     int return<==0 and skip;
     if(fcnn->Layer[fcnn->HiddenLayerNum+1].AcitFuncNum=5 AND fcnn->LossFuncNum=1) then 
     {
         MatSub(&fcnn->Layer[fcnn->HiddenLayerNum+1].ActiMat,&fcnn->OnehotMat,&fcnn->Layer[fcnn->HiddenLayerNum+1].DeltaMat,RValue)
         
     }
     else
     {
         Mat NNOuputLayerBackward_2_tempMat and skip;
         MatCreate(&NNOuputLayerBackward_2_tempMat,fcnn->sampleCapacity,fcnn->Layer[fcnn->HiddenLayerNum+1].NeuronNum,RValue);
         LossFunDerivation(&fcnn->Layer[fcnn->HiddenLayerNum+1].ActiMat,&NNOuputLayerBackward_2_tempMat,fcnn->OnehotMat,fcnn->LossFuncNum,RValue);
         ActiFunDerivation(fcnn->Layer[fcnn->HiddenLayerNum+1].SumMat,&fcnn->Layer[fcnn->HiddenLayerNum+1].ActiFunDerivationMat,fcnn->Layer[fcnn->HiddenLayerNum+1].AcitFuncNum,RValue);
         MatProduct(&fcnn->Layer[fcnn->HiddenLayerNum+1].ActiMat,&fcnn->Layer[fcnn->HiddenLayerNum+1].ActiFunDerivationMat,&fcnn->Layer[fcnn->HiddenLayerNum+1].DeltaMat,RValue);
         MatDelete(&NNOuputLayerBackward_2_tempMat)
     };
     Mat NNOuputLayerBackward_ActiPlusTrans and skip;
     MatCreate(&NNOuputLayerBackward_ActiPlusTrans,fcnn->Layer[fcnn->HiddenLayerNum].NeuronNum+1,fcnn->sampleCapacity,RValue);
     MatTrans(&fcnn->Layer[fcnn->HiddenLayerNum].ActiMatPlus,&NNOuputLayerBackward_ActiPlusTrans,RValue);
     MatMul(&NNOuputLayerBackward_ActiPlusTrans,&fcnn->Layer[fcnn->HiddenLayerNum+1].DeltaMat,&fcnn->Layer[fcnn->HiddenLayerNum+1].NablaWbMat,RValue);
     MatNumMul(1.0/ fcnn->sampleCapacity,&fcnn->Layer[fcnn->HiddenLayerNum+1].NablaWbMat,&fcnn->Layer[fcnn->HiddenLayerNum+1].NablaWbMat,RValue);
     MatDelete(&NNOuputLayerBackward_ActiPlusTrans);
     return<==1 and RValue:=NULL;
     skip
     )
     }; 
  function NNBackward ( FCNN *fcnn,Mat* RValue )
 {
     frame(NNBackward_i,NNBackward_tempTransW,NNBackward_ActiFuncMat,NNBackward_tempMulMat,NNBackward_tempProdMat,NNBackward_tempTransActi,return) and ( 
     int return<==0 and skip;
     NNOuputLayerBackward(fcnn,RValue);
     int NNBackward_i<==fcnn->HiddenLayerNum and skip;
     
     while( (NNBackward_i>0) )
     {
         Mat NNBackward_tempTransW and skip;
         Mat NNBackward_ActiFuncMat and skip;
         Mat NNBackward_tempMulMat and skip;
         Mat NNBackward_tempProdMat and skip;
         Mat NNBackward_tempTransActi and skip;
         MatCreate(&NNBackward_tempTransW,fcnn->Layer[NNBackward_i+1].WeightMat.col,fcnn->Layer[NNBackward_i+1].WeightMat.row,RValue);
         MatCreate(&NNBackward_ActiFuncMat,fcnn->Layer[NNBackward_i].SumMat.row,fcnn->Layer[NNBackward_i].SumMat.col,RValue);
         MatCreate(&NNBackward_tempMulMat,fcnn->Layer[NNBackward_i+1].DeltaMat.row,fcnn->Layer[NNBackward_i+1].WeightMat.row,RValue);
         MatCreate(&NNBackward_tempProdMat,fcnn->Layer[NNBackward_i].SumMat.row,fcnn->Layer[NNBackward_i].SumMat.col,RValue);
         MatCreate(&NNBackward_tempTransActi,fcnn->Layer[NNBackward_i-1].ActiMatPlus.col,fcnn->Layer[NNBackward_i-1].ActiMatPlus.row,RValue);
         MatTrans(&fcnn->Layer[NNBackward_i+1].WeightMat,&NNBackward_tempTransW,RValue);
         ActiFunDerivation(fcnn->Layer[NNBackward_i].SumMat,&NNBackward_ActiFuncMat,fcnn->Layer[NNBackward_i].AcitFuncNum,RValue);
         MatMul(&fcnn->Layer[NNBackward_i+1].DeltaMat,&NNBackward_tempTransW,&NNBackward_tempMulMat,RValue);
         MatProduct(&NNBackward_tempMulMat,&NNBackward_ActiFuncMat,&fcnn->Layer[NNBackward_i].DeltaMat,RValue);
         MatTrans(&fcnn->Layer[NNBackward_i-1].ActiMatPlus,&NNBackward_tempTransActi,RValue);
         MatMul(&NNBackward_tempTransActi,&fcnn->Layer[NNBackward_i].DeltaMat,&fcnn->Layer[NNBackward_i].NablaWbMat,RValue);
         MatNumMul(1.0/ fcnn->sampleCapacity,&fcnn->Layer[NNBackward_i].NablaWbMat,&fcnn->Layer[NNBackward_i].NablaWbMat,RValue);
         MatDelete(&NNBackward_tempTransW);
         MatDelete(&NNBackward_ActiFuncMat);
         MatDelete(&NNBackward_tempMulMat);
         MatDelete(&NNBackward_tempProdMat);
         MatDelete(&NNBackward_tempTransActi);
         NNBackward_i:=NNBackward_i-1
         
     };
     return<==1 and RValue:=NULL;
     skip
     )
     }; 
  function MBGD ( FCNN *fcnn,float alpha )
 {
     frame(MBGD_temp,MBGD_i) and ( 
     Mat MBGD_temp and skip;
     int MBGD_i<==1 and skip;
     
     while( (MBGD_i<=fcnn->HiddenLayerNum+1) )
     {
         MatCreate(&MBGD_temp,fcnn->Layer[MBGD_i].NablaWbMat.row,fcnn->Layer[MBGD_i].NablaWbMat.col,RValue);
         MatNumMul(alpha,&fcnn->Layer[MBGD_i].NablaWbMat,&MBGD_temp,RValue);
         MatSub(&fcnn->Layer[MBGD_i].WeightBiasMat,&MBGD_temp,&fcnn->Layer[MBGD_i].WeightBiasMat,RValue);
         MatDelete(&MBGD_temp);
         MBGD_i:=MBGD_i+1
         
     }
     )
     }; 
      struct AdamPara {
     float beta1 and 
     float beta2 and 
     float eta and 
     float epsilon and 
     int time and 
     Mat *v and 
     Mat *hat_v and 
     Mat *s and 
     Mat *hat_s and 
     Mat *hat_g 
 };
 function SpaceCreateAdamPara ( FCNN *fcnn,AdamPara *adamPara )
 {
     frame(SpaceCreateAdamPara_i) and ( 
     adamPara->v:=(Mat *)malloc((fcnn->HiddenLayerNum+2)*sizeof(Mat));
     adamPara->hat_v:=(Mat *)malloc((fcnn->HiddenLayerNum+2)*sizeof(Mat));
     adamPara->s:=(Mat *)malloc((fcnn->HiddenLayerNum+2)*sizeof(Mat));
     adamPara->hat_s:=(Mat *)malloc((fcnn->HiddenLayerNum+2)*sizeof(Mat));
     adamPara->hat_g:=(Mat *)malloc((fcnn->HiddenLayerNum+2)*sizeof(Mat));
     adamPara->v[0].row:=0;
     adamPara->v[0].col:=0;
     adamPara->hat_v[0].row:=0;
     adamPara->hat_v[0].col:=0;
     adamPara->s[0].row:=0;
     adamPara->s[0].col:=0;
     adamPara->hat_s[0].row:=0;
     adamPara->hat_s[0].col:=0;
     adamPara->hat_g[0].row:=0;
     adamPara->hat_g[0].col:=0;
     int SpaceCreateAdamPara_i<==1 and skip;
     
     while( (SpaceCreateAdamPara_i<fcnn->HiddenLayerNum+2) )
     {
         MatCreate(&adamPara->v[SpaceCreateAdamPara_i],fcnn->Layer[SpaceCreateAdamPara_i-1].NeuronNum+1,fcnn->Layer[SpaceCreateAdamPara_i].NeuronNum,RValue);
         MatZeros(&adamPara->v[SpaceCreateAdamPara_i],RValue);
         MatCreate(&adamPara->hat_v[SpaceCreateAdamPara_i],fcnn->Layer[SpaceCreateAdamPara_i-1].NeuronNum+1,fcnn->Layer[SpaceCreateAdamPara_i].NeuronNum,RValue);
         MatZeros(&adamPara->hat_v[SpaceCreateAdamPara_i],RValue);
         MatCreate(&adamPara->s[SpaceCreateAdamPara_i],fcnn->Layer[SpaceCreateAdamPara_i-1].NeuronNum+1,fcnn->Layer[SpaceCreateAdamPara_i].NeuronNum,RValue);
         MatZeros(&adamPara->s[SpaceCreateAdamPara_i],RValue);
         MatCreate(&adamPara->hat_s[SpaceCreateAdamPara_i],fcnn->Layer[SpaceCreateAdamPara_i-1].NeuronNum+1,fcnn->Layer[SpaceCreateAdamPara_i].NeuronNum,RValue);
         MatZeros(&adamPara->hat_s[SpaceCreateAdamPara_i],RValue);
         MatCreate(&adamPara->hat_g[SpaceCreateAdamPara_i],fcnn->Layer[SpaceCreateAdamPara_i-1].NeuronNum+1,fcnn->Layer[SpaceCreateAdamPara_i].NeuronNum,RValue);
         MatZeros(&adamPara->hat_g[SpaceCreateAdamPara_i],RValue);
         SpaceCreateAdamPara_i:=SpaceCreateAdamPara_i+1
         
     }
     )
     }; 
  function initAdam ( FCNN fcnn,AdamPara *adamPara )
 {
     adamPara->beta1:=0.9;
     adamPara->beta2:=0.999;
     adamPara->eta:=0.0001;
     adamPara->epsilon:=0.00000008;
     SpaceCreateAdamPara(&fcnn,adamPara);
     adamPara->time:=1
     
 };
 function Adam ( FCNN *fcnn,AdamPara *adamPara )
 {
     frame(Adam_i,Adam_temp$_1,Adam_temp$_2) and ( 
     int Adam_i<==1 and skip;
     
     while( (Adam_i<=fcnn->HiddenLayerNum+1) )
     {
         MatNumMul(adamPara->beta1,&adamPara->v[Adam_i],&adamPara->v[Adam_i],RValue);
         MatNumMul(1-adamPara->beta1,&fcnn->Layer[Adam_i].NablaWbMat,&adamPara->hat_g[Adam_i],RValue);
         MatAdd(&adamPara->v[Adam_i],&adamPara->hat_g[Adam_i],&adamPara->v[Adam_i],RValue);
         MatNumMul(adamPara->beta2,&adamPara->s[Adam_i],&adamPara->s[Adam_i],RValue);
         MatSquare(&adamPara->hat_g[Adam_i],&adamPara->hat_g[Adam_i],RValue);
         MatNumMul(1-adamPara->beta2,&adamPara->hat_g[Adam_i],&adamPara->hat_g[Adam_i],RValue);
         MatAdd(&adamPara->s[Adam_i],&adamPara->hat_g[Adam_i],&adamPara->s[Adam_i],RValue);
         int Adam_temp$_1 and skip;
         Adam_temp$_1:=pow(adamPara->beta1,adamPara->time);
         MatNumMul((1/ (1-Adam_temp$_1)),&adamPara->v[Adam_i],&adamPara->hat_v[Adam_i],RValue);
         int Adam_temp$_2 and skip;
         Adam_temp$_2:=pow(adamPara->beta2,adamPara->time);
         MatNumMul((1/ (1-Adam_temp$_2)),&adamPara->s[Adam_i],&adamPara->hat_s[Adam_i],RValue);
         MatNumMul(adamPara->eta,&adamPara->hat_v[Adam_i],&adamPara->hat_v[Adam_i],RValue);
         MatSqrt(&adamPara->hat_s[Adam_i],&adamPara->hat_s[Adam_i],RValue);
         MatNumAdd(adamPara->epsilon,&adamPara->hat_s[Adam_i],&adamPara->hat_s[Adam_i],RValue);
         MatDiv(&adamPara->hat_v[Adam_i],&adamPara->hat_s[Adam_i],&adamPara->hat_g[Adam_i],RValue);
         MatSub(&fcnn->Layer[Adam_i].WeightBiasMat,&adamPara->hat_g[Adam_i],&fcnn->Layer[Adam_i].WeightBiasMat,RValue);
         Adam_i:=Adam_i+1
         
     };
     adamPara->time:=adamPara->time+1
     )
     }; 
  function judge_max ( float *arr,int n,int RValue )
 {
     frame(judge_max_index,judge_max_i,judge_max_max,return) and ( 
     int return<==0 and skip;
     int judge_max_index<==0 and skip;
     int judge_max_i and skip;
     float judge_max_max<==arr[0] and skip;
     judge_max_i:=0;
     
     while( (judge_max_i<n) )
     {
         if(arr[judge_max_i]>judge_max_max) then 
         {
             judge_max_max:=arr[judge_max_i];
             judge_max_index:=judge_max_i
             
         }
         else 
         {
              skip 
         };
         judge_max_i:=judge_max_i+1
         
     };
     return<==1 and RValue:=judge_max_index;
     skip
     )
     }; 
  function testAcc ( FCNN fcnn,DataSet dataset,float RValue )
 {
     frame(testAcc_MatCompare,testAcc_buf,testAcc_i,testAcc_correctNum,testAcc_1_temp$_1,return) and ( 
     int return<==0 and skip;
     Mat testAcc_MatCompare and skip;
     char testAcc_buf[20] and skip;
     int testAcc_i<==0 and skip;
     MatCreate(&testAcc_MatCompare,dataset.TestLabel.row,1,RValue);
     MatZeros(&testAcc_MatCompare,RValue);
     testAcc_i:=0;
     
     while( (testAcc_i<dataset.TestLabel.row) )
     {
         (testAcc_MatCompare.element[testAcc_i])[0]:=(float)judge_max(fcnn.Layer[fcnn.HiddenLayerNum+1].ActiMat.element[testAcc_i],fcnn.Layer[fcnn.HiddenLayerNum+1].ActiMat.col,RValue);
         testAcc_i:=testAcc_i+1
         
     };
     int testAcc_correctNum<==0 and skip;
     testAcc_i:=0;
     
     while( (testAcc_i<dataset.TestLabel.row) )
     {
         int testAcc_1_temp$_1 and skip;
         testAcc_1_temp$_1:=equal((testAcc_MatCompare.element[testAcc_i])[0],(dataset.TestLabel.element[testAcc_i])[0],RValue);
         if(testAcc_1_temp$_1) then 
         {
             testAcc_correctNum:=testAcc_correctNum+1
             
         }
         else 
         {
              skip 
         };
         testAcc_i:=testAcc_i+1
         
     };
     return<==1 and RValue:=(float)testAcc_correctNum/ (float)dataset.TestLabel.row;
     skip
     )
     }; 
      struct _iobuf {
     char *_ptr and 
     int _cnt and 
     char *_base and 
     int _flag and 
     int _file and 
     int _charbuf and 
     int _bufsiz and 
     char *_tmpfname 
 };
 float Xval[54880000]<=={0} and skip;
 float Yval[70000]<=={0} and skip;
 function MinstHWDataLoading (  )
 {
     frame(MinstHWDataLoading_buf,MinstHWDataLoading_fp,MinstHWDataLoading_fp2,MinstHWDataLoading_len$,MinstHWDataLoading_i,MinstHWDataLoading_j,MinstHWDataLoading_flag) and ( 
     char MinstHWDataLoading_buf[16] and skip;
     FILE *MinstHWDataLoading_fp<==NULL and skip;
     FILE *MinstHWDataLoading_fp2<==NULL and skip;
     int MinstHWDataLoading_len$ and skip;
     int MinstHWDataLoading_i<==0 and skip;
     int MinstHWDataLoading_j<==0 and skip;
     char *MinstHWDataLoading_flag and skip;
     MinstHWDataLoading_fp:=fopen("../../DataSet/MinstHandWriting/Transformation/MinstHW-70000/DataFeatrue.msd","r");
     MinstHWDataLoading_flag:=fgets(MinstHWDataLoading_buf,16,MinstHWDataLoading_fp);
     while( (MinstHWDataLoading_flag!=NULL) )
     {
         MinstHWDataLoading_len$:=strlen(MinstHWDataLoading_buf);
         MinstHWDataLoading_buf[MinstHWDataLoading_len$-1]:='\0';
         Xval[MinstHWDataLoading_i]:=(float)atof(MinstHWDataLoading_buf);
         MinstHWDataLoading_i:=MinstHWDataLoading_i+1;
         MinstHWDataLoading_flag:=fgets(MinstHWDataLoading_buf,16,MinstHWDataLoading_fp)
     };
     fclose(MinstHWDataLoading_fp) and skip;
     MinstHWDataLoading_fp2:=fopen("../../DataSet/MinstHandWriting/Transformation/MinstHW-70000/DataLabel.msd","r");
     MinstHWDataLoading_flag:=fgets(MinstHWDataLoading_buf,16,MinstHWDataLoading_fp2);
     while( (MinstHWDataLoading_flag!=NULL) )
     {
         MinstHWDataLoading_len$:=strlen(MinstHWDataLoading_buf);
         MinstHWDataLoading_buf[MinstHWDataLoading_len$-1]:='\0';
         Yval[MinstHWDataLoading_j]:=(float)atof(MinstHWDataLoading_buf);
         MinstHWDataLoading_j:=MinstHWDataLoading_j+1;
         MinstHWDataLoading_flag:=fgets(MinstHWDataLoading_buf,16,MinstHWDataLoading_fp2)
     };
     fclose(MinstHWDataLoading_fp2) and skip
     )
     }; 
  function main ( int  RValue )
 {
     frame(main_buf3,main_buf4,main_NueronNumArray,main_ActiFuncNumArray,main_buf1,main_buf2,main_userDefine,main_dataSet,main_fcnn,main_fclayer,main_adamPara,main_loss,main_losstest,main_trainOperationNum,main_i,main_j,main_temp$_1) and (
     char main_buf3[20] and skip;
     char main_buf4[20] and skip;
     MinstHWDataLoading();
     int main_NueronNumArray[4]<=={784,512,256,10} and skip;
     int main_ActiFuncNumArray[4]<=={0,3,3,5} and skip;
     char main_buf1[40] and skip;
     char main_buf2[40] and skip;
     Custom main_userDefine and skip;
     DataSet main_dataSet and skip;
     FCNN main_fcnn and skip;
     FCLayer main_fclayer and skip;
     InitCustom(&main_userDefine,RValue);
     InitDataSet(&main_dataSet,RValue);
     InitFCNN(&main_fcnn,RValue);
     InitFCLayer(&main_fclayer,RValue);
     main_userDefine.CompleteSampleNum:=70000;
     main_userDefine.TrainSampleNum:=60000;
     main_userDefine.TestSampleNum:=10000;
     main_userDefine.SampleDimensionNum:=784;
     main_userDefine.HiddenLayerNum:=2;
     main_userDefine.ClassificationNum:=10;
     main_userDefine.LossFuncNum:=1;
     main_userDefine.WeightInitWayNum:=3;
     main_userDefine.BatchSize:=200;
     main_userDefine.XValArray:=Xval;
     main_userDefine.YValArray:=Yval;
     main_userDefine.NeuronNumArray:=main_NueronNumArray;
     main_userDefine.ActiFuncNumArray:=main_ActiFuncNumArray;
     DumpCustom(main_userDefine,RValue);
     LoadParaFromCustom(main_userDefine,&main_dataSet,&main_fcnn);
     DatasetConstruction(main_userDefine,&main_dataSet);
     CreateNNSpaceAndLoadinPara2FCLayer(&main_fcnn,main_userDefine,RValue);
     NNWeightinit(&main_fcnn,RValue);
     AdamPara main_adamPara and skip;
     initAdam(main_fcnn,&main_adamPara);
     float main_loss<==0.0 and skip;
     float main_losstest<==0.0 and skip;
     int main_trainOperationNum<==40 and skip;
     int main_i<==0 and skip;
     
     while( (main_i<main_trainOperationNum) )
     {
         int main_j<==0 and skip;
         
         while( (main_j<main_dataSet.BatchNum) )
         {
             main_loss:=NNforward(main_dataSet.BatchTrainFeature[main_j],main_dataSet.BatchTrainLabelOneHot[main_j],&main_fcnn,RValue);
             NNBackward(&main_fcnn,RValue);
             Adam(&main_fcnn,&main_adamPara);
             if((main_j+1) % 30=0) then 
             {
                 output ("epoch ",main_i+1,"/",main_trainOperationNum," iteration ",main_j+1,"/",main_dataSet.BatchNum," loss=",F2S(main_loss,main_buf1,RValue),"\n") and skip
                 
             }
             else 
             {
                  skip 
             };
             main_j:=main_j+1
             
         };
         main_losstest:=NNforward(main_dataSet.TestFeature,main_dataSet.TestLabelOneHot,&main_fcnn,RValue);
         float main_temp$_1 and skip;
         main_temp$_1:=testAcc(main_fcnn,main_dataSet,RValue);
         output ("====== epoch ",main_i+1,"/",main_trainOperationNum,"  testloss=",F2S(main_losstest,main_buf2,RValue),"  acc=",F2S(main_temp$_1,main_buf1,RValue)," ======\n") and skip;
         main_i:=main_i+1
         
     }
     )
 };
  main(RValue)
 )
