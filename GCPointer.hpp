#ifndef _GCPOINTER_HPP
#define _GCPOINTER_HPP

/* MIT License
 *
 * Copyright (c) 2018 kmc7468
 *
 * Permission is hereby granted, reset of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/////////////////////////////////////////////////////////////////
///// Includes and Global Macros
/////////////////////////////////////////////////////////////////

#include <cstddef>
#include <utility>
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

#if _GCPOINTER_IS_CPLUSPLUS11
#	define _GCPOINTER_DEFAULT = default
#	define _GCPOINTER_DELETE = delete
#	define _GCPOINTER_NOEXCEPT noexcept
#	define _GCPOINTER_FINAL final
#else
#	define _GCPOINTER_DEFAULT
#	define _GCPOINTER_DELETE
#	define _GCPOINTER_NOEXCEPT throw()
#	define _GCPOINTER_FINAL
#endif

#ifdef _GCPOINTER_HAS_NAMESPACE
#	define _GCPOINTER_DETAILS details
#else
#	define _GCPOINTER_DETAILS _GCPOINTER_DETAILS
#endif
#	define _GCPOINTER_DETAILS_BEGIN namespace _GCPOINTER_DETAILS {
#	define _GCPOINTER_DETAILS_END }

/////////////////////////////////////////////////////////////////
///// Declarations
/////////////////////////////////////////////////////////////////

#ifdef _GCPOINTER_HAS_NAMESPACE
namespace _GCPOINTER_HAS_NAMESPACE
{
#endif

_GCPOINTER_DETAILS_BEGIN
template<typename Ty_>
class gc_core_ptr;
_GCPOINTER_DETAILS_END

template<typename Ty_>
class gc_ptr;
template<typename Ty_>
class gc_field_ptr;

#if !_GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
class gc_rref
{
public:
	explicit gc_rref(const Ty_& object) _GCPOINTER_NOEXCEPT;
	explicit gc_rref(Ty_& object) _GCPOINTER_NOEXCEPT;
	gc_rref(const gc_rref& object) _GCPOINTER_NOEXCEPT;
	~gc_rref();

public:
	gc_rref<Ty_>& operator=(const gc_rref& object) _GCPOINTER_DELETE;
	bool operator==(const gc_rref& object) const _GCPOINTER_NOEXCEPT;
	bool operator!=(const gc_rref& object) const _GCPOINTER_NOEXCEPT;

public:
	const Ty_& object() const _GCPOINTER_NOEXCEPT;
	Ty_& object() _GCPOINTER_NOEXCEPT;

private:
	Ty_& object_;
};

template<typename Ty_>
gc_rref<Ty_> gc_move(const Ty_& data) _GCPOINTER_NOEXCEPT;
template<typename Ty_>
gc_rref<Ty_> gc_move(Ty_& data) _GCPOINTER_NOEXCEPT;
#else
template<typename Ty_>
using gc_rref = Ty_&&;

template<typename Ty_>
gc_ptr<Ty_>&& gc_move(gc_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT;
template<typename Ty_>
gc_field_ptr<Ty_>&& gc_move(gc_field_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT;
#endif

_GCPOINTER_DETAILS_BEGIN
template<typename Ty_>
class gc_ptr_data _GCPOINTER_FINAL
{
	friend class gc_core_ptr<Ty_>;

private:
	gc_ptr_data();
	gc_ptr_data(std::size_t strong_reference_count);
	gc_ptr_data(const std::vector<gc_field_ptr<Ty_>*>& field_reference_count);
	gc_ptr_data(std::size_t strong_reference_count, const std::vector<gc_field_ptr<Ty_>*>& field_reference_count);
	gc_ptr_data(const gc_ptr_data& data) _GCPOINTER_DELETE;
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_ptr_data(gc_ptr_data&& data) _GCPOINTER_DELETE;
#endif
	~gc_ptr_data() _GCPOINTER_DEFAULT;

private:
	gc_ptr_data& operator=(const gc_ptr_data& data) _GCPOINTER_DELETE;
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_ptr_data& operator=(gc_ptr_data&& data) _GCPOINTER_DELETE;
#endif
	bool operator==(const gc_ptr_data& data) _GCPOINTER_DELETE;
	bool operator!=(const gc_ptr_data& data) _GCPOINTER_DELETE;

private:
	void inc_strong_reference_count() _GCPOINTER_NOEXCEPT;
	bool dec_strong_reference_count();

private:
	std::size_t strong_reference_count_;
	std::vector<gc_field_ptr<Ty_>*> field_reference_count_;
};

template<typename Ty_>
class gc_core_ptr _GCPOINTER_FINAL
{
	friend class gc_ptr<Ty_>;
	friend class gc_field_ptr<Ty_>;

private:
	gc_core_ptr() _GCPOINTER_NOEXCEPT;
	gc_core_ptr(Ty_* data);
	gc_core_ptr(const gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT;
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_core_ptr(gc_core_ptr&& ptr) _GCPOINTER_NOEXCEPT;
#else
	gc_core_ptr(gc_rref<gc_core_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT;
#endif
	~gc_core_ptr();

private:
	gc_core_ptr& operator=(const gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT;
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_core_ptr& operator=(gc_core_ptr&& ptr) _GCPOINTER_NOEXCEPT;
#else
	gc_core_ptr& operator=(gc_rref<gc_core_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT;
#endif
	bool operator==(const gc_core_ptr& ptr) _GCPOINTER_DELETE;
	bool operator!=(const gc_core_ptr& ptr) _GCPOINTER_DELETE;

private:
	void reset() _GCPOINTER_NOEXCEPT;
	void swap(gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT;
	
	bool empty() const _GCPOINTER_NOEXCEPT;
	Ty_* get() const _GCPOINTER_NOEXCEPT;

private:
	Ty_* data_;
	gc_ptr_data<Ty_>* mem_data_;
};
_GCPOINTER_DETAILS_END

template<typename Ty_>
class gc_ptr
{
public:
	typedef Ty_ element_type;
	typedef gc_field_ptr<Ty_> field_type;

public:
	gc_ptr() _GCPOINTER_NOEXCEPT;
	gc_ptr(Ty_* data);
	gc_ptr(const gc_ptr& ptr) _GCPOINTER_NOEXCEPT;
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_ptr(gc_ptr&& ptr) _GCPOINTER_NOEXCEPT;
#else
	gc_ptr(gc_rref<gc_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT;
#endif
	~gc_ptr() _GCPOINTER_DEFAULT;

public:
	gc_ptr& operator=(const gc_ptr& ptr) _GCPOINTER_NOEXCEPT;
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_ptr& operator=(gc_ptr&& ptr) _GCPOINTER_NOEXCEPT;
#else
	gc_ptr& operator=(gc_rref<gc_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT;
#endif
	bool operator==(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT;
	bool operator!=(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT;
	Ty_* operator->() const _GCPOINTER_NOEXCEPT;
	Ty_& operator*() const _GCPOINTER_NOEXCEPT;
	operator bool() const _GCPOINTER_NOEXCEPT;

public:
	void reset() _GCPOINTER_NOEXCEPT;
	void swap(gc_ptr& ptr) _GCPOINTER_NOEXCEPT;

	bool empty() const _GCPOINTER_NOEXCEPT;
	Ty_* get() const _GCPOINTER_NOEXCEPT;

private:
	_GCPOINTER_DETAILS::gc_core_ptr<Ty_> core_;
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

#if !_GCPOINTER_IS_CPLUSPLUS11
//
// details::gc_rref
//

template<typename Ty_>
gc_rref<Ty_>::gc_rref(const Ty_& object) _GCPOINTER_NOEXCEPT
	: object_(const_cast<Ty_&>(object))
{}
template<typename Ty_>
gc_rref<Ty_>::gc_rref(Ty_& object) _GCPOINTER_NOEXCEPT
	: object_(object)
{}
template<typename Ty_>
gc_rref<Ty_>::gc_rref(const gc_rref& object) _GCPOINTER_NOEXCEPT
	: object_(object.object_)
{}
template<typename Ty_>
gc_rref<Ty_>::~gc_rref()
{}

template<typename Ty_>
bool gc_rref<Ty_>::operator==(const gc_rref& object) const _GCPOINTER_NOEXCEPT
{
	return object_ == object.object_;
}
template<typename Ty_>
bool gc_rref<Ty_>::operator!=(const gc_rref& object) const _GCPOINTER_NOEXCEPT
{
	return object_ != object.object_;
}

template<typename Ty_>
const Ty_& gc_rref<Ty_>::object() const _GCPOINTER_NOEXCEPT
{
	return object_;
}
template<typename Ty_>
Ty_& gc_rref<Ty_>::object() _GCPOINTER_NOEXCEPT
{
	return object_;
}

template<typename Ty_>
gc_rref<Ty_> gc_move(const Ty_& data) _GCPOINTER_NOEXCEPT
{
	return gc_rref<Ty_>(data);
}
template<typename Ty_>
gc_rref<Ty_> gc_move(Ty_& data) _GCPOINTER_NOEXCEPT
{
	return gc_rref<Ty_>(data);
}
#else
template<typename Ty_>
gc_ptr<Ty_>&& gc_move(gc_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT
{
	return std::move(ptr);
}
template<typename Ty_>
gc_field_ptr<Ty_>&& gc_move(gc_field_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT
{
	return std::move(ptr);
}
#endif

//
// details::gc_ptr_data
//

_GCPOINTER_DETAILS_BEGIN
template<typename Ty_>
gc_ptr_data<Ty_>::gc_ptr_data()
	: strong_reference_count_(0)
{}
template<typename Ty_>
gc_ptr_data<Ty_>::gc_ptr_data(std::size_t strong_reference_count)
	: strong_reference_count_(strong_reference_count)
{}
template<typename Ty_>
gc_ptr_data<Ty_>::gc_ptr_data(const std::vector<gc_field_ptr<Ty_>*>& field_reference_count)
	: strong_reference_count_(0), field_reference_count_(field_reference_count)
{}
template<typename Ty_>
gc_ptr_data<Ty_>::gc_ptr_data(std::size_t strong_reference_count, const std::vector<gc_field_ptr<Ty_>*>& field_reference_count)
	: strong_reference_count_(strong_reference_count), field_reference_count_(field_reference_count)
{}
#if !_GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
gc_ptr_data<Ty_>::~gc_ptr_data()
{}
#endif

template<typename Ty_>
void gc_ptr_data<Ty_>::inc_strong_reference_count() _GCPOINTER_NOEXCEPT
{
	++strong_reference_count_;
}
template<typename Ty_>
bool gc_ptr_data<Ty_>::dec_strong_reference_count()
{
	if (!--strong_reference_count_)
	{
		// TODO
		return true;
	}
	
	return false;
}
_GCPOINTER_DETAILS_END

//
// details::gc_core_ptr
//

_GCPOINTER_DETAILS_BEGIN
template<typename Ty_>
gc_core_ptr<Ty_>::gc_core_ptr() _GCPOINTER_NOEXCEPT
	: data_(NULL), mem_data_(NULL)
{}
template<typename Ty_>
gc_core_ptr<Ty_>::gc_core_ptr(Ty_* data)
	: data_(data), mem_data_(new gc_ptr_data<Ty_>(1))
{}
template<typename Ty_>
gc_core_ptr<Ty_>::gc_core_ptr(const gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT
	: data_(ptr.data_), mem_data_(ptr.mem_data_)
{
	if (mem_data_)
	{
		mem_data_->inc_strong_reference_count();
	}
}
#if _GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
gc_core_ptr<Ty_>::gc_core_ptr(gc_core_ptr&& ptr) _GCPOINTER_NOEXCEPT
	: data_(ptr.data_), mem_data_(ptr.mem_data_)
{
	ptr.data_ = NULL;
	ptr.mem_data_ = NULL;
}
#else
template<typename Ty_>
gc_core_ptr<Ty_>::gc_core_ptr(gc_rref<gc_core_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT
	: data_(ptr.object().data_), mem_data_(ptr.object().mem_data_)
{
	ptr.object().data_ = NULL;
	ptr.object().mem_data_ = NULL;
}
#endif
template<typename Ty_>
gc_core_ptr<Ty_>::~gc_core_ptr()
{
	reset();
}

template<typename Ty_>
gc_core_ptr<Ty_>& gc_core_ptr<Ty_>::operator=(const gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT
{
	reset();

	data_ = ptr.data_;
	mem_data_ = ptr.mem_data_;

	if (mem_data_)
	{
		mem_data_->inc_strong_reference_count();
	}

	return *this;
}
#if _GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
gc_core_ptr<Ty_>& gc_core_ptr<Ty_>::operator=(gc_core_ptr&& ptr) _GCPOINTER_NOEXCEPT
{
	reset();

	data_ = ptr.data_;
	mem_data_ = ptr.mem_data_;

	ptr.data_ = NULL;
	ptr.mem_data_ = NULL;

	return *this;
}
#else
template<typename Ty_>
gc_core_ptr<Ty_>& gc_core_ptr<Ty_>::operator=(gc_rref<gc_core_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT
{
	reset();

	data_ = ptr.object().data_;
	mem_data_ = ptr.object().mem_data_;

	ptr.object().data_ = NULL;
	ptr.object().mem_data_ = NULL;

	return *this;
}
#endif

template<typename Ty_>
void gc_core_ptr<Ty_>::reset() _GCPOINTER_NOEXCEPT
{
	if (mem_data_)
	{
		if (mem_data_->dec_strong_reference_count())
		{
			delete mem_data_;
		}

		data_ = NULL;
		mem_data_ = NULL;
	}
}
template<typename Ty_>
void gc_core_ptr<Ty_>::swap(gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT
{
	std::swap(data_, ptr.data_);
	std::swap(mem_data_, ptr.mem_data_);
}

template<typename Ty_>
bool gc_core_ptr<Ty_>::empty() const _GCPOINTER_NOEXCEPT
{
	return mem_data_ == NULL;
}
template<typename Ty_>
Ty_* gc_core_ptr<Ty_>::get() const _GCPOINTER_NOEXCEPT
{
	return data_;
}
_GCPOINTER_DETAILS_END

//
// gc_ptr
//

template<typename Ty_>
gc_ptr<Ty_>::gc_ptr() _GCPOINTER_NOEXCEPT
{}
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(Ty_* data)
	: core_(data)
{}
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(const gc_ptr& ptr) _GCPOINTER_NOEXCEPT
	: core_(ptr.core_)
{}
#if _GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(gc_ptr&& ptr) _GCPOINTER_NOEXCEPT
	: core_(std::move(ptr.core_))
{}
#else
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(gc_rref<gc_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT
	: core_(gc_move(ptr.object().core_))
{}
template<typename Ty_>
gc_ptr<Ty_>::~gc_ptr()
{}
#endif

template<typename Ty_>
gc_ptr<Ty_>& gc_ptr<Ty_>::operator=(const gc_ptr& ptr) _GCPOINTER_NOEXCEPT
{
	core_ = ptr.core_;
	return *this;
}
#if _GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
gc_ptr<Ty_>& gc_ptr<Ty_>::operator=(gc_ptr&& ptr) _GCPOINTER_NOEXCEPT
{
	core_ = std::move(ptr.core_);
	return *this;
}
#else
template<typename Ty_>
gc_ptr<Ty_>& gc_ptr<Ty_>::operator=(gc_rref<gc_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT
{
	core_ = gc_move(ptr.object().core_);

	return *this;
}
#endif
template<typename Ty_>
Ty_* gc_ptr<Ty_>::operator->() const _GCPOINTER_NOEXCEPT
{
	return core_.get();
}
template<typename Ty_>
Ty_& gc_ptr<Ty_>::operator*() const _GCPOINTER_NOEXCEPT
{
	return *core_.get();
}
template<typename Ty_>
bool gc_ptr<Ty_>::operator==(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT
{
	return core_.mem_data_ == ptr.core_.mem_data_;
}
template<typename Ty_>
bool gc_ptr<Ty_>::operator!=(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT
{
	return core_.mem_data_ != ptr.core_.mem_data_;
}
template<typename Ty_>
gc_ptr<Ty_>::operator bool() const _GCPOINTER_NOEXCEPT
{
	return !empty();
}

template<typename Ty_>
void gc_ptr<Ty_>::reset() _GCPOINTER_NOEXCEPT
{
	core_.reset();
}
template<typename Ty_>
void gc_ptr<Ty_>::swap(gc_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT
{
	core_.swap(ptr.core_);
}

template<typename Ty_>
bool gc_ptr<Ty_>::empty() const _GCPOINTER_NOEXCEPT
{
	return core_.empty();
}
template<typename Ty_>
Ty_* gc_ptr<Ty_>::get() const _GCPOINTER_NOEXCEPT
{
	return core_.get();
}

#ifdef _GCPOINTER_HAS_NAMESPACE
}
#endif

#endif