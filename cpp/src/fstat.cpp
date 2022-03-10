#include <bits/types/struct_timespec.h>
#include <chrono>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <mqueue.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <unistd.h>

template <>
struct fmt::formatter<::timespec> {
  constexpr auto parse(format_parse_context &context) -> decltype(context.begin()) {
    return context.begin();
  }
  template <typename Context>
  auto format(::timespec const &value, Context &context) -> decltype(context.out()) {
    return format_to(context.out(), "{{tv_sec = {}, tv_nsec = {}}}", value.tv_sec, value.tv_nsec);
  }
};
template <>
struct fmt::formatter<struct ::stat> {
  constexpr auto parse(format_parse_context &context) -> decltype(context.begin()) {
    return context.begin();
  }
  template <typename Context>
  auto format(struct ::stat const &value, Context &context) -> decltype(context.out()) {
    return format_to(context.out(),
                     "{{st_dev = {}, st_ino = {}, st_mode = {:04o}, st_nlink = {}, st_uid = {}, "
                     "st_gid = {}, st_rdev = {}, st_size = {}, st_blksize = {}, st_blocks = {}, "
                     "st_atim = {}, st_mtim = {}, st_ctim = {}}}",
                     value.st_dev,
                     value.st_ino,
                     value.st_mode,
                     value.st_nlink,
                     value.st_uid,
                     value.st_gid,
                     value.st_rdev,
                     value.st_size,
                     value.st_blksize,
                     value.st_blocks,
                     value.st_atim,
                     value.st_mtim,
                     value.st_ctim);
  }
};

auto to_timepoint(::timespec value) noexcept -> std::chrono::system_clock::time_point {
  using namespace std::chrono;
  auto const duration = seconds{value.tv_sec} + nanoseconds{value.tv_nsec};
  return system_clock::time_point{duration_cast<system_clock::duration>(duration)};
}

int main() {
  auto const mq_name = "/sandbox.fstat.mq";

  auto epoll = ::epoll_create1(0);
  struct ::stat epoll_stat {};

  auto mq = ::mq_open(mq_name, O_RDONLY | O_CREAT, 0666, nullptr);
  struct ::stat mq_stat {};

  fmt::print("epoll+fstat: {} {}\n", epoll, ::fstat(epoll, &epoll_stat));
  fmt::print("mq+fstat: {} {}\n", mq, ::fstat(mq, &mq_stat));

  fmt::print("epoll_stat: {}\n", to_timepoint(epoll_stat.st_atim));
  fmt::print("mq_stat:    {}\n", to_timepoint(mq_stat.st_ctim));

  ::mq_unlink(mq_name);
  ::close(epoll);
}
