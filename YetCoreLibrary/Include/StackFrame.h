#pragma once

#include "ExecutionContext.h"
#include "ExpressionLocation.h"

/// <summary>
/// Represents function's stack frame.
/// Used for debugging.
/// </summary>
struct StackFrame {
	StackFrame* previous = nullptr;
	const char* functionName = nullptr;
	ExecutionContext* context = nullptr;
	ExpressionLocation location = {};

	StackFrame(ExecutionContext*& context, const char* functionName) : functionName(functionName) {
		if (context == nullptr) {
			context = yet_ExecutionContext_currentR__get__V__PV();
		}
		this->context = context;
		previous = context->current;
		context->current = this;
		context->frameCount++;
	}

	~StackFrame() {
		// TODO: check against double deinit
		context->current = previous;
		context->frameCount--;
	}

	/// <summary>
	/// A handy way of setting location for C++ side implementations.
	/// Very useful in combination with a <code>__LINE__</code> macro
	/// </summary>
	/// <example>
	/// <code>
	/// frame.setLineAfter(__LINE__);
	/// auto result = yet_Module_path__get__V__PC8();
	/// </code>
	/// </example>
	/// <param name="line"></param>
	void setLineAfter(int line) {
		location.setLine(line + 1);
	}

	void setLocation(int firstLine, int firstColumn, int lastLine, int lastColumn) {
		location.set(firstLine, firstColumn, lastLine, lastColumn);
	}
};
