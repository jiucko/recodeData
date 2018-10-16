#pragma once
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include <vector>
#include <unordered_map>

class CMdSpi : public CThostFtdcMdSpi
{

public:
	CMdSpi();
	~CMdSpi();
	void init();
public:
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	
	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
	void ReqUserLogin();
	void SubscribeMarketData();
	// 
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	void _dealData(CThostFtdcDepthMarketDataField *pDepthMarketData);

public:
	CThostFtdcMdApi* getMdApi() { return pUserApi; }

private:
	
	// UserApi����
	CThostFtdcMdApi* pUserApi;
														// ������
	int iRequestID = 0;

	int iInstrumentID = 0;

	char *ppInstrumentID[6]; 		// ���鶩���б�ע�⣬�����ԼID���ʱ�ģ�����ʱ����޸�
	TThostFtdcBrokerIDType	BROKER_ID = "9999";				// ���͹�˾����
	TThostFtdcInvestorIDType INVESTOR_ID = "127196";		// ע�������������Լ���Ͷ���ߴ���
	TThostFtdcPasswordType  PASSWORD = "ytl304354";			// ע�������������Լ����û�����

	std::unordered_map<std::string, CThostFtdcDepthMarketDataField> lastData;
	std::unordered_map<std::string, int> unit_info;
};