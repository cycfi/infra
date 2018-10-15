/*=============================================================================
   Copyright (c) 2016-2018 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_BUFFER_OCTOBER_15_2018)
#define CYCFI_BUFFER_OCTOBER_15_2018

#include <cstddef>
#include <iterator>
#include <common/assert.hpp>

namespace cycfi
{
   ////////////////////////////////////////////////////////////////////////////
   // Iterator Range holds an iterator pair
	////////////////////////////////////////////////////////////////////////////
   template <typename Iterator>
   class iterator_range
   {
   public:

      using traits = std::iterator_traits<Iterator>;
      using difference_type = typename traits::difference_type;
      using value_type = typename traits::value_type;
      using pointer = typename traits::pointer;
      using reference = typename traits::reference;

                           iterator_range();
                           iterator_range(Iterator f, Iterator l);

      Iterator             begin() const  { return _f; }
      Iterator             end() const    { return _l; }
      value_type           operator[](std::size_t i) const;

   private:

      Iterator             _f;
      Iterator             _l;
   };

   ////////////////////////////////////////////////////////////////////////////
   // audio_buffer holds a pointer to a multichannel audio buffer
   ////////////////////////////////////////////////////////////////////////////
   template <typename T>
   class audio_buffer
   {
   public:

      using sample_type = T;

      audio_buffer(T* base, std::size_t num_channels, std::size_t size)
         : _base(base)
         , _num_channels(num_channels)
         , _size(size)
      {}

      iterator_range<T*>   operator[](std::size_t channel) const;
      std::size_t          num_channels() const { return _num_channels; }
      std::size_t          size() const         { return _size; }

   private:

      T*                   _base;
      std::size_t          _num_channels;
      std::size_t          _size;
   };

   ////////////////////////////////////////////////////////////////////////////
   // Implementation
   ////////////////////////////////////////////////////////////////////////////
   template <typename Iterator>
   iterator_range<Iterator>::iterator_range()
    : _f(), _l() {}

   template <typename Iterator>
   iterator_range<Iterator>::iterator_range(Iterator f, Iterator l)
    : _f(f), _l(l)
   {
      CYCFI_ASSERT((_f <= _l), "Invalid range");
   }

   template <typename Iterator>
   inline iterator_range<Iterator>
   make_iterator_range(Iterator f, Iterator l)
   {
      return iterator_range<Iterator>(f, l);
   }

   template <typename Iterator>
   inline typename iterator_range<Iterator>::value_type
   iterator_range<Iterator>::operator[](std::size_t i) const
   {
      CYCFI_ASSERT(i < (_l - _f), "Index out of range");
      return _f[i];
   }

   template <typename T>
   inline iterator_range<T*>
   audio_buffer<T>::operator[](std::size_t channel) const
   {
      T* start = _base + (channel * _size);
      return { start, start + _size };
   }
}

#endif
