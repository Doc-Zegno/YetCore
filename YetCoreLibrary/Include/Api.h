#pragma once

#ifdef YETCORELIBRARY_EXPORTS
#define YETCORELIBRARY_API __declspec(dllexport)
#else
#define YETCORELIBRARY_API __declspec(dllimport)
#endif
