#include "LinearAllocator.h"

#include <cassert>
#include <new>
#include <utility>

#include "UtilityAllocator.h"
namespace s21 {
LinearAllocator::LinearAllocator(const std::size_t size_bytes,
                                 void* const start) noexcept
    : Allocator(size_bytes, start), current_(const_cast<void*>(start)) {}

LinearAllocator::LinearAllocator(LinearAllocator&& other) noexcept
    : Allocator(std::move(other)), current_(other.current_) {
  other.current_ = nullptr;
}

LinearAllocator::~LinearAllocator() noexcept { LinearAllocator::Clear(); }

LinearAllocator& LinearAllocator::operator=(LinearAllocator&& rhs) noexcept {
  Allocator::operator=(std::move(rhs));
  current_ = rhs.current_;
  rhs.current_ = nullptr;
  return *this;
}

void* LinearAllocator::Allocate(const std::size_t& size,
                                const std::uintptr_t& alignment) {
  assert(size > 0 && alignment > 0);

  std::size_t adjustment = align_forward_adjustment(current_, alignment);

  if (used_bytes_ + adjustment + size > size_) throw std::bad_alloc();

  void* alignedAddr = ptr_add(current_, adjustment);

  current_ = ptr_add(alignedAddr, size);

  used_bytes_ = reinterpret_cast<std::uintptr_t>(current_) -
                reinterpret_cast<std::uintptr_t>(start_);

  ++num_allocations_;
  // logger_.Log("Allocated " + std::to_string(used_bytes_) + " bytes out of " +
  //             std::to_string(size_) + " bytes");
  return alignedAddr;
}

void LinearAllocator::Free([[maybe_unused]] void* const ptr) noexcept {
  // you can't free from a linear allocator
}

void LinearAllocator::Rewind(void* const mark) noexcept {
  assert(current_ >= mark && start_ <= mark);

  current_ = mark;

  used_bytes_ = reinterpret_cast<std::uintptr_t>(current_) -
                reinterpret_cast<std::uintptr_t>(start_);
}

void LinearAllocator::Clear() noexcept {
  num_allocations_ = 0;
  used_bytes_ = 0;
  current_ = const_cast<void*>(start_);
}

void* LinearAllocator::GetCurrent() const noexcept { return current_; }
}  // namespace s21
