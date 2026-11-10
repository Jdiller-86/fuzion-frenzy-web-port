#pragma once
#include "slice-prototypes.hpp"
#include <cstdint>
void ff_sdk_observe_enter(X86Context&, uint8_t*, uint32_t);
void ff_sdk_observe_exit(X86Context&, uint8_t*, uint32_t);
void ff_sdk_rep_movs(X86Context&, uint8_t*, uint32_t);
void ff_sdk_rep_stos(X86Context&, uint8_t*, uint32_t);
#define x86_rep_movs ff_sdk_rep_movs
#define x86_rep_stos ff_sdk_rep_stos
