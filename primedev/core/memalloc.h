#pragma once

extern "C" void* _malloc_base(size_t size);
extern "C" void* _calloc_base(size_t const count, size_t const size);
extern "C" void* _realloc_base(void* block, size_t size);
extern "C" void* _recalloc_base(void* const block, size_t const count, size_t const size);
extern "C" void _free_base(void* const block);
extern "C" char* _strdup_base(const char* src);

void* operator new(size_t n);
void operator delete(void* p) noexcept;
