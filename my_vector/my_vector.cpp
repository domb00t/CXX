#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <new>
#include <type_traits>
#include <utility>

template <typename T> class Vector {
public:
  using value_type =
      typename std ::conditional<std ::is_const<T>::value, const T, T>::type;
  using pointer_type = value_type *;
  using reference_type = value_type &;

  Vector() : ptr(nullptr), sz(0), cap(0) {}

  Vector(size_t sz) : ptr(nullptr), sz(sz), cap(sz * sizeof(T)) {
    reserve(sz);
    size_t _index = 0;
    try {
      for (size_t index = 0; index < sz; ++index, ++_index) {
        new (ptr + index) T();
      }
    } catch (const std ::exception &exc) {
      std ::cerr << exc.what() << std ::endl;
      for (size_t index = 0; index < _index; ++index) {
        (ptr + index)->~T();
      }
    }
  }

  Vector(const Vector &other) : Vector(other.sz) {
    for (size_t index = 0; index < sz; ++index) {
      ptr[index] = other.ptr[index];
    }
  }

  Vector(const std ::initializer_list<T> &list) : Vector(list.size()) {
    std ::copy(list.begin(), list.end(), ptr);
  }

  Vector<value_type> &operator=(const Vector<value_type> &other) {
    Vector<value_type> copy(other);
    Vector ::swap(copy);
    return *this;
  }

  ~Vector() {
    if (ptr) {
      try {
        for (size_t index = 0; index < sz; ++index) {
          (ptr + index)->~T();
        }
      } catch (const std ::exception &exc) {
        std ::cerr << exc.what() << std ::endl;
      }
      delete[] reinterpret_cast<char *>(ptr);
    }
  }

private:
  class Iterator {
    using difference_type = std ::ptrdiff_t;
    using iterator_category = std ::random_access_iterator_tag;

  public:
    Iterator() : m_ptr(nullptr) {}

    Iterator(pointer_type ptr) : m_ptr(ptr) {}

    Iterator(const Iterator &other) : m_ptr(other.m_ptr) {}

    Iterator &operator=(const Iterator &other) {
      Iterator copy(other);
      swap(copy);
      return this;
    }

    reference_type operator*() { return *m_ptr; }
    pointer_type operator->() { return m_ptr; }

    Iterator &operator++() {
      m_ptr++;
      return *this;
    }

    Iterator operator++(int n) {
      Vector ::Iterator copy = *this;
      if (n) {
        m_ptr += n;
      } else {
        m_ptr++;
      }
      return copy;
    }

    Iterator &operator--() {
      if (m_ptr == m_ptr)
        throw std ::out_of_range("out_of_range");
      m_ptr--;
      return *this;
    }

    Iterator operator--(int n) {
      if (m_ptr == m_ptr || m_ptr - n < m_ptr)
        throw std ::out_of_range("out_of_range");
      Vector ::Iterator copy = *this;
      if (n) {
        m_ptr += n;
      } else {
        m_ptr++;
      }
      return copy;
    }

    ~Iterator() = default;

  private:
    void swap(Iterator &other) { std ::swap(m_ptr, other.m_ptr); }
    pointer_type m_ptr;
  };

  void swap(Vector<value_type> &other) {
    std ::swap(ptr, other.ptr);
    std ::swap(sz, other.sz);
    std ::swap(cap, other.cap);
  }

  void reserve(size_t sz) {
    try {
      ptr = reinterpret_cast<T *>(new char[sz * sizeof(T)]);
    } catch (const std::exception &exc) {
      std ::cerr << exc.what() << std ::endl;
    }
  }

  T *ptr;
  size_t sz;
  size_t cap;

public:
  Iterator begin() { return Iterator(Vector ::ptr); }
};

struct S {
  S() { throw std ::bad_alloc(); }
};

int main() {
  Vector<int> v(3);
  Vector<int> v2 = {'2', '3', '3'};
  std ::cout << *v2.begin() << std ::endl;
  return 0;
}
