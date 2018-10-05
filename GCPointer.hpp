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

/////////////////////////////////////////////////////////////////
///// Declarations
/////////////////////////////////////////////////////////////////

#ifdef _GCPOINTER_HAS_NAMESPACE
namespace _GCPOINTER_HAS_NAMESPACE
{
#endif

template<typename Ty_>
class _gc_core_ptr;
template<typename Ty_>
class gc_ptr;
template<typename Ty_>
class gc_field_ptr;

#if !_GCPOINTER_IS_CPLUSPLUS11
template<bool Cond_>
struct _gc_enable_if
{
	typedef void type;
};
template<>
struct _gc_enable_if<false>
{};

template<typename Ty_>
struct _gc_is_gc_ptr
{
	enum
	{
		value = 0
	};
};
template<typename Ty_>
struct _gc_is_gc_ptr<gc_ptr<Ty_>>
{
	enum
	{
		value = 1
	};
};
template<typename Ty_>
struct _gc_is_gc_ptr<gc_field_ptr<Ty_>>
{
	enum
	{
		value = 1
	};
};
template<typename Ty_>
struct _gc_is_gc_ptr<_gc_core_ptr<Ty_>>
{
	enum
	{
		value = 1
	};
};

template<typename Ty_, typename = void>
class _gc_moveable_object;

template<typename Ty_>
class _gc_moveable_object<Ty_, typename _gc_enable_if<_gc_is_gc_ptr<Ty_>::value>::type>
{
public:
	_gc_moveable_object(Ty_& object) _GCPOINTER_NOEXCEPT;
	_gc_moveable_object(const _gc_moveable_object& object) _GCPOINTER_NOEXCEPT;
	~_gc_moveable_object();

public:
	_gc_moveable_object<Ty_>& operator=(const _gc_moveable_object& object) _GCPOINTER_DELETE;
	bool operator==(const _gc_moveable_object& object) const _GCPOINTER_NOEXCEPT;
	bool operator!=(const _gc_moveable_object& object) const _GCPOINTER_NOEXCEPT;

public:
	const Ty_& object() const _GCPOINTER_NOEXCEPT;
	Ty_& object() _GCPOINTER_NOEXCEPT;

private:
	Ty_& object_;
};

template<typename Ty_>
_gc_moveable_object<_gc_core_ptr<Ty_>> gc_move(_gc_core_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT;
template<typename Ty_>
_gc_moveable_object<gc_ptr<Ty_>> gc_move(gc_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT;
template<typename Ty_>
_gc_moveable_object<gc_field_ptr<Ty_>> gc_move(gc_field_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT;
#else
template<typename Ty_>
gc_ptr<Ty_>&& gc_move(gc_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT;
template<typename Ty_>
gc_field_ptr<Ty_>&& gc_move(gc_field_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT;
#endif

template<typename Ty_>
class _gc_ptr_data _GCPOINTER_FINAL
{
	friend class _gc_core_ptr<Ty_>;

private:
	_gc_ptr_data();
	_gc_ptr_data(std::size_t strong_reference_count);
	_gc_ptr_data(const std::vector<gc_field_ptr<Ty_>*>& field_reference_count);
	_gc_ptr_data(std::size_t strong_reference_count, const std::vector<gc_field_ptr<Ty_>*>& field_reference_count);
	_gc_ptr_data(const _gc_ptr_data& data) _GCPOINTER_DELETE;
#if _GCPOINTER_IS_CPLUSPLUS11
	_gc_ptr_data(_gc_ptr_data&& data) _GCPOINTER_DELETE;
#endif
	~_gc_ptr_data() _GCPOINTER_DEFAULT;

private:
	_gc_ptr_data& operator=(const _gc_ptr_data& data) _GCPOINTER_DELETE;
#if _GCPOINTER_IS_CPLUSPLUS11
	_gc_ptr_data& operator=(_gc_ptr_data&& data) _GCPOINTER_DELETE;
#endif
	bool operator==(const _gc_ptr_data& data) _GCPOINTER_DELETE;
	bool operator!=(const _gc_ptr_data& data) _GCPOINTER_DELETE;

private:
	void inc_strong_reference_count() _GCPOINTER_NOEXCEPT;
	bool dec_strong_reference_count();

private:
	std::size_t strong_reference_count_;
	std::vector<gc_field_ptr<Ty_>*> field_reference_count_;
};

template<typename Ty_>
class _gc_core_ptr _GCPOINTER_FINAL
{
	friend class gc_ptr<Ty_>;
	friend class gc_field_ptr<Ty_>;

private:
	_gc_core_ptr() _GCPOINTER_NOEXCEPT;
	_gc_core_ptr(Ty_* data);
	_gc_core_ptr(const _gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT;
#if _GCPOINTER_IS_CPLUSPLUS11
	_gc_core_ptr(_gc_core_ptr&& ptr) _GCPOINTER_NOEXCEPT;
#else
	_gc_core_ptr(_gc_moveable_object<_gc_core_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT;
#endif
	~_gc_core_ptr();

private:
	_gc_core_ptr& operator=(const _gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT;
#if _GCPOINTER_IS_CPLUSPLUS11
	_gc_core_ptr& operator=(_gc_core_ptr&& ptr) _GCPOINTER_NOEXCEPT;
#else
	_gc_core_ptr& operator=(_gc_moveable_object<_gc_core_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT;
#endif
	bool operator==(const _gc_core_ptr& ptr) _GCPOINTER_DELETE;
	bool operator!=(const _gc_core_ptr& ptr) _GCPOINTER_DELETE;

private:
	void _delete();

private:
	Ty_* data_;
	_gc_ptr_data<Ty_>* mem_data_;
};

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
	gc_ptr(_gc_moveable_object<gc_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT;
#endif
	~gc_ptr() _GCPOINTER_DEFAULT;

public:
	gc_ptr& operator=(const gc_ptr& ptr) _GCPOINTER_NOEXCEPT;
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_ptr& operator=(gc_ptr&& ptr) _GCPOINTER_NOEXCEPT;
#else
	gc_ptr& operator=(_gc_moveable_object<gc_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT;
#endif
	bool operator==(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT;
	bool operator!=(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT;

private:
	_gc_core_ptr<Ty_> core_;
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
// _gc_moveable_object
//

template<typename Ty_>
_gc_moveable_object<Ty_, typename _gc_enable_if<_gc_is_gc_ptr<Ty_>::value>::type>::_gc_moveable_object(Ty_& object) _GCPOINTER_NOEXCEPT
	: object_(object)
{}
template<typename Ty_>
_gc_moveable_object<Ty_, typename _gc_enable_if<_gc_is_gc_ptr<Ty_>::value>::type>::_gc_moveable_object(const _gc_moveable_object& object) _GCPOINTER_NOEXCEPT
	: object_(object.object_)
{}
template<typename Ty_>
_gc_moveable_object<Ty_, typename _gc_enable_if<_gc_is_gc_ptr<Ty_>::value>::type>::~_gc_moveable_object()
{}

template<typename Ty_>
bool _gc_moveable_object<Ty_, typename _gc_enable_if<_gc_is_gc_ptr<Ty_>::value>::type>::operator==(const _gc_moveable_object& object) const _GCPOINTER_NOEXCEPT
{
	return object_ == object.object_;
}
template<typename Ty_>
bool _gc_moveable_object<Ty_, typename _gc_enable_if<_gc_is_gc_ptr<Ty_>::value>::type>::operator!=(const _gc_moveable_object& object) const _GCPOINTER_NOEXCEPT
{
	return object_ != object.object_;
}

template<typename Ty_>
const Ty_& _gc_moveable_object<Ty_, typename _gc_enable_if<_gc_is_gc_ptr<Ty_>::value>::type>::object() const _GCPOINTER_NOEXCEPT
{
	return object_;
}
template<typename Ty_>
Ty_& _gc_moveable_object<Ty_, typename _gc_enable_if<_gc_is_gc_ptr<Ty_>::value>::type>::object() _GCPOINTER_NOEXCEPT
{
	return object_;
}

template<typename Ty_>
_gc_moveable_object<_gc_core_ptr<Ty_>> gc_move(_gc_core_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT
{
	return ptr;
}
template<typename Ty_>
_gc_moveable_object<gc_ptr<Ty_>> gc_move(gc_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT
{
	return ptr;
}
template<typename Ty_>
_gc_moveable_object<gc_field_ptr<Ty_>> gc_move(gc_field_ptr<Ty_>& ptr) _GCPOINTER_NOEXCEPT
{
	return ptr;
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
void _gc_ptr_data<Ty_>::inc_strong_reference_count() _GCPOINTER_NOEXCEPT
{
	++strong_reference_count_;
}
template<typename Ty_>
bool _gc_ptr_data<Ty_>::dec_strong_reference_count()
{
	if (!--strong_reference_count_)
	{
		// TODO
		return true;
	}
	
	return false;
}

//
// _gc_core_ptr
//

template<typename Ty_>
_gc_core_ptr<Ty_>::_gc_core_ptr() _GCPOINTER_NOEXCEPT
	: data_(NULL), mem_data_(NULL)
{}
template<typename Ty_>
_gc_core_ptr<Ty_>::_gc_core_ptr(Ty_* data)
	: data_(data), mem_data_(new _gc_ptr_data<Ty_>(1))
{}
template<typename Ty_>
_gc_core_ptr<Ty_>::_gc_core_ptr(const _gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT
	: data_(ptr.data_), mem_data_(ptr.mem_data_)
{
	if (mem_data_)
	{
		mem_data_->inc_strong_reference_count();
	}
}
#if _GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
_gc_core_ptr<Ty_>::_gc_core_ptr(_gc_core_ptr&& ptr) _GCPOINTER_NOEXCEPT
	: data_(ptr.data_), mem_data_(ptr.mem_data_)
{
	ptr.data_ = NULL;
	ptr.mem_data_ = NULL;
}
#else
template<typename Ty_>
_gc_core_ptr<Ty_>::_gc_core_ptr(_gc_moveable_object<_gc_core_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT
	: data_(ptr.object().data_), mem_data_(ptr.object().mem_data_)
{
	ptr.object().data_ = NULL;
	ptr.object().mem_data_ = NULL;
}
#endif
template<typename Ty_>
_gc_core_ptr<Ty_>::~_gc_core_ptr()
{
	_delete();
}

template<typename Ty_>
_gc_core_ptr<Ty_>& _gc_core_ptr<Ty_>::operator=(const _gc_core_ptr& ptr) _GCPOINTER_NOEXCEPT
{
	_delete();

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
_gc_core_ptr<Ty_>& _gc_core_ptr<Ty_>::operator=(_gc_core_ptr&& ptr) _GCPOINTER_NOEXCEPT
{
	_delete();

	data_ = ptr.data_;
	mem_data_ = ptr.mem_data_;

	ptr.data_ = NULL;
	ptr.mem_data_ = NULL;

	return *this;
}
#else
template<typename Ty_>
_gc_core_ptr<Ty_>& _gc_core_ptr<Ty_>::operator=(_gc_moveable_object<_gc_core_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT
{
	_delete();

	data_ = ptr.object().data_;
	mem_data_ = ptr.object().mem_data_;

	ptr.object().data_ = NULL;
	ptr.object().mem_data_ = NULL;

	return *this;
}
#endif

template<typename Ty_>
void _gc_core_ptr<Ty_>::_delete()
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
gc_ptr<Ty_>::gc_ptr(_gc_moveable_object<gc_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT
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
gc_ptr<Ty_>& gc_ptr<Ty_>::operator=(_gc_moveable_object<gc_ptr<Ty_>> ptr) _GCPOINTER_NOEXCEPT
{
	core_ = gc_move(ptr.object().core_);

	return *this;
}
#endif
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

#ifdef _GCPOINTER_HAS_NAMESPACE
}
#endif

#endif