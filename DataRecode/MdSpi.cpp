#include "MdSpi.h"
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

#pragma warning(disable : 4996)


CMdSpi::CMdSpi()
{
	init();
}

CMdSpi::~CMdSpi()
{

}

void CMdSpi::init()
{
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();

	iRequestID = 0;

	lastData.clear();

	unit_info["rb1901"] = 10;
	unit_info["jm1901"] = 60;
	unit_info["j1901"]  = 100;
	unit_info["MA901"]  = 1;
	unit_info["ru1901"] = 10;
	unit_info["SR901"]  = 1;
	

	ppInstrumentID[0] = "rb1901";
	ppInstrumentID[1] = "jm1901";
	ppInstrumentID[2] = "j1901";
	ppInstrumentID[3] = "MA901";
	ppInstrumentID[4] = "ru1901";
	ppInstrumentID[5] = "SR901";

	iInstrumentID = 6;
}
void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	//cerr << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	//cerr << "--->>> " << __FUNCTION__ << endl;
	//cerr << "--->>> Reason = " << nReason << endl;
}
		
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	//cerr << "--->>> " << __FUNCTION__ << endl;
	//cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
	//cerr << "--->>> " << __FUNCTION__ << endl;
	///�û���¼����
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	//cerr << "--->>> �����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///��ȡ��ǰ������
		//cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
		// ����������
		SubscribeMarketData();	

	}
}

void CMdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	//cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << __FUNCTION__ << endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << __FUNCTION__ << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
/*
	//cerr << __FUNCTION__ << endl;
	cerr << "--->>> ���¼�=" << pDepthMarketData->LastPrice << endl;
	
	string s = "H:\\Data\\" ;
	s.append(pDepthMarketData->InstrumentID);
	s.append("_");
	s.append(pUserApi->GetTradingDay());
	s.append(".csv");
	//InstrumentID + TradingDay..UpdateTime..UpdateMillisec + LastPrice + Volume + Turnover + OpenInterest + BidPrice1 + BidVolume1 + AskPrice1 + AskVolume1
	ofstream outfile;
	outfile.open(s, ios::app);
	outfile << pDepthMarketData->InstrumentID<<","<< pDepthMarketData->TradingDay<<","<<pDepthMarketData->UpdateTime<<":"<< pDepthMarketData->UpdateMillisec<<","<< pDepthMarketData->LastPrice
		<< "," << pDepthMarketData-> Volume << "," << pDepthMarketData-> BidPrice1 << "," << pDepthMarketData-> BidVolume1 << "," << pDepthMarketData-> AskPrice1 
		<< "," << pDepthMarketData-> AskVolume1 << "," << pDepthMarketData-> OpenInterest << "," << pDepthMarketData->Turnover<< endl;



	outfile.close();
	*/
	_dealData(pDepthMarketData);
	
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}


void CMdSpi::_dealData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (pDepthMarketData == NULL)
		return;
	
	if (pDepthMarketData->Volume == 0)
		return;

	if (lastData[pDepthMarketData->InstrumentID].Volume == 0)
	{
		lastData[pDepthMarketData->InstrumentID] = *pDepthMarketData;
	}

	string s = "H:\\Data\\";
	s.append(pDepthMarketData->InstrumentID);
	s.append("_");
	s.append(pUserApi->GetTradingDay());
	s.append(".csv");

	//InstrumentID + TradingDay..UpdateTime..UpdateMillisec + LastPrice + Volume + Turnover + OpenInterest + BidPrice1 + BidVolume1 + AskPrice1 + AskVolume1

	ofstream outfile;
	outfile.open(s, ios::app);


	//�жϽ��׷���
	int open_diff = pDepthMarketData->OpenInterest - lastData[pDepthMarketData->InstrumentID].OpenInterest;
	int vol_diff = pDepthMarketData->Volume - lastData[pDepthMarketData->InstrumentID].Volume;
	cout << pDepthMarketData->LastPrice <<" "<<vol_diff << "  "<< open_diff <<"  ";

	outfile << pDepthMarketData->InstrumentID << "," << pDepthMarketData->TradingDay << "," << pDepthMarketData->UpdateTime << ":" << pDepthMarketData->UpdateMillisec << "," << pDepthMarketData->LastPrice << "," << vol_diff << "," << open_diff << ",";
		
	if (open_diff > 0 && open_diff < vol_diff)
	{
		if (pDepthMarketData->LastPrice >= lastData[pDepthMarketData->InstrumentID].BidPrice1)
		{
			outfile << "�࿪" ;
		}
		else if(pDepthMarketData->LastPrice <= lastData[pDepthMarketData->InstrumentID].AskPrice1)
		{
			outfile << "�տ�" ;
		}
	}
	else if (open_diff > 0 && open_diff == vol_diff)
	{			
		outfile << "˫��" ;
	}
	else if(open_diff < 0 && open_diff > (-1)*vol_diff)
	{
		if (pDepthMarketData->LastPrice >= lastData[pDepthMarketData->InstrumentID].BidPrice1)
		{
			outfile << "��ƽ" ;
		}
		else if (pDepthMarketData->LastPrice <= lastData[pDepthMarketData->InstrumentID].AskPrice1)
		{
			outfile << "��ƽ" ;
		}
	}
	else if (open_diff < 0 && open_diff == (-1)*vol_diff)
	{
		outfile << "˫ƽ" ;
	}
	else
	{
		if (pDepthMarketData->LastPrice >= lastData[pDepthMarketData->InstrumentID].BidPrice1)
		{
			outfile << "�໻" ;
		}
		else if (pDepthMarketData->LastPrice <= lastData[pDepthMarketData->InstrumentID].AskPrice1)
		{
			outfile << "�ջ�" ;
		}
	}

	if (unit_info[pDepthMarketData->InstrumentID] == 0)
	{
		unit_info[pDepthMarketData->InstrumentID] = 10;
	}

	outfile << "," << ((pDepthMarketData->Turnover/ unit_info[pDepthMarketData->InstrumentID])/pDepthMarketData->Volume) <<","<< pDepthMarketData->Volume << "," << pDepthMarketData->Turnover <<"," << pDepthMarketData->OpenInterest << "," << pDepthMarketData->BidPrice1 << "," <<pDepthMarketData->BidVolume1 << "," << pDepthMarketData->AskPrice1
		<< "," << pDepthMarketData->AskVolume1 << "," << endl;

	lastData[pDepthMarketData->InstrumentID] = *pDepthMarketData;

}