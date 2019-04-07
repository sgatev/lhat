#ifndef LHAT_NAMELESS_OVERLOADED_H
#define LHAT_NAMELESS_OVERLOADED_H

namespace lhat {
namespace nameless {
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
auto MakeOverloaded(Ts&&... ts) {
  return Overloaded<Ts...>(std::forward<Ts>(ts)...);
}
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_OVERLOADED_H
