
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// THIS FILE IS GENERATED BY gentpcecode.py, DO NOT EDIT MANUALLY //
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


#include "tpce_generated.hpp"

using namespace duckdb;
using namespace std;

namespace TPCE {
struct tpce_append_information {
	tpce_append_information(Connection &con, string schema, string table) :
		appender(con, schema, table) {}

	Appender appender;
};

static void append_value(tpce_append_information &info, int32_t value) {
	info.appender.Append<int32_t>(value);
}

static void append_bigint(tpce_append_information &info, int64_t value) {
	info.appender.Append<int64_t>(value);
}

static void append_string(tpce_append_information &info, const char *value) {
	info.appender.Append<Value>(Value(value));
}

static void append_double(tpce_append_information &info, double value) {
	info.appender.Append<double>(value);
}

static void append_bool(tpce_append_information &info, bool value) {
	info.appender.Append<bool>(value);
}

static void append_timestamp(tpce_append_information &info, CDateTime time) {
	int32_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, msec = 0;
	time.GetYMDHMS(&year, &month, &day, &hour, &minute, &second, &msec);
	info.appender.Append<Value>(Value::TIMESTAMP(year, month, day, hour, minute, second, msec * 1000));
}

void append_char(tpce_append_information &info, char value) {
	char val[2];
	val[0] = value;
	val[1] = '\0';
	append_string(info, val);
}

template <typename T> class DuckDBBaseLoader : public CBaseLoader<T> {
  protected:
	tpce_append_information info;

  public:
	DuckDBBaseLoader(Connection &con, string schema, string table) :
		info(con, schema, table) {
	}

	void FinishLoad() {

	}
};


class DuckDBAccountPermissionLoad : public DuckDBBaseLoader<ACCOUNT_PERMISSION_ROW> {
public:
	DuckDBAccountPermissionLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const ACCOUNT_PERMISSION_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.AP_CA_ID);
		append_string(info, next_record.AP_ACL);
		append_string(info, next_record.AP_TAX_ID);
		append_string(info, next_record.AP_L_NAME);
		append_string(info, next_record.AP_F_NAME);
		info.appender.EndRow();
	}

};
class DuckDBAddressLoad : public DuckDBBaseLoader<ADDRESS_ROW> {
public:
	DuckDBAddressLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const ADDRESS_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.AD_ID);
		append_string(info, next_record.AD_LINE1);
		append_string(info, next_record.AD_LINE2);
		append_string(info, next_record.AD_ZC_CODE);
		append_string(info, next_record.AD_CTRY);
		info.appender.EndRow();
	}

};
class DuckDBBrokerLoad : public DuckDBBaseLoader<BROKER_ROW> {
public:
	DuckDBBrokerLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const BROKER_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.B_ID);
		append_string(info, next_record.B_ST_ID);
		append_string(info, next_record.B_NAME);
		append_value(info, next_record.B_NUM_TRADES);
		append_double(info, next_record.B_COMM_TOTAL);
		info.appender.EndRow();
	}

};
class DuckDBCashTransactionLoad : public DuckDBBaseLoader<CASH_TRANSACTION_ROW> {
public:
	DuckDBCashTransactionLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const CASH_TRANSACTION_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.CT_T_ID);
		append_timestamp(info, next_record.CT_DTS);
		append_double(info, next_record.CT_AMT);
		append_string(info, next_record.CT_NAME);
		info.appender.EndRow();
	}

};
class DuckDBChargeLoad : public DuckDBBaseLoader<CHARGE_ROW> {
public:
	DuckDBChargeLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const CHARGE_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.CH_TT_ID);
		append_value(info, next_record.CH_C_TIER);
		append_double(info, next_record.CH_CHRG);
		info.appender.EndRow();
	}

};
class DuckDBCommissionRateLoad : public DuckDBBaseLoader<COMMISSION_RATE_ROW> {
public:
	DuckDBCommissionRateLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const COMMISSION_RATE_ROW &next_record) {
		info.appender.BeginRow();
		append_value(info, next_record.CR_C_TIER);
		append_string(info, next_record.CR_TT_ID);
		append_string(info, next_record.CR_EX_ID);
		append_value(info, next_record.CR_FROM_QTY);
		append_value(info, next_record.CR_TO_QTY);
		append_double(info, next_record.CR_RATE);
		info.appender.EndRow();
	}

};
class DuckDBCompanyLoad : public DuckDBBaseLoader<COMPANY_ROW> {
public:
	DuckDBCompanyLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const COMPANY_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.CO_ID);
		append_string(info, next_record.CO_ST_ID);
		append_string(info, next_record.CO_NAME);
		append_string(info, next_record.CO_IN_ID);
		append_string(info, next_record.CO_SP_RATE);
		append_string(info, next_record.CO_CEO);
		append_bigint(info, next_record.CO_AD_ID);
		append_string(info, next_record.CO_DESC);
		append_timestamp(info, next_record.CO_OPEN_DATE);
		info.appender.EndRow();
	}

};
class DuckDBCompanyCompetitorLoad : public DuckDBBaseLoader<COMPANY_COMPETITOR_ROW> {
public:
	DuckDBCompanyCompetitorLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const COMPANY_COMPETITOR_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.CP_CO_ID);
		append_bigint(info, next_record.CP_COMP_CO_ID);
		append_string(info, next_record.CP_IN_ID);
		info.appender.EndRow();
	}

};
class DuckDBCustomerLoad : public DuckDBBaseLoader<CUSTOMER_ROW> {
public:
	DuckDBCustomerLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const CUSTOMER_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.C_ID);
		append_string(info, next_record.C_TAX_ID);
		append_string(info, next_record.C_ST_ID);
		append_string(info, next_record.C_L_NAME);
		append_string(info, next_record.C_F_NAME);
		append_string(info, next_record.C_M_NAME);
		append_char(info, next_record.C_GNDR);
		append_char(info, next_record.C_TIER);
		append_timestamp(info, next_record.C_DOB);
		append_bigint(info, next_record.C_AD_ID);
		append_string(info, next_record.C_CTRY_1);
		append_string(info, next_record.C_AREA_1);
		append_string(info, next_record.C_LOCAL_1);
		append_string(info, next_record.C_EXT_1);
		append_string(info, next_record.C_CTRY_2);
		append_string(info, next_record.C_AREA_2);
		append_string(info, next_record.C_LOCAL_2);
		append_string(info, next_record.C_EXT_2);
		append_string(info, next_record.C_CTRY_3);
		append_string(info, next_record.C_AREA_3);
		append_string(info, next_record.C_LOCAL_3);
		append_string(info, next_record.C_EXT_3);
		append_string(info, next_record.C_EMAIL_1);
		append_string(info, next_record.C_EMAIL_2);
		info.appender.EndRow();
	}

};
class DuckDBCustomerAccountLoad : public DuckDBBaseLoader<CUSTOMER_ACCOUNT_ROW> {
public:
	DuckDBCustomerAccountLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const CUSTOMER_ACCOUNT_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.CA_ID);
		append_bigint(info, next_record.CA_B_ID);
		append_bigint(info, next_record.CA_C_ID);
		append_string(info, next_record.CA_NAME);
		append_char(info, next_record.CA_TAX_ST);
		append_double(info, next_record.CA_BAL);
		info.appender.EndRow();
	}

};
class DuckDBCustomerTaxrateLoad : public DuckDBBaseLoader<CUSTOMER_TAXRATE_ROW> {
public:
	DuckDBCustomerTaxrateLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const CUSTOMER_TAXRATE_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.CX_TX_ID);
		append_bigint(info, next_record.CX_C_ID);
		info.appender.EndRow();
	}

};
class DuckDBDailyMarketLoad : public DuckDBBaseLoader<DAILY_MARKET_ROW> {
public:
	DuckDBDailyMarketLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const DAILY_MARKET_ROW &next_record) {
		info.appender.BeginRow();
		append_timestamp(info, next_record.DM_DATE);
		append_string(info, next_record.DM_S_SYMB);
		append_double(info, next_record.DM_CLOSE);
		append_double(info, next_record.DM_HIGH);
		append_double(info, next_record.DM_LOW);
		append_bigint(info, next_record.DM_VOL);
		info.appender.EndRow();
	}

};
class DuckDBExchangeLoad : public DuckDBBaseLoader<EXCHANGE_ROW> {
public:
	DuckDBExchangeLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const EXCHANGE_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.EX_ID);
		append_string(info, next_record.EX_NAME);
		append_value(info, next_record.EX_NUM_SYMB);
		append_value(info, next_record.EX_OPEN);
		append_value(info, next_record.EX_CLOSE);
		append_string(info, next_record.EX_DESC);
		append_bigint(info, next_record.EX_AD_ID);
		info.appender.EndRow();
	}

};
class DuckDBFinancialLoad : public DuckDBBaseLoader<FINANCIAL_ROW> {
public:
	DuckDBFinancialLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const FINANCIAL_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.FI_CO_ID);
		append_value(info, next_record.FI_YEAR);
		append_value(info, next_record.FI_QTR);
		append_timestamp(info, next_record.FI_QTR_START_DATE);
		append_double(info, next_record.FI_REVENUE);
		append_double(info, next_record.FI_NET_EARN);
		append_double(info, next_record.FI_BASIC_EPS);
		append_double(info, next_record.FI_DILUT_EPS);
		append_double(info, next_record.FI_MARGIN);
		append_double(info, next_record.FI_INVENTORY);
		append_double(info, next_record.FI_ASSETS);
		append_double(info, next_record.FI_LIABILITY);
		append_bigint(info, next_record.FI_OUT_BASIC);
		append_bigint(info, next_record.FI_OUT_DILUT);
		info.appender.EndRow();
	}

};
class DuckDBHoldingLoad : public DuckDBBaseLoader<HOLDING_ROW> {
public:
	DuckDBHoldingLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const HOLDING_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.H_T_ID);
		append_bigint(info, next_record.H_CA_ID);
		append_string(info, next_record.H_S_SYMB);
		append_timestamp(info, next_record.H_DTS);
		append_double(info, next_record.H_PRICE);
		append_value(info, next_record.H_QTY);
		info.appender.EndRow();
	}

};
class DuckDBHoldingHistoryLoad : public DuckDBBaseLoader<HOLDING_HISTORY_ROW> {
public:
	DuckDBHoldingHistoryLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const HOLDING_HISTORY_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.HH_H_T_ID);
		append_bigint(info, next_record.HH_T_ID);
		append_value(info, next_record.HH_BEFORE_QTY);
		append_value(info, next_record.HH_AFTER_QTY);
		info.appender.EndRow();
	}

};
class DuckDBHoldingSummaryLoad : public DuckDBBaseLoader<HOLDING_SUMMARY_ROW> {
public:
	DuckDBHoldingSummaryLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const HOLDING_SUMMARY_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.HS_CA_ID);
		append_string(info, next_record.HS_S_SYMB);
		append_value(info, next_record.HS_QTY);
		info.appender.EndRow();
	}

};
class DuckDBIndustryLoad : public DuckDBBaseLoader<INDUSTRY_ROW> {
public:
	DuckDBIndustryLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const INDUSTRY_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.IN_ID);
		append_string(info, next_record.IN_NAME);
		append_string(info, next_record.IN_SC_ID);
		info.appender.EndRow();
	}

};
class DuckDBLastTradeLoad : public DuckDBBaseLoader<LAST_TRADE_ROW> {
public:
	DuckDBLastTradeLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const LAST_TRADE_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.LT_S_SYMB);
		append_timestamp(info, next_record.LT_DTS);
		append_double(info, next_record.LT_PRICE);
		append_double(info, next_record.LT_OPEN_PRICE);
		append_bigint(info, next_record.LT_VOL);
		info.appender.EndRow();
	}

};
class DuckDBNewsItemLoad : public DuckDBBaseLoader<NEWS_ITEM_ROW> {
public:
	DuckDBNewsItemLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const NEWS_ITEM_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.NI_ID);
		append_string(info, next_record.NI_HEADLINE);
		append_string(info, next_record.NI_SUMMARY);
		append_string(info, next_record.NI_ITEM);
		append_timestamp(info, next_record.NI_DTS);
		append_string(info, next_record.NI_SOURCE);
		append_string(info, next_record.NI_AUTHOR);
		info.appender.EndRow();
	}

};
class DuckDBNewsXRefLoad : public DuckDBBaseLoader<NEWS_XREF_ROW> {
public:
	DuckDBNewsXRefLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const NEWS_XREF_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.NX_NI_ID);
		append_bigint(info, next_record.NX_CO_ID);
		info.appender.EndRow();
	}

};
class DuckDBSectorLoad : public DuckDBBaseLoader<SECTOR_ROW> {
public:
	DuckDBSectorLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const SECTOR_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.SC_ID);
		append_string(info, next_record.SC_NAME);
		info.appender.EndRow();
	}

};
class DuckDBSecurityLoad : public DuckDBBaseLoader<SECURITY_ROW> {
public:
	DuckDBSecurityLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const SECURITY_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.S_SYMB);
		append_string(info, next_record.S_ISSUE);
		append_string(info, next_record.S_ST_ID);
		append_string(info, next_record.S_NAME);
		append_string(info, next_record.S_EX_ID);
		append_bigint(info, next_record.S_CO_ID);
		append_bigint(info, next_record.S_NUM_OUT);
		append_timestamp(info, next_record.S_START_DATE);
		append_timestamp(info, next_record.S_EXCH_DATE);
		append_double(info, next_record.S_PE);
		append_double(info, next_record.S_52WK_HIGH);
		append_timestamp(info, next_record.S_52WK_HIGH_DATE);
		append_double(info, next_record.S_52WK_LOW);
		append_timestamp(info, next_record.S_52WK_LOW_DATE);
		append_double(info, next_record.S_DIVIDEND);
		append_double(info, next_record.S_YIELD);
		info.appender.EndRow();
	}

};
class DuckDBSettlementLoad : public DuckDBBaseLoader<SETTLEMENT_ROW> {
public:
	DuckDBSettlementLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const SETTLEMENT_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.SE_T_ID);
		append_string(info, next_record.SE_CASH_TYPE);
		append_timestamp(info, next_record.SE_CASH_DUE_DATE);
		append_double(info, next_record.SE_AMT);
		info.appender.EndRow();
	}

};
class DuckDBStatusTypeLoad : public DuckDBBaseLoader<STATUS_TYPE_ROW> {
public:
	DuckDBStatusTypeLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const STATUS_TYPE_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.ST_ID);
		append_string(info, next_record.ST_NAME);
		info.appender.EndRow();
	}

};
class DuckDBTaxRateLoad : public DuckDBBaseLoader<TAX_RATE_ROW> {
public:
	DuckDBTaxRateLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const TAX_RATE_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.TX_ID);
		append_string(info, next_record.TX_NAME);
		append_double(info, next_record.TX_RATE);
		info.appender.EndRow();
	}

};
class DuckDBTradeLoad : public DuckDBBaseLoader<TRADE_ROW> {
public:
	DuckDBTradeLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const TRADE_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.T_ID);
		append_timestamp(info, next_record.T_DTS);
		append_string(info, next_record.T_ST_ID);
		append_string(info, next_record.T_TT_ID);
		append_bool(info, next_record.T_IS_CASH);
		append_string(info, next_record.T_S_SYMB);
		append_value(info, next_record.T_QTY);
		append_double(info, next_record.T_BID_PRICE);
		append_bigint(info, next_record.T_CA_ID);
		append_string(info, next_record.T_EXEC_NAME);
		append_double(info, next_record.T_TRADE_PRICE);
		append_double(info, next_record.T_CHRG);
		append_double(info, next_record.T_COMM);
		append_double(info, next_record.T_TAX);
		append_bool(info, next_record.T_LIFO);
		info.appender.EndRow();
	}

};
class DuckDBTradeHistoryLoad : public DuckDBBaseLoader<TRADE_HISTORY_ROW> {
public:
	DuckDBTradeHistoryLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const TRADE_HISTORY_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.TH_T_ID);
		append_timestamp(info, next_record.TH_DTS);
		append_string(info, next_record.TH_ST_ID);
		info.appender.EndRow();
	}

};
class DuckDBTradeRequestLoad : public DuckDBBaseLoader<TRADE_REQUEST_ROW> {
public:
	DuckDBTradeRequestLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const TRADE_REQUEST_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.TR_T_ID);
		append_string(info, next_record.TR_TT_ID);
		append_string(info, next_record.TR_S_SYMB);
		append_value(info, next_record.TR_QTY);
		append_double(info, next_record.TR_BID_PRICE);
		append_bigint(info, next_record.TR_B_ID);
		info.appender.EndRow();
	}

};
class DuckDBTradeTypeLoad : public DuckDBBaseLoader<TRADE_TYPE_ROW> {
public:
	DuckDBTradeTypeLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const TRADE_TYPE_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.TT_ID);
		append_string(info, next_record.TT_NAME);
		append_bool(info, next_record.TT_IS_SELL);
		append_bool(info, next_record.TT_IS_MRKT);
		info.appender.EndRow();
	}

};
class DuckDBWatchItemLoad : public DuckDBBaseLoader<WATCH_ITEM_ROW> {
public:
	DuckDBWatchItemLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const WATCH_ITEM_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.WI_WL_ID);
		append_string(info, next_record.WI_S_SYMB);
		info.appender.EndRow();
	}

};
class DuckDBWatchListLoad : public DuckDBBaseLoader<WATCH_LIST_ROW> {
public:
	DuckDBWatchListLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const WATCH_LIST_ROW &next_record) {
		info.appender.BeginRow();
		append_bigint(info, next_record.WL_ID);
		append_bigint(info, next_record.WL_C_ID);
		info.appender.EndRow();
	}

};
class DuckDBZipCodeLoad : public DuckDBBaseLoader<ZIP_CODE_ROW> {
public:
	DuckDBZipCodeLoad(Connection &con, string schema, string table) :
		DuckDBBaseLoader(con, schema, table) {

	}

	void WriteNextRecord(const ZIP_CODE_ROW &next_record) {
		info.appender.BeginRow();
		append_string(info, next_record.ZC_CODE);
		append_string(info, next_record.ZC_TOWN);
		append_string(info, next_record.ZC_DIV);
		info.appender.EndRow();
	}

};
CBaseLoader<ACCOUNT_PERMISSION_ROW> *
DuckDBLoaderFactory::CreateAccountPermissionLoader() {
	return new DuckDBAccountPermissionLoad(con, schema, "account_permission" + suffix);
}

CBaseLoader<ADDRESS_ROW> *
DuckDBLoaderFactory::CreateAddressLoader() {
	return new DuckDBAddressLoad(con, schema, "address" + suffix);
}

CBaseLoader<BROKER_ROW> *
DuckDBLoaderFactory::CreateBrokerLoader() {
	return new DuckDBBrokerLoad(con, schema, "broker" + suffix);
}

CBaseLoader<CASH_TRANSACTION_ROW> *
DuckDBLoaderFactory::CreateCashTransactionLoader() {
	return new DuckDBCashTransactionLoad(con, schema, "cash_transaction" + suffix);
}

CBaseLoader<CHARGE_ROW> *
DuckDBLoaderFactory::CreateChargeLoader() {
	return new DuckDBChargeLoad(con, schema, "charge" + suffix);
}

CBaseLoader<COMMISSION_RATE_ROW> *
DuckDBLoaderFactory::CreateCommissionRateLoader() {
	return new DuckDBCommissionRateLoad(con, schema, "commission_rate" + suffix);
}

CBaseLoader<COMPANY_ROW> *
DuckDBLoaderFactory::CreateCompanyLoader() {
	return new DuckDBCompanyLoad(con, schema, "company" + suffix);
}

CBaseLoader<COMPANY_COMPETITOR_ROW> *
DuckDBLoaderFactory::CreateCompanyCompetitorLoader() {
	return new DuckDBCompanyCompetitorLoad(con, schema, "company_competitor" + suffix);
}

CBaseLoader<CUSTOMER_ROW> *
DuckDBLoaderFactory::CreateCustomerLoader() {
	return new DuckDBCustomerLoad(con, schema, "customer" + suffix);
}

CBaseLoader<CUSTOMER_ACCOUNT_ROW> *
DuckDBLoaderFactory::CreateCustomerAccountLoader() {
	return new DuckDBCustomerAccountLoad(con, schema, "customer_account" + suffix);
}

CBaseLoader<CUSTOMER_TAXRATE_ROW> *
DuckDBLoaderFactory::CreateCustomerTaxrateLoader() {
	return new DuckDBCustomerTaxrateLoad(con, schema, "customer_taxrate" + suffix);
}

CBaseLoader<DAILY_MARKET_ROW> *
DuckDBLoaderFactory::CreateDailyMarketLoader() {
	return new DuckDBDailyMarketLoad(con, schema, "daily_market" + suffix);
}

CBaseLoader<EXCHANGE_ROW> *
DuckDBLoaderFactory::CreateExchangeLoader() {
	return new DuckDBExchangeLoad(con, schema, "exchange" + suffix);
}

CBaseLoader<FINANCIAL_ROW> *
DuckDBLoaderFactory::CreateFinancialLoader() {
	return new DuckDBFinancialLoad(con, schema, "financial" + suffix);
}

CBaseLoader<HOLDING_ROW> *
DuckDBLoaderFactory::CreateHoldingLoader() {
	return new DuckDBHoldingLoad(con, schema, "holding" + suffix);
}

CBaseLoader<HOLDING_HISTORY_ROW> *
DuckDBLoaderFactory::CreateHoldingHistoryLoader() {
	return new DuckDBHoldingHistoryLoad(con, schema, "holding_history" + suffix);
}

CBaseLoader<HOLDING_SUMMARY_ROW> *
DuckDBLoaderFactory::CreateHoldingSummaryLoader() {
	return new DuckDBHoldingSummaryLoad(con, schema, "holding_summary" + suffix);
}

CBaseLoader<INDUSTRY_ROW> *
DuckDBLoaderFactory::CreateIndustryLoader() {
	return new DuckDBIndustryLoad(con, schema, "industry" + suffix);
}

CBaseLoader<LAST_TRADE_ROW> *
DuckDBLoaderFactory::CreateLastTradeLoader() {
	return new DuckDBLastTradeLoad(con, schema, "last_trade" + suffix);
}

CBaseLoader<NEWS_ITEM_ROW> *
DuckDBLoaderFactory::CreateNewsItemLoader() {
	return new DuckDBNewsItemLoad(con, schema, "news_item" + suffix);
}

CBaseLoader<NEWS_XREF_ROW> *
DuckDBLoaderFactory::CreateNewsXRefLoader() {
	return new DuckDBNewsXRefLoad(con, schema, "news_xref" + suffix);
}

CBaseLoader<SECTOR_ROW> *
DuckDBLoaderFactory::CreateSectorLoader() {
	return new DuckDBSectorLoad(con, schema, "sector" + suffix);
}

CBaseLoader<SECURITY_ROW> *
DuckDBLoaderFactory::CreateSecurityLoader() {
	return new DuckDBSecurityLoad(con, schema, "security" + suffix);
}

CBaseLoader<SETTLEMENT_ROW> *
DuckDBLoaderFactory::CreateSettlementLoader() {
	return new DuckDBSettlementLoad(con, schema, "settlement" + suffix);
}

CBaseLoader<STATUS_TYPE_ROW> *
DuckDBLoaderFactory::CreateStatusTypeLoader() {
	return new DuckDBStatusTypeLoad(con, schema, "status_type" + suffix);
}

CBaseLoader<TAX_RATE_ROW> *
DuckDBLoaderFactory::CreateTaxRateLoader() {
	return new DuckDBTaxRateLoad(con, schema, "tax_rate" + suffix);
}

CBaseLoader<TRADE_ROW> *
DuckDBLoaderFactory::CreateTradeLoader() {
	return new DuckDBTradeLoad(con, schema, "trade" + suffix);
}

CBaseLoader<TRADE_HISTORY_ROW> *
DuckDBLoaderFactory::CreateTradeHistoryLoader() {
	return new DuckDBTradeHistoryLoad(con, schema, "trade_history" + suffix);
}

CBaseLoader<TRADE_REQUEST_ROW> *
DuckDBLoaderFactory::CreateTradeRequestLoader() {
	return new DuckDBTradeRequestLoad(con, schema, "trade_request" + suffix);
}

CBaseLoader<TRADE_TYPE_ROW> *
DuckDBLoaderFactory::CreateTradeTypeLoader() {
	return new DuckDBTradeTypeLoad(con, schema, "trade_type" + suffix);
}

CBaseLoader<WATCH_ITEM_ROW> *
DuckDBLoaderFactory::CreateWatchItemLoader() {
	return new DuckDBWatchItemLoad(con, schema, "watch_item" + suffix);
}

CBaseLoader<WATCH_LIST_ROW> *
DuckDBLoaderFactory::CreateWatchListLoader() {
	return new DuckDBWatchListLoad(con, schema, "watch_list" + suffix);
}

CBaseLoader<ZIP_CODE_ROW> *
DuckDBLoaderFactory::CreateZipCodeLoader() {
	return new DuckDBZipCodeLoad(con, schema, "zip_code" + suffix);
}

static string AccountPermissionSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".account_permission" + suffix + " ("
	    "ap_ca_id BIGINT,"
	    "ap_acl VARCHAR,"
	    "ap_tax_id VARCHAR,"
	    "ap_l_name VARCHAR,"
	    "ap_f_name VARCHAR)";
}

static string AddressSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".address" + suffix + " ("
	    "ad_id BIGINT,"
	    "ad_line1 VARCHAR,"
	    "ad_line2 VARCHAR,"
	    "ad_zc_code VARCHAR,"
	    "ad_ctry VARCHAR)";
}

static string BrokerSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".broker" + suffix + " ("
	    "b_id BIGINT,"
	    "b_st_id VARCHAR,"
	    "b_name VARCHAR,"
	    "b_num_trades INTEGER,"
	    "b_comm_total DECIMAL)";
}

static string CashTransactionSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".cash_transaction" + suffix + " ("
	    "ct_t_id BIGINT,"
	    "ct_dts TIMESTAMP,"
	    "ct_amt DECIMAL,"
	    "ct_name VARCHAR)";
}

static string ChargeSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".charge" + suffix + " ("
	    "ch_tt_id VARCHAR,"
	    "ch_c_tier INTEGER,"
	    "ch_chrg DECIMAL)";
}

static string CommissionRateSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".commission_rate" + suffix + " ("
	    "cr_c_tier INTEGER,"
	    "cr_tt_id VARCHAR,"
	    "cr_ex_id VARCHAR,"
	    "cr_from_qty INTEGER,"
	    "cr_to_qty INTEGER,"
	    "cr_rate DECIMAL)";
}

static string CompanySchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".company" + suffix + " ("
	    "co_id BIGINT,"
	    "co_st_id VARCHAR,"
	    "co_name VARCHAR,"
	    "co_in_id VARCHAR,"
	    "co_sp_rate VARCHAR,"
	    "co_ceo VARCHAR,"
	    "co_ad_id BIGINT,"
	    "co_desc VARCHAR,"
	    "co_open_date TIMESTAMP)";
}

static string CompanyCompetitorSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".company_competitor" + suffix + " ("
	    "cp_co_id BIGINT,"
	    "cp_comp_co_id BIGINT,"
	    "cp_in_id VARCHAR)";
}

static string CustomerSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".customer" + suffix + " ("
	    "c_id BIGINT,"
	    "c_tax_id VARCHAR,"
	    "c_st_id VARCHAR,"
	    "c_l_name VARCHAR,"
	    "c_f_name VARCHAR,"
	    "c_m_name VARCHAR,"
	    "c_gndr VARCHAR,"
	    "c_tier VARCHAR,"
	    "c_dob TIMESTAMP,"
	    "c_ad_id BIGINT,"
	    "c_ctry_1 VARCHAR,"
	    "c_area_1 VARCHAR,"
	    "c_local_1 VARCHAR,"
	    "c_ext_1 VARCHAR,"
	    "c_ctry_2 VARCHAR,"
	    "c_area_2 VARCHAR,"
	    "c_local_2 VARCHAR,"
	    "c_ext_2 VARCHAR,"
	    "c_ctry_3 VARCHAR,"
	    "c_area_3 VARCHAR,"
	    "c_local_3 VARCHAR,"
	    "c_ext_3 VARCHAR,"
	    "c_email_1 VARCHAR,"
	    "c_email_2 VARCHAR)";
}

static string CustomerAccountSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".customer_account" + suffix + " ("
	    "ca_id BIGINT,"
	    "ca_b_id BIGINT,"
	    "ca_c_id BIGINT,"
	    "ca_name VARCHAR,"
	    "ca_tax_st VARCHAR,"
	    "ca_bal DECIMAL)";
}

static string CustomerTaxrateSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".customer_taxrate" + suffix + " ("
	    "cx_tx_id VARCHAR,"
	    "cx_c_id BIGINT)";
}

static string DailyMarketSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".daily_market" + suffix + " ("
	    "dm_date TIMESTAMP,"
	    "dm_s_symb VARCHAR,"
	    "dm_close DECIMAL,"
	    "dm_high DECIMAL,"
	    "dm_low DECIMAL,"
	    "dm_vol BIGINT)";
}

static string ExchangeSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".exchange" + suffix + " ("
	    "ex_id VARCHAR,"
	    "ex_name VARCHAR,"
	    "ex_num_symb INTEGER,"
	    "ex_open INTEGER,"
	    "ex_close INTEGER,"
	    "ex_desc VARCHAR,"
	    "ex_ad_id BIGINT)";
}

static string FinancialSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".financial" + suffix + " ("
	    "fi_co_id BIGINT,"
	    "fi_year INTEGER,"
	    "fi_qtr INTEGER,"
	    "fi_qtr_start_date TIMESTAMP,"
	    "fi_revenue DECIMAL,"
	    "fi_net_earn DECIMAL,"
	    "fi_basic_eps DECIMAL,"
	    "fi_dilut_eps DECIMAL,"
	    "fi_margin DECIMAL,"
	    "fi_inventory DECIMAL,"
	    "fi_assets DECIMAL,"
	    "fi_liability DECIMAL,"
	    "fi_out_basic BIGINT,"
	    "fi_out_dilut BIGINT)";
}

static string HoldingSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".holding" + suffix + " ("
	    "h_t_id BIGINT,"
	    "h_ca_id BIGINT,"
	    "h_s_symb VARCHAR,"
	    "h_dts TIMESTAMP,"
	    "h_price DECIMAL,"
	    "h_qty INTEGER)";
}

static string HoldingHistorySchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".holding_history" + suffix + " ("
	    "hh_h_t_id BIGINT,"
	    "hh_t_id BIGINT,"
	    "hh_before_qty INTEGER,"
	    "hh_after_qty INTEGER)";
}

static string HoldingSummarySchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".holding_summary" + suffix + " ("
	    "hs_ca_id BIGINT,"
	    "hs_s_symb VARCHAR,"
	    "hs_qty INTEGER)";
}

static string IndustrySchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".industry" + suffix + " ("
	    "in_id VARCHAR,"
	    "in_name VARCHAR,"
	    "in_sc_id VARCHAR)";
}

static string LastTradeSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".last_trade" + suffix + " ("
	    "lt_s_symb VARCHAR,"
	    "lt_dts TIMESTAMP,"
	    "lt_price DECIMAL,"
	    "lt_open_price DECIMAL,"
	    "lt_vol BIGINT)";
}

static string NewsItemSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".news_item" + suffix + " ("
	    "ni_id BIGINT,"
	    "ni_headline VARCHAR,"
	    "ni_summary VARCHAR,"
	    "ni_item VARCHAR,"
	    "ni_dts TIMESTAMP,"
	    "ni_source VARCHAR,"
	    "ni_author VARCHAR)";
}

static string NewsXrefSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".news_xref" + suffix + " ("
	    "nx_ni_id BIGINT,"
	    "nx_co_id BIGINT)";
}

static string SectorSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".sector" + suffix + " ("
	    "sc_id VARCHAR,"
	    "sc_name VARCHAR)";
}

static string SecuritySchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".security" + suffix + " ("
	    "s_symb VARCHAR,"
	    "s_issue VARCHAR,"
	    "s_st_id VARCHAR,"
	    "s_name VARCHAR,"
	    "s_ex_id VARCHAR,"
	    "s_co_id BIGINT,"
	    "s_num_out BIGINT,"
	    "s_start_date TIMESTAMP,"
	    "s_exch_date TIMESTAMP,"
	    "s_pe DECIMAL,"
	    "s_52wk_high DECIMAL,"
	    "s_52wk_high_date TIMESTAMP,"
	    "s_52wk_low DECIMAL,"
	    "s_52wk_low_date TIMESTAMP,"
	    "s_dividend DECIMAL,"
	    "s_yield DECIMAL)";
}

static string SettlementSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".settlement" + suffix + " ("
	    "se_t_id BIGINT,"
	    "se_cash_type VARCHAR,"
	    "se_cash_due_date TIMESTAMP,"
	    "se_amt DECIMAL)";
}

static string StatusTypeSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".status_type" + suffix + " ("
	    "st_id VARCHAR,"
	    "st_name VARCHAR)";
}

static string TaxRateSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".tax_rate" + suffix + " ("
	    "tx_id VARCHAR,"
	    "tx_name VARCHAR,"
	    "tx_rate DECIMAL)";
}

static string TradeSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".trade" + suffix + " ("
	    "t_id BIGINT,"
	    "t_dts TIMESTAMP,"
	    "t_st_id VARCHAR,"
	    "t_tt_id VARCHAR,"
	    "t_is_cash BOOLEAN,"
	    "t_s_symb VARCHAR,"
	    "t_qty INTEGER,"
	    "t_bid_price DECIMAL,"
	    "t_ca_id BIGINT,"
	    "t_exec_name VARCHAR,"
	    "t_trade_price DECIMAL,"
	    "t_chrg DECIMAL,"
	    "t_comm DECIMAL,"
	    "t_tax DECIMAL,"
	    "t_lifo BOOLEAN)";
}

static string TradeHistorySchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".trade_history" + suffix + " ("
	    "th_t_id BIGINT,"
	    "th_dts TIMESTAMP,"
	    "th_st_id VARCHAR)";
}

static string TradeRequestSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".trade_request" + suffix + " ("
	    "tr_t_id BIGINT,"
	    "tr_tt_id VARCHAR,"
	    "tr_s_symb VARCHAR,"
	    "tr_qty INTEGER,"
	    "tr_bid_price DECIMAL,"
	    "tr_b_id BIGINT)";
}

static string TradeTypeSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".trade_type" + suffix + " ("
	    "tt_id VARCHAR,"
	    "tt_name VARCHAR,"
	    "tt_is_sell BOOLEAN,"
	    "tt_is_mrkt BOOLEAN)";
}

static string WatchItemSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".watch_item" + suffix + " ("
	    "wi_wl_id BIGINT,"
	    "wi_s_symb VARCHAR)";
}

static string WatchListSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".watch_list" + suffix + " ("
	    "wl_id BIGINT,"
	    "wl_c_id BIGINT)";
}

static string ZipCodeSchema(string schema, string suffix) {
	return "CREATE TABLE " + schema + ".zip_code" + suffix + " ("
	    "zc_code VARCHAR,"
	    "zc_town VARCHAR,"
	    "zc_div VARCHAR)";
}

void CreateTPCESchema(duckdb::DuckDB &db, duckdb::Connection &con, std::string &schema, std::string &suffix) {
	con.Query(AccountPermissionSchema(schema, suffix));
	con.Query(AddressSchema(schema, suffix));
	con.Query(BrokerSchema(schema, suffix));
	con.Query(CashTransactionSchema(schema, suffix));
	con.Query(ChargeSchema(schema, suffix));
	con.Query(CommissionRateSchema(schema, suffix));
	con.Query(CompanySchema(schema, suffix));
	con.Query(CompanyCompetitorSchema(schema, suffix));
	con.Query(CustomerSchema(schema, suffix));
	con.Query(CustomerAccountSchema(schema, suffix));
	con.Query(CustomerTaxrateSchema(schema, suffix));
	con.Query(DailyMarketSchema(schema, suffix));
	con.Query(ExchangeSchema(schema, suffix));
	con.Query(FinancialSchema(schema, suffix));
	con.Query(HoldingSchema(schema, suffix));
	con.Query(HoldingHistorySchema(schema, suffix));
	con.Query(HoldingSummarySchema(schema, suffix));
	con.Query(IndustrySchema(schema, suffix));
	con.Query(LastTradeSchema(schema, suffix));
	con.Query(NewsItemSchema(schema, suffix));
	con.Query(NewsXrefSchema(schema, suffix));
	con.Query(SectorSchema(schema, suffix));
	con.Query(SecuritySchema(schema, suffix));
	con.Query(SettlementSchema(schema, suffix));
	con.Query(StatusTypeSchema(schema, suffix));
	con.Query(TaxRateSchema(schema, suffix));
	con.Query(TradeSchema(schema, suffix));
	con.Query(TradeHistorySchema(schema, suffix));
	con.Query(TradeRequestSchema(schema, suffix));
	con.Query(TradeTypeSchema(schema, suffix));
	con.Query(WatchItemSchema(schema, suffix));
	con.Query(WatchListSchema(schema, suffix));
	con.Query(ZipCodeSchema(schema, suffix));
}

} /* namespace TPCE */
