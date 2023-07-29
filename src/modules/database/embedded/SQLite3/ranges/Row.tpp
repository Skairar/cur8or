#pragma once

#include "Row.hpp"


namespace database::embedded::SQLite3{


template <class FieldType>
Row<FieldType>::Row(
  sqlite3_stmt* stmt,
  int columnCount
) noexcept
 : stmt_{stmt},
   columnCount_{columnCount}
{
  //empty
}


template <class FieldType>
RowIterator<FieldType> Row<FieldType>::begin() const noexcept{
  return RowIterator<FieldType>{this};
}


template <class FieldType>
RowIterator<FieldType> Row<FieldType>::end() const noexcept{
  return RowIterator<FieldType>{this, columnCount_};
}


template <class FieldType>
RowIterator<FieldType>::RowIterator(
  const Row<FieldType>* row,
  int column
) noexcept
 : row_{row},
   column_{column}
{
  //empty
}


template <class FieldType>
RowIterator<FieldType>::pointer
  RowIterator<FieldType>::operator->() const noexcept{
  return std::make_unique<FieldType>(row_->stmt_, column_);
}


template <class FieldType>
RowIterator<FieldType>::value_type
  RowIterator<FieldType>::operator*() const noexcept{
  return FieldType{row_->stmt_, column_};
}


template <class FieldType>
RowIterator<FieldType>::value_type
  RowIterator<FieldType>::operator[](
    const typename RowIterator<FieldType>::difference_type& distance
  ) const noexcept{
  return FieldType{row_->stmt_, column_ + distance};
}


template <class FieldType>  
RowIterator<FieldType>& RowIterator<FieldType>::operator++() noexcept{
  ++column_;
  return *this;
}


template <class FieldType>
RowIterator<FieldType> RowIterator<FieldType>::operator++(int) noexcept{
  return RowIterator<FieldType>{row_, column_++};
}


template <class FieldType>
RowIterator<FieldType>& RowIterator<FieldType>::operator--() noexcept{
  --column_;
  return *this;
}


template <class FieldType>
RowIterator<FieldType> RowIterator<FieldType>::operator--(int) noexcept{
  return RowIterator<FieldType>{row_, column_--};
}


template <class FieldType>
RowIterator<FieldType>& RowIterator<FieldType>::operator+=(
  const typename RowIterator<FieldType>::difference_type& distance
) noexcept{
  column_ += distance;
  return *this;
}


template <class FieldType>
RowIterator<FieldType>& RowIterator<FieldType>::operator-=(
  const typename RowIterator<FieldType>::difference_type& distance
) noexcept{
  column_ -= distance;
  return *this;
}


template <class FieldTypeS>
auto operator<=>(
  const RowIterator<FieldTypeS>& a,
  const RowIterator<FieldTypeS>& b
) noexcept{
  return a.column_ <=> b.column_;
}


template <class FieldTypeS>
bool operator==(
  const RowIterator<FieldTypeS>& a,
  const RowIterator<FieldTypeS>& b
) noexcept{
  return (a.column_ == b.column_) && (b.row_ == b.row_);
}


template <class FieldTypeS>
RowIterator<FieldTypeS>::difference_type operator-(
  const RowIterator<FieldTypeS>& a,
  const RowIterator<FieldTypeS>& b
) noexcept{
  return a.column_ - b.column_;
}


template <class FieldTypeS>
RowIterator<FieldTypeS> operator+(
  const RowIterator<FieldTypeS>& iter,
  const typename RowIterator<FieldTypeS>::difference_type& distance
) noexcept{
  return RowIterator<FieldTypeS>{iter.row_, iter.column_ + distance};
}


template <class FieldTypeS>
RowIterator<FieldTypeS> operator+(
  const typename RowIterator<FieldTypeS>::difference_type& distance,
  const RowIterator<FieldTypeS>& iter
) noexcept{
  return RowIterator<FieldTypeS>{iter.row_, iter.column_ + distance};
}


template <class FieldTypeS>
RowIterator<FieldTypeS> operator-(
  const RowIterator<FieldTypeS>& iter,
  const typename RowIterator<FieldTypeS>::difference_type& distance
) noexcept{
  return RowIterator<FieldTypeS>{iter.row_, iter.column_ - distance};
}


}