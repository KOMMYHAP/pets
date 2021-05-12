#pragma once
#include <memory>
#include <atomic>

class Operation : public std::enable_shared_from_this<Operation>
{
public:
	enum class State
	{
		WaitingForStart,
		Started,
		Completed,
	};
	enum class Result
	{
		Completed,
		InProcess
	};

	virtual ~Operation() = default;

	bool Completed() const { return _state == State::Completed; }
	bool Good() const { return !_exception; }
	bool Bad() const { return !Good(); }
	
	void PreAction();
	Result Do();
	void PostAction();

	void OnCompleted();

protected:
	virtual void PreActionImpl() {}
	virtual Result DoImpl() = 0;
	virtual void PostActionImpl() {}
	
	/**
	 * \brief Function called on main thread to signal about completed operation and caught (or not) exception.
	 * Default implementation just rethrows caught exception.
	 * \param exception Exception caught when operation was processing.
	 */
	virtual void OnCompleted(std::exception_ptr exception);

private:
	std::atomic<State>	_state {State::WaitingForStart};
	std::exception_ptr	_exception;
};
