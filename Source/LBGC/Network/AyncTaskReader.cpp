// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "AyncTaskReader.h"
#include "TcpClient.h"
#include "../GameInstance/LBGCGameInstance.h"
#include <Sockets.h>
#include "../MsgModule/MsgCommon.h"
#include "../MsgModule/CommonTool.h"


#define TASK_READER_CHECK_SERVER_ENDIAN \
if (m_nReadSise == 1)\
{\
	if (m_arrReadMsg[0] != 0xAE)\
	{\
		m_client->GetSocket()->Shutdown(ESocketShutdownMode::Read);\
		UE_LOG(LogTemp, Warning, TEXT("Error: The Server is not little endian, flag[%02x]"), m_arrReadMsg[0]);\
		break;\
	}\
	UE_LOG(LogTemp, Warning, TEXT("The Server is little endian, flag[0x%02X]"), m_arrReadMsg[0]);\
	continue;\
}\

#define TASK_READER_CHECK_HEADER \
if (m_msgHeader.m_nMsgLen <= 0 || \
	m_msgHeader.m_nMsgLen > NSTcpClient::g_nOnceMsgSize ||\
	m_msgHeader.m_nMsgType < MSG_TYPE_CS_START ||\
	m_msgHeader.m_nMsgType >= MSG_CODE_MAX)\
{\
	m_bHeaderIntegrated = true;\
	m_nLastHasReadSize = 0;\
	m_nNextNeedReadSize = 0;\
	break;\
}

#define TASK_READER_MSG_CALL_HANDLER \
uint16 userDataSize = m_msgHeader.m_nMsgLen - sizeof(MsgHeader) - sizeof(MsgEnder); \
m_msgEnder = *(MsgEnder*)(m_onceMsg.GetData() + sizeof(MsgHeader) + userDataSize);\
MsgEnder checkEnd;\
CommonTool::HashMd5Data(m_onceMsg.GetData(), sizeof(MsgHeader) + userDataSize, checkEnd.m_bytesMD5);\
if (!CommonTool::IsMd5Equal_16byte(checkEnd.m_bytesMD5, m_msgEnder.m_bytesMD5))\
{\
	m_bHeaderIntegrated = true;\
	m_nLastHasReadSize = 0;\
	m_nNextNeedReadSize = 0;\
	break;\
}\
AsyncTask(ENamedThreads::GameThread, [&]()\
{\
	m_dgReadCallback.ExecuteIfBound(m_onceMsg);\
});

AyncTaskReader::AyncTaskReader(const FMsgReaderDelegate& ReadCallback)
	: m_dgReadCallback(ReadCallback)
	, m_nHasReadDataSize(false)
	, m_nReadSise(0)
	, m_nNextNeedReadSize(0)
	, m_nLastHasReadSize(0)
	, m_bHeaderIntegrated(false)
{
	m_arrReadMsg.Init(0, NSTcpClient::g_nReadBufferMaxSize);
	m_onceMsg.Init(0, NSTcpClient::g_nOnceMsgSize);
}

void AyncTaskReader::DoWork()
{
	UTcpClient* m_client = LBGC_INSTANCE->GetTcpClient();
	if (!m_client ||
		!m_client->GetSocket() ||
		!m_client->IsConnect())
	{
		return;
	}

	while (!m_client->IsWillDestroy())
	{
		FWindowsPlatformProcess::Sleep(0.01F);

		if (!m_client->GetSocket()->HasPendingData((uint32&)m_nReadSise))
		{
			continue;
		}
		m_client->GetSocket()->Recv(m_arrReadMsg.GetData(), NSTcpClient::g_nReadBufferMaxSize, m_nReadSise);

		// first connect read, server system is not little endian, break
		TASK_READER_CHECK_SERVER_ENDIAN;

		// Start processing messages
		m_msgHeader.reset();
		m_msgEnder.reset();
		m_nHasReadDataSize = 0;
		int32 remainSize = 0;
		int32 remainBodySize = 0;

		while (m_nHasReadDataSize < m_nReadSise)
		{
			FWindowsPlatformProcess::Sleep(0.01F);
			if (m_nLastHasReadSize > 0 && m_nNextNeedReadSize > 0)
			{
				memmove(m_onceMsg.GetData() + m_nLastHasReadSize, m_arrReadMsg.GetData(), m_nNextNeedReadSize);
				m_msgHeader = *(MsgHeader*)(m_onceMsg.GetData() + m_nHasReadDataSize);
				TASK_READER_CHECK_HEADER;

				if (m_bHeaderIntegrated)
				{
					TASK_READER_MSG_CALL_HANDLER;

					m_nHasReadDataSize += m_nNextNeedReadSize;
				}
				else
				{
					remainBodySize = m_nReadSise - m_nNextNeedReadSize;
					if (remainBodySize >= m_msgHeader.m_nMsgLen - sizeof(MsgHeader))
					{
						memmove(m_onceMsg.GetData() + sizeof(MsgHeader), m_arrReadMsg.GetData() + m_nNextNeedReadSize, m_msgHeader.m_nMsgLen - sizeof(MsgHeader));

						TASK_READER_MSG_CALL_HANDLER;

						m_nHasReadDataSize += (m_nNextNeedReadSize + m_msgHeader.m_nMsgLen - sizeof(MsgHeader));
					}
				}

				m_bHeaderIntegrated = true;
				m_nLastHasReadSize = 0;
				m_nNextNeedReadSize = 0;
				continue;
			}
			remainSize = m_nReadSise - m_nHasReadDataSize;
			if (remainSize >= sizeof(MsgHeader))
			{
				m_msgHeader = *(MsgHeader*)(m_arrReadMsg.GetData() + m_nHasReadDataSize);
				TASK_READER_CHECK_HEADER;

				remainBodySize = remainSize - sizeof(MsgHeader);
				if (remainBodySize >= m_msgHeader.m_nMsgLen - sizeof(MsgHeader))
				{
					memmove(m_onceMsg.GetData(), m_arrReadMsg.GetData() + m_nHasReadDataSize, m_msgHeader.m_nMsgLen);

					//TASK_READER_MSG_CALL_HANDLER;
					uint16 userDataSize = m_msgHeader.m_nMsgLen - sizeof(MsgHeader) - sizeof(MsgEnder); 
					m_msgEnder = *(MsgEnder*)(m_onceMsg.GetData() + sizeof(MsgHeader) + userDataSize); 
					MsgEnder checkEnd; 
					CommonTool::HashMd5Data(m_onceMsg.GetData(), sizeof(MsgHeader) + userDataSize, checkEnd.m_bytesMD5); 
					if (!CommonTool::IsMd5Equal_16byte(checkEnd.m_bytesMD5, m_msgEnder.m_bytesMD5))
					{
						m_bHeaderIntegrated = true; 
						m_nLastHasReadSize = 0; 
						m_nNextNeedReadSize = 0; 
						break;
					}

					AsyncTask(ENamedThreads::GameThread, [&]()
						{
							m_dgReadCallback.ExecuteIfBound(m_onceMsg);
						});


					m_nHasReadDataSize += m_msgHeader.m_nMsgLen;
					m_nLastHasReadSize = 0;
					m_nNextNeedReadSize = 0;
					continue;
				}
				m_nLastHasReadSize = remainSize;
				m_nNextNeedReadSize = (m_msgHeader.m_nMsgLen - remainBodySize - sizeof(MsgHeader));
				memmove(m_onceMsg.GetData(), m_arrReadMsg.GetData() + m_nHasReadDataSize, remainSize);
				m_nHasReadDataSize += remainSize;
				continue;
			}
			m_bHeaderIntegrated = false;
			m_nLastHasReadSize = remainSize;
			m_nNextNeedReadSize = (sizeof(MsgHeader) - m_nLastHasReadSize);
			memmove(m_onceMsg.GetData(), m_arrReadMsg.GetData() + m_nHasReadDataSize, remainSize);
			m_nHasReadDataSize += remainSize;
		}
	}
}
