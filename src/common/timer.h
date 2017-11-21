#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>
#include <string>
#include <iostream>

namespace internalTimer
{
	using namespace std::chrono;
	inline void sleep(unsigned t = 5) { std::this_thread::sleep_for(milliseconds(t)); }
	class Timer
	{
		using clock = high_resolution_clock;

	public:
		Timer() : begin_time_(clock::now()) {}
		static Timer now() { return Timer(); }

        int ToDAY() const
        {
            return ToSEC() / 86400LL;
        }

		long long ToSEC() const
		{
			return duration_cast<seconds>(begin_time_.time_since_epoch()).count();
		}
		long long ToMILLI() const
		{
			return duration_cast<milliseconds>(begin_time_.time_since_epoch()).count();
		}
		long long ToMICRO() const
		{
			return duration_cast<microseconds>(begin_time_.time_since_epoch()).count();
		}
		long long ToNANO() const
		{
			return duration_cast<nanoseconds>(begin_time_.time_since_epoch()).count();
		}
		void fromSEC(long long m)
		{
			begin_time_ = time_point<clock>(seconds(m));
		}
		void fromMilli(long long m)
		{
			begin_time_ = time_point<clock>(milliseconds(m));
		}
		void fromMICRO(long long m)
		{
			begin_time_ = time_point<clock>(microseconds(m));
		}
		void fromNANO(long long m)
		{
			begin_time_ = time_point<clock>(nanoseconds(m));
		}
		void reset()
		{
			begin_time_ = clock::now();
		}
		long long elapsed() const
		{
			return duration_cast<milliseconds>(clock::now() - begin_time_).count();
		}
		long long elapsed_micro() const
		{
			return duration_cast<microseconds>(clock::now() - begin_time_).count();
		}
		long long elapsed_nano() const
		{
			return duration_cast<nanoseconds>(clock::now() - begin_time_).count();
		}
		long long elapsed_seconds() const
		{
			return duration_cast<seconds>(clock::now() - begin_time_).count();
		}
		long long elapsed_minutes() const
		{
			return duration_cast<minutes>(clock::now() - begin_time_).count();
		}
		long long elapsed_hours() const
		{
			return duration_cast<hours>(clock::now() - begin_time_).count();
		}

	private:
		time_point<clock> begin_time_;
	};
	inline std::ostream& operator << (std::ostream & o, const Timer & t)
	{
		o << t.ToMILLI();
		return o;
	}

	class scopeTimer
	{
	private:
		Timer timer_;
		std::string operation_;
		std::ostream& os_;

	public:
		scopeTimer(const std::string& operation = std::string(), std::ostream& os = std::cout) :
			timer_(), operation_(operation), os_(os)
		{}
		~scopeTimer() { os_ << operation_ << "\tuse time: " << timer_.elapsed() << "ms" << std::endl; }

	};
}
#endif // !TIMER_H


