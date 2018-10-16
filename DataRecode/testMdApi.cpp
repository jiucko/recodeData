// testTraderApi.cpp : 定义控制台应用程序的入口点。
//

#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "MdSpi.h"

void main()
{
	
	char  FRONT_ADDR[] = "tcp://180.168.146.187:10011";		// 前置地址

	CMdSpi* pUserSpi = new CMdSpi();
	
	if (pUserSpi)
	{
		pUserSpi->getMdApi()->RegisterSpi(pUserSpi);						// 注册事件类
		pUserSpi->getMdApi()->RegisterFront(FRONT_ADDR);					// connect
		pUserSpi->getMdApi()->Init();

		pUserSpi->getMdApi()->Join();
	}
//	pUserApi->Release();
}