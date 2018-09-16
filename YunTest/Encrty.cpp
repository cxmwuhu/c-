#include "StdAfx.h"
#include "Encrty.h"

#define pub_key "cert_pub.pem"
#define pri_key "cert.pem"


CEncrty::CEncrty(void)
{
}


CEncrty::~CEncrty(void)
{
}

// RSA的API中当使用参数RSA_PKCS1_PADDING时，明文长度不能大于密文长度-11；当使用参数RSA_NO_PADDING时，明文长度需要正好是128。
int CEncrty::Encrypt(int len, unsigned char * from, unsigned char * to)
{
	if(len>rsalen-11)
	{
		printf("使用参数RSA_PKCS1_PADDING时出错\n");
		return -1;
	}
	return RSA_public_encrypt(len,from,to,pub_rsa,RSA_PKCS1_PADDING);
}


// 解密
int CEncrty::Decrypt(int len, unsigned char * from, unsigned char * to)
{
	if(len>rsalen)
	{
		return -1;
	}
	return RSA_private_decrypt(len,from,to,pri_rsa,RSA_PKCS1_PADDING);
}


// 初始化秘钥
int CEncrty::initkey()
{
	// -------------------------------------------------------
        // 利用公钥加密明文的过程
    // -------------------------------------------------------
	//打开公钥秘钥文件
	FILE* key_file = fopen(pub_key,"r");
	if(key_file == NULL)
	{
		printf("pub_key打开失败\n");
		return -1;
	}
	//从文件中读取公钥
	pub_rsa = PEM_read_RSA_PUBKEY(key_file,NULL,NULL,NULL);
	if(pub_rsa == NULL)
	{
		ERR_print_errors_fp(stdout);
		printf("pub_key读取失败\n");
		return -1;
	}
	fclose(key_file);
	////打开私钥秘钥文件
	key_file = fopen(pri_key,"r");
	if(key_file == NULL)
	{
		printf("pri_key打开失败\n");
		return -1;
	}
	//从文件中读取私钥
	pri_rsa = PEM_read_RSAPrivateKey(key_file,NULL,NULL,NULL);
	if(pri_rsa == NULL)
	{
		ERR_print_errors_fp(stdout);
		printf("pri_key读取失败\n");
		return -1;
	}
	fclose(key_file);
	//判断RSA的API中当使用参数RSA_PKCS1_PADDING时，明文长度不能大于密文长度-11
	rsalen = RSA_size(pri_rsa);
	return 0;
}


// AES_CBC加密函数
int CEncrty::AES_CBC_Encrty(unsigned char * outbuf, int outlen, unsigned char * inbuf, int inlen, const char * key, int keylen, const char * iv, int ivlen)
{
	OpenSSL_add_all_algorithms();
	int rv;
	EVP_CIPHER_CTX ctx;
	unsigned char key_pad[16];
	unsigned char iv_pad[16];
	memset(key_pad,0,16);
	memset(iv_pad,0,16);
	memcpy(key_pad,key,16);
	memcpy(iv_pad,iv,16);
	unsigned char outmsg[1024] = {0};
	int outbufflen=0,tempupdate,tempfinal;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_CIPHER_CTX_set_padding(&ctx,NULL);
	rv = EVP_EncryptInit_ex(&ctx,EVP_aes_128_cbc(),NULL,key_pad,iv_pad);
	if(rv!=1)
	{
		printf("EVP_EncryptInit_ex ERROR\n");
		return 0;
	}
	//判断输入长度是否是16的倍数
	int loopsize = inlen/16;    //循环次数
	int leavesize = inlen%16;
	for(int i=0; i<loopsize; i++)
	{
	    rv = EVP_EncryptUpdate(&ctx,outbuf,&tempupdate,inbuf,16);
	    if(rv!=1)
	    {
		    printf("EVP_EncryptUpdate ERROR\n");
		    return 0;
	    }
	    outbufflen += tempupdate;
	    outbuf += tempupdate;
		inbuf +=16;
	}
	if(leavesize)
	{
		rv = EVP_EncryptUpdate(&ctx,outbuf,&tempupdate,inbuf,leavesize);
		if(rv!=1)
		{
			printf("EVP_EncryptUpdate ERROR\n");
			return 0;
		}
		outbufflen += tempupdate;
		outbuf += tempupdate;
	}
	rv = EVP_EncryptFinal_ex(&ctx,outbuf,&tempfinal);
	outbufflen += tempfinal;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return outbufflen;
}

// AES解密
int CEncrty::AES_CBC_Decrypt(unsigned char * outbuf, int outl, unsigned char * inbuf, int inlen , unsigned char * key, int keylen, unsigned char * iv, int ivlen, int padding)
{
	OpenSSL_add_all_algorithms();
	int rv;
	int outlen = 0;
	int templen;
	EVP_CIPHER_CTX ctx;    //定义CIPHER向量
	unsigned char key_buf[16];
	unsigned char iv_buf[16];
	memset(key_buf,0,16);
	memset(iv_buf,0,16);
	memcpy(key_buf,key,16);
	memcpy(iv_buf,iv,16);
	EVP_CIPHER_CTX_init(&ctx);
	EVP_CIPHER_CTX_set_padding(&ctx,padding);
	rv = EVP_DecryptInit(&ctx,EVP_aes_128_cbc(),key_buf,iv_buf);
	int loopsize = inlen/16;
	for(int i=0; i<loopsize; i++)
	{
		rv = EVP_DecryptUpdate(&ctx,outbuf,&templen,inbuf,16);
		if(rv != 1)
		{
			printf("EVP_DecryptUpdate error\n");
			return 0;
		}
		outlen += templen;
		outbuf += templen;
		inbuf += 16;
	}

	rv = EVP_DecryptFinal_ex(&ctx,outbuf,&templen);
	outlen += templen;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return outlen;
}
