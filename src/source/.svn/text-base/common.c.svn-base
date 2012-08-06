/* 
 * File:   common.c
 * Author: angelo
 *
 * Created on August 16, 2010, 10:30 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "../header/common.h"

PlainText* fromString(pairing_t pairing, char* message) {
    PlainText *plainText = (PlainText *) malloc(sizeof (PlainText));
    element_init_GT(plainText->m, pairing);
    element_from_bytes(plainText->m, message);

    return plainText;
}

char* toString(pairing_t pairing, PlainText* message) {
    char* buffer = malloc(sizeof (char) *pairing_length_in_bytes_GT(pairing));
    element_to_bytes(buffer, message->m);

    return buffer;
}

unsigned char* ciphertext2bytes(pairing_t pairing, CipherText* ct) {
    int size = element_length_in_bytes(ct->C->C) +
            element_length_in_bytes(ct->C->C_0) * 4;

    unsigned char* data;
    data = malloc(sizeof (unsigned char) * size * 2);

    int length = element_to_bytes(&data[0], ct->C->C);

    length += element_to_bytes(&data[length], ct->C->C_0);
    length += element_to_bytes(&data[length], ct->C->C_1);
    length += element_to_bytes(&data[length], ct->C->C_2);
    length += element_to_bytes(&data[length], ct->C->C_3);

    length += element_to_bytes(&data[length], ct->U->C);
    length += element_to_bytes(&data[length], ct->U->C_0);
    length += element_to_bytes(&data[length], ct->U->C_1);
    length += element_to_bytes(&data[length], ct->U->C_2);
    length += element_to_bytes(&data[length], ct->U->C_3);

    return data;
}

CipherText* bytes2ciphertext(pairing_t pairing, unsigned char* buffer) {
    CipherText* ct = malloc(sizeof (CipherText));

    Enc *C = malloc(sizeof (Enc));

    element_init_GT(C->C, pairing);
    int length = element_from_bytes(C->C, &buffer[0]);

    element_init_G1(C->C_0, pairing);
    length += element_from_bytes(C->C_0, &buffer[length]);

    element_init_G1(C->C_1, pairing);
    length += element_from_bytes(C->C_1, &buffer[length]);

    element_init_G1(C->C_2, pairing);
    length += element_from_bytes(C->C_2, &buffer[length]);

    element_init_G1(C->C_3, pairing);
    length += element_from_bytes(C->C_3, &buffer[length]);
    ct->C = C;

    Enc *U = malloc(sizeof (Enc));

    element_init_GT(U->C, pairing);
    length += element_from_bytes(U->C, &buffer[length]);

    element_init_G1(U->C_0, pairing);
    length += element_from_bytes(U->C_0, &buffer[length]);

    element_init_G1(U->C_1, pairing);
    length += element_from_bytes(U->C_1, &buffer[length]);

    element_init_G1(U->C_2, pairing);
    length += element_from_bytes(U->C_2, &buffer[length]);

    element_init_G1(U->C_3, pairing);
    length += element_from_bytes(U->C_3, &buffer[length]);

    ct->U = U;

    return ct;
}

unsigned char* key2bytes(pairing_t pairing, Key* key) {
    int size = element_length_in_bytes(key->pk.pk) +
            element_length_in_bytes(key->sk.D_0) * 4;

    unsigned char* data;
    data = malloc(sizeof (unsigned char) * size);

    int length = element_to_bytes(&data[0], key->pk.pk);

    length += element_to_bytes(&data[length], key->sk.D_0);
    length += element_to_bytes(&data[length], key->sk.D_1);
    length += element_to_bytes(&data[length], key->sk.D_2);
    length += element_to_bytes(&data[length], key->sk.D_3);

    return data;
}

Key* bytes2key(pairing_t pairing, unsigned char* buffer) {
    Key* key = malloc(sizeof (Key));

    element_init_G1(key->pk.pk, pairing);
    int length = element_from_bytes(key->pk.pk, &buffer[0]);

    element_init_G1(key->sk.D_0, pairing);
    length += element_from_bytes(key->sk.D_0, &buffer[length]);

    element_init_G1(key->sk.D_1, pairing);
    length += element_from_bytes(key->sk.D_1, &buffer[length]);

    element_init_G1(key->sk.D_2, pairing);
    length += element_from_bytes(key->sk.D_2, &buffer[length]);

    element_init_G1(key->sk.D_3, pairing);
    length += element_from_bytes(key->sk.D_3, &buffer[length]);

    return key;
}

unsigned char* pk2bytes(pairing_t pairing, Pk* key) {
    int size = element_length_in_bytes(key->pk);

    unsigned char* data;
    data = malloc(sizeof (unsigned char) * size);

    int length = element_to_bytes(&data[0], key->pk);

    return data;
}

Pk* bytes2pk(pairing_t pairing, unsigned char* buffer) {
    Pk* key = malloc(sizeof (Pk));

    element_init_G1(key->pk, pairing);
    int length = element_from_bytes(key->pk, &buffer[0]);

    return key;
}

unsigned char* sk2bytes(pairing_t pairing, Sk* key) {
    int size = element_length_in_bytes(key->D_0) * 4;

    unsigned char* data;
    data = malloc(sizeof (unsigned char) * size);

    int length = element_to_bytes(&data[0], key->D_0);

    length += element_to_bytes(&data[length], key->D_1);
    length += element_to_bytes(&data[length], key->D_2);
    length += element_to_bytes(&data[length], key->D_3);

    return data;
}

Sk* bytes2sk(pairing_t pairing, unsigned char* buffer) {
    Sk* key = malloc(sizeof (Sk));

    element_init_G1(key->D_0, pairing);
    int length = element_from_bytes(key->D_0, &buffer[0]);

    element_init_G1(key->D_1, pairing);
    length += element_from_bytes(key->D_1, &buffer[length]);

    element_init_G1(key->D_2, pairing);
    length += element_from_bytes(key->D_2, &buffer[length]);

    element_init_G1(key->D_3, pairing);
    length += element_from_bytes(key->D_3, &buffer[length]);

    return key;
}

unsigned char* pub2bytes(pairing_t pairing, Pub* pub) {
    int size = element_length_in_bytes(pub->omega) +
            element_length_in_bytes(pub->T_1) * 6;

    unsigned char* data;
    data = malloc(sizeof (unsigned char) * size);

    int length = element_to_bytes(&data[0], pub->omega);

    length += element_to_bytes(&data[length], pub->T_1);
    length += element_to_bytes(&data[length], pub->T_2);
    length += element_to_bytes(&data[length], pub->T_3);
    length += element_to_bytes(&data[length], pub->g);
    length += element_to_bytes(&data[length], pub->g_0);
    length += element_to_bytes(&data[length], pub->g_1);

    return data;
}

Pub* bytes2pub(pairing_t pairing, unsigned char* buffer) {
    Pub* pub = malloc(sizeof (Pub));

    pub->pairing[0] = pairing[0];

    element_init_GT(pub->omega, pairing);
    int length = element_from_bytes(pub->omega, &buffer[0]);

    element_init_G1(pub->T_1, pairing);
    length += element_from_bytes(pub->T_1, &buffer[length]);

    element_init_G1(pub->T_2, pairing);
    length += element_from_bytes(pub->T_2, &buffer[length]);

    element_init_G1(pub->T_3, pairing);
    length += element_from_bytes(pub->T_3, &buffer[length]);

    element_init_G1(pub->g, pairing);
    length += element_from_bytes(pub->g, &buffer[length]);

    element_init_G1(pub->g_0, pairing);
    length += element_from_bytes(pub->g_0, &buffer[length]);

    element_init_G1(pub->g_1, pairing);
    length += element_from_bytes(pub->g_1, &buffer[length]);

    return pub;
}

unsigned char* msk2bytes(pairing_t pairing, Msk* msk) {
    int size = element_length_in_bytes(msk->t_1) * 4;

    unsigned char* data;
    data = malloc(sizeof (unsigned char) * size);

    int length = element_to_bytes(&data[0], msk->t_1);

    length += element_to_bytes(&data[length], msk->t_2);
    length += element_to_bytes(&data[length], msk->t_3);
    length += element_to_bytes(&data[length], msk->w);

    return data;
}

Msk* bytes2msk(pairing_t pairing, unsigned char* buffer) {
    Msk* msk = malloc(sizeof (Msk));

    element_init_Zr(msk->t_1, pairing);
    int length = element_from_bytes(msk->t_1, &buffer[0]);

    element_init_Zr(msk->t_2, pairing);
    length += element_from_bytes(msk->t_2, &buffer[length]);

    element_init_Zr(msk->t_3, pairing);
    length += element_from_bytes(msk->t_3, &buffer[length]);

    element_init_Zr(msk->w, pairing);
    length += element_from_bytes(msk->w, &buffer[length]);

    return msk;
}


unsigned char* pubtriple2bytes(pairing_t pairing, PubTriple* pub) {
    int size = pubtriple_size(pairing);

    unsigned char* data;
    data = malloc(sizeof (unsigned char) * size);

    int length = element_to_bytes(&data[0], pub->pub.omega);
    length += element_to_bytes(&data[length], pub->pub.T_1);
    length += element_to_bytes(&data[length], pub->pub.T_2);
    length += element_to_bytes(&data[length], pub->pub.T_3);
    length += element_to_bytes(&data[length], pub->pub.g);
    length += element_to_bytes(&data[length], pub->pub.g_0);
    length += element_to_bytes(&data[length], pub->pub.g_1);

    length += element_to_bytes(&data[length], pub->msk.t_1);
    length += element_to_bytes(&data[length], pub->msk.t_2);
    length += element_to_bytes(&data[length], pub->msk.t_3);
    length += element_to_bytes(&data[length], pub->msk.w);

    return data;
}

PubTriple* bytes2pubtriple(pairing_t pairing, unsigned char* buffer) {
    PubTriple* pubt = malloc(sizeof (PubTriple));

    pubt->pub = *bytes2pub(pairing, buffer);
    pubt->msk = *bytes2msk(pairing, &buffer[pub_size(pairing)]);
    
    return pubt;
}


int ciphertext_size(pairing_t pairing) {
    return (pairing_length_in_bytes_GT(pairing) +
            pairing_length_in_bytes_G1(pairing) * 4) * 2;
}

int key_size(pairing_t pairing) {
    return (pairing_length_in_bytes_G1(pairing) * 5);
}

int pk_size(pairing_t pairing) {
    return pairing_length_in_bytes_G1(pairing);
}

int sk_size(pairing_t pairing) {
    return pairing_length_in_bytes_G1(pairing) * 4;
}

int pub_size(pairing_t pairing) {
    return (pairing_length_in_bytes_GT(pairing) +
            pairing_length_in_bytes_G1(pairing) * 6);
}

int msk_size(pairing_t pairing) {
    return pairing_length_in_bytes_Zr(pairing) * 4;
}

int pubtriple_size(pairing_t pairing) {
    return pub_size(pairing) + msk_size(pairing);
}


void clear_ciphertext( CipherText* ct){
element_clear (ct->C->C);
element_clear (ct->C->C_0);
element_clear (ct->C->C_1);
element_clear (ct->C->C_2);
element_clear (ct->C->C_3);
element_clear (ct->U->C);
element_clear (ct->U->C_0);
element_clear (ct->U->C_1);
element_clear (ct->U->C_2);
element_clear (ct->U->C_3);
}

void clear_plaintext(PlainText* message){
element_clear(message->m);
}

void clear_key(Key* key){
element_clear(key->pk.pk);
element_clear(key->sk.D_0);
element_clear(key->sk.D_1);
element_clear(key->sk.D_2);
element_clear(key->sk.D_3);
}




void store(unsigned char* buffer, int len, char* filename) {
    FILE *file;
    file = fopen(filename, "w");

    fwrite(buffer, sizeof (unsigned char), len, file);

    fclose(file);
}

void load(unsigned char* buffer, int len, char* filename) {
    FILE *file;
    file = fopen(filename, "r");
	if (file == 0) {printf("Error in loading file. Copy needed files in the folder.\n");exit(1);}

  fread(buffer, sizeof (unsigned char), len, file);
	
    fclose(file);
	
}
