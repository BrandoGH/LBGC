// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace CommonTool
{

template<class ArrayType, int len>
int GetArraySize(ArrayType(&arr)[len]) { return len; }

template<class BytesArrayMd51, class BytesArrayMd512>
bool IsMd5Equal_16byte(BytesArrayMd51& readMd5Data, BytesArrayMd512& checkMd5Data)
{
	if (GetArraySize(readMd5Data) != 16 ||
		GetArraySize(checkMd5Data) != 16)
	{
		return false;
	}

	if (sizeof(readMd5Data[0]) != sizeof(uint8) || sizeof(checkMd5Data[0]) != sizeof(uint8))
	{
		return false;
	}

	for (int i = 0; i < 16; ++i)
	{
		if (readMd5Data[i] != checkMd5Data[i])
		{
			return false;
		}
	}

	return true;
}

FString HashMd5Data(
	const uint8* inData,
	int32 inDataSize,
	uint8* outData,
	int32 outDataSize = 16,
	bool needRetMd5String = false);

}

