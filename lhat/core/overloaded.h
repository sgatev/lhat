#ifndef LHAT_CORE_OVERLOADED_H
#define LHAT_CORE_OVERLOADED_H

namespace lhat {
namespace core {
template <class... Ts>
struct Overloaded;

template <class T>
struct Overloaded<T> : T {
  Overloaded(T&& t) : T(std::forward<T>(t)) {}
  using T::operator();
};

template <class T, class... Ts>
struct Overloaded<T, Ts...> : T, Overloaded<Ts...> {
  Overloaded(T&& t, Ts&&... ts)
      : T(std::forward<T>(t)), Overloaded<Ts...>(std::forward<Ts>(ts)...) {}

  using T::operator();
  using Overloaded<Ts...>::operator();
};

template <class... Ts>
auto Overload(Ts&&... ts) {
  return Overloaded<Ts...>(std::forward<Ts>(ts)...);
}
}  // namespace core
}  // namespace lhat

#endif  // LHAT_CORE_OVERLOADED_H