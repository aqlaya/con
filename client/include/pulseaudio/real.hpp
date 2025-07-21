#pragma once

#include <type_traits>
#include <string>
#include <iostream>

#if defined (PULSE_AUDIO_LOG) 
    inline void __pulse_debug_log(const char* value) { std::cerr << value; }
    template <typename T, std::enable_if_t< std::is_integral_v<T>, int > = 0>
    inline void __pulse_debug_log(T value) { std::cerr << value; }
    template <typename T, std::enable_if_t<std::is_member_function_pointer_v<decltype(&T::operator std::string)>, int> = 0 >
    inline void __pulse_debug_log(T value) { std::cerr << std::string(value); }
    template <typename ... Args>
    inline void __pulse_debug_log(Args ... args) { ( (__pulse_debug_log(args) , __pulse_debug_log(' ')),  ... ), __pulse_debug_log('\n'); }
#else 
template <typename ... Args>
inline void __pulse_debug_log(Args ...) {}
#endif

void __pulse_debug_construct(const char* name ) {
    __pulse_debug_log("[[Object has created]]:", name);
}

void __pulse_debug_destruct(const char* name) {
    __pulse_debug_log("[[Object has removed]]", name);
}