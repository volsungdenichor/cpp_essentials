#pragma once

#define FORWARD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define LAMBDA(...) [](auto&& _) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }