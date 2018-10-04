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

#if defined(_MSC_VER) && _MSC_FULL_VER >= 190024210 // Visual Studio 2015 Update 3
#	define _GCPOINTER_CPLUSPLUS _MSVC_LANG
#else
#	define _GCPOINTER_CPLUSPLUS __cplusplus
#endif

#define _GCPOINTER_CPLUSPLUS11 201103L
#define _GCPOINTER_IS_CPLUSPLUS11 (_GCPOINTER_CPLUSPLUS >= _GCPOINTER_CPLUSPLUS11)
#define _GCPOINTER_CPLUSPLUS14 201402L
#define _GCPOINTER_IS_CPLUSPLUS14 (_GCPOINTER_CPLUSPLUS >= _GCPOINTER_CPLUSPLUS14)
#define _GCPOINTER_CPLUSPLUS17 201703L
#define _GCPOINTER_IS_CPLUSPLUS17 (_GCPOINTER_CPLUSPLUS >= _GCPOINTER_CPLUSPLUS17)

/////////////////////////////////////////////////////////////////
///// Declarations
/////////////////////////////////////////////////////////////////

template<typename Ty_>
class gc_ptr;
template<typename Ty_>
class gc_field_ptr;

template<typename Ty_>
class _gc_ptr_data
{
	friend class gc_ptr<Ty_>;
	friend class gc_field_ptr<Ty_>;

public:
	_gc_ptr_data();
	_gc_ptr_data(std::size_t strong_reference_count);
	_gc_ptr_data(const std::vector<gc_field_ptr<Ty_>*>& field_reference_count);
	_gc_ptr_data(std::size_t strong_reference_count, const std::vector<gc_field_ptr<Ty_>*>& field_reference_count);
#if _GCPOINTER_IS_CPLUSPLUS11
	_gc_ptr_data(const _gc_ptr_data& data) = delete;
	_gc_ptr_data(_gc_ptr_data&& data) = delete;
	~_gc_ptr_data() = default;
#else
	~_gc_ptr_data();

private:
	_gc_ptr_data(const _gc_ptr_data& data);
#endif

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

	using element_type = Ty_;
	using field_type = gc_field_ptr<Ty_>;

public:
	gc_ptr();

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

//
// gc_ptr
//

template<typename Ty_>
gc_ptr<Ty_>::gc_ptr()
	: data_(NULL), mem_data_(NULL)
{}

#endif