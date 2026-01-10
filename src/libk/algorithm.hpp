#ifndef LIBK_ALGORITHM_HPP
#define LIBK_ALGORITHM_HPP

#include <stddef.h>

namespace std {
  template <class T>
  [[nodiscard]] const T& min(const T& lhs, const T& rhs)
  {
    return lhs < rhs ? lhs : rhs;
  }

  template <class ForwardIt, class T>
  void fill(ForwardIt first, ForwardIt last, const T& value)
  {
    for (; first != last; ++first) {
      *first = value;
    }
  }

  template <class InputIt, class OutputIt>
  OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
  {
    for (; first != last; ++first, ++d_first) {
      *d_first = *first;
    }
    return d_first;
  }

  template <class InputIt, class OutputIt, class UnaryPred>
  OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPred pred)
  {
    for (; first != last; ++first) {
      if (pred(*first)) {
        *d_first = *first;
        ++d_first;
      }
    }
    return d_first;
  }

  template <class BidirIt1, class BidirIt2>
  BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
  {
    while (first != last) {
      *(--d_last) = *(--last);
    }
    return d_last;
  }

  template <class InputIt, class OutputIt, class UnaryOp>
  constexpr OutputIt
  transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOp unary_op)
  {
    for (; first1 != last1; ++d_first, ++first1) {
      *d_first = unary_op(*first1);
    }
    return d_first;
  }

  template <class InputIt1, class InputIt2, class OutputIt, class BinaryOp>
  constexpr OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
      OutputIt d_first, BinaryOp binary_op)
  {
    for (; first1 != last1; ++d_first, ++first1, ++first2) {
      *d_first = binary_op(*first1, *first2);
    }
    return d_first;
  }

  template <class InputIt, class UnaryFunc>
  constexpr UnaryFunc for_each(InputIt first, InputIt last, UnaryFunc f)
  {
    for (; first != last; ++first) {
      f(*first);
    }
    return f;
  }

  template <class InputIt, class T>
  size_t count(InputIt first, InputIt last, const T& value)
  {
    size_t ret = 0;
    for (; first != last; ++first) {
      if (*first == value) {
        ++ret;
      }
    }
    return ret;
  }

  template <class InputIt, class UnaryPred>
  size_t count_if(InputIt first, InputIt last, UnaryPred p)
  {
    size_t ret = 0;
    for (; first != last; ++first) {
      if (p(*first)) {
        ++ret;
      }
    }
    return ret;
  }

  template <class InputIt, class T>
  constexpr InputIt find(InputIt first, InputIt last, const T& value)
  {
    for (; first != last; ++first) {
      if (*first == value) {
        return first;
      }
    }
    return last;
  }

  template <class InputIt, class UnaryPred>
  constexpr InputIt find_if(InputIt first, InputIt last, UnaryPred p)
  {
    for (; first != last; ++first) {
      if (p(*first)) {
        return first;
      }
    }
    return last;
  }

  template <class InputIt, class UnaryPred>
  constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPred q)
  {
    for (; first != last; ++first) {
      if (!q(*first)) {
        return first;
      }
    }
    return last;
  }

  template <class InputIt, class OutputIt>
  OutputIt move(InputIt first, InputIt last, OutputIt d_first)
  {
    for (; first != last; ++d_first, ++first) {
      *d_first = std::move(*first);
    }
    return d_first;
  }

  template <class BidirIt1, class BidirIt2>
  BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
  {
    while (first != last) {
      *(--d_last) = std::move(*(--last));
    }
    return d_last;
  }

  template <class ForwardIt, class T>
  ForwardIt remove(ForwardIt first, ForwardIt last, const T& value)
  {
    first = std::find(first, last, value);
    if (first == last) {
      return first;
    }
    for (ForwardIt i = first; ++i != last;) {
      if (!(*i == value)) {
        *first++ = std::move(*i);
      }
    }
    return first;
  }

  template <class ForwardIt, class UnaryPred>
  ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPred p)
  {
    first = std::find_if(first, last, p);
    if (first == last) {
      return first;
    }
    for (ForwardIt i = first; ++i != last;) {
      if (!p(*i)) {
        *first++ = std::move(*i);
      }
    }
    return first;
  }

  template <class ForwardIt1, class ForwardIt2>
  constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b)
  {
    //    using std::swap; // TODO: Implement std::swap
    swap(*a, *b);
  }

}

#endif
