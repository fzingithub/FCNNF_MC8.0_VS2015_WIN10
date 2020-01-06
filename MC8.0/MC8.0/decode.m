frame(mDecodeVar) and (

 struct S_TAG_RECOG__00_FF {
 int index and 
 int state and 
 int counter and 
 int num 
 };
 struct S_TAG_DECODE_00_FF {
 unsigned char *decodeAddr and 
 int decodeSt and 
 int delelteIdx[32] and 
 int blkNum and 
 int blkIdx and 
 int blkLen[4] and 
 int winPos 
 };
 S_TAG_DECODE_00_FF mDecodeVar and skip;
 function DecodeRec00 ( int deleteIdx[],S_TAG_RECOG__00_FF *recog,unsigned char *buffer )
 {
     frame(DecodeRec00_ch,DecodeRec00_ch1,return_1) and ( 
     int return_1<==0 and skip;
     unsigned char DecodeRec00_ch and skip;
     unsigned char DecodeRec00_ch1 and skip;
     DecodeRec00_ch:=buffer[recog->index];
     if(DecodeRec00_ch=0) then 
     {
         recog->counter:=recog->counter+1;
         if(recog->counter=20) then 
         {
             DecodeRec00_ch1:=buffer[recog->index+1];
             if(DecodeRec00_ch1=170) then 
             {
                 deleteIdx[recog->num]:=recog->index+1;
                 recog->num:=recog->num+1;
                 recog->index:=recog->index+1
                 
             }
             else 
             {
                  skip 
             };
             recog->counter:=0;
             recog->state:=13107
             
         }
         else 
         {
              skip 
         }
         
     }
     else
     {
         if(DecodeRec00_ch=255) then 
         {
             recog->counter:=1;
             recog->state:=43690
         }
         else
         {
             recog->counter:=0;
             recog->state:=13107
         }
     };
      return_1<==1 and skip
     )
     }; 
  function DecodeRecFF ( int deleteIdx[],S_TAG_RECOG__00_FF *recog,unsigned char *buffer )
 {
     frame(DecodeRecFF_ch,DecodeRecFF_ch1,return_1) and ( 
     int return_1<==0 and skip;
     unsigned char DecodeRecFF_ch and skip;
     unsigned char DecodeRecFF_ch1 and skip;
     DecodeRecFF_ch:=buffer[recog->index];
     if(DecodeRecFF_ch=0) then 
     {
         recog->counter:=1;
         recog->state:=21845
         
     }
     else
     {
         if(DecodeRecFF_ch=255) then 
         {
             recog->counter:=recog->counter+1;
             if(recog->counter=20) then 
             {
                 DecodeRecFF_ch1:=buffer[recog->index+1];
                 if(DecodeRecFF_ch1=85) then 
                 {
                     deleteIdx[recog->num]:=recog->index+1;
                     recog->num:=recog->num+1;
                     recog->index:=recog->index+1
                     
                 }
                 else 
                 {
                      skip 
                 };
                 recog->counter:=0;
                 recog->state:=13107
                 
             }
             else 
             {
                  skip 
             }
         }
         else
         {
             recog->counter:=0;
             recog->state:=13107
         }
     };
      return_1<==1 and skip
     )
     }; 
  function DecodeFindIndexDelete ( unsigned char buffer[],int len$,int deleteIdx[],int RValue )
 {
     frame(DecodeFindIndexDelete_recog,return_1) and ( 
     int return_1<==0 and skip;
     S_TAG_RECOG__00_FF DecodeFindIndexDelete_recog and skip;
     DecodeFindIndexDelete_recog.state:=13107;
     DecodeFindIndexDelete_recog.counter:=0;
     DecodeFindIndexDelete_recog.num:=0;
     DecodeFindIndexDelete_recog.index:=0;
     
     while(DecodeFindIndexDelete_recog.index<(len$-1))
     {
         if(DecodeFindIndexDelete_recog.state=21845) then 
         {
             DecodeRec00(deleteIdx,&DecodeFindIndexDelete_recog,buffer)
             
         }
         else
         {
             if(DecodeFindIndexDelete_recog.state=43690) then 
             {
                 DecodeRecFF(deleteIdx,&DecodeFindIndexDelete_recog,buffer)
             }
             else
             {
                 if(buffer[DecodeFindIndexDelete_recog.index]=0) then 
                 {
                     DecodeFindIndexDelete_recog.counter:=1;
                     DecodeFindIndexDelete_recog.state:=21845
                 }
                 else
                 {
                     if(buffer[DecodeFindIndexDelete_recog.index]=255) then 
                     {
                         DecodeFindIndexDelete_recog.counter:=1;
                         DecodeFindIndexDelete_recog.state:=43690
                         
                     }
                     else 
                     {
                          skip 
                     }
                 }
             }
         };
         DecodeFindIndexDelete_recog.index:=DecodeFindIndexDelete_recog.index+1
         
     };
     return_1<==1 and RValue:=DecodeFindIndexDelete_recog.num;
     skip
     )
     }; 
  function DecodeRemoveBytes ( unsigned char buffer[],int bLen,int deleteIdx[],int dLen )
 {
     frame(DecodeRemoveBytes_i,DecodeRemoveBytes_j,return_1) and ( 
     int return_1<==0 and skip;
     int DecodeRemoveBytes_i and skip;
     int DecodeRemoveBytes_j and skip;
     DecodeRemoveBytes_i:=0;
     
     while(DecodeRemoveBytes_i<dLen-1)
     {
         DecodeRemoveBytes_j:=deleteIdx[DecodeRemoveBytes_i]+1;
         
         while(DecodeRemoveBytes_j<deleteIdx[DecodeRemoveBytes_i+1])
         {
             buffer[DecodeRemoveBytes_j-DecodeRemoveBytes_i-1]:=buffer[DecodeRemoveBytes_j];
             DecodeRemoveBytes_j:=DecodeRemoveBytes_j+1
             
         };
         DecodeRemoveBytes_i:=DecodeRemoveBytes_i+1
         
     };
     if(dLen>0) then 
     {
         DecodeRemoveBytes_j:=deleteIdx[dLen-1]+1;
         
         while(DecodeRemoveBytes_j<bLen)
         {
             buffer[DecodeRemoveBytes_j-dLen]:=buffer[DecodeRemoveBytes_j];
             DecodeRemoveBytes_j:=DecodeRemoveBytes_j+1
             
         }
         
     }
     else 
     {
          skip 
     };
      return_1<==1 and skip
     )
     }; 
  function DecodeWinMerge ( unsigned char *startAddr,int blkSize,int blkNum,int blkLen[],int RValue )
 {
     frame(DecodeWinMerge_i,DecodeWinMerge_j,DecodeWinMerge_tmpBuffer,DecodeWinMerge_tmpRemoveIdx,DecodeWinMerge_tmpNum,DecodeWinMerge_shiftLen,DecodeWinMerge_removeLen,DecodeWinMerge_idxTarget,return_1) and ( 
     int return_1<==0 and skip;
     int DecodeWinMerge_i and skip;
     int DecodeWinMerge_j and skip;
     unsigned char DecodeWinMerge_tmpBuffer[40] and skip;
     int DecodeWinMerge_tmpRemoveIdx[16] and skip;
     int DecodeWinMerge_tmpNum and skip;
     int DecodeWinMerge_shiftLen and skip;
     int DecodeWinMerge_removeLen and skip;
     int DecodeWinMerge_idxTarget and skip;
     DecodeWinMerge_tmpRemoveIdx[0]:=-1;
     DecodeWinMerge_i:=1;
     
     while(DecodeWinMerge_i<blkNum)
     {
         DecodeWinMerge_j:=0;
         
         while(DecodeWinMerge_j<20)
         {
             DecodeWinMerge_tmpBuffer[DecodeWinMerge_j]:=startAddr[(DecodeWinMerge_i-1)*blkSize+blkLen[DecodeWinMerge_i-1]-20+DecodeWinMerge_j];
             DecodeWinMerge_tmpBuffer[20+DecodeWinMerge_j]:=startAddr[DecodeWinMerge_i*blkSize+DecodeWinMerge_j];
             DecodeWinMerge_j:=DecodeWinMerge_j+1
             
         };
         DecodeWinMerge_tmpNum:=DecodeFindIndexDelete(DecodeWinMerge_tmpBuffer,40,&DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i],RValue);
         if(DecodeWinMerge_tmpNum=0) then 
         {
             DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]:=-1
             
         }
         else
         {
             DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]:=DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]-20
         };
         DecodeWinMerge_i:=DecodeWinMerge_i+1
         
     };
     DecodeWinMerge_shiftLen:=0;
     DecodeWinMerge_removeLen:=0;
     DecodeWinMerge_idxTarget:=-1;
     DecodeWinMerge_i:=1;
     
     while(DecodeWinMerge_i<blkNum)
     {
         DecodeWinMerge_shiftLen:=DecodeWinMerge_shiftLen+(blkSize-blkLen[DecodeWinMerge_i-1]);
         if(DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i-1]>=0) then 
         {
             DecodeWinMerge_removeLen:=DecodeWinMerge_removeLen+1
             
         }
         else 
         {
              skip 
         };
         if(DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]<0) then 
         {
             DecodeWinMerge_j:=0;
             
             while(DecodeWinMerge_j<blkLen[DecodeWinMerge_i])
             {
                 DecodeWinMerge_idxTarget:=DecodeWinMerge_i*blkSize+DecodeWinMerge_j-DecodeWinMerge_removeLen-DecodeWinMerge_shiftLen;
                 startAddr[DecodeWinMerge_idxTarget]:=startAddr[DecodeWinMerge_i*blkSize+DecodeWinMerge_j];
                 DecodeWinMerge_j:=DecodeWinMerge_j+1
                 
             }
             
         }
         else
         {
             DecodeWinMerge_j:=0;
             
             while(DecodeWinMerge_j<DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i])
             {
                 DecodeWinMerge_idxTarget:=DecodeWinMerge_i*blkSize+DecodeWinMerge_j-DecodeWinMerge_removeLen-DecodeWinMerge_shiftLen;
                 startAddr[DecodeWinMerge_idxTarget]:=startAddr[DecodeWinMerge_i*blkSize+DecodeWinMerge_j];
                 DecodeWinMerge_j:=DecodeWinMerge_j+1
                 
             };
             DecodeWinMerge_j:=DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]+1;
             
             while(DecodeWinMerge_j<blkLen[DecodeWinMerge_i])
             {
                 DecodeWinMerge_idxTarget:=DecodeWinMerge_i*blkSize+DecodeWinMerge_j-DecodeWinMerge_removeLen-DecodeWinMerge_shiftLen-1;
                 startAddr[DecodeWinMerge_idxTarget]:=startAddr[DecodeWinMerge_i*blkSize+DecodeWinMerge_j];
                 DecodeWinMerge_j:=DecodeWinMerge_j+1
                 
             }
         };
         DecodeWinMerge_i:=DecodeWinMerge_i+1
         
     };
     return_1<==1 and RValue:=(DecodeWinMerge_idxTarget+1);
     skip
     )
     }; 
  function DecodeRemoveBlk ( unsigned char *startAddr,int winPos,int blkNum,int winRemoveNum )
 {
     frame(DecodeRemoveBlk_i,return_1) and ( 
     int return_1<==0 and skip;
     int DecodeRemoveBlk_i and skip;
     DecodeRemoveBlk_i:=winPos+4;
     
     while(DecodeRemoveBlk_i<blkNum)
     {
         memcpy(&startAddr[(DecodeRemoveBlk_i-winRemoveNum)*32],&startAddr[DecodeRemoveBlk_i*32],32) and skip;
         DecodeRemoveBlk_i:=DecodeRemoveBlk_i+1
         
     };
      return_1<==1 and skip
     )
     }; 
  function DecodeLenUpdate ( int lastIdx,int winPos,int winSize,int blkNum,int newLen[],int RValue )
 {
     frame(DecodeLenUpdate_i,DecodeLenUpdate_winRemoveNum,return_1,break_1) and ( 
     int break_1<==0 and skip;
     int return_1<==0 and skip;
     int DecodeLenUpdate_i and skip;
     int DecodeLenUpdate_winRemoveNum and skip;
     break_1<==0 and skip;
     DecodeLenUpdate_i:=0;
     
     while( break_1=0 AND  DecodeLenUpdate_i<winSize)
     {
         if(lastIdx>32) then 
         {
             newLen[winPos+DecodeLenUpdate_i]:=32;
             lastIdx:=lastIdx-32
             
         }
         else
         {
             newLen[winPos+DecodeLenUpdate_i]:=lastIdx;
             break_1<==1 and skip
          };
         if(break_1=0)   then 
         {
             DecodeLenUpdate_i:=DecodeLenUpdate_i+1
         }
         else
         {
             skip
         }
         
     };
     break_1<==0 and skip;
     DecodeLenUpdate_winRemoveNum:=winSize-DecodeLenUpdate_i-1;
     if(DecodeLenUpdate_winRemoveNum>0) then 
     {
         DecodeLenUpdate_i:=winPos+winSize;
         
         while(DecodeLenUpdate_i<blkNum)
         {
             newLen[DecodeLenUpdate_i-DecodeLenUpdate_winRemoveNum]:=newLen[DecodeLenUpdate_i];
             DecodeLenUpdate_i:=DecodeLenUpdate_i+1
             
         }
         
     }
     else 
     {
          skip 
     };
     return_1<==1 and RValue:=DecodeLenUpdate_winRemoveNum;
     skip
     )
     }; 
  function DecodeRemoveByteBlk ( unsigned char buffer[],int len$,int delelteIdx[],int RValue )
 {
     frame(DecodeRemoveByteBlk_delCnt,return_1) and ( 
     int return_1<==0 and skip;
     int DecodeRemoveByteBlk_delCnt and skip;
     DecodeRemoveByteBlk_delCnt:=DecodeFindIndexDelete(buffer,32,delelteIdx,RValue);
     DecodeRemoveBytes(buffer,len$,delelteIdx,DecodeRemoveByteBlk_delCnt);
     return_1<==1 and RValue:=(len$-DecodeRemoveByteBlk_delCnt);
     skip
     )
     }; 
  function DecodeDataMerge ( unsigned char *startAddr,int winPos,int *blkNum,int newLen[],int RValue )
 {
     frame(DecodeDataMerge_winEleCnt,DecodeDataMerge_tmpWinSize,DecodeDataMerge_winRemoveNum,return_1) and ( 
     int return_1<==0 and skip;
     int DecodeDataMerge_winEleCnt and skip;
     int DecodeDataMerge_tmpWinSize and skip;
     int DecodeDataMerge_winRemoveNum and skip;
     if((winPos+4)>* blkNum) then 
     {
         DecodeDataMerge_tmpWinSize:=(* blkNum)-winPos
         
     }
     else
     {
         DecodeDataMerge_tmpWinSize:=4
     };
     DecodeDataMerge_winRemoveNum:=0;
     if(DecodeDataMerge_tmpWinSize>1) then 
     {
         DecodeDataMerge_winEleCnt:=DecodeWinMerge(&startAddr[winPos*32],32,DecodeDataMerge_tmpWinSize,&newLen[winPos],RValue);
         DecodeDataMerge_winRemoveNum:=DecodeLenUpdate(DecodeDataMerge_winEleCnt,winPos,DecodeDataMerge_tmpWinSize,* blkNum,newLen,RValue);
         if(DecodeDataMerge_winRemoveNum>0) then 
         {
             DecodeRemoveBlk(startAddr,winPos,* blkNum,DecodeDataMerge_winRemoveNum);
             * blkNum:=* blkNum-DecodeDataMerge_winRemoveNum
             
         }
         else 
         {
              skip 
         }
         
     }
     else 
     {
          skip 
     };
     return_1<==1 and RValue:=DecodeDataMerge_winRemoveNum;
     skip
     )
     }; 
  function DecodeInitial ( unsigned char *addr )
 {
     frame(return_1) and ( 
     int return_1<==0 and skip;
     mDecodeVar.decodeAddr:=addr;
     mDecodeVar.decodeSt:=13107;
     mDecodeVar.blkNum:=4;
     mDecodeVar.blkIdx:=0;
     mDecodeVar.winPos:=0;
      return_1<==1 and skip
     )
     }; 
  function DecodeGetState ( int *st,int *blockNum,int *lastLen )
 {
     frame(return_1) and ( 
     int return_1<==0 and skip;
     * st:=mDecodeVar.decodeSt;
     * blockNum:=mDecodeVar.blkNum;
     * lastLen:=mDecodeVar.blkLen[mDecodeVar.blkNum-1];
      return_1<==1 and skip
     )
     }; 
  function DecodeEntry (  )
 {
     frame(DecodeEntry_i,DecodeEntry_winRemoveNum,nm_1$,break_1$,switch_1,return_1) and ( 
     int return_1<==0 and skip;
     int DecodeEntry_i and skip;
     int DecodeEntry_winRemoveNum and skip;
     int switch_1 and skip;
     int break_1$ and skip;
     break_1$<==0 and skip;
      switch_1<==0 and skip;
      int nm_1$ and skip;
     nm_1$ := mDecodeVar.decodeSt;
     if (nm_1$=13107 OR (switch_1=1 AND break_1$=0 AND return_1=0) ) then 
     {
         switch_1<==1 and skip;
         mDecodeVar.decodeSt:=21845;
         break_1$<==1 and skip
          
     }
     else
     {
         skip
     };
     if (nm_1$=21845 OR (switch_1=1 AND break_1$=0 AND return_1=0) ) then 
     {
         switch_1<==1 and skip;
         DecodeEntry_i:=0;
         
         while(((mDecodeVar.blkIdx<mDecodeVar.blkNum) AND (DecodeEntry_i<4)))
         {
             mDecodeVar.blkLen[mDecodeVar.blkIdx+DecodeEntry_i]:=DecodeRemoveByteBlk(&mDecodeVar.decodeAddr[(mDecodeVar.blkIdx+DecodeEntry_i)*32],32,mDecodeVar.delelteIdx,RValue);
             DecodeEntry_i:=DecodeEntry_i+1
             
         };
         if(mDecodeVar.blkIdx>=mDecodeVar.blkNum) then 
         {
             mDecodeVar.decodeSt:=43690
             
         }
         else
         {
             mDecodeVar.blkIdx:=mDecodeVar.blkIdx+4
         };
         break_1$<==1 and skip
          
     }
     else
     {
         skip
     };
     if (nm_1$=43690 OR (switch_1=1 AND break_1$=0 AND return_1=0) ) then 
     {
         switch_1<==1 and skip;
         DecodeEntry_i:=0;
         
         while(((mDecodeVar.winPos<mDecodeVar.blkNum) AND (DecodeEntry_i<4)))
         {
             DecodeEntry_winRemoveNum:=DecodeDataMerge(mDecodeVar.decodeAddr,mDecodeVar.winPos,&mDecodeVar.blkNum,mDecodeVar.blkLen,RValue);
             mDecodeVar.winPos:=mDecodeVar.winPos+4-1-DecodeEntry_winRemoveNum;
             DecodeEntry_i:=DecodeEntry_i+1
             
         };
         if(mDecodeVar.winPos>=mDecodeVar.blkNum) then 
         {
             mDecodeVar.decodeSt:=52428
             
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
     if(break_1$=0 AND return_1=0) then 
     {
         break_1$<==1 and skip
          
     }
     else
     {
          skip
         })
         }; 
  function FillBuffer ( unsigned char buffer[],int len$ )
 {
     frame(FillBuffer_flg,FillBuffer_i,return_1) and ( 
     int return_1<==0 and skip;
     unsigned char FillBuffer_flg and skip;
     int FillBuffer_i and skip;
     FillBuffer_flg:=0;
     FillBuffer_i:=0;
     
     while(FillBuffer_i<len$)
     {
         if(FillBuffer_flg=0) then 
         {
             buffer[FillBuffer_i]:=0;
             if((FillBuffer_i+1) % 21=0) then 
             {
                 buffer[FillBuffer_i]:=170;
                 FillBuffer_flg:=1
                 
             }
             else 
             {
                  skip 
             }
             
         }
         else
         {
             buffer[FillBuffer_i]:=255;
             if((FillBuffer_i+1) % 21=0) then 
             {
                 buffer[FillBuffer_i]:=85;
                 FillBuffer_flg:=0
                 
             }
             else 
             {
                  skip 
             }
         };
         FillBuffer_i:=FillBuffer_i+1
         
     };
      return_1<==1 and skip
     )
     }; 
  function TestCmdE2DataMerge (  )
 {
     frame(TestCmdE2DataMerge_len08,TestCmdE2DataMerge_buffer,TestCmdE2DataMerge_blkNum,TestCmdE2DataMerge_i,TestCmdE2DataMerge_cnt,TestCmdE2DataMerge_lastLen,TestCmdE2DataMerge_st,return_1,break_1) and (
     int break_1<==0 and skip;
     int return_1<==0 and skip;
     int TestCmdE2DataMerge_len08<==32*4 and skip;
     unsigned char TestCmdE2DataMerge_buffer[4,32] and skip;
     int TestCmdE2DataMerge_blkNum<==4 and skip;
     int TestCmdE2DataMerge_i and skip;
     int TestCmdE2DataMerge_cnt and skip;
     int TestCmdE2DataMerge_lastLen and skip;
     int TestCmdE2DataMerge_st and skip;
     FillBuffer((unsigned char *)TestCmdE2DataMerge_buffer,TestCmdE2DataMerge_len08);
     DecodeInitial((unsigned char *)TestCmdE2DataMerge_buffer);
     break_1<==0 and skip;
     while( break_1=0 AND 1)
     {
         DecodeEntry();
         DecodeGetState(&TestCmdE2DataMerge_st,&TestCmdE2DataMerge_blkNum,&TestCmdE2DataMerge_lastLen);
         if(TestCmdE2DataMerge_st=52428) then 
         {
             break_1<==1 and skip
              
         }
         else 
         {
              skip 
         }
     };
     break_1<==0 and skip;
     TestCmdE2DataMerge_cnt:=(TestCmdE2DataMerge_blkNum-1)*32+TestCmdE2DataMerge_lastLen;
     TestCmdE2DataMerge_i:=0;
     
     while(TestCmdE2DataMerge_i<TestCmdE2DataMerge_cnt)
     {
         output (((unsigned char *)TestCmdE2DataMerge_buffer)[TestCmdE2DataMerge_i],"x ","x ") and skip;
         if((TestCmdE2DataMerge_i+1) % 20=0) then 
         {
             output ("\n") and skip
             
         }
         else 
         {
              skip 
         };
         TestCmdE2DataMerge_i:=TestCmdE2DataMerge_i+1
         
     };
     output (TestCmdE2DataMerge_cnt,"\n","\n") and skip;
      return_1<==1 and skip
     )
 };
TestCmdE2DataMerge (  )

  )