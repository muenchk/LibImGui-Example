#pragma once

#include "LibImGuiUI/Tab.h"
#include "LibImGuiUI/Window.h"
#include "imgui.h"

enum class FontType
{
	Daedric,
	Dragon,
	Dwemer,
	Falmer,
	Mage,
	NotoSansJP,
	MainTitle,
	HeaderFont,
	ButtonFont,
	SettingsFont,
	SmallFont,
};

#ifndef UTIL_STABLE_CLASSES
namespace util
{
	template <class T>
	class shared_ptr;
}
#endif

namespace LibImGuiUI
{
	enum class APIVersion
	{
		V1 = 1,
	};
	class LibImGuiUI_APIv1
	{
	private:
		/// <summary>
		/// Returns ImGui internal to initialize the callers ImGui
		/// </summary>
		/// <param name="allocFunc">The allocation function returned by ImGui::GetAllocatorFunctions</param>
		/// <param name="freeFunc">The deallocation function returned by ImGui::GetAllocatorFunctions</param>
		/// <param name="userData">The user data returned by ImGui::GetAllocatorFunctions</param>
		/// <param name="context">The currently active ImGui context</param>
		virtual void GetImGuiInternal(ImGuiMemAllocFunc* allocFunc, ImGuiMemFreeFunc* freeFunc, void** userData, ImGuiContext** context) = 0;

	public:
		static inline LibImGuiUI_APIv1* instance = nullptr;

		/// <summary>
		/// Returns the ImGui Font and scaled fontSize for the given fonttype
		/// </summary>
		/// <param name="fontType"></param>
		/// <returns></returns>
		virtual std::pair<ImFont*, float> GetFontByType(FontType fontType) = 0;
		/// <summary>
		/// Returns a user defined colour (defined in theme)
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		virtual ImVec4 GetUserColour(const char* name) = 0;
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
		/// Returns the number of currently open windows
		/// </summary>
		/// <returns></returns>
		virtual int GetNumberOfOpenWindows() = 0;
		/// <summary>
		/// Returns whether windows can currently be openened
		/// </summary>
		/// <returns></returns>
		virtual bool CanOpen() = 0;

		void InitImgui()
		{
			static bool loaded = false;
			if (!loaded) {
				loaded = true;
				ImGuiMemAllocFunc allocFunc;
				ImGuiMemFreeFunc freeFunc;
				void* userData;
				ImGuiContext* context;
				GetImGuiInternal(&allocFunc, &freeFunc, &userData, &context);
				ImGui::SetAllocatorFunctions(allocFunc, freeFunc, userData);
				ImGui::SetCurrentContext(context);
			}
		}
	};

	typedef void* (*_RequestPluginAPI)(APIVersion);

	[[nodiscard]] inline void* RequestPluginAPI(APIVersion version = APIVersion::V1)
	{
		std::string str = "LibImGuiUI.dll";
		std::wstring wstr = std::wstring(str.begin(), str.end());
		auto pluginHandle = GetModuleHandle(wstr.c_str());
		if (pluginHandle != 0) {
			_RequestPluginAPI requestAPIFunction = (_RequestPluginAPI)GetProcAddress(pluginHandle, "RequestPluginAPI");
			if (requestAPIFunction) {
				switch (version) {
				case APIVersion::V1:
					LibImGuiUI_APIv1::instance = (LibImGuiUI_APIv1*)requestAPIFunction(version);
					if (LibImGuiUI_APIv1::instance != nullptr)
						LibImGuiUI_APIv1::instance->InitImgui();
					return LibImGuiUI_APIv1::instance;
					break;
				}
			}
		}
		return nullptr;
	}
}

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
		std::function<decltype(dealloc_intern)> deallocator;

		using value_type = T;

		heap_allocator() noexcept
		{
			deallocator = &dealloc_intern;
		}
		template <class U>
		heap_allocator(const heap_allocator<U>& other) noexcept
		{
			deallocator = other.deallocator;
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
			deallocator(p);
		}
		void deallocate(T* p, std::size_t) noexcept
		{
			deallocator(p);
		}

		// Boost.Container's allocator interface needs a bit more than std::allocator;
		// see boost::container::allocator_traits — it fills in defaults for
		// construct/destroy/max_size/propagate_on_* if you don't specify them.

		friend bool operator==(const heap_allocator&, const heap_allocator&) noexcept { return true; }
		friend bool operator!=(const heap_allocator&, const heap_allocator&) noexcept { return false; }
	};

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

		virtual void Destroy(void* obj) = 0;
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
		virtual void Destroy(void* obj) override
		{
			if (_allocatorREF)
				_allocatorREF->dealloc(obj);
			else
				delete obj;
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
		virtual void Destroy(void* obj) override
		{
			((T*)(obj))->~T();
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
		long use_count() noexcept
		{
			return _crtl ? _crtl->_ref_count : 0;
		}
		long weak_count() noexcept
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
					_crtl->Destroy(_ref);
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
			PtrBase<T>::_crtl = new (mem) control_blockT<Y>();
			PtrBase<T>::_crtl->_allocatorCRTL = std::addressof(allocator);
			PtrBase<T>::_crtl->_allocatorREF = nullptr;
		}

		template <class Y, std::enable_if_t<std::is_convertible<Y*, T*>::value, int> = 0>
		explicit shared_ptr(Y* ptr, heap_allocator<void*>* allocator) noexcept
		{
			PtrBase<T>::_ref = ptr;
			void* mem = allocator->malloc(sizeof(control_blockT<Y>));
			PtrBase<T>::_crtl = new (mem) control_blockT<Y>();
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

	template <class T, class... _Args>
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
			(void)ret.ContstructFromWeak(*this);
			return ret;
		}

		friend struct std::less<weak_ptr<T>>;
		friend struct std::hash<weak_ptr<T>>;
	};
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
			return hash<util::control_block*>()(key._crtl);
		}
	};

	template <class T>
	struct hash<util::weak_ptr<T>>
	{
		[[nodiscard]] static size_t operator()(const util::weak_ptr<T>& key) noexcept
		{
			return hash<util::control_block*>()(key._crtl);
		}
	};
}
#endif
