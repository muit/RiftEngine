// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"
#include <EASTL/memory.h>
#include <taskflow/taskflow.hpp>
#include <tracy/common/TracySystem.hpp>

#include "Strings/String.h"


EA_DISABLE_VC_WARNING(4267)


using TaskFlow = tf::Taskflow;
using Task     = tf::Task;
using Flow     = tf::FlowBuilder;
using SubFlow  = tf::SubflowBuilder;

using TaskLambda = eastl::function<void()>;
using SubTaskLambda = eastl::function<void(tf::SubflowBuilder&)>;


struct TaskSystem {
	using ThreadPool = TaskFlow::Executor;

private:

	std::shared_ptr<ThreadPool> threadPool;

public:

	TaskSystem() : threadPool{ std::make_shared<ThreadPool>(std::thread::hardware_concurrency()) }
	{
		for (size_t i = 0; i < threadPool->_threads.size(); ++i)
		{
			// Name each worker thread in the debugger
			tracy::SetThreadName(threadPool->_threads[i], CString::Printf("Worker %i", i+1).c_str());
		}
	}

	FORCEINLINE TaskFlow CreateFlow() const { return TaskFlow{ threadPool }; }

	FORCEINLINE u32 GetNumWorkerThreads() const { return (u32)threadPool->num_workers(); }
};

EA_RESTORE_VC_WARNING() // warning: 4267
