#pragma once

#include <cstdint>

/// <summary>
/// Represents region occupied by some expression
/// in the original source code file.
/// Negative values mean "unknown"
/// </summary>
struct ExpressionLocation {
	int16_t firstLine = -1;
	int16_t firstColumn = -1;
	int16_t lastLine = -1;
	int16_t lastColumn = -1;

	/// <summary>
	/// Use when only the first line of expression is known
	/// </summary>
	/// <param name="line">The first line of expression</param>
	void setLine(int line) {
		firstLine = int16_t(line);
		firstColumn = -1;
		lastLine = -1;
		lastColumn = -1;
	}

	void set(int firstLine, int firstColumn, int lastLine, int lastColumn) {
		this->firstLine = int16_t(firstLine);
		this->firstColumn = int16_t(firstColumn);
		this->lastLine = int16_t(lastLine);
		this->lastColumn = int16_t(lastColumn);
	}

	void reset() {
		firstLine = -1;
		firstColumn = -1;
		lastLine = -1;
		lastColumn = -1;
	}
};
