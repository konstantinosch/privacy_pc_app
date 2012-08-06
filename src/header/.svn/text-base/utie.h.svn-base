/* 
 * File:   utie.h
 * Author: angelo
 *
 * Created on May 19, 2009, 9:44 AM
 */

#ifndef _UTIE_H
#define	_UTIE_H

#include <pbc.h>
#include <pbc_time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common.h"
#include "utma_weak.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct {
        element_t a_1;
        element_t a_2;
        element_t a_3;
        element_t a_4;
        element_t a_5;

        element_t c_1;
        element_t c_2;
    } IECypherText;

    typedef struct {
          pairing_t pairing;
          element_t g_1;
          element_t g_2;
      } IEPub;

    typedef struct {
        element_t s;
        element_t t;
    } CASK;

    typedef struct {
        element_t S;
        element_t T;
        IECypherText *C;
    } CAPK;

    typedef struct {
        CASK cask;
        CAPK capk;
    } CAKey;

    typedef struct {
        element_t x;
    } IssuerSK;

    typedef struct {
        element_t y;
    } IssuerPK;

    typedef struct {
        IssuerSK sk;
        IssuerPK pk;
    } IssuerKey;

    typedef struct {
        element_t a_1;
        element_t a_2;
        element_t a_3;
        element_t a_4;
        element_t a_5;
    } PublicKeyCertificate;


    IEPub* GenIEPub(pairing_t *pairing);

    CAKey* GenerateCAKey(IEPub *pub);

    IssuerKey* GenerateKey(IEPub* pub);

    PublicKeyCertificate* RegisterPublicKey(IEPub* pub, CAKey* cakey, IssuerKey* issuerKey);

    IECypherText* InitTag(IEPub *pub, IssuerPK *issuerPk, PublicKeyCertificate *certificate, PlainText *pt);

    IECypherText* IERandomize(IEPub *pub, CAKey *cakey, IECypherText* ct);

    PlainText* ReadAndDecrypt(IEPub *pub, CAKey *cakey, IssuerSK *issuerSk, IECypherText* ct);

    int VerifyCertificate(IEPub *pub, CAKey *cakey, IECypherText* ct);


#ifdef	__cplusplus
}
#endif

#endif	/* _UTIE_H */

