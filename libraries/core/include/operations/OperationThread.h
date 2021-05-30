#pragma once
#include <atomic>
#include <mutex>
#include <thread>
#include <optional>
#include <vector>

#include "OperationThreadBase.h"

class OperationThread : public OperationThreadBase
{
public:
	OperationThread();
	~OperationThread();

	void SetName(const std::string& name) override;
	void Dispatch(std::weak_ptr<Operation> operation) override;
	std::thread::id GetId() const override;

private:
	struct State
	{
		std::atomic_bool						stopped {false};
		std::condition_variable					condition;
		std::mutex								mutex;
		std::optional<std::string>				changedName;
		std::vector<std::weak_ptr<Operation>>	operations;
	};

	static void ThreadFunc(std::weak_ptr<State> weakState);
	
	std::shared_ptr<State>					_state;
	std::thread								_thread;
};
