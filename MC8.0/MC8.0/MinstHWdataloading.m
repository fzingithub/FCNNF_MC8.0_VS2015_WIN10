frame(nums,labels) and (
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
 float nums[54880000]<=={0} and skip;
 float labels[70000]<=={0} and skip;
 function main ( int  RValue )
 {
     frame(main_buf,main_fp,main_fp2,main_len$,main_i,main_j,main_flag,return) and (
     int return<==0 and skip;
     char main_buf[16] and skip;
     FILE *main_fp<==NULL and skip;
     FILE *main_fp2<==NULL and skip;
     int main_len$ and skip;
     int main_i<==0 and skip;
     int main_j<==0 and skip;
     char *main_flag and skip;
     main_fp:=fopen("C:/Users/Administrator/Documents/GitHub/MinstHW/DataFeatrue.txt","r");
     main_flag:=fgets(main_buf,16,main_fp);
     while( (main_flag!=NULL) )
     {
         main_len$:=strlen(main_buf);
         main_buf[main_len$-1]:='\0';
         nums[main_i]:=atof(main_buf);
         main_i:=main_i+1;
         main_flag:=fgets(main_buf,16,main_fp)
     };
     fclose(main_fp) and skip;
     main_fp2:=fopen("C:/Users/Administrator/Documents/GitHub/MinstHW/DataLabel.txt","r");
     main_flag:=fgets(main_buf,16,main_fp2);
     while( (main_flag!=NULL) )
     {
         main_len$:=strlen(main_buf);
         main_buf[main_len$-1]:='\0';
         labels[main_j]:=atof(main_buf);
         main_j:=main_j+1;
         main_flag:=fgets(main_buf,16,main_fp2)
     };
     fclose(main_fp2) and skip;
     return<==1 and RValue:=0;
     skip
     )
 };
  main(RValue)
 )
