#define _GCPOINTER_INTERNAL
#include "GCPointer.hpp"

#if !_GCPOINTER_IS_CPLUSPLUS11 && defined(_GCPOINTER_MULTITHREADING)

#if defined(_GCPOINTER_WINDOWS)
#	include <Windows.h>
#elif defined(_GCPOINTER_POSIX)
#	include <pthread.h>
#endif

_GCPOINTER_DETAILS_BEGIN
template<bool Cond_, typename Ty_ = void>
struct enable_if
{
	typedef Ty_ type;
};
template<typename Ty_>
struct enable_if<false, Ty_>
{};

#if defined(_GCPOINTER_WINDOWS) || defined(_GCPOINTER_POSIX)
//
// details::atomic
//

atomic::atomic() _GCPOINTER_NOEXCEPT
	: value_(0)
{}
atomic::atomic(std::size_t value) _GCPOINTER_NOEXCEPT
	: value_(value)
{}
atomic::~atomic()
{}
#endif

#if defined(_GCPOINTER_WINDOWS)
//
// details::mutex
//

struct mutex::handle_t_
{
	HANDLE handle;
};

mutex::mutex()
{
	handle_ = new handle_t_();
	handle_->handle = CreateMutex(NULL, FALSE, NULL);
}
mutex::~mutex()
{
	CloseHandle(handle_->handle);

	delete handle_;
	handle_ = NULL;
}

void mutex::lock()
{
	WaitForSingleObject(handle_->handle, INFINITE);
}
bool mutex::try_lock()
{
	return WaitForSingleObject(handle_->handle, 0) == WAIT_OBJECT_0;
}
void mutex::unlock()
{
	ReleaseMutex(handle_->handle);
}

//
// details::atomic
//

template<typename Dummy_>
static typename enable_if<sizeof(Dummy_) == 4, std::size_t>::type inc(volatile std::size_t* value) _GCPOINTER_NOEXCEPT
{
	return InterlockedIncrement(value);
}
template<typename Dummy_>
static typename enable_if<sizeof(Dummy_) == 8, std::size_t>::type inc(volatile std::size_t* value) _GCPOINTER_NOEXCEPT
{
	return InterlockedIncrement64(reinterpret_cast<volatile LONG64*>(value));
}
template<typename Dummy_>
static typename enable_if<sizeof(Dummy_) == 4, std::size_t>::type dec(volatile std::size_t* value) _GCPOINTER_NOEXCEPT
{
	return InterlockedDecrement(value);
}
template<typename Dummy_>
static typename enable_if<sizeof(Dummy_) == 8, std::size_t>::type dec(volatile std::size_t* value) _GCPOINTER_NOEXCEPT
{
	return InterlockedDecrement64(reinterpret_cast<volatile LONG64*>(value));
}

std::size_t atomic::operator++() _GCPOINTER_NOEXCEPT
{
	return inc<std::size_t>(&value_);
}
std::size_t atomic::operator--() _GCPOINTER_NOEXCEPT
{
	return dec<std::size_t>(&value_);
}
#elif defined(_GCPOINTER_POSIX)
//
// details::mutex
//

struct mutex::handle_t_
{
	pthread_mutex_t handle;
};

mutex::mutex()
{
	handle_ = new handle_t_();
	pthread_mutex_init(&handle_->handle, NULL);
}
mutex::~mutex()
{
	pthread_mutex_destroy(&handle_->handle);
	
	delete handle_;
	handle_ = NULL;
}

void mutex::lock()
{
	pthread_mutex_lock(&handle_->handle);
}
bool mutex::try_lock()
{
	return pthread_mutex_trylock(&handle_->handle) == 0;
}
void mutex::unlock()
{
	pthread_mutex_unlock(&handle_->handle);
}

//
// details::atomic
//

std::size_t atomic::operator++() _GCPOINTER_NOEXCEPT
{
	mutex_guard guard(mutex_);

	return ++value_;
}
std::size_t atomic::operator--() _GCPOINTER_NOEXCEPT
{
	mutex_guard guard(mutex_);

	return --value_;
}
#endif
_GCPOINTER_DETAILS_END

#endif