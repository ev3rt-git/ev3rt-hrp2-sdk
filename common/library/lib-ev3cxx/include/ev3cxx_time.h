#pragma once

/**
* TOPPERS API
*/
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>

namespace ev3cxx {

namespace detail {

class us_counter_t {
public:
	typedef SYSUTM time_type;
	typedef ER error_type;

	us_counter_t()
		:m_last_value(0)
	{}

	time_type value() {
		m_error = get_utm(&m_last_value);
		return m_last_value;
	}

	time_type operator()() { return value(); }

	operator bool() const { return is_ok(); }

	bool is_ok() const { return m_error == E_OK; }
	
	error_type error() const { return m_error; }

	error_type clear() {
		error_type e = m_error;
		m_error = E_OK;
		return e;
	}
private:
	time_type m_last_value;
	error_type m_error;
}; // class us_counter_t

template <typename Timer>
void wait(Timer & timer, typename Timer::time_type time) {
	typename Timer::time_type base = timer.value();
	while (time > 0) {
		typename Timer::time_type new_base = timer.value();
		typename Timer::time_type difference = new_base - base;
		if (time < difference)
			break;
		time -= difference;
		base = new_base;
		tslp_tsk(0);
	}
}

template <typename Timer, typename Process>
void wait(Timer & timer, typename Timer::time_type time, Process process) {
	typename Timer::time_type base = timer.value();
	while (time > 0) {
		typename Timer::time_type new_base = timer.value();
		typename Timer::time_type difference = new_base - base;
		if (time < difference)
			break;
		time -= difference;
		base = new_base;
		process();
		tslp_tsk(0);
	}
}

template <typename Timer, typename Process>
typename Timer::time_type wait(Timer & timer, typename Timer::time_type time, Process process, int) {
	typename Timer::time_type base = timer.value();
	if (time == 0)
		for (;;) {
			if (process())
				return timer.value() - base;
			tslp_tsk(0);
		}
	typename Timer::time_type base_time = base;
	while (time > 0) {
		typename Timer::time_type new_base = timer.value();
		typename Timer::time_type difference = new_base - base;
		if (time < difference)
			return 0;
		time -= difference;
		base = new_base;
		if (process()) {
			typename Timer::time_type res = timer.value() - base_time;
			return res == 0 ? 1 : res;
		}
		tslp_tsk(0);
	}
	return 0;
}

template <typename Timer>
class stopwatch
{
public:
	typedef typename Timer::time_type time_type;

	stopwatch()
		: m_timer(0), m_running(false)
	{}

	explicit stopwatch(Timer & timer)
		: m_timer(&timer), m_running(true)
	{
		this->clear();
	}

	void init(Timer & timer) {
		m_timer = &timer;
		m_running = true;
		m_base = m_timer->value();
	}

	void init_stopped(Timer & timer) {
		m_timer = &timer;
		m_running = false;
		m_base = 0;
	}

	void clear() {
		if (m_running)
			m_base = m_timer->value();
		else
			m_base = 0;
	}

	bool running() const {
		return m_running;
	}

	void cancel() {
		m_running = false;
		m_base = 0;
	}

	void restart() {
		m_running = true;
		m_base = m_timer->value();
	}

	void start() {
		if (!m_running) {
			m_base = m_timer->value() - m_base;
			m_running = true;
		}
	}

	void stop() {
		if (m_running) {
			m_base = m_timer->value() - m_base;
			m_running = false;
		}
	}

	time_type operator()() const { return this->get(); }

	time_type get() const {
		if (m_running)
			return m_timer->value() - m_base;
		else
			return m_base;
	}

	void set(time_type value) {
		if (m_running)
			m_base = m_timer->value() - value;
		else
			m_base = value;
	}

	void decrease(time_type time) {
		if (m_running)
			m_base += time;
		else
			m_base -= time;
	}

private:
	Timer * m_timer;
	volatile bool m_running;
	volatile time_type m_base;
}; // class stopwatch

template <typename Timer>
class timeout
	: public stopwatch<Timer>
{
public:
	typedef stopwatch<Timer> base_type;
	typedef typename base_type::time_type time_type;

	timeout()
	{}

	timeout(Timer & timer, time_type timeout)
		: stopwatch<Timer>(timer), m_timeout(timeout)
	{}

	void init(Timer & timer, time_type timeout) {
		stopwatch<Timer>::init(timer);
		m_timeout = timeout;
	}

	void init_stopped(Timer & timer, time_type timeout) {
		stopwatch<Timer>::init_stopped(timer);
		m_timeout = timeout;
	}

	void force() {
		this->set(m_timeout + 1);
	}

	void ack() {
		this->decrease(m_timeout);
	}

	operator bool() const { return (*this)() > m_timeout; }

	void set_timeout(const time_type & value) {
		m_timeout = value;
	}

	time_type get_timeout() const { return m_timeout; }

	void reset(const time_type & new_timeout) {
		this->clear();
		m_timeout = new_timeout;
	}

	time_type remaining() const { return *this ? 0 : (m_timeout - (*this)()); }

private:
	volatile time_type m_timeout;
}; // class timeout

/**
 * Global object ev3cxx::detail::us_counter for internal time handling
 */
extern us_counter_t us_counter;

} // namespace detail

struct StopWatch
	:detail::stopwatch<detail::us_counter_t>
{
	StopWatch(const bool& run = true)
		:detail::stopwatch<detail::us_counter_t>(detail::us_counter)
	{
		if (run)
			return;
		stop();
		clear();
	}

	bool isRunning()
	{
		return running();
	}

	void reset(const bool& start = true)
	{
		if (start)
			restart();
		else
			cancel();
	}

	time_type getMs()
	{
		return get() / 1000;
	}

	time_type getUs()
	{
		return get();
	}
};

struct Timeout
	:detail::timeout<detail::us_counter_t>
{
	Timeout(detail::timeout<detail::us_counter_t>::time_type timeout)
		:detail::timeout<detail::us_counter_t>(detail::us_counter, timeout)
	{
	}
};

extern void wait(detail::us_counter_t::time_type time);

template <typename Process>
void wait(detail::us_counter_t::time_type time, Process process)
{
	detail::wait(detail::us_counter, time * 1000, process);
}

template <typename Process>
void wait(detail::us_counter_t::time_type time, Process process, int)
{
	detail::wait(detail::us_counter, time * 1000, process, 0);
}


extern void uwait(detail::us_counter_t::time_type time);

template <typename Process>
void uwait(detail::us_counter_t::time_type time, Process process)
{
	detail::uwait(detail::us_counter, time, process);
}

template <typename Process>
void uwait(detail::us_counter_t::time_type time, Process process, int)
{
	detail::uwait(detail::us_counter, time, process, 0);
}


extern void delayMs(unsigned int ms);
extern void delayUs(unsigned int us);

extern detail::us_counter_t::time_type usec(detail::us_counter_t::time_type t);
extern detail::us_counter_t::time_type msec(detail::us_counter_t::time_type t);
extern detail::us_counter_t::time_type  sec(detail::us_counter_t::time_type t);

} // namespace ev3cxx
