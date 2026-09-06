#pragma once

#include "ImGuiAPI.h"

enum class FontType
{
	Daedric = 0,
	Dragon = 1,
	Dwemer = 2,
	Falmer = 3,
	Mage = 4,
	NotoSansJP = 5,
	MainTitle = 6,
	HeaderFont = 7,
	ButtonFont = 8,
	SettingsFont = 9,
	SmallFont = 10,
	MinorMessageFont = 11,
	MajorMessageFont = 12,
};

enum class WidgetType
{
	IWidgetMinorMessageQueue = 1,
	IWidgetMajorMessageQueue = 2,
	IWidgetPanel = 3,
	IWidgetTexture = 4,
	IWidgetTextureAndText = 5,
	IWidgetText = 6,
	IWidgetActorPanel = 7,
	IWidgetInfoBar = 10,
	IWidgetCooldown = 20,
};

#ifndef UTIL_STABLE_CLASSES
#	define UTIL_STABLE_CLASSES 1

extern "C"
{
	__declspec(dllimport) void* __stdcall GetProcessHeap();
	__declspec(dllimport) void* __stdcall HeapAlloc(void* hHeap, unsigned long dwFlags, std::size_t dwBytes);
	__declspec(dllimport) int __stdcall HeapFree(void* hHeap, unsigned long dwFlags, void* lpMem);
}
#	pragma comment(lib, "kernel32.lib")

namespace util
{
	template <class T>
	struct heap_allocator
	{
	private:
		static void dealloc_intern(void* p)
		{
			HeapFree(GetProcessHeap(), 0, p);
		}

	public:
		//std::function<decltype(dealloc_intern)> deallocator;

		using value_type = T;

		heap_allocator() noexcept
		{
			//deallocator = &dealloc_intern;
		}
		template <class U>
		heap_allocator(const heap_allocator<U>& other) noexcept
		{
			//deallocator = other.deallocator;
		}

		T* allocate(std::size_t n)
		{
			void* p = HeapAlloc(GetProcessHeap(), 0, n * sizeof(T));
			if (!p)
				throw std::bad_alloc();
			return static_cast<T*>(p);
		}
		void* malloc(std::size_t n)
		{
			void* p = HeapAlloc(GetProcessHeap(), 0, n);
			if (!p)
				throw std::bad_alloc();
			return p;
		}
		void dealloc(void* p)
		{
			//deallocator(p);
			HeapFree(GetProcessHeap(), 0, p);
		}
		void deallocate(T* p, std::size_t) noexcept
		{
			//deallocator(p);
			HeapFree(GetProcessHeap(), 0, p);
		}

		// Boost.Container's allocator interface needs a bit more than std::allocator;
		// see boost::container::allocator_traits — it fills in defaults for
		// construct/destroy/max_size/propagate_on_* if you don't specify them.

		friend bool operator==(const heap_allocator&, const heap_allocator&) noexcept { return true; }
		friend bool operator!=(const heap_allocator&, const heap_allocator&) noexcept { return false; }
	};

	using string = boost::container::basic_string<char, std::char_traits<char>, heap_allocator<char>>;

	struct message
	{
		/// <summary>
		/// small texture displayed before the text
		/// </summary>
		string smallTexturePath = "";
		/// <summary>
		/// the actual message text
		/// </summary>
		string text = "";
	};

	template <class T>
	using vector = boost::container::vector<T, heap_allocator<T>>;
	template <class T>
	using set = boost::container::set<T, std::less<T>, heap_allocator<T>>;

	namespace synchronisation
	{

		/// <summary>
		/// Spinlock aka. Busy Waiting
		/// </summary>
		class Spinlock
		{
			std::atomic_flag* _flag;

		public:
			Spinlock(std::atomic_flag& flag)
			{
				_flag = &flag;
				while (flag.test_and_set(std::memory_order_acquire));
			}

			Spinlock(const Spinlock&) = delete;

			~Spinlock()
			{
				_flag->clear(std::memory_order_release);
			}
		};
	}

	// ------------------------- Shared Ptr -------------------------
	// The following code is based upon microsofts implementation
	// of shared and weak pointers. It is however not an exact copy.
	// --------------------------------------------------------------

	class
		/// ABI cannot cope without vtable
		//#	if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
		//		__declspec(novtable)
		//#	endif
		control_block
	{
	public:
		//std::atomic<long> _ref_count = 1;
		//std::atomic<long> _weak_count = 1;
		int16_t _ref_count = 1;
		int16_t _weak_count = 1;
		std::atomic_flag _flag = ATOMIC_FLAG_INIT;
		heap_allocator<void*>* _allocatorCRTL = nullptr;
		heap_allocator<void*>* _allocatorREF = nullptr;
		bool _destroyed = false;

		long Incref()
		{
			synchronisation::Spinlock guard(_flag);
			return ++_ref_count;
		}
		long Decref()
		{
			synchronisation::Spinlock guard(_flag);
			return --_ref_count;
		}
		long Incweak()
		{
			synchronisation::Spinlock guard(_flag);
			return ++_weak_count;
		}
		long Decweak()
		{
			synchronisation::Spinlock guard(_flag);
			return --_weak_count;
		}

		bool Incref_nz()
		{
			synchronisation::Spinlock guard(_flag);
			if (_ref_count > 0) {
				++_ref_count;
				return true;
			}
			return false;
		}

		bool Incweak_nz()
		{
			synchronisation::Spinlock guard(_flag);
			if (_ref_count > 0) {
				++_weak_count;
				return true;
			}
			return false;
		}

		virtual void Destroy() = 0;
		virtual void Delete() = 0;

		virtual ~control_block()
		{
		}
	};

	template <class T>
	class
		//#	if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
		//		__declspec(novtable)
		//#	endif
		control_blockT : public control_block
	{
	public:
		T* _ref = nullptr;

		virtual void Destroy() override
		{
			if (_allocatorREF)
				_allocatorREF->dealloc(_ref);
			//else
			//delete obj;
		}
		virtual void Delete() override
		{
			_allocatorCRTL->dealloc((void*)this);
		}
	};

	template <class T>
	class
		//#	if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
		//		__declspec(novtable)
		//#	endif
		control_blockTA : public control_block
	{
	public:
		T* _ref = nullptr;

		virtual void Destroy() override
		{
			SKSE::log::info("Destroy {}", (uintptr_t)_ref);
			assert(_ref_count == 0);
			assert(!_destroyed);
			_destroyed = true;
			_ref->~T();
		}
		virtual void Delete() override
		{
			_allocatorCRTL->dealloc((void*)this);
		}
	};

	template <typename T>
	class shared_ptr;
	template <typename T>
	class weak_ptr;

	template <class T>
	class PtrBase
	{
	private:
		T* _ref = nullptr;
		control_block* _crtl = nullptr;

	public:
		long use_count() const noexcept
		{
			return _crtl ? _crtl->_ref_count : 0;
		}
		long weak_count() const noexcept
		{
			return _crtl ? _crtl->_weak_count : 0;
		}

		bool less(const PtrBase& other) noexcept
		{
			return _crtl < other._crtl;
		}

		PtrBase(const PtrBase&) = delete;
		PtrBase& operator=(const PtrBase&) = delete;

	protected:
		T* get() const noexcept
		{
			return _ref;
		}

		void* getCrtl() const noexcept
		{
			return _crtl;
		}

		constexpr PtrBase() noexcept = default;

		template <class Y>
		void move(PtrBase<Y>&& other) noexcept
		{
			_ref = other._ref;
			_crtl = other._crtl;

			other._ref = nullptr;
			other._crtl = nullptr;
		}
		template <class Y>
		void WeakMove(PtrBase<Y>&& other) noexcept
		{
			_crtl = other._crtl;
			other._crtl = nullptr;
			if (_crtl && _crtl->Incref_nz()) {
				_ref = other._ref;
				_crtl->Decref();
			}
			other._ref = nullptr;
		}

		template <class Y>
		void copy(const shared_ptr<Y>& other) noexcept
		{
			other.Incref();
			_ref = other._ref;
			_crtl = other._crtl;
		}

		template <class Y>
		bool ConstructFromWeak(const weak_ptr<Y>& other) noexcept
		{
			if (other._crtl && other._crtl->Incref_nz()) {
				_ref = other._ref;
				_crtl = other._crtl;
				return true;
			}
			return false;
		}

		template <class Y>
		void AliasConstruct(const shared_ptr<Y>& _Other, T* _Px) noexcept
		{
			// implement shared_ptr's aliasing ctor
			_Other.Incref();

			_ref = _Px;
			_crtl = _Other._crtl;
		}

		template <class Y>
		void AliasMoveConstruct(shared_ptr<Y>&& _Other, T* _Px) noexcept
		{
			// implement shared_ptr's aliasing move ctor
			_ref = _Px;
			_crtl = _Other._crtl;

			_Other._ref = nullptr;
			_Other._crtl = nullptr;
		}
		long Incref() const noexcept
		{
			if (_crtl)
				return _crtl->Incref();
			return -1;
		}

		long Decref() noexcept
		{
			if (_crtl) {
				long ret = 1;
				if (ret = _crtl->Decref(); ret == 0) {
					// we have deleted the last shared_ptr instance and can delete the underlying poínter
					_crtl->Destroy();
					_ref = nullptr;
					Decweak();
				}
				return ret;
			}
			return -1;
		}

		void ConstructWeak(const PtrBase& other) noexcept
		{
			if (other._crtl && other._crtl->Incweak()) {
				_crtl = other._crtl;
				if (other._crtl->Incref_nz()) {
					_ref = other._ref;
					other._crtl->Decref();
				}
			}
		}

		long Incweak() const noexcept
		{
			if (_crtl)
				return _crtl->Incweak();
			return -1;
		}

		long Decweak() noexcept
		{
			if (_crtl) {
				long ret = 1;
				if (ret = _crtl->Decweak(); ret == 0) {
					// we have deleted all weak refs. If there aren't any full refs delete ctrl object
					_crtl->Delete();
					_crtl = nullptr;
				}
				return ret;
			}
			return -1;
		}

		void Swap(PtrBase& other) noexcept
		{  // swap pointers
			std::swap(_ref, other._ref);
			std::swap(_crtl, other._crtl);
		}

		friend class shared_ptr<T>;

		template <class Y>
		friend class PtrBase;

		template <class Y>
		friend class weak_ptr;

		friend struct std::less<weak_ptr<T>>;
		friend struct std::less<shared_ptr<T>>;
		friend struct std::hash<weak_ptr<T>>;
		friend struct std::hash<shared_ptr<T>>;
	};

	template <class T>
	class shared_ptr : public PtrBase<T>
	{
	public:
		constexpr shared_ptr() noexcept = default;

#	if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
		constexpr shared_ptr(std::nullptr_t) noexcept {}  // construct empty shared_ptr
#	else
		shared_ptr(std::nullptr_t) noexcept
		{
		}  // construct empty shared_ptr
#	endif

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		shared_ptr(Y* ptr, control_block* crtl)
		{
			PtrBase<T>::_ref = ptr;
			PtrBase<T>::_crtl = crtl;
			static heap_allocator<void*> allocator;
			PtrBase<T>::_crtl->_allocatorCRTL = std::addressof(allocator);
			PtrBase<T>::_crtl->_allocatorREF = std::addressof(allocator);
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		shared_ptr(Y* ptr, control_block* crtl, heap_allocator<void*>* allocator)
		{
			PtrBase<T>::_ref = ptr;
			PtrBase<T>::_crtl = crtl;
			PtrBase<T>::_crtl->_allocatorCRTL = allocator;
			PtrBase<T>::_crtl->_allocatorREF = allocator;
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		explicit shared_ptr(Y* ptr) noexcept
		{
			static heap_allocator<void*> allocator;
			PtrBase<T>::_ref = ptr;
			void* mem = allocator.malloc(sizeof(control_blockT<Y>));
			control_blockT<Y>* crtl = new (mem) control_blockT<Y>();
			crtl->_ref = ptr;
			PtrBase<T>::_crtl = crtl;
			PtrBase<T>::_crtl->_allocatorCRTL = std::addressof(allocator);
			PtrBase<T>::_crtl->_allocatorREF = nullptr;
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		explicit shared_ptr(Y* ptr, heap_allocator<void*>* allocator) noexcept
		{
			PtrBase<T>::_ref = ptr;
			void* mem = allocator->malloc(sizeof(control_blockT<Y>));
			control_blockT<Y>* crtl = new (mem) control_blockT<Y>();
			crtl->_ref = ptr;
			PtrBase<T>::_crtl = crtl;
			PtrBase<T>::_crtl->_allocatorCRTL = allocator;
			PtrBase<T>::_crtl->_allocatorREF = allocator;
		}

		shared_ptr(const shared_ptr& other) noexcept
		{  // construct shared_ptr object that owns same resource as _Other
			this->copy(other);
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		shared_ptr(const shared_ptr<Y>& other) noexcept
		{
			// construct shared_ptr object that owns same resource as _Other
			this->copy(other);
		}

		shared_ptr(shared_ptr&& other) noexcept
		{  // construct shared_ptr object that takes resource from other
			this->move(std::move(other));
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		shared_ptr(shared_ptr<Y>&& other) noexcept
		{  // construct shared_ptr object that takes resource from other
			this->move(std::move(other));
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		explicit shared_ptr(const weak_ptr<Y> other)
		{
			if (this->ContstructFromWeak(other) == false) {
				throw std::bad_weak_ptr{};
			}
		}

		template <class Y>
		shared_ptr(const shared_ptr<Y>& _Right, T* _Px) noexcept
		{
			// construct shared_ptr object that aliases _Right
			this->AliasConstruct(_Right, _Px);
		}

		template <class Y>
		shared_ptr(shared_ptr<Y>&& _Right, T* _Px) noexcept
		{
			// move construct shared_ptr object that aliases _Right
			this->AliasMoveConstruct(std::move(_Right), _Px);
		}

		using PtrBase<T>::ConstructFromWeak;

		~shared_ptr() noexcept
		{  // release resource
			this->Decref();
		}

		void swap(shared_ptr& _Other) noexcept
		{
			this->Swap(_Other);
		}

		shared_ptr& operator=(const shared_ptr& other) noexcept
		{
			shared_ptr(other).swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		shared_ptr& operator=(const shared_ptr<Y>& other) noexcept
		{
			shared_ptr(other).swap(*this);
			return *this;
		}

		shared_ptr& operator=(shared_ptr&& _Right) noexcept
		{  // take resource from _Right
			shared_ptr(std::move(_Right)).swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		shared_ptr& operator=(shared_ptr<Y>&& _Right) noexcept
		{  // take resource from _Right
			shared_ptr(std::move(_Right)).swap(*this);
			return *this;
		}

		void reset() noexcept
		{  // release resource and convert to empty shared_ptr object
			shared_ptr().swap(*this);
		}
		using PtrBase<T>::get;

		[[nodiscard]] T& operator*() const noexcept
		{
			return *(get());
		}

		[[nodiscard]] T* operator->() const noexcept
		{
			return get();
		}

		explicit operator bool() const noexcept
		{
			return get() != nullptr;
		}

		friend struct std::less<shared_ptr<T>>;
		friend struct std::hash<shared_ptr<T>>;

		template <class Y, class... _Args>
		friend shared_ptr<Y> make_shared(_Args&&... args);

		template <class Y>
		friend shared_ptr<Y> make_shared();
	};

	template <class T, class S>
	inline bool operator==(const shared_ptr<T>& lhs, const shared_ptr<S>& rhs)
	{
		return lhs.get() == rhs.get();
	}

#	if __cplusplus >= 202002L
	template <class T, class S>
	inline bool operator<=>(const shared_ptr<T>& lhs, const shared_ptr<S>& rhs)
	{
		return lhs.get() <=> rhs.get();
	}
#	else
	template <class T1, class T2>
	[[nodiscard]] bool operator!=(const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs) noexcept
	{
		return lhs.get() != rhs.get();
	}

	template <class T1, class T2>
	[[nodiscard]] bool operator<(const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs) noexcept
	{
		return lhs.get() < rhs.get();
	}

	template <class T1, class T2>
	[[nodiscard]] bool operator>=(const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs) noexcept
	{
		return lhs.get() >= rhs.get();
	}

	template <class T1, class T2>
	[[nodiscard]] bool operator>(const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs) noexcept
	{
		return lhs.get() > rhs.get();
	}

	template <class T1, class T2>
	[[nodiscard]] bool operator<=(const shared_ptr<T1>& lhs, const shared_ptr<T2>& rhs) noexcept
	{
		return lhs.get() <= rhs.get();
	}

#	endif

	template <class T>
	inline bool operator==(const shared_ptr<T>& lhs, std::nullptr_t)
	{
		return lhs.get() == nullptr;
	}
#	if __cplusplus >= 202002L

	template <class T>
	inline bool operator<=>(const shared_ptr<T>& lhs, std::nullptr_t)
	{
		return lhs.get() <=> nullptr;
	}
#	else
	template <class T>
	[[nodiscard]] bool operator==(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return nullptr == rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator!=(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() != nullptr;
	}

	template <class T>
	[[nodiscard]] bool operator!=(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return nullptr != rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator<(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() < static_cast<typename shared_ptr<T>::elementTpe*>(nullptr);
	}

	template <class T>
	[[nodiscard]] bool operator<(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return static_cast<typename shared_ptr<T>::elementTpe*>(nullptr) < rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator>=(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() >= static_cast<typename shared_ptr<T>::elementTpe*>(nullptr);
	}

	template <class T>
	[[nodiscard]] bool operator>=(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return static_cast<typename shared_ptr<T>::elementTpe*>(nullptr) >= rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator>(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() > static_cast<typename shared_ptr<T>::elementTpe*>(nullptr);
	}

	template <class T>
	[[nodiscard]] bool operator>(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return static_cast<typename shared_ptr<T>::elementTpe*>(nullptr) > rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator<=(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() <= static_cast<typename shared_ptr<T>::elementTpe*>(nullptr);
	}

	template <class T>
	[[nodiscard]] bool operator<=(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return static_cast<typename shared_ptr<T>::elementTpe*>(nullptr) <= rhs.get();
	}
#	endif

	/* template <class T, class... _Args>
	shared_ptr<T> make_shared(_Args&&... args)
	{
		static heap_allocator<void*> allocator;
		void* mem = allocator.malloc(sizeof(T) + sizeof(control_blockTA<T>));
		control_blockTA<T>* crtl = new (mem) control_blockTA<T>();
		T* tptr = new ((char*)mem + sizeof(control_blockTA<T>)) T(std::forward<_Args>(args)...);
		shared_ptr<T> ptr(tptr, crtl, std::addressof(allocator));
		return ptr;
	}
	template <class T>
	shared_ptr<T> make_shared()
	{
		static heap_allocator<void*> allocator;
		void* mem = allocator.malloc(sizeof(T) + sizeof(control_blockTA<T>));
		control_blockTA<T>* crtl = new (mem) control_blockTA<T>();
		T* tptr = new ((char*)mem + sizeof(control_blockTA<T>)) T();
		shared_ptr<T> ptr(tptr, crtl, std::addressof(allocator));
		return ptr;
	}*/
	template <class T>
	shared_ptr<T> make_shared()
	{
		static heap_allocator<void*> allocator;

		// Compute aligned offset for T after the control block
		constexpr size_t controlSize = sizeof(control_blockTA<T>);
		constexpr size_t alignOfT = alignof(T);
		constexpr size_t alignedOffset = (controlSize + alignOfT - 1) & ~(alignOfT - 1);
		constexpr size_t totalSize = alignedOffset + sizeof(T);

		void* mem = allocator.malloc(totalSize);

		control_blockTA<T>* crtl = new (mem) control_blockTA<T>();
		T* tptr = new ((char*)mem + alignedOffset) T();
		crtl->_ref = tptr;

		shared_ptr<T> ptr(tptr, crtl, std::addressof(allocator));
		return ptr;
	}

	template <class T, class... _Args>
	shared_ptr<T> make_shared(_Args&&... args)
	{
		static heap_allocator<void*> allocator;

		constexpr size_t controlSize = sizeof(control_blockTA<T>);
		constexpr size_t alignOfT = alignof(T);
		constexpr size_t alignedOffset = (controlSize + alignOfT - 1) & ~(alignOfT - 1);
		constexpr size_t totalSize = alignedOffset + sizeof(T);

		void* mem = allocator.malloc(totalSize);

		control_blockTA<T>* crtl = new (mem) control_blockTA<T>();
		T* tptr = new ((char*)mem + alignedOffset) T(std::forward<_Args>(args)...);
		crtl->_ref = tptr;

		shared_ptr<T> ptr(tptr, crtl, std::addressof(allocator));
		return ptr;
	}
	template <class T>
	shared_ptr<T> create_shared(T* obj)
	{
		static heap_allocator<void*> allocator;
		shared_ptr<T> ptr(obj, std::addressof(allocator));
		return ptr;
	}

	template <class T>
	class weak_ptr : public PtrBase<T>
	{
	public:
		constexpr weak_ptr() noexcept {}

		weak_ptr(const weak_ptr& other) noexcept
		{
			this->ConstructWeak(other);  // same type, no conversion
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		weak_ptr(const shared_ptr<Y>& other) noexcept
		{
			this->ConstructWeak(other);  // shared_ptr keeps resource alive during conversion
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		weak_ptr(const weak_ptr<Y>& other) noexcept
		{
			this->ConstructWeak(other);
		}

		weak_ptr(weak_ptr&& other) noexcept
		{
			this->move(std::move(other));
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		weak_ptr(weak_ptr<Y>&& other) noexcept
		{
			this->WeakMove(std::move(other));
		}

		~weak_ptr() noexcept
		{
			this->Decweak();
		}

		void swap(weak_ptr& _Other) noexcept
		{
			this->Swap(_Other);
		}

		weak_ptr& operator=(const weak_ptr& other) noexcept
		{
			weak_ptr(other).swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		weak_ptr& operator=(const weak_ptr<Y>& other) noexcept
		{
			weak_ptr(other).swap(*this);
			return *this;
		}

		weak_ptr& operator=(weak_ptr&& other) noexcept
		{
			weak_ptr(std::move(other)).swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		weak_ptr& operator=(weak_ptr<Y>&& other) noexcept
		{
			weak_ptr(std::move(other)).swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		weak_ptr& operator=(const shared_ptr<Y>& other) noexcept
		{
			weak_ptr(other).swap(*this);
			return *this;
		}

		void reset() noexcept
		{  // release resource, convert to null weak_ptr object
			weak_ptr{}.swap(*this);
		}

		[[nodiscard]] bool expired() const noexcept
		{
			return this->use_count() == 0;
		}

		[[nodiscard]] shared_ptr<T> lock() const noexcept
		{  // convert to shared_ptr
			shared_ptr<T> ret;
			(void)ret.ConstructFromWeak(*this);
			return ret;
		}

		friend struct std::less<weak_ptr<T>>;
		friend struct std::hash<weak_ptr<T>>;
	};

	template <class T>
	class controlBlockUnique
	{
	public:
		virtual void Destroy() = 0;
		virtual void Delete() = 0;
		virtual T* get() = 0;
	};

	template <class T, class U>
	class controlBlockUniqueT : public controlBlockUnique<T>
	{
	public:
		U* _ref = nullptr;
		heap_allocator<void*>* _allocator = nullptr;

		virtual void Destroy()
		{
			_ref->~U();
		}
		virtual void Delete()
		{
			if (_allocator)
				_allocator->dealloc(_ref);
			else
				delete _ref;
		}
		virtual T* get()
		{
			// unique ptr can only be constructed if U is derived from T anyway
			return static_cast<T*>(_ref);
		}
	};

	template <class T>
	class unique_ptr
	{
	private:
		controlBlockUnique<T>* _crtl = nullptr;
		heap_allocator<void*>* _allocator = nullptr;

	public:
#	if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
		constexpr unique_ptr(std::nullptr_t) noexcept {}  // construct empty unique_ptr
#	else
		unique_ptr(std::nullptr_t) noexcept
		{
		}  // construct empty unique_ptr
#	endif

		_CONSTEXPR23 unique_ptr& operator=(nullptr_t) noexcept
		{
			reset();
			return *this;
		}

		_CONSTEXPR23 explicit unique_ptr(T* ptr)
		{
			static heap_allocator<void*> allocator;
			controlBlockUniqueT<T, T>* crtl = new ((char*)allocator.malloc(sizeof(controlBlockUniqueT<T, T>))) controlBlockUniqueT<T, T>();
			crtl->_ref = ptr;
			crtl->_allocator = nullptr;
			_crtl = crtl;
			_allocator = std::addressof(allocator);
		}

		_CONSTEXPR23 unique_ptr(T* ptr, heap_allocator<void*>* allocator)
		{
			controlBlockUniqueT<T, T>* crtl = new ((char*)allocator->malloc(sizeof(controlBlockUniqueT<T, T>))) controlBlockUniqueT<T, T>();
			crtl->_ref = ptr;
			crtl->_allocator = allocator;
			_crtl = crtl;
			_allocator = allocator;
		}

		template <std::derived_from<T> U>
		_CONSTEXPR23 explicit unique_ptr(U* ptr)
		{
			static heap_allocator<void*> allocator;
			controlBlockUniqueT<T, U>* crtl = new ((char*)allocator.malloc(sizeof(controlBlockUniqueT<T, U>))) controlBlockUniqueT<T, U>();
			crtl->_ref = ptr;
			crtl->_allocator = nullptr;
			_crtl = crtl;
			_allocator = std::addressof(allocator);
		}

		template <std::derived_from<T> U>
		_CONSTEXPR23 unique_ptr(U* ptr, heap_allocator<void*>* allocator)
		{
			controlBlockUniqueT<T, U>* crtl = new ((char*)allocator->malloc(sizeof(controlBlockUniqueT<T, U>))) controlBlockUniqueT<T, U>();
			crtl->_ref = ptr;
			crtl->_allocator = allocator;
			_crtl = crtl;
			_allocator = allocator;
		}

		_CONSTEXPR23 unique_ptr(unique_ptr<T>&& _Right) noexcept
		{
			_crtl = _Right._crtl;
			_Right._crtl = nullptr;
			_allocator = _Right._allocator;
		}

		_CONSTEXPR23 unique_ptr<T>& operator=(unique_ptr<T>&& _Right) noexcept
		{
			reset();
			_crtl = _Right._crtl;
			_Right._crtl = nullptr;
			_allocator = _Right._allocator;
			return *this;
		}

		_CONSTEXPR23 void swap(unique_ptr<T>& _Right) noexcept
		{
			auto ptr = _crtl;
			auto all = _allocator;

			_crtl = _Right._crtl;
			_allocator = _Right._allocator;

			_Right._crtl = ptr;
			_Right._allocator = all;
		}

		_CONSTEXPR23 ~unique_ptr() noexcept
		{
			if (_crtl) {
				_crtl->Destroy();
				_crtl->Delete();
				_allocator->dealloc(_crtl);
			}
		}

		_NODISCARD _CONSTEXPR23 T& operator*() const noexcept(noexcept(*_STD declval<T*>()))
		{
			return *_crtl->get();
		}

		_NODISCARD _CONSTEXPR23 T* operator->() const noexcept
		{
			return _crtl->get();
		}

		_NODISCARD _CONSTEXPR23 T* get() const noexcept
		{
			return _crtl->get();
		}

		template <class U>
		_NODISCARD _CONSTEXPR23 U* get() const noexcept
		{
			return dynamic_cast<U*>(_crtl->get());
		}

		_CONSTEXPR23 explicit operator bool() const noexcept
		{
			return _crtl != nullptr;
		}

		_CONSTEXPR23 T* release() noexcept
		{
			T* ptr = _crtl.get<T>();
			_crtl->Dealloc();
			_crtl = nullptr;
			_allocator = nullptr;
			return ptr;
		}

		_CONSTEXPR23 void reset() noexcept
		{
			if (_crtl) {
				_crtl->Destroy();
				_crtl->Delete();
				_allocator->dealloc(_crtl);
			}
			_crtl = nullptr;
		}

		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

		friend struct std::less<unique_ptr<T>>;
		friend struct std::hash<unique_ptr<T>>;

		template <class Y, class... _Args>
		friend unique_ptr<Y> make_unique(_Args&&... args);

		template <class Y>
		friend unique_ptr<Y> make_unique();
	};

	template <class T, class S>
	inline bool operator==(const unique_ptr<T>& lhs, const unique_ptr<S>& rhs)
	{
		return lhs.get() == rhs.get();
	}

#	if __cplusplus >= 202002L
	template <class T, class S>
	inline bool operator<=>(const unique_ptr<T>& lhs, const unique_ptr<S>& rhs)
	{
		return lhs.get() <=> rhs.get();
	}
#	else
	template <class T1, class T2>
	[[nodiscard]] bool operator!=(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) noexcept
	{
		return lhs.get() != rhs.get();
	}

	template <class T1, class T2>
	[[nodiscard]] bool operator<(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) noexcept
	{
		return lhs.get() < rhs.get();
	}

	template <class T1, class T2>
	[[nodiscard]] bool operator>=(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) noexcept
	{
		return lhs.get() >= rhs.get();
	}

	template <class T1, class T2>
	[[nodiscard]] bool operator>(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) noexcept
	{
		return lhs.get() > rhs.get();
	}

	template <class T1, class T2>
	[[nodiscard]] bool operator<=(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) noexcept
	{
		return lhs.get() <= rhs.get();
	}

#	endif

	template <class T>
	inline bool operator==(const unique_ptr<T>& lhs, std::nullptr_t)
	{
		return lhs.get() == nullptr;
	}
#	if __cplusplus >= 202002L

	template <class T>
	inline bool operator<=>(const unique_ptr<T>& lhs, std::nullptr_t)
	{
		return lhs.get() <=> nullptr;
	}
#	else
	template <class T>
	[[nodiscard]] bool operator==(std::nullptr_t, const unique_ptr<T>& rhs) noexcept
	{
		return nullptr == rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator!=(const unique_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() != nullptr;
	}

	template <class T>
	[[nodiscard]] bool operator!=(std::nullptr_t, const unique_ptr<T>& rhs) noexcept
	{
		return nullptr != rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator<(const unique_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() < static_cast<typename unique_ptr<T>::elementTpe*>(nullptr);
	}

	template <class T>
	[[nodiscard]] bool operator<(std::nullptr_t, const unique_ptr<T>& rhs) noexcept
	{
		return static_cast<typename unique_ptr<T>::elementTpe*>(nullptr) < rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator>=(const unique_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() >= static_cast<typename unique_ptr<T>::elementTpe*>(nullptr);
	}

	template <class T>
	[[nodiscard]] bool operator>=(std::nullptr_t, const unique_ptr<T>& rhs) noexcept
	{
		return static_cast<typename unique_ptr<T>::elementTpe*>(nullptr) >= rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator>(const unique_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() > static_cast<typename unique_ptr<T>::elementTpe*>(nullptr);
	}

	template <class T>
	[[nodiscard]] bool operator>(std::nullptr_t, const unique_ptr<T>& rhs) noexcept
	{
		return static_cast<typename unique_ptr<T>::elementTpe*>(nullptr) > rhs.get();
	}

	template <class T>
	[[nodiscard]] bool operator<=(const unique_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.get() <= static_cast<typename unique_ptr<T>::elementTpe*>(nullptr);
	}

	template <class T>
	[[nodiscard]] bool operator<=(std::nullptr_t, const unique_ptr<T>& rhs) noexcept
	{
		return static_cast<typename unique_ptr<T>::elementTpe*>(nullptr) <= rhs.get();
	}
#	endif

	template <class T>
	unique_ptr<T> make_unique()
	{
		static heap_allocator<void*> allocator;

		void* mem = allocator.malloc(sizeof(T));
		T* tptr = new ((char*)mem) T();

		unique_ptr<T> ptr(tptr, std::addressof(allocator));
		return ptr;
	}
	template <class T, class... _Args>
	unique_ptr<T> make_unique(_Args&&... args)
	{
		static heap_allocator<void*> allocator;

		void* mem = allocator.malloc(sizeof(T));
		T* tptr = new ((char*)mem) T(std::forward<_Args>(args)...);

		unique_ptr<T> ptr(tptr, std::addressof(allocator));
		return ptr;
	}
	template <class T, std::derived_from<T> U>
	unique_ptr<T> make_unique()
	{
		static heap_allocator<void*> allocator;

		void* mem = allocator.malloc(sizeof(U));
		U* tptr = new ((char*)mem) U();

		unique_ptr<T> ptr(tptr, std::addressof(allocator));
		return ptr;
	}
	template <class T, std::derived_from<T> U, class... _Args>
	unique_ptr<T> make_unique(_Args&&... args)
	{
		static heap_allocator<void*> allocator;

		void* mem = allocator.malloc(sizeof(U));
		U* tptr = new ((char*)mem) U(std::forward<_Args>(args)...);

		unique_ptr<T> ptr(tptr, std::addressof(allocator));
		return ptr;
	}
	template <class T>
	unique_ptr<T> create_unique(T* ptr)
	{
		static heap_allocator<void*> allocator;

		unique_ptr<T> ptr(ptr, std::addressof(allocator));
		return ptr;
	}
	template <class T, std::derived_from<T> U>
	unique_ptr<T> create_unique(U* ptr)
	{
		static heap_allocator<void*> allocator;

		unique_ptr<T> ptr(ptr, std::addressof(allocator));
		return ptr;
	}
}
template <class _Ty1, class _Ty2>
[[nodiscard]] util::shared_ptr<_Ty1> dynamic_pointer_cast(const util::shared_ptr<_Ty2>& _Other) noexcept
{
	// dynamic_cast for shared_ptr that properly respects the reference count control block
	const auto _Ptr = dynamic_cast<_Ty1*>(_Other.get());

	if (_Ptr) {
		return util::shared_ptr<_Ty1>(_Other, _Ptr);
	}

	return {};
}

template <class _Ty1, class _Ty2>
[[nodiscard]] util::shared_ptr<_Ty1> dynamic_pointer_cast(util::shared_ptr<_Ty2>&& _Other) noexcept
{
	// dynamic_cast for shared_ptr that properly respects the reference count control block
	const auto _Ptr = dynamic_cast<_Ty1*>(_Other.get());

	if (_Ptr) {
		return util::shared_ptr<_Ty1>(std::move(_Other), _Ptr);
	}

	return {};
}

namespace std
{
	template <class T>
	struct less<util::shared_ptr<T>>
	{
		bool operator()(const util::shared_ptr<T>& _left, const util::shared_ptr<T>& _right) const
		{
			return less<util::control_block*>{}(_left._crtl, _right._crtl);
		}
	};

	template <class T>
	struct less<util::weak_ptr<T>>
	{
		bool operator()(const util::weak_ptr<T>& _left, const util::weak_ptr<T>& _right) const
		{
			return less<util::control_block*>{}(_left._crtl, _right._crtl);
		}
	};

	template <class T>
	struct hash<util::shared_ptr<T>>
	{
		[[nodiscard]] static size_t operator()(const util::shared_ptr<T>& key) noexcept
		{
			return hash<void*>()(static_cast<void*>(key._ref));
		}
	};

	template <class T>
	struct hash<util::weak_ptr<T>>
	{
		[[nodiscard]] static size_t operator()(const util::weak_ptr<T>& key) noexcept
		{
			return hash<void*>()(static_cast<void*>(key._ref));
		}
	};
	template <class T>
	struct less<util::unique_ptr<T>>
	{
		bool operator()(const util::unique_ptr<T>& _left, const util::unique_ptr<T>& _right) const
		{
			return left._ref < _right._ref;
		}
	};

	template <class T>
	struct hash<util::unique_ptr<T>>
	{
		[[nodiscard]] static size_t operator()(const util::unique_ptr<T>& key) noexcept
		{
			return hash<void*>()(static_cast<void*>(key._ref));
		}
	};
}
#endif


namespace LibImGuiUI
{
	namespace UserInterface
	{
		class Window;
		class Tab;
		class Widget;
	}

	enum class MessageType
	{
		Minor = 0,
		Debug = 1,
		GameMessage = 2,
	};

	enum class MessageReturnType
	{
		Scheduled = 0,
		RejectedWrongMessageTypeForFunction = 1,
		/// <summary>
		/// Returned when the requested message type is disabled in the settings, or unknown
		/// </summary>
		Unavailable = 2,
	};

	enum class APIVersion
	{
		V1 = 1,
	};
	class LibImGuiUI_APIv1
	{
	public:
		static inline LibImGuiUI_APIv1* instance = nullptr;
		static inline LibImGuiUIImGui::ImGuiPluginAPI* imgui = nullptr;

		/// <summary>
		/// Returns the ImGui Font and scaled fontSize for the given fonttype
		/// </summary>
		/// <param name="fontType"></param>
		/// <returns></returns>
		virtual std::pair<LibImGuiUIImGui::ImFont*, float> GetFontByType(FontType fontType) = 0;
		/// <summary>
		/// Returns a user defined colour (defined in theme)
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		virtual LibImGuiUIImGui::ImVec4 GetUserColour(const char* name) = 0;
		/// <summary>
		/// Registers a tab for one of the default windows, Settings, RuleEditor, Performance Monitor
		/// </summary>
		/// <param name="tab"></param>
		virtual void RegisterTab(util::shared_ptr<UserInterface::Tab> tab) = 0;
		/// <summary>
		/// Registers a window
		/// </summary>
		/// <param name="window"></param>
		virtual void RegisterWindow(util::shared_ptr<UserInterface::Window> window) = 0;
		/// <summary>
		/// Unregisters a tab from one of the default windows. The tab will show up anymore when the window is openened
		/// </summary>
		/// <param name="tab"></param>
		virtual void UnregisterTab(util::shared_ptr<UserInterface::Tab> tab) = 0;
		/// <summary>
		/// Unregisters a window
		/// </summary>
		/// <param name="window"></param>
		virtual void UnregisterWindow(util::shared_ptr<UserInterface::Window> window) = 0;

		/// <summary>
		/// Opens the given window [Window must be already be registered]
		/// </summary>
		/// <param name="window"></param>
		virtual void OpenWindow(util::shared_ptr<UserInterface::Window> window) = 0;
		/// <summary>
		/// Closes the given window [Window must be registered and open]
		/// </summary>
		/// <param name="window"></param>
		virtual void CloseWindow(util::shared_ptr<UserInterface::Window> window) = 0;
		/// <summary>
		/// Closes all open windows
		/// </summary>
		virtual void CloseAllWindows() = 0;

		/// <summary>
		/// Registers a widget
		/// </summary>
		/// <param name="widget"></param>
		virtual void RegisterWidget(util::shared_ptr<UserInterface::Widget> widget) = 0;
		/// <summary>
		/// Unregisters a widget
		/// </summary>
		/// <param name="widget"></param>
		virtual void UnregisterWidget(util::shared_ptr<UserInterface::Widget> widget) = 0;

		/// <summary>
		/// Displays a message with aa texture in front
		/// </summary>
		/// <param name="type"></param>
		/// <param name="message"></param>
		virtual MessageReturnType DisplayMinorMessage(MessageType type, util::message message) = 0;
		/// <summary>
		/// Displays a message
		/// </summary>
		/// <param name="type"></param>
		/// <param name="message"></param>
		virtual MessageReturnType DisplayMinorMessage(MessageType type, util::string message) = 0;
		/// <summary>
		/// Displays a message with sub Messages
		/// Only supported message type is MajorWithSubMessages
		/// </summary>
		/// <param name="type"></param>
		/// <param name="message"></param>
		virtual MessageReturnType DisplayMessage(util::string message, util::vector<util::message> subMessages) = 0;

		/// <summary>
		/// Returns the number of currently open windows
		/// </summary>
		/// <returns></returns>
		virtual int GetNumberOfOpenWindows() = 0;
		/// <summary>
		/// Returns whether windows can currently be openened
		/// </summary>
		/// <returns></returns>
		virtual bool CanOpen() = 0;

		/// <summary>
		/// creates and return a widget matching the given interface type (you can use dyanmic_pointer_cast to get the correct type) initialized with the location and data stored under the given name.
		/// Passing "" as name will result in generic Widget that cannot load or store data
		/// </summary>
		/// <param name="tpe"></param>
		/// <param name="name"></param>
		/// <returns></returns>
		virtual util::shared_ptr<UserInterface::Widget> CreateWidget(WidgetType type, util::string name) = 0;

		/// <summary>
		/// Prompts the mod to save the location data and widget data for the given widget if its identifier is non empty
		/// Widget data is automatically changed if the user rearranges widgets using this mods settings menu
		/// </summary>
		/// <param name="widget"></param>
		virtual void SaveWidgetData(util::shared_ptr<UserInterface::Widget> widget) = 0;
	};

	typedef void* (*_RequestPluginAPI)(APIVersion);
	typedef void* (*_RequestPluginImGuiAPI)();

#define IMGUI_PLUGIN_API_COMPATIBLE(api_ptr) (                      \
	(api_ptr) != NULL &&                                            \
	(api_ptr)->sentinel == 0xDEADBEEF &&                            \
	(api_ptr)->struct_size >= sizeof(ImGuiPluginAPI) &&             \
	((api_ptr)->version >> 16) == IMGUI_PLUGIN_API_VERSION_MAJOR && \
	((api_ptr)->version & 0xFFFF) >= IMGUI_PLUGIN_API_VERSION_MINOR)

	[[nodiscard]] inline void* RequestPluginAPI(APIVersion version = APIVersion::V1)
	{
		std::string str = "LibImGuiUI.dll";
		std::wstring wstr = std::wstring(str.begin(), str.end());
		auto pluginHandle = GetModuleHandle(wstr.c_str());
		if (pluginHandle != 0) {
			_RequestPluginImGuiAPI requestImGuiAPIFunction = (_RequestPluginImGuiAPI)GetProcAddress(pluginHandle, "RequestPluginImGuiAPI");
			if (requestImGuiAPIFunction) {
				LibImGuiUI_APIv1::imgui = (LibImGuiUIImGui::ImGuiPluginAPI*)requestImGuiAPIFunction();
			}
			_RequestPluginAPI requestAPIFunction = (_RequestPluginAPI)GetProcAddress(pluginHandle, "RequestPluginAPI");
			if (requestAPIFunction) {
				switch (version) {
				case APIVersion::V1:
					LibImGuiUI_APIv1::instance = (LibImGuiUI_APIv1*)requestAPIFunction(version);
					return LibImGuiUI_APIv1::instance;
					break;
				}
			}
		}
		return nullptr;
	}

	namespace UserInterface
	{
		class Window
		{
		public:
			struct KeyInformation
			{
				/// <summary>
				/// unique keys that open the window
				/// </summary>
				uint32_t* keys = nullptr;
				/// <summary>
				/// number of keys
				/// </summary>
				size_t numKeys;

				/// <summary>
				/// whether in addition to the above keys, the action will also be performed if simply the ESC key is pressed
				/// </summary>
				bool alternateKeyEsacape = false;
			};

		protected:
			/// <summary>
			/// whether the window is currently open
			/// </summary>
			std::atomic<bool> _open = false;

		public:
			/// <summary>
			/// opens the window
			/// </summary>
			/// <returns>Whether the window was opened</returns>
			virtual bool Open()
			{
				_open = true;
				return _open;
			}
			/// <summary>
			/// closes the window
			/// </summary>
			/// <returns>Whether the window was closed</returns>
			virtual bool Close()
			{
				_open = false;
				return _open;
			}
			/// <summary>
			/// Returns wether the window can be openend
			/// </summary>
			/// <returns></returns>
			virtual bool CanOpen()
			{
				return true;
			}

			/// <summary>
			/// draws the ui elements
			/// </summary>
			virtual void Draw() = 0;

			/// <summary>
			/// returns the name of the window
			/// </summary>
			/// <returns>The name of the window</returns>
			virtual const char* GetName() = 0;

			/// <summary>
			/// returns whether the window is currently open
			/// </summary>
			/// <returns>Whether the window is currently open</returns>
			virtual bool IsOpen() { return _open; }

			/// <summary>
			/// returns whether this window can be opened by the user
			/// </summary>
			/// <returns></returns>
			virtual bool HasOpenKeys() { return true; }
			/// <summary>
			/// returns the KeyInformation for closing the window
			/// </summary>
			/// <returns></returns>
			virtual KeyInformation* GetCloseKeys() = 0;
			/// <summary>
			/// returns the KeyInformation for closing the window
			/// </summary>
			/// <returns></returns>
			virtual KeyInformation* GetAlternateCloseKeys()
			{
				return nullptr;
			}
			/// <summary>
			/// returns whether the window uses alernate closing keys
			/// </summary>
			/// <returns></returns>
			virtual bool HasAlternateCloseKeys() { return false; }
			/// <summary>
			/// returns the KeyInformation for opening the window
			/// </summary>
			/// <returns></returns>
			virtual KeyInformation* GetOpenKeys() = 0;
			/// <summary>
			/// returns the KeyInformation for opening the window
			/// </summary>
			/// <returns></returns>
			virtual KeyInformation* GetAlternateOpenKeys()
			{
				return nullptr;
			}
			/// <summary>
			/// retruns whether the window uses alternate open keys
			/// </summary>
			/// <returns></returns>
			virtual bool HasAlternateOpenKeys() { return false; }

			/// <summary>
			/// Returns whether this is a non-ui blocking side window
			/// </summary>
			/// <returns></returns>
			virtual bool IsSideWindow() { return false; }
		};

		class Tab
		{
		public:
			enum class TabType
			{
				SettingsTab,    // window mainly used for settings
				RuleEditorTab,  // window used for rule editors
				ProfilingTab,   // window used to display performance metrics, uses small font to fit in content
				MiscTab,        // currently unused
			};
			/// <summary>
			/// draws the ui elememts
			/// </summary>
			virtual void Draw() = 0;
			/// <summary>
			/// returns the name of the tab
			/// </summary>
			/// <returns></returns>
			virtual const char* GetName() = 0;

			/// <summary>
			/// returns the type of the tab (i.e. the window it should associated with)
			/// </summary>
			/// <returns></returns>
			virtual TabType GetType() = 0;

			/// <summary>
			/// event called when the parent window is opened
			/// </summary>
			virtual void OnWindowOpen() = 0;
			/// <summary>
			/// event called when the parent window is closed
			/// </summary>
			virtual void OnWindowClose() = 0;

			virtual void AfterDraw() {};
		};

		typedef uint32_t Alignment;

		struct AlignmentFlags
		{
			enum Alignments
			{
				Left = 1 << 0,
				CenterX = 1 << 1,
				Right = 1 << 2,
				Top = 1 << 3,
				CenterY = 1 << 4,
				Bottom = 1 << 5
			};
		};

		struct Dimension
		{
			bool fractionOfScreen = true;
			float length = 0.f;
		};

		enum class OffsetPosition
		{
			/// <summary>
			/// Offset from 0 of the axis to lower border of the window (for x: offset from 0 to left border
			/// </summary>
			Low,
			/// <summary>
			/// Offset is calculated from the highest value of the respectve axis to the higher border of the window (for x: from screenwidth to the right border)
			/// </summary>
			High
		};

		struct Offset
		{
			float offset = 0.f;
			bool fractionOfScreen = true;
			OffsetPosition offsetType = OffsetPosition::Low;
		};

		enum class WidgetAnchor
		{
			Unset = 0,
			ScreenPos = 1,
			ObjectAnchor = 2,
		};

		struct LocationScreenPos
		{
			Offset offsetX = { 0.f, true, OffsetPosition::Low };
			Offset offsetY = { 0.f, true, OffsetPosition::Low };
			Dimension height = { false, 40.f };
			Dimension width = { false, 40.f };

			/// <summary>
			/// scales all widget dimensions that don't use fractions
			/// </summary>
			bool scaleWidget = false;

			float GetOffsetX(float screenWidth)
			{
				float offset = 0.f;
				if (offsetX.fractionOfScreen)
					offset = offsetX.offset * screenWidth;
				else
					offset = offsetX.offset;
				if (offsetX.offsetType == OffsetPosition::High)
					return screenWidth - offset;
				return offset;
			}

			float GetOffsetY(float screenHeight)
			{
				float offset = 0.f;
				if (offsetY.fractionOfScreen)
					offset = offsetY.offset * screenHeight;
				else
					offset = offsetY.offset;
				if (offsetY.offsetType == OffsetPosition::High)
					return screenHeight - offset;
				return offset;
			}

			float GetWidth(float screenWidth, float scale)
			{
				if (width.fractionOfScreen)
					return width.length * screenWidth;
				else
					return scaleWidget ? width.length * scale : width.length;
			}

			float GetHeight(float screenHeight, float scale)
			{
				if (height.fractionOfScreen)
					return height.length * screenHeight;
				else
					return scaleWidget ? height.length * scale : height.length;
			}
		};

		struct LocationObjectAnchor
		{
			enum class ObjectAnchor
			{
				ActorTorso = 0,
				ActorHead = 1,
				ActorEye = 2,
				ActorSaddle = 5,
			};

			ObjectAnchor anchor;
			Alignment widgetAnchorPoint;
			Offset offsetX{ 0.f, false, OffsetPosition::Low };
			Offset offsetY{ 0.f, false, OffsetPosition::Low };
			Dimension height = { false, 40.f };
			Dimension width = { false, 40.f };

			/// <summary>
			/// scales all widget dimensions that don't use fractions
			/// </summary>
			bool scaleWidget = false;

			/// <summary>
			/// min scaling for distant objects
			/// </summary>
			float distanceMinScale = 0.5f;

			/// <summary>
			/// The object this widget is attached to. Only used when anchor is object related
			/// </summary>
			RE::ObjectRefHandle anchorObject;

			/// <summary>
			/// widget will still be displayed even if the object is invisible, hidden, or has an alpha of 0
			/// </summary>
			bool allowObjectInvisibility = false;

			/// <summary>
			/// widget will display even if the object is occluded
			/// </summary>
			bool allowObjectOccluded = true;

			float GetOffsetX(float screenWidth)
			{
				float offset = 0.f;
				if (offsetX.fractionOfScreen)
					offset = offsetX.offset * screenWidth;
				else
					offset = offsetX.offset;
				if (offsetX.offsetType == OffsetPosition::High)
					return -offset;
				return offset;
			}

			float GetOffsetY(float screenHeight)
			{
				float offset = 0.f;
				if (offsetY.fractionOfScreen)
					offset = offsetY.offset * screenHeight;
				else
					offset = offsetY.offset;
				if (offsetY.offsetType == OffsetPosition::High)
					return -offset;
				return offset;
			}

			float GetWidth(float screenWidth, float scale)
			{
				if (width.fractionOfScreen)
					return width.length * screenWidth;
				else
					return scaleWidget ? width.length * scale : width.length;
			}

			float GetHeight(float screenHeight, float scale)
			{
				if (height.fractionOfScreen)
					return height.length * screenHeight;
				else
					return scaleWidget ? height.length * scale : height.length;
			}
		};

		struct Location
		{
			WidgetAnchor anchor = WidgetAnchor::ScreenPos;

			union
			{
				LocationScreenPos screenPosLocation;
				LocationObjectAnchor objectAnchorLocation;
			};

			~Location()
			{
			}

			float GetOffsetX(float screenWidth)
			{
				switch (anchor) {
				case WidgetAnchor::ObjectAnchor:
					return objectAnchorLocation.GetOffsetX(screenWidth);
				case WidgetAnchor::ScreenPos:
				default:
					return screenPosLocation.GetOffsetX(screenWidth);
				}
			}

			float GetOffsetY(float screenHeight)
			{
				switch (anchor) {
				case WidgetAnchor::ObjectAnchor:
					return objectAnchorLocation.GetOffsetY(screenHeight);
				case WidgetAnchor::ScreenPos:
				default:
					return screenPosLocation.GetOffsetY(screenHeight);
				}
			}

			float GetWidth(float screenWidth, float scale)
			{
				switch (anchor) {
				case WidgetAnchor::ObjectAnchor:
					return objectAnchorLocation.GetWidth(screenWidth, scale);
				case WidgetAnchor::ScreenPos:
				default:
					return screenPosLocation.GetWidth(screenWidth, scale);
				}
			}

			float GetHeight(float screenHeight, float scale)
			{
				switch (anchor) {
				case WidgetAnchor::ObjectAnchor:
					return objectAnchorLocation.GetHeight(screenHeight, scale);
				case WidgetAnchor::ScreenPos:
				default:
					return screenPosLocation.GetHeight(screenHeight, scale);
				}
			}
		};

		struct RenderPosition
		{
			float offsetX;
			float offsetY;
			float width;
			float height;
		};

		class Widget
		{
		public:
			struct Dimensions
			{
				float width;
				float height;
			};

			enum WidgetFlags
			{
				/// <summary>
				/// whether the widget is currently being shown
				/// </summary>
				WidgetFlags_Shown = 1 << 0,
				/// <summary>
				/// whether the widget should skip drawing
				/// </summary>
				WidgetFlags_SkipDrawing = 1 << 1,
				/// <summary>
				/// whether the widget was shown last frame [internal use]
				/// </summary>
				WidgetFlags_WasShown = 1 << 2,
			};

		protected:
			/// <summary>
			/// widget flags used during runtime
			/// </summary>
			uint32_t _widgetFlags = 0;

			/// <summary>
			/// how transparent the widget is (lowest - 0.f - effectively hidden, highest - 1.0 - hides everything beneath
			/// </summary>
			float _opacity = 0.f;

			/// <summary>
			/// location of the widget on screen
			/// </summary>
			Location _widgetLocation = { WidgetAnchor::Unset, LocationScreenPos{} };

			/// <summary>
			/// frame accurate position where the widget will be rendered
			/// </summary>
			RenderPosition _renderPos;

			/// <summary>
			/// unique identifier used to store and load widget locations and data
			/// if this is empty, the widget is generic cannot load or store data
			/// </summary>
			util::string _identifier;

			/// <summary>
			/// actual size calculated for this widget
			/// </summary>
			Dimensions _size = { 0.f, 0.f };

			/// <summary>
			/// widgets that are drawn over this widget
			/// </summary>
			util::vector<util::shared_ptr<Widget>> _subordinateWidgets;

		public:
			/// <summary>
			/// shows the widget
			/// </summary>
			/// <returns>Whether the window was opened</returns>
			virtual bool Show()
			{
				_widgetFlags |= WidgetFlags_Shown;
				return true;
			}
			/// <summary>
			/// hides the widget
			/// </summary>
			/// <returns>Whether the window was closed</returns>
			virtual bool Hide()
			{
				_widgetFlags &= ~WidgetFlags_Shown;
				_widgetFlags &= ~WidgetFlags_WasShown;
				return true;
			}

			/// <summary>
			/// draws the ui elements
			/// </summary>
			virtual void Draw() = 0;

			/// <summary>
			/// Draws subordinate widgets
			/// </summary>
			virtual void DrawSubordinateWidgets()
			{
				for (auto& widget : _subordinateWidgets) {
					widget->_renderPos = _renderPos;
					widget->Draw();
					widget->DrawSubordinateWidgets();
				}
			}

			/// <summary>
			/// returns whether the window is currently open
			/// </summary>
			/// <returns>Whether the window is currently open</returns>
			virtual bool IsShown() { return _widgetFlags & WidgetFlags_Shown; }
			/// <summary>
			/// returns whether this widget was already rendered once
			/// </summary>
			/// <returns></returns>
			virtual bool WasShown() { return (_widgetFlags & WidgetFlags_WasShown) > 0; }

			/// <summary>
			/// Sets whether the window was already rendered
			/// </summary>
			virtual void SetWasShown() { _widgetFlags |= WidgetFlags_WasShown; };
			/// <summary>
			/// Unsets the window was already shown flag
			/// </summary>
			virtual void UnsetWasShown() { _widgetFlags &= ~WidgetFlags_WasShown; };

			/// <summary>
			/// This function is called by ui framework and should update all necessary values.
			/// This roundabout way should be preferred over manually updating values as that may cause problem when a frame is currently being rendered
			/// </summary>
			virtual void UpdateWidget() = 0;

			/// <summary>
			/// Sets the widgets location
			/// Do not use this while the widget is being shown
			/// </summary>
			virtual void SetLocationScreenPos(Offset offsetX, Offset offsetY, Dimension width, Dimension height, bool scaleWidget)
			{
				_widgetLocation.anchor = WidgetAnchor::ScreenPos;
				_widgetLocation.screenPosLocation.offsetX = offsetX;
				_widgetLocation.screenPosLocation.offsetY = offsetY;
				_widgetLocation.screenPosLocation.height = height;
				_widgetLocation.screenPosLocation.width = width;
				_widgetLocation.screenPosLocation.scaleWidget = scaleWidget;
			}

			/// <summary>
			/// sets the widgets location for and object anchored widget
			/// </summary>
			/// <param name="offsetX"></param>
			/// <param name="offsetY"></param>
			/// <param name="width"></param>
			/// <param name="height"></param>
			/// <param name="scaleWidget"></param>
			/// <param name="widgetAnchorPoint"></param>
			/// <param name="distanceMinScale"></param>
			virtual void SetLocationObjectAnchor(Offset offsetX, Offset offsetY, Dimension width, Dimension height, bool scaleWidget, Alignment widgetAnchorPoint, float distanceMinScale, RE::ObjectRefHandle anchorObject, LocationObjectAnchor::ObjectAnchor anchor = LocationObjectAnchor::ObjectAnchor::ActorTorso, bool allowObjectInvisibility = false, bool allowObjectOccluded = true)
			{
				_widgetLocation.anchor = WidgetAnchor::ObjectAnchor;
				_widgetLocation.objectAnchorLocation = {};
				_widgetLocation.objectAnchorLocation.offsetX = offsetX;
				_widgetLocation.objectAnchorLocation.offsetY = offsetY;
				_widgetLocation.objectAnchorLocation.height = height;
				_widgetLocation.objectAnchorLocation.width = width;
				_widgetLocation.objectAnchorLocation.scaleWidget = scaleWidget;
				_widgetLocation.objectAnchorLocation.widgetAnchorPoint = widgetAnchorPoint;
				_widgetLocation.objectAnchorLocation.distanceMinScale = distanceMinScale;
				_widgetLocation.objectAnchorLocation.anchorObject = anchorObject;
				_widgetLocation.objectAnchorLocation.anchor = anchor;
				_widgetLocation.objectAnchorLocation.allowObjectInvisibility = allowObjectInvisibility;
				_widgetLocation.objectAnchorLocation.allowObjectOccluded = allowObjectOccluded;
			}

			/// <summary>
			/// Sets the widgets location
			/// Do not use this while the widget is being shown
			/// </summary>
			virtual void SetLocation(Location location)
			{
				_widgetLocation = location;
			}

			/// <summary>
			/// Sets the renderposition for this frame
			/// </summary>
			/// <param name="pos"></param>
			virtual void SetRenderPosition(RenderPosition pos)
			{
				_renderPos = pos;
			}

			/// <summary>
			/// returns the render position of the widget for this frame
			/// </summary>
			/// <returns></returns>
			virtual RenderPosition& GetRenderPosition()
			{
				return _renderPos;
			}

			/// <summary>
			/// returns the location of the widget
			/// </summary>
			/// <returns></returns>
			virtual Location& GetLocation()
			{
				return _widgetLocation;
			}

			/// <summary>
			/// loads the widgets data from the given string
			/// </summary>
			/// <param name=""></param>
			virtual void LoadDataFromString(util::string) = 0;

			/// <summary>
			/// returns the widgets data as a string
			/// </summary>
			/// <returns></returns>
			virtual util::string GetStringFromData() = 0;

			/// <summary>
			/// returns the unqiue identifier of the widget if it has one
			/// </summary>
			/// <returns></returns>
			virtual util::string GetIdentifier()
			{
				return _identifier;
			}

			/// <summary>
			/// Sets the unique identifier of the widget.
			/// To mark a widget as generic set the identifier to "".
			/// </summary>
			/// <param name="identifier"></param>
			virtual void SetIdentifier(util::string identifier)
			{
				_identifier = identifier;
			}

			/// <summary>
			/// Calculates the actual space used up by the widget given the available [maxWidth] and [maxHeight]
			/// </summary>
			virtual Dimensions& CalcSize(float maxWidth, float maxHeight) = 0;

			/// <summary>
			/// Returns the last calculated size
			/// </summary>
			virtual Dimensions& GetSize()
			{
				return _size;
			}

			virtual bool SkipDrawing()
			{
				return (_widgetFlags & WidgetFlags_SkipDrawing) > 0;
			}

			virtual void DoSkipDrawing(bool skip)
			{
				if (skip)
					_widgetFlags |= WidgetFlags_SkipDrawing;
				else
					_widgetFlags &= ~WidgetFlags_SkipDrawing;
			}

			virtual RE::ObjectRefHandle GetAnchorObject()
			{
				if (_widgetLocation.anchor == WidgetAnchor::ObjectAnchor)
					return _widgetLocation.objectAnchorLocation.anchorObject;
				else
					return {};
			}

			virtual void SetAnchorObject(RE::ObjectRefHandle object)
			{
				if (_widgetLocation.anchor == WidgetAnchor::ObjectAnchor)
					_widgetLocation.objectAnchorLocation.anchorObject = object;
			}

			virtual void AddSubordinateWidget(util::shared_ptr<Widget> widget)
			{
				if (auto itr = std::find(_subordinateWidgets.begin(), _subordinateWidgets.end(), widget); itr == _subordinateWidgets.end())
					_subordinateWidgets.push_back(widget);
			}

			virtual void RemoveSubordinateWidget(util::shared_ptr<Widget> widget)
			{
				if (auto itr = std::find(_subordinateWidgets.begin(), _subordinateWidgets.end(), widget); itr != _subordinateWidgets.end())
					_subordinateWidgets.erase(itr);
			}

			virtual void RemoveAllSubordinateWidgets()
			{
				_subordinateWidgets.clear();
			}

			/// <summary>
			/// Creates a duplicate of the widget with all members including the identifier
			/// If cloning is not available an empty ponter is returned
			/// </summary>
			/// <returns></returns>
			virtual util::shared_ptr<Widget> Clone()
			{
				return {};
			}
			/// <summary>
			/// Returns whether this widget supports duplication
			/// </summary>
			/// <returns></returns>
			virtual bool CloneAvailable()
			{
				return false;
			}
		};

		/// <summary>
		/// Widget panel that organizes widgets into rows and columns and draws them
		/// </summary>
		class IWidgetPanel : virtual public Widget
		{
		protected:
			/// <summary>
			/// maximum number of rows in the panel
			/// </summary>
			int _rows = 100;
			/// <summary>
			/// maximum number of columns in the panel
			/// </summary>
			int _columns = 4;
			/// <summary>
			///	the alignment of the text within the widget
			/// </summary>
			Alignment _alignment;

		public:
			/// <summary>
			/// Adds a widget to the panel
			/// </summary>
			/// <param name="widget"></param>
			virtual void AddWidget(util::shared_ptr<Widget> widget) = 0;
			/// <summary>
			/// Removes a wiidget from the panel
			/// </summary>
			/// <param name="widget"></param>
			virtual void RemoveWidget(util::shared_ptr<Widget> widget) = 0;
			/// <summary>
			/// Removes a wiidget from the panel
			/// </summary>
			/// <param name="widget"></param>
			virtual void RemoveWidget(util::string identifier) = 0;
			/// <summary>
			/// Removes all widgets from the panel
			/// </summary>
			virtual void RemoveAllWidgets() = 0;
			/// <summary>
			/// Sets the maximum number of rows to be displayed
			/// </summary>
			/// <param name="rows"></param>
			virtual void SetRows(int rows)
			{
				_rows = rows;
			}
			/// <summary>
			/// Sets the maximum number of columns
			/// </summary>
			/// <param name="columns"></param>
			virtual void SetColumns(int columns)
			{
				_columns = columns;
			}
			/// <summary>
			/// Sets the fill direction of the panel
			/// </summary>
			/// <param name="alignment"></param>
			virtual void SetFillDirection(Alignment alignment)
			{
				_alignment = alignment;
			}
		};

		/// <summary>
		/// Widget that draws a texture within its bounds
		/// </summary>
		class ITextureWidget : virtual public Widget
		{
		protected:
			/// <summary>
			///	relative size of x to y for the image (i.e, 16:9, 1:1)
			/// </summary>
			Dimensions _scale;

			/// <summary>
			/// colour multiplied with base texture colours
			/// byte order from highest to lowest -> a,b,g,r
			/// </summary>
			uint32_t _multiplyTextureColour = 0xFFFFFFFF;

		public:
			/// <summary>
			/// Set the dimensions of the widget content in relative values
			/// </summary>
			/// <param name="scale"></param>
			virtual void SetDimensions(Dimensions scale)
			{
				_scale = scale;
			}
			/// <summary>
			/// reeturns the relative dimensions of the widget content
			/// </summary>
			/// <returns></returns>
			virtual Dimensions& GetDimensions()
			{
				return _scale;
			}

			virtual void SetTexturePath(util::string path) = 0;

			virtual void SetMultiplyTextureColour(uint32_t colour)
			{
				_multiplyTextureColour = colour;
			}
		};

		class ICooldownWidget : public virtual Widget
		{
		public:
			enum class CooldownStyle
			{
				/// <summary>
				/// Like a Pie chart
				/// </summary>
				FilledCircleRadialSweep = 0,
				FilledRectangleHorizontalSweep = 1,
			};

		protected:
			CooldownStyle _style = CooldownStyle::FilledCircleRadialSweep;

			/// <summary>
			/// progress of the coolown [0.f - 1.f]
			/// </summary>
			float _progress = 0.f;

			uint32_t _cooldownColourBase = 0x33FFFFFF;
			uint32_t _cooldownColourHighlight = 0x66FFFFFF;

		public:
			/// <summary>
			/// Sets the cooldown style of the widget
			/// </summary>
			/// <param name="style"></param>
			void SetCooldownStyle(CooldownStyle style)
			{
				_style = style;
			}

			/// <summary>
			/// returns the cooldown style of the widget
			/// </summary>
			/// <returns></returns>
			CooldownStyle GetCooldownStyle()
			{
				return _style;
			}

			/// <summary>
			/// updates the current progress
			/// </summary>
			/// <param name="progress"></param>
			void SetProgress(float progress)
			{
				_progress = progress;
			}

			/// <summary>
			/// Sets the base colour of the cooldown widget
			/// </summary>
			/// <param name="colour"></param>
			void SetCooldownBaseColour(uint32_t colour)
			{
				_cooldownColourBase = colour;
			}

			/// <summary>
			/// Sets the highlight colour of the cooldown widget
			/// </summary>
			/// <param name="colour"></param>
			void SetCooldownHghlightColour(uint32_t colour)
			{
				_cooldownColourHighlight = colour;
			}
		};

		/// <summary>
		/// Simple widget that draws text
		/// </summary>
		class ITextWidget : virtual public Widget
		{
		protected:
			/// <summary>
			///	text to be displayed by the widget
			/// </summary>
			util::string _text;

			/// <summary>
			///	the alignment of the text within the widget
			/// </summary>
			Alignment _alignment;

			/// <summary>
			///	fontsize used by the text
			/// </summary>
			float _fontSize = 20.f;

			/// <summary>
			/// colour of the text
			/// byte order from highest to lowest -> a,b,g,r
			/// </summary>
			uint32_t _textColour = 0xFFF0F0F0;

		public:
			/// <summary>
			///	Sets the text to be displayed by the widget
			/// </summary>
			virtual void SetText(util::string text)
			{
				_text = text;
			}

			/// <summary>
			///	Sets the fontsize used by the text
			/// </summary>
			virtual void SetFontSize(float fontSize)
			{
				_fontSize = fontSize;
			}

			virtual void SetTextAlignment(Alignment alignment)
			{
				_alignment = alignment;
			}

			virtual void SetTextColour(uint32_t colour)
			{
				_textColour = colour;
			}
		};

		class ITextureAndTextWidget : virtual public ITextureWidget, virtual public ITextWidget
		{
		protected:
		public:
		};

		class IWidgetActorPanel : virtual public IWidgetPanel
		{
		protected:
		public:
		};

		/// <summary>
		/// Draws multiple widget on top of each other
		/// </summary>
		class IWidgetStack : virtual public Widget
		{
		protected:
		public:
		};

		/// <summary>
		/// Draws a bar
		/// </summary>
		class IBarWidget : virtual public Widget
		{
		public:
			enum class BarOrientation
			{
				Horizontal = 0,
				Vertical = 1,
			};

		protected:
			/// <summary>
			/// orientation of the bar
			/// </summary>
			BarOrientation _barOrientation;

			/// <summary>
			/// direction in which the base will be filled
			/// </summary>
			Alignment _fillDirection;
			/// <summary>
			/// current value of the bar
			/// </summary>
			float _currentValue = 0.f;
			float _maxValue = 100.f;

			/// <summary>
			/// current width of the bar
			/// </summary>
			float _currentWidth = 1.f;

			/// <summary>
			/// function delegate used to update the current and max value of the progress bar (between 0.f and 1.f) and the current length of the bar between (0.f, and 1.f)
			/// </summary>
			std::function<void(float& currentValue, float& _maxValue, float& currentBarLength)> _updateProgress;

			/// <summary>
			/// base fill colour of the bar, actual colour may depend of the bartype
			/// </summary>
			uint32_t _baseFillColour = 0xFFFFFFFF;

		public:
			/// <summary>
			/// returns the orientation of the bar
			/// </summary>
			/// <returns></returns>
			BarOrientation GetOrientation()
			{
				return _barOrientation;
			}
			/// <summary>
			/// sets the orientation of the bar
			/// </summary>
			/// <param name="orientation"></param>
			void SetBarOrientation(BarOrientation orientation)
			{
				_barOrientation = orientation;
			}
			/// <summary>
			/// returns the fill direction of the bar
			/// </summary>
			/// <returns></returns>
			Alignment GetFillDirection()
			{
				return _fillDirection;
			}
			/// <summary>
			/// sets the fill direction of the bar
			/// </summary>
			/// <param name="fillDirection"></param>
			void SetFillDirection(Alignment fillDirection)
			{
				_fillDirection = fillDirection;
			}

			/// <summary>
			/// Sets the callback used to update the progress and bar lengths
			/// </summary>
			/// <param name="updateProgress"></param>
			void SetUpdateProgressCallback(std::function<void(float& currentValue, float& _maxValue, float& currentBarLength)> updateProgress)
			{
				_updateProgress = updateProgress;
			}

			uint32_t GetBaseFillColour()
			{
				return _baseFillColour;
			}

			void SetBaseFillColour(uint32_t colour)
			{
				_baseFillColour = colour;
			}
		};
	}
}

