#include "pch.h"
#include "ExecutionContext.h"

namespace {
	thread_local ExecutionContext context;
}

YETCORELIBRARY_API ExecutionContext* yet_ExecutionContext_currentR__get__V__1tPointer_1c() {
	return &context;
}
