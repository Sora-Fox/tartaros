/*
 * This file is part of the Tartaros project.
 *
 * Copyright (C) 2026 Sora Fox [github.com/Sora-Fox]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBK_UTILITY_HPP
#define LIBK_UTILITY_HPP

namespace std {
  template <typename T>
  struct remove_reference
  {
    typedef T type;
  };

  template <typename T>
  struct remove_reference<T&>
  {
    typedef T type;
  };

  template <typename T>
  struct remove_reference<T&&>
  {
    typedef T type;
  };

  template <typename T>
  using remove_reference_t = typename remove_reference<T>::type;

  template <typename T>
  remove_reference_t<T>&& move(T&& t) noexcept
  {
    return static_cast<remove_reference_t<T>&&>(t);
  }

  template <typename T>
  T&& forward(remove_reference_t<T>& t) noexcept
  {
    return static_cast<T&&>(t);
  }

  template <typename T>
  T&& forward(remove_reference_t<T>&& t) noexcept
  {
    return static_cast<T&&>(t);
  }

  template <typename T>
  void swap(T& a, T& b)
  {
    T temp = move(a);
    a = move(b);
    b = move(temp);
  }

  template <typename T, typename U = T>
  T exchange(T& obj, U&& new_value)
  {
    T old = move(obj);
    obj = forward<U>(new_value);
    return old;
  }

  template <typename T>
  T&& declval();
}

#endif
