#include "tools/TimeState.h"

TimeState TimeState::Seconds(int64_t seconds)
{
	return Milliseconds(seconds * 1000);
}

TimeState TimeState::Milliseconds(int64_t ms)
{
	return TimeState(ms);
}

TimeState::TimeState()
	: TimeState(0)
{}

bool TimeState::IsZero() const
{
	return _ms == 0;
}

int64_t TimeState::Sec() const
{
	return _ms / 1000;
}

int64_t TimeState::Ms() const
{
	return _ms;
}

TimeState& TimeState::operator-=(const TimeState& other)
{
	_ms -= other.Ms();
	return *this;
}

TimeState& TimeState::operator+=(const TimeState& other)
{
	_ms += other.Ms();
	return *this;
}

TimeState::TimeState(int64_t ms)
	: _ms(ms)
{
}
