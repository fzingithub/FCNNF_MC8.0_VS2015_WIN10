typedef union{
   char            *cString;   /* A character string */
   int             nInteger;   /* An intliteral value */
   float           floatval;   /* A float value */
   char            character;  /* A character */
   CSyntaxNode     *tnode;     /* Node in the syntax tree */
   NODETYPE        nodetype;
   RETURNTYPE      returntype;
} YYSTYPE;
#define	INCLUDE	258
#define	IF	259
#define	ELSE	260
#define	EMPTY	261
#define	AWAIT	262
#define	PROJECTION	263
#define	PBEGIN	264
#define	PEND	265
#define	POINTERNULL	266
#define	ARRAY	267
#define	STRUCT	268
#define	DOT	269
#define	UNION	270
#define	FILEDEC	271
#define	DEFINE	272
#define	MY_TRUE	273
#define	MY_FALSE	274
#define	EXIST	275
#define	FRAME	276
#define	FOR	277
#define	WHILE	278
#define	DO	279
#define	TRUE	280
#define	FALSE	281
#define	REPEAT	282
#define	UNTIL	283
#define	DISPLAY	284
#define	SKIP	285
#define	THEN	286
#define	COLON	287
#define	VOID	288
#define	BOOL	289
#define	CHAR	290
#define	SHORT	291
#define	INT	292
#define	LONG	293
#define	STR	294
#define	FLOAT	295
#define	DOUBLE	296
#define	CRITICAL_SECTION	297
#define	TIMECAPS	298
#define	OPEN_PAR	299
#define	CLOSE_PAR	300
#define	TIMES	301
#define	OPEN_BPAR	302
#define	CLOSE_BPAR	303
#define	OPEN_MPAR	304
#define	CLOSE_MPAR	305
#define	FUNCTION	306
#define	INTER_OR	307
#define	UNSIGNED	308
#define	SIGNED	309
#define	SIZEOF	310
#define	EXTERN	311
#define	COMMA	312
#define	CHOP	313
#define	OR	314
#define	PARALLEL	315
#define	AND	316
#define	ASS_P	317
#define	ASS_N	318
#define	BOOL_OR	319
#define	BOOL_AND	320
#define	XOR	321
#define	ADDRESS	322
#define	NE	323
#define	EQ	324
#define	GE	325
#define	LE	326
#define	GT	327
#define	LT	328
#define	LST	329
#define	RST	330
#define	ADD	331
#define	SUB	332
#define	MUL	333
#define	DIV	334
#define	MOD	335
#define	BNE	336
#define	IMPLY	337
#define	IFF	338
#define	PROPIMPLY	339
#define	CHOPSTAR	340
#define	NEXT	341
#define	NEGATION	342
#define	LENGTH	343
#define	REQUEST	344
#define	FINAL	345
#define	KEEP	346
#define	ALWAYS	347
#define	HALT	348
#define	W_NEXT	349
#define	SOMETIMES	350
#define	ID	351
#define	STRLITERAL	352
#define	STRUCT_TYPE	353
#define	INTLITERAL	354
#define	FLOATLITERAL	355
#define	CHARLITERAL	356


extern YYSTYPE yylval;
