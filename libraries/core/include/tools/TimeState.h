#pragma once
#include <cstdint>

class TimeState
{
public:
	static TimeState Seconds(int64_t seconds);
	static TimeState Milliseconds(int64_t ms);

	TimeState();

	bool IsZero() const;
	int64_t Sec() const;
	int64_t Ms() const;

	TimeState& operator-=(const TimeState& other);
	TimeState& operator+=(const TimeState& other);

private:
	TimeState(int64_t ms);

	int64_t		_ms;
};

inline bool operator<(TimeState lhs, TimeState rhs)
{
	return lhs.Ms() < rhs.Ms();
}

inline bool operator>(TimeState lhs, TimeState rhs)
{
	return lhs.Ms() > rhs.Ms();
}

inline bool operator==(TimeState lhs, TimeState rhs)
{
	return lhs.Ms() == rhs.Ms();
}

inline bool operator!=(TimeState lhs, TimeState rhs)
{
	return !(lhs.Ms() == rhs.Ms());
}

inline bool operator<=(TimeState lhs, TimeState rhs)
{
	return !(lhs.Ms() > rhs.Ms());
}

inline bool operator>=(TimeState lhs, TimeState rhs)
{
	return !(lhs.Ms() < rhs.Ms());
}

inline TimeState operator-(TimeState lhs, TimeState rhs)
{
	return TimeState::Milliseconds(lhs.Ms() - rhs.Ms());
}

inline TimeState operator+(TimeState lhs, TimeState rhs)
{
	return TimeState::Milliseconds(lhs.Ms() + rhs.Ms());
}
