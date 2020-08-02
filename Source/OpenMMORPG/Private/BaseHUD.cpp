// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseHUD.h"


#include <openssl/evp.h>


#include "openssl/sha.h"
#include <sstream>
#include <iomanip>


void ABaseHUD::DrawHUD()
{
    Super::DrawHUD();
    unsigned char test[] = "test";
    FString hashTest = "Hash test sha(256): " + GetSHA256_s("test", strlen("test"));

    DrawText(hashTest, FColor::White, 50, 50);
}

FString ABaseHUD::GetSHA256_s(const void * data, size_t data_len)
{


    auto mdctx = EVP_MD_CTX_new();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    EVP_DigestInit(mdctx, EVP_sha256());
    EVP_DigestUpdate(mdctx, data, (size_t)data_len);
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    
    //EVP_MD_CTX_cleanup(mdctx);
 
    std::stringstream s;
    s.fill('0');
 
    for (size_t i = 0; i < md_len; ++i)
        s << std::setw(2) << std::hex << (unsigned short)md_value[i];
 
    return s.str().c_str();
    //return s.str().c_str();
}

