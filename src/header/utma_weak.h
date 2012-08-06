#ifndef _UTMA_WEAK_H
#define	_UTMA_WEAK_H

#include <pbc.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common.h"

#ifdef	__cplusplus
extern "C" {
#endif

    PubTriple* GenPub(pairing_t *pairing);

    Key* GenKey(Pub *pub, Msk *msk);

    CipherText* rE(Pub *pub, Pk *pk, PlainText *M);

    PlainText* rD(Pub *pub, Sk *sk, CipherText *Ct);

    CipherText* Randomize(Pub *pub, CipherText *Ct);

    Enc* star(Pub *pub, Enc *encp);

    Enc* enc(Pub *pub, Pk *pk, PlainText *M);

    PlainText* dec(Pub *pub, Sk *sk, Enc *enc);

    void freeCypherText(CipherText* ct);

#ifdef	__cplusplus
}
#endif

#endif	/* _UTMA_WEAK_H */

