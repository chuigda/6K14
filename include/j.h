#ifndef P6K14_J_H
#define P6K14_J_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
   PL9_G_Alpha   = 0,
   PL9_G_Beta    = 1,
   PL9_G_Gamma   = 2,
   PL9_G_Delta   = 3,
   PL9_G_Epsilon = 4,

   PL9_G_Pi    = 5,
   PL9_G_Sigma = 6,
   PL9_G_Tau   = 7,
   PL9_G_Eta   = 8
} PL9Greek;

char const* pl9_ExplainGreek(PL9Greek greek);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* P6K14_J_H */
