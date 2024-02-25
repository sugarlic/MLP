#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <cstdint>
namespace s21 {

class Allocator {
 public:
  Allocator(const std::size_t size_bytes, void* const start) noexcept;

  Allocator(const Allocator&) = delete;
  Allocator& operator=(Allocator&) = delete;
  Allocator(Allocator&&) noexcept;
  Allocator& operator=(Allocator&&) noexcept;

  virtual ~Allocator() noexcept;

  virtual void* Allocate(
      const std::size_t& size,
      const std::uintptr_t& alignment = sizeof(std::intptr_t)) = 0;

  virtual void Free(void* const ptr) = 0;

  const std::size_t& GetSize() const noexcept;
  const std::size_t& GetUsed() const noexcept;
  const std::size_t& GetNumAllocation() const noexcept;

  const void* GetStart() const noexcept;

 protected:
  std::size_t size_;
  std::size_t used_bytes_;
  std::size_t num_allocations_;

  void* start_;
};
}  // namespace s21

#endif  // ALLOCATOR_H