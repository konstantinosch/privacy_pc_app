/* 
 * File:   common.h
 * Author: angelo
 *
 * Created on May 19, 2009, 11:01 AM
 */

#ifndef _COMMON_H
#define	_COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <pbc.h>

    typedef struct {
        element_t C;
        element_t C_0;
        element_t C_1;
        element_t C_2;
        element_t C_3;
    } Enc;

    typedef struct {
        Enc *C;
        Enc *U;
    } CipherText;

    typedef struct {
        element_t m;
    } PlainText;

    typedef struct {
        pairing_t pairing;

        element_t g;
        element_t g_0;
        element_t g_1;
        element_t omega;
        element_t T_1, T_2, T_3;
    } Pub;

    typedef struct {
        element_t t_1;
        element_t t_2;
        element_t t_3;
        element_t w;
    } Msk;

    typedef struct {
        Pub pub;
        Msk msk;
    } PubTriple;

    typedef struct {
        element_t pk;
    } Pk;

    typedef struct {
        element_t D_0;
        element_t D_1;
        element_t D_2;
        element_t D_3;
    } Sk;

    typedef struct {
        Pk pk;
        Sk sk;
    } Key;


	

    PlainText* fromString(pairing_t pairing, char* message);

    char* toString(pairing_t pairing, PlainText* message);


    unsigned char * ciphertext2bytes(pairing_t pairing, CipherText* ct);

    CipherText* bytes2ciphertext(pairing_t pairing, unsigned char* buffer);


    unsigned char* key2bytes(pairing_t pairing, Key* key);

    Key* bytes2key(pairing_t pairing, unsigned char* buffer);


    unsigned char* pk2bytes(pairing_t pairing, Pk* key);

    Pk* bytes2pk(pairing_t pairing, unsigned char* buffer);


    unsigned char* sk2bytes(pairing_t pairing, Sk* key);

    Sk* bytes2sk(pairing_t pairing, unsigned char* buffer);


    unsigned char* pub2bytes(pairing_t pairing, Pub* key);

    Pub* bytes2pub(pairing_t pairing, unsigned char* buffer);


    unsigned char* msk2bytes(pairing_t pairing, Msk* key);

    Msk* bytes2msk(pairing_t pairing, unsigned char* buffer);


    int ciphertext_size(pairing_t pairing);

    int key_size(pairing_t pairing);

    int pk_size(pairing_t pairing);

    int sk_size(pairing_t pairing);

    int msk_size(pairing_t pairing);

    int pub_size(pairing_t pairing);

    int pubtriple_size(pairing_t pairing);
	
	void clear_ciphertext( CipherText* ct);
	
	void clear_plaintext(PlainText* message);

	void clear_key(Key* key);




    void store(unsigned char* buffer, int len, char* filename);

    void load(unsigned char* buffer, int len, char* filename);
    
#ifdef	__cplusplus
}
#endif

#endif	/* _COMMON_H */

