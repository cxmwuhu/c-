#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "openssl/err.h"
#include "openssl/aes.h"
#include "openssl/evp.h"

#pragma once
class CEncrty
{
public:
	CEncrty(void);
	~CEncrty(void);

private:
	// ���ļ��ж�ȡ��Կ
    RSA* pub_rsa;
	// ���ļ��ж�ȡ˽Կ
    RSA *pri_rsa;
	int rsalen;
public:
	int Encrypt(int len, unsigned char * from, unsigned char * to);
	// ����
	int Decrypt(int len, unsigned char * from, unsigned char * to);
	// ��ʼ����Կ
	int initkey();
	// AES_CBC���ܺ���
	int AES_CBC_Encrty(unsigned char * outbuf, int outlen, unsigned char * inbuf, int inlen, const char * key, int keylen, const char * iv, int ivlen);
	// AES����
	int AES_CBC_Decrypt(unsigned char * outbuf, int outl, unsigned char * inbuf, int inlen , unsigned char * key, int keylen, unsigned char * iv, int ivlen, int padding);
};

