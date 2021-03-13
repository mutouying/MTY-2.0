#include "stdafx.h"
#include "KTime.h"

//秒为单位
#define defTimeSecBySec 1
#define defTimeMinBySec (60 * defTimeSecBySec)
#define defTimeHourBySec (60 * defTimeMinBySec)
#define defTimeDayBySec (24 * defTimeHourBySec)

//毫秒为单位
#define defTimeSecByMillSec (1000 * defTimeSecBySec)
#define defTimeMinByMillSec (1000 * defTimeMinBySec)
#define defTimeHourByMillSec (1000 * defTimeHourBySec)
#define defTimeDayByMillSec (1000 * defTimeDayBySec)

#define defTimeMax64_t_ 0x793406fffi64 //_MAX__TIME64_T 

KTimeSpan::KTimeSpan( __time64_t nTimeSpan /*= 0*/ ) : m_nCurTimeSpan(nTimeSpan)
{
	
}

KTimeSpan::KTimeSpan(int nDay, int nHour, int nMin, int nSecond)
{
	m_nCurTimeSpan = nDay * defTimeDayBySec + nHour * defTimeHourBySec + nMin * defTimeMinBySec + nSecond * defTimeSecBySec;
}

KTimeSpan::~KTimeSpan()
{

}

int KTimeSpan::GetDays() const
{
	return int(m_nCurTimeSpan / defTimeDayBySec);
}

int KTimeSpan::GetSeconds() const
{
	return int(m_nCurTimeSpan / defTimeSecBySec);
}

int KTimeSpan::GetHours() const
{
	return int(m_nCurTimeSpan / defTimeHourBySec);
}

int KTimeSpan::GetMinuti() const
{
	return int(m_nCurTimeSpan / defTimeMinBySec);
}

//////////////////////////////////////////////////////////////////////////
KTime::KTime() : m_nCurTime(0)
{
}

KTime::KTime( __time64_t time64 )
{
	m_nCurTime = 0;
	SetTime(time64);
}

KTime::KTime( __time32_t time32 )
{
	m_nCurTime = 0;
	SetTime(time32);
}

KTime::KTime(unsigned int nYear, unsigned int nMonth, unsigned int nDay, unsigned int nHour, unsigned int nMin, unsigned int nSecond)
{
	SYSTEMTIME stTime;
	stTime.wYear = static_cast<WORD>(nYear);
	stTime.wMonth = static_cast<WORD>(nMonth);
	stTime.wDay = static_cast<WORD>(nDay);
	stTime.wHour = static_cast<WORD>(nHour);
	stTime.wMinute = static_cast<WORD>(nMin);
	stTime.wSecond = static_cast<WORD>(nSecond);
	stTime.wDayOfWeek = 0;
	stTime.wMilliseconds = 0;

	*this = stTime;
}

KTime::KTime(LPCTSTR szDate)
{
	*this = szDate;
}

KTime::~KTime()
{
}

void KTime::operator=( __time64_t time64 )
{
	SetTime(time64);
}

void KTime::operator=( __time32_t time32 )
{
	SetTime(time32);
}

void KTime::operator=( const SYSTEMTIME &st )
{
	SetTime(SystemTimeToTime64(st));
}

void KTime::operator=(const KTime &time)
{
	SetTime(time.m_nCurTime);
}

void KTime::operator=(LPCTSTR szDate)
{
	CString strYear;
	CString strMonth;
	CString strDay;
	CString strHour;
	CString strMinute;
	CString strSecond;
	CString strDate(szDate);

	if (strDate.Find(_T("年")) > 0)
	{
		int nBeforePos = 0;
		int nAfterPos = 0;

		nAfterPos = strDate.Find(_T("年"));
		if (nAfterPos != -1)
		{
			strYear = strDate.Mid(nBeforePos, nAfterPos - nBeforePos);
			strYear.Trim();
		}

		nBeforePos = nAfterPos + 1;
		nAfterPos = strDate.Find(_T("月"));
		if (nAfterPos != -1)
		{
			strMonth = strDate.Mid(nBeforePos, nAfterPos - nBeforePos);
			strMonth.Trim();
		}

		nBeforePos = nAfterPos + 1;
		nAfterPos = strDate.Find(_T("日"));
		if (nAfterPos != -1)
		{
			strDay = strDate.Mid(nBeforePos, nAfterPos - nBeforePos);
			strDay.Trim();
		}

		nBeforePos = nAfterPos + 1;
		nAfterPos = strDate.Find(_T("时"));
		if (nAfterPos != -1)
		{
			strHour = strDate.Mid(nBeforePos, nAfterPos - nBeforePos);
			strHour.Trim();
		}

		nBeforePos = nAfterPos + 1;
		nAfterPos = strDate.Find(_T("分"));
		if (nAfterPos != -1)
		{
			strMinute = strDate.Mid(nBeforePos, nAfterPos - nBeforePos);
			strMinute.Trim();
		}

		nBeforePos = nAfterPos + 1;
		nAfterPos = strDate.Find(_T("秒"));
		if (nAfterPos != -1)
		{
			strSecond = strDate.Mid(nBeforePos, nAfterPos - nBeforePos);
			strSecond.Trim();
		}

		SYSTEMTIME st = {0};
		st.wYear = static_cast<WORD>(_tstol(strYear));
		st.wMonth = static_cast<WORD>(_tstol(strMonth));
		st.wDay = static_cast<WORD>(_tstol(strDay));
		st.wHour = static_cast<WORD>(_tstol(strHour));
		st.wMinute = static_cast<WORD>(_tstol(strMinute));
		st.wSecond = static_cast<WORD>(_tstol(strSecond));

		if (st.wYear != 0 && st.wMonth != 0 && st.wDay != 0)
		{
			*this = st;
		}
		else
		{
			*this = GetDefaultTime();
		}
	}
	else if (strDate.Find(_T("-")) > 0)
	{
		int nBeforePos = 0;
		int nAfterPos = 0;

		CString strYearMonDay;
		CString strHourMinSec;

		int nPos = strDate.Find(_T(" "));
		if (nPos != -1)
		{
			strYearMonDay = strDate.Left(nPos);
			strHourMinSec = strDate.Mid(nPos + 1);
		}
		else
		{
			strYearMonDay = strDate;
		}

		nAfterPos = strYearMonDay.Find(_T("-"), nBeforePos);
		if (nAfterPos != -1)
		{
			strYear = strYearMonDay.Mid(nBeforePos, nAfterPos - nBeforePos);
			nBeforePos = nAfterPos + 1;
		}

		nAfterPos = strYearMonDay.Find(_T("-"), nBeforePos);
		if (nAfterPos != -1)
		{
			strMonth = strYearMonDay.Mid(nBeforePos, nAfterPos - nBeforePos);
			nBeforePos = nAfterPos + 1;
		}

		if (nAfterPos != -1)
			strDay = strYearMonDay.Mid(nAfterPos + 1);

		nBeforePos = 0;
		nAfterPos = strHourMinSec.Find(_T(":"), nBeforePos);
		if (nAfterPos != -1)
		{
			strHour = strHourMinSec.Mid(nBeforePos, nAfterPos - nBeforePos);
			nBeforePos = nAfterPos + 1;
		}

		nAfterPos = strHourMinSec.Find(_T(":"), nBeforePos);
		if (nAfterPos != -1)
		{
			strMinute = strHourMinSec.Mid(nBeforePos, nAfterPos - nBeforePos);
			nBeforePos = nAfterPos + 1;
		}

		if (nAfterPos != -1)
			strSecond = strHourMinSec.Mid(nAfterPos + 1);

		strYear.Trim();
		strMonth.Trim();
		strDay.Trim();
		strHour.Trim();
		strMinute.Trim();
		strSecond.Trim();

		SYSTEMTIME st = {0};
		st.wYear = static_cast<WORD>(_tstol(strYear));
		st.wMonth = static_cast<WORD>(_tstol(strMonth));
		st.wDay = static_cast<WORD>(_tstol(strDay));
		st.wHour = static_cast<WORD>(_tstol(strHour));
		st.wMinute = static_cast<WORD>(_tstol(strMinute));
		st.wSecond = static_cast<WORD>(_tstol(strSecond));

		if (st.wYear != 0 && st.wMonth != 0 && st.wDay != 0)
		{
			*this = st;
		}
		else
		{
			*this = GetDefaultTime();
		}
	}
}

void KTime::operator+=( const KTimeSpan &timeSpan )
{
	m_nCurTime += timeSpan.GetSeconds();
}

void KTime::operator-=( const KTimeSpan &timeSpan )
{
	m_nCurTime -= timeSpan.GetSeconds();
}

KTimeSpan KTime::operator-( const KTime &time2 ) const
{
	return m_nCurTime - time2.m_nCurTime;
}

BOOL KTime::operator==(const KTime &time2) const
{
	return (time2.m_nCurTime == m_nCurTime);
}

unsigned int KTime::GetYear() const
{
	SYSTEMTIME stTime = Time64ToSystemTime(m_nCurTime);
	return stTime.wYear;
}

unsigned int KTime::GetMonth() const
{
	SYSTEMTIME stTime = Time64ToSystemTime(m_nCurTime);
	return stTime.wMonth;
}

unsigned int KTime::GetDay() const
{
	SYSTEMTIME stTime = Time64ToSystemTime(m_nCurTime);
	return stTime.wDay;
}

unsigned int KTime::GetHour() const
{
	SYSTEMTIME stTime = Time64ToSystemTime(m_nCurTime);
	return stTime.wHour;
}

unsigned int KTime::GetMin() const
{
	SYSTEMTIME stTime = Time64ToSystemTime(m_nCurTime);
	return stTime.wMinute;
}

unsigned int KTime::GetSecond() const
{
	SYSTEMTIME stTime = Time64ToSystemTime(m_nCurTime);
	return stTime.wSecond;
}

unsigned int KTime::GetDayOfWeek() const
{
	SYSTEMTIME stTime = Time64ToSystemTime(m_nCurTime);
	return stTime.wDayOfWeek;
}

unsigned int KTime::GetDate() const
{
	SYSTEMTIME stTime = Time64ToSystemTime(m_nCurTime);
	return stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
}

__time64_t KTime::GetTime64() const
{
	return m_nCurTime;
}

__time32_t KTime::GetTime32() const
{
	return (__time32_t)m_nCurTime;
}

void KTime::SetTime(__time64_t time64)
{
	if (IsValidTime(time64))
		m_nCurTime = time64;
	else
		m_nCurTime = GetDefaultTime64();
}

KTime KTime::GetCurrentTime()
{
	return _time64(NULL);
}

__time64_t KTime::SystemTimeToTime64( const SYSTEMTIME &stTime )
{
	tm tmTime = {stTime.wSecond, stTime.wMinute, stTime.wHour, stTime.wDay, stTime.wMonth-1, stTime.wYear-1900, stTime.wDayOfWeek, 0, 0};
	if (tmTime.tm_sec < 0 || tmTime.tm_sec > 59)
		tmTime.tm_sec = 0;

	if (tmTime.tm_min < 0 || tmTime.tm_min > 59)
		tmTime.tm_min = 0;

	if (tmTime.tm_hour < 0 || tmTime.tm_hour > 23)
		tmTime.tm_hour = 0;

	if (tmTime.tm_mday < 1 || tmTime.tm_mday > 31)
		tmTime.tm_mday = 1;

	if (tmTime.tm_mon < 0 || tmTime.tm_mon > 11)
		tmTime.tm_mon = 0;

	if (tmTime.tm_year < 0 || tmTime.tm_year >= 1100)
		tmTime.tm_year = (2000 - 1900);

	__time64_t nTime = _mktime64(&tmTime);
	return nTime;
}

SYSTEMTIME KTime::Time64ToSystemTime(const __time64_t &time64)
{
	tm tmTime = {0};
	SYSTEMTIME stTime = {0};

	if (IsValidTime(time64))
	{
		_localtime64_s(&tmTime, &time64);

		stTime.wYear = static_cast<WORD>(tmTime.tm_year + 1900);
		stTime.wMonth = static_cast<WORD>(tmTime.tm_mon + 1);
		stTime.wDay = static_cast<WORD>(tmTime.tm_mday);
		stTime.wHour = static_cast<WORD>(tmTime.tm_hour);
		stTime.wMinute = static_cast<WORD>(tmTime.tm_min);
		stTime.wSecond = static_cast<WORD>(tmTime.tm_sec);
		stTime.wDayOfWeek = static_cast<WORD>(tmTime.tm_wday);
		stTime.wMilliseconds = 0;
	}
	else
	{
		stTime = GetDefaultTime();
	}
	
	return stTime;
}

BOOL KTime::IsValidTime(const __time64_t &time64)
{
	return (time64 < defTimeMax64_t_ && time64 >= 0);
}

SYSTEMTIME KTime::GetDefaultTime()
{
	SYSTEMTIME stTime = {0};

	stTime.wYear = 2000;
	stTime.wMonth = 1;
	stTime.wDay = 1;
	stTime.wHour = 0;
	stTime.wMinute = 0;
	stTime.wSecond = 0;
	stTime.wDayOfWeek = 0;
	stTime.wMilliseconds = 0;

	return stTime;
}

__time64_t KTime::GetDefaultTime64()
{
	SYSTEMTIME stTime = GetDefaultTime();
	return SystemTimeToTime64(stTime);
}

SYSTEMTIME KTime::GetSystemTime()
{
	return Time64ToSystemTime(m_nCurTime);
}

BOOL KTime::IsSameDay(const KTime &time2) const
{
	return GetYear() == time2.GetYear() && 
		GetMonth() == time2.GetMonth() && 
		GetDay() == time2.GetDay(); 
}

BOOL KTime::IsMoreThanInputTimeSpecNatureDay(const KTime &time, int nOffsetDay) const
{
	KTime timeAdd = time;
	timeAdd += KTimeSpan(nOffsetDay, 0, 0, 0);

	return IsSameDay(timeAdd);
}

BOOL KTime::IsMoreThanInputTimeAtLeastSpecNatureDay(const KTime &time, int nOffsetDay) const
{
	KTime timeAdd = time;
	timeAdd += KTimeSpan(nOffsetDay, 0, 0, 0);

	return ((*this) > timeAdd || IsSameDay(timeAdd));
}

BOOL KTime::operator<( const KTime &time2 ) const
{
	return m_nCurTime < time2.m_nCurTime; 
}

BOOL KTime::operator>( const KTime &time2 ) const
{
	return m_nCurTime > time2.m_nCurTime; 
}

BOOL KTime::operator<=( const KTime &time2 ) const
{
	return m_nCurTime <= time2.m_nCurTime; 
}

BOOL KTime::operator>=( const KTime &time2 ) const
{
	return m_nCurTime >= time2.m_nCurTime; 
}
