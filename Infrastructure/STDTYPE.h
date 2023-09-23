
#ifndef STDTYPE_H_
#define STDTYPE_H_

typedef char s8;
typedef unsigned char u8;

typedef short int s16;
typedef unsigned short int u16;

typedef long int s32;
typedef unsigned long int u32;

typedef long long int s64;
typedef unsigned long long int u64;

typedef float f32;
typedef double f64;
typedef long double f128;

// pointer to function take nothing and return void(nothing)
typedef void(* PTR_ToVoidFun_t)(void);

#define NULL	   (void*)0		

typedef enum{False=0,True} Bool_t;
typedef enum{E_OK =0,E_NOK} E_STATUS_t;


#endif /* STDTYPE_H_ */