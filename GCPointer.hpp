#ifndef _GCPOINTER_HPP
#define _GCPOINTER_HPP

/* MIT License
 *
 * Copyright(c) 2018 kmc7468
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/////////////////////////////////////////////////////////////////
///// Includes and Global Macros
/////////////////////////////////////////////////////////////////

#include <cstddef>
#include <vector>

#define _GCPOINTER_CPLUSPLUS98 199711L
#define _GCPOINTER_CPLUSPLUS03 _GCPOINTER_CPLUSPLUS98
#define _GCPOINTER_CPLUSPLUS11 201103L
#define _GCPOINTER_CPLUSPLUS14 201402L
#define _GCPOINTER_CPLUSPLUS17 201703L

#ifdef _GCPOINTER_FLAGS_CPLUSPLUS98
#	define _GCPOINTER_CPLUSPLUS _GCPOINTER_CPLUSPLUS98
#elif defined(_GCPOINTER_FLAGS_CPLUSPLUS03)
#	define _GCPOINTER_CPLUSPLUS _GCPOINTER_CPLUSPLUS03
#elif defined(_GCPOINTER_FLAGS_CPLUSPLUS11)
#	define _GCPOINTER_CPLUSPLUS _GCPOINTER_CPLUSPLUS11
#elif defined(_GCPOINTER_FLAGS_CPLUSPLUS14)
#	define _GCPOINTER_CPLUSPLUS _GCPOINTER_CPLUSPLUS14
#elif defined(_GCPOINTER_FLAGS_CPLUSPLUS17)
#	define _GCPOINTER_CPLUSPLUS _GCPOINTER_CPLUSPLUS17
#else
#	if defined(_MSC_VER) && _MSC_FULL_VER >= 190024210 // Visual Studio 2015 Update 3
#		define _GCPOINTER_CPLUSPLUS _MSVC_LANG
#	else
#		define _GCPOINTER_CPLUSPLUS __cplusplus
#	endif
#endif

#define _GCPOINTER_IS_CPLUSPLUS98 (_GCPOINTER_CPLUSPLUS >= _GCPOINTER_CPLUSPLUS98)
#define _GCPOINTER_IS_CPLUSPLUS03 (_GCPOINTER_CPLUSPLUS >= _GCPOINTER_CPLUSPLUS03)
#define _GCPOINTER_IS_CPLUSPLUS11 (_GCPOINTER_CPLUSPLUS >= _GCPOINTER_CPLUSPLUS11)
#define _GCPOINTER_IS_CPLUSPLUS14 (_GCPOINTER_CPLUSPLUS >= _GCPOINTER_CPLUSPLUS14)
#define _GCPOINTER_IS_CPLUSPLUS17 (_GCPOINTER_CPLUSPLUS >= _GCPOINTER_CPLUSPLUS17)

#define _GCPOINTER_DELETE

/////////////////////////////////////////////////////////////////
///// Declarations
/////////////////////////////////////////////////////////////////

#ifdef _GCPOINTER_HAS_NAMESPACE
namespace _GCPOINTER_HAS_NAMESPACE
{
#endif

template<typename Ty_>
class gc_ptr;
template<typename Ty_>
class gc_field_ptr;

template<typename Ty_>
class _gc_ptr_data
{
	friend class gc_ptr<Ty_>;
	friend class gc_field_ptr<Ty_>;

private:
	_gc_ptr_data();
	_gc_ptr_data(std::size_t strong_reference_count);
	_gc_ptr_data(const std::vector<gc_field_ptr<Ty_>*>& field_reference_count);
	_gc_ptr_data(std::size_t strong_reference_count, const std::vector<gc_field_ptr<Ty_>*>& field_reference_count);
#if _GCPOINTER_IS_CPLUSPLUS11
	_gc_ptr_data(const _gc_ptr_data& data) = delete;
	_gc_ptr_data(_gc_ptr_data&& data) = delete;
	~_gc_ptr_data() = default;
#else
	_gc_ptr_data(const _gc_ptr_data& data) _GCPOINTER_DELETE;
	~_gc_ptr_data();
#endif

private:
#if _GCPOINTER_IS_CPLUSPLUS11
	_gc_ptr_data& operator=(const _gc_ptr_data& data) = delete;
	_gc_ptr_data& operator=(_gc_ptr_data&& data) = delete;
	bool operator==(const _gc_ptr_data& data) = delete;
	bool operator!=(const _gc_ptr_data& data) = delete;
#else
	_gc_ptr_data& operator=(const _gc_ptr_data& data) _GCPOINTER_DELETE;
	bool operator==(const _gc_ptr_data& data) _GCPOINTER_DELETE;
	bool operator!=(const _gc_ptr_data& data) _GCPOINTER_DELETE;
#endif

private:
	void inc_strong_reference_count();

private:
	std::size_t strong_reference_count_;
	std::vector<gc_field_ptr<Ty_>*> field_reference_count_;
};

template<typename Ty_>
class gc_ptr
{
public:
	typedef Ty_ element_type;
	typedef gc_field_ptr<Ty_> field_type;

public:
	gc_ptr();
	gc_ptr(Ty_* data);
	gc_ptr(const gc_ptr& ptr);
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_ptr(gc_ptr&& ptr) noexcept;
#endif
	~gc_ptr();

private:
	Ty_* data_;
	_gc_ptr_data<Ty_>* mem_data_;
};

template<typename Ty_>
class gc_field_ptr
{
public:
	typedef Ty_ element_type;
	typedef gc_field_ptr<Ty_> strong_type;
};

/////////////////////////////////////////////////////////////////
///// Definitions
/////////////////////////////////////////////////////////////////

//
// _gc_ptr_data
//

template<typename Ty_>
_gc_ptr_data<Ty_>::_gc_ptr_data()
	: strong_reference_count_(0)
{}
template<typename Ty_>
_gc_ptr_data<Ty_>::_gc_ptr_data(std::size_t strong_reference_count)
	: strong_reference_count_(strong_reference_count)
{}
template<typename Ty_>
_gc_ptr_data<Ty_>::_gc_ptr_data(const std::vector<gc_field_ptr<Ty_>*>& field_reference_count)
	: strong_reference_count_(0), field_reference_count_(field_reference_count)
{}
template<typename Ty_>
_gc_ptr_data<Ty_>::_gc_ptr_data(std::size_t strong_reference_count, const std::vector<gc_field_ptr<Ty_>*>& field_reference_count)
	: strong_reference_count_(strong_reference_count), field_reference_count_(field_reference_count)
{}
#if !_GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
_gc_ptr_data<Ty_>::~_gc_ptr_data()
{}
#endif

template<typename Ty_>
void _gc_ptr_data<Ty_>::inc_strong_reference_count()
{
	++strong_reference_count_;
}

//
// gc_ptr
//

template<typename Ty_>
gc_ptr<Ty_>::gc_ptr()
	: data_(NULL), mem_data_(NULL)
{}
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(Ty_* data)
	: data_(data), mem_data_(new _gc_ptr_data<Ty_>(1))
{}
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(const gc_ptr& ptr)
	: data_(ptr.data_), mem_data_(ptr.mem_data_)
{
	if (mem_data_)
	{
		mem_data_->inc_strong_reference_count();
	}
}
#if _GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(gc_ptr&& ptr) noexcept
	: data_(ptr.data_), mem_data_(ptr.mem_data_)
{
	ptr.data_ = ptr.mem_data_ = NULL;
}
#endif
template<typename Ty_>
gc_ptr<Ty_>::~gc_ptr()
{
	// TODO
}

#ifdef _GCPOINTER_HAS_NAMESPACE
}
#endif

#endif