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
///// Global Macros
/////////////////////////////////////////////////////////////////

#define _GCPOINTER_MAJOR 1ull
#define _GCPOINTER_MINOR 0ull

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
#	define _GCPOINTER_DELETE = delete
#	define _GCPOINTER_NOEXCEPT noexcept
#	define _GCPOINTER_FINAL final
#	define _GCPOINTER_OVERRIDE override
#	define _GCPOINTER_NULL nullptr
#else
#	define _GCPOINTER_DELETE
#	define _GCPOINTER_NOEXCEPT throw()
#	define _GCPOINTER_FINAL
#	define _GCPOINTER_OVERRIDE
#	define _GCPOINTER_NULL NULL
#endif

#ifdef _GCPOINTER_HAS_NAMESPACE
#	define _GCPOINTER_DETAILS details
#else
#	define _GCPOINTER_DETAILS gc_details
#endif
#define _GCPOINTER_DETAILS_BEGIN namespace _GCPOINTER_DETAILS {
#define _GCPOINTER_DETAILS_END }

#if !defined(_GCPOINTER_WINDOWS) && (defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__))
#	define _GCPOINTER_WINDOWS
#elif !defined(_GCPOINTER_POSIX) && (defined(__gnu_linux__) || defined(__unix__) || defined(__unix) || defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__)))
#	define _GCPOINTER_POSIX
#endif

/////////////////////////////////////////////////////////////////
///// Includes
/////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstddef>
#include <utility>

#if _GCPOINTER_IS_CPLUSPLUS11
#	include <functional>
#	include <type_traits>

#	ifdef _GCPOINTER_MULTITHREADING
#		include <atomic>
#		include <mutex>
#	endif
#endif

/////////////////////////////////////////////////////////////////
///// Declarations
/////////////////////////////////////////////////////////////////

#ifdef _GCPOINTER_HAS_NAMESPACE
namespace _GCPOINTER_HAS_NAMESPACE
{
#endif

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
	explicit operator const Ty_&() const _GCPOINTER_NOEXCEPT;
	explicit operator Ty_&() _GCPOINTER_NOEXCEPT;

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
using gc_rref = typename std::remove_reference<Ty_>::type&&;

template<typename Ty_>
#if _GCPOINTER_IS_CPLUSPLUS14
constexpr
#endif
typename std::remove_reference<Ty_>::type&& gc_move(Ty_&& data) _GCPOINTER_NOEXCEPT;
#endif

#ifdef _GCPOINTER_MULTITHREADING
_GCPOINTER_DETAILS_BEGIN
#	if _GCPOINTER_IS_CPLUSPLUS11
using mutex = std::mutex;
using mutex_guard = std::lock_guard<mutex>;
using atomic = std::atomic<std::size_t>;
#	else
class mutex
{
private:
	struct handle_t_;													// Impl

public:
	mutex();															// Impl
	mutex(const mutex& mutex) _GCPOINTER_DELETE;
	~mutex();															// Impl

public:
	mutex& operator=(const mutex& mutex) _GCPOINTER_DELETE;
	bool operator==(const mutex& mutex) _GCPOINTER_DELETE;
	bool operator!=(const mutex& mutex) _GCPOINTER_DELETE;

public:
	void lock();														// Impl
	bool try_lock();													// Impl
	void unlock();														// Impl

private:
	handle_t_* handle_;
};

class mutex_guard
{
public:
	mutex_guard(mutex& mutex);
	mutex_guard(const mutex_guard& guard) _GCPOINTER_DELETE;
	~mutex_guard();

public:
	mutex_guard& operator=(const mutex_guard& guard) _GCPOINTER_DELETE;
	bool operator==(const mutex_guard& guard) _GCPOINTER_DELETE;
	bool operator!=(const mutex_guard& guard) _GCPOINTER_DELETE;

private:
	mutex& mutex_;
};

class atomic
{
public:
	atomic() _GCPOINTER_NOEXCEPT;										// Impl
	atomic(std::size_t value) _GCPOINTER_NOEXCEPT;						// Impl
	atomic(const atomic& atomic) _GCPOINTER_DELETE;
	~atomic();															// Impl

public:
	atomic& operator=(const atomic& atomic) _GCPOINTER_DELETE;
	bool operator==(std::size_t value) const _GCPOINTER_NOEXCEPT;
	bool operator==(const atomic& atomic) const _GCPOINTER_NOEXCEPT;
	bool operator!=(std::size_t value) const _GCPOINTER_NOEXCEPT;
	bool operator!=(const atomic& atomic) const _GCPOINTER_NOEXCEPT;
	std::size_t operator++() _GCPOINTER_NOEXCEPT;						// Impl
	std::size_t operator--() _GCPOINTER_NOEXCEPT;						// Impl

private:
	volatile std::size_t value_;
#ifndef _GCPOINTER_WINDOWS
	mutex mutex_;
#endif
};
#	endif
_GCPOINTER_DETAILS_END
#endif

template<typename Ty_>
class gc_ptr;
template<typename Ty_>
class gc_weak_ptr;

_GCPOINTER_DETAILS_BEGIN
class gc_data_base
{
	template<typename Ty_>
	friend class gc_ptr;
	template<typename Ty_>
	friend class gc_weak_ptr;
	
public:
	gc_data_base(const gc_data_base& data) _GCPOINTER_DELETE;
	virtual inline ~gc_data_base();

protected:
	inline gc_data_base() _GCPOINTER_NOEXCEPT;

public:
	gc_data_base& operator=(const gc_data_base& data) _GCPOINTER_DELETE;
	bool operator==(const gc_data_base& data) _GCPOINTER_DELETE;
	bool operator!=(const gc_data_base& data) _GCPOINTER_DELETE;

protected:
	virtual void delete_data() _GCPOINTER_NOEXCEPT = 0;
	virtual void delete_this() _GCPOINTER_NOEXCEPT = 0;

private:
	inline void inc_strong_ref() _GCPOINTER_NOEXCEPT;
	inline void dec_strong_ref() _GCPOINTER_NOEXCEPT;
	inline void inc_weak_ref() _GCPOINTER_NOEXCEPT;
	inline void dec_weak_ref() _GCPOINTER_NOEXCEPT;

	inline std::size_t use_count() const _GCPOINTER_NOEXCEPT;

private:
#ifdef _GCPOINTER_MULTITHREADING
	_GCPOINTER_DETAILS::atomic strong_ref_;
	_GCPOINTER_DETAILS::atomic weak_ref_;

	_GCPOINTER_DETAILS::mutex strong_ref_mutex_;
#else
	std::size_t strong_ref_;
	std::size_t weak_ref_;
#endif
};

template<typename Ty_>
class gc_data _GCPOINTER_FINAL : public gc_data_base
{
	template<typename Ty2_>
	friend class gc_ptr;
	template<typename Ty2_>
	friend class gc_weak_ptr;

public:
	gc_data(const gc_data& data) _GCPOINTER_DELETE;
	virtual ~gc_data() _GCPOINTER_OVERRIDE;

private:
	gc_data() _GCPOINTER_NOEXCEPT;
	gc_data(Ty_* data) _GCPOINTER_NOEXCEPT;

public:
	gc_data& operator=(const gc_data& data) _GCPOINTER_DELETE;
	bool operator==(const gc_data& data) _GCPOINTER_DELETE;
	bool operator!=(const gc_data& data) _GCPOINTER_DELETE;

protected:
	virtual inline void delete_data() _GCPOINTER_NOEXCEPT _GCPOINTER_OVERRIDE;
	virtual inline void delete_this() _GCPOINTER_NOEXCEPT _GCPOINTER_OVERRIDE;

private:
	Ty_* data_;
};

template<typename Ty_, typename Deleter_>
class gc_data_deleter _GCPOINTER_FINAL : public gc_data_base
{
	template<typename Ty2_>
	friend class gc_ptr;
	template<typename Ty2_>
	friend class gc_weak_ptr;

public:
	gc_data_deleter(const gc_data_deleter& data) _GCPOINTER_DELETE;
	virtual ~gc_data_deleter() _GCPOINTER_OVERRIDE;

private:
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_data_deleter(Deleter_&& deleter);
	gc_data_deleter(Ty_* data, Deleter_&& deleter);
#else
	gc_data_deleter(Deleter_deleter);
	gc_data_deleter(Ty_* data, Deleter_ deleter);
#endif

public:
	gc_data_deleter& operator=(const gc_data_deleter& data) _GCPOINTER_DELETE;
	bool operator==(const gc_data_deleter& data) _GCPOINTER_DELETE;
	bool operator!=(const gc_data_deleter& data) _GCPOINTER_DELETE;

protected:
	virtual inline void delete_data() _GCPOINTER_NOEXCEPT _GCPOINTER_OVERRIDE;
	virtual inline void delete_this() _GCPOINTER_NOEXCEPT _GCPOINTER_OVERRIDE;

private:
	Ty_* data_;
	Deleter_ deleter_;
};
_GCPOINTER_DETAILS_END

template<typename Ty_>
class gc_ptr _GCPOINTER_FINAL
{
	template<typename Ty2_>
	friend class gc_ptr;
	template<typename Ty2_>
	friend class gc_weak_ptr;

public:
	typedef Ty_ element_type;
	typedef gc_weak_ptr<Ty_> weak_type;

public:
	gc_ptr() _GCPOINTER_NOEXCEPT;
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_ptr(std::nullptr_t) _GCPOINTER_NOEXCEPT;
	template<typename Deleter_>
	gc_ptr(std::nullptr_t, Deleter_&& deleter) _GCPOINTER_NOEXCEPT;
#endif
	explicit gc_ptr(Ty_* data);
#if _GCPOINTER_IS_CPLUSPLUS11
	template<typename Deleter_>
	gc_ptr(Ty_* data, Deleter_&& deleter);
#else
	template<typename Deleter_>
	gc_ptr(Ty_* data, Deleter_ deleter);
#endif
	gc_ptr(const gc_ptr& ptr) _GCPOINTER_NOEXCEPT;
	gc_ptr(gc_rref<gc_ptr> ptr) _GCPOINTER_NOEXCEPT;
	~gc_ptr();

public:
#if _GCPOINTER_IS_CPLUSPLUS11
	gc_ptr& operator=(std::nullptr_t) _GCPOINTER_NOEXCEPT;
#endif
	gc_ptr& operator=(const gc_ptr& ptr) _GCPOINTER_NOEXCEPT;
	gc_ptr& operator=(gc_rref<gc_ptr> ptr) _GCPOINTER_NOEXCEPT;
	bool operator==(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT;
	bool operator!=(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT;
	Ty_& operator[](std::size_t index) const _GCPOINTER_NOEXCEPT;
	Ty_* operator->() const _GCPOINTER_NOEXCEPT;
	Ty_& operator*() const _GCPOINTER_NOEXCEPT;
	operator bool() const _GCPOINTER_NOEXCEPT;

public:
	void reset() _GCPOINTER_NOEXCEPT;
	void swap(gc_ptr& other) _GCPOINTER_NOEXCEPT;

	std::size_t use_count() const _GCPOINTER_NOEXCEPT;
	bool expired() const _GCPOINTER_NOEXCEPT;
	Ty_* get() const _GCPOINTER_NOEXCEPT;

private:
	Ty_* data_;
	_GCPOINTER_DETAILS::gc_data_base* ref_;
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
	return &object_ == &object.object_;
}
template<typename Ty_>
bool gc_rref<Ty_>::operator!=(const gc_rref& object) const _GCPOINTER_NOEXCEPT
{
	return &object_ != &object.object_;
}
template<typename Ty_>
gc_rref<Ty_>::operator const Ty_&() const _GCPOINTER_NOEXCEPT
{
	return object_;
}
template<typename Ty_>
gc_rref<Ty_>::operator Ty_&() _GCPOINTER_NOEXCEPT
{
	return object_;
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
#if _GCPOINTER_IS_CPLUSPLUS14
constexpr
#endif
typename std::remove_reference<Ty_>::type&& gc_move(Ty_&& data) _GCPOINTER_NOEXCEPT
{
	return std::move(std::forward<Ty_>(data));
}
#endif

//
// details::gc_data_base
//

_GCPOINTER_DETAILS_BEGIN
gc_data_base::~gc_data_base()
{}

gc_data_base::gc_data_base() _GCPOINTER_NOEXCEPT
	: strong_ref_(1), weak_ref_(0)
{}

void gc_data_base::inc_strong_ref() _GCPOINTER_NOEXCEPT
{
#ifdef _GCPOINTER_MULTITHREADING
	mutex_guard guard(strong_ref_mutex_);
#endif

	++strong_ref_;
}
void gc_data_base::dec_strong_ref() _GCPOINTER_NOEXCEPT
{
	{
#ifdef _GCPOINTER_MULTITHREADING
		mutex_guard guard(strong_ref_mutex_);
#endif

		--strong_ref_;

		if (strong_ref_ == 0)
		{
			delete_data();
		}
	}

	if (strong_ref_ == 0 && weak_ref_ == 0)
	{
		delete_this();
	}
}
void gc_data_base::inc_weak_ref() _GCPOINTER_NOEXCEPT
{
	++weak_ref_;
}
void gc_data_base::dec_weak_ref() _GCPOINTER_NOEXCEPT
{
	--weak_ref_;

	if (strong_ref_ == 0 && weak_ref_ == 0)
	{
		delete_this();
	}
}

std::size_t gc_data_base::use_count() const _GCPOINTER_NOEXCEPT
{
	return strong_ref_;
}
_GCPOINTER_DETAILS_END

//
// details::gc_data
//

_GCPOINTER_DETAILS_BEGIN
template<typename Ty_>
gc_data<Ty_>::~gc_data()
{}

template<typename Ty_>
gc_data<Ty_>::gc_data() _GCPOINTER_NOEXCEPT
	: data_(_GCPOINTER_NULL)
{}
template<typename Ty_>
gc_data<Ty_>::gc_data(Ty_* data) _GCPOINTER_NOEXCEPT
	: data_(data)
{}

template<typename Ty_>
void gc_data<Ty_>::delete_data() _GCPOINTER_NOEXCEPT
{
	delete data_;
	data_ = _GCPOINTER_NULL;
}
template<typename Ty_>
void gc_data<Ty_>::delete_this() _GCPOINTER_NOEXCEPT
{
	delete this;
}
_GCPOINTER_DETAILS_END

//
// details::gc_data_deleter
//

_GCPOINTER_DETAILS_BEGIN
#if _GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_, typename Deleter_>
gc_data_deleter<Ty_, Deleter_>::gc_data_deleter(Deleter_&& deleter)
	: data_(_GCPOINTER_NULL), deleter_(std::forward<Deleter_>(deleter))
{}
template<typename Ty_, typename Deleter_>
gc_data_deleter<Ty_, Deleter_>::gc_data_deleter(Ty_* data, Deleter_&& deleter)
	: data_(data), deleter_(std::forward<Deleter_>(deleter))
{}
#else
template<typename Ty_, typename Deleter_>
gc_data_deleter<Ty_, Deleter_>::gc_data_deleter(Deleter_ deleter)
	: data_(_GCPOINTER_NULL), deleter_(deleter)
{}
template<typename Ty_, typename Deleter_>
gc_data_deleter<Ty_, Deleter_>::gc_data_deleter(Ty_* data, Deleter_ deleter)
	: data_(data), deleter_(deleter)
{}
#endif

template<typename Ty_, typename Deleter_>
void gc_data_deleter<Ty_, typename Deleter_>::delete_data() _GCPOINTER_NOEXCEPT
{
	deleter_(data_);
	data_ = _GCPOINTER_NULL;
}
template<typename Ty_, typename Deleter_>
void gc_data_deleter<Ty_, typename Deleter_>::delete_this() _GCPOINTER_NOEXCEPT
{
	delete this;
}
_GCPOINTER_DETAILS_END

//
// gc_ptr
//

template<typename Ty_>
gc_ptr<Ty_>::gc_ptr() _GCPOINTER_NOEXCEPT
	: data_(_GCPOINTER_NULL), ref_(_GCPOINTER_NULL)
{}
#if _GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(std::nullptr_t) _GCPOINTER_NOEXCEPT
	: data_(_GCPOINTER_NULL), ref_(_GCPOINTER_NULL)
{}
template<typename Ty_>
template<typename Deleter_>
gc_ptr<Ty_>::gc_ptr(std::nullptr_t, Deleter_&&) _GCPOINTER_NOEXCEPT
	: data_(_GCPOINTER_NULL), ref_(_GCPOINTER_NULL)
{}
#endif
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(Ty_* data)
{
	data_ = data;

	if (data)
	{
		ref_ = new _GCPOINTER_DETAILS::gc_data<Ty_>(data);
	}
}
template<typename Ty_>
template<typename Deleter_>
#if _GCPOINTER_IS_CPLUSPLUS11
gc_ptr<Ty_>::gc_ptr(Ty_* data, Deleter_&& deleter)
{
	if (data)
	{
		data_ = new _GCPOINTER_DETAILS::gc_data_deleter<Ty_, Deleter_>(data, std::forward<Deleter_>(deleter));
	}
}
#else
gc_ptr<Ty_>::gc_ptr(Ty_* data, Deleter_ deleter)
{
	if (data)
	{
		data_ = new _GCPOINTER_DETAILS::gc_data_deleter<Ty_, Deleter_>(data, deleter);
	}
}
#endif
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(const gc_ptr& ptr) _GCPOINTER_NOEXCEPT
	: data_(ptr.data_), ref_(ptr.ref_)
{
	if (ref_)
	{
		ref_->inc_strong_ref();
	}
}
template<typename Ty_>
gc_ptr<Ty_>::gc_ptr(gc_rref<gc_ptr> ptr) _GCPOINTER_NOEXCEPT
#if _GCPOINTER_IS_CPLUSPLUS11
	: data_(ptr.data_), ref_(ptr.ref_)
#else
	: data_(ptr.object().data_), ref_(ptr.object().ref_)
#endif
{
#if _GCPOINTER_IS_CPLUSPLUS11
	ptr.data_ = _GCPOINTER_NULL;
	ptr.ref_ = _GCPOINTER_NULL;
#else
	ptr.object().data_ = _GCPOINTER_NULL;
	ptr.object().ref_ = _GCPOINTER_NULL;
#endif
}
template<typename Ty_>
gc_ptr<Ty_>::~gc_ptr()
{
	reset();
}

#if _GCPOINTER_IS_CPLUSPLUS11
template<typename Ty_>
gc_ptr<Ty_>& gc_ptr<Ty_>::operator=(std::nullptr_t) _GCPOINTER_NOEXCEPT
{
	reset();

	return *this;
}
#endif
template<typename Ty_>
gc_ptr<Ty_>& gc_ptr<Ty_>::operator=(const gc_ptr& ptr) _GCPOINTER_NOEXCEPT
{
	reset();

	data_ = ptr.data_;
	ref_ = ptr.ref_;

	if (ref_)
	{
		ref_->inc_strong_ref();
	}

	return *this;
}
template<typename Ty_>
gc_ptr<Ty_>& gc_ptr<Ty_>::operator=(gc_rref<gc_ptr<Ty_> > ptr) _GCPOINTER_NOEXCEPT
{
	reset();

#if _GCPOINTER_IS_CPLUSPLUS11
	data_ = ptr.data_;
	ref_ = ptr.ref_;

	ptr.data_ = _GCPOINTER_NULL;
	ptr.ref_ = _GCPOINTER_NULL;
#else
	data_ = ptr.object().data_;
	ref_ = ptr.object().ref_;

	ptr.object().data_ = _GCPOINTER_NULL;
	ptr.object().ref_ = _GCPOINTER_NULL;
#endif

	return *this;
}
template<typename Ty_>
bool gc_ptr<Ty_>::operator==(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT
{
	return data_ == ptr.data_;
}
template<typename Ty_>
bool gc_ptr<Ty_>::operator!=(const gc_ptr& ptr) const _GCPOINTER_NOEXCEPT
{
	return data_ != ptr.data_;
}
template<typename Ty_>
Ty_& gc_ptr<Ty_>::operator[](std::size_t index) const _GCPOINTER_NOEXCEPT
{
	return get()[index];
}
template<typename Ty_>
Ty_* gc_ptr<Ty_>::operator->() const _GCPOINTER_NOEXCEPT
{
	return get();
}
template<typename Ty_>
Ty_& gc_ptr<Ty_>::operator*() const _GCPOINTER_NOEXCEPT
{
	return *get();
}
template<typename Ty_>
gc_ptr<Ty_>::operator bool() const _GCPOINTER_NOEXCEPT
{
	return !expired();
}

template<typename Ty_>
void gc_ptr<Ty_>::reset() _GCPOINTER_NOEXCEPT
{
	if (ref_)
	{
		ref_->dec_strong_ref();
	}

	data_ = _GCPOINTER_NULL;
	ref_ = _GCPOINTER_NULL;
}
template<typename Ty_>
void gc_ptr<Ty_>::swap(gc_ptr& other) _GCPOINTER_NOEXCEPT
{
	std::swap(data_, other.data_);
	std::swap(ref_, other.ref_);
}

template<typename Ty_>
std::size_t gc_ptr<Ty_>::use_count() const _GCPOINTER_NOEXCEPT
{
	return ref_ ? ref_->use_count() : 0;
}
template<typename Ty_>
bool gc_ptr<Ty_>::expired() const _GCPOINTER_NOEXCEPT
{
	return use_count() == 0;
}
template<typename Ty_>
Ty_* gc_ptr<Ty_>::get() const _GCPOINTER_NOEXCEPT
{
	return data_;
}

#ifdef _GCPOINTER_HAS_NAMESPACE
}
#endif

#endif