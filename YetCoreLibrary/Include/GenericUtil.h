#pragma once

template<bool condition, typename TIf, typename TElse>
struct EnableIfElse {};

template<typename TIf, typename TElse>
struct EnableIfElse<true, TIf, TElse> {
	using Type = TIf;
};

template<typename TIf, typename TElse>
struct EnableIfElse<false, TIf, TElse> {
	using Type = TElse;
};
