#ifndef _KTime_H_
#define _KTime_H_
#include <Windows.h>
#include <time.h>

class KTimeSpan
{
public:
	KTimeSpan(__time64_t nTimeSpan = 0);
	KTimeSpan(int nDay, int nHour, int nMin, int nSecond);
	virtual ~KTimeSpan();

    int GetDays() const;
    int GetHours() const;
    int GetMinuti() const;
    int GetSeconds() const;

private:
	__time64_t m_nCurTimeSpan;
};

class KTime
{
public:
	KTime();
	KTime(__time64_t time64);
	KTime(__time32_t time32);
	KTime(unsigned int nYear, unsigned int nMonth, unsigned int nDay, unsigned int nHour, unsigned int nMin, unsigned int nSecond);
	explicit KTime(LPCTSTR szDate);
	virtual ~KTime();
	
	void operator=(__time64_t time64);
	void operator=(__time32_t time32);
	void operator=(const SYSTEMTIME &st);
	void operator=(const KTime &time);
	void operator=(LPCTSTR szDate);	//2013年8月22日 5时3分2秒 或者 2013-09-04 12:22:21

	void operator+=(const KTimeSpan &timeSpan);
	void operator-=(const KTimeSpan &timeSpan);
	KTimeSpan operator-(const KTime &time2) const;
	BOOL operator==(const KTime &time2) const;
	BOOL operator<(const KTime &time2) const; 
	BOOL operator>(const KTime &time2) const; 
	BOOL operator<=(const KTime &time2) const; 
	BOOL operator>=(const KTime &time2) const; 

    unsigned int GetYear() const;
    unsigned int GetMonth() const;
    unsigned int GetDay() const;
    unsigned int GetHour() const;
    unsigned int GetMin() const;
    unsigned int GetSecond() const;
    unsigned int GetDayOfWeek() const;
	unsigned int GetDate() const;
    BOOL IsSameDay(const KTime &time2) const; 
	BOOL IsMoreThanInputTimeSpecNatureDay(const KTime &time, int nOffsetDay) const;
	BOOL IsMoreThanInputTimeAtLeastSpecNatureDay(const KTime &time, int nOffsetDay) const;

	SYSTEMTIME GetSystemTime();

    __time64_t GetTime64() const;
    __time32_t GetTime32() const;

protected:
	void SetTime(__time64_t time64);

public:
	static KTime GetCurrentTime();
	static __time64_t SystemTimeToTime64(const SYSTEMTIME &st);
    static SYSTEMTIME Time64ToSystemTime(const __time64_t &time64);
	static BOOL IsValidTime(const __time64_t &time64);

private:
	static SYSTEMTIME GetDefaultTime();
	static __time64_t GetDefaultTime64();

private:
	__time64_t m_nCurTime;
};


#endif