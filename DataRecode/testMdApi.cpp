// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "MdSpi.h"

void main()
{
	
	char  FRONT_ADDR[] = "tcp://180.168.146.187:10011";		// ǰ�õ�ַ

	CMdSpi* pUserSpi = new CMdSpi();
	
	if (pUserSpi)
	{
		pUserSpi->getMdApi()->RegisterSpi(pUserSpi);						// ע���¼���
		pUserSpi->getMdApi()->RegisterFront(FRONT_ADDR);					// connect
		pUserSpi->getMdApi()->Init();

		pUserSpi->getMdApi()->Join();
	}
//	pUserApi->Release();
}