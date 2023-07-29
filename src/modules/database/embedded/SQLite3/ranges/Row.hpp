#pragma once

#include <ranges>
#include <iterator>
#include <type_traits>
#include <memory>

#include "Table.hpp"


namespace database::embedded::SQLite3{


template <class FieldType>
class RowIterator;


template <class FieldType>
class Row : public std::ranges::view_interface<Row<FieldType>>
{

  friend class Table;
  friend class RowIterator<FieldType>;


public:

  Row(sqlite3_stmt* stmt = nullptr, int columnCount_ = 0) noexcept;

  [[nodiscard]]
  RowIterator<FieldType> begin() const noexcept;

  [[nodiscard]]
  RowIterator<FieldType> end() const noexcept;

private:

  sqlite3_stmt* stmt_;

  int columnCount_;
};

template <class FieldTypeS>
auto operator<=>(
  const RowIterator<FieldTypeS>& a,
  const RowIterator<FieldTypeS>& b
) noexcept;
template <class FieldTypeS>
bool operator==(
  const RowIterator<FieldTypeS>& a,
  const RowIterator<FieldTypeS>& b
) noexcept;
template <class FieldTypeS>
RowIterator<FieldTypeS>::difference_type operator-(
  const RowIterator<FieldTypeS>& a,
  const RowIterator<FieldTypeS>& b
) noexcept;
template <class FieldTypeS>
RowIterator<FieldTypeS> operator+(
  const RowIterator<FieldTypeS>& iter,
  const typename Row<FieldTypeS>::difference_type& distance
) noexcept;
template <class FieldTypeS>
RowIterator<FieldTypeS> operator+(
  const typename RowIterator<FieldTypeS>::difference_type& distance,
  const RowIterator<FieldTypeS>& iter
) noexcept;
template <class FieldTypeS>
RowIterator<FieldTypeS> operator-(
  const RowIterator<FieldTypeS>& iter,
  const typename RowIterator<FieldTypeS>::difference_type& distance
) noexcept;



//Tried to have this iterator nested in its Row class at first, but compiler
//was very vocal about not liking that, attempting to overload operator
//for class nested inside a template is not properly recognized due to
//nested type not being considered deducible context by the standard
//itself. Oh well.
template <class FieldType>
class RowIterator
{
public:

  using iterator_category = std::random_access_iterator_tag;
  using value_type = std::remove_cv_t<FieldType>;
  using pointer = std::unique_ptr<value_type>;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;

  explicit RowIterator(const Row<FieldType>* row = nullptr, int column_ = 0) noexcept;

  pointer operator->() const noexcept;
  value_type operator*() const noexcept;
  value_type operator[](const difference_type& distance) const noexcept;
  
  RowIterator& operator++() noexcept;
  RowIterator operator++(int) noexcept;
  RowIterator& operator--() noexcept;
  RowIterator operator--(int) noexcept;
  RowIterator& operator+=(const difference_type& distance) noexcept;
  RowIterator& operator-=(const difference_type& distance) noexcept;

  /*
  auto operator<=>(const RowIterator& b) noexcept;
  bool operator==(const RowIterator& b) noexcept;
  difference_type operator-(const RowIterator& b) noexcept;
  RowIterator operator+(const difference_type& distance) noexcept;
  RowIterator operator-(const difference_type& distance) noexcept;
  friend RowIterator operator+(
    const difference_type& distance,
    const RowIterator& iter
  );
  */
/*
  friend auto operator<=> <>(
    const Base& a,
    const Base& b
  ) noexcept;
  friend bool operator== <>(
    const Base& a,
    const Base& b
  ) noexcept;
  friend Base::difference_type operator- <>(
    const Base& a,
    const Base& b
  ) noexcept;
  friend Base operator+ <>(
    const Base& iter,
    const typename Base::difference_type& distance
  ) noexcept;
  friend Base operator+ <>(
    const typename Base::difference_type& distance,
    const Base& iter
  ) noexcept;
  friend Base operator-<>(
    const Base& iter,
    const typename Base::difference_type& distance
  ) noexcept;
*/



  template <class FieldTypeS>
  friend auto operator<=>(
    const RowIterator<FieldTypeS>& a,
    const RowIterator<FieldTypeS>& b
  ) noexcept;
  template <class FieldTypeS>
  friend bool operator==(
    const RowIterator<FieldTypeS>& a,
    const RowIterator<FieldTypeS>& b
  ) noexcept;
  template <class FieldTypeS>
  friend RowIterator<FieldTypeS>::difference_type operator-(
    const RowIterator<FieldTypeS>& a,
    const RowIterator<FieldTypeS>& b
  ) noexcept;
  template <class FieldTypeS>
  friend RowIterator<FieldTypeS> operator+(
    const RowIterator<FieldTypeS>& iter,
    const typename Row<FieldTypeS>::difference_type& distance
  ) noexcept;
  template <class FieldTypeS>
  friend RowIterator<FieldTypeS> operator+(
    const typename RowIterator<FieldTypeS>::difference_type& distance,
    const RowIterator<FieldTypeS>& iter
  ) noexcept;
  template <class FieldTypeS>
  friend RowIterator<FieldTypeS> operator-(
    const RowIterator<FieldTypeS>& iter,
    const typename RowIterator<FieldTypeS>::difference_type& distance
  ) noexcept;


private:

  const Row<FieldType>* row_;

  int column_;

};

/*
template <class Derived>
class RowIteratorBase{
  public:
  using difference_type = Derived::difference_type;
  using value_type = Derived::value_type;
};
*/

/*
template <class FieldType>
auto operator<=>(
  const typename Row<FieldType>::Iterator& a,
  const typename Row<FieldType>::Iterator& b
) noexcept;


template <class FieldType>
bool operator==(
  const typename Row<FieldType>::Iterator& a,
  const typename Row<FieldType>::Iterator& b
) noexcept;


template <class FieldType>
Row<FieldType>::Iterator::difference_type operator-(
  const typename Row<FieldType>::Iterator& a,
  const typename Row<FieldType>::Iterator& b
) noexcept;


template <class FieldType>
Row<FieldType>::Iterator operator+(
  const typename Row<FieldType>::Iterator& iter,
  const typename Row<FieldType>::Iterator::difference_type& distance
) noexcept;


template <class FieldType>
Row<FieldType>::Iterator operator+(
  const typename Row<FieldType>::Iterator::difference_type& distance,
  const typename Row<FieldType>::Iterator& iter
) noexcept;


template <class FieldType>
Row<FieldType>::Iterator operator-(
  const typename Row<FieldType>::Iterator& iter,
  const typename Row<FieldType>::Iterator::difference_type& distance
) noexcept;
*/


}


#include "Row.tpp"