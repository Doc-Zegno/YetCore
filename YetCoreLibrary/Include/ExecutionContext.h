#pragma once

#include "Api.h"

struct StackFrame;

/// <summary>
/// Contains information about execution's context
/// including current stack trace
/// </summary>
struct ExecutionContext {
	StackFrame* current = nullptr;
	int frameCount = 0;
};

using EC = ExecutionContext;

extern "C" {
	YETCORELIBRARY_API ExecutionContext* yet_ExecutionContext_current__get__V__PV();
}
