// FOR_YOU: here you can type your discoveries about tomb4 procedures
// if you mean use the C language to call them, you can create the
// prototypes of them like it has been done in the "DefTomb4Funct.h" file
// and "functions.h" file.
// While if you wish use only assembly code, you can type here
// the #define ADR_NOME_PROC 0x404040  to remember and use in the asm
// code all addresses of the tomb4 procedures

// TYPE_HERE: prototype (c++ language), mnemonic constants for 
// addresses (assembly)

typedef void (__cdecl *TYPE_AddQuadSorted) (DXVERTEX* v, short s1, short s2, short s3, short s4, TEXTURESTRUCT* t, long l1);
TYPE_AddQuadSorted* AddQuadSorted = (TYPE_AddQuadSorted*) 0x5339A4;

#define clipflags (*(short**)(0x753854))
#define phd_winxmin *(short*)(0x753C04)
#define phd_winxmax *(short*)(0x753BEC)
#define phd_winymin *(short*)(0x753BBC)
#define phd_winymax *(short*)(0x753C10)
#define phd_persp *(long*)(0x753BDC)

#define f_moneoznear *(float*)(0x753C28)
#define f_mznear *(float*)(0x753BA4)