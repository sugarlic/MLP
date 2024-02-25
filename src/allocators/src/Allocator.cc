#include "Allocator.h"

#include <cassert>
namespace s21 {
Allocator::Allocator(const std::size_t size_bytes, void* const start) noexcept
    : size_(size_bytes), used_bytes_(0), num_allocations_(0), start_(start) {
  assert(size_bytes > 0);
}

Allocator::Allocator(Allocator&& other) noexcept
    : size_(other.size_),
      used_bytes_(other.used_bytes_),
      num_allocations_(other.num_allocations_),
      start_(other.start_) {
  other.start_ = nullptr;
  other.size_ = 0;
  other.num_allocations_ = 0;
  other.used_bytes_ = 0;
}

Allocator& Allocator::operator=(Allocator&& other) noexcept {
  size_ = other.size_;
  used_bytes_ = other.used_bytes_;
  num_allocations_ = other.num_allocations_;
  start_ = other.start_;

  other.start_ = nullptr;
  other.size_ = 0;
  other.num_allocations_ = 0;
  other.used_bytes_ = 0;

  return *this;
}

Allocator::~Allocator() noexcept {
  assert(num_allocations_ == 0 && used_bytes_ == 0);
}

const std::size_t& Allocator::GetSize() const noexcept { return size_; }

const std::size_t& Allocator::GetUsed() const noexcept { return used_bytes_; }

const std::size_t& Allocator::GetNumAllocation() const noexcept {
  return num_allocations_;
}

const void* Allocator::GetStart() const noexcept { return start_; }
}  // namespace s21
