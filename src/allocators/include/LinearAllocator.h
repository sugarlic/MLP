#ifndef LINEARALLOCATOR_H
#define LINEARALLOCATOR_H

#include "Allocator.h"
#include "Logger.h"
namespace s21 {
class LinearAllocator : public Allocator {
 public:
  LinearAllocator(const std::size_t size_bytes, void* const start) noexcept;
  ~LinearAllocator() noexcept;
  LinearAllocator(const LinearAllocator&) = delete;
  LinearAllocator& operator=(const LinearAllocator&) = delete;
  LinearAllocator(LinearAllocator&&) noexcept;
  LinearAllocator& operator=(LinearAllocator&&) noexcept;

  virtual void* Allocate(
      const std::size_t& size,
      const std::uintptr_t& alignment = sizeof(std::intptr_t)) override;

  virtual void Free(void* const ptr) noexcept override final;

  void* GetCurrent() const noexcept;

  void Rewind(void* const mark) noexcept;
  void Clear() noexcept;

 protected:
  void* current_;

 private:
  Logger logger_{"LinearAllocator"};
};
}  // namespace s21

#endif  // LINEARALLOCATOR_H