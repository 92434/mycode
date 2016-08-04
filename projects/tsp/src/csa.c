//////////////////////////////////////////   
//////////////////////////////////////////       
//////////////////////////////////////////   
//////////////////////////////////////////   
//////////////////////////////////////////   
/// Ahmed Alfaresi                 ///   
/// Zia Kamawal                    ///   
///                                    ///   
/// ECE 575 Final Project          ///   
///                                    ///   
/// Common Scrambling Alorithm     ///   
//////////////////////////////////////////   
//////////////////////////////////////////   
//////////////////////////////////////////   
//////////////////////////////////////////   
//////////////////////////////////////////   
//////////////////////////////////////////   
   
   
   
   
   
   
/****************************************************************************  
  
This code was taken from csa.irde.to.  It was made optimized and usable for out project.  We created the user interface and allow for creating and reading of files.  We have also incorporated a functionality that check two files for equivalency.  This does the quivalency check in a way that would ensure that the same data data is always recoginzed as the same.  We also have the functionality of receiving characters or 8 bit hex values in the form 0xFF.  
  
Basically the user is given three options: encrypt, decrypt and compare.  
  
encrypt:  
Will first ask the user for a file in the current directory that contains a plaintext, up to 256 byte hex value.  Then will ask for a file that contains a key, 8 byte hex value.  The program will then encrypt the plaintext with the key and will output the ciphertext.  The user will then be prompted to give a new filename to where the ciphertext will be saved.  The user will then be directed back to the main menu.  
  
decrypt:  
Will first ask the user for a file in the current directory that contains a ciphertext, up to 256 byte hex value.  Then will ask for a file that contains a key, 8 byte hex value.  The program will then decrypt the ciphertext with the key and will output the original plaintext.  The user will then be prompted to give a new filename to where the plaintext will be saved.  The user will then be directed back to the main menu.  
  
compare:  
Will ask the user to input two filenames to be compared.  The program will compare the two files.  If they are the same, it will display PASSED!!.  If they are different, it will display FAILED!!.  
  
******************************************************************************/   
   
   
   
   
   
   
/*  
    dvb common scrambling algorithm  
  
    refs:  
        uk patent: gb 2322 994  
        uk patent: gb 2322 995  
        freedec v2.1  
        iso/iec 13818-1  
        etr289 / dvb document a007  
  
*/   
   
   
   
#include <stdio.h>   
#include "csa.h"   
   
   
//stream cypher   
   
// 107 state bits   
// 26 nibbles (4 bit)   
// +  3 bits   
// reg A[1]-A[10], 10 nibbles   
// reg B[1]-B[10], 10 nibbles   
// reg X,           1 nibble   
// reg Y,           1 nibble   
// reg Z,           1 nibble   
// reg D,           1 nibble   
// reg E,           1 nibble   
// reg F,           1 nibble   
// reg p,           1 bit   
// reg q,           1 bit   
// reg r,           1 bit   
   
int sbox1[0x20] = {2,0,1,1,2,3,3,0, 3,2,2,0,1,1,0,3, 0,3,3,0,2,2,1,1, 2,2,0,3,1,1,3,0};   
int sbox2[0x20] = {3,1,0,2,2,3,3,0, 1,3,2,1,0,0,1,2, 3,1,0,3,3,2,0,2, 0,0,1,2,2,1,3,1};   
int sbox3[0x20] = {2,0,1,2,2,3,3,1, 1,1,0,3,3,0,2,0, 1,3,0,1,3,0,2,2, 2,0,1,2,0,3,3,1};   
int sbox4[0x20] = {3,1,2,3,0,2,1,2, 1,2,0,1,3,0,0,3, 1,0,3,1,2,3,0,3, 0,3,2,0,1,2,2,1};   
int sbox5[0x20] = {2,0,0,1,3,2,3,2, 0,1,3,3,1,0,2,1, 2,3,2,0,0,3,1,1, 1,0,3,2,3,1,0,2};   
int sbox6[0x20] = {0,1,2,3,1,2,2,0, 0,1,3,0,2,3,1,3, 2,3,0,2,3,0,1,1, 2,1,1,2,0,3,3,0};   
int sbox7[0x20] = {0,3,2,2,3,0,0,1, 3,0,1,3,1,2,2,1, 1,0,3,3,0,1,1,2, 2,3,1,0,2,3,0,2};   
   
int stream_cypher(int init, unsigned char *CK, unsigned char *sb, unsigned char *cb)   
{   
    int i,j;   
    int in1;        // most  significant nibble of input byte   
    int in2;        // least significant nibble of input byte   
    int op;   
    int extra_B;   
    int s1,s2,s3,s4,s5,s6,s7;   
    int next_A1;   
    int next_B1;   
    int next_E;   
   
    static int A[11];   
    static int B[11];   
    static int X;   
    static int Y;   
    static int Z;   
    static int D;   
    static int E;   
    static int F;   
    static int p;   
    static int q;   
    static int r;   
   
    // reset   
    if (init)   
    {   
        // load first 32 bits of CK into A[1]..A[8]   
        // load last  32 bits of CK into B[1]..B[8]   
        // all other regs = 0   
        A[1] = (CK[0] >> 4) & 0xf;   
        A[2] = (CK[0] >> 0) & 0xf;   
        A[3] = (CK[1] >> 4) & 0xf;   
        A[4] = (CK[1] >> 0) & 0xf;   
        A[5] = (CK[2] >> 4) & 0xf;   
        A[6] = (CK[2] >> 0) & 0xf;   
        A[7] = (CK[3] >> 4) & 0xf;   
        A[8] = (CK[3] >> 0) & 0xf;   
        A[9] = 0;   
        A[10] = 0;   
        B[1] = (CK[4] >> 4) & 0xf;   
        B[2] = (CK[4] >> 0) & 0xf;   
        B[3] = (CK[5] >> 4) & 0xf;   
        B[4] = (CK[5] >> 0) & 0xf;   
        B[5] = (CK[6] >> 4) & 0xf;   
        B[6] = (CK[6] >> 0) & 0xf;   
        B[7] = (CK[7] >> 4) & 0xf;   
        B[8] = (CK[7] >> 0) & 0xf;   
        B[9] = 0;   
        B[10] = 0;   
   
        X=0;   
        Y=0;   
        Z=0;   
        D=0;   
        E=0;   
        F=0;   
        p=0;   
        q=0;   
        r=0;   
    }   
   
    // 8 bytes per operation   
    for(i=0; i<8; i++)   
    {   
        if (init)   
        {   
            in1 = (sb[i] >> 4) & 0x0f;   
            in2 = (sb[i] >> 0) & 0x0f;   
        }   
        op = 0;   
        // 2 bits per iteration   
        for(j=0; j<4; j++)   
        {   
            // from A[1]..A[10], 35 bits are selected as inputs to 7 s-boxes   
            // 5 bits input per s-box, 2 bits output per s-box   
            s1 = sbox1[ (((A[4]>>0)&1)<<4) | (((A[1]>>2)&1)<<3) | (((A[6]>>1)&1)<<2) | (((A[7]>>3)&1)<<1) | (((A[9]>>0)&1)<<0) ];   
            s2 = sbox2[ (((A[2]>>1)&1)<<4) | (((A[3]>>2)&1)<<3) | (((A[6]>>3)&1)<<2) | (((A[7]>>0)&1)<<1) | (((A[9]>>1)&1)<<0) ];   
            s3 = sbox3[ (((A[1]>>3)&1)<<4) | (((A[2]>>0)&1)<<3) | (((A[5]>>1)&1)<<2) | (((A[5]>>3)&1)<<1) | (((A[6]>>2)&1)<<0) ];   
            s4 = sbox4[ (((A[3]>>3)&1)<<4) | (((A[1]>>1)&1)<<3) | (((A[2]>>3)&1)<<2) | (((A[4]>>2)&1)<<1) | (((A[8]>>0)&1)<<0) ];   
            s5 = sbox5[ (((A[5]>>2)&1)<<4) | (((A[4]>>3)&1)<<3) | (((A[6]>>0)&1)<<2) | (((A[8]>>1)&1)<<1) | (((A[9]>>2)&1)<<0) ];   
            s6 = sbox6[ (((A[3]>>1)&1)<<4) | (((A[4]>>1)&1)<<3) | (((A[5]>>0)&1)<<2) | (((A[7]>>2)&1)<<1) | (((A[9]>>3)&1)<<0) ];   
            s7 = sbox7[ (((A[2]>>2)&1)<<4) | (((A[3]>>0)&1)<<3) | (((A[7]>>1)&1)<<2) | (((A[8]>>2)&1)<<1) | (((A[8]>>3)&1)<<0) ];   
   
            // use 4x4 xor to produce extra nibble for T3   
            extra_B = ( ((B[3]&1)<<3) ^ ((B[6]&2)<<2) ^ ((B[7]&4)<<1) ^ ((B[9]&8)>>0) ) |   
                      ( ((B[6]&1)<<2) ^ ((B[8]&2)<<1) ^ ((B[3]&8)>>1) ^ ((B[4]&4)>>0) ) |   
                      ( ((B[5]&8)>>2) ^ ((B[8]&4)>>1) ^ ((B[4]&1)<<1) ^ ((B[5]&2)>>0) ) |   
                      ( ((B[9]&4)>>2) ^ ((B[6]&8)>>3) ^ ((B[3]&2)>>1) ^ ((B[8]&1)>>0) ) ;   
   
   
   
            // T1 = xor all inputs   
            // in1,in2, D are only used in T1 during initialisation, not generation   
            next_A1 = A[10] ^ X;   
            if (init) next_A1 = next_A1 ^ D ^ ((j % 2) ? in2 : in1);   
   
   
            // T2 =  xor all inputs   
            // in1,in2 are only used in T1 during initialisation, not generation   
            // if p=0, use this, if p=1, rotate the result left   
            next_B1 = B[7] ^ B[10] ^ Y;   
            if (init) next_B1 = next_B1 ^ ((j % 2) ? in1 : in2);   
   
            // if p=1, rotate left   
            if (p) next_B1 = ( (next_B1 << 1) | ((next_B1 >> 3) & 1) ) & 0xf;   
   
   
            // T3 = xor all inputs   
            D = E ^ Z ^ extra_B;   
   
   
            // T4 = sum, carry of Z + E + r   
            next_E = F;   
            if (q)   
            {   
                F = Z + E + r;   
                // r is the carry   
                r = (F >> 4) & 1;   
                F = F & 0x0f;   
            }   
            else   
            {   
                F = E;   
            }   
            E = next_E;   
   
   
            A[10] = A[9];   
            A[9] = A[8];   
            A[8] = A[7];   
            A[7] = A[6];   
            A[6] = A[5];   
            A[5] = A[4];   
            A[4] = A[3];   
            A[3] = A[2];   
            A[2] = A[1];   
            A[1]= next_A1;   
   
            B[10] = B[9];   
            B[9] = B[8];   
            B[8] = B[7];   
            B[7] = B[6];   
            B[6] = B[5];   
            B[5] = B[4];   
            B[4] = B[3];   
            B[3] = B[2];   
            B[2] = B[1];   
            B[1] = next_B1;   
   
            X = ((s4&1)<<3) | ((s3&1)<<2) | (s2&2) | ((s1&2)>>1);   
            Y = ((s6&1)<<3) | ((s5&1)<<2) | (s4&2) | ((s3&2)>>1);   
            Z = ((s2&1)<<3) | ((s1&1)<<2) | (s6&2) | ((s5&2)>>1);   
            p = (s7&2)>>1;   
            q = (s7&1);   
   
            // require 4 loops per output byte   
            // 2 output bits are a function of the 4 bits of D   
            // xor 2 by 2   
            op = (op << 2)^ ( (((D^(D>>1))>>1)&2) | ((D^(D>>1))&1) );   
        }   
        // return input data during init   
        cb[i] = (init) ? sb[i] : op;   
    }   
    return 0;   
}   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
//block cypher   
   
// key preparation   
unsigned char key_perm[0x40] = {   
    0x12,0x24,0x09,0x07,0x2A,0x31,0x1D,0x15,0x1C,0x36,0x3E,0x32,0x13,0x21,0x3B,0x40,   
    0x18,0x14,0x25,0x27,0x02,0x35,0x1B,0x01,0x22,0x04,0x0D,0x0E,0x39,0x28,0x1A,0x29,   
    0x33,0x23,0x34,0x0C,0x16,0x30,0x1E,0x3A,0x2D,0x1F,0x08,0x19,0x17,0x2F,0x3D,0x11,   
    0x3C,0x05,0x38,0x2B,0x0B,0x06,0x0A,0x2C,0x20,0x3F,0x2E,0x0F,0x03,0x26,0x10,0x37,   
};   
   
// block - sbox   
unsigned char block_sbox[0x100] = {   
    0x3A,0xEA,0x68,0xFE,0x33,0xE9,0x88,0x1A,0x83,0xCF,0xE1,0x7F,0xBA,0xE2,0x38,0x12,   
    0xE8,0x27,0x61,0x95,0x0C,0x36,0xE5,0x70,0xA2,0x06,0x82,0x7C,0x17,0xA3,0x26,0x49,   
    0xBE,0x7A,0x6D,0x47,0xC1,0x51,0x8F,0xF3,0xCC,0x5B,0x67,0xBD,0xCD,0x18,0x08,0xC9,   
    0xFF,0x69,0xEF,0x03,0x4E,0x48,0x4A,0x84,0x3F,0xB4,0x10,0x04,0xDC,0xF5,0x5C,0xC6,   
    0x16,0xAB,0xAC,0x4C,0xF1,0x6A,0x2F,0x3C,0x3B,0xD4,0xD5,0x94,0xD0,0xC4,0x63,0x62,   
    0x71,0xA1,0xF9,0x4F,0x2E,0xAA,0xC5,0x56,0xE3,0x39,0x93,0xCE,0x65,0x64,0xE4,0x58,   
    0x6C,0x19,0x42,0x79,0xDD,0xEE,0x96,0xF6,0x8A,0xEC,0x1E,0x85,0x53,0x45,0xDE,0xBB,   
    0x7E,0x0A,0x9A,0x13,0x2A,0x9D,0xC2,0x5E,0x5A,0x1F,0x32,0x35,0x9C,0xA8,0x73,0x30,   
   
    0x29,0x3D,0xE7,0x92,0x87,0x1B,0x2B,0x4B,0xA5,0x57,0x97,0x40,0x15,0xE6,0xBC,0x0E,   
    0xEB,0xC3,0x34,0x2D,0xB8,0x44,0x25,0xA4,0x1C,0xC7,0x23,0xED,0x90,0x6E,0x50,0x00,   
    0x99,0x9E,0x4D,0xD9,0xDA,0x8D,0x6F,0x5F,0x3E,0xD7,0x21,0x74,0x86,0xDF,0x6B,0x05,   
    0x8E,0x5D,0x37,0x11,0xD2,0x28,0x75,0xD6,0xA7,0x77,0x24,0xBF,0xF0,0xB0,0x02,0xB7,   
    0xF8,0xFC,0x81,0x09,0xB1,0x01,0x76,0x91,0x7D,0x0F,0xC8,0xA0,0xF2,0xCB,0x78,0x60,   
    0xD1,0xF7,0xE0,0xB5,0x98,0x22,0xB3,0x20,0x1D,0xA6,0xDB,0x7B,0x59,0x9F,0xAE,0x31,   
    0xFB,0xD3,0xB6,0xCA,0x43,0x72,0x07,0xF4,0xD8,0x41,0x14,0x55,0x0D,0x54,0x8B,0xB9,   
    0xAD,0x46,0x0B,0xAF,0x80,0x52,0x2C,0xFA,0x8C,0x89,0x66,0xFD,0xB2,0xA9,0x9B,0xC0,   
};   
   
// block - perm   
unsigned long block_perm[0x100] = {   
    0x00,0x02,0x80,0x82,0x20,0x22,0xA0,0xA2, 0x10,0x12,0x90,0x92,0x30,0x32,0xB0,0xB2,   
    0x04,0x06,0x84,0x86,0x24,0x26,0xA4,0xA6, 0x14,0x16,0x94,0x96,0x34,0x36,0xB4,0xB6,   
    0x40,0x42,0xC0,0xC2,0x60,0x62,0xE0,0xE2, 0x50,0x52,0xD0,0xD2,0x70,0x72,0xF0,0xF2,   
    0x44,0x46,0xC4,0xC6,0x64,0x66,0xE4,0xE6, 0x54,0x56,0xD4,0xD6,0x74,0x76,0xF4,0xF6,   
    0x01,0x03,0x81,0x83,0x21,0x23,0xA1,0xA3, 0x11,0x13,0x91,0x93,0x31,0x33,0xB1,0xB3,   
    0x05,0x07,0x85,0x87,0x25,0x27,0xA5,0xA7, 0x15,0x17,0x95,0x97,0x35,0x37,0xB5,0xB7,   
    0x41,0x43,0xC1,0xC3,0x61,0x63,0xE1,0xE3, 0x51,0x53,0xD1,0xD3,0x71,0x73,0xF1,0xF3,   
    0x45,0x47,0xC5,0xC7,0x65,0x67,0xE5,0xE7, 0x55,0x57,0xD5,0xD7,0x75,0x77,0xF5,0xF7,   
   
    0x08,0x0A,0x88,0x8A,0x28,0x2A,0xA8,0xAA, 0x18,0x1A,0x98,0x9A,0x38,0x3A,0xB8,0xBA,   
    0x0C,0x0E,0x8C,0x8E,0x2C,0x2E,0xAC,0xAE, 0x1C,0x1E,0x9C,0x9E,0x3C,0x3E,0xBC,0xBE,   
    0x48,0x4A,0xC8,0xCA,0x68,0x6A,0xE8,0xEA, 0x58,0x5A,0xD8,0xDA,0x78,0x7A,0xF8,0xFA,   
    0x4C,0x4E,0xCC,0xCE,0x6C,0x6E,0xEC,0xEE, 0x5C,0x5E,0xDC,0xDE,0x7C,0x7E,0xFC,0xFE,   
    0x09,0x0B,0x89,0x8B,0x29,0x2B,0xA9,0xAB, 0x19,0x1B,0x99,0x9B,0x39,0x3B,0xB9,0xBB,   
    0x0D,0x0F,0x8D,0x8F,0x2D,0x2F,0xAD,0xAF, 0x1D,0x1F,0x9D,0x9F,0x3D,0x3F,0xBD,0xBF,   
    0x49,0x4B,0xC9,0xCB,0x69,0x6B,0xE9,0xEB, 0x59,0x5B,0xD9,0xDB,0x79,0x7B,0xF9,0xFB,   
    0x4D,0x4F,0xCD,0xCF,0x6D,0x6F,0xED,0xEF, 0x5D,0x5F,0xDD,0xDF,0x7D,0x7F,0xFD,0xFF,   
};   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
int key_schedule(unsigned char *CK, int *kk)   
{   
    int i,j,k;   
    int bit[64];   
    int newbit[64];   
    int kb[9][8];   
   
    // 56 steps   
    // 56 key bytes kk(56)..kk(1) by key schedule from CK   
   
    // kb(7,1) .. kb(7,8) = CK(1) .. CK(8)   
    kb[7][1] = CK[0];   
    kb[7][2] = CK[1];   
    kb[7][3] = CK[2];   
    kb[7][4] = CK[3];   
    kb[7][5] = CK[4];   
    kb[7][6] = CK[5];   
    kb[7][7] = CK[6];   
    kb[7][8] = CK[7];   
   
    // calculate kb[6] .. kb[1]   
    for(i=0; i<7; i++)   
    {   
        // 64 bit perm on kb   
        for(j=0; j<8; j++)   
        {   
            for(k=0; k<8; k++)   
            {   
                bit[j*8+k] = (kb[7-i][1+j] >> (7-k)) & 1;   
                newbit[key_perm[j*8+k]-1] = bit[j*8+k];   
            }   
        }   
        for(j=0; j<8; j++)   
        {   
            kb[6-i][1+j] = 0;   
            for(k=0; k<8; k++)   
            {   
                kb[6-i][1+j] |= newbit[j*8+k] << (7-k);   
            }   
        }   
    }   
   
    // xor to give kk   
    for(i=0; i<7; i++)   
    {   
        for(j=0; j<8; j++)   
        {   
            kk[1+i*8+j] = kb[1+i][1+j] ^ i;   
        }   
    }   
   
    return 0;   
}   
   
   
   
   
int block_decypher(int *kk, unsigned char *ib, unsigned char *bd)   
{   
    int i;   
    int sbox_in;   
    int sbox_out;   
    int perm_out;   
    int R[9];   
    int next_R8;   
   
    R[1] = ib[0];   
    R[2] = ib[1];   
    R[3] = ib[2];   
    R[4] = ib[3];   
    R[5] = ib[4];   
    R[6] = ib[5];   
    R[7] = ib[6];   
    R[8] = ib[7];   
   
    // loop over kk[56]..kk[1]   
    for(i=56; i>0; i--)   
    {   
        sbox_in = kk[i] ^ R[7];   
        sbox_out = block_sbox[sbox_in];   
        perm_out = block_perm[sbox_out];   
   
        next_R8 = R[7];   
        R[7] = R[6] ^ perm_out;   
        R[6] = R[5];   
        R[5] = R[4] ^ R[8] ^ sbox_out;   
        R[4] = R[3] ^ R[8] ^ sbox_out;   
        R[3] = R[2] ^ R[8] ^ sbox_out;   
        R[2] = R[1];   
        R[1] = R[8] ^ sbox_out;   
   
        R[8] = next_R8;   
    }   
   
    bd[0] = R[1];   
    bd[1] = R[2];   
    bd[2] = R[3];   
    bd[3] = R[4];   
    bd[4] = R[5];   
    bd[5] = R[6];   
    bd[6] = R[7];   
    bd[7] = R[8];   
   
    return 0;   
}   
   
   
int block_encypher(int *kk, unsigned char *bd, unsigned char *ib)   
{   
    int i;   
    int sbox_in;   
    int sbox_out;   
    int perm_out;   
    int R[9];   
    int next_R1;   
   
    R[1] = bd[0];   
    R[2] = bd[1];   
    R[3] = bd[2];   
    R[4] = bd[3];   
    R[5] = bd[4];   
    R[6] = bd[5];   
    R[7] = bd[6];   
    R[8] = bd[7];   
   
    // loop over kk[1]..kk[56]   
    for(i=1; i<=56; i++)   
    {   
        sbox_in = kk[i] ^ R[8];   
        sbox_out = block_sbox[sbox_in];   
        perm_out = block_perm[sbox_out];   
   
        next_R1 = R[2];   
        R[2] = R[3] ^ R[1];   
        R[3] = R[4] ^ R[1];   
        R[4] = R[5] ^ R[1];   
        R[5] = R[6];   
        R[6] = R[7] ^ perm_out;   
        R[7] = R[8];   
        R[8] = R[1] ^ sbox_out;   
   
        R[1] = next_R1;   
    }   
   
    ib[0] = R[1];   
    ib[1] = R[2];   
    ib[2] = R[3];   
    ib[3] = R[4];   
    ib[4] = R[5];   
    ib[5] = R[6];   
    ib[6] = R[7];   
    ib[7] = R[8];   
   
    return 0;   
}   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
#define N 23 // assume TS packets, 184/8   
   
void decrypt(unsigned char *ck, unsigned char *encrypted, unsigned char *h)   
{   
    FILE *fp4;   
    char f_out_dname[20];   
    unsigned char decrypted[0x100];   
    int in_dex1;   
    int i,j;   
    int kk[57];   
    unsigned char stream[8];   
    unsigned char ib[8];   
    unsigned char block[8];   
    //int fail;   
    printf("\nkey");   
    for(i=0; i<8; i++)   
    {   
        if (!(i%0x10)) printf("\n%04x - ",i);   
        printf("%02x ",ck[i]);   
    }  
    printf("\nencrypted Plaintext");   
    for(i=0; i<188; i++)   
    {   
        if (!(i%0x10)) printf("\n%04x - ",i);   
        printf("%02x ",encrypted[i]);   
    }  
	
    key_schedule(ck,kk);   
     
   
    // 1st 4 bytes not encrypted   
    for(i=0; i<4; i++)   
        decrypted[i] = encrypted[i];   
   
    // 1st 8 bytes of initialisation   
    stream_cypher(1, ck, &encrypted[4], ib);   
   
    for(j=1; j<(N+1); j++)   
    {   
        block_decypher(kk, ib, block);   
          
   
        if (j != N)   
        {   
            stream_cypher(0, ck, NULL, stream);   
               
   
            // xor sb x stream   
            for(i=0; i<8; i++)  ib[i] = encrypted[4+8*j+i] ^ stream[i];   
        }   
        else   
        {   
            // last block - sb[N+1] = IV(initialisation vetor)(=0)   
            for(i=0; i<8; i++)  ib[i] = 0;   
        }   
           
        // xor ib x block   
        for(i=0; i<8; i++)  decrypted[4+8*(j-1)+i] = ib[i] ^ block[i];   
           
    }   
   
   
    printf("\nDecrypted Plaintext");   
    for(i=0; i<188; i++)   
    {   
        if (!(i%0x10)) printf("\n%04x - ",i);   
        printf("%02x ",decrypted[i]);   
    }   
   return;
      
   
   
   
    printf("\nPlease enter filename for output:\n");   
        scanf("%s",f_out_dname);   
   
    fp4 = fopen(f_out_dname,"w");   
       
    for(in_dex1=0; in_dex1<0x100; in_dex1++)   
    {   
        if(h[0]=='0' && h[1]=='x')   
        {   
            fprintf(fp4, "0x%x ",decrypted[in_dex1]);   
        }   
        else   
        {   
            fprintf(fp4, "%c", decrypted[in_dex1]);   
        }   
   
    }   
   
    fclose(fp4);   
       
   
}   
   
/*  
    encryption  
        run the block cypher backwards on the whole packet  
        this produces the ib[1..N] and sb[1]  
        the go forwards  
        xor the ib[2..N] with the stream cypher to give the encrypted data, sb[2..N]  
*/   
void encrypt(unsigned char *ck, unsigned char *decrypted, unsigned char *h)   
{   
   
    FILE *fp3;   
    unsigned char encrypted[0x100];   
       
    int in_dex;   
    char f_out_name[20];   
       
    int i,j;   
    int kk[57];   
    unsigned char stream[8];   
    unsigned char ib[N+2][8];   // since we'll use 1..N and N+1 for IV   
    unsigned char block[8];   
    //int fail;   
   
    key_schedule(ck,kk);   
     
   
    // 1st 4 bytes not encrypted   
    for(i=0; i<4; i++)   
        encrypted[i] = decrypted[i];   
   
   
    // ignore residue for now   
   
   
    // last word   
    // IV is really ib[n+1] = 0   
    for(i=0; i<8; i++) ib[N+1][i] = 0;   
   
    for (j=N; j>0; j--)   
    {   
        // xor db x ib[n][j]   
        for(i=0; i<8; i++)  block[i] = decrypted[4+(j*8)-8+i] ^ ib[j+1][i];   
        block_encypher(kk, block, ib[j]);   
    }   
   
    // ib is now ib[1] which is in fact sb[1]   
    // and sb[1] is the input to the stream cypher   
    // so now we can run the stream cypher to generate all the sb[]   
   
   
    // 1st 8 bytes of initialisation - ib[1] has popped out of the last block cypher ...   
    stream_cypher(1, ck, ib[1], stream);   
   
    // sb[1] is just ib[1];   
    for(i=0; i<8; i++)  encrypted[4+0+i] = ib[1][i];   
   
    for(j=2; j<(N+1); j++)   
    {   
        stream_cypher(0, ck, ib[j], stream);   
   
        // xor ib x stream   
        for(i=0; i<8; i++)  encrypted[4+8*(j-1)+i] = ib[j][i] ^ stream[i];   
          
    }   
   
   
    printf("\nEncrypted Ciphertext");   
    for(i=0; i<188; i++)   
    {   
        if (!(i%0x10)) printf("\n%04x - ",i);   
        printf("%02x ",encrypted[i]);   
    }   
   
      
   
   
   
    printf("\nPlease enter filename for output:\n");   
        scanf("%s",f_out_name);   
   
    fp3 = fopen(f_out_name,"w");   
       
    for(in_dex=0; in_dex<0x100; in_dex++)   
    {   
        if(h[0]=='0' && h[1]=='x')   
        {   
            fprintf(fp3, "0x%x ",encrypted[in_dex]);   
        }   
        else   
        {   
            fprintf(fp3, "%c", encrypted[in_dex]);   
        }   
   
    }   
   
    fclose(fp3);   

}   

#if 0
 struct			test_s
{
  unsigned char		key[256];
  unsigned char		in[256];
  unsigned char		out[256];
};

static  struct test_s	csa_tests[2] =
{
    /* test 1 */
    {
      /* control words */
      {0x07, 0xe0, 0x1b, 0x02, 0xc9, 0xe0, 0x45, 0xee},

      {0x00, 0x00,0x00,0x00,0xde, 0xcf, 0x0a, 0x0d, 0xb2, 0xd7, 0xc4, 0x40, 0xde, 0x5d, 0x63, 0x18, 0x5a, 0x98, 0x17, 0xaa,
       0xc9, 0xbc, 0x27, 0xc6, 0xcb, 0x49, 0x40, 0x48, 0xfd, 0x20, 0xb7, 0x05, 0x5b, 0x27, 0xcb, 0xeb,
       0x9a, 0xf0, 0xac, 0x45, 0x6d, 0x56, 0xf4, 0x7b, 0x6f, 0xa0, 0x57, 0xf3, 0x9b, 0xf7, 0xa2, 0xc7,
       0xd4, 0x68, 0x24, 0x00, 0x2f, 0x28, 0x13, 0x96, 0x94, 0xa8, 0x7c, 0xf4, 0x6f, 0x07, 0x2a, 0x0e,
       0xe8, 0xa1, 0xeb, 0xc7, 0x80, 0xac, 0x1f, 0x79, 0xbf, 0x5d, 0xb6, 0x10, 0x7c, 0x2e, 0x52, 0xe9,
       0x34, 0x2c, 0xa8, 0x39, 0x01, 0x73, 0x04, 0x24, 0xa8, 0x1e, 0xdb, 0x5b, 0xcb, 0x24, 0xf6, 0x31,
       0xab, 0x02, 0x6b, 0xf9, 0xf6, 0xf7, 0xe9, 0x52, 0xad, 0xcf, 0x62, 0x0f, 0x42, 0xf6, 0x66, 0x5d,
       0xc0, 0x86, 0xf2, 0x7b, 0x40, 0x20, 0xa9, 0xbd, 0x1f, 0xfd, 0x16, 0xad, 0x2e, 0x75, 0xa6, 0xa0,
       0x85, 0xf3, 0x9c, 0x31, 0x20, 0x4e, 0xfb, 0x95, 0x61, 0x78, 0xce, 0x10, 0xc1, 0x48, 0x5f, 0xd3,
       0x61, 0x05, 0x12, 0xf4, 0xe2, 0x04, 0xae, 0xe0, 0x86, 0x01, 0x56, 0x55, 0xb1, 0x0f, 0xa6, 0x33,
       0x95, 0x20, 0x92, 0xf0, 0xbe, 0x39, 0x31, 0xe1, 0x2a, 0xf7, 0x93, 0xb4, 0xf7, 0xe4, 0xf1, 0x85,
       0xae, 0x50, 0xf1, 0x63, 0xd4, 0x5d, 0x9c, 0x6c},

      {0xaf, 0xbe, 0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0xfb, 0xe6, 0xb5, 0xad, 0x7c,
       0xf9, 0xf3, 0xe5, 0xb1, 0x6c, 0x7c, 0xf9, 0xf3, 0xe6, 0xb5, 0xad, 0x6b, 0x5f, 0x3e, 0x7c, 0xf9,
       0x6c, 0x5b, 0x1f, 0x3e, 0x7c, 0xf9, 0xad, 0x6b, 0x5a, 0xd7, 0xcf, 0x9f, 0x3e, 0x5b, 0x16, 0xc7,
       0xcf, 0x9f, 0x3e, 0x6b, 0x5a, 0xd6, 0xb5, 0xf3, 0xe7, 0xcf, 0x96, 0xc5, 0xb1, 0xf3, 0xe7, 0xcf,
       0x9a, 0xd6, 0xb5, 0xad, 0x7c, 0xf9, 0xf3, 0xe5, 0xb1, 0x6c, 0x7c, 0xf9, 0xf3, 0xe6, 0xb5, 0xad,
       0x6b, 0x5f, 0x3e, 0x7c, 0xf9, 0x6c, 0x5b, 0x1f, 0x3e, 0x7c, 0xf9, 0xad, 0x6b, 0x5a, 0xd7, 0xcf,
       0x9f, 0x3e, 0x5b, 0x16, 0xc7, 0xcf, 0x9f, 0x3e, 0x6b, 0x5a, 0xd6, 0xb5, 0xf3, 0xe7, 0xcf, 0x96,
       0xc5, 0xb1, 0xf3, 0xe7, 0xcf, 0x9a, 0xd6, 0xb5, 0xad, 0x7c, 0xf9, 0xf3, 0xe5, 0xb1, 0x6c, 0x7c,
       0xf9, 0xf3, 0xe6, 0xb5, 0xad, 0x6b, 0x5f, 0x3e, 0x7c, 0xf9, 0x6c, 0x5b, 0x1f, 0x3e, 0x7c, 0xf9,
       0xad, 0x6b, 0x5a, 0xd7, 0xcf, 0x9f, 0x3e, 0x5b, 0x16, 0xc7, 0xcf, 0x9f, 0x3e, 0x6b, 0x5a, 0xd6,
       0xb5, 0xf3, 0xe7, 0xcf, 0x96, 0xc5, 0xb1, 0xf3, 0xe7, 0xcf, 0x9a, 0xd0, 0x00, 0x00, 0x00, 0x00,
       0xff, 0xfc, 0x44, 0x00, 0x66, 0xb1, 0x11, 0x11}
    },

    /* test 2 */
    {
      {0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00},

      {0x00, 0x00,0x00,0x00,0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
       0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
       0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
       0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
       0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
       0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
       0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
       0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
       0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
       0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
       0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
       0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7},

      {0x2d, 0x0a, 0x47, 0x20, 0x18, 0x11, 0x9c, 0x8a, 0xd1, 0x2a, 0x65, 0x6b, 0x89, 0xe4, 0x35, 0x2b,
       0xc2, 0xb5, 0x90, 0x61, 0xd1, 0x7e, 0x02, 0xe1, 0x3f, 0x46, 0x70, 0xcf, 0x77, 0x91, 0x2f, 0x22,
       0x93, 0xc1, 0x6c, 0xfe, 0x49, 0xad, 0x7c, 0xc2, 0xaf, 0x86, 0x1b, 0xa3, 0x29, 0xbe, 0xaa, 0x64,
       0xf0, 0x22, 0xb9, 0x5e, 0x98, 0xaa, 0x60, 0xef, 0xdf, 0xd6, 0x44, 0x77, 0xe6, 0xbf, 0xbb, 0x94,
       0xb2, 0x0a, 0x63, 0x0e, 0x5c, 0xf2, 0xac, 0xb4, 0x49, 0xcc, 0x9e, 0x4f, 0x94, 0x4c, 0x30, 0x12,
       0xe8, 0x55, 0xc2, 0x44, 0xa4, 0x52, 0xcb, 0x61, 0x81, 0xc9, 0xb6, 0xa6, 0x6b, 0xef, 0xaf, 0xa6,
       0x71, 0x1d, 0x7b, 0x58, 0x2f, 0xfa, 0xd1, 0x0c, 0x07, 0x9d, 0x1f, 0x35, 0x87, 0xbe, 0x02, 0x9f,
       0x20, 0xc6, 0x60, 0x8f, 0x1c, 0x30, 0x0f, 0x96, 0xd0, 0x71, 0xd6, 0x51, 0x10, 0xdf, 0x5b, 0xf6,
       0x44, 0x2f, 0x80, 0x28, 0xb7, 0xec, 0x23, 0x59, 0x4b, 0x94, 0x0b, 0x9a, 0x74, 0xa1, 0x1f, 0xf7,
       0x9e, 0x76, 0xb4, 0xdf, 0xbb, 0x3c, 0x8c, 0x88, 0x97, 0x22, 0x56, 0x73, 0x16, 0x05, 0xac, 0xf9,
       0x4f, 0x77, 0x9d, 0x38, 0xa0, 0x6b, 0x05, 0xd2, 0xe6, 0x15, 0x01, 0xb1, 0x5c, 0xc9, 0x62, 0xa9,
       0x9b, 0x1a, 0x6a, 0x1a, 0xcf, 0xe6, 0xa8, 0xba}
    }
};

int main(void)   
{   
#if 1
	int i;
	unsigned char hexorchar[3]={"0x"};
	for(i=0;i<2;i++){
  		decrypt(csa_tests[i].key, csa_tests[i].in,hexorchar );
	}
	return 0;
#endif
    while(1){   
   
   
    FILE *fp1;  //fp for key   
    FILE *fp2;  //fp for plaintext   
    FILE *fp5;  //fp for file1   
    FILE *fp6;  //fp for file2   
    //FILE *fp3;   
   
    //char testend;   
   
    char eord;      //to choose encryotion or decryption   
    int in_dex;     //array index   
   
      
    //unsigned int key_in[8];   
    unsigned char key_char[0x08];       //key array   
    unsigned char plaintext_char[0x100];    //plaintext arrray   
    //unsigned char encrypted_char[0x100];    //plaintext arrray   
    char f_key_name[20];       //file name for key   
    char f_name[20];           //file name for plaintext   
    //char f_out_name[20];   
    char ff1_name[20];   
    char ff2_name[20];   
    unsigned char hexorchartext[2];   
    unsigned char hexorcharkey[2];   
    char file1[0x100]; //plaintext arrray   
    char file2[0x100]; //plaintext arrray   
   
   
    printf("\nWould you like to encrypt, decrypt or compare?   e/d/c\n");   
   
   
    scanf("%c",&eord);      //takes in user input of e or d   
   
   
    if (eord=='e'){   
   
    printf("\nPlease enter filename for plaintext:\n");   
        scanf("%s",f_name);   
   
   
   
    while( (fp2 = fopen(f_name,"r")) == NULL){   
        printf("Re-enter a valid filename:\n");   
        scanf("%s",f_name);   
    }   
   
    fscanf(fp2, "%c%c", &hexorchartext[0], &hexorchartext[1]);   
   
    rewind(fp2);   
   
    if(hexorchartext[0]=='0' && hexorchartext[1]=='x')   
    {   
   
        for(in_dex=0;in_dex<0x100;in_dex++){   
           
            fscanf(fp2, "%c", &plaintext_char[in_dex]);   
               
        }   
    }   
   
    else   
    {   
        for(in_dex=0;!feof(fp2);in_dex++){   
           
            fscanf(fp2, "%c", &plaintext_char[in_dex]);   
            printf("%c", plaintext_char[in_dex]);   
        }   
    }   
   
    fclose(fp2);   
   
   
   
   
   
   
   
   
   
    printf("\nPlease enter filename for key:\n");   
        scanf("%s",f_key_name);   
   
   
   
    while( (fp1 = fopen(f_key_name,"r")) == NULL){   
        printf("Re-enter a valid filename:\n");   
        scanf("%s",f_key_name);   
    }   
   
    fscanf(fp1, "%c%c", &hexorcharkey[0], &hexorcharkey[1]);   
   
    rewind(fp1);   
   
    if(hexorcharkey[0]=='0' && hexorcharkey[1]=='x')   
    {   
   
        for(in_dex=0;in_dex<0x08;in_dex++){   
           
            fscanf(fp1, "%c", &key_char[in_dex]);   
        }   
    }   
   
    else   
    {   
        for(in_dex=0;in_dex<0x08;in_dex++){   
           
            fscanf(fp1, "%c", &key_char[in_dex]);   
        }   
    }   
   
    fclose(fp1);   
   
   
    encrypt(key_char, plaintext_char, hexorchartext);   
   
   
   
   
           
   
    }   
   
   
    else if (eord=='d'){   
   
   
   
   
    printf("\nPlease enter filename for ciphertext:\n");   
        scanf("%s",f_name);   
   
   
   
    while( (fp2 = fopen(f_name,"r")) == NULL){   
        printf("Re-enter a valid filename:\n");   
        scanf("%s",f_name);   
    }   
   
    fscanf(fp2, "%c%c", &hexorchartext[0], &hexorchartext[1]);   
   
    rewind(fp2);   
   
    if(hexorchartext[0]=='0' && hexorchartext[1]=='x')   
    {   
   
        for(in_dex=0;in_dex<0x100;in_dex++){   
           
            fscanf(fp2, "%c", &plaintext_char[in_dex]);   
        }   
    }   
   
    else   
    {   
        for(in_dex=0;!feof(fp2);in_dex++){   
           
            fscanf(fp2, "%c", &plaintext_char[in_dex]);   
        }   
    }   
   
    fclose(fp2);   
   
   
   
   
   
   
   
   
   
    printf("\nPlease enter filename for key:\n");   
        scanf("%s",f_key_name);   
   
   
   
    while( (fp1 = fopen(f_key_name,"r")) == NULL){   
        printf("Re-enter a valid filename:\n");   
        scanf("%s",f_key_name);   
    }   
   
    fscanf(fp1, "%c%c", &hexorcharkey[0], &hexorcharkey[1]);   
   
    rewind(fp1);   
   
    if(hexorcharkey[0]=='0' && hexorcharkey[1]=='x')   
    {   
   
        for(in_dex=0;in_dex<0x08;in_dex++){   
           
            fscanf(fp1, "%c", &key_char[in_dex]);   
        }   
    }   
   
    else   
    {   
        for(in_dex=0;in_dex<0x08;in_dex++){   
           
            fscanf(fp1, "%c", &key_char[in_dex]);   
        }   
    }   
   
    fclose(fp1);   
   
   
    decrypt(key_char, plaintext_char, hexorchartext);   
   
   
    }   
   
   
    else if (eord=='c'){   
   
       
    printf("\nPlease enter filename:\n");   
        scanf("%s",ff1_name);   
   
    while( (fp5 = fopen(ff1_name,"r")) == NULL){   
        printf("Re-enter a valid filename:\n");   
        scanf("%s",ff1_name);   
    }   
   
    fscanf(fp5, "%c%c", &hexorchartext[0], &hexorchartext[1]);   
   
    rewind(fp5);   
   
   
   
   
    printf("\nPlease enter filename:\n");   
        scanf("%s",ff2_name);   
   
    while( (fp6 = fopen(ff2_name,"r")) == NULL){   
        printf("Re-enter a valid filename:\n");   
        scanf("%s",ff2_name);   
    }   
   
   
   
   
       
    if(hexorchartext[0]=='0' && hexorchartext[1]=='x')   
    {   
        //printf("we are in HEX mode\n");   
   
        for(in_dex=0;in_dex<0x100;in_dex++){   
           
            fscanf(fp5, "%c", &file1[in_dex]);   
            fscanf(fp6, "%c", &file2[in_dex]);   
   
            //printf("%x   %x\n",file1[in_dex],file2[in_dex]);   
   
   
   
            if(!feof(fp5) && !feof(fp6) && file1[in_dex] != file2[in_dex])   
            {   
                in_dex=0x105;   
            }   
        }   
    }   
   
    else   
    {   
        //printf("we are in CHAR mode\n");   
   
        for(in_dex=0;in_dex<0x100;in_dex++){   
           
            fscanf(fp5, "%c", &file1[in_dex]);   
            fscanf(fp6, "%c", &file2[in_dex]);   
   
            //printf("%c   %c\n",file1[in_dex],file2[in_dex]);   
   
   
            if(!feof(fp5) && !feof(fp6) && file1[in_dex] != file2[in_dex])   
            {   
                in_dex=0x105;   
            }   
        }   
    }   
   
   
    //printf("%x",in_dex);   
    if(in_dex>0x102)   
        printf("\nFAILED!!");   
    else   
        printf("\nPASSED!!");   
       
       
    }   
   
    else   
        printf("\n");   
   
    }   
   
   
}   
#endif