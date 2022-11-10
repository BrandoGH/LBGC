// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonTool.h"

namespace CommonTool
{

FString CommonTool::HashMd5Data(const uint8* inData, int32 inDataSize, uint8* outData, int32 outDataSize, bool needRetMd5String)
{
	FString retMD5String;
	// Must be 16 bit (string = 32 length)
	if (outDataSize != 16 ||
		outData == NULL)
	{
		return retMD5String;
	}

	FMD5 md5Gen;
	md5Gen.Update(inData, inDataSize);
	md5Gen.Final(outData);

	if (needRetMd5String)
	{
		for (int32 i = 0; i < outDataSize; ++i)
		{
			retMD5String += FString::Printf(TEXT("%02x"), outData[i]);
		}
	}

	return retMD5String;
}
}

