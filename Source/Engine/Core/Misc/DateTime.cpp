// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DateTime.h"

#include "Core/Platform/PlatformTime.h"
#include <EAStdC/EAString.h>
#include <date/tz.h>

#include "Char.h"

using namespace EA::StdC;


/* FDateTime constants
 *****************************************************************************/

const int32 DateTime::DaysPerMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const int32 DateTime::DaysToMonth[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };


/* FDateTime structors
 *****************************************************************************/

DateTime::DateTime(int32 Year, int32 Month, int32 Day, int32 Hour, int32 Minute, int32 Second, int32 Millisecond)
{
	if (!Validate(Year, Month, Day, Hour, Minute, Second, Millisecond))
	{
		// LOG Invalid date-time
	}

	int32 TotalDays = 0;

	if ((Month > 2) && IsLeapYear(Year))
	{
		++TotalDays;
	}

	--Year;											// the current year is not a full year yet
	--Month;										// the current month is not a full month yet

	TotalDays += Year * 365;
	TotalDays += Year / 4;							// leap year day every four years...
	TotalDays -= Year / 100;						// ...except every 100 years...
	TotalDays += Year / 400;						// ...but also every 400 years
	TotalDays += DaysToMonth[Month];				// days in this year up to last month
	TotalDays += Day - 1;							// days in this month minus today

	Ticks = TotalDays * ETimespan::TicksPerDay
		+ Hour * ETimespan::TicksPerHour
		+ Minute * ETimespan::TicksPerMinute
		+ Second * ETimespan::TicksPerSecond
		+ Millisecond * ETimespan::TicksPerMillisecond;
}


/* FDateTime interface
 *****************************************************************************/


year_month_day DateTime::GetDateComponents() const
{
	return { floor<days>(time) };
}


uint32 DateTime::GetDay() const
{
	return (uint32)GetDateComponents().day();
}


EDayOfWeek DateTime::GetDayOfWeek() const
{
	const weekday wd { floor<days>(time) };

	// January 1, 0001 was a Monday
	return static_cast<EDayOfWeek>((wd - Sunday).count());
}


uint32 DateTime::GetDayOfYear() const
{
	auto timeDays = std::chrono::floor<days>(time);
	const year_month_day ymd { timeDays };

	return (timeDays.time_since_epoch() - local_days{ ymd.year() / jan / 1 }.time_since_epoch()).count();
}


uint32 DateTime::GetHour12() const
{
	uint32 Hour = GetHour();

	if (Hour < 1)
		return 12;

	if (Hour > 12)
		return (Hour - 12);

	return Hour;
}


uint32 DateTime::GetMonth() const
{
	return (uint32)year_month_day{ floor<days>(time) }.month();
}

int32 DateTime::GetYear() const
{
	return (int32)GetDateComponents().year();
}

String DateTime::ToHttpDate() const
{
	String DayStr;
	String MonthStr;

	switch (GetDayOfWeek())
	{
		case EDayOfWeek::Monday:	DayStr = TX("Mon");	break;
		case EDayOfWeek::Tuesday:	DayStr = TX("Tue");	break;
		case EDayOfWeek::Wednesday:	DayStr = TX("Wed");	break;
		case EDayOfWeek::Thursday:	DayStr = TX("Thu");	break;
		case EDayOfWeek::Friday:	DayStr = TX("Fri");	break;
		case EDayOfWeek::Saturday:	DayStr = TX("Sat");	break;
		case EDayOfWeek::Sunday:	DayStr = TX("Sun");	break;
	}

	switch (GetMonthOfYear())
	{
		case EMonthOfYear::January:		MonthStr = TX("Jan");	break;
		case EMonthOfYear::February:	MonthStr = TX("Feb");	break;
		case EMonthOfYear::March:		MonthStr = TX("Mar");	break;
		case EMonthOfYear::April:		MonthStr = TX("Apr");	break;
		case EMonthOfYear::May:			MonthStr = TX("May");	break;
		case EMonthOfYear::June:		MonthStr = TX("Jun");	break;
		case EMonthOfYear::July:		MonthStr = TX("Jul");	break;
		case EMonthOfYear::August:		MonthStr = TX("Aug");	break;
		case EMonthOfYear::September:	MonthStr = TX("Sep");	break;
		case EMonthOfYear::October:		MonthStr = TX("Oct");	break;
		case EMonthOfYear::November:	MonthStr = TX("Nov");	break;
		case EMonthOfYear::December:	MonthStr = TX("Dec");	break;
	}

	String time;
	time.sprintf(TX("%02i:%02i:%02i"), GetHour(), GetMinute(), GetSecond());

	String dateTime;
	dateTime.sprintf(TX("%s, %02d %s %d %s GMT"), DayStr.c_str(), GetDay(), MonthStr.c_str(), GetYear(), time.c_str());
	return dateTime;
}


String DateTime::ToIso8601() const
{
	return ToString(TX("%Y-%m-%dT%H:%M:%S.%sZ"));
}


String DateTime::ToString() const
{
	return ToString(TX("%Y.%m.%d-%H.%M.%S"));
}


String DateTime::ToString(const TCHAR* Format) const
{
	String Result;

	if (Format != nullptr)
	{
		while (*Format != TX('\0'))
		{
			if ((*Format == TX('%')) && (*(++Format) != TX('\0')))
			{
				switch (*Format)
				{
				case TX('a'): Result += IsMorning() ? TX("am") : TX("pm"); break;
				case TX('A'): Result += IsMorning() ? TX("AM") : TX("PM"); break;
				case TX('d'): Result.append_sprintf(TX("%02i"), GetDay()); break;
				case TX('D'): Result.append_sprintf(TX("%03i"), GetDayOfYear()); break;
				case TX('m'): Result.append_sprintf(TX("%02i"), GetMonth()); break;
				case TX('y'): Result.append_sprintf(TX("%02i"), GetYear() % 100); break;
				case TX('Y'): Result.append_sprintf(TX("%04i"), GetYear()); break;
				case TX('h'): Result.append_sprintf(TX("%02i"), GetHour12()); break;
				case TX('H'): Result.append_sprintf(TX("%02i"), GetHour()); break;
				case TX('M'): Result.append_sprintf(TX("%02i"), GetMinute()); break;
				case TX('S'): Result.append_sprintf(TX("%02i"), GetSecond()); break;
				case TX('s'): Result.append_sprintf(TX("%03i"), GetMillisecond()); break;
				default:		 Result += *Format;
				}
			}
			else
			{
				Result += *Format;
			}

			// move to the next one
			Format++;
		}
	}

	return Result;
}


/* FDateTime static interface
 *****************************************************************************/

int32 DateTime::DaysInMonth(int32 Year, int32 Month)
{
	assert((Month >= 1) && (Month <= 12));

	if ((Month == 2) && IsLeapYear(Year))
	{
		return 29;
	}

	return DaysPerMonth[Month];
}


int32 DateTime::DaysInYear(int32 Year)
{
	if (IsLeapYear(Year))
	{
		return 366;
	}

	return 365;
}


bool DateTime::IsLeapYear(int32 Year)
{
	if ((Year % 4) == 0)
	{
		return (((Year % 100) != 0) || ((Year % 400) == 0));
	}

	return false;
}


DateTime DateTime::Now()
{
	return DateTime(floor<std::chrono::milliseconds>(SysClock::now()));
}


bool DateTime::Parse(const String& DateTimeString, DateTime& OutDateTime)
{
	// first replace -, : and . with space
	String FixedString = CString::ReplaceCopy(DateTimeString, '-', ' ');
	CString::Replace(FixedString, ':', ' ');
	CString::Replace(FixedString, '.', ' ');

	// split up on a single delimiter
	TArray<String> Tokens;
	CString::Split(FixedString, Tokens, TX(' '));

	// make sure it parsed it properly (within reason)
	if ((Tokens.Size() < 6) || (Tokens.Size() > 7))
	{
		return false;
	}

	const int32 Year  = AtoI32(Tokens[0].c_str());
	const int32 Month = AtoI32(Tokens[1].c_str());
	const int32 Day   = AtoI32(Tokens[2].c_str());
	const int32 Hour  = AtoI32(Tokens[3].c_str());
	const int32 Minute = AtoI32(Tokens[4].c_str());
	const int32 Second = AtoI32(Tokens[5].c_str());
	const int32 Millisecond = Tokens.Size() > 6 ? AtoI32(Tokens[6].c_str()) : 0;

	if (!Validate(Year, Month, Day, Hour, Minute, Second, Millisecond))
	{
		return false;
	}

	// convert the tokens to numbers
	OutDateTime.Ticks = DateTime(Year, Month, Day, Hour, Minute, Second, Millisecond).Ticks;

	return true;
}


bool DateTime::ParseHttpDate(const String& HttpDate, DateTime& OutDateTime)
{
	auto ParseTime = [](const String& Time, int32& Hour, int32& Minute, int32& Second) -> bool
	{
		// 2DIGIT ":" 2DIGIT ":" 2DIGIT
		// ; 00:00 : 00 - 23 : 59 : 59
		TArray<String> Tokens;

		// split up on a single delimiter
		int32 NumTokens = CString::Split(Time, Tokens, TX(':'));

		if (NumTokens == 3)
		{
			Hour   = AtoI32(Tokens[0].c_str());
			Minute = AtoI32(Tokens[1].c_str());
			Second = AtoI32(Tokens[2].c_str());

			return (Hour >= 0 && Hour < 24) && (Minute >= 0 && Minute <= 59) && (Second >= 0 && Second <= 59);
		}

		return false;
	};

	auto ParseWkday = [](const String& WkDay) -> int32
	{
		const int32 NumChars = WkDay.size();

		if (NumChars == 3)
		{
			if (WkDay == TX("Mon"))
			{
				return 1;
			}
			else if (WkDay == TX("Tue"))
			{
				return 2;
			}
			else if (WkDay == TX("Wed"))
			{
				return 3;
			}
			else if (WkDay == TX("Thu"))
			{
				return 4;
			}
			else if (WkDay == TX("Fri"))
			{
				return 5;
			}
			else if (WkDay == TX("Sat"))
			{
				return 6;
			}
			else if (WkDay == TX("Sun"))
			{
				return 7;
			}
		}

		return -1;
	};

	auto ParseWeekday = [](const String& WeekDay) -> int32
	{
		const int32 NumChars = WeekDay.size();

		if (NumChars >= 6 && NumChars <= 9)
		{
			if (WeekDay == TX("Monday"))
			{
				return 1;
			}
			else if (WeekDay == TX("Tueday"))
			{
				return 2;
			}
			else if (WeekDay == TX("Wednesday"))
			{
				return 3;
			}
			else if (WeekDay == TX("Thursday"))
			{
				return 4;
			}
			else if (WeekDay == TX("Friday"))
			{
				return 5;
			}
			else if (WeekDay == TX("Saturday"))
			{
				return 6;
			}
			else if (WeekDay == TX("Sunday"))
			{
				return 7;
			}
		}

		return -1;
	};

	auto ParseMonth = [](const String& Month) -> int32
	{
		const int32 NumChars = Month.size();

		if (NumChars == 3)
		{
			if (Month == TX("Jan"))
			{
				return 1;
			}
			else if (Month == TX("Feb"))
			{
				return 2;
			}
			else if (Month == TX("Mar"))
			{
				return 3;
			}
			else if (Month == TX("Apr"))
			{
				return 4;
			}
			else if (Month == TX("May"))
			{
				return 5;
			}
			else if (Month == TX("Jun"))
			{
				return 6;
			}
			else if (Month == TX("Jul"))
			{
				return 7;
			}
			else if (Month == TX("Aug"))
			{
				return 8;
			}
			else if (Month == TX("Sep"))
			{
				return 9;
			}
			else if (Month == TX("Oct"))
			{
				return 10;
			}
			else if (Month == TX("Nov"))
			{
				return 11;
			}
			else if (Month == TX("Dec"))
			{
				return 12;
			}
		}

		return -1;
	};

	auto ParseDate1 = [ParseMonth](const String& DayStr, const String& MonStr, const String& YearStr, int32& Month, int32& Day, int32& Year) -> bool
	{
		// date1 = 2DIGIT SP month SP 4DIGIT
		// ; day month year(e.g., 02 Jun 1982)

		Day = AtoI32(DayStr.c_str());
		Month = ParseMonth(MonStr);
		Year = (YearStr.size() == 4) ? AtoI32(YearStr.c_str()) : -1;

		return (Day > 0 && Day <= 31) && (Month > 0 && Month <= 12) && (Year > 0 && Year <= 9999);
	};

	auto ParseDate2 = [ParseMonth](const String& Date2, int32& Month, int32& Day, int32& Year) -> bool
	{
		// date2 = 2DIGIT "-" month "-" 2DIGIT
		// ; day - month - year(e.g., 02 - Jun - 82)
		TArray<String> Tokens;

		// split up on a single delimiter
		int32 NumTokens = CString::Split(Date2, Tokens, TX('-'));
		if (NumTokens == 3)
		{
			Day = AtoI32(Tokens[0].c_str());
			Month = ParseMonth(Tokens[1]);
			Year = AtoI32(Tokens[2].c_str());
			
			// Horrible assumption here, but this is a deprecated part of the spec
			Year += 1900;
		}

		return (Day > 0 && Day <= 31) && (Month > 0 && Month <= 12) && (Year > 0 && Year <= 9999);
	};

	auto ParseDate3 = [ParseMonth](const String& MonStr, const String& DayStr, int32& Month, int32& Day) -> bool
	{
		// date3 = month SP(2DIGIT | (SP 1DIGIT))
		// ; month day(e.g., Jun  2)
		const int32 NumDigits = DayStr.size();
		Day = (NumDigits > 0 && NumDigits <= 2) ? AtoI32(DayStr.c_str()) : -1;
		Month = ParseMonth(MonStr);

		return (Day > 0 && Day <= 31) && (Month > 0 && Month <= 12);
	};

	if (!HttpDate.empty())
	{
		TArray<String> Tokens;

		// split up on a single delimiter
		int32 NumTokens = CString::Split(HttpDate, Tokens, TX(' '));

		if (NumTokens > 0 && Tokens.Size() == NumTokens)
		{
			int32 Month = 0;
			int32 Day = 0;
			int32 Year = 0;
			int32 Hour = 0;
			int32 Minute = 0;
			int32 Second = 0;
			int32 Millisecond = 0;

			if (CString::EndsWith(Tokens[0], TX(',')))
			{
				Tokens[0].erase(Tokens[0].size() - 1, 1);
			}

			if (Tokens[Tokens.Size() - 1] == TX("GMT"))
			{
				// rfc1123 - date | rfc850 - date 
				if (Tokens.Size() == 6)
				{
					int32 WkDay = ParseWkday(Tokens[0]);

					if (WkDay > 0)
					{
						// rfc1123 - date = wkday "," SP date1 SP time SP "GMT"
						if (ParseDate1(Tokens[1], Tokens[2], Tokens[3], Month, Day, Year))
						{
							ParseTime(Tokens[4], Hour, Minute, Second);
						}
					}
				}
				else if (Tokens.Size() == 4)
				{
					// rfc850 - date = weekday "," SP date2 SP time SP "GMT"
					int32 WeekDay = ParseWeekday(Tokens[0]);

					if (WeekDay > 0)
					{
						if (ParseDate2(Tokens[1], Month, Day, Year))
						{
							ParseTime(Tokens[2], Hour, Minute, Second);
						}
					}
				}

			}
			else if (Tokens.Size() == 5)
			{
				// asctime - date = wkday SP date3 SP time SP 4DIGIT
				int32 WkDay = ParseWkday(Tokens[0]);

				if (WkDay > 0)
				{
					if (ParseDate3(Tokens[1], Tokens[2], Month, Day))
					{
						if (ParseTime(Tokens[3], Hour, Minute, Second))
						{
							if (Tokens[4].size() == 4)
							{
								Year = AtoI32(Tokens[4].c_str());
							}
						}
					}
				}
			}

			if (Validate(Year, Month, Day, Hour, Minute, Second, Millisecond))
			{
				// convert the tokens to numbers
				OutDateTime = DateTime(Year, Month, Day, Hour, Minute, Second, Millisecond);

				return true;
			}
		}
	}

	return false;
}


bool DateTime::ParseIso8601(const TCHAR* DateTimeString, DateTime& OutDateTime)
{
	// DateOnly: YYYY-MM-DD
	// DateTime: YYYY-mm-ddTHH:MM:SS(.ssss)(Z|+th:tm|-th:tm)

	const TCHAR* ptr = DateTimeString;
	TCHAR* Next = nullptr;

	int32 Year = 0, Month = 0, Day = 0;
	int32 Hour = 0, Minute = 0, Second = 0, Millisecond = 0;
	int32 TzHour = 0, TzMinute = 0;

	// get date
	Year = FChar::StrtoI32(ptr, &Next, 10);

	if ((Next <= ptr) || (*Next == TX('\0')))
	{
		return false;
	}

	ptr = Next + 1; // skip separator
	Month = FChar::StrtoI32(ptr, &Next, 10);

	if ((Next <= ptr) || (*Next == TX('\0')))
	{
		return false;
	}

	ptr = Next + 1; // skip separator
	Day = FChar::StrtoI32(ptr, &Next, 10);

	if (Next <= ptr)
	{
		return false;
	}

	// check whether this is date and time
	if (*Next == TX('T'))
	{
		ptr = Next + 1;

		// parse time
		Hour = FChar::StrtoI32(ptr, &Next, 10);

		if ((Next <= ptr) || (*Next == TX('\0')))
		{
			return false;
		}

		ptr = Next + 1; // skip separator
		Minute = FChar::StrtoI32(ptr, &Next, 10);

		if ((Next <= ptr) || (*Next == TX('\0')))
		{
			return false;
		}

		ptr = Next + 1; // skip separator
		Second = FChar::StrtoI32(ptr, &Next, 10);

		if (Next <= ptr)
		{
			return false;
		}

		// check for milliseconds
		if (*Next == TX('.'))
		{
			ptr = Next + 1;
			Millisecond = FChar::StrtoI32(ptr, &Next, 10);

			// should be no more than 3 digits
			if ((Next <= ptr) || (Next > ptr + 3))
			{
				return false;
			}

			for (int32 Digits = Next - ptr; Digits < 3; ++Digits)
			{
				Millisecond *= 10;
			}
		}

		// see if the timezone offset is included
		if (*Next == TX('+') || *Next == TX('-'))
		{
			// include the separator since it's + or -
			ptr = Next;

			// parse the timezone offset
			TzHour = FChar::StrtoI32(ptr, &Next, 10);

			if ((Next <= ptr) || (*Next == TX('\0')))
			{
				return false;
			}

			ptr = Next + 1; // skip separator
			TzMinute = FChar::StrtoI32(ptr, &Next, 10);
			
			if (Next <= ptr)
			{
				return false;
			}
		}
		else if ((*Next != TX('\0')) && (*Next != TX('Z')))
		{
			return false;
		}
	}
	else if (*Next != TX('\0'))
	{
		return false;
	}

	if (!Validate(Year, Month, Day, Hour, Minute, Second, Millisecond))
	{
		return false;
	}

	DateTime Final(Year, Month, Day, Hour, Minute, Second, Millisecond);

	// adjust for the timezone (bringing the DateTime into UTC)
	int32 TzOffsetMinutes = (TzHour < 0) ? TzHour * 60 - TzMinute : TzHour * 60 + TzMinute;
	Final -= Timespan::FromMinutes(TzOffsetMinutes);
	OutDateTime = Final;

	return true;
}


DateTime DateTime::UtcNow()
{
	int32 Year, Month, Day, DayOfWeek;
	int32 Hour, Minute, Second, Millisecond;

	PlatformTime::UtcTime(Year, Month, DayOfWeek, Day, Hour, Minute, Second, Millisecond);

	return DateTime(Year, Month, Day, Hour, Minute, Second, Millisecond);
}


bool DateTime::Validate(int32 Year, int32 Month, int32 Day, int32 Hour, int32 Minute, int32 Second, int32 Millisecond)
{
	return (Year >= 1) && (Year <= 9999) &&
		(Month >= 1) && (Month <= 12) &&
		(Day >= 1) && (Day <= DaysInMonth(Year, Month)) &&
		(Hour >= 0) && (Hour <= 23) &&
		(Minute >= 0) && (Minute <= 59) &&
		(Second >= 0) && (Second <= 59) &&
		(Millisecond >= 0) && (Millisecond <= 999);
}

