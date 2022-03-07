#pragma once

#include "Api.h"

struct StackFrame;
struct ExecutionContext;

extern "C" {
	YETCORELIBRARY_API ExecutionContext* yet_ExecutionContext_currentR__get__V__1tPointer_1c();
}

/// <summary>
/// Contains information about execution's context
/// including current stack trace
/// </summary>
struct ExecutionContext {
	StackFrame* current = nullptr;
	int frameCount = 0;

	ExecutionContext() {}

	/// <summary>
	/// Get the execution context for current thread
	/// </summary>
	static ExecutionContext* getCurrent() {
		return yet_ExecutionContext_currentR__get__V__1tPointer_1c();
	}
};

using EC = ExecutionContext;
