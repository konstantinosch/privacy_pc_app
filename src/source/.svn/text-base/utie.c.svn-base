#include "utie.h"

IEPub* GenIEPub(pairing_t *pairing) {
    IEPub* pubTriple;
    element_t exp;
    pairing_pp_t pp;

    // TODO: verify the pairing is symmetric....

//    fprintf(stdout, "[GenIEPub] Allocate the space for the result.\n");

    pubTriple = (IEPub *) malloc(sizeof(IEPub));
    pubTriple->pairing[0] = *pairing[0];

//    fprintf(stdout, "[GenIEPub] Space allocated.\n");

    // Init elments
//    fprintf(stdout, "[GenIEPub] Init elements.\n");

    element_init_G1(pubTriple->g_1, *pairing);
    element_random(pubTriple->g_1);
    element_init_G2(pubTriple->g_2, *pairing);
    element_random(pubTriple->g_2);

//    fprintf(stdout, "[GenIEPub] Elements initialized.\n");

    return pubTriple;

}

CAKey* GenerateCAKey(IEPub *pub) {
    CAKey *cakey = (CAKey *) malloc(sizeof(CAKey));
    
    // generate sk

    element_init_Zr(cakey->cask.s, pub->pairing);
    element_random(cakey->cask.s);
    
    element_init_Zr(cakey->cask.t, pub->pairing);
    element_random(cakey->cask.t);

    // generate pk

    element_init_G2(cakey->capk.S, pub->pairing);
    element_pow_zn(cakey->capk.S, pub->g_2, cakey->cask.s);
    
    element_init_G2(cakey->capk.T, pub->pairing);
    element_pow_zn(cakey->capk.T, pub->g_2, cakey->cask.t);

    // TODO: generate the dummy cypher_text

    IECypherText* dummy_enc = (IECypherText*) malloc(sizeof(IECypherText));

    element_t w, y, exp, temp;

    element_init_G1(y, pub->pairing);
    element_random(y);

    element_init_Zr(w, pub->pairing);
    element_random(w);

    // init a_1
    element_init_G1(dummy_enc->a_1, pub->pairing);
    element_pow_zn(dummy_enc->a_1, pub->g_1, w);

    // init a_2
    element_init_G1(dummy_enc->a_2, pub->pairing);
    element_pow_zn(dummy_enc->a_2, dummy_enc->a_1, cakey->cask.t);

    // init a_3
    element_init_G1(dummy_enc->a_3, pub->pairing);

    element_init_G1(temp, pub->pairing);

    element_pow_zn(dummy_enc->a_3, dummy_enc->a_1, cakey->cask.s);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, w, cakey->cask.s);
    element_mul(exp, exp, cakey->cask.t);
    element_pow_zn(temp, y, exp);
    element_clear(exp);

    element_mul(dummy_enc->a_3, dummy_enc->a_3, temp);

    element_clear(temp);


    // init a_4
    element_init_G1(dummy_enc->a_4, pub->pairing);
    element_pow_zn(dummy_enc->a_4, y, w);

    // init a_5
    element_init_G1(dummy_enc->a_5, pub->pairing);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, w, cakey->cask.t);

    element_pow_zn(dummy_enc->a_5, y, exp);

    element_clear(exp);

    element_init_G1(dummy_enc->c_1, pub->pairing);
    element_random(dummy_enc->c_1);

    element_init_G1(dummy_enc->c_2, pub->pairing);
    element_random(dummy_enc->c_2);

    cakey->capk.C = dummy_enc;

    return cakey;
}

IssuerKey* GenerateKey(IEPub* pub) {
    IssuerKey *issuerKey = (IssuerKey *) malloc(sizeof(IssuerKey));

    element_init_Zr(issuerKey->sk.x, pub->pairing);
    element_random(issuerKey->sk.x);

    element_init_G1(issuerKey->pk.y, pub->pairing);
    element_pow_zn(issuerKey->pk.y, pub->g_1, issuerKey->sk.x);

//    element_fprintf(stdout, "[GenerateKey] SK %B\n", issuerKey->sk.x);
//    element_fprintf(stdout, "[GenerateKey] PK %B\n", issuerKey->pk.y);

    return issuerKey;
}

PublicKeyCertificate* RegisterPublicKey(IEPub* pub, CAKey* cakey, IssuerKey* issuerKey) {
    // Verify the key...
    
    element_t exp;

    element_t vpk;
    element_init_G1(vpk, pub->pairing);
    element_pow_zn(vpk, pub->g_1, issuerKey->sk.x);

//    element_fprintf(stdout, "[RegisterPublicKey] PK %B\n", issuerKey->pk.y);
//    element_fprintf(stdout, "[RegisterPublicKey] Computed PK %B\n", vpk);


    if (element_cmp(vpk, issuerKey->pk.y) != 0)
        return NULL;

    PublicKeyCertificate* cert = (PublicKeyCertificate*) malloc(sizeof(PublicKeyCertificate));

    element_t w;

    element_init_Zr(w, pub->pairing);
    element_random(w);

    // init a_1
    element_init_G1(cert->a_1, pub->pairing);
    element_pow_zn(cert->a_1, pub->g_1, w);

    // init a_2
    element_init_G1(cert->a_2, pub->pairing);
    element_pow_zn(cert->a_2, cert->a_1, cakey->cask.t);

    // init a_3
    element_init_G1(cert->a_3, pub->pairing);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, issuerKey->sk.x, cakey->cask.s);
    element_mul(exp, exp, cakey->cask.t);
    element_add(exp, exp, cakey->cask.s);

    element_pow_zn(cert->a_3, cert->a_1, exp);

    element_clear(exp);

    // init a_4
    element_init_G1(cert->a_4, pub->pairing);
    element_pow_zn(cert->a_4, cert->a_1, issuerKey->sk.x);

    // init a_5
    element_init_G1(cert->a_5, pub->pairing);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, issuerKey->sk.x, cakey->cask.t);

    element_pow_zn(cert->a_5, cert->a_1, exp);

    element_clear(exp);


    return cert;
}

IECypherText* InitTag(IEPub *pub, IssuerPK *issuerPk, PublicKeyCertificate *cert, PlainText *pt) {
/*    assert(pub);
    assert(issuerPk);
    assert(cert);
    assert(pt);
*/
    element_t r, k;
    IECypherText* ct = (IECypherText*) malloc(sizeof(IECypherText));
    
//    fprintf(stdout, "[InitTag] Init Randomness...\n");

    element_init_Zr(r, pub->pairing);
    element_random(r);

    element_init_Zr(k, pub->pairing);
    element_random(k);

//    fprintf(stdout, "[InitTag] Randomize the certificate...\n");

    // randomize the certificate
    element_init_G1(ct->a_1, pub->pairing);
    element_pow_zn(ct->a_1, cert->a_1, r);
    
    element_init_G1(ct->a_2, pub->pairing);
    element_pow_zn(ct->a_2, cert->a_2, r);

    element_init_G1(ct->a_3, pub->pairing);
    element_pow_zn(ct->a_3, cert->a_3, r);

    element_init_G1(ct->a_4, pub->pairing);
    element_pow_zn(ct->a_4, cert->a_4, r);

    element_init_G1(ct->a_5, pub->pairing);
    element_pow_zn(ct->a_5, cert->a_5, r);

    // init c_1, c_2

//    fprintf(stdout, "[InitTag] Generate c_1...\n");

    element_init_G1(ct->c_1, pub->pairing);
    element_pow_zn(ct->c_1, pub->g_1, k);

//    fprintf(stdout, "[InitTag] Generate c_2...\n");

    element_init_G1(ct->c_2, pub->pairing);
    element_pow_zn(ct->c_2, issuerPk->y, k);
    element_mul(ct->c_2, ct->c_2, pt->m);

    // cleanup

//    fprintf(stdout, "[InitTag] clear r...\n");
    element_clear(r);
//    fprintf(stdout, "[InitTag] clear k...\n");
    element_clear(k);

    return ct;
}

IECypherText* IERandomize(IEPub *pub, CAKey *cakey, IECypherText* ct) {
    IECypherText* cypherText = ct;

    // verify the cyphertext..
    if (VerifyCertificate(pub, cakey, ct) != 0) {
        // the cyphertext is corrupted, use the dummy encryption C
        fprintf(stdout, "the cyphertext is corrupted, use the dummy encryption C\n");
        cypherText = cakey->capk.C;
    }

    // randomize it
    element_t v,z;

    element_init_Zr(v, pub->pairing);
    element_random(v);
    element_init_Zr(z, pub->pairing);
    element_random(z);

    IECypherText* rct = (IECypherText*) malloc(sizeof(IECypherText));

    // compute a_is

    element_init_G1(rct->a_1, pub->pairing);
    element_pow_zn(rct->a_1, cypherText->a_1, v);

    element_init_G1(rct->a_2, pub->pairing);
    element_pow_zn(rct->a_2, cypherText->a_2, v);

    element_init_G1(rct->a_3, pub->pairing);
    element_pow_zn(rct->a_3, cypherText->a_3, v);

    element_init_G1(rct->a_4, pub->pairing);
    element_pow_zn(rct->a_4, cypherText->a_4, v);

    element_init_G1(rct->a_5, pub->pairing);
    element_pow_zn(rct->a_5, cypherText->a_5, v);

    // compute c_is

    element_init_G1(rct->c_1, pub->pairing);
    element_pow_zn(rct->c_1, rct->a_1, z);
    element_mul(rct->c_1, rct->c_1, cypherText->c_1);

    element_init_G1(rct->c_2, pub->pairing);
    element_pow_zn(rct->c_2, rct->a_4, z);
    element_mul(rct->c_2, rct->c_2, cypherText->c_2);

    return rct;
}

PlainText* ReadAndDecrypt(IEPub *pub, CAKey *cakey, IssuerSK *issuerSk, IECypherText* ct) {
    // verify the cyphertext..
    if (VerifyCertificate(pub, cakey, ct) != 0)
        return NULL;

    // decrypt it
    PlainText* pt = (PlainText*)malloc(sizeof(PlainText));

    element_init_G1(pt->m, pub->pairing);

    element_pow_zn(pt->m, ct->c_1, issuerSk->x);
    element_div(pt->m, ct->c_2, pt->m);

    return pt;
}

int VerifyCertificate(IEPub *pub, CAKey *cakey, IECypherText* ct) {
    pairing_pp_t pp;
    element_t v_1, v_2;

    // compare e(a1,T) == e(a_2, g_2)

    element_init_GT(v_1, pub->pairing);
    pairing_pp_init(pp, ct->a_1, pub->pairing); // a_1
    pairing_pp_apply(v_1, cakey->capk.T, pp); // p_0 = e(a_1, T)
    pairing_pp_clear(pp); // don't need pp anymore

    element_init_GT(v_2, pub->pairing);
    pairing_pp_init(pp, ct->a_2, pub->pairing); // a_2
    pairing_pp_apply(v_2, pub->g_2, pp); // p_0 = e(a_2, g_2)
    pairing_pp_clear(pp); // don't need pp anymore

    if (element_cmp(v_1, v_2) != 0) {
        element_clear(v_1);
        element_clear(v_2);

        return -1;
    }

    element_clear(v_1);
    element_clear(v_2);

    // compare e(a_4,T) == e(a_5, g_2)

    element_init_GT(v_1, pub->pairing);
    pairing_pp_init(pp, ct->a_4, pub->pairing); // a_4
    pairing_pp_apply(v_1, cakey->capk.T, pp); // p_0 = e(a_4, T)
    pairing_pp_clear(pp); // don't need pp anymore

    element_init_GT(v_2, pub->pairing);
    pairing_pp_init(pp, ct->a_5, pub->pairing); // a_5
    pairing_pp_apply(v_2, pub->g_2, pp); // p_0 = e(a_5, g_2)
    pairing_pp_clear(pp); // don't need pp anymore

    if (element_cmp(v_1, v_2) != 0) {
        element_clear(v_1);
        element_clear(v_2);

        return -1;
    }

    element_clear(v_1);
    element_clear(v_2);


    // compare e(a_3,g_2) == e(a_1*a_5, S)

    element_init_GT(v_1, pub->pairing);
    pairing_pp_init(pp, ct->a_3, pub->pairing); // a_3
    pairing_pp_apply(v_1, pub->g_2, pp); // p_0 = e(a_3, g_2)
    pairing_pp_clear(pp); // don't need pp anymore

    element_init_GT(v_2, pub->pairing);

    element_t prod;
    element_init_G1(prod, pub->pairing);
    element_mul(prod, ct->a_1, ct->a_5);

    pairing_pp_init(pp, prod, pub->pairing); // a_1 * a_5
    pairing_pp_apply(v_2, cakey->capk.S, pp); // p_0 = e(a_1*a_5, S)
    pairing_pp_clear(pp); // don't need pp anymore
    element_clear(prod);

    if (element_cmp(v_1, v_2) != 0) {
        element_clear(v_1);
        element_clear(v_2);

        return -1;
    }

    element_clear(v_1);
    element_clear(v_2);

    return 0;
}