#include "utma_weak.h"

PubTriple* GenPub(pairing_t *pairing) {
    PubTriple* pubTriple;
    element_t exp;
    pairing_pp_t pp;

    // TODO: verify the pairing is symmetric....

//    fprintf(stdout, "[GenPub] Allocate the space for pubTrible.\n");

    pubTriple = (PubTriple *) malloc(sizeof(PubTriple));
    pubTriple->pub.pairing[0] = *pairing[0];

//    fprintf(stdout, "[GenPub] Space allocated.\n");

    // Init elments
//    fprintf(stdout, "[GenPub] Init elements.\n");

    element_init_Zr(pubTriple->msk.t_1, *pairing);
    element_init_Zr(pubTriple->msk.t_2, *pairing);
    element_init_Zr(pubTriple->msk.t_3, *pairing);
    element_init_Zr(pubTriple->msk.w, *pairing);

    element_init_G1(pubTriple->pub.g, *pairing);
    element_init_G1(pubTriple->pub.g_0, *pairing);
    element_init_G1(pubTriple->pub.g_1, *pairing);

    element_init_GT(pubTriple->pub.omega, *pairing);
    element_init_G1(pubTriple->pub.T_1, *pairing);
    element_init_G1(pubTriple->pub.T_2, *pairing);
    element_init_G1(pubTriple->pub.T_3, *pairing);

//    fprintf(stdout, "[GenPub] Elements initialized.\n");

    // Chose at random...
//    fprintf(stdout, "[GenPub] Assign random values to the elements.\n");

    element_random(pubTriple->msk.t_1);
    element_random(pubTriple->msk.t_2);
    element_random(pubTriple->msk.t_3);
    element_random(pubTriple->msk.w);

    element_random(pubTriple->pub.g);
    element_random(pubTriple->pub.g_0);
    element_random(pubTriple->pub.g_1);

//    element_fprintf(stdout, "[GenPub] g = %B\n", pubTriple->pub.g);
//    fprintf(stdout, "[GenPub] Assign random values to the elements. Finished\n");

    // Compute omega...
//    fprintf(stdout, "[GenPub] Compute omega.\n");

    pairing_pp_init(pp, pubTriple->pub.g, *pairing); // g is some element of G1
    pairing_pp_apply(pubTriple->pub.omega, pubTriple->pub.g, pp); // omega = e(g, g)
    pairing_pp_clear(pp); // don't need pp anymore

    element_init_Zr(exp, *pairing);
    element_mul(exp, pubTriple->msk.w, pubTriple->msk.t_1);
    element_mul(exp, exp, pubTriple->msk.t_2);
    element_mul(exp, exp, pubTriple->msk.t_3);

    element_pow_zn(pubTriple->pub.omega, pubTriple->pub.omega, exp);
    element_clear(exp);

//    fprintf(stdout, "Compute omega. Finished.\n");

    // Compute T_i
//    fprintf(stdout, "Compute T_i.\n");

    element_pow_zn(pubTriple->pub.T_1, pubTriple->pub.g, pubTriple->msk.t_1);
    element_pow_zn(pubTriple->pub.T_2, pubTriple->pub.g, pubTriple->msk.t_2);
    element_pow_zn(pubTriple->pub.T_3, pubTriple->pub.g, pubTriple->msk.t_3);
    
//    fprintf(stdout, "Compute T_i. Finished.\n");

    return pubTriple;
}

Key* GenKey(Pub *pub, Msk *msk) {
    Key* key;

    element_t r;
    element_t D_1_1, D_1_2;
    element_t D_2_1, D_2_2;
    element_t D_3_1, D_3_2;
    element_t exp;

    key = (Key *) malloc(sizeof(Key));

    // Choose r at random
    element_init_Zr(r, pub->pairing);
    element_random(r);

    // Generate Pk, g_0 * g_1^r
    element_init_G1(key->pk.pk, pub->pairing);
    element_pow_zn(key->pk.pk, pub->g_1, r);
    element_mul(key->pk.pk, pub->g_0, key->pk.pk);

    // Generate D_0 = g^(r * t_1 *t_2 *t_3)

    element_init_G1(key->sk.D_0, pub->pairing);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, r, msk->t_1);
    element_mul(exp, exp, msk->t_2);
    element_mul(exp, exp, msk->t_3);
    element_pow_zn(key->sk.D_0, pub->g, exp);
    element_clear(exp);

    // Generate D_1 = g^-(w * t_1 *t_3) * Pk^-(r * t_1 * t_3)

    element_init_G1(key->sk.D_1, pub->pairing);
    element_init_G1(D_1_1, pub->pairing);
    element_init_G1(D_1_2, pub->pairing);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, msk->w, msk->t_1);
    element_mul(exp, exp, msk->t_3);
    element_neg(exp, exp);
    element_pow_zn(D_1_1, pub->g, exp);
    element_clear(exp);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, r, msk->t_1);
    element_mul(exp, exp, msk->t_3);
    element_neg(exp, exp);
    element_pow_zn(D_1_2, key->pk.pk, exp);
    element_clear(exp);

    element_mul(key->sk.D_1, D_1_1, D_1_2);
    element_clear(D_1_1);
    element_clear(D_1_2);

    // Generate D_2 = g^-(w * t_1 *t_2) * Pk^-(r * t_1 * t_2)

    element_init_G1(key->sk.D_2, pub->pairing);
    element_init_G1(D_2_1, pub->pairing);
    element_init_G1(D_2_2, pub->pairing);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, msk->w, msk->t_1);
    element_mul(exp, exp, msk->t_2);
    element_neg(exp, exp);
    element_pow_zn(D_2_1, pub->g, exp);
    element_clear(exp);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, r, msk->t_1);
    element_mul(exp, exp, msk->t_2);
    element_neg(exp, exp);

    element_pow_zn(D_2_2, key->pk.pk, exp);
    element_clear(exp);

    element_mul(key->sk.D_2, D_2_1, D_2_2);
    element_clear(D_2_1);
    element_clear(D_2_2);

    // Generate D_3 = g^-(w * t_2 *t_3) * Pk^-(r * t_2 * t_3)

    element_init_G1(key->sk.D_3, pub->pairing);
    element_init_G1(D_3_1, pub->pairing);
    element_init_G1(D_3_2, pub->pairing);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, msk->w, msk->t_2);
    element_mul(exp, exp, msk->t_3);
    element_neg(exp, exp);
    element_pow_zn(D_3_1, pub->g, exp);
    element_clear(exp);

    element_init_Zr(exp, pub->pairing);
    element_mul(exp, r, msk->t_2);
    element_mul(exp, exp, msk->t_3);
    element_neg(exp, exp);
    element_pow_zn(D_3_2, key->pk.pk, exp);
    element_clear(exp);

    element_mul(key->sk.D_3, D_3_1, D_3_2);
    element_clear(D_3_1);
    element_clear(D_3_2);

    // Cleanup
    element_clear(r);

    return key;
}

CipherText* rE(Pub *pub, Pk *pk, PlainText *M) {
    CipherText* ct;
    PlainText one;

//    element_fprintf(stdout, "[rE] PlainText = %B\n", M->m);

    ct = (CipherText *) malloc(sizeof(CipherText));

    element_init_GT(one.m, pub->pairing);
    element_set1(one.m);

    ct->C = enc(pub, pk, M);
    ct->U = enc(pub, pk, &one);

    element_clear(one.m);

    return ct;
}

PlainText* rD(Pub *pub, Sk *sk, CipherText *Ct) {
    return dec(pub, sk, Ct->C);
}

Enc* enc(Pub *pub, Pk *pk, PlainText *M) {
    Enc* result;
    element_t exp, s, s_1, s_2;

//    element_fprintf(stdout, "[enc] PlainText = %B\n", M->m);

    result = (Enc *) malloc(sizeof(Enc));

//    fprintf(stdout, "[enc] Choose s, s_1 and s_2 at random.\n");

    // Choose s, s_1 and s_2 at random
    element_init_Zr(s, pub->pairing);
    element_random(s);
    element_init_Zr(s_1, pub->pairing);
    element_random(s_1);
    element_init_Zr(s_2, pub->pairing);
    element_random(s_2);

    // Compute C = omega^s * M
//    fprintf(stdout, "[enc] Compute C.\n");

    element_init_GT(result->C, pub->pairing);
    element_pow_zn(result->C, pub->omega, s);
    element_mul(result->C, result->C, M->m);
    
    // Compute C_0 = Pk^s
//    fprintf(stdout, "[enc] Compute C_0.\n");

    element_init_G1(result->C_0, pub->pairing);
    element_pow_zn(result->C_0, pk->pk, s);

    // Compute C_1 = T_2^s_2
//    fprintf(stdout, "[enc] Compute C_1.\n");

    element_init_G1(result->C_1, pub->pairing);
    element_pow_zn(result->C_1, pub->T_2, s_2);

    // Compute C_2 = T_3^(s-s_1-s_2)
//    fprintf(stdout, "[enc] Compute C_2.\n");

    element_init_G1(result->C_2, pub->pairing);

    element_init_Zr(exp, pub->pairing);
    element_sub(exp, s, s_1);
    element_sub(exp, exp, s_2);
    element_pow_zn(result->C_2, pub->T_3, exp);

    element_clear(exp);

    // Compute C_3 = T_1^s_1
//    fprintf(stdout, "[enc] Compute C_3.\n");

    element_init_G1(result->C_3, pub->pairing);
    element_pow_zn(result->C_3, pub->T_1, s_1);

    element_clear(s);
    element_clear(s_1);
    element_clear(s_2);

    /*
    element_fprintf(stdout, "[enc] C = %B\n", result->C);
    element_fprintf(stdout, "[enc] C_0 = %B\n", result->C_0);
    element_fprintf(stdout, "[enc] C_1 = %B\n", result->C_1);
    element_fprintf(stdout, "[enc] C_2 = %B\n", result->C_2);
    element_fprintf(stdout, "[enc] C_3 = %B\n", result->C_3);
    */
    return result;
}

PlainText* dec(Pub *pub, Sk *sk, Enc *enc) {
    pairing_pp_t pp;
    element_t p_0, p_1, p_2, p_3;
    PlainText* result;

    /*
    element_fprintf(stdout, "[dec] C = %B\n", enc.C);
    element_fprintf(stdout, "[dec] C_0 = %B\n", enc.C_0);
    element_fprintf(stdout, "[dec] C_1 = %B\n", enc.C_1);
    element_fprintf(stdout, "[dec] C_2 = %B\n", enc.C_2);
    element_fprintf(stdout, "[dec] C_3 = %B\n", enc.C_3);
     */

    // Compute p_0 = e(C_0, D_0)
//    fprintf(stdout, "[dec] Compute p_0.\n");

    element_init_GT(p_0, pub->pairing);
    pairing_pp_init(pp, enc->C_0, pub->pairing); // C_0 is some element of G1
    pairing_pp_apply(p_0, sk->D_0, pp); // p_0 = e(C_0, D_0)
    pairing_pp_clear(pp); // don't need pp anymore

    // Compute p_1 = e(C_1, D_1)
//    fprintf(stdout, "[dec] Compute p_1.\n");

    element_init_GT(p_1, pub->pairing);
    pairing_pp_init(pp, enc->C_1, pub->pairing); // C_1 is some element of G1
    pairing_pp_apply(p_1, sk->D_1, pp); // p_1 = e(C_1, D_1)
    pairing_pp_clear(pp); // don't need pp anymore

    // Compute p_2 = e(C_2, D_2)
//    fprintf(stdout, "[dec] Compute p_2.\n");

    element_init_GT(p_2, pub->pairing);
    pairing_pp_init(pp, enc->C_2, pub->pairing); // C_2 is some element of G1
    pairing_pp_apply(p_2, sk->D_2, pp); // p_2 = e(C_2, D_2)
    pairing_pp_clear(pp); // don't need pp anymore

    // Compute p_3 = e(C_3, D_3)
//    fprintf(stdout, "[dec] Compute p_3.\n");

    element_init_GT(p_3, pub->pairing);
    pairing_pp_init(pp, enc->C_3, pub->pairing); // C_3 is some element of G1
    pairing_pp_apply(p_3, sk->D_3, pp); // p_3 = e(C_3, D_3)
    pairing_pp_clear(pp); // don't need pp anymore

    // Compute m
//    fprintf(stdout, "[dec] Compute m.\n");

    result = (PlainText *) malloc(sizeof(PlainText));
    element_init_GT(result->m, pub->pairing);
    element_mul(result->m, enc->C, p_0);
    element_mul(result->m, result->m, p_1);
    element_mul(result->m, result->m, p_2);
    element_mul(result->m, result->m, p_3);

    element_clear(p_0);
    element_clear(p_1);
    element_clear(p_2);
    element_clear(p_3);

    return result;
}

CipherText* Randomize(Pub *pub, CipherText *Ct) {
    CipherText* rct;
    Enc* U_star;
    Enc* U_star_star;
    Enc* C_hat;

    // Compute U*
//    fprintf(stdout, "[Randomize] Compute U*.\n");

    U_star = star(pub, Ct->U);

//    fprintf(stdout, "[Randomize] Compute U*. Finished.\n");

    // Compute C_hat
//    fprintf(stdout, "[Randomize] Compute C hat.\n");

    C_hat = (Enc *) malloc(sizeof(Enc));

    element_init_GT(C_hat->C, pub->pairing);
    element_mul(C_hat->C, Ct->C->C, U_star->C);

    element_init_G1(C_hat->C_0, pub->pairing);
    element_mul(C_hat->C_0, Ct->C->C_0, U_star->C_0);

    element_init_G1(C_hat->C_1, pub->pairing);
    element_mul(C_hat->C_1, Ct->C->C_1, U_star->C_1);

    element_init_G1(C_hat->C_2, pub->pairing);
    element_mul(C_hat->C_2, Ct->C->C_2, U_star->C_2);

    element_init_G1(C_hat->C_3, pub->pairing);
    element_mul(C_hat->C_3, Ct->C->C_3, U_star->C_3);

//    fprintf(stdout, "[Randomize] Compute C hat. Finished.\n");

    U_star_star = star(pub, U_star);

    rct = (CipherText *) malloc(sizeof(CipherText));
    rct->C = C_hat;
    rct->U = U_star_star;

    free(U_star);

    return rct;
}

Enc* star(Pub *pub, Enc *encp) {
    element_t r, r_2, r_3;
    element_t t_1, t_2, exp;
    Enc* U_star;

    // Init randomness
    element_init_Zr(r, pub->pairing);
    element_random(r);
    element_init_Zr(r_2, pub->pairing);
    element_random(r_2);
    element_init_Zr(r_3, pub->pairing);
    element_random(r_3);

//    element_fprintf(stdout, "[star] r = %B\n", r);
//    element_fprintf(stdout, "[star] r_2 = %B\n", r_2);
//    element_fprintf(stdout, "[star] r_3 = %B\n", r_3);

    // Compute U*
    U_star = (Enc *) malloc(sizeof(Enc));

    // Compute U*.C
    element_init_GT(U_star->C, pub->pairing);
    element_pow_zn(U_star->C, encp->C, r);

//    element_fprintf(stdout, "[star] U_star->C = %B\n", U_star->C);

    // Compute U*.C_0
    element_init_G1(U_star->C_0, pub->pairing);
    element_pow_zn(U_star->C_0, encp->C_0, r);

//    element_fprintf(stdout, "[star] U_star->C_0 = %B\n", U_star->C_0);

    // Compute U*.C_1
    element_init_G1(t_1, pub->pairing);
    element_init_G1(t_2, pub->pairing);
    element_init_G1(U_star->C_1, pub->pairing);

    element_pow_zn(t_1, encp->C_1, r);
    element_pow_zn(t_2, pub->T_2, r_2);
    element_mul(U_star->C_1, t_1, t_2);

    element_clear(t_1);
    element_clear(t_2);

//    element_fprintf(stdout, "[star] U_star->C_1 = %B\n", U_star->C_1);

    // Compute U*.C_2
    element_init_G1(t_1, pub->pairing);
    element_init_G1(t_2, pub->pairing);
    element_init_G1(U_star->C_2, pub->pairing);

    element_pow_zn(t_1, encp->C_2, r);
    element_pow_zn(t_2, pub->T_3, r_3);
    element_mul(U_star->C_2, t_1, t_2);

    element_clear(t_1);
    element_clear(t_2);

//    element_fprintf(stdout, "[star] U_star->C_2 = %B\n", U_star->C_2);

    // Compute U*.C_3
    element_init_G1(t_1, pub->pairing);
    element_init_G1(t_2, pub->pairing);
    element_init_G1(U_star->C_3, pub->pairing);
    element_init_Zr(exp, pub->pairing);

    element_pow_zn(t_1, encp->C_3, r);

    element_add(exp, r_2, r_3);
    element_neg(exp, exp);
    element_pow_zn(t_2, pub->T_1, exp);

    element_mul(U_star->C_3, t_1, t_2);

    element_clear(t_1);
    element_clear(t_2);

    element_clear(r);
    element_clear(r_2);
    element_clear(r_3);

//    element_fprintf(stdout, "[star] U_star->C_3 = %B\n", U_star->C_3);

    /*    element_set(U_star->C, encp.C);
    element_set(U_star->C_0, encp.C_0);
    element_set(U_star->C_1, encp.C_1);
    element_set(U_star->C_2, encp.C_2);
    element_set(U_star->C_3, encp.C_3);
*/
/*    fprintf(stdout, "\n");
    element_fprintf(stdout, "[star] U_star->C = %B\n", U_star->C);
    element_fprintf(stdout, "[star] U_star->C_0 = %B\n", U_star->C_0);
    element_fprintf(stdout, "[star] U_star->C_1 = %B\n", U_star->C_1);
    element_fprintf(stdout, "[star] U_star->C_2 = %B\n", U_star->C_2);
    element_fprintf(stdout, "[star] U_star->C_3 = %B\n", U_star->C_3);
*/

    return U_star;
}

void freeCypherText(CipherText* ct) {
    free(ct->C);
    free(ct->U);
    free(ct);
}
