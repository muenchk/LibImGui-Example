
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

namespace LibImGuiUIImGui
{
#define IM_COUNTOF(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))

	namespace ImGui
	{
		inline void* MemAlloc(size_t size);
		inline void MemFree(void* ptr);
	}

	typedef unsigned int ImGuiID;
	typedef signed char ImS8;
	typedef unsigned char ImU8;
	typedef signed short ImS16;
	typedef unsigned short ImU16;
	typedef signed int ImS32;
	typedef unsigned int ImU32;
	typedef signed long long ImS64;
	typedef unsigned long long ImU64;

	struct ImVec1
	{
		float x;
		constexpr ImVec1() : x(0.0f) {}
		constexpr ImVec1(float _x) : x(_x) {}
	};
	struct ImVec2
	{
		float x, y;
		constexpr ImVec2() : x(0.0f), y(0.0f) {}
		constexpr ImVec2(float _x, float _y) : x(_x), y(_y) {}
	};

	inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}
	inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
	}
	inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	struct ImVec4
	{
		float x, y, z, w;
		constexpr ImVec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		constexpr ImVec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	};
	struct ImNewWrapper
	{};
#define IM_ALLOC(_SIZE) ImGui::MemAlloc(_SIZE)
#define IM_FREE(_PTR) ImGui::MemFree(_PTR)
#define IM_PLACEMENT_NEW(_PTR) new (ImNewWrapper(), _PTR)
#define IM_NEW(_TYPE) new (ImNewWrapper(), ImGui::MemAlloc(sizeof(_TYPE))) _TYPE
	template <typename T>
	void IM_DELETE(T* p)
	{
		if (p) {
			p->~T();
			ImGui::MemFree(p);
		}
	}
#define IM_MEMALIGN(_OFF, _ALIGN) (((_OFF) + ((_ALIGN) - 1)) & ~((_ALIGN) - 1))

	template <typename T>
	struct ImVector
	{
		int Size;
		int Capacity;
		T* Data;

		typedef T value_type;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;

		inline ImVector()
		{
			Size = Capacity = 0;
			Data = NULL;
		}
		inline ImVector(const ImVector<T>& src)
		{
			Size = Capacity = 0;
			Data = NULL;
			operator=(src);
		}
		inline ImVector<T>& operator=(const ImVector<T>& src)
		{
			clear();
			resize(src.Size);
			if (Data && src.Data)
				memcpy(Data, src.Data, (size_t)Size * sizeof(T));
			return *this;
		}
		inline ~ImVector()
		{
			if (Data)
				IM_FREE(Data);
		}

		inline void clear()
		{
			if (Data) {
				Size = Capacity = 0;
				IM_FREE(Data);
				Data = NULL;
			}
		}
		inline void clear_delete()
		{
			for (int n = 0; n < Size; n++) IM_DELETE(Data[n]);
			clear();
		}
		inline void clear_destruct()
		{
			for (int n = 0; n < Size; n++) Data[n].~T();
			clear();
		}

		inline bool empty() const { return Size == 0; }
		inline int size() const { return Size; }
		inline int size_in_bytes() const { return Size * (int)sizeof(T); }
		inline int max_size() const { return 0x7FFFFFFF / (int)sizeof(T); }
		inline int capacity() const { return Capacity; }
		inline T& operator[](int i)
		{
			assert(i >= 0 && i < Size);
			return Data[i];
		}
		inline const T& operator[](int i) const
		{
			assert(i >= 0 && i < Size);
			return Data[i];
		}

		inline T* begin() { return Data; }
		inline const T* begin() const { return Data; }
		inline T* end() { return Data + Size; }
		inline const T* end() const { return Data + Size; }
		inline T& front()
		{
			assert(Size > 0);
			return Data[0];
		}
		inline const T& front() const
		{
			assert(Size > 0);
			return Data[0];
		}
		inline T& back()
		{
			assert(Size > 0);
			return Data[Size - 1];
		}
		inline const T& back() const
		{
			assert(Size > 0);
			return Data[Size - 1];
		}
		inline void swap(ImVector<T>& rhs)
		{
			int rhs_size = rhs.Size;
			rhs.Size = Size;
			Size = rhs_size;
			int rhs_cap = rhs.Capacity;
			rhs.Capacity = Capacity;
			Capacity = rhs_cap;
			T* rhs_data = rhs.Data;
			rhs.Data = Data;
			Data = rhs_data;
		}

		inline int _grow_capacity(int sz) const
		{
			int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8;
			return new_capacity > sz ? new_capacity : sz;
		}
		inline void resize(int new_size)
		{
			if (new_size > Capacity)
				reserve(_grow_capacity(new_size));
			Size = new_size;
		}
		inline void resize(int new_size, const T& v)
		{
			if (new_size > Capacity)
				reserve(_grow_capacity(new_size));
			if (new_size > Size)
				for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v));
			Size = new_size;
		}
		inline void shrink(int new_size)
		{
			assert(new_size <= Size);
			Size = new_size;
		}
		inline void reserve(int new_capacity)
		{
			if (new_capacity <= Capacity)
				return;
			T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T));
			if (Data) {
				memcpy(new_data, Data, (size_t)Size * sizeof(T));
				IM_FREE(Data);
			}
			Data = new_data;
			Capacity = new_capacity;
		}
		inline void reserve_discard(int new_capacity)
		{
			if (new_capacity <= Capacity)
				return;
			if (Data)
				IM_FREE(Data);
			Data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T));
			Capacity = new_capacity;
		}

		inline void push_back(const T& v)
		{
			if (Size == Capacity)
				reserve(_grow_capacity(Size + 1));
			memcpy(&Data[Size], &v, sizeof(v));
			Size++;
		}
		inline void pop_back()
		{
			assert(Size > 0);
			Size--;
		}
		inline void push_front(const T& v)
		{
			if (Size == 0)
				push_back(v);
			else
				insert(Data, v);
		}
		inline T* erase(const T* it)
		{
			assert(it >= Data && it < Data + Size);
			const ptrdiff_t off = it - Data;
			memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(T));
			Size--;
			return Data + off;
		}
		inline T* erase(const T* it, const T* it_last)
		{
			assert(it >= Data && it < Data + Size && it_last >= it && it_last <= Data + Size);
			const ptrdiff_t count = it_last - it;
			const ptrdiff_t off = it - Data;
			memmove(Data + off, Data + off + count, ((size_t)Size - (size_t)off - (size_t)count) * sizeof(T));
			Size -= (int)count;
			return Data + off;
		}
		inline T* erase_unsorted(const T* it)
		{
			assert(it >= Data && it < Data + Size);
			const ptrdiff_t off = it - Data;
			if (it < Data + Size - 1)
				memcpy(Data + off, Data + Size - 1, sizeof(T));
			Size--;
			return Data + off;
		}
		inline T* insert(const T* it, const T& v)
		{
			assert(it >= Data && it <= Data + Size);
			const ptrdiff_t off = it - Data;
			if (Size == Capacity)
				reserve(_grow_capacity(Size + 1));
			if (off < (int)Size)
				memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T));
			memcpy(&Data[off], &v, sizeof(v));
			Size++;
			return Data + off;
		}
		inline bool contains(const T& v) const
		{
			const T* data = Data;
			const T* data_end = Data + Size;
			while (data < data_end)
				if (*data++ == v)
					return true;
			return false;
		}
		inline T* find(const T& v)
		{
			T* data = Data;
			const T* data_end = Data + Size;
			while (data < data_end)
				if (*data == v)
					break;
				else
					++data;
			return data;
		}
		inline const T* find(const T& v) const
		{
			const T* data = Data;
			const T* data_end = Data + Size;
			while (data < data_end)
				if (*data == v)
					break;
				else
					++data;
			return data;
		}
		inline int find_index(const T& v) const
		{
			const T* data_end = Data + Size;
			const T* it = find(v);
			if (it == data_end)
				return -1;
			const ptrdiff_t off = it - Data;
			return (int)off;
		}
		inline bool find_erase(const T& v)
		{
			const T* it = find(v);
			if (it < Data + Size) {
				erase(it);
				return true;
			}
			return false;
		}
		inline bool find_erase_unsorted(const T& v)
		{
			const T* it = find(v);
			if (it < Data + Size) {
				erase_unsorted(it);
				return true;
			}
			return false;
		}
		inline int index_from_ptr(const T* it) const
		{
			assert(it >= Data && it < Data + Size);
			const ptrdiff_t off = it - Data;
			return (int)off;
		}
	};
	struct Clipper;

	namespace ImGui
	{

		inline ImVec4 ColorConvertU32ToFloat4(ImU32 in);
		inline ImU32 ColorConvertFloat4ToU32(ImVec4 in);
		inline void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
		inline void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

		namespace detail
		{
			inline Clipper* ImGuiListClipper_Create();
			inline void ImGuiListClipper_Destroy(Clipper*);
		}
	}

	struct ImDrawChannel;
	struct ImDrawCmd;
	struct ImDrawData;
	struct ImDrawList_Opaque;
	struct ImDrawListSharedData;
	struct ImDrawListSplitter;
	struct ImDrawVert;
	struct ImFont;
	struct ImFontAtlas;
	struct ImFontAtlasBuilder;
	struct ImFontAtlasRect;
	struct ImFontBaked;
	struct ImFontConfig;
	struct ImFontGlyph;
	struct ImFontGlyphRangesBuilder;
	struct ImFontLoader;
	struct ImTextureData;
	struct ImTextureRect;
	struct ImColor;

	struct ImGuiContext;
	struct ImGuiIO;
	struct ImGuiInputTextCallbackData;
	struct ImGuiKeyData;
	struct ImGuiListClipper;
	struct ImGuiMultiSelectIO;
	struct ImGuiOnceUponAFrame;
	struct ImGuiPayload;
	struct ImGuiPlatformIO;
	struct ImGuiPlatformImeData;
	struct ImGuiSelectionBasicStorage;
	struct ImGuiSelectionExternalStorage;
	struct ImGuiSelectionRequest;
	struct ImGuiSizeCallbackData;
	struct ImGuiStorage;
	struct ImGuiStoragePair;
	struct ImGuiStyle;
	struct ImGuiTableSortSpecs;
	struct ImGuiTableColumnSortSpecs;
	struct ImGuiTextBuffer;
	struct ImGuiTextFilter;
	struct ImGuiViewport;

	enum ImGuiDir : int;
	enum ImGuiKey : int;
	enum ImGuiMouseSource : int;
	enum ImGuiSortDirection : ImU8;
	typedef int ImGuiCol;
	typedef int ImGuiCond;
	typedef int ImGuiDataType;
	typedef int ImGuiMouseButton;
	typedef int ImGuiMouseCursor;
	typedef int ImGuiStyleVar;
	typedef int ImGuiTableBgTarget;

	typedef int ImDrawFlags;
	typedef int ImDrawListFlags;
	typedef int ImDrawTextFlags;
	typedef int ImFontFlags;
	typedef int ImFontAtlasFlags;
	typedef int ImGuiBackendFlags;
	typedef int ImGuiButtonFlags;
	typedef int ImGuiChildFlags;
	typedef int ImGuiColorEditFlags;
	typedef int ImGuiConfigFlags;
	typedef int ImGuiComboFlags;
	typedef int ImGuiDragDropFlags;
	typedef int ImGuiFocusedFlags;
	typedef int ImGuiHoveredFlags;
	typedef int ImGuiInputFlags;
	typedef int ImGuiInputTextFlags;
	typedef int ImGuiItemFlags;
	typedef int ImGuiKeyChord;
	typedef int ImGuiListClipperFlags;
	typedef int ImGuiPopupFlags;
	typedef int ImGuiMultiSelectFlags;
	typedef int ImGuiSelectableFlags;
	typedef int ImGuiSliderFlags;
	typedef int ImGuiTabBarFlags;
	typedef int ImGuiTabItemFlags;
	typedef int ImGuiTableFlags;
	typedef int ImGuiTableColumnFlags;
	typedef int ImGuiTableRowFlags;
	typedef int ImGuiTreeNodeFlags;
	typedef int ImGuiViewportFlags;
	typedef int ImGuiWindowFlags;

	typedef unsigned int ImWchar32;
	typedef unsigned short ImWchar16;
	typedef ImWchar16 ImWchar;

	struct ImBitVector;
	struct ImRect;
	struct ImGuiTextIndex;

	struct ImDrawDataBuilder;
	struct ImFontAtlasBuilder;
	struct ImFontAtlasPostProcessData;
	struct ImFontAtlasRectEntry;

	struct ImGuiBoxSelectState;
	struct ImGuiColorMod;
	struct ImGuiContextHook;
	struct ImGuiDataTypeInfo;
	struct ImGuiDeactivatedItemData;
	struct ImGuiErrorRecoveryState;
	struct ImGuiGroupData;
	struct ImGuiInputTextState;
	struct ImGuiInputTextDeactivateData;
	struct ImGuiLastItemData;
	struct ImGuiLocEntry;
	struct ImGuiMenuColumns;
	struct ImGuiMultiSelectState;
	struct ImGuiMultiSelectTempData;
	struct ImGuiNavItemData;
	struct ImGuiMetricsConfig;
	struct ImGuiNextWindowData;
	struct ImGuiNextItemData;
	struct ImGuiOldColumnData;
	struct ImGuiOldColumns;
	struct ImGuiPopupData;
	struct ImGuiSettingsHandler;
	struct ImGuiStyleMod;
	struct ImGuiStyleVarInfo;
	struct ImGuiTabBar;
	struct ImGuiTabItem;
	struct ImGuiTable;
	struct ImGuiTableHeaderData;
	struct ImGuiTableColumn;
	struct ImGuiTableInstanceData;
	struct ImGuiTableTempData;
	struct ImGuiTableSettings;
	struct ImGuiTableColumnsSettings;
	struct ImGuiTreeNodeStackData;
	struct ImGuiTypingSelectState;
	struct ImGuiTypingSelectRequest;
	struct ImGuiWindow;
	struct ImGuiWindowTempData;
	struct ImGuiWindowSettings;

	enum ImGuiLocKey : int;
	typedef int ImGuiLayoutType;

	typedef int ImDrawTextFlags;
	typedef int ImGuiActivateFlags;
	typedef int ImGuiDebugLogFlags;
	typedef int ImGuiFocusRequestFlags;
	typedef int ImGuiItemStatusFlags;
	typedef int ImGuiOldColumnFlags;
	typedef int ImGuiLogFlags;
	typedef int ImGuiNavRenderCursorFlags;
	typedef int ImGuiNavMoveFlags;
	typedef int ImGuiNextItemDataFlags;
	typedef int ImGuiNextWindowDataFlags;
	typedef int ImGuiScrollFlags;
	typedef int ImGuiSeparatorFlags;
	typedef int ImGuiTextFlags;
	typedef int ImGuiTooltipFlags;
	typedef int ImGuiTypingSelectFlags;
	typedef int ImGuiWindowBgClickFlags;
	typedef int ImGuiWindowRefreshFlags;

	typedef ImS16 ImGuiTableColumnIdx;
	typedef ImU16 ImGuiTableDrawChannelIdx;

	typedef void (*ImDrawCallback)(const ImDrawList_Opaque* parent_list, const ImDrawCmd* cmd);
	typedef void (*ImGuiDemoMarkerCallback)(const char* file, int line, const char* section);
	typedef void (*ImGuiErrorCallback)(ImGuiContext* ctx, void* user_data, const char* msg);

	typedef struct ImGuiInputTextCallbackData ImGuiInputTextCallbackData;
	typedef int (*ImGuiInputTextCallback)(ImGuiInputTextCallbackData* data);

	typedef void (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);

	typedef FILE* ImFileHandle;

#if (defined __SSE__ || defined __x86_64__ || defined _M_X64 || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1))) && !defined(IMGUI_DISABLE_SSE) && !defined(_M_ARM64) && !defined(_M_ARM64EC)
#	define IMGUI_ENABLE_SSE
#	include <immintrin.h>
#	if (defined __AVX__ || defined __SSE4_2__)
#		define IMGUI_ENABLE_SSE4_2
#		include <nmmintrin.h>
#	endif
#endif

#if defined(IMGUI_ENABLE_SSE4_2) && !defined(IMGUI_USE_LEGACY_CRC32_ADLER) && !defined(__EMSCRIPTEN__)
#	define IMGUI_ENABLE_SSE4_2_CRC
#endif

#define ImFabs(X) fabsf(X)
#define ImSqrt(X) sqrtf(X)
#define ImFmod(X, Y) fmodf((X), (Y))
#define ImCos(X) cosf(X)
#define ImSin(X) sinf(X)
#define ImAcos(X) acosf(X)
#define ImAtan2(Y, X) atan2f((Y), (X))
#define ImAtof(STR) atof(STR)
#define ImCeil(X) ceilf(X)
	inline float ImPow(float x, float y) { return powf(x, y); }
	inline double ImPow(double x, double y) { return pow(x, y); }
	inline float ImLog(float x) { return logf(x); }
	inline double ImLog(double x) { return log(x); }
	inline int ImAbs(int x) { return x < 0 ? -x : x; }
	inline float ImAbs(float x) { return fabsf(x); }
	inline double ImAbs(double x) { return fabs(x); }
	inline float ImSign(float x) { return (x < 0.0f) ? -1.0f : (x > 0.0f) ? 1.0f :
		                                                                    0.0f; }
	inline double ImSign(double x) { return (x < 0.0) ? -1.0 : (x > 0.0) ? 1.0 :
		                                                                   0.0; }
#ifdef IMGUI_ENABLE_SSE
	inline float ImRsqrt(float x) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); }
#else
	inline float ImRsqrt(float x) { return 1.0f / sqrtf(x); }
#endif
	inline double ImRsqrt(double x) { return 1.0 / sqrt(x); }

	template <typename T>
	T ImMin(T lhs, T rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}
	template <typename T>
	T ImMax(T lhs, T rhs)
	{
		return lhs >= rhs ? lhs : rhs;
	}
	template <typename T>
	T ImClamp(T v, T mn, T mx)
	{
		return (v < mn) ? mn : (v > mx) ? mx :
		                                  v;
	}
	template <typename T>
	T ImLerp(double a, double b, float t)
	{
		return (T)(a + (b - a) * (double)t);
	}
	template <typename T>
	T ImLerp(T a, T b, float t)
	{
		return (T)((float)a + (float)(b - a) * t);
	}
	template <typename T>
	void ImSwap(T& a, T& b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}
	template <typename T>
	T ImAddClampOverflow(T a, T b, T mn, T mx)
	{
		if (b < 0 && (a < mn - b))
			return mn;
		if (b > 0 && (a > mx - b))
			return mx;
		return a + b;
	}
	template <typename T>
	T ImSubClampOverflow(T a, T b, T mn, T mx)
	{
		if (b > 0 && (a < mn + b))
			return mn;
		if (b < 0 && (a > mx + b))
			return mx;
		return a - b;
	}

	inline ImVec2 ImMin(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
	inline ImVec2 ImMax(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
	inline ImVec2 ImClamp(const ImVec2& v, const ImVec2& mn, const ImVec2& mx) { return ImVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x :
																																	v.x,
		(v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y :
											 v.y); }
	inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, float t) { return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
	inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, const ImVec2& t) { return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
	inline ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t) { return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
	inline float ImSaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f :
		                                                                       f; }
	inline float ImLengthSqr(const ImVec2& lhs) { return (lhs.x * lhs.x) + (lhs.y * lhs.y); }
	inline float ImLengthSqr(const ImVec4& lhs) { return (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z) + (lhs.w * lhs.w); }
	inline float ImInvLength(const ImVec2& lhs, float fail_value)
	{
		float d = (lhs.x * lhs.x) + (lhs.y * lhs.y);
		if (d > 0.0f)
			return ImRsqrt(d);
		return fail_value;
	}
	inline float ImTrunc(float f) { return (float)(int)(f); }
	inline ImVec2 ImTrunc(const ImVec2& v) { return ImVec2((float)(int)(v.x), (float)(int)(v.y)); }
	inline float ImFloor(float f) { return (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1); }
	inline ImVec2 ImFloor(const ImVec2& v) { return ImVec2(ImFloor(v.x), ImFloor(v.y)); }
	inline float ImTrunc64(float f) { return (float)(ImS64)(f); }
	inline float ImRound64(float f) { return (float)(ImS64)(f + 0.5f); }
	inline int ImModPositive(int a, int b) { return (a + b) % b; }
	inline float ImDot(const ImVec2& a, const ImVec2& b) { return a.x * b.x + a.y * b.y; }
	inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a) { return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
	inline float ImLinearSweep(float current, float target, float speed)
	{
		if (current < target)
			return ImMin(current + speed, target);
		if (current > target)
			return ImMax(current - speed, target);
		return current;
	}
	inline float ImLinearRemapClamp(float s0, float s1, float d0, float d1, float x) { return ImSaturate((x - s0) / (s1 - s0)) * (d1 - d0) + d0; }
	inline ImVec2 ImMul(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
	inline bool ImIsFloatAboveGuaranteedIntegerPrecision(float f) { return f <= -16777216 || f >= 16777216; }
	inline float ImExponentialMovingAverage(float avg, float sample, int n)
	{
		avg -= avg / (float)n;
		avg += sample / (float)n;
		return avg;
	}

	ImVec2 ImBezierCubicCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, float t);
	ImVec2 ImBezierCubicClosestPoint(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, int num_segments);
	ImVec2 ImBezierCubicClosestPointCasteljau(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, float tess_tol);
	ImVec2 ImBezierQuadraticCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, float t);
	ImVec2 ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& p);
	bool ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
	ImVec2 ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
	void ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w);
	inline float ImTriangleArea(const ImVec2& a, const ImVec2& b, const ImVec2& c) { return ImFabs((a.x * (b.y - c.y)) + (b.x * (c.y - a.y)) + (c.x * (a.y - b.y))) * 0.5f; }
	inline bool ImTriangleIsClockwise(const ImVec2& a, const ImVec2& b, const ImVec2& c) { return ((b.x - a.x) * (c.y - b.y)) - ((c.x - b.x) * (b.y - a.y)) > 0.0f; }

#pragma region Enum Definitions

	enum ImGuiWindowFlags_
	{
		ImGuiWindowFlags_None = 0,
		ImGuiWindowFlags_NoTitleBar = 1 << 0,
		ImGuiWindowFlags_NoResize = 1 << 1,
		ImGuiWindowFlags_NoMove = 1 << 2,
		ImGuiWindowFlags_NoScrollbar = 1 << 3,
		ImGuiWindowFlags_NoScrollWithMouse = 1 << 4,
		ImGuiWindowFlags_NoCollapse = 1 << 5,
		ImGuiWindowFlags_AlwaysAutoResize = 1 << 6,
		ImGuiWindowFlags_NoBackground = 1 << 7,
		ImGuiWindowFlags_NoSavedSettings = 1 << 8,
		ImGuiWindowFlags_NoMouseInputs = 1 << 9,
		ImGuiWindowFlags_MenuBar = 1 << 10,
		ImGuiWindowFlags_HorizontalScrollbar = 1 << 11,
		ImGuiWindowFlags_NoFocusOnAppearing = 1 << 12,
		ImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13,
		ImGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14,
		ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,
		ImGuiWindowFlags_NoNavInputs = 1 << 16,
		ImGuiWindowFlags_NoNavFocus = 1 << 17,
		ImGuiWindowFlags_UnsavedDocument = 1 << 18,
		ImGuiWindowFlags_NoNav = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
		ImGuiWindowFlags_NoDecoration = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
		ImGuiWindowFlags_NoInputs = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

		ImGuiWindowFlags_ChildWindow = 1 << 24,
		ImGuiWindowFlags_Tooltip = 1 << 25,
		ImGuiWindowFlags_Popup = 1 << 26,
		ImGuiWindowFlags_Modal = 1 << 27,
		ImGuiWindowFlags_ChildMenu = 1 << 28,
	};

	enum ImGuiChildFlags_
	{
		ImGuiChildFlags_None = 0,
		ImGuiChildFlags_Borders = 1 << 0,
		ImGuiChildFlags_AlwaysUseWindowPadding = 1 << 1,
		ImGuiChildFlags_ResizeX = 1 << 2,
		ImGuiChildFlags_ResizeY = 1 << 3,
		ImGuiChildFlags_AutoResizeX = 1 << 4,
		ImGuiChildFlags_AutoResizeY = 1 << 5,
		ImGuiChildFlags_AlwaysAutoResize = 1 << 6,
		ImGuiChildFlags_FrameStyle = 1 << 7,
		ImGuiChildFlags_NavFlattened = 1 << 8,
	};
	enum ImGuiItemFlags_
	{
		ImGuiItemFlags_None = 0,
		ImGuiItemFlags_NoTabStop = 1 << 0,
		ImGuiItemFlags_NoNav = 1 << 1,
		ImGuiItemFlags_NoNavDefaultFocus = 1 << 2,
		ImGuiItemFlags_ButtonRepeat = 1 << 3,
		ImGuiItemFlags_AutoClosePopups = 1 << 4,
		ImGuiItemFlags_AllowDuplicateId = 1 << 5,
		ImGuiItemFlags_Disabled = 1 << 6,
	};
	enum ImGuiInputTextFlags_
	{

		ImGuiInputTextFlags_None = 0,
		ImGuiInputTextFlags_CharsDecimal = 1 << 0,
		ImGuiInputTextFlags_CharsHexadecimal = 1 << 1,
		ImGuiInputTextFlags_CharsScientific = 1 << 2,
		ImGuiInputTextFlags_CharsUppercase = 1 << 3,
		ImGuiInputTextFlags_CharsNoBlank = 1 << 4,

		ImGuiInputTextFlags_AllowTabInput = 1 << 5,
		ImGuiInputTextFlags_EnterReturnsTrue = 1 << 6,
		ImGuiInputTextFlags_EscapeClearsAll = 1 << 7,
		ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 8,

		ImGuiInputTextFlags_ReadOnly = 1 << 9,
		ImGuiInputTextFlags_Password = 1 << 10,
		ImGuiInputTextFlags_AlwaysOverwrite = 1 << 11,
		ImGuiInputTextFlags_AutoSelectAll = 1 << 12,
		ImGuiInputTextFlags_ParseEmptyRefVal = 1 << 13,
		ImGuiInputTextFlags_DisplayEmptyRefVal = 1 << 14,
		ImGuiInputTextFlags_NoHorizontalScroll = 1 << 15,
		ImGuiInputTextFlags_NoUndoRedo = 1 << 16,

		ImGuiInputTextFlags_ElideLeft = 1 << 17,

		ImGuiInputTextFlags_CallbackCompletion = 1 << 18,
		ImGuiInputTextFlags_CallbackHistory = 1 << 19,
		ImGuiInputTextFlags_CallbackAlways = 1 << 20,
		ImGuiInputTextFlags_CallbackCharFilter = 1 << 21,
		ImGuiInputTextFlags_CallbackResize = 1 << 22,
		ImGuiInputTextFlags_CallbackEdit = 1 << 23,

		ImGuiInputTextFlags_WordWrap = 1 << 24,
	};
	enum ImGuiTreeNodeFlags_
	{
		ImGuiTreeNodeFlags_None = 0,
		ImGuiTreeNodeFlags_Selected = 1 << 0,
		ImGuiTreeNodeFlags_Framed = 1 << 1,
		ImGuiTreeNodeFlags_AllowOverlap = 1 << 2,
		ImGuiTreeNodeFlags_NoTreePushOnOpen = 1 << 3,
		ImGuiTreeNodeFlags_NoAutoOpenOnLog = 1 << 4,
		ImGuiTreeNodeFlags_DefaultOpen = 1 << 5,
		ImGuiTreeNodeFlags_OpenOnDoubleClick = 1 << 6,
		ImGuiTreeNodeFlags_OpenOnArrow = 1 << 7,
		ImGuiTreeNodeFlags_Leaf = 1 << 8,
		ImGuiTreeNodeFlags_Bullet = 1 << 9,
		ImGuiTreeNodeFlags_FramePadding = 1 << 10,
		ImGuiTreeNodeFlags_SpanAvailWidth = 1 << 11,
		ImGuiTreeNodeFlags_SpanFullWidth = 1 << 12,
		ImGuiTreeNodeFlags_SpanLabelWidth = 1 << 13,
		ImGuiTreeNodeFlags_SpanAllColumns = 1 << 14,
		ImGuiTreeNodeFlags_LabelSpanAllColumns = 1 << 15,

		ImGuiTreeNodeFlags_NavLeftJumpsToParent = 1 << 17,
		ImGuiTreeNodeFlags_CollapsingHeader = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog,

		ImGuiTreeNodeFlags_DrawLinesNone = 1 << 18,
		ImGuiTreeNodeFlags_DrawLinesFull = 1 << 19,
		ImGuiTreeNodeFlags_DrawLinesToNodes = 1 << 20,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		ImGuiTreeNodeFlags_NavLeftJumpsBackHere = ImGuiTreeNodeFlags_NavLeftJumpsToParent,
		ImGuiTreeNodeFlags_SpanTextWidth = ImGuiTreeNodeFlags_SpanLabelWidth,

#endif
	};
	enum ImGuiPopupFlags_
	{
		ImGuiPopupFlags_None = 0,
		ImGuiPopupFlags_MouseButtonLeft = 1 << 2,
		ImGuiPopupFlags_MouseButtonRight = 2 << 2,
		ImGuiPopupFlags_MouseButtonMiddle = 3 << 2,
		ImGuiPopupFlags_NoReopen = 1 << 5,

		ImGuiPopupFlags_NoOpenOverExistingPopup = 1 << 7,
		ImGuiPopupFlags_NoOpenOverItems = 1 << 8,
		ImGuiPopupFlags_AnyPopupId = 1 << 10,
		ImGuiPopupFlags_AnyPopupLevel = 1 << 11,
		ImGuiPopupFlags_AnyPopup = ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel,
		ImGuiPopupFlags_MouseButtonShift_ = 2,
		ImGuiPopupFlags_MouseButtonMask_ = 0x0C,
		ImGuiPopupFlags_InvalidMask_ = 0x03,
	};
	enum ImGuiSelectableFlags_
	{
		ImGuiSelectableFlags_None = 0,
		ImGuiSelectableFlags_NoAutoClosePopups = 1 << 0,
		ImGuiSelectableFlags_SpanAllColumns = 1 << 1,
		ImGuiSelectableFlags_AllowDoubleClick = 1 << 2,
		ImGuiSelectableFlags_Disabled = 1 << 3,
		ImGuiSelectableFlags_AllowOverlap = 1 << 4,
		ImGuiSelectableFlags_Highlight = 1 << 5,
		ImGuiSelectableFlags_SelectOnNav = 1 << 6,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		ImGuiSelectableFlags_DontClosePopups = ImGuiSelectableFlags_NoAutoClosePopups,

#endif
	};
	enum ImGuiComboFlags_
	{
		ImGuiComboFlags_None = 0,
		ImGuiComboFlags_PopupAlignLeft = 1 << 0,
		ImGuiComboFlags_HeightSmall = 1 << 1,
		ImGuiComboFlags_HeightRegular = 1 << 2,
		ImGuiComboFlags_HeightLarge = 1 << 3,
		ImGuiComboFlags_HeightLargest = 1 << 4,
		ImGuiComboFlags_NoArrowButton = 1 << 5,
		ImGuiComboFlags_NoPreview = 1 << 6,
		ImGuiComboFlags_WidthFitPreview = 1 << 7,
		ImGuiComboFlags_HeightMask_ = ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest,
	};
	enum ImGuiTabBarFlags_
	{
		ImGuiTabBarFlags_None = 0,
		ImGuiTabBarFlags_Reorderable = 1 << 0,
		ImGuiTabBarFlags_AutoSelectNewTabs = 1 << 1,
		ImGuiTabBarFlags_TabListPopupButton = 1 << 2,
		ImGuiTabBarFlags_NoCloseWithMiddleMouseButton = 1 << 3,
		ImGuiTabBarFlags_NoTabListScrollingButtons = 1 << 4,
		ImGuiTabBarFlags_NoTooltip = 1 << 5,
		ImGuiTabBarFlags_DrawSelectedOverline = 1 << 6,

		ImGuiTabBarFlags_FittingPolicyMixed = 1 << 7,
		ImGuiTabBarFlags_FittingPolicyShrink = 1 << 8,
		ImGuiTabBarFlags_FittingPolicyScroll = 1 << 9,
		ImGuiTabBarFlags_FittingPolicyMask_ = ImGuiTabBarFlags_FittingPolicyMixed | ImGuiTabBarFlags_FittingPolicyShrink | ImGuiTabBarFlags_FittingPolicyScroll,
		ImGuiTabBarFlags_FittingPolicyDefault_ = ImGuiTabBarFlags_FittingPolicyMixed,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		ImGuiTabBarFlags_FittingPolicyResizeDown = ImGuiTabBarFlags_FittingPolicyShrink,
#endif
	};
	enum ImGuiTabItemFlags_
	{
		ImGuiTabItemFlags_None = 0,
		ImGuiTabItemFlags_UnsavedDocument = 1 << 0,
		ImGuiTabItemFlags_SetSelected = 1 << 1,
		ImGuiTabItemFlags_NoCloseWithMiddleMouseButton = 1 << 2,
		ImGuiTabItemFlags_NoPushId = 1 << 3,
		ImGuiTabItemFlags_NoTooltip = 1 << 4,
		ImGuiTabItemFlags_NoReorder = 1 << 5,
		ImGuiTabItemFlags_Leading = 1 << 6,
		ImGuiTabItemFlags_Trailing = 1 << 7,
		ImGuiTabItemFlags_NoAssumedClosure = 1 << 8,
	};
	enum ImGuiFocusedFlags_
	{
		ImGuiFocusedFlags_None = 0,
		ImGuiFocusedFlags_ChildWindows = 1 << 0,
		ImGuiFocusedFlags_RootWindow = 1 << 1,
		ImGuiFocusedFlags_AnyWindow = 1 << 2,
		ImGuiFocusedFlags_NoPopupHierarchy = 1 << 3,

		ImGuiFocusedFlags_RootAndChildWindows = ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows,
	};
	enum ImGuiHoveredFlags_
	{
		ImGuiHoveredFlags_None = 0,
		ImGuiHoveredFlags_ChildWindows = 1 << 0,
		ImGuiHoveredFlags_RootWindow = 1 << 1,
		ImGuiHoveredFlags_AnyWindow = 1 << 2,
		ImGuiHoveredFlags_NoPopupHierarchy = 1 << 3,

		ImGuiHoveredFlags_AllowWhenBlockedByPopup = 1 << 5,

		ImGuiHoveredFlags_AllowWhenBlockedByActiveItem = 1 << 7,
		ImGuiHoveredFlags_AllowWhenOverlappedByItem = 1 << 8,
		ImGuiHoveredFlags_AllowWhenOverlappedByWindow = 1 << 9,
		ImGuiHoveredFlags_AllowWhenDisabled = 1 << 10,
		ImGuiHoveredFlags_NoNavOverride = 1 << 11,
		ImGuiHoveredFlags_AllowWhenOverlapped = ImGuiHoveredFlags_AllowWhenOverlappedByItem | ImGuiHoveredFlags_AllowWhenOverlappedByWindow,
		ImGuiHoveredFlags_RectOnly = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped,
		ImGuiHoveredFlags_RootAndChildWindows = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows,

		ImGuiHoveredFlags_ForTooltip = 1 << 12,

		ImGuiHoveredFlags_Stationary = 1 << 13,
		ImGuiHoveredFlags_DelayNone = 1 << 14,
		ImGuiHoveredFlags_DelayShort = 1 << 15,
		ImGuiHoveredFlags_DelayNormal = 1 << 16,
		ImGuiHoveredFlags_NoSharedDelay = 1 << 17,
	};
	enum ImGuiDragDropFlags_
	{
		ImGuiDragDropFlags_None = 0,

		ImGuiDragDropFlags_SourceNoPreviewTooltip = 1 << 0,
		ImGuiDragDropFlags_SourceNoDisableHover = 1 << 1,
		ImGuiDragDropFlags_SourceNoHoldToOpenOthers = 1 << 2,
		ImGuiDragDropFlags_SourceAllowNullID = 1 << 3,
		ImGuiDragDropFlags_SourceExtern = 1 << 4,
		ImGuiDragDropFlags_PayloadAutoExpire = 1 << 5,
		ImGuiDragDropFlags_PayloadNoCrossContext = 1 << 6,
		ImGuiDragDropFlags_PayloadNoCrossProcess = 1 << 7,

		ImGuiDragDropFlags_AcceptBeforeDelivery = 1 << 10,
		ImGuiDragDropFlags_AcceptNoDrawDefaultRect = 1 << 11,
		ImGuiDragDropFlags_AcceptNoPreviewTooltip = 1 << 12,
		ImGuiDragDropFlags_AcceptDrawAsHovered = 1 << 13,
		ImGuiDragDropFlags_AcceptPeekOnly = ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		ImGuiDragDropFlags_SourceAutoExpirePayload = ImGuiDragDropFlags_PayloadAutoExpire,
#endif
	};
	enum ImGuiDataType_
	{
		ImGuiDataType_S8,
		ImGuiDataType_U8,
		ImGuiDataType_S16,
		ImGuiDataType_U16,
		ImGuiDataType_S32,
		ImGuiDataType_U32,
		ImGuiDataType_S64,
		ImGuiDataType_U64,
		ImGuiDataType_Float,
		ImGuiDataType_Double,
		ImGuiDataType_Bool,
		ImGuiDataType_String,
		ImGuiDataType_COUNT
	};
	enum ImGuiDir : int
	{
		ImGuiDir_None = -1,
		ImGuiDir_Left = 0,
		ImGuiDir_Right = 1,
		ImGuiDir_Up = 2,
		ImGuiDir_Down = 3,
		ImGuiDir_COUNT
	};
	enum ImGuiSortDirection : ImU8
	{
		ImGuiSortDirection_None = 0,
		ImGuiSortDirection_Ascending = 1,
		ImGuiSortDirection_Descending = 2
	};
	enum ImGuiKey : int
	{

		ImGuiKey_None = 0,
		ImGuiKey_NamedKey_BEGIN = 512,

		ImGuiKey_Tab = 512,
		ImGuiKey_LeftArrow,
		ImGuiKey_RightArrow,
		ImGuiKey_UpArrow,
		ImGuiKey_DownArrow,
		ImGuiKey_PageUp,
		ImGuiKey_PageDown,
		ImGuiKey_Home,
		ImGuiKey_End,
		ImGuiKey_Insert,
		ImGuiKey_Delete,
		ImGuiKey_Backspace,
		ImGuiKey_Space,
		ImGuiKey_Enter,
		ImGuiKey_Escape,
		ImGuiKey_LeftCtrl,
		ImGuiKey_LeftShift,
		ImGuiKey_LeftAlt,
		ImGuiKey_LeftSuper,
		ImGuiKey_RightCtrl,
		ImGuiKey_RightShift,
		ImGuiKey_RightAlt,
		ImGuiKey_RightSuper,
		ImGuiKey_Menu,
		ImGuiKey_0,
		ImGuiKey_1,
		ImGuiKey_2,
		ImGuiKey_3,
		ImGuiKey_4,
		ImGuiKey_5,
		ImGuiKey_6,
		ImGuiKey_7,
		ImGuiKey_8,
		ImGuiKey_9,
		ImGuiKey_A,
		ImGuiKey_B,
		ImGuiKey_C,
		ImGuiKey_D,
		ImGuiKey_E,
		ImGuiKey_F,
		ImGuiKey_G,
		ImGuiKey_H,
		ImGuiKey_I,
		ImGuiKey_J,
		ImGuiKey_K,
		ImGuiKey_L,
		ImGuiKey_M,
		ImGuiKey_N,
		ImGuiKey_O,
		ImGuiKey_P,
		ImGuiKey_Q,
		ImGuiKey_R,
		ImGuiKey_S,
		ImGuiKey_T,
		ImGuiKey_U,
		ImGuiKey_V,
		ImGuiKey_W,
		ImGuiKey_X,
		ImGuiKey_Y,
		ImGuiKey_Z,
		ImGuiKey_F1,
		ImGuiKey_F2,
		ImGuiKey_F3,
		ImGuiKey_F4,
		ImGuiKey_F5,
		ImGuiKey_F6,
		ImGuiKey_F7,
		ImGuiKey_F8,
		ImGuiKey_F9,
		ImGuiKey_F10,
		ImGuiKey_F11,
		ImGuiKey_F12,
		ImGuiKey_F13,
		ImGuiKey_F14,
		ImGuiKey_F15,
		ImGuiKey_F16,
		ImGuiKey_F17,
		ImGuiKey_F18,
		ImGuiKey_F19,
		ImGuiKey_F20,
		ImGuiKey_F21,
		ImGuiKey_F22,
		ImGuiKey_F23,
		ImGuiKey_F24,
		ImGuiKey_Apostrophe,
		ImGuiKey_Comma,
		ImGuiKey_Minus,
		ImGuiKey_Period,
		ImGuiKey_Slash,
		ImGuiKey_Semicolon,
		ImGuiKey_Equal,
		ImGuiKey_LeftBracket,
		ImGuiKey_Backslash,
		ImGuiKey_RightBracket,
		ImGuiKey_GraveAccent,
		ImGuiKey_CapsLock,
		ImGuiKey_ScrollLock,
		ImGuiKey_NumLock,
		ImGuiKey_PrintScreen,
		ImGuiKey_Pause,
		ImGuiKey_Keypad0,
		ImGuiKey_Keypad1,
		ImGuiKey_Keypad2,
		ImGuiKey_Keypad3,
		ImGuiKey_Keypad4,
		ImGuiKey_Keypad5,
		ImGuiKey_Keypad6,
		ImGuiKey_Keypad7,
		ImGuiKey_Keypad8,
		ImGuiKey_Keypad9,
		ImGuiKey_KeypadDecimal,
		ImGuiKey_KeypadDivide,
		ImGuiKey_KeypadMultiply,
		ImGuiKey_KeypadSubtract,
		ImGuiKey_KeypadAdd,
		ImGuiKey_KeypadEnter,
		ImGuiKey_KeypadEqual,
		ImGuiKey_AppBack,
		ImGuiKey_AppForward,
		ImGuiKey_Oem102,

		ImGuiKey_GamepadStart,
		ImGuiKey_GamepadBack,
		ImGuiKey_GamepadFaceLeft,
		ImGuiKey_GamepadFaceRight,
		ImGuiKey_GamepadFaceUp,
		ImGuiKey_GamepadFaceDown,
		ImGuiKey_GamepadDpadLeft,
		ImGuiKey_GamepadDpadRight,
		ImGuiKey_GamepadDpadUp,
		ImGuiKey_GamepadDpadDown,
		ImGuiKey_GamepadL1,
		ImGuiKey_GamepadR1,
		ImGuiKey_GamepadL2,
		ImGuiKey_GamepadR2,
		ImGuiKey_GamepadL3,
		ImGuiKey_GamepadR3,
		ImGuiKey_GamepadLStickLeft,
		ImGuiKey_GamepadLStickRight,
		ImGuiKey_GamepadLStickUp,
		ImGuiKey_GamepadLStickDown,
		ImGuiKey_GamepadRStickLeft,
		ImGuiKey_GamepadRStickRight,
		ImGuiKey_GamepadRStickUp,
		ImGuiKey_GamepadRStickDown,

		ImGuiKey_MouseLeft,
		ImGuiKey_MouseRight,
		ImGuiKey_MouseMiddle,
		ImGuiKey_MouseX1,
		ImGuiKey_MouseX2,
		ImGuiKey_MouseWheelX,
		ImGuiKey_MouseWheelY,

		ImGuiKey_ReservedForModCtrl,
		ImGuiKey_ReservedForModShift,
		ImGuiKey_ReservedForModAlt,
		ImGuiKey_ReservedForModSuper,

		ImGuiKey_NamedKey_END,
		ImGuiKey_NamedKey_COUNT = ImGuiKey_NamedKey_END - ImGuiKey_NamedKey_BEGIN,

		ImGuiMod_None = 0,
		ImGuiMod_Ctrl = 1 << 12,
		ImGuiMod_Shift = 1 << 13,
		ImGuiMod_Alt = 1 << 14,
		ImGuiMod_Super = 1 << 15,
		ImGuiMod_Mask_ = 0xF000,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		ImGuiKey_COUNT = ImGuiKey_NamedKey_END,
		ImGuiMod_Shortcut = ImGuiMod_Ctrl,

#endif
	};
	enum ImGuiInputFlags_
	{
		ImGuiInputFlags_None = 0,
		ImGuiInputFlags_Repeat = 1 << 0,

		ImGuiInputFlags_RouteActive = 1 << 10,
		ImGuiInputFlags_RouteFocused = 1 << 11,
		ImGuiInputFlags_RouteGlobal = 1 << 12,
		ImGuiInputFlags_RouteAlways = 1 << 13,

		ImGuiInputFlags_RouteOverFocused = 1 << 14,
		ImGuiInputFlags_RouteOverActive = 1 << 15,
		ImGuiInputFlags_RouteUnlessBgFocused = 1 << 16,
		ImGuiInputFlags_RouteFromRootWindow = 1 << 17,

		ImGuiInputFlags_Tooltip = 1 << 18,
	};
	enum ImGuiConfigFlags_
	{
		ImGuiConfigFlags_None = 0,
		ImGuiConfigFlags_NavEnableKeyboard = 1 << 0,
		ImGuiConfigFlags_NavEnableGamepad = 1 << 1,
		ImGuiConfigFlags_NoMouse = 1 << 4,
		ImGuiConfigFlags_NoMouseCursorChange = 1 << 5,
		ImGuiConfigFlags_NoKeyboard = 1 << 6,

		ImGuiConfigFlags_IsSRGB = 1 << 20,
		ImGuiConfigFlags_IsTouchScreen = 1 << 21,
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		ImGuiConfigFlags_NavEnableSetMousePos = 1 << 2,
		ImGuiConfigFlags_NavNoCaptureKeyboard = 1 << 3,
#endif
	};
	enum ImGuiBackendFlags_
	{
		ImGuiBackendFlags_None = 0,
		ImGuiBackendFlags_HasGamepad = 1 << 0,
		ImGuiBackendFlags_HasMouseCursors = 1 << 1,
		ImGuiBackendFlags_HasSetMousePos = 1 << 2,
		ImGuiBackendFlags_RendererHasVtxOffset = 1 << 3,
		ImGuiBackendFlags_RendererHasTextures = 1 << 4,
	};
	enum ImGuiCol_
	{
		ImGuiCol_Text,
		ImGuiCol_TextDisabled,
		ImGuiCol_WindowBg,
		ImGuiCol_ChildBg,
		ImGuiCol_PopupBg,
		ImGuiCol_Border,
		ImGuiCol_BorderShadow,
		ImGuiCol_FrameBg,
		ImGuiCol_FrameBgHovered,
		ImGuiCol_FrameBgActive,
		ImGuiCol_TitleBg,
		ImGuiCol_TitleBgActive,
		ImGuiCol_TitleBgCollapsed,
		ImGuiCol_MenuBarBg,
		ImGuiCol_ScrollbarBg,
		ImGuiCol_ScrollbarGrab,
		ImGuiCol_ScrollbarGrabHovered,
		ImGuiCol_ScrollbarGrabActive,
		ImGuiCol_CheckMark,
		ImGuiCol_CheckboxSelectedBg,
		ImGuiCol_SliderGrab,
		ImGuiCol_SliderGrabActive,
		ImGuiCol_Button,
		ImGuiCol_ButtonHovered,
		ImGuiCol_ButtonActive,
		ImGuiCol_Header,
		ImGuiCol_HeaderHovered,
		ImGuiCol_HeaderActive,
		ImGuiCol_Separator,
		ImGuiCol_SeparatorHovered,
		ImGuiCol_SeparatorActive,
		ImGuiCol_ResizeGrip,
		ImGuiCol_ResizeGripHovered,
		ImGuiCol_ResizeGripActive,
		ImGuiCol_InputTextCursor,
		ImGuiCol_TabHovered,
		ImGuiCol_Tab,
		ImGuiCol_TabSelected,
		ImGuiCol_TabSelectedOverline,
		ImGuiCol_TabDimmed,
		ImGuiCol_TabDimmedSelected,
		ImGuiCol_TabDimmedSelectedOverline,
		ImGuiCol_PlotLines,
		ImGuiCol_PlotLinesHovered,
		ImGuiCol_PlotHistogram,
		ImGuiCol_PlotHistogramHovered,
		ImGuiCol_TableHeaderBg,
		ImGuiCol_TableBorderStrong,
		ImGuiCol_TableBorderLight,
		ImGuiCol_TableRowBg,
		ImGuiCol_TableRowBgAlt,
		ImGuiCol_TextLink,
		ImGuiCol_TextSelectedBg,
		ImGuiCol_TreeLines,
		ImGuiCol_DragDropTarget,
		ImGuiCol_DragDropTargetBg,
		ImGuiCol_UnsavedMarker,
		ImGuiCol_NavCursor,
		ImGuiCol_NavWindowingHighlight,
		ImGuiCol_NavWindowingDimBg,
		ImGuiCol_ModalWindowDimBg,
		ImGuiCol_COUNT,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		ImGuiCol_TabActive = ImGuiCol_TabSelected,
		ImGuiCol_TabUnfocused = ImGuiCol_TabDimmed,
		ImGuiCol_TabUnfocusedActive = ImGuiCol_TabDimmedSelected,
		ImGuiCol_NavHighlight = ImGuiCol_NavCursor,
#endif
	};
	enum ImGuiStyleVar_
	{

		ImGuiStyleVar_Alpha,
		ImGuiStyleVar_DisabledAlpha,
		ImGuiStyleVar_WindowPadding,
		ImGuiStyleVar_WindowRounding,
		ImGuiStyleVar_WindowBorderSize,
		ImGuiStyleVar_WindowMinSize,
		ImGuiStyleVar_WindowTitleAlign,
		ImGuiStyleVar_ChildRounding,
		ImGuiStyleVar_ChildBorderSize,
		ImGuiStyleVar_PopupRounding,
		ImGuiStyleVar_PopupBorderSize,
		ImGuiStyleVar_FramePadding,
		ImGuiStyleVar_FrameRounding,
		ImGuiStyleVar_FrameBorderSize,
		ImGuiStyleVar_ItemSpacing,
		ImGuiStyleVar_ItemInnerSpacing,
		ImGuiStyleVar_IndentSpacing,
		ImGuiStyleVar_CellPadding,
		ImGuiStyleVar_ScrollbarSize,
		ImGuiStyleVar_ScrollbarRounding,
		ImGuiStyleVar_ScrollbarPadding,
		ImGuiStyleVar_GrabMinSize,
		ImGuiStyleVar_GrabRounding,
		ImGuiStyleVar_ImageRounding,
		ImGuiStyleVar_ImageBorderSize,
		ImGuiStyleVar_TabRounding,
		ImGuiStyleVar_TabBorderSize,
		ImGuiStyleVar_TabMinWidthBase,
		ImGuiStyleVar_TabMinWidthShrink,
		ImGuiStyleVar_TabBarBorderSize,
		ImGuiStyleVar_TabBarOverlineSize,
		ImGuiStyleVar_TableAngledHeadersAngle,
		ImGuiStyleVar_TableAngledHeadersTextAlign,
		ImGuiStyleVar_TreeLinesSize,
		ImGuiStyleVar_TreeLinesRounding,
		ImGuiStyleVar_DragDropTargetRounding,
		ImGuiStyleVar_ButtonTextAlign,
		ImGuiStyleVar_SelectableTextAlign,
		ImGuiStyleVar_SeparatorSize,
		ImGuiStyleVar_SeparatorTextBorderSize,
		ImGuiStyleVar_SeparatorTextAlign,
		ImGuiStyleVar_SeparatorTextPadding,
		ImGuiStyleVar_COUNT
	};
	enum ImGuiButtonFlags_
	{
		ImGuiButtonFlags_None = 0,
		ImGuiButtonFlags_MouseButtonLeft = 1 << 0,
		ImGuiButtonFlags_MouseButtonRight = 1 << 1,
		ImGuiButtonFlags_MouseButtonMiddle = 1 << 2,
		ImGuiButtonFlags_MouseButtonMask_ = ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle,
		ImGuiButtonFlags_EnableNav = 1 << 3,
		ImGuiButtonFlags_AllowOverlap = 1 << 12,
	};
	enum ImGuiPopupPositionPolicy
	{
		ImGuiPopupPositionPolicy_Default,
		ImGuiPopupPositionPolicy_ComboBox,
		ImGuiPopupPositionPolicy_Tooltip,
	};
	typedef void (*ImGuiContextHookCallback)(ImGuiContext* ctx, ImGuiContextHook* hook);
	enum ImGuiContextHookType
	{
		ImGuiContextHookType_NewFramePre,
		ImGuiContextHookType_NewFramePost,
		ImGuiContextHookType_EndFramePre,
		ImGuiContextHookType_EndFramePost,
		ImGuiContextHookType_RenderPre,
		ImGuiContextHookType_RenderPost,
		ImGuiContextHookType_Shutdown,
		ImGuiContextHookType_PendingRemoval_
	};
	enum ImGuiColorEditFlags_
	{
		ImGuiColorEditFlags_None = 0,
		ImGuiColorEditFlags_NoAlpha = 1 << 1,
		ImGuiColorEditFlags_NoPicker = 1 << 2,
		ImGuiColorEditFlags_NoOptions = 1 << 3,
		ImGuiColorEditFlags_NoSmallPreview = 1 << 4,
		ImGuiColorEditFlags_NoInputs = 1 << 5,
		ImGuiColorEditFlags_NoTooltip = 1 << 6,
		ImGuiColorEditFlags_NoLabel = 1 << 7,
		ImGuiColorEditFlags_NoSidePreview = 1 << 8,
		ImGuiColorEditFlags_NoDragDrop = 1 << 9,
		ImGuiColorEditFlags_NoBorder = 1 << 10,
		ImGuiColorEditFlags_NoColorMarkers = 1 << 11,

		ImGuiColorEditFlags_AlphaOpaque = 1 << 12,
		ImGuiColorEditFlags_AlphaNoBg = 1 << 13,
		ImGuiColorEditFlags_AlphaPreviewHalf = 1 << 14,

		ImGuiColorEditFlags_AlphaBar = 1 << 18,
		ImGuiColorEditFlags_HDR = 1 << 19,
		ImGuiColorEditFlags_DisplayRGB = 1 << 20,
		ImGuiColorEditFlags_DisplayHSV = 1 << 21,
		ImGuiColorEditFlags_DisplayHex = 1 << 22,
		ImGuiColorEditFlags_Uint8 = 1 << 23,
		ImGuiColorEditFlags_Float = 1 << 24,
		ImGuiColorEditFlags_PickerHueBar = 1 << 25,
		ImGuiColorEditFlags_PickerHueWheel = 1 << 26,
		ImGuiColorEditFlags_InputRGB = 1 << 27,
		ImGuiColorEditFlags_InputHSV = 1 << 28,

		ImGuiColorEditFlags_DefaultOptions_ = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueBar,

		ImGuiColorEditFlags_AlphaMask_ = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaOpaque | ImGuiColorEditFlags_AlphaNoBg | ImGuiColorEditFlags_AlphaPreviewHalf,
		ImGuiColorEditFlags_DisplayMask_ = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex,
		ImGuiColorEditFlags_DataTypeMask_ = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_Float,
		ImGuiColorEditFlags_PickerMask_ = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_PickerHueBar,
		ImGuiColorEditFlags_InputMask_ = ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_InputHSV,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		ImGuiColorEditFlags_AlphaPreview = 0,
#endif

	};
	enum ImGuiSliderFlags_
	{
		ImGuiSliderFlags_None = 0,
		ImGuiSliderFlags_Logarithmic = 1 << 5,
		ImGuiSliderFlags_NoRoundToFormat = 1 << 6,
		ImGuiSliderFlags_NoInput = 1 << 7,
		ImGuiSliderFlags_WrapAround = 1 << 8,
		ImGuiSliderFlags_ClampOnInput = 1 << 9,
		ImGuiSliderFlags_ClampZeroRange = 1 << 10,
		ImGuiSliderFlags_NoSpeedTweaks = 1 << 11,
		ImGuiSliderFlags_ColorMarkers = 1 << 12,
		ImGuiSliderFlags_AlwaysClamp = ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_ClampZeroRange,
		ImGuiSliderFlags_InvalidMask_ = 0x7000000F,
	};
	enum ImGuiMouseButton_
	{
		ImGuiMouseButton_Left = 0,
		ImGuiMouseButton_Right = 1,
		ImGuiMouseButton_Middle = 2,
		ImGuiMouseButton_COUNT = 5
	};
	enum ImGuiMouseCursor_
	{
		ImGuiMouseCursor_None = -1,
		ImGuiMouseCursor_Arrow = 0,
		ImGuiMouseCursor_TextInput,
		ImGuiMouseCursor_ResizeAll,
		ImGuiMouseCursor_ResizeNS,
		ImGuiMouseCursor_ResizeEW,
		ImGuiMouseCursor_ResizeNESW,
		ImGuiMouseCursor_ResizeNWSE,
		ImGuiMouseCursor_Hand,
		ImGuiMouseCursor_Wait,
		ImGuiMouseCursor_Progress,
		ImGuiMouseCursor_NotAllowed,
		ImGuiMouseCursor_COUNT
	};
	enum ImGuiCond_
	{
		ImGuiCond_None = 0,
		ImGuiCond_Always = 1 << 0,
		ImGuiCond_Once = 1 << 1,
		ImGuiCond_FirstUseEver = 1 << 2,
		ImGuiCond_Appearing = 1 << 3,
	};
	enum ImGuiTableFlags_
	{

		ImGuiTableFlags_None = 0,
		ImGuiTableFlags_Resizable = 1 << 0,
		ImGuiTableFlags_Reorderable = 1 << 1,
		ImGuiTableFlags_Hideable = 1 << 2,
		ImGuiTableFlags_Sortable = 1 << 3,
		ImGuiTableFlags_NoSavedSettings = 1 << 4,
		ImGuiTableFlags_ContextMenuInBody = 1 << 5,

		ImGuiTableFlags_RowBg = 1 << 6,
		ImGuiTableFlags_BordersInnerH = 1 << 7,
		ImGuiTableFlags_BordersOuterH = 1 << 8,
		ImGuiTableFlags_BordersInnerV = 1 << 9,
		ImGuiTableFlags_BordersOuterV = 1 << 10,
		ImGuiTableFlags_BordersH = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersOuterH,
		ImGuiTableFlags_BordersV = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV,
		ImGuiTableFlags_BordersInner = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersInnerH,
		ImGuiTableFlags_BordersOuter = ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersOuterH,
		ImGuiTableFlags_Borders = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter,
		ImGuiTableFlags_NoBordersInBody = 1 << 11,
		ImGuiTableFlags_NoBordersInBodyUntilResize = 1 << 12,

		ImGuiTableFlags_SizingFixedFit = 1 << 13,
		ImGuiTableFlags_SizingFixedSame = 2 << 13,
		ImGuiTableFlags_SizingStretchProp = 3 << 13,
		ImGuiTableFlags_SizingStretchSame = 4 << 13,

		ImGuiTableFlags_NoHostExtendX = 1 << 16,
		ImGuiTableFlags_NoHostExtendY = 1 << 17,
		ImGuiTableFlags_NoKeepColumnsVisible = 1 << 18,
		ImGuiTableFlags_PreciseWidths = 1 << 19,

		ImGuiTableFlags_NoClip = 1 << 20,

		ImGuiTableFlags_PadOuterX = 1 << 21,
		ImGuiTableFlags_NoPadOuterX = 1 << 22,
		ImGuiTableFlags_NoPadInnerX = 1 << 23,

		ImGuiTableFlags_ScrollX = 1 << 24,
		ImGuiTableFlags_ScrollY = 1 << 25,

		ImGuiTableFlags_SortMulti = 1 << 26,
		ImGuiTableFlags_SortTristate = 1 << 27,

		ImGuiTableFlags_HighlightHoveredColumn = 1 << 28,

		ImGuiTableFlags_SizingMask_ = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_SizingStretchSame,
	};
	enum ImGuiTableColumnFlags_
	{

		ImGuiTableColumnFlags_None = 0,
		ImGuiTableColumnFlags_Disabled = 1 << 0,
		ImGuiTableColumnFlags_DefaultHide = 1 << 1,
		ImGuiTableColumnFlags_DefaultSort = 1 << 2,
		ImGuiTableColumnFlags_WidthStretch = 1 << 3,
		ImGuiTableColumnFlags_WidthFixed = 1 << 4,
		ImGuiTableColumnFlags_NoResize = 1 << 5,
		ImGuiTableColumnFlags_NoReorder = 1 << 6,
		ImGuiTableColumnFlags_NoHide = 1 << 7,
		ImGuiTableColumnFlags_NoClip = 1 << 8,
		ImGuiTableColumnFlags_NoSort = 1 << 9,
		ImGuiTableColumnFlags_NoSortAscending = 1 << 10,
		ImGuiTableColumnFlags_NoSortDescending = 1 << 11,
		ImGuiTableColumnFlags_NoHeaderLabel = 1 << 12,
		ImGuiTableColumnFlags_NoHeaderWidth = 1 << 13,
		ImGuiTableColumnFlags_PreferSortAscending = 1 << 14,
		ImGuiTableColumnFlags_PreferSortDescending = 1 << 15,
		ImGuiTableColumnFlags_IndentEnable = 1 << 16,
		ImGuiTableColumnFlags_IndentDisable = 1 << 17,
		ImGuiTableColumnFlags_AngledHeader = 1 << 18,

		ImGuiTableColumnFlags_IsEnabled = 1 << 24,
		ImGuiTableColumnFlags_IsVisible = 1 << 25,
		ImGuiTableColumnFlags_IsSorted = 1 << 26,
		ImGuiTableColumnFlags_IsHovered = 1 << 27,

		ImGuiTableColumnFlags_WidthMask_ = ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_WidthFixed,
		ImGuiTableColumnFlags_IndentMask_ = ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_IndentDisable,
		ImGuiTableColumnFlags_StatusMask_ = ImGuiTableColumnFlags_IsEnabled | ImGuiTableColumnFlags_IsVisible | ImGuiTableColumnFlags_IsSorted | ImGuiTableColumnFlags_IsHovered,
		ImGuiTableColumnFlags_NoDirectResize_ = 1 << 30,
	};
	enum ImGuiTableRowFlags_
	{
		ImGuiTableRowFlags_None = 0,
		ImGuiTableRowFlags_Headers = 1 << 0,
	};
	enum ImGuiTableBgTarget_
	{
		ImGuiTableBgTarget_None = 0,
		ImGuiTableBgTarget_RowBg0 = 1,
		ImGuiTableBgTarget_RowBg1 = 2,
		ImGuiTableBgTarget_CellBg = 3,
	};
	enum ImGuiListClipperFlags_
	{
		ImGuiListClipperFlags_None = 0,
		ImGuiListClipperFlags_NoSetTableRowCounters = 1 << 0,
	};
	enum ImGuiMultiSelectFlags_
	{
		ImGuiMultiSelectFlags_None = 0,
		ImGuiMultiSelectFlags_SingleSelect = 1 << 0,
		ImGuiMultiSelectFlags_NoSelectAll = 1 << 1,
		ImGuiMultiSelectFlags_NoRangeSelect = 1 << 2,
		ImGuiMultiSelectFlags_NoAutoSelect = 1 << 3,
		ImGuiMultiSelectFlags_NoAutoClear = 1 << 4,
		ImGuiMultiSelectFlags_NoAutoClearOnReselect = 1 << 5,
		ImGuiMultiSelectFlags_BoxSelect1d = 1 << 6,
		ImGuiMultiSelectFlags_BoxSelect2d = 1 << 7,
		ImGuiMultiSelectFlags_BoxSelectNoScroll = 1 << 8,
		ImGuiMultiSelectFlags_ClearOnEscape = 1 << 9,
		ImGuiMultiSelectFlags_ClearOnClickVoid = 1 << 10,
		ImGuiMultiSelectFlags_ScopeWindow = 1 << 11,
		ImGuiMultiSelectFlags_ScopeRect = 1 << 12,
		ImGuiMultiSelectFlags_SelectOnAuto = 1 << 13,
		ImGuiMultiSelectFlags_SelectOnClickAlways = 1 << 14,
		ImGuiMultiSelectFlags_SelectOnClickRelease = 1 << 15,

		ImGuiMultiSelectFlags_NavWrapX = 1 << 16,
		ImGuiMultiSelectFlags_NoSelectOnRightClick = 1 << 17,
		ImGuiMultiSelectFlags_SelectOnMask_ = ImGuiMultiSelectFlags_SelectOnAuto | ImGuiMultiSelectFlags_SelectOnClickAlways | ImGuiMultiSelectFlags_SelectOnClickRelease,
	};
	enum ImDrawFlags_
	{
		ImDrawFlags_None = 0,
		ImDrawFlags_RoundCornersTopLeft = 1 << 4,
		ImDrawFlags_RoundCornersTopRight = 1 << 5,
		ImDrawFlags_RoundCornersBottomLeft = 1 << 6,
		ImDrawFlags_RoundCornersBottomRight = 1 << 7,
		ImDrawFlags_RoundCornersNone = 1 << 8,
		ImDrawFlags_Closed = 1 << 9,
		ImDrawFlags_RoundCornersTop = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight,
		ImDrawFlags_RoundCornersBottom = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
		ImDrawFlags_RoundCornersLeft = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersTopLeft,
		ImDrawFlags_RoundCornersRight = ImDrawFlags_RoundCornersBottomRight | ImDrawFlags_RoundCornersTopRight,
		ImDrawFlags_RoundCornersAll = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
		ImDrawFlags_RoundCornersDefault_ = ImDrawFlags_RoundCornersAll,
		ImDrawFlags_RoundCornersMask_ = ImDrawFlags_RoundCornersAll | ImDrawFlags_RoundCornersNone,
		ImDrawFlags_InvalidMask_ = (ImDrawFlags)0x8000000F,
	};
	enum ImDrawListFlags_
	{
		ImDrawListFlags_None = 0,
		ImDrawListFlags_AntiAliasedLines = 1 << 0,
		ImDrawListFlags_AntiAliasedLinesUseTex = 1 << 1,
		ImDrawListFlags_AntiAliasedFill = 1 << 2,
		ImDrawListFlags_AllowVtxOffset = 1 << 3,
	};
	enum ImFontAtlasFlags_
	{
		ImFontAtlasFlags_None = 0,
		ImFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,
		ImFontAtlasFlags_NoMouseCursors = 1 << 1,
		ImFontAtlasFlags_NoBakedLines = 1 << 2,
	};
	enum ImFontFlags_
	{
		ImFontFlags_None = 0,
		ImFontFlags_NoLoadError = 1 << 1,
		ImFontFlags_NoLoadGlyphs = 1 << 2,
		ImFontFlags_LockBakedSizes = 1 << 3,
		ImFontFlags_ImplicitRefSize = 1 << 4,
	};
	enum ImGuiViewportFlags_
	{
		ImGuiViewportFlags_None = 0,
		ImGuiViewportFlags_IsPlatformWindow = 1 << 0,
		ImGuiViewportFlags_IsPlatformMonitor = 1 << 1,
		ImGuiViewportFlags_OwnedByApp = 1 << 2,
	};
	enum ImDrawTextFlags_
	{
		ImDrawTextFlags_None = 0,
		ImDrawTextFlags_CpuFineClip = 1 << 0,
		ImDrawTextFlags_WrapKeepBlanks = 1 << 1,
		ImDrawTextFlags_StopOnNewLine = 1 << 2,
	};
	enum ImGuiItemFlagsPrivate_
	{

		ImGuiItemFlags_ReadOnly = 1 << 11,
		ImGuiItemFlags_MixedValue = 1 << 12,
		ImGuiItemFlags_NoWindowHoverableCheck = 1 << 13,
		ImGuiItemFlags_AllowOverlap = 1 << 14,
		ImGuiItemFlags_NoNavDisableMouseHover = 1 << 15,
		ImGuiItemFlags_NoMarkEdited = 1 << 16,
		ImGuiItemFlags_NoFocus = 1 << 17,

		ImGuiItemFlags_Inputable = 1 << 20,
		ImGuiItemFlags_HasSelectionUserData = 1 << 21,
		ImGuiItemFlags_IsMultiSelect = 1 << 22,

		ImGuiItemFlags_Default_ = ImGuiItemFlags_AutoClosePopups,

	};
	enum ImGuiItemStatusFlags_
	{
		ImGuiItemStatusFlags_None = 0,
		ImGuiItemStatusFlags_HoveredRect = 1 << 0,
		ImGuiItemStatusFlags_HasDisplayRect = 1 << 1,
		ImGuiItemStatusFlags_Edited = 1 << 2,
		ImGuiItemStatusFlags_ToggledSelection = 1 << 3,
		ImGuiItemStatusFlags_ToggledOpen = 1 << 4,
		ImGuiItemStatusFlags_HasDeactivated = 1 << 5,
		ImGuiItemStatusFlags_Deactivated = 1 << 6,
		ImGuiItemStatusFlags_HoveredWindow = 1 << 7,
		ImGuiItemStatusFlags_Visible = 1 << 8,
		ImGuiItemStatusFlags_HasClipRect = 1 << 9,
		ImGuiItemStatusFlags_HasShortcut = 1 << 10,

		ImGuiItemStatusFlags_EditedInternal = 1 << 11,

#ifdef IMGUI_ENABLE_TEST_ENGINE
		ImGuiItemStatusFlags_Openable = 1 << 20,
		ImGuiItemStatusFlags_Opened = 1 << 21,
		ImGuiItemStatusFlags_Checkable = 1 << 22,
		ImGuiItemStatusFlags_Checked = 1 << 23,
		ImGuiItemStatusFlags_Inputable = 1 << 24,
#endif
	};
	enum ImGuiHoveredFlagsPrivate_
	{
		ImGuiHoveredFlags_DelayMask_ = ImGuiHoveredFlags_DelayNone | ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay,
		ImGuiHoveredFlags_AllowedMaskForIsWindowHovered = ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_NoPopupHierarchy | ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_Stationary,
		ImGuiHoveredFlags_AllowedMaskForIsItemHovered = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped | ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_NoNavOverride | ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayMask_,
	};
	enum ImGuiInputTextFlagsPrivate_
	{

		ImGuiInputTextFlags_Multiline = 1 << 26,
		ImGuiInputTextFlags_TempInput = 1 << 27,
		ImGuiInputTextFlags_LocalizeDecimalPoint = 1 << 28,
	};
	enum ImGuiButtonFlagsPrivate_
	{
		ImGuiButtonFlags_PressedOnClick = 1 << 4,
		ImGuiButtonFlags_PressedOnClickRelease = 1 << 5,
		ImGuiButtonFlags_PressedOnClickReleaseAnywhere = 1 << 6,
		ImGuiButtonFlags_PressedOnRelease = 1 << 7,
		ImGuiButtonFlags_PressedOnDoubleClick = 1 << 8,
		ImGuiButtonFlags_PressedOnDragDropHold = 1 << 9,

		ImGuiButtonFlags_FlattenChildren = 1 << 11,

		ImGuiButtonFlags_AlignTextBaseLine = 1 << 15,
		ImGuiButtonFlags_NoKeyModsAllowed = 1 << 16,
		ImGuiButtonFlags_NoHoldingActiveId = 1 << 17,
		ImGuiButtonFlags_NoNavFocus = 1 << 18,
		ImGuiButtonFlags_NoHoveredOnFocus = 1 << 19,
		ImGuiButtonFlags_NoSetKeyOwner = 1 << 20,
		ImGuiButtonFlags_NoTestKeyOwner = 1 << 21,
		ImGuiButtonFlags_NoFocus = 1 << 22,
		ImGuiButtonFlags_PressedOnMask_ = ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnClickReleaseAnywhere | ImGuiButtonFlags_PressedOnRelease | ImGuiButtonFlags_PressedOnDoubleClick | ImGuiButtonFlags_PressedOnDragDropHold,
		ImGuiButtonFlags_PressedOnDefault_ = ImGuiButtonFlags_PressedOnClickRelease,

	};
	enum ImGuiComboFlagsPrivate_
	{
		ImGuiComboFlags_CustomPreview = 1 << 20,
	};
	enum ImGuiSliderFlagsPrivate_
	{
		ImGuiSliderFlags_Vertical = 1 << 20,
		ImGuiSliderFlags_ReadOnly = 1 << 21,
	};
	enum ImGuiSelectableFlagsPrivate_
	{

		ImGuiSelectableFlags_NoHoldingActiveID = 1 << 20,
		ImGuiSelectableFlags_SelectOnClick = 1 << 22,
		ImGuiSelectableFlags_SelectOnRelease = 1 << 23,
		ImGuiSelectableFlags_SpanAvailWidth = 1 << 24,
		ImGuiSelectableFlags_SetNavIdOnHover = 1 << 25,
		ImGuiSelectableFlags_NoPadWithHalfSpacing = 1 << 26,
		ImGuiSelectableFlags_NoSetKeyOwner = 1 << 27,
	};
	enum ImGuiTreeNodeFlagsPrivate_
	{
		ImGuiTreeNodeFlags_NoNavFocus = 1 << 27,
		ImGuiTreeNodeFlags_ClipLabelForTrailingButton = 1 << 28,
		ImGuiTreeNodeFlags_UpsideDownArrow = 1 << 29,
		ImGuiTreeNodeFlags_OpenOnMask_ = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow,
		ImGuiTreeNodeFlags_DrawLinesMask_ = ImGuiTreeNodeFlags_DrawLinesNone | ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DrawLinesToNodes,
	};
	enum ImGuiSeparatorFlags_
	{
		ImGuiSeparatorFlags_None = 0,
		ImGuiSeparatorFlags_Horizontal = 1 << 0,
		ImGuiSeparatorFlags_Vertical = 1 << 1,
		ImGuiSeparatorFlags_SpanAllColumns = 1 << 2,
	};
	enum ImGuiFocusRequestFlags_
	{
		ImGuiFocusRequestFlags_None = 0,
		ImGuiFocusRequestFlags_RestoreFocusedChild = 1 << 0,
		ImGuiFocusRequestFlags_UnlessBelowModal = 1 << 1,
	};
	enum ImGuiTextFlags_
	{
		ImGuiTextFlags_None = 0,
		ImGuiTextFlags_NoWidthForLargeClippedText = 1 << 0,
	};
	enum ImGuiTooltipFlags_
	{
		ImGuiTooltipFlags_None = 0,
		ImGuiTooltipFlags_OverridePrevious = 1 << 1,
	};
	enum ImGuiLayoutType_
	{
		ImGuiLayoutType_Horizontal = 0,
		ImGuiLayoutType_Vertical = 1
	};
	enum ImGuiLogFlags_
	{
		ImGuiLogFlags_None = 0,

		ImGuiLogFlags_OutputTTY = 1 << 0,
		ImGuiLogFlags_OutputFile = 1 << 1,
		ImGuiLogFlags_OutputBuffer = 1 << 2,
		ImGuiLogFlags_OutputClipboard = 1 << 3,
		ImGuiLogFlags_OutputMask_ = ImGuiLogFlags_OutputTTY | ImGuiLogFlags_OutputFile | ImGuiLogFlags_OutputBuffer | ImGuiLogFlags_OutputClipboard,
	};
	enum ImGuiAxis
	{
		ImGuiAxis_None = -1,
		ImGuiAxis_X = 0,
		ImGuiAxis_Y = 1
	};
	enum ImGuiPlotType
	{
		ImGuiPlotType_Lines,
		ImGuiPlotType_Histogram,
	};
	enum ImGuiWindowRefreshFlags_
	{
		ImGuiWindowRefreshFlags_None = 0,
		ImGuiWindowRefreshFlags_TryToAvoidRefresh = 1 << 0,
		ImGuiWindowRefreshFlags_RefreshOnHover = 1 << 1,
		ImGuiWindowRefreshFlags_RefreshOnFocus = 1 << 2,

	};
	enum ImGuiWindowBgClickFlags_
	{
		ImGuiWindowBgClickFlags_None = 0,
		ImGuiWindowBgClickFlags_Move = 1 << 0,
	};
	enum ImGuiNextWindowDataFlags_
	{
		ImGuiNextWindowDataFlags_None = 0,
		ImGuiNextWindowDataFlags_HasPos = 1 << 0,
		ImGuiNextWindowDataFlags_HasSize = 1 << 1,
		ImGuiNextWindowDataFlags_HasContentSize = 1 << 2,
		ImGuiNextWindowDataFlags_HasCollapsed = 1 << 3,
		ImGuiNextWindowDataFlags_HasSizeConstraint = 1 << 4,
		ImGuiNextWindowDataFlags_HasFocus = 1 << 5,
		ImGuiNextWindowDataFlags_HasBgAlpha = 1 << 6,
		ImGuiNextWindowDataFlags_HasScroll = 1 << 7,
		ImGuiNextWindowDataFlags_HasWindowFlags = 1 << 8,
		ImGuiNextWindowDataFlags_HasChildFlags = 1 << 9,
		ImGuiNextWindowDataFlags_HasRefreshPolicy = 1 << 10,
	};
	enum ImGuiNextItemDataFlags_
	{
		ImGuiNextItemDataFlags_None = 0,
		ImGuiNextItemDataFlags_HasWidth = 1 << 0,
		ImGuiNextItemDataFlags_HasOpen = 1 << 1,
		ImGuiNextItemDataFlags_HasShortcut = 1 << 2,
		ImGuiNextItemDataFlags_HasRefVal = 1 << 3,
		ImGuiNextItemDataFlags_HasStorageID = 1 << 4,
		ImGuiNextItemDataFlags_HasColorMarker = 1 << 5,
	};
	enum ImGuiInputFlagsPrivate_
	{
		ImGuiInputFlags_RepeatRateDefault = 1 << 1,
		ImGuiInputFlags_RepeatRateNavMove = 1 << 2,
		ImGuiInputFlags_RepeatRateNavTweak = 1 << 3,
		ImGuiInputFlags_RepeatUntilRelease = 1 << 4,
		ImGuiInputFlags_RepeatUntilKeyModsChange = 1 << 5,
		ImGuiInputFlags_RepeatUntilKeyModsChangeFromNone = 1 << 6,
		ImGuiInputFlags_RepeatUntilOtherKeyPress = 1 << 7,

		ImGuiInputFlags_LockThisFrame = 1 << 20,
		ImGuiInputFlags_LockUntilRelease = 1 << 21,
		ImGuiInputFlags_CondHovered = 1 << 22,
		ImGuiInputFlags_CondActive = 1 << 23,
		ImGuiInputFlags_CondDefault_ = ImGuiInputFlags_CondHovered | ImGuiInputFlags_CondActive,

		ImGuiInputFlags_RepeatRateMask_ = ImGuiInputFlags_RepeatRateDefault | ImGuiInputFlags_RepeatRateNavMove | ImGuiInputFlags_RepeatRateNavTweak,
		ImGuiInputFlags_RepeatUntilMask_ = ImGuiInputFlags_RepeatUntilRelease | ImGuiInputFlags_RepeatUntilKeyModsChange | ImGuiInputFlags_RepeatUntilKeyModsChangeFromNone | ImGuiInputFlags_RepeatUntilOtherKeyPress,
		ImGuiInputFlags_RepeatMask_ = ImGuiInputFlags_Repeat | ImGuiInputFlags_RepeatRateMask_ | ImGuiInputFlags_RepeatUntilMask_,
		ImGuiInputFlags_CondMask_ = ImGuiInputFlags_CondHovered | ImGuiInputFlags_CondActive,
		ImGuiInputFlags_RouteTypeMask_ = ImGuiInputFlags_RouteActive | ImGuiInputFlags_RouteFocused | ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_RouteAlways,
		ImGuiInputFlags_RouteOptionsMask_ = ImGuiInputFlags_RouteOverFocused | ImGuiInputFlags_RouteOverActive | ImGuiInputFlags_RouteUnlessBgFocused | ImGuiInputFlags_RouteFromRootWindow,
		ImGuiInputFlags_SupportedByIsKeyPressed = ImGuiInputFlags_RepeatMask_,
		ImGuiInputFlags_SupportedByIsMouseClicked = ImGuiInputFlags_Repeat,
		ImGuiInputFlags_SupportedByShortcut = ImGuiInputFlags_RepeatMask_ | ImGuiInputFlags_RouteTypeMask_ | ImGuiInputFlags_RouteOptionsMask_,
		ImGuiInputFlags_SupportedBySetNextItemShortcut = ImGuiInputFlags_RepeatMask_ | ImGuiInputFlags_RouteTypeMask_ | ImGuiInputFlags_RouteOptionsMask_ | ImGuiInputFlags_Tooltip,
		ImGuiInputFlags_SupportedBySetKeyOwner = ImGuiInputFlags_LockThisFrame | ImGuiInputFlags_LockUntilRelease,
		ImGuiInputFlags_SupportedBySetItemKeyOwner = ImGuiInputFlags_SupportedBySetKeyOwner | ImGuiInputFlags_CondMask_,
	};
	enum ImGuiActivateFlags_
	{
		ImGuiActivateFlags_None = 0,
		ImGuiActivateFlags_PreferInput = 1 << 0,
		ImGuiActivateFlags_PreferTweak = 1 << 1,
		ImGuiActivateFlags_TryToPreserveState = 1 << 2,
		ImGuiActivateFlags_FromTabbing = 1 << 3,
		ImGuiActivateFlags_FromShortcut = 1 << 4,
		ImGuiActivateFlags_FromFocusApi = 1 << 5,
	};
	enum ImGuiScrollFlags_
	{
		ImGuiScrollFlags_None = 0,
		ImGuiScrollFlags_KeepVisibleEdgeX = 1 << 0,
		ImGuiScrollFlags_KeepVisibleEdgeY = 1 << 1,
		ImGuiScrollFlags_KeepVisibleCenterX = 1 << 2,
		ImGuiScrollFlags_KeepVisibleCenterY = 1 << 3,
		ImGuiScrollFlags_AlwaysCenterX = 1 << 4,
		ImGuiScrollFlags_AlwaysCenterY = 1 << 5,
		ImGuiScrollFlags_NoScrollParent = 1 << 6,
		ImGuiScrollFlags_MaskX_ = ImGuiScrollFlags_KeepVisibleEdgeX | ImGuiScrollFlags_KeepVisibleCenterX | ImGuiScrollFlags_AlwaysCenterX,
		ImGuiScrollFlags_MaskY_ = ImGuiScrollFlags_KeepVisibleEdgeY | ImGuiScrollFlags_KeepVisibleCenterY | ImGuiScrollFlags_AlwaysCenterY,
	};
	enum ImGuiNavRenderCursorFlags_
	{
		ImGuiNavRenderCursorFlags_None = 0,
		ImGuiNavRenderCursorFlags_Compact = 1 << 1,
		ImGuiNavRenderCursorFlags_AlwaysDraw = 1 << 2,
		ImGuiNavRenderCursorFlags_NoRounding = 1 << 3,
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		ImGuiNavHighlightFlags_None = ImGuiNavRenderCursorFlags_None,
		ImGuiNavHighlightFlags_Compact = ImGuiNavRenderCursorFlags_Compact,
		ImGuiNavHighlightFlags_AlwaysDraw = ImGuiNavRenderCursorFlags_AlwaysDraw,
		ImGuiNavHighlightFlags_NoRounding = ImGuiNavRenderCursorFlags_NoRounding,

#endif
	};
	enum ImGuiNavMoveFlags_
	{
		ImGuiNavMoveFlags_None = 0,
		ImGuiNavMoveFlags_LoopX = 1 << 0,
		ImGuiNavMoveFlags_LoopY = 1 << 1,
		ImGuiNavMoveFlags_WrapX = 1 << 2,
		ImGuiNavMoveFlags_WrapY = 1 << 3,
		ImGuiNavMoveFlags_WrapMask_ = ImGuiNavMoveFlags_LoopX | ImGuiNavMoveFlags_LoopY | ImGuiNavMoveFlags_WrapX | ImGuiNavMoveFlags_WrapY,
		ImGuiNavMoveFlags_AllowCurrentNavId = 1 << 4,
		ImGuiNavMoveFlags_AlsoScoreVisibleSet = 1 << 5,
		ImGuiNavMoveFlags_ScrollToEdgeY = 1 << 6,
		ImGuiNavMoveFlags_Forwarded = 1 << 7,
		ImGuiNavMoveFlags_DebugNoResult = 1 << 8,
		ImGuiNavMoveFlags_FocusApi = 1 << 9,
		ImGuiNavMoveFlags_IsTabbing = 1 << 10,
		ImGuiNavMoveFlags_IsPageMove = 1 << 11,
		ImGuiNavMoveFlags_Activate = 1 << 12,
		ImGuiNavMoveFlags_NoSelect = 1 << 13,
		ImGuiNavMoveFlags_NoSetNavCursorVisible = 1 << 14,
		ImGuiNavMoveFlags_NoClearActiveId = 1 << 15,
	};
	enum ImGuiNavLayer
	{
		ImGuiNavLayer_Main = 0,
		ImGuiNavLayer_Menu = 1,
		ImGuiNavLayer_COUNT
	};
	enum ImGuiTypingSelectFlags_
	{
		ImGuiTypingSelectFlags_None = 0,
		ImGuiTypingSelectFlags_AllowBackspace = 1 << 0,
		ImGuiTypingSelectFlags_AllowSingleCharMode = 1 << 1,
	};
	enum ImGuiOldColumnFlags_
	{
		ImGuiOldColumnFlags_None = 0,
		ImGuiOldColumnFlags_NoBorder = 1 << 0,
		ImGuiOldColumnFlags_NoResize = 1 << 1,
		ImGuiOldColumnFlags_NoPreserveWidths = 1 << 2,
		ImGuiOldColumnFlags_NoForceWithinWindow = 1 << 3,
		ImGuiOldColumnFlags_GrowParentContentsSize = 1 << 4,

	};
	enum ImGuiTabBarFlagsPrivate_
	{
		ImGuiTabBarFlags_DockNode = 1 << 20,
		ImGuiTabBarFlags_IsFocused = 1 << 21,
		ImGuiTabBarFlags_SaveSettings = 1 << 22,
	};

	enum ImGuiTabItemFlagsPrivate_
	{
		ImGuiTabItemFlags_SectionMask_ = ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_Trailing,
		ImGuiTabItemFlags_NoCloseButton = 1 << 20,
		ImGuiTabItemFlags_Button = 1 << 21,
		ImGuiTabItemFlags_Invisible = 1 << 22,

	};
	enum ImGuiLocKey : int
	{
		ImGuiLocKey_VersionStr,
		ImGuiLocKey_TableSizeOne,
		ImGuiLocKey_TableSizeAllFit,
		ImGuiLocKey_TableSizeAllDefault,
		ImGuiLocKey_TableResetOrder,
		ImGuiLocKey_WindowingMainMenuBar,
		ImGuiLocKey_WindowingPopup,
		ImGuiLocKey_WindowingUntitled,
		ImGuiLocKey_OpenLink_s,
		ImGuiLocKey_CopyLink,
		ImGuiLocKey_COUNT
	};

#pragma endregion

#pragma region Structs

	namespace ImGui
	{
		namespace ImGuiStorage_
		{
			void Clear(ImGuiStorage* self);
			inline int GetInt(const ImGuiStorage* self, ImGuiID key, int default_val = 0);
			inline void SetInt(ImGuiStorage* self, ImGuiID key, int val);
			inline bool GetBool(const ImGuiStorage* self, ImGuiID key, bool default_val = false);
			inline void SetBool(ImGuiStorage* self, ImGuiID key, bool val);
			inline float GetFloat(const ImGuiStorage* self, ImGuiID key, float default_val = 0.0f);
			inline void SetFloat(ImGuiStorage* self, ImGuiID key, float val);
			inline void* GetVoidPtr(const ImGuiStorage* self, ImGuiID key);
			inline void SetVoidPtr(ImGuiStorage* self, ImGuiID key, void* val);

			inline int* GetIntRef(ImGuiStorage* self, ImGuiID key, int default_val = 0);
			inline bool* GetBoolRef(ImGuiStorage* self, ImGuiID key, bool default_val = false);
			inline float* GetFloatRef(ImGuiStorage* self, ImGuiID key, float default_val = 0.0f);
			inline void** GetVoidPtrRef(ImGuiStorage* self, ImGuiID key, void* default_val = NULL);

			inline void BuildSortByKey(ImGuiStorage* self);

			inline void SetAllInt(ImGuiStorage* self, int val);
		}
	}

	typedef int ImPoolIdx;
	struct ImGuiStorage
	{
		ImVector<ImGuiStoragePair> Data;

		void Clear()
		{
			ImGui::ImGuiStorage_::Clear(this);
		}
		int GetInt(ImGuiID key, int default_val = 0) const
		{
			return ImGui::ImGuiStorage_::GetInt(this, key, default_val);
		}
		void SetInt(ImGuiID key, int val)
		{
			ImGui::ImGuiStorage_::SetInt(this, key, val);
		}
		bool GetBool(ImGuiID key, bool default_val = false) const
		{
			return ImGui::ImGuiStorage_::GetBool(this, key, default_val);
		}
		void SetBool(ImGuiID key, bool val)
		{
			ImGui::ImGuiStorage_::SetBool(this, key, val);
		}
		float GetFloat(ImGuiID key, float default_val = 0.0f) const
		{
			return ImGui::ImGuiStorage_::GetFloat(this, key, default_val);
		}
		void SetFloat(ImGuiID key, float val)
		{
			ImGui::ImGuiStorage_::SetFloat(this, key, val);
		}
		void* GetVoidPtr(ImGuiID key) const
		{
			return ImGui::ImGuiStorage_::GetVoidPtr(this, key);
		}
		void SetVoidPtr(ImGuiID key, void* val)
		{
			ImGui::ImGuiStorage_::SetVoidPtr(this, key, val);
		}

		int* GetIntRef(ImGuiID key, int default_val = 0)
		{
			return ImGui::ImGuiStorage_::GetIntRef(this, key, default_val);
		}
		bool* GetBoolRef(ImGuiID key, bool default_val = false)
		{
			return ImGui::ImGuiStorage_::GetBoolRef(this, key, default_val);
		}
		float* GetFloatRef(ImGuiID key, float default_val = 0.0f)
		{
			return ImGui::ImGuiStorage_::GetFloatRef(this, key, default_val);
		}
		void** GetVoidPtrRef(ImGuiID key, void* default_val = NULL)
		{
			return ImGui::ImGuiStorage_::GetVoidPtrRef(this, key, default_val);
		}

		void BuildSortByKey()
		{
			ImGui::ImGuiStorage_::BuildSortByKey(this);
		}

		void SetAllInt(int val)
		{
			ImGui::ImGuiStorage_::SetAllInt(this, val);
		}
	};
	template <typename T>
	struct ImPool
	{
		ImVector<T> Buf;
		ImGuiStorage Map;
		ImPoolIdx FreeIdx;
		ImPoolIdx AliveCount;

		ImPool() { FreeIdx = AliveCount = 0; }
		~ImPool() { Clear(); }
		T* GetByKey(ImGuiID key)
		{
			int idx = Map.GetInt(key, -1);
			return (idx != -1) ? &Buf[idx] : NULL;
		}
		T* GetByIndex(ImPoolIdx n) { return &Buf[n]; }
		ImPoolIdx GetIndex(const T* p) const
		{
			assert(p >= Buf.Data && p < Buf.Data + Buf.Size);
			return (ImPoolIdx)(p - Buf.Data);
		}
		T* GetOrAddByKey(ImGuiID key)
		{
			int* p_idx = Map.GetIntRef(key, -1);
			if (*p_idx != -1)
				return &Buf[*p_idx];
			*p_idx = FreeIdx;
			return Add();
		}
		bool Contains(const T* p) const { return (p >= Buf.Data && p < Buf.Data + Buf.Size); }
		void Clear()
		{
			for (int n = 0; n < Map.Data.Size; n++) {
				int idx = Map.Data[n].val_i;
				if (idx != -1)
					Buf[idx].~T();
			}
			Map.Clear();
			Buf.clear();
			FreeIdx = AliveCount = 0;
		}
		T* Add()
		{
			int idx = FreeIdx;
			if (idx == Buf.Size) {
				Buf.resize(Buf.Size + 1);
				FreeIdx++;
			} else {
				FreeIdx = *(int*)&Buf[idx];
			}
			IM_PLACEMENT_NEW(&Buf[idx])
			T();
			AliveCount++;
			return &Buf[idx];
		}
		void Remove(ImGuiID key, const T* p) { Remove(key, GetIndex(p)); }
		void Remove(ImGuiID key, ImPoolIdx idx)
		{
			Buf[idx].~T();
			*(int*)&Buf[idx] = FreeIdx;
			FreeIdx = idx;
			Map.SetInt(key, -1);
			AliveCount--;
		}
		void Reserve(int capacity)
		{
			Buf.reserve(capacity);
			Map.Data.reserve(capacity);
		}

		int GetAliveCount() const { return AliveCount; }
		int GetBufSize() const { return Buf.Size; }
		int GetMapSize() const { return Map.Data.Size; }
		T* TryGetMapData(ImPoolIdx n)
		{
			int idx = Map.Data[n].val_i;
			if (idx == -1)
				return NULL;
			return GetByIndex(idx);
		}
	};
	template <typename T>
	struct ImChunkStream
	{
		ImVector<char> Buf;

		void clear() { Buf.clear(); }
		bool empty() const { return Buf.Size == 0; }
		int size() const { return Buf.Size; }
		T* alloc_chunk(size_t sz)
		{
			size_t HDR_SZ = 4;
			sz = IM_MEMALIGN(HDR_SZ + sz, 4u);
			int off = Buf.Size;
			Buf.resize(off + (int)sz);
			((int*)(void*)(Buf.Data + off))[0] = (int)sz;
			return (T*)(void*)(Buf.Data + off + (int)HDR_SZ);
		}
		T* begin()
		{
			size_t HDR_SZ = 4;
			if (!Buf.Data)
				return NULL;
			return (T*)(void*)(Buf.Data + HDR_SZ);
		}
		T* next_chunk(T* p)
		{
			size_t HDR_SZ = 4;
			assert(p >= begin() && p < end());
			p = (T*)(void*)((char*)(void*)p + chunk_size(p));
			if (p == (T*)(void*)((char*)end() + HDR_SZ))
				return (T*)0;
			assert(p < end());
			return p;
		}
		int chunk_size(const T* p) { return ((const int*)p)[-1]; }
		T* end() { return (T*)(void*)(Buf.Data + Buf.Size); }
		int offset_from_ptr(const T* p)
		{
			assert(p >= begin() && p < end());
			const ptrdiff_t off = (const char*)p - Buf.Data;
			return (int)off;
		}
		T* ptr_from_offset(int off)
		{
			assert(off >= 4 && off < Buf.Size);
			return (T*)(void*)(Buf.Data + off);
		}
		void swap(ImChunkStream<T>& rhs) { rhs.Buf.swap(Buf); }
	};
	struct ImGuiTableSortSpecs
	{
		const ImGuiTableColumnSortSpecs* Specs;
		int SpecsCount;
		bool SpecsDirty;

		ImGuiTableSortSpecs() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiTableColumnSortSpecs
	{
		ImGuiID ColumnUserID;
		ImS16 ColumnIndex;
		ImS16 SortOrder;
		ImGuiSortDirection SortDirection;

		ImGuiTableColumnSortSpecs() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiTextBuffer
	{
		ImVector<char> Buf;
		static char EmptyString[1];

		ImGuiTextBuffer() {}
		inline char operator[](int i) const
		{
			assert(Buf.Data != NULL);
			return Buf.Data[i];
		}
		const char* begin() const { return Buf.Data ? &Buf.front() : EmptyString; }
		const char* end() const { return Buf.Data ? &Buf.back() : EmptyString; }
		int size() const { return Buf.Size ? Buf.Size - 1 : 0; }
		bool empty() const { return Buf.Size <= 1; }
		void clear() { Buf.clear(); }
		void resize(int size)
		{
			if (Buf.Size > size)
				Buf.Data[size] = 0;
			Buf.resize(size ? size + 1 : 0, 0);
		}
		void reserve(int capacity) { Buf.reserve(capacity); }
		const char* c_str() const { return Buf.Data ? Buf.Data : EmptyString; }
	};
	struct ImGuiInputTextCallbackData
	{
		ImGuiContext* Ctx;
		ImGuiInputTextFlags EventFlag;
		ImGuiInputTextFlags Flags;
		void* UserData;
		ImGuiID ID;
		ImGuiKey EventKey;
		ImWchar EventChar;
		bool EventActivated;
		bool BufDirty;
		char* Buf;
		int BufTextLen;
		int BufSize;
		int CursorPos;
		int SelectionStart;
		int SelectionEnd;

		void SelectAll()
		{
			SelectionStart = 0;
			CursorPos = SelectionEnd = BufTextLen;
		}
		void SetSelection(int s, int e)
		{
			assert(s >= 0 && s <= BufTextLen);
			assert(e >= 0 && e <= BufTextLen);
			SelectionStart = s;
			CursorPos = SelectionEnd = e;
		}
		void ClearSelection() { SelectionStart = SelectionEnd = BufTextLen; }
		bool HasSelection() const { return SelectionStart != SelectionEnd; }
	};
	struct ImGuiPayload
	{
		void* Data;
		int DataSize;

		ImGuiID SourceId;
		ImGuiID SourceParentId;
		int DataFrameCount;
		char DataType[32 + 1];
		bool Preview;
		bool Delivery;

		ImGuiPayload() { Clear(); }
		void Clear()
		{
			SourceId = SourceParentId = 0;
			Data = NULL;
			DataSize = 0;
			memset(DataType, 0, sizeof(DataType));
			DataFrameCount = -1;
			Preview = Delivery = false;
		}
		bool IsDataType(const char* type) const { return DataFrameCount != -1 && strcmp(type, DataType) == 0; }
		bool IsPreview() const { return Preview; }
		bool IsDelivery() const { return Delivery; }
	};

#ifndef IM_COL32_R_SHIFT
#	ifdef IMGUI_USE_BGRA_PACKED_COLOR
#		define IM_COL32_R_SHIFT 16
#		define IM_COL32_G_SHIFT 8
#		define IM_COL32_B_SHIFT 0
#		define IM_COL32_A_SHIFT 24
#		define IM_COL32_A_MASK 0xFF000000
#	else
#		define IM_COL32_R_SHIFT 0
#		define IM_COL32_G_SHIFT 8
#		define IM_COL32_B_SHIFT 16
#		define IM_COL32_A_SHIFT 24
#		define IM_COL32_A_MASK 0xFF000000
#	endif
#endif
#define IM_COL32(R, G, B, A) (((ImU32)(A) << IM_COL32_A_SHIFT) | ((ImU32)(B) << IM_COL32_B_SHIFT) | ((ImU32)(G) << IM_COL32_G_SHIFT) | ((ImU32)(R) << IM_COL32_R_SHIFT))
#define IM_COL32_WHITE IM_COL32(255, 255, 255, 255)
#define IM_COL32_BLACK IM_COL32(0, 0, 0, 255)
#define IM_COL32_BLACK_TRANS IM_COL32(0, 0, 0, 0)
	struct ImColor
	{
		ImVec4 Value;

		constexpr ImColor() {}
		constexpr ImColor(float r, float g, float b, float a = 1.0f) : Value(r, g, b, a) {}
		constexpr ImColor(const ImVec4& col) : Value(col) {}
		constexpr ImColor(int r, int g, int b, int a = 255) : Value((float)r * (1.0f / 255.0f), (float)g * (1.0f / 255.0f), (float)b * (1.0f / 255.0f), (float)a * (1.0f / 255.0f)) {}
		constexpr ImColor(ImU32 rgba) : Value((float)((rgba >> IM_COL32_R_SHIFT) & 0xFF) * (1.0f / 255.0f), (float)((rgba >> IM_COL32_G_SHIFT) & 0xFF) * (1.0f / 255.0f), (float)((rgba >> IM_COL32_B_SHIFT) & 0xFF) * (1.0f / 255.0f), (float)((rgba >> IM_COL32_A_SHIFT) & 0xFF) * (1.0f / 255.0f)) {}
		inline operator ImU32() const { return ImGui::ColorConvertFloat4ToU32(Value); }
		inline operator ImVec4() const { return Value; }

		inline void SetHSV(float h, float s, float v, float a = 1.0f)
		{
			ImGui::ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z);
			Value.w = a;
		}
		static ImColor HSV(float h, float s, float v, float a = 1.0f)
		{
			float r, g, b;
			ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
			return ImColor(r, g, b, a);
		}
	};
	enum ImGuiSelectionRequestType
	{
		ImGuiSelectionRequestType_None = 0,
		ImGuiSelectionRequestType_SetAll,
		ImGuiSelectionRequestType_SetRange,
	};
	typedef ImS64 ImGuiSelectionUserData;
	struct ImGuiSelectionRequest
	{
		ImGuiSelectionRequestType Type;
		bool Selected;
		ImS8 RangeDirection;
		ImGuiSelectionUserData RangeFirstItem;
		ImGuiSelectionUserData RangeLastItem;
	};
	struct ImRect
	{
		ImVec2 Min;
		ImVec2 Max;

		constexpr ImRect() : Min(0.0f, 0.0f), Max(0.0f, 0.0f) {}
		constexpr ImRect(const ImVec2& min, const ImVec2& max) : Min(min), Max(max) {}
		constexpr ImRect(const ImVec4& v) : Min(v.x, v.y), Max(v.z, v.w) {}
		constexpr ImRect(float x1, float y1, float x2, float y2) : Min(x1, y1), Max(x2, y2) {}

		ImVec2 GetCenter() const { return ImVec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f); }
		ImVec2 GetSize() const { return ImVec2(Max.x - Min.x, Max.y - Min.y); }
		float GetWidth() const { return Max.x - Min.x; }
		float GetHeight() const { return Max.y - Min.y; }
		float GetArea() const { return (Max.x - Min.x) * (Max.y - Min.y); }
		ImVec2 GetTL() const { return Min; }
		ImVec2 GetTR() const { return ImVec2(Max.x, Min.y); }
		ImVec2 GetBL() const { return ImVec2(Min.x, Max.y); }
		ImVec2 GetBR() const { return Max; }
		bool Contains(const ImVec2& p) const { return p.x >= Min.x && p.y >= Min.y && p.x < Max.x && p.y < Max.y; }
		bool Contains(const ImRect& r) const { return r.Min.x >= Min.x && r.Min.y >= Min.y && r.Max.x <= Max.x && r.Max.y <= Max.y; }
		bool ContainsWithPad(const ImVec2& p, const ImVec2& pad) const { return p.x >= Min.x - pad.x && p.y >= Min.y - pad.y && p.x < Max.x + pad.x && p.y < Max.y + pad.y; }
		bool Overlaps(const ImRect& r) const { return r.Min.y < Max.y && r.Max.y > Min.y && r.Min.x < Max.x && r.Max.x > Min.x; }
		void Add(const ImVec2& p)
		{
			if (Min.x > p.x)
				Min.x = p.x;
			if (Min.y > p.y)
				Min.y = p.y;
			if (Max.x < p.x)
				Max.x = p.x;
			if (Max.y < p.y)
				Max.y = p.y;
		}
		void Add(const ImRect& r)
		{
			if (Min.x > r.Min.x)
				Min.x = r.Min.x;
			if (Min.y > r.Min.y)
				Min.y = r.Min.y;
			if (Max.x < r.Max.x)
				Max.x = r.Max.x;
			if (Max.y < r.Max.y)
				Max.y = r.Max.y;
		}
		void AddX(float x)
		{
			if (Min.x > x)
				Min.x = x;
			if (Max.x < x)
				Max.x = x;
		}
		void AddY(float y)
		{
			if (Min.y > y)
				Min.y = y;
			if (Max.y < y)
				Max.y = y;
		}
		void Expand(const float amount)
		{
			Min.x -= amount;
			Min.y -= amount;
			Max.x += amount;
			Max.y += amount;
		}
		void Expand(const ImVec2& amount)
		{
			Min.x -= amount.x;
			Min.y -= amount.y;
			Max.x += amount.x;
			Max.y += amount.y;
		}
		void Translate(const ImVec2& d)
		{
			Min.x += d.x;
			Min.y += d.y;
			Max.x += d.x;
			Max.y += d.y;
		}
		void TranslateX(float dx)
		{
			Min.x += dx;
			Max.x += dx;
		}
		void TranslateY(float dy)
		{
			Min.y += dy;
			Max.y += dy;
		}
		void ClipWith(const ImRect& r)
		{
			Min = ImMax(Min, r.Min);
			Max = ImMin(Max, r.Max);
		}
		void ClipWithFull(const ImRect& r)
		{
			Min = ImClamp(Min, r.Min, r.Max);
			Max = ImClamp(Max, r.Min, r.Max);
		}
		bool IsInverted() const { return Min.x > Max.x || Min.y > Max.y; }
		ImVec4 ToVec4() const { return ImVec4(Min.x, Min.y, Max.x, Max.y); }
		const ImVec4& AsVec4() const { return *(const ImVec4*)&Min.x; }
	};

	struct ImGuiMetricsConfig
	{
		bool ShowDebugLog = false;
		bool ShowIDStackTool = false;
		bool ShowWindowsRects = false;
		bool ShowWindowsBeginOrder = false;
		bool ShowTablesRects = false;
		bool ShowDrawCmdMesh = true;
		bool ShowDrawCmdBoundingBoxes = true;
		bool ShowTextEncodingViewer = false;
		bool ShowTextureUsedRect = false;
		int ShowWindowsRectsType = -1;
		int ShowTablesRectsType = -1;
		int HighlightMonitorIdx = -1;
		ImGuiID HighlightViewportID = 0;
		bool ShowFontPreview = true;
	};
	struct ImGuiStackLevelInfo
	{
		ImGuiID ID;
		ImS8 QueryFrameCount;
		bool QuerySuccess;
		ImS8 DataType;
		int DescOffset;

		ImGuiStackLevelInfo()
		{
			memset((void*)this, 0, sizeof(*this));
			DataType = -1;
			DescOffset = -1;
		}
	};
	struct ImGuiDebugItemPathQuery
	{
		ImGuiID MainID;
		bool Active;
		bool Complete;
		ImS8 Step;
		ImVector<ImGuiStackLevelInfo> Results;
		ImGuiTextBuffer ResultsDescBuf;
		ImGuiTextBuffer ResultPathBuf;

		ImGuiDebugItemPathQuery() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiIDStackTool
	{
		bool OptHexEncodeNonAsciiChars;
		bool OptCopyToClipboardOnCtrlC;
		int LastActiveFrame;
		float CopyToClipboardLastTime;

		ImGuiIDStackTool()
		{
			memset((void*)this, 0, sizeof(*this));
			LastActiveFrame = -1;
			OptHexEncodeNonAsciiChars = true;
			CopyToClipboardLastTime = -FLT_MAX;
		}
	};
	typedef struct ImFont ImFont;
	struct ImGuiViewport
	{
		ImGuiID ID;
		ImGuiViewportFlags Flags;
		ImVec2 Pos;
		ImVec2 Size;
		ImVec2 FramebufferScale;
		ImVec2 WorkPos;
		ImVec2 WorkSize;

		void* PlatformHandle;
		void* PlatformHandleRaw;

		ImGuiViewport() { memset((void*)this, 0, sizeof(*this)); }

		ImVec2 GetCenter() const { return ImVec2(Pos.x + Size.x * 0.5f, Pos.y + Size.y * 0.5f); }
		ImVec2 GetWorkCenter() const { return ImVec2(WorkPos.x + WorkSize.x * 0.5f, WorkPos.y + WorkSize.y * 0.5f); }
	};
	struct ImFontBaked
	{
		ImVector<float> IndexAdvanceX;
		float FallbackAdvanceX;
		float Size;
		float RasterizerDensity;

		ImVector<ImU16> IndexLookup;
		ImVector<ImFontGlyph> Glyphs;
		int FallbackGlyphIndex;

		float Ascent, Descent;
		unsigned int MetricsTotalSurface: 26;
		unsigned int WantDestroy: 1;
		unsigned int LoadNoFallback: 1;
		unsigned int LoadNoRenderOnLayout: 1;
		int LastUsedFrame;
		ImGuiID BakedId;
		ImFont* OwnerFont;
		void* FontLoaderDatas;
	};
	struct ImDrawData
	{
		bool Valid;
		int CmdListsCount;
		int TotalIdxCount;
		int TotalVtxCount;
		ImVector<ImDrawList_Opaque*> CmdLists;
		ImVec2 DisplayPos;
		ImVec2 DisplaySize;
		ImVec2 FramebufferScale;
		ImGuiViewport* OwnerViewport;
		ImVector<ImTextureData*>* Textures;
	};
	struct ImDrawDataBuilder
	{
		ImVector<ImDrawList_Opaque*>* Layers[2];
		ImVector<ImDrawList_Opaque*> LayerData1;

		ImDrawDataBuilder() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiViewportP : public ImGuiViewport
	{
		float BgFgDrawListsLastTimeActive[2];
		ImDrawList_Opaque* BgFgDrawLists[2];
		ImDrawData DrawDataP;
		ImDrawDataBuilder DrawDataBuilder;

		ImVec2 WorkInsetMin;
		ImVec2 WorkInsetMax;
		ImVec2 BuildWorkInsetMin;
		ImVec2 BuildWorkInsetMax;

		ImVec2 CalcWorkRectPos(const ImVec2& inset_min) const { return ImVec2(Pos.x + inset_min.x, Pos.y + inset_min.y); }
		ImVec2 CalcWorkRectSize(const ImVec2& inset_min, const ImVec2& inset_max) const { return ImVec2(ImMax(0.0f, Size.x - inset_min.x - inset_max.x), ImMax(0.0f, Size.y - inset_min.y - inset_max.y)); }
		void UpdateWorkRect()
		{
			WorkPos = CalcWorkRectPos(WorkInsetMin);
			WorkSize = CalcWorkRectSize(WorkInsetMin, WorkInsetMax);
		}

		ImRect GetMainRect() const { return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
		ImRect GetWorkRect() const { return ImRect(WorkPos.x, WorkPos.y, WorkPos.x + WorkSize.x, WorkPos.y + WorkSize.y); }
		ImRect GetBuildWorkRect() const
		{
			ImVec2 pos = CalcWorkRectPos(BuildWorkInsetMin);
			ImVec2 size = CalcWorkRectSize(BuildWorkInsetMin, BuildWorkInsetMax);
			return ImRect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	};

	typedef struct ImTextureData ImTextureData;
	typedef ImU64 ImTextureID;
	struct ImTextureRef
	{
		ImTextureRef()
		{
			_TexData = NULL;
			_TexID = 0;
		}
		ImTextureRef(ImTextureID tex_id)
		{
			_TexData = NULL;
			_TexID = tex_id;
		}
		ImTextureRef(void* tex_id)
		{
			_TexData = NULL;
			_TexID = (ImTextureID)(size_t)tex_id;
		}

		ImTextureData* _TexData;
		ImTextureID _TexID;
	};
	struct ImGuiErrorRecoveryState
	{
		short SizeOfWindowStack;
		short SizeOfIDStack;
		short SizeOfTreeStack;
		short SizeOfColorStack;
		short SizeOfStyleVarStack;
		short SizeOfFontStack;
		short SizeOfFocusScopeStack;
		short SizeOfGroupStack;
		short SizeOfItemFlagsStack;
		short SizeOfBeginPopupStack;
		short SizeOfDisabledStack;

		ImGuiErrorRecoveryState() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiTypingSelectRequest
	{
		ImGuiTypingSelectFlags Flags;
		int SearchBufferLen;
		const char* SearchBuffer;
		bool SelectRequest;
		bool SingleCharMode;
		ImS8 SingleCharSize;
	};
	struct ImGuiTypingSelectState
	{
		ImGuiTypingSelectRequest Request;
		char SearchBuffer[64];
		ImGuiID FocusScope;
		int LastRequestFrame = 0;
		float LastRequestTime = 0.0f;
		bool SingleCharModeLock = false;

		ImGuiTypingSelectState() { memset((void*)this, 0, sizeof(*this)); }
		void Clear()
		{
			SearchBuffer[0] = 0;
			SingleCharModeLock = false;
		}
	};
	struct ImGuiPlatformImeData
	{
		bool WantVisible;
		bool WantTextInput;
		ImVec2 InputPos;
		float InputLineHeight;
		ImGuiID ViewportId;

		ImGuiPlatformImeData() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiLastItemData
	{
		ImGuiID ID;
		ImGuiItemFlags ItemFlags;
		ImGuiItemStatusFlags StatusFlags;
		ImRect Rect;
		ImRect NavRect;

		ImRect DisplayRect;
		ImRect ClipRect;
		ImGuiKeyChord Shortcut;

		ImGuiLastItemData() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiWindowStackData
	{
		ImGuiWindow* Window;
		ImGuiLastItemData ParentLastItemDataBackup;
		ImGuiErrorRecoveryState StackSizesInBegin;
		bool DisabledOverrideReenable;
		float DisabledOverrideReenableAlphaBackup;
	};
	struct ImGuiDeactivatedItemData
	{
		ImGuiID ID;
		int ElapseFrame;
		bool HasBeenEditedBefore;
		bool IsAlive;
	};
	struct ImGuiDataTypeStorage
	{
		ImU8 Data[8];
	};
	template <int BITCOUNT, int OFFSET = 0>
	struct ImBitArray
	{
		ImU32 Data[(BITCOUNT + 31) >> 5];
		ImBitArray() { ClearAllBits(); }
		void ClearAllBits() { memset(Data, 0, sizeof(Data)); }
		void SetAllBits() { memset(Data, 255, sizeof(Data)); }
		bool TestBit(int n) const
		{
			n += OFFSET;
			assert(n >= 0 && n < BITCOUNT);
			return IM_BITARRAY_TESTBIT(Data, n);
		}
		void SetBit(int n)
		{
			n += OFFSET;
			assert(n >= 0 && n < BITCOUNT);
			ImBitArraySetBit(Data, n);
		}
		void ClearBit(int n)
		{
			n += OFFSET;
			assert(n >= 0 && n < BITCOUNT);
			ImBitArrayClearBit(Data, n);
		}
		void SetBitRange(int n, int n2)
		{
			n += OFFSET;
			n2 += OFFSET;
			assert(n >= 0 && n < BITCOUNT && n2 > n && n2 <= BITCOUNT);
			ImBitArraySetBitRange(Data, n, n2);
		}
		bool operator[](int n) const
		{
			n += OFFSET;
			assert(n >= 0 && n < BITCOUNT);
			return IM_BITARRAY_TESTBIT(Data, n);
		}
	};
#define ImGuiKeyOwner_NoOwner ((ImGuiID) - 1)
	typedef ImBitArray<ImGuiKey_NamedKey_COUNT, -ImGuiKey_NamedKey_BEGIN> ImBitArrayForNamedKeys;
	struct ImGuiKeyOwnerData
	{
		ImGuiID OwnerCurr;
		ImGuiID OwnerNext;
		bool LockThisFrame;
		bool LockUntilRelease;

		ImGuiKeyOwnerData()
		{
			OwnerCurr = OwnerNext = ImGuiKeyOwner_NoOwner;
			LockThisFrame = LockUntilRelease = false;
		}
	};
	typedef ImS16 ImGuiKeyRoutingIndex;
	struct ImGuiKeyRoutingData
	{
		ImGuiKeyRoutingIndex NextEntryIndex;
		ImU16 Mods;
		ImU16 RoutingCurrScore;
		ImU16 RoutingNextScore;
		ImGuiID RoutingCurr;
		ImGuiID RoutingNext;

		ImGuiKeyRoutingData()
		{
			NextEntryIndex = -1;
			Mods = 0;
			RoutingCurrScore = RoutingNextScore = 0;
			RoutingCurr = RoutingNext = -1;
		}
	};

	struct ImGuiKeyRoutingTable
	{
		ImGuiKeyRoutingIndex Index[ImGuiKey_NamedKey_COUNT];
		ImVector<ImGuiKeyRoutingData> Entries;
		ImVector<ImGuiKeyRoutingData> EntriesNext;

		ImGuiKeyRoutingTable() { Clear(); }
		void Clear()
		{
			for (int n = 0; n < IM_COUNTOF(Index); n++) Index[n] = -1;
			Entries.clear();
			EntriesNext.clear();
		}
	};
	struct ImGuiNextItemData
	{
		ImGuiNextItemDataFlags HasFlags;
		ImGuiItemFlags ItemFlags;

		ImGuiID FocusScopeId;
		ImGuiSelectionUserData SelectionUserData;
		float Width;
		ImGuiKeyChord Shortcut;
		ImGuiInputFlags ShortcutFlags;
		bool OpenVal;
		ImU8 OpenCond;
		ImGuiDataTypeStorage RefVal;
		ImGuiID StorageId;
		ImU32 ColorMarker;

		ImGuiNextItemData()
		{
			memset((void*)this, 0, sizeof(*this));
			SelectionUserData = -1;
		}
		inline void ClearFlags()
		{
			HasFlags = ImGuiNextItemDataFlags_None;
			ItemFlags = ImGuiItemFlags_None;
		}
	};
	struct ImGuiNextWindowData
	{
		ImGuiNextWindowDataFlags HasFlags;

		ImGuiCond PosCond;
		ImGuiCond SizeCond;
		ImGuiCond CollapsedCond;
		ImVec2 PosVal;
		ImVec2 PosPivotVal;
		ImVec2 SizeVal;
		ImVec2 ContentSizeVal;
		ImVec2 ScrollVal;
		ImGuiWindowFlags WindowFlags;
		ImGuiChildFlags ChildFlags;
		bool CollapsedVal;
		ImRect SizeConstraintRect;
		ImGuiSizeCallback SizeCallback;
		void* SizeCallbackUserData;
		float BgAlphaVal;
		ImVec2 MenuBarOffsetMinVal;
		ImGuiWindowRefreshFlags RefreshFlagsVal;

		ImGuiNextWindowData() { memset((void*)this, 0, sizeof(*this)); }
		inline void ClearFlags() { HasFlags = ImGuiNextWindowDataFlags_None; }
	};

	struct ImFontStackData
	{
		ImFont* Font;
		float FontSizeBeforeScaling;
		float FontSizeAfterScaling;
	};
	struct ImGuiFocusScopeData
	{
		ImGuiID ID;
		ImGuiID WindowID;
	};
	struct ImGuiNavItemData
	{
		ImGuiWindow* Window;
		ImGuiID ID;
		ImGuiID FocusScopeId;
		ImRect RectRel;
		ImGuiItemFlags ItemFlags;
		float DistBox;
		float DistCenter;
		float DistAxial;
		ImGuiSelectionUserData SelectionUserData;

		ImGuiNavItemData() { Clear(); }
		void Clear()
		{
			Window = NULL;
			ID = FocusScopeId = 0;
			ItemFlags = 0;
			SelectionUserData = -1;
			DistBox = DistCenter = DistAxial = FLT_MAX;
		}
	};
	struct ImGuiListClipperRange
	{
		int Min;
		int Max;
		bool PosToIndexConvert;
		ImS8 PosToIndexOffsetMin;
		ImS8 PosToIndexOffsetMax;

		static ImGuiListClipperRange FromIndices(int min, int max)
		{
			ImGuiListClipperRange r = { min, max, false, 0, 0 };
			return r;
		}
		static ImGuiListClipperRange FromPositions(float y1, float y2, int off_min, int off_max)
		{
			ImGuiListClipperRange r = { (int)y1, (int)y2, true, (ImS8)off_min, (ImS8)off_max };
			return r;
		}
	};
	struct ImGuiListClipperData
	{
		ImGuiListClipper* ListClipper;
		float LossynessOffset;
		int StepNo;
		int ItemsFrozen;
		ImVector<ImGuiListClipperRange> Ranges;

		ImGuiListClipperData() { memset((void*)this, 0, sizeof(*this)); }
		void Reset(ImGuiListClipper* clipper)
		{
			ListClipper = clipper;
			StepNo = ItemsFrozen = 0;
			Ranges.resize(0);
		}
	};
	struct ImGuiPtrOrIndex
	{
		void* Ptr;
		int Index;

		ImGuiPtrOrIndex(void* ptr)
		{
			Ptr = ptr;
			Index = -1;
		}
		ImGuiPtrOrIndex(int index)
		{
			Ptr = NULL;
			Index = index;
		}
	};
	struct ImGuiComboPreviewData
	{
		ImRect PreviewRect;
		ImVec2 BackupCursorPos;
		ImVec2 BackupCursorMaxPos;
		ImVec2 BackupCursorPosPrevLine;
		float BackupPrevLineTextBaseOffset;
		ImGuiLayoutType BackupLayout;

		ImGuiComboPreviewData() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiBoxSelectState
	{
		ImGuiID ID;
		bool IsActive;
		bool IsStarting;
		bool IsStartedFromVoid;
		bool IsStartedSetNavIdOnce;
		bool RequestClear;
		ImGuiKeyChord KeyMods: 16;
		ImVec2 StartPosRel;
		ImVec2 EndPosRel;
		ImVec2 ScrollAccum;
		ImGuiWindow* Window;

		bool UnclipMode;
		ImRect UnclipRect;
		ImRect UnclipRects[2];
		ImRect BoxSelectRectPrev;
		ImRect BoxSelectRectCurr;

		ImGuiBoxSelectState() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiTextIndex
	{
		ImVector<int> Offsets;
		int EndOffset = 0;

		void clear()
		{
			Offsets.clear();
			EndOffset = 0;
		}
		int size() { return Offsets.Size; }
		const char* get_line_begin(const char* base, int n) { return base + (Offsets.Size != 0 ? Offsets[n] : 0); }
		const char* get_line_end(const char* base, int n) { return base + (n + 1 < Offsets.Size ? (Offsets[n + 1] - 1) : EndOffset); }
		void append(const char* base, int old_size, int new_size);
	};
	ImGuiStoragePair* ImLowerBound(ImGuiStoragePair* in_begin, ImGuiStoragePair* in_end, ImGuiID key);
	struct ImGuiInputTextDeactivatedState
	{
		ImGuiID ID;
		ImVector<char> TextA;

		ImGuiInputTextDeactivatedState() { memset((void*)this, 0, sizeof(*this)); }
		void ClearFreeMemory()
		{
			ID = 0;
			TextA.clear();
		}
	};
	namespace ImStb
	{
		struct STB_TexteditState;
	}
	typedef ImStb::STB_TexteditState ImStbTexteditState;
	struct ImGuiInputTextState
	{
		ImGuiContext* Ctx;
		ImStbTexteditState* Stb;
		ImGuiInputTextFlags Flags;
		ImGuiID ID;
		int TextLen;
		const char* TextSrc;
		ImVector<char> TextA;
		ImVector<char> TextToRevertTo;
		ImVector<char> CallbackTextBackup;
		int BufCapacity;
		ImVec2 Scroll;
		int LineCount;
		float WrapWidth;
		float CursorAnim;
		bool CursorFollow;
		bool CursorCenterY;
		bool SelectedAllMouseLock;
		bool EditedBefore;
		bool EditedThisFrame;
		bool WantReloadUserBuf;
		ImS8 LastMoveDirectionLR;
		int ReloadSelectionStart;
		int ReloadSelectionEnd;
	};
	enum ImGuiInputEventType
	{
		ImGuiInputEventType_None = 0,
		ImGuiInputEventType_MousePos,
		ImGuiInputEventType_MouseWheel,
		ImGuiInputEventType_MouseButton,
		ImGuiInputEventType_Key,
		ImGuiInputEventType_Text,
		ImGuiInputEventType_Focus,
		ImGuiInputEventType_COUNT
	};

	enum ImGuiInputSource : int
	{
		ImGuiInputSource_None = 0,
		ImGuiInputSource_Mouse,
		ImGuiInputSource_Keyboard,
		ImGuiInputSource_Gamepad,
		ImGuiInputSource_COUNT
	};

	struct ImGuiInputEventMousePos
	{
		float PosX, PosY;
		ImGuiMouseSource MouseSource;
	};
	struct ImGuiInputEventMouseWheel
	{
		float WheelX, WheelY;
		ImGuiMouseSource MouseSource;
	};
	struct ImGuiInputEventMouseButton
	{
		int Button;
		bool Down;
		ImGuiMouseSource MouseSource;
	};
	struct ImGuiInputEventKey
	{
		ImGuiKey Key;
		bool Down;
		float AnalogValue;
	};
	struct ImGuiInputEventText
	{
		unsigned int Char;
	};
	struct ImGuiInputEventAppFocused
	{
		bool Focused;
	};
	struct ImGuiInputEvent
	{
		ImGuiInputEventType Type;
		ImGuiInputSource Source;
		ImU32 EventId;
		union
		{
			ImGuiInputEventMousePos MousePos;
			ImGuiInputEventMouseWheel MouseWheel;
			ImGuiInputEventMouseButton MouseButton;
			ImGuiInputEventKey Key;
			ImGuiInputEventText Text;
			ImGuiInputEventAppFocused AppFocused;
		};
		bool AddedByTestEngine;

		ImGuiInputEvent() { memset((void*)this, 0, sizeof(*this)); }
	};

	typedef unsigned short ImDrawIdx;
	struct ImDrawCmdHeader
	{
		ImVec4 ClipRect;
		ImTextureRef TexRef;
		unsigned int VtxOffset;
	};
	struct ImDrawListSplitter
	{
		int _Current;
		int _Count;
		ImVector<ImDrawChannel> _Channels;
	};
#define IM_DRAWLIST_ARCFAST_TABLE_SIZE 48
	struct ImDrawListSharedData
	{
		ImVec2 TexUvWhitePixel;
		const ImVec4* TexUvLines;
		ImFontAtlas* FontAtlas;
		ImFont* Font;
		float FontSize;
		float FontScale;
		float CurveTessellationTol;
		float CircleSegmentMaxError;
		float InitialFringeScale;
		ImDrawListFlags InitialFlags;
		ImVec4 ClipRectFullscreen;
		ImVector<ImVec2> TempBuffer;
		ImVector<ImDrawList_Opaque*> DrawLists;
		ImGuiContext* Context;

		ImVec2 ArcFastVtx[IM_DRAWLIST_ARCFAST_TABLE_SIZE];
		float ArcFastRadiusCutoff;
		ImU8 CircleSegmentCounts[64];
	};

	using ImDrawList = ImDrawList_Opaque;
	namespace ImGui
	{
		namespace DrawList
		{

			void PushClipRect(ImDrawList* dl, ImVec2 clip_rect_min, ImVec2 clip_rect_max, bool intersect_with_current_clip_rect);
			void PopClipRect(ImDrawList* dl);
			void PushClipRectFullScreen(ImDrawList* dl);
			void PushTexture(ImDrawList* dl, ImTextureRef tex_ref);
			void PopTexture(ImDrawList* dl);
			ImVec2 GetClipRectMin(ImDrawList* dl);
			ImVec2 GetClipRectMax(ImDrawList* dl);

			void AddLine(ImDrawList* dl, const ImVec2 p1, const ImVec2 p2, uint32_t col, float thickness = 1.0f);
			void AddLineH(ImDrawList* dl, float min_x, float max_x, float y, uint32_t col, float thickness = 1.0f);
			void AddLineV(ImDrawList* dl, float x, float min_y, float max_y, uint32_t col, float thickness = 1.0f);

			void AddRect(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, uint32_t col, float rounding = 0.0f, int rounding_corners = ~0, float thickness = 1.0f);
			void AddRectFilled(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, uint32_t col, float rounding = 0.0f, int rounding_corners = ~0);
			void AddRectFilledMultiColor(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, uint32_t col_upr_left, uint32_t col_upr_right, uint32_t col_bot_right, uint32_t col_bot_left);

			void AddCircle(ImDrawList* dl, ImVec2 center, float radius, uint32_t col, int num_segments = 0, float thickness = 1.0f);
			void AddCircleFilled(ImDrawList* dl, const ImVec2 center, float radius, uint32_t col, int num_segments = 0);

			void AddTriangle(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, uint32_t col, float thickness = 1.0f);
			void AddTriangleFilled(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, uint32_t col);

			void AddQuad(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, uint32_t col, float thickness = 1.0f);
			void AddQuadFilled(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, uint32_t col);

			void AddNgon(ImDrawList* dl, ImVec2 center, float radius, uint32_t col, int num_segments, float thickness = 1.0f);
			void AddNgonFilled(ImDrawList* dl, ImVec2 center, float radius, uint32_t col, int num_segments);

			void AddEllipse(ImDrawList* dl, ImVec2 center, ImVec2 radius, uint32_t col, float rot = 0.0f, int num_segments = 0, float thickness = 1.0f);
			void AddEllipseFilled(ImDrawList* dl, ImVec2 center, ImVec2 radius, uint32_t col, float rot = 0.0f, int num_segments = 0);

			void AddText(ImDrawList* dl, ImVec2 pos, uint32_t col, const char* text_begin, const char* text_end = NULL);
			void AddText(ImDrawList* dl, ImFont* font, float font_size, ImVec2 pos, uint32_t col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);

			void AddBezierQuadratic(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, uint32_t col, float thickness, int num_segments = 0);
			void AddBezierCubic(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, uint32_t col, float thickness, int num_segments = 0);

			void AddPolyline(ImDrawList* dl, const ImVec2* points, int num_points, uint32_t col, float thickness, ImDrawFlags flags);
			void AddConvexPolyFilled(ImDrawList* dl, const ImVec2* points, int num_points, uint32_t col);
			void AddConcavePolyFilled(ImDrawList* dl, const ImVec2* points, int num_points, uint32_t col);

			void AddImageQuad(ImDrawList* dl, ImTextureRef tex_ref, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, ImVec2 uv1, ImVec2 uv2, ImVec2 uv3, ImVec2 uv4, uint32_t col);
			void AddImageRounded(ImDrawList* dl, ImTextureRef tex_ref, ImVec2 p_min, ImVec2 p_max, ImVec2 uv_min, ImVec2 uv_max, uint32_t col, float rounding, ImDrawFlags flags);
			void AddImage(ImDrawList* dl, ImTextureRef tex_ref, const ImVec2 p_min, const ImVec2 p_max, const ImVec2 uv_min = ImVec2(0, 0), const ImVec2 uv_max = ImVec2(1, 1), uint32_t col = 0xFFFFFFFF);

			void PathClear(ImDrawList* dl);
			void PathLineTo(ImDrawList* dl, ImVec2 pos);
			void PathLineToMergeDuplicate(ImDrawList* dl, ImVec2 pos);
			void PathFillConvex(ImDrawList* dl, uint32_t col);
			void PathFillConcave(ImDrawList* dl, uint32_t col);
			void PathStroke(ImDrawList* dl, uint32_t col, float thickness, ImDrawFlags flags);
			void PathArcTo(ImDrawList* dl, ImVec2 center, float radius, float a_min, float a_max, int num_segments = 0);
			void PathArcToFast(ImDrawList* dl, ImVec2 center, float radius, int a_min_of_12, int a_max_of_12);
			void PathEllipticalArcTo(ImDrawList* dl, ImVec2 center, ImVec2 radius, float rot, float a_min, float a_max, int num_segments = 0);
			void PathBezierCubicCurveTo(ImDrawList* dl, ImVec2 p2, ImVec2 p3, ImVec2 p4, int num_segments = 0);
			void PathBezierQuadraticCurveTo(ImDrawList* dl, ImVec2 p2, ImVec2 p3, int num_segments = 0);
			void PathRect(ImDrawList* dl, ImVec2 rect_min, ImVec2 rect_max, float rounding = 0.0f, ImDrawFlags flags = 0);

			void ChannelsSplit(ImDrawList* dl, int count);
			void ChannelsMerge(ImDrawList* dl);
			void ChannelsSetCurrent(ImDrawList* dl, int n);

			void PrimReserve(ImDrawList* dl, int idx_count, int vtx_count);
			void PrimUnreserve(ImDrawList* dl, int idx_count, int vtx_count);
			void PrimRect(ImDrawList* dl, ImVec2 a, ImVec2 b, uint32_t col);
			void PrimRectUV(ImDrawList* dl, ImVec2 a, ImVec2 b, ImVec2 uv_a, ImVec2 uv_b, uint32_t col);
			void PrimQuadUV(ImDrawList* dl, ImVec2 a, ImVec2 b, ImVec2 c, ImVec2 d, ImVec2 uv_a, ImVec2 uv_b, ImVec2 uv_c, ImVec2 uv_d, uint32_t col);
			void PrimWriteVtx(ImDrawList* dl, ImVec2 pos, ImVec2 uv, uint32_t col);
			void PrimWriteIdx(ImDrawList* dl, ImDrawIdx idx);
			void PrimVtx(ImDrawList* dl, ImVec2 pos, ImVec2 uv, uint32_t col);

			void AddCallback(ImDrawList* dl, ImDrawCallback callback, void* userdata, size_t userdata_size);
			void AddDrawCmd(ImDrawList* dl);
		}
	}
	struct ImDrawList_Opaque
	{
		ImVector<ImDrawCmd> CmdBuffer;
		ImVector<ImDrawIdx> IdxBuffer;
		ImVector<ImDrawVert> VtxBuffer;
		ImDrawListFlags Flags;

		unsigned int _VtxCurrentIdx;
		ImDrawListSharedData* _Data;
		ImDrawVert* _VtxWritePtr;
		ImDrawIdx* _IdxWritePtr;
		ImVector<ImVec2> _Path;
		ImDrawCmdHeader _CmdHeader;
		ImDrawListSplitter _Splitter;
		ImVector<ImVec4> _ClipRectStack;
		ImVector<ImTextureRef> _TextureStack;
		ImVector<ImU8> _CallbacksDataBuf;
		float _FringeScale;
		const char* _OwnerName;

		void PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect = false)
		{
			ImGui::DrawList::PushClipRect(this, clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
		}
		void PushClipRectFullScreen()
		{
			ImGui::DrawList::PushClipRectFullScreen(this);
		}
		void PopClipRect()
		{
			ImGui::DrawList::PopClipRect(this);
		}
		void PushTexture(ImTextureRef tex_ref)
		{
			ImGui::DrawList::PushTexture(this, tex_ref);
		}
		void PopTexture()
		{
			ImGui::DrawList::PopTexture(this);
		}
		inline ImVec2 GetClipRectMin() const
		{
			const ImVec4& cr = _ClipRectStack.back();
			return ImVec2(cr.x, cr.y);
		}
		inline ImVec2 GetClipRectMax() const
		{
			const ImVec4& cr = _ClipRectStack.back();
			return ImVec2(cr.z, cr.w);
		}

		void AddLine(const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness = 1.0f)
		{
			ImGui::DrawList::AddLine(this, p1, p2, col, thickness);
		}
		void AddLineH(float min_x, float max_x, float y, ImU32 col, float thickness = 1.0f)
		{
			ImGui::DrawList::AddLineH(this, min_x, max_x, y, col, thickness);
		}
		void AddLineV(float x, float min_y, float max_y, ImU32 col, float thickness = 1.0f)
		{
			ImGui::DrawList::AddLineV(this, x, min_y, max_y, col, thickness);
		}
		void AddRect(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, float thickness = 1.0f, ImDrawFlags flags = 0)
		{
			ImGui::DrawList::AddRect(this, p_min, p_max, col, rounding, thickness, flags);
		}
		void AddRectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0)
		{
			ImGui::DrawList::AddRectFilled(this, p_min, p_max, col, rounding, flags);
		}
		void AddRectFilledMultiColor(const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left)
		{
			ImGui::DrawList::AddRectFilledMultiColor(this, p_min, p_max, col_upr_left, col_upr_right, col_bot_right, col_bot_left);
		}
		void AddQuad(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness = 1.0f)
		{
			ImGui::DrawList::AddQuad(this, p1, p2, p3, p4, col, thickness);
		}
		void AddQuadFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col)
		{
			ImGui::DrawList::AddQuadFilled(this, p1, p2, p3, p4, col);
		}
		void AddTriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness = 1.0f)
		{
			ImGui::DrawList::AddTriangle(this, p1, p2, p3, col, thickness);
		}
		void AddTriangleFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col)
		{
			ImGui::DrawList::AddTriangleFilled(this, p1, p2, p3, col);
		}
		void AddCircle(const ImVec2& center, float radius, ImU32 col, int num_segments = 0, float thickness = 1.0f)
		{
			ImGui::DrawList::AddCircle(this, center, radius, col, num_segments, thickness);
		}
		void AddCircleFilled(const ImVec2& center, float radius, ImU32 col, int num_segments = 0)
		{
			ImGui::DrawList::AddCircleFilled(this, center, radius, col, num_segments);
		}
		void AddNgon(const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness = 1.0f)
		{
			ImGui::DrawList::AddNgon(this, center, radius, col, num_segments, thickness);
		}
		void AddNgonFilled(const ImVec2& center, float radius, ImU32 col, int num_segments)
		{
			ImGui::DrawList::AddNgonFilled(this, center, radius, col, num_segments);
		}
		void AddEllipse(const ImVec2& center, const ImVec2& radius, ImU32 col, float rot = 0.0f, int num_segments = 0, float thickness = 1.0f)
		{
			ImGui::DrawList::AddEllipse(this, center, radius, col, rot, num_segments, thickness);
		}
		void AddEllipseFilled(const ImVec2& center, const ImVec2& radius, ImU32 col, float rot = 0.0f, int num_segments = 0)
		{
			ImGui::DrawList::AddEllipseFilled(this, center, radius, col, rot, num_segments);
		}
		void AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL)
		{
			ImGui::DrawList::AddText(this, pos, col, text_begin, text_end);
		}
		void AddText(ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL)
		{
			ImGui::DrawList::AddText(this, font, font_size, pos, col, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
		}
		void AddBezierCubic(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments = 0)
		{
			ImGui::DrawList::AddBezierCubic(this, p1, p2, p3, p4, col, thickness, num_segments);
		}
		void AddBezierQuadratic(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness, int num_segments = 0)
		{
			ImGui::DrawList::AddBezierQuadratic(this, p1, p2, p3, col, thickness, num_segments);
		}
		void AddPolyline(const ImVec2* points, int num_points, ImU32 col, float thickness, ImDrawFlags flags = 0)
		{
			ImGui::DrawList::AddPolyline(this, points, num_points, col, thickness, flags);
		}
		void AddConvexPolyFilled(const ImVec2* points, int num_points, ImU32 col)
		{
			ImGui::DrawList::AddConvexPolyFilled(this, points, num_points, col);
		}
		void AddConcavePolyFilled(const ImVec2* points, int num_points, ImU32 col)
		{
			ImGui::DrawList::AddConcavePolyFilled(this, points, num_points, col);
		}

		void AddImage(ImTextureRef tex_ref, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1), ImU32 col = IM_COL32_WHITE)
		{
			ImGui::DrawList::AddImage(this, tex_ref, p_min, p_max, uv_min, uv_max, col);
		}
		void AddImageQuad(ImTextureRef tex_ref, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& uv1 = ImVec2(0, 0), const ImVec2& uv2 = ImVec2(1, 0), const ImVec2& uv3 = ImVec2(1, 1), const ImVec2& uv4 = ImVec2(0, 1), ImU32 col = IM_COL32_WHITE)
		{
			ImGui::DrawList::AddImageQuad(this, tex_ref, p1, p2, p3, p4, uv1, uv2, uv3, uv4, col);
		}
		void AddImageRounded(ImTextureRef tex_ref, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min, const ImVec2& uv_max, ImU32 col, float rounding, ImDrawFlags flags = 0)
		{
			ImGui::DrawList::AddImageRounded(this, tex_ref, p_min, p_max, uv_min, uv_max, col, rounding, flags);
		}

		inline void PathClear() { _Path.Size = 0; }
		inline void PathLineTo(const ImVec2& pos) { _Path.push_back(pos); }
		inline void PathLineToMergeDuplicate(const ImVec2& pos)
		{
			if (_Path.Size == 0 || memcmp(&_Path.Data[_Path.Size - 1], &pos, 8) != 0)
				_Path.push_back(pos);
		}
		inline void PathFillConvex(ImU32 col)
		{
			AddConvexPolyFilled(_Path.Data, _Path.Size, col);
			_Path.Size = 0;
		}
		inline void PathFillConcave(ImU32 col)
		{
			AddConcavePolyFilled(_Path.Data, _Path.Size, col);
			_Path.Size = 0;
		}
		inline void PathStroke(ImU32 col, float thickness = 1.0f, ImDrawFlags flags = 0)
		{
			AddPolyline(_Path.Data, _Path.Size, col, thickness, flags);
			_Path.Size = 0;
		}
		void PathArcTo(const ImVec2& center, float radius, float a_min, float a_max, int num_segments = 0)
		{
			ImGui::DrawList::PathArcTo(this, center, radius, a_min, a_max, num_segments);
		}
		void PathArcToFast(const ImVec2& center, float radius, int a_min_of_12, int a_max_of_12)
		{
			ImGui::DrawList::PathArcToFast(this, center, radius, a_min_of_12, a_max_of_12);
		}
		void PathEllipticalArcTo(const ImVec2& center, const ImVec2& radius, float rot, float a_min, float a_max, int num_segments = 0)
		{
			ImGui::DrawList::PathEllipticalArcTo(this, center, radius, rot, a_min, a_max, num_segments);
		}
		void PathBezierCubicCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments = 0)
		{
			ImGui::DrawList::PathBezierCubicCurveTo(this, p2, p3, p4, num_segments);
		}
		void PathBezierQuadraticCurveTo(const ImVec2& p2, const ImVec2& p3, int num_segments = 0)
		{
			ImGui::DrawList::PathBezierQuadraticCurveTo(this, p2, p3, num_segments);
		}
		void PathRect(const ImVec2& rect_min, const ImVec2& rect_max, float rounding = 0.0f, ImDrawFlags flags = 0)
		{
			ImGui::DrawList::PathRect(this, rect_min, rect_max, rounding, flags);
		}

		void AddCallback(ImDrawCallback callback, void* userdata = NULL, size_t userdata_size = 0)
		{
			ImGui::DrawList::AddCallback(this, callback, userdata, userdata_size);
		}

		void AddDrawCmd()
		{
			ImGui::DrawList::AddDrawCmd(this);
		}

		void PrimReserve(int idx_count, int vtx_count)
		{
			ImGui::DrawList::PrimReserve(this, idx_count, vtx_count);
		}
		void PrimUnreserve(int idx_count, int vtx_count)
		{
			ImGui::DrawList::PrimUnreserve(this, idx_count, vtx_count);
		}
		void PrimRect(const ImVec2& a, const ImVec2& b, ImU32 col)
		{
			ImGui::DrawList::PrimRect(this, a, b, col);
		}
		void PrimRectUV(const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col)
		{
			ImGui::DrawList::PrimRectUV(this, a, b, uv_a, uv_b, col);
		}
		void PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
		{
			ImGui::DrawList::PrimQuadUV(this, a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);
		}

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS

#else

#endif
	};
	struct Clipper
	{
		int DisplayStart;
		int DisplayEnd;
		int UserIndex;

		virtual void Begin(int items_count, float items_height = -1.0f) = 0;
		virtual void End() = 0;
		virtual bool Step() = 0;

		virtual void IncludeItemsByIndex(int item_begin, int item_end) = 0;

		virtual void SeekCursorForItem(int item_index) = 0;
	};
	struct ImGuiListClipper
	{
		int DisplayStart;
		int DisplayEnd;
		int UserIndex;

		ImGuiListClipper()
		{
			clipper = ImGui::detail::ImGuiListClipper_Create();
		}
		~ImGuiListClipper()
		{
			ImGui::detail::ImGuiListClipper_Destroy(clipper);
		}
		void Begin(int items_count, float items_height = -1.0f)
		{
			clipper->Begin(items_count, items_height);
		}
		void End()
		{
			clipper->End();
		}
		bool Step()
		{
			return clipper->Step();
		}

		inline void IncludeItemByIndex(int item_index) { IncludeItemsByIndex(item_index, item_index + 1); }
		void IncludeItemsByIndex(int item_begin, int item_end)
		{
			clipper->IncludeItemsByIndex(item_begin, item_end);
		}

		void SeekCursorForItem(int item_index)
		{
			clipper->SeekCursorForItem(item_index);
		}

	private:
		Clipper* clipper;
		void UpdateValues()
		{
			DisplayStart = clipper->DisplayStart;
			DisplayEnd = clipper->DisplayEnd;
			UserIndex = clipper->UserIndex;
		}
	};
	struct ImVec2ih
	{
		short x, y;
		constexpr ImVec2ih() : x(0), y(0) {}
		constexpr ImVec2ih(short _x, short _y) : x(_x), y(_y) {}
		constexpr explicit ImVec2ih(const ImVec2& rhs) : x((short)rhs.x), y((short)rhs.y) {}
	};
	struct ImGuiMenuColumns
	{
		ImU32 TotalWidth;
		ImU32 NextTotalWidth;
		ImU16 Spacing;
		ImU16 OffsetIcon;
		ImU16 OffsetLabel;
		ImU16 OffsetShortcut;
		ImU16 OffsetMark;
		ImU16 Widths[4];
	};
	struct ImGuiShrinkWidthItem
	{
		int Index;
		float Width;
		float InitialWidth;
	};
	struct ImGuiDebugAllocEntry
	{
		int FrameCount;
		ImS16 AllocCount;
		ImS16 FreeCount;
	};
	struct ImGuiDebugAllocInfo
	{
		int TotalAllocCount;
		int TotalFreeCount;
		ImS16 LastEntriesIdx;
		ImGuiDebugAllocEntry LastEntriesBuf[6];

		ImGuiDebugAllocInfo() { memset((void*)this, 0, sizeof(*this)); }
	};
	struct ImGuiWindowSettings
	{
		ImGuiID ID;
		ImVec2ih Pos;
		ImVec2ih Size;
		bool Collapsed;
		bool IsChild;
		bool WantApply;
		bool WantDelete;

		ImGuiWindowSettings() { memset((void*)this, 0, sizeof(*this)); }
		char* GetName() { return (char*)(this + 1); }
	};
	struct ImGuiWindowTempData
	{
		ImVec2 CursorPos;
		ImVec2 CursorPosPrevLine;
		ImVec2 CursorStartPos;
		ImVec2 CursorMaxPos;
		ImVec2 IdealMaxPos;
		ImVec2 CurrLineSize;
		ImVec2 PrevLineSize;
		float CurrLineTextBaseOffset;
		float PrevLineTextBaseOffset;
		bool IsSameLine;
		bool IsSetPos;
		ImVec1 Indent;
		ImVec1 ColumnsOffset;
		ImVec1 GroupOffset;
		ImVec2 CursorStartPosLossyness;

		ImGuiNavLayer NavLayerCurrent;
		short NavLayersActiveMask;
		short NavLayersActiveMaskNext;
		bool NavIsScrollPushableX;
		bool NavHideHighlightOneFrame;
		bool NavWindowHasScrollY;

		bool MenuBarAppending;
		ImVec2 MenuBarOffset;
		ImGuiMenuColumns MenuColumns;
		int TreeDepth;
		ImU32 TreeHasStackDataDepthMask;
		ImU32 TreeRecordsClippedNodesY2Mask;
		ImVector<ImGuiWindow*> ChildWindows;
		ImGuiStorage* StateStorage;
		ImGuiOldColumns* CurrentColumns;
		int CurrentTableIdx;
		ImGuiLayoutType LayoutType;
		ImGuiLayoutType ParentLayoutType;
		ImU32 ModalDimBgColor;
		ImGuiItemStatusFlags WindowItemStatusFlags;
		ImGuiItemStatusFlags ChildItemStatusFlags;

		float ItemWidth;
		float ItemWidthDefault;
		float TextWrapPos;
		ImVector<float> ItemWidthStack;
		ImVector<float> TextWrapPosStack;
	};

	namespace ImGui
	{
		namespace detail
		{

			inline ImGuiID ImGuiWindow_GetID_Str(ImGuiWindow* self, const char* str, const char* str_end);
			inline ImGuiID ImGuiWindow_GetID_Ptr(ImGuiWindow* self, const void* ptr);
			inline ImGuiID ImGuiWindow_GetID_Int(ImGuiWindow* self, int n);
			inline ImGuiID ImGuiWindow_GetIDFromPos(ImGuiWindow* self, const ImVec2 p_abs);
			inline ImGuiID ImGuiWindow_GetIDFromRectangle(ImGuiWindow* self, const ImRect& r_abs);
		}
	}

	struct ImGuiWindow
	{
		ImGuiContext* Ctx;
		char* Name;
		ImGuiID ID;
		ImGuiWindowFlags Flags;
		ImGuiChildFlags ChildFlags;
		ImGuiViewportP* Viewport;
		ImVec2 Pos;
		ImVec2 Size;
		ImVec2 SizeFull;
		ImVec2 ContentSize;
		ImVec2 ContentSizeIdeal;
		ImVec2 ContentSizeExplicit;
		ImVec2 WindowPadding;
		float WindowRounding;
		float WindowBorderSize;
		float TitleBarHeight, MenuBarHeight;
		float DecoOuterSizeX1, DecoOuterSizeY1;
		float DecoOuterSizeX2, DecoOuterSizeY2;
		float DecoInnerSizeX1, DecoInnerSizeY1;
		int NameBufLen;
		ImGuiID MoveId;
		ImGuiID ChildId;
		ImGuiID PopupId;
		ImVec2 Scroll;
		ImVec2 ScrollMax;
		ImVec2 ScrollTarget;
		ImVec2 ScrollTargetCenterRatio;
		ImVec2 ScrollTargetEdgeSnapDist;
		ImVec2 ScrollbarSizes;
		bool ScrollbarX, ScrollbarY;
		bool ScrollbarXStabilizeEnabled;
		ImU8 ScrollbarXStabilizeToggledHistory;
		bool Active;
		bool WasActive;
		bool WriteAccessed;
		bool Collapsed;
		bool WantCollapseToggle;
		bool SkipItems;
		bool SkipRefresh;
		bool Appearing;
		bool Hidden;
		bool IsFallbackWindow;
		bool IsExplicitChild;
		bool HasCloseButton;
		signed char ResizeBorderHovered;
		signed char ResizeBorderHeld;
		short BeginCount;
		short BeginCountPreviousFrame;
		short BeginOrderWithinParent;
		short BeginOrderWithinContext;
		short FocusOrder;
		ImGuiDir AutoPosLastDirection;
		ImS8 AutoFitFramesX, AutoFitFramesY;
		bool AutoFitOnlyGrows;
		ImS8 HiddenFramesCanSkipItems;
		ImS8 HiddenFramesCannotSkipItems;
		ImS8 HiddenFramesForRenderOnly;
		ImS8 DisableInputsFrames;
		ImGuiWindowBgClickFlags BgClickFlags: 8;
		ImGuiCond SetWindowPosAllowFlags: 8;
		ImGuiCond SetWindowSizeAllowFlags: 8;
		ImGuiCond SetWindowCollapsedAllowFlags: 8;
		ImVec2 SetWindowPosVal;
		ImVec2 SetWindowPosPivot;

		ImVector<ImGuiID> IDStack;
		ImGuiWindowTempData DC;

		ImRect OuterRectClipped;
		ImRect InnerRect;
		ImRect InnerClipRect;
		ImRect WorkRect;
		ImRect ParentWorkRect;
		ImRect ClipRect;
		ImRect ContentRegionRect;
		ImVec2ih HitTestHoleSize;
		ImVec2ih HitTestHoleOffset;

		int LastFrameActive;
		float LastTimeActive;
		ImGuiStorage StateStorage;
		ImVector<ImGuiOldColumns> ColumnsStorage;
		float FontWindowScale;
		float FontWindowScaleParents;
		float FontRefSize;
		int SettingsOffset;

		ImDrawList_Opaque* DrawList;
		ImDrawList_Opaque DrawListInst;
		ImGuiWindow* ParentWindow;
		ImGuiWindow* ParentWindowInBeginStack;
		ImGuiWindow* RootWindow;
		ImGuiWindow* RootWindowPopupTree;
		ImGuiWindow* RootWindowForTitleBarHighlight;
		ImGuiWindow* RootWindowForNav;
		ImGuiWindow* ParentWindowForFocusRoute;

		ImGuiWindow* NavLastChildNavWindow;
		ImGuiID NavLastIds[ImGuiNavLayer_COUNT];
		ImRect NavRectRel[ImGuiNavLayer_COUNT];
		ImVec2 NavPreferredScoringPosRel[ImGuiNavLayer_COUNT];
		ImGuiID NavRootFocusScopeId;

		int MemoryDrawListIdxCapacity;
		int MemoryDrawListVtxCapacity;
		bool MemoryCompacted;

	public:
		ImGuiID GetID(const char* str, const char* str_end = NULL)
		{
			return ImGui::detail::ImGuiWindow_GetID_Str(this, str, str_end);
		}
		ImGuiID GetID(const void* ptr)
		{
			return ImGui::detail::ImGuiWindow_GetID_Ptr(this, ptr);
		}
		ImGuiID GetID(int n)
		{
			return ImGui::detail::ImGuiWindow_GetID_Int(this, n);
		}
		ImGuiID GetIDFromPos(const ImVec2& p_abs)
		{
			return ImGui::detail::ImGuiWindow_GetIDFromPos(this, p_abs);
		}
		ImGuiID GetIDFromRectangle(const ImRect& r_abs)
		{
			return ImGui::detail::ImGuiWindow_GetIDFromRectangle(this, r_abs);
		}

		ImRect Rect() const { return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
		ImRect TitleBarRect() const { return ImRect(Pos, ImVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight)); }
		ImRect MenuBarRect() const
		{
			float y1 = Pos.y + TitleBarHeight;
			return ImRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight);
		}
	};
	struct ImGuiPlatformIO
	{
		const char* (*Platform_GetClipboardTextFn)(ImGuiContext* ctx);
		void (*Platform_SetClipboardTextFn)(ImGuiContext* ctx, const char* text);
		void* Platform_ClipboardUserData;

		bool (*Platform_OpenInShellFn)(ImGuiContext* ctx, const char* path);
		void* Platform_OpenInShellUserData;

		void (*Platform_SetImeDataFn)(ImGuiContext* ctx, ImGuiViewport* viewport, ImGuiPlatformImeData* data);
		void* Platform_ImeUserData;
		ImWchar Platform_LocaleDecimalPoint;

		int Renderer_TextureMaxWidth;
		int Renderer_TextureMaxHeight;

		void* Renderer_RenderState;

		ImDrawCallback DrawCallback_ResetRenderState;
		ImDrawCallback DrawCallback_SetSamplerLinear;
		ImDrawCallback DrawCallback_SetSamplerNearest;
		ImVector<ImTextureData*> Textures;
	};
	struct ImGuiKeyData
	{
		bool Down;
		float DownDuration;
		float DownDurationPrev;
		float AnalogValue;
	};

	struct ImGuiIO
	{
		ImGuiConfigFlags ConfigFlags;
		ImGuiBackendFlags BackendFlags;
		ImVec2 DisplaySize;
		ImVec2 DisplayFramebufferScale;
		float DeltaTime;
		float IniSavingRate;
		const char* IniFilename;
		const char* LogFilename;
		void* UserData;

		ImFontAtlas* Fonts;
		ImFont* FontDefault;
		bool FontAllowUserScaling;

		bool ConfigNavSwapGamepadButtons;
		bool ConfigNavMoveSetMousePos;
		bool ConfigNavCaptureKeyboard;
		bool ConfigNavEscapeClearFocusItem;
		bool ConfigNavEscapeClearFocusWindow;
		bool ConfigNavCursorVisibleAuto;
		bool ConfigNavCursorVisibleAlways;

		bool MouseDrawCursor;
		bool ConfigMacOSXBehaviors;
		bool ConfigInputTrickleEventQueue;
		bool ConfigInputTextCursorBlink;
		bool ConfigInputTextEnterKeepActive;
		bool ConfigDragClickToInputText;
		bool ConfigWindowsResizeFromEdges;
		bool ConfigWindowsMoveFromTitleBarOnly;
		bool ConfigWindowsCopyContentsWithCtrlC;
		bool ConfigScrollbarScrollByPage;
		float ConfigMemoryCompactTimer;

		float MouseDoubleClickTime;
		float MouseDoubleClickMaxDist;
		float MouseDragThreshold;
		float KeyRepeatDelay;
		float KeyRepeatRate;

		bool ConfigErrorRecovery;
		bool ConfigErrorRecoveryEnableAssert;
		bool ConfigErrorRecoveryEnableDebugLog;
		bool ConfigErrorRecoveryEnableTooltip;

		bool ConfigDebugIsDebuggerPresent;

		bool ConfigDebugHighlightIdConflicts;
		bool ConfigDebugHighlightIdConflictsShowItemPicker;

		bool ConfigDebugBeginReturnValueOnce;
		bool ConfigDebugBeginReturnValueLoop;

		bool ConfigDebugIgnoreFocusLoss;

		bool ConfigDebugIniSettings;

		const char* BackendPlatformName;
		const char* BackendRendererName;
		void* BackendPlatformUserData;
		void* BackendRendererUserData;
		void* BackendLanguageUserData;

		bool WantCaptureMouse;
		bool WantCaptureKeyboard;
		bool WantTextInput;
		bool WantSetMousePos;
		bool WantSaveIniSettings;
		bool NavActive;
		bool NavVisible;
		float Framerate;
		int MetricsRenderVertices;
		int MetricsRenderIndices;
		int MetricsRenderWindows;
		int MetricsActiveWindows;
		ImVec2 MouseDelta;

		ImGuiContext* Ctx;

		ImVec2 MousePos;
		bool MouseDown[5];
		float MouseWheel;
		float MouseWheelH;
		ImGuiMouseSource MouseSource;
		bool KeyCtrl;
		bool KeyShift;
		bool KeyAlt;
		bool KeySuper;

		ImGuiKeyChord KeyMods;
		ImGuiKeyData KeysData[ImGuiKey_NamedKey_COUNT];
		bool WantCaptureMouseUnlessPopupClose;
		ImVec2 MousePosPrev;
		ImVec2 MouseClickedPos[5];
		double MouseClickedTime[5];
		bool MouseClicked[5];
		bool MouseDoubleClicked[5];
		ImU16 MouseClickedCount[5];
		ImU16 MouseClickedLastCount[5];
		bool MouseReleased[5];
		double MouseReleasedTime[5];
		bool MouseDownOwned[5];
		bool MouseDownOwnedUnlessPopupClose[5];
		bool MouseWheelRequestAxisSwap;
		bool MouseCtrlLeftAsRightClick;
		float MouseDownDuration[5];
		float MouseDownDurationPrev[5];
		float MouseDragMaxDistanceSqr[5];
		float PenPressure;
		bool AppFocusLost;
		bool AppAcceptingEvents;
		ImWchar16 InputQueueSurrogate;
		ImVector<ImWchar> InputQueueCharacters;
	};
	struct ImGuiStyle
	{
		float FontSizeBase;
		float FontScaleMain;
		float FontScaleDpi;

		float Alpha;
		float DisabledAlpha;
		ImVec2 WindowPadding;
		float WindowRounding;
		float WindowBorderSize;
		float WindowBorderHoverPadding;
		ImVec2 WindowMinSize;
		ImVec2 WindowTitleAlign;
		ImGuiDir WindowMenuButtonPosition;
		float ChildRounding;
		float ChildBorderSize;
		float PopupRounding;
		float PopupBorderSize;
		ImVec2 FramePadding;
		float FrameRounding;
		float FrameBorderSize;
		ImVec2 ItemSpacing;
		ImVec2 ItemInnerSpacing;
		ImVec2 CellPadding;
		ImVec2 TouchExtraPadding;
		float IndentSpacing;
		float ColumnsMinSpacing;
		float ScrollbarSize;
		float ScrollbarRounding;
		float ScrollbarPadding;
		float GrabMinSize;
		float GrabRounding;
		float LogSliderDeadzone;
		float ImageRounding;
		float ImageBorderSize;
		float TabRounding;
		float TabBorderSize;
		float TabMinWidthBase;
		float TabMinWidthShrink;
		float TabCloseButtonMinWidthSelected;
		float TabCloseButtonMinWidthUnselected;
		float TabBarBorderSize;
		float TabBarOverlineSize;
		float TableAngledHeadersAngle;
		ImVec2 TableAngledHeadersTextAlign;
		ImGuiTreeNodeFlags TreeLinesFlags;
		float TreeLinesSize;
		float TreeLinesRounding;
		float DragDropTargetRounding;
		float DragDropTargetBorderSize;
		float DragDropTargetPadding;
		float ColorMarkerSize;
		ImGuiDir ColorButtonPosition;
		ImVec2 ButtonTextAlign;
		ImVec2 SelectableTextAlign;
		float SeparatorSize;
		float SeparatorTextBorderSize;
		ImVec2 SeparatorTextAlign;
		ImVec2 SeparatorTextPadding;
		ImVec2 DisplayWindowPadding;
		ImVec2 DisplaySafeAreaPadding;
		float MouseCursorScale;
		bool AntiAliasedLines;
		bool AntiAliasedLinesUseTex;
		bool AntiAliasedFill;
		float CurveTessellationTol;
		float CircleTessellationMaxError;

		ImVec4 Colors[ImGuiCol_COUNT];

		float HoverStationaryDelay;
		float HoverDelayShort;
		float HoverDelayNormal;
		ImGuiHoveredFlags HoverFlagsForTooltipMouse;
		ImGuiHoveredFlags HoverFlagsForTooltipNav;

		float _MainScale;
		float _NextFrameFontSizeBase;
	};
	struct ImGuiContext
	{
		bool Initialized;
		bool WithinFrameScope;
		bool WithinFrameScopeWithImplicitWindow;
		bool TestEngineHookItems;
		int FrameCount;
		int FrameCountEnded;
		int FrameCountRendered;
		double Time;
		char ContextName[16];
		ImGuiIO IO;
		ImGuiPlatformIO PlatformIO;
		ImGuiStyle Style;
		ImVector<ImFontAtlas*> FontAtlases;
		ImFont* Font;
		ImFontBaked* FontBaked;
		float FontSize;
		float FontSizeBase;
		float FontBakedScale;
		float FontRasterizerDensity;
		float CurrentDpiScale;
		ImDrawListSharedData DrawListSharedData;
		ImGuiID WithinEndChildID;
		ImGuiID WithinEndPopupID;
		void* TestEngine;

		ImVector<ImGuiInputEvent> InputEventsQueue;
		ImVector<ImGuiInputEvent> InputEventsTrail;
		ImGuiMouseSource InputEventsNextMouseSource;
		ImU32 InputEventsNextEventId;

		ImVector<ImGuiWindow*> Windows;
		ImVector<ImGuiWindow*> WindowsFocusOrder;
		ImVector<ImGuiWindow*> WindowsTempSortBuffer;
		ImVector<ImGuiWindowStackData> CurrentWindowStack;
		ImGuiStorage WindowsById;
		int WindowsActiveCount;
		float WindowsBorderHoverPadding;
		ImGuiID DebugBreakInWindow;
		ImGuiWindow* CurrentWindow;
		ImGuiWindow* HoveredWindow;
		ImGuiWindow* HoveredWindowUnderMovingWindow;
		ImGuiWindow* HoveredWindowBeforeClear;
		ImGuiWindow* MovingWindow;
		ImGuiWindow* WheelingWindow;
		ImVec2 WheelingWindowRefMousePos;
		int WheelingWindowStartFrame;
		int WheelingWindowScrolledFrame;
		float WheelingWindowReleaseTimer;
		ImVec2 WheelingWindowWheelRemainder;
		ImVec2 WheelingAxisAvg;

		ImGuiID DebugDrawIdConflictsId;
		ImGuiID DebugHookIdInfoId;
		ImGuiID HoveredId;
		ImGuiID HoveredIdPreviousFrame;
		int HoveredIdPreviousFrameItemCount;
		float HoveredIdTimer;
		float HoveredIdNotActiveTimer;
		bool HoveredIdAllowOverlap;
		bool HoveredIdIsDisabled;
		bool ItemUnclipByLog;
		ImGuiID ActiveId;
		ImGuiID ActiveIdIsAlive;
		float ActiveIdTimer;
		bool ActiveIdIsJustActivated;
		bool ActiveIdAllowOverlap;
		bool ActiveIdNoClearOnFocusLoss;
		bool ActiveIdHasBeenPressedBefore;
		bool ActiveIdHasBeenEditedBefore;
		bool ActiveIdHasBeenEditedThisFrame;
		bool ActiveIdFromShortcut;
		ImS8 ActiveIdMouseButton;
		ImGuiID ActiveIdDisabledId;
		ImVec2 ActiveIdClickOffset;
		ImGuiInputSource ActiveIdSource;
		ImGuiWindow* ActiveIdWindow;
		ImGuiID ActiveIdPreviousFrame;
		ImGuiDeactivatedItemData DeactivatedItemData;
		ImGuiDataTypeStorage ActiveIdValueOnActivation;
		ImGuiID LastActiveId;
		float LastActiveIdTimer;

		double LastKeyModsChangeTime;
		double LastKeyModsChangeFromNoneTime;
		double LastKeyboardKeyPressTime;
		ImBitArrayForNamedKeys KeysMayBeCharInput;
		ImGuiKeyOwnerData KeysOwnerData[ImGuiKey_NamedKey_COUNT];
		ImGuiKeyRoutingTable KeysRoutingTable;
		ImU32 ActiveIdUsingNavDirMask;
		bool ActiveIdUsingAllKeyboardKeys;
		ImGuiKeyChord DebugBreakInShortcutRouting;

		ImGuiID CurrentFocusScopeId;
		ImGuiItemFlags CurrentItemFlags;
		ImGuiID DebugLocateId;
		ImGuiNextItemData NextItemData;
		ImGuiLastItemData LastItemData;
		ImGuiNextWindowData NextWindowData;
		bool DebugShowGroupRects;
		bool GcCompactAll;

		ImGuiCol DebugFlashStyleColorIdx;
		ImVector<ImGuiColorMod> ColorStack;
		ImVector<ImGuiStyleMod> StyleVarStack;
		ImVector<ImFontStackData> FontStack;
		ImVector<ImGuiFocusScopeData> FocusScopeStack;
		ImVector<ImGuiItemFlags> ItemFlagsStack;
		ImVector<ImGuiGroupData> GroupStack;
		ImVector<ImGuiPopupData> OpenPopupStack;
		ImVector<ImGuiPopupData> BeginPopupStack;
		ImVector<ImGuiTreeNodeStackData> TreeNodeStack;

		ImVector<ImGuiViewportP*> Viewports;

		bool NavCursorVisible;
		bool NavHighlightItemUnderNav;

		bool NavMousePosDirty;
		bool NavIdIsAlive;
		ImGuiID NavId;
		ImGuiWindow* NavWindow;
		ImGuiID NavFocusScopeId;
		ImGuiNavLayer NavLayer;
		ImGuiItemFlags NavIdItemFlags;
		ImGuiID NavActivateId;
		ImGuiID NavActivateDownId;
		ImGuiID NavActivatePressedId;
		ImGuiActivateFlags NavActivateFlags;
		ImVector<ImGuiFocusScopeData> NavFocusRoute;
		ImGuiID NavHighlightActivatedId;
		float NavHighlightActivatedTimer;
		ImGuiID NavOpenContextMenuItemId;
		ImGuiID NavOpenContextMenuWindowId;
		ImGuiID NavNextActivateId;
		ImGuiActivateFlags NavNextActivateFlags;
		ImGuiInputSource NavInputSource;
		ImGuiSelectionUserData NavLastValidSelectionUserData;
		ImS8 NavCursorHideFrames;

		bool NavAnyRequest;
		bool NavInitRequest;
		bool NavInitRequestFromMove;
		ImGuiNavItemData NavInitResult;
		bool NavMoveSubmitted;
		bool NavMoveScoringItems;
		bool NavMoveForwardToNextFrame;
		ImGuiNavMoveFlags NavMoveFlags;
		ImGuiScrollFlags NavMoveScrollFlags;
		ImGuiKeyChord NavMoveKeyMods;
		ImGuiDir NavMoveDir;
		ImGuiDir NavMoveDirForDebug;
		ImGuiDir NavMoveClipDir;
		ImRect NavScoringRect;
		ImRect NavScoringNoClipRect;
		int NavScoringDebugCount;
		int NavTabbingDir;
		int NavTabbingCounter;
		ImGuiNavItemData NavMoveResultLocal;
		ImGuiNavItemData NavMoveResultLocalVisible;
		ImGuiNavItemData NavMoveResultOther;
		ImGuiNavItemData NavTabbingResultFirst;

		ImGuiID NavJustMovedFromFocusScopeId;
		ImGuiID NavJustMovedToId;
		ImGuiID NavJustMovedToFocusScopeId;
		ImGuiKeyChord NavJustMovedToKeyMods;
		bool NavJustMovedToIsTabbing;
		bool NavJustMovedToHasSelectionData;

		bool ConfigNavEnableTabbing;
		bool ConfigNavWindowingWithGamepad;
		ImGuiKeyChord ConfigNavWindowingKeyNext;
		ImGuiKeyChord ConfigNavWindowingKeyPrev;

		ImGuiWindow* NavWindowingTarget;
		ImGuiWindow* NavWindowingTargetAnim;
		ImGuiWindow* NavWindowingListWindow;
		float NavWindowingTimer;
		float NavWindowingHighlightAlpha;
		ImGuiInputSource NavWindowingInputSource;
		bool NavWindowingToggleLayer;
		ImGuiKey NavWindowingToggleKey;
		ImVec2 NavWindowingAccumDeltaPos;
		ImVec2 NavWindowingAccumDeltaSize;

		float DimBgRatio;

		bool DragDropActive;
		bool DragDropWithinSource;
		bool DragDropWithinTarget;
		ImGuiDragDropFlags DragDropSourceFlags;
		int DragDropSourceFrameCount;
		int DragDropMouseButton;
		ImGuiPayload DragDropPayload;
		ImRect DragDropTargetRect;
		ImRect DragDropTargetClipRect;
		ImGuiID DragDropTargetId;
		ImGuiID DragDropTargetFullViewport;
		ImGuiDragDropFlags DragDropAcceptFlagsCurr;
		ImGuiDragDropFlags DragDropAcceptFlagsPrev;
		float DragDropAcceptIdCurrRectSurface;
		ImGuiID DragDropAcceptIdCurr;
		ImGuiID DragDropAcceptIdPrev;
		int DragDropAcceptFrameCount;
		ImGuiID DragDropHoldJustPressedId;
		ImVector<unsigned char> DragDropPayloadBufHeap;
		unsigned char DragDropPayloadBufLocal[16];

		int ClipperTempDataStacked;
		ImVector<ImGuiListClipperData> ClipperTempData;

		ImGuiTable* CurrentTable;
		ImGuiID DebugBreakInTable;
		int TablesTempDataStacked;
		ImVector<ImGuiTableTempData> TablesTempData;
		ImPool<ImGuiTable> Tables;
		ImVector<float> TablesLastTimeActive;
		ImVector<ImDrawChannel> DrawChannelsTempMergeBuffer;

		ImGuiTabBar* CurrentTabBar;
		ImPool<ImGuiTabBar> TabBars;
		ImVector<ImGuiPtrOrIndex> CurrentTabBarStack;
		ImVector<ImGuiShrinkWidthItem> ShrinkWidthBuffer;

		ImGuiBoxSelectState BoxSelectState;
		ImGuiMultiSelectTempData* CurrentMultiSelect;
		int MultiSelectTempDataStacked;
		ImVector<ImGuiMultiSelectTempData> MultiSelectTempData;
		ImPool<ImGuiMultiSelectState> MultiSelectStorage;

		ImGuiID HoverItemDelayId;
		ImGuiID HoverItemDelayIdPreviousFrame;
		float HoverItemDelayTimer;
		float HoverItemDelayClearTimer;
		ImGuiID HoverItemUnlockedStationaryId;
		ImGuiID HoverWindowUnlockedStationaryId;

		ImGuiMouseCursor MouseCursor;
		float MouseStationaryTimer;
		ImVec2 MouseLastValidPos;

		ImGuiInputTextState InputTextState;
		ImGuiTextIndex InputTextLineIndex;
		ImGuiInputTextDeactivatedState InputTextDeactivatedState;
		ImFontBaked InputTextPasswordFontBackupBaked;
		ImFontFlags InputTextPasswordFontBackupFlags;
		ImGuiID InputTextReactivateId;
		ImGuiID TempInputId;
		ImGuiDataTypeStorage DataTypeZeroValue;
		int BeginMenuDepth;
		int BeginComboDepth;
		ImGuiColorEditFlags ColorEditOptions;
		ImGuiID ColorEditCurrentID;
		ImGuiID ColorEditSavedID;
		float ColorEditSavedHue;
		float ColorEditSavedSat;
		ImU32 ColorEditSavedColor;
		ImVec4 ColorPickerRef;
		ImGuiComboPreviewData ComboPreviewData;
		ImRect WindowResizeBorderExpectedRect;
		bool WindowResizeRelativeMode;
		short ScrollbarSeekMode;
		float ScrollbarClickDeltaToGrabCenter;
		float SliderGrabClickOffset;
		float SliderCurrentAccum;
		bool SliderCurrentAccumDirty;
		bool DragCurrentAccumDirty;
		float DragCurrentAccum;
		float DragSpeedDefaultRatio;
		float DisabledAlphaBackup;
		short DisabledStackSize;
		short TooltipOverrideCount;
		ImGuiWindow* TooltipPreviousWindow;
		ImVector<char> ClipboardHandlerData;
		ImVector<ImGuiID> MenusIdSubmittedThisFrame;
		ImGuiTypingSelectState TypingSelectState;

		ImGuiPlatformImeData PlatformImeData;
		ImGuiPlatformImeData PlatformImeDataPrev;

		ImVector<ImTextureData*> UserTextures;

		bool SettingsLoaded;
		float SettingsDirtyTimer;
		ImGuiTextBuffer SettingsIniData;
		ImVector<ImGuiSettingsHandler> SettingsHandlers;
		ImChunkStream<ImGuiWindowSettings> SettingsWindows;
		ImChunkStream<ImGuiTableSettings> SettingsTables;

		ImVector<ImGuiContextHook> Hooks;
		ImGuiID HookIdNext;
		ImGuiDemoMarkerCallback DemoMarkerCallback;

		const char* LocalizationTable[ImGuiLocKey_COUNT];

		bool LogEnabled;
		bool LogLineFirstItem;
		ImGuiLogFlags LogFlags;
		ImGuiWindow* LogWindow;
		ImFileHandle LogFile;
		ImGuiTextBuffer LogBuffer;
		const char* LogNextPrefix;
		const char* LogNextSuffix;
		float LogLinePosY;
		int LogDepthRef;
		int LogDepthToExpand;
		int LogDepthToExpandDefault;

		ImGuiErrorCallback ErrorCallback;
		void* ErrorCallbackUserData;
		ImVec2 ErrorTooltipLockedPos;
		bool ErrorFirst;
		int ErrorCountCurrentFrame;
		ImGuiErrorRecoveryState StackSizesInNewFrame;
		ImGuiErrorRecoveryState* StackSizesInBeginForCurrentWindow;

		int DebugDrawIdConflictsCount;
		ImGuiDebugLogFlags DebugLogFlags;
		ImGuiTextBuffer DebugLogBuf;
		ImGuiTextIndex DebugLogIndex;
		int DebugLogSkippedErrors;
		ImGuiDebugLogFlags DebugLogAutoDisableFlags;
		ImU8 DebugLogAutoDisableFrames;
		ImU8 DebugLocateFrames;
		bool DebugBreakInLocateId;
		ImGuiKeyChord DebugBreakKeyChord;
		ImS8 DebugBeginReturnValueCullDepth;
		bool DebugItemPickerActive;
		ImU8 DebugItemPickerMouseButton;
		ImGuiID DebugItemPickerBreakId;
		float DebugFlashStyleColorTime;
		ImVec4 DebugFlashStyleColorBackup;
		ImGuiMetricsConfig DebugMetricsConfig;
		ImGuiDebugItemPathQuery DebugItemPathQuery;
		ImGuiIDStackTool DebugIDStackTool;
		ImGuiDebugAllocInfo DebugAllocInfo;
#if defined(IMGUI_DEBUG_HIGHLIGHT_ALL_ID_CONFLICTS) && !defined(IMGUI_DISABLE_DEBUG_TOOLS)
		ImGuiStorage DebugDrawIdConflictsAliveCount;
		ImGuiStorage DebugDrawIdConflictsHighlightSet;
#endif

		float FramerateSecPerFrame[60];
		int FramerateSecPerFrameIdx;
		int FramerateSecPerFrameCount;
		float FramerateSecPerFrameAccum;
		int WantCaptureMouseNextFrame;
		int WantCaptureKeyboardNextFrame;
		int WantTextInputNextFrame;
		ImVector<char> TempBuffer;
		char TempKeychordName[64];
	};

#pragma endregion

	struct ImGuiSizeCallbackData
	{
		void* UserData;
		ImVec2 Pos;
		ImVec2 CurrentSize;
		ImVec2 DesiredSize;
	};

	/* =========================================================================
	 * THE API TABLE
	 * ========================================================================= */
	struct ImGuiPluginAPI
	{
		/* -----------------------------------------------------------------
     * API meta
     * ----------------------------------------------------------------- */
		uint32_t version;     /**< IMGUI_PLUGIN_API_VERSION filled by host */
		uint32_t struct_size; /**< sizeof(ImGuiPluginAPI) — for safe slicing */

		/* -------------------------------------------------------------------------
		* Style
		* ------------------------------------------------------------------------- */
		ImGuiStyle* (*GetStyle)(void);
		ImGuiIO* (*GetIO)(void);

		/* -------------------------------------------------------------------------
		* Context
		* ------------------------------------------------------------------------- */
		ImGuiContext* (*GetCurrentContext)(void);
		void (*SetCurrentContext)(ImGuiContext* ctx);

		/* -------------------------------------------------------------------------
		* windows
		* ------------------------------------------------------------------------- */
		bool (*Begin)(const char* name, bool* p_open, ImGuiWindowFlags flags);
		void (*End)(void);

		/* -------------------------------------------------------------------------
		* child windows
		* ------------------------------------------------------------------------- */
		bool (*BeginChild)(const char* str_id, const ImVec2 size, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags);
		bool (*BeginChildID)(ImGuiID id, const ImVec2 size, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags);
		void (*EndChild)(void);

		/* -------------------------------------------------------------------------
		* windows utilities
		* ------------------------------------------------------------------------- */
		bool (*IsWindowAppearing)(void);
		bool (*IsWindowCollapsed)(void);
		bool (*IsWindowFocused)(ImGuiFocusedFlags flags);
		bool (*IsWindowHovered)(ImGuiHoveredFlags flags);
		ImVec2 (*GetWindowPos)(void);
		ImVec2 (*GetWindowSize)(void);
		float (*GetWindowWidth)(void);
		float (*GetWindowHeight)(void);

		/* -------------------------------------------------------------------------
		* window management
		* ------------------------------------------------------------------------- */
		void (*SetNextWindowPos)(ImVec2 pos, ImGuiCond cond,
			ImVec2 pivot);
		void (*SetNextWindowSize)(ImVec2 size, ImGuiCond cond);
		void (*SetNextWindowSizeConstraints)(ImVec2 size_min,
			ImVec2 size_max,
			ImGuiSizeCallback cb,
			void* cb_user_data);
		void (*SetNextWindowContentSize)(ImVec2 size);
		void (*SetNextWindowCollapsed)(bool collapsed, ImGuiCond cond);
		void (*SetNextWindowFocus)(void);
		void (*SetNextWindowScroll)(const ImVec2 scroll);
		void (*SetNextWindowBgAlpha)(float alpha);
		void (*SetWindowPos)(ImVec2 pos, ImGuiCond cond);
		void (*SetWindowSize)(ImVec2 size, ImGuiCond cond);
		void (*SetWindowCollapsed)(bool collapsed, ImGuiCond cond);
		void (*SetWindowFocus)(void);
		void (*SetWindowFontScale)(float scale);
		void (*SetWindowPosByName)(const char* name,
			ImVec2 pos, ImGuiCond cond);
		void (*SetWindowSizeByName)(const char* name,
			ImVec2 size, ImGuiCond cond);
		void (*SetWindowCollapsedByName)(const char* name,
			bool collapsed, ImGuiCond cond);
		void (*SetWindowFocusByName)(const char* name);

		/* -------------------------------------------------------------------------
		* windows scrolling
		* ------------------------------------------------------------------------- */
		float (*GetScrollX)(void);
		float (*GetScrollY)(void);
		void (*SetScrollX)(float scroll_x);
		void (*SetScrollY)(float scroll_y);
		float (*GetScrollMaxX)(void);
		float (*GetScrollMaxY)(void);
		void (*SetScrollHereX)(float center_x_ratio);
		void (*SetScrollHereY)(float center_y_ratio);
		void (*SetScrollFromPosX)(float local_x, float center_x_ratio);
		void (*SetScrollFromPosY)(float local_y, float center_y_ratio);

		/* -------------------------------------------------------------------------
		* parameter stacks (font)
		* ------------------------------------------------------------------------- */
		void (*PushFont)(ImFont* font, float font_size_base_unscaled);
		void (*PopFont)(void);
		ImFont* (*GetFont)(void);
		float (*GetFontSize)(void);
		ImFontBaked* (*GetFontBaked)(void);

		/* -------------------------------------------------------------------------
		* Parameters stacks (shared)
		* ------------------------------------------------------------------------- */
		void (*PushStyleColor_U32)(ImGuiCol idx, ImU32 col);
		void (*PushStyleColor_Vec4)(ImGuiCol idx, const ImVec4 col);
		void (*PopStyleColor)(int count);
		void (*PushStyleVar_Float)(ImGuiStyleVar idx, float val);
		void (*PushStyleVar_Vec2)(ImGuiStyleVar idx, const ImVec2 val);
		void (*PushStyleVarX)(ImGuiStyleVar idx, float val_x);
		void (*PushStyleVarY)(ImGuiStyleVar idx, float val_y);
		void (*PopStyleVar)(int count);
		void (*PushItemFlag)(ImGuiItemFlags option, bool enabled);
		void (*PopItemFlag)(void);

		/* -------------------------------------------------------------------------
		* Parameters stacks (current window)
		* ------------------------------------------------------------------------- */
		void (*PushItemWidth)(float item_width);
		void (*PopItemWidth)(void);
		void (*SetNextItemWidth)(float item_width);
		float (*CalcItemWidth)(void);
		void (*PushTextWrapPos)(float wrap_local_pos_x);
		void (*PopTextWrapPos)(void);

		/* -------------------------------------------------------------------------
		* Style read access
		* ------------------------------------------------------------------------- */
		ImVec2 (*GetFontTexUvWhitePixel)(void);
		ImU32 (*GetColorU32_ColFloat)(ImGuiCol idx, float alpha_mul);
		ImU32 (*GetColorU32_Vec4)(const ImVec4 col);
		ImU32 (*GetColorU32_U32Float)(ImU32 col, float alpha_mul);
		const ImVec4 (*GetStyleColorVec4)(ImGuiCol idx);

		/* -------------------------------------------------------------------------
		* Layout cursor positioning
		* ------------------------------------------------------------------------- */
		ImVec2 (*GetCursorScreenPos)(void);
		void (*SetCursorScreenPos)(ImVec2 pos);
		ImVec2 (*GetContentRegionAvail)(void);
		ImVec2 (*GetCursorPos)(void);
		float (*GetCursorPosX)(void);
		float (*GetCursorPosY)(void);
		void (*SetCursorPos)(ImVec2 local_pos);
		void (*SetCursorPosX)(float local_x);
		void (*SetCursorPosY)(float local_y);
		ImVec2 (*GetCursorStartPos)(void);

		/* -------------------------------------------------------------------------
		* Other layout functions
		* ------------------------------------------------------------------------- */
		void (*Separator)(void);
		void (*SameLine)(float offset_from_start_x, float spacing);
		void (*NewLine)(void);
		void (*Spacing)(void);
		void (*Dummy)(ImVec2 size);
		void (*Indent)(float indent_w);
		void (*Unindent)(float indent_w);
		void (*BeginGroup)(void);
		void (*EndGroup)(void);
		void (*AlignTextToFramePadding)(void);
		float (*GetTextLineHeight)(void);
		float (*GetTextLineHeightWithSpacing)(void);
		float (*GetFrameHeight)(void);
		float (*GetFrameHeightWithSpacing)(void);

		/* -------------------------------------------------------------------------
		* id stack / scopes
		* ------------------------------------------------------------------------- */
		void (*PushIDStr)(const char* str_id);
		void (*PushIDStrRange)(const char* str_id_begin,
			const char* str_id_end);
		void (*PushIDPtr)(const void* ptr_id);
		void (*PushIDInt)(int int_id);
		void (*PopID)(void);
		ImGuiID (*GetIDStr)(const char* str_id);
		ImGuiID (*GetIDStrRange)(const char* str_id_begin,
			const char* str_id_end);
		ImGuiID (*GetIDPtr)(const void* ptr_id);
		ImGuiID (*GetID)(int int_id);

		/* -------------------------------------------------------------------------
		* Widgets: Text
		* ------------------------------------------------------------------------- */
		void (*TextUnformatted)(const char* text, const char* text_end);
		void (*Text)(const char* fmt, ...);
		void (*TextV)(const char* fmt, va_list args);
		void (*TextColored)(ImVec4 col, const char* fmt, ...);
		void (*TextColoredV)(ImVec4 col, const char* fmt, va_list args);
		void (*TextDisabled)(const char* fmt, ...);
		void (*TextDisabledV)(const char* fmt, va_list args);
		void (*TextWrapped)(const char* fmt, ...);
		void (*TextWrappedV)(const char* fmt, va_list args);
		void (*LabelText)(const char* label, const char* fmt, ...);
		void (*LabelTextV)(const char* label, const char* fmt, va_list args);
		void (*BulletText)(const char* fmt, ...);
		void (*BulletTextV)(const char* fmt, va_list args);
		void (*SeparatorText)(const char* label);

		/* -------------------------------------------------------------------------
		* Widgets: Main
		* ------------------------------------------------------------------------- */
		bool (*Button)(const char* label, ImVec2 size);
		bool (*SmallButton)(const char* label);
		bool (*InvisibleButton)(const char* str_id, ImVec2 size,
			ImGuiButtonFlags flags);
		bool (*ArrowButton)(const char* str_id, ImGuiDir dir);
		bool (*Checkbox)(const char* label, bool* v);
		bool (*CheckboxFlags_IntPtr)(const char* label, int* flags,
			int flags_value);
		bool (*CheckboxFlags_UintPtr)(const char* label,
			unsigned int* flags,
			unsigned int flags_value);
		bool (*RadioButton)(const char* label, bool active);
		bool (*RadioButtonIntPtr)(const char* label, int* v, int v_button);
		void (*ProgressBar)(float fraction, ImVec2 size_arg,
			const char* overlay);
		void (*Bullet)(void);
		bool (*TextLink)(const char* label);
		bool (*TextLinkOpenURL)(const char* label, const char* url);

		/* -------------------------------------------------------------------------
		* Widgets: Image
		* ------------------------------------------------------------------------- */
		void (*Image)(ImTextureRef tex_ref,
			ImVec2 size,
			ImVec2 uv0, ImVec2 uv1);
		void (*ImageWithBg)(ImTextureRef tex_ref, const ImVec2 image_size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 bg_col, const ImVec4 tint_col);
		bool (*ImageButton)(const char* str_id,
			ImTextureRef user_texture_id,
			ImVec2 size,
			ImVec2 uv0, ImVec2 uv1,
			ImVec4 bg_col, ImVec4 tint_col);

		/* -------------------------------------------------------------------------
		* Widgets: Combo
		* ------------------------------------------------------------------------- */
		bool (*BeginCombo)(const char* label, const char* preview_value,
			ImGuiComboFlags flags);
		void (*EndCombo)(void);
		bool (*Combo)(const char* label, int* current_item,
			const char* const* items, int items_count,
			int popup_max_height_in_items);
		bool (*ComboStr)(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items);
		bool (*ComboCallback)(const char* label, int* current_item,
			const char* (*getter)(void* user_data, int idx),
			void* user_data, int items_count,
			int popup_max_height_in_items);

		/* -------------------------------------------------------------------------
		* Widgets: Drag Sliders
		* ------------------------------------------------------------------------- */
		bool (*DragFloat)(const char* label, float* v,
			float v_speed, float v_min, float v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*DragFloat2)(const char* label, float v[2],
			float v_speed, float v_min, float v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*DragFloat3)(const char* label, float v[3],
			float v_speed, float v_min, float v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*DragFloat4)(const char* label, float v[4],
			float v_speed, float v_min, float v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*DragFloatRange2)(const char* label,
			float* v_current_min, float* v_current_max,
			float v_speed, float v_min, float v_max,
			const char* format, const char* format_max,
			ImGuiSliderFlags flags);
		bool (*DragInt)(const char* label, int* v,
			float v_speed, int v_min, int v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*DragInt2)(const char* label, int v[2],
			float v_speed, int v_min, int v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*DragInt3)(const char* label, int v[3],
			float v_speed, int v_min, int v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*DragInt4)(const char* label, int v[4],
			float v_speed, int v_min, int v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*DragIntRange2)(const char* label,
			int* v_current_min, int* v_current_max,
			float v_speed, int v_min, int v_max,
			const char* format, const char* format_max,
			ImGuiSliderFlags flags);
		bool (*DragScalar)(const char* label, ImGuiDataType data_type,
			void* p_data, float v_speed,
			const void* p_min, const void* p_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*DragScalarN)(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);

		/* -------------------------------------------------------------------------
		* Widgets: Regular Sliders
		* ------------------------------------------------------------------------- */
		bool (*SliderFloat)(const char* label, float* v,
			float v_min, float v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderFloat2)(const char* label, float v[2],
			float v_min, float v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderFloat3)(const char* label, float v[3],
			float v_min, float v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderFloat4)(const char* label, float v[4],
			float v_min, float v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderAngle)(const char* label, float* v_rad,
			float v_degrees_min, float v_degrees_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderInt)(const char* label, int* v,
			int v_min, int v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderInt2)(const char* label, int v[2],
			int v_min, int v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderInt3)(const char* label, int v[3],
			int v_min, int v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderInt4)(const char* label, int v[4],
			int v_min, int v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderScalar)(const char* label, ImGuiDataType data_type,
			void* p_data, const void* p_min, const void* p_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*SliderScalarN)(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
		bool (*VSliderFloat)(const char* label, ImVec2 size,
			float* v, float v_min, float v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*VSliderInt)(const char* label, ImVec2 size,
			int* v, int v_min, int v_max,
			const char* format, ImGuiSliderFlags flags);
		bool (*VSliderScalar)(const char* label, const ImVec2 size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);

		/* -------------------------------------------------------------------------
		* Widgets: Input with Keyboard
		* ------------------------------------------------------------------------- */
		bool (*InputText)(const char* label,
			char* buf, size_t buf_size,
			ImGuiInputTextFlags flags,
			ImGuiInputTextCallback callback,
			void* user_data);
		bool (*InputTextMultiline)(const char* label, char* buf, size_t buf_size, const ImVec2 size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
		bool (*InputTextWithHint)(const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
		bool (*InputFloat)(const char* label, float* v,
			float step, float step_fast,
			const char* format, ImGuiInputTextFlags flags);
		bool (*InputFloat2)(const char* label, float v[2],
			const char* format, ImGuiInputTextFlags flags);
		bool (*InputFloat3)(const char* label, float v[3],
			const char* format, ImGuiInputTextFlags flags);
		bool (*InputFloat4)(const char* label, float v[4],
			const char* format, ImGuiInputTextFlags flags);
		bool (*InputInt)(const char* label, int* v,
			int step, int step_fast, ImGuiInputTextFlags flags);
		bool (*InputInt2)(const char* label, int v[2],
			ImGuiInputTextFlags flags);
		bool (*InputInt3)(const char* label, int v[3],
			ImGuiInputTextFlags flags);
		bool (*InputInt4)(const char* label, int v[4],
			ImGuiInputTextFlags flags);
		bool (*InputDouble)(const char* label, double* v,
			double step, double step_fast,
			const char* format, ImGuiInputTextFlags flags);
		bool (*InputScalar)(const char* label, ImGuiDataType data_type,
			void* p_data,
			const void* p_step, const void* p_step_fast,
			const char* format, ImGuiInputTextFlags flags);
		bool (*InputScalarN)(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_step, const void* p_step_fast, const char* format, ImGuiInputTextFlags flags);

		/* -------------------------------------------------------------------------
		* Widgets: Color Editor/Picker
		* ------------------------------------------------------------------------- */
		bool (*ColorEdit3)(const char* label, float col[3], ImGuiColorEditFlags flags);
		bool (*ColorEdit4)(const char* label, float col[4], ImGuiColorEditFlags flags);
		bool (*ColorPicker3)(const char* label, float col[3], ImGuiColorEditFlags flags);
		bool (*ColorPicker4)(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col);
		bool (*ColorButton)(const char* desc_id, const ImVec4 col, ImGuiColorEditFlags flags, const ImVec2 size);
		bool (*SetColorEditOptions)(ImGuiColorEditFlags flags);

		/* -------------------------------------------------------------------------
		* Tooltips
		* ------------------------------------------------------------------------- */
		bool (*BeginTooltip)(void);
		void (*EndTooltip)(void);
		void (*SetTooltip)(const char* fmt, ...);
		void (*SetTooltipV)(const char* fmt, va_list args);
		bool (*BeginItemTooltip)(void);
		void (*SetItemTooltip)(const char* fmt, ...);
		void (*SetItemTooltipV)(const char* fmt, va_list args);

		/* -------------------------------------------------------------------------
		* Tables
		* ------------------------------------------------------------------------- */
		bool (*BeginTable)(const char* str_id, int columns,
			ImGuiTableFlags flags,
			ImVec2 outer_size, float inner_width);
		void (*EndTable)(void);
		void (*TableNextRow)(ImGuiTableRowFlags row_flags, float min_row_height);
		bool (*TableNextColumn)(void);
		bool (*TableSetColumnIndex)(int column_n);

		/* -------------------------------------------------------------------------
		* Tables: Headers + Columns declaration
		* ------------------------------------------------------------------------- */
		void (*TableSetupColumn)(const char* label,
			ImGuiTableColumnFlags flags,
			float init_width_or_weight,
			ImGuiID user_id);
		void (*TableSetupScrollFreeze)(int cols, int rows);
		void (*TableHeader)(const char* label);
		void (*TableHeadersRow)(void);
		void (*TableAngledHeadersRow)(void);

		/* -------------------------------------------------------------------------
		* Tables: Sorting + Miscellaneous functions
		* ------------------------------------------------------------------------- */
		ImGuiTableSortSpecs* (*TableGetSortSpecs)(void);
		int (*TableGetColumnCount)(void);
		int (*TableGetColumnIndex)(void);
		int (*TableGetRowIndex)(void);
		const char* (*TableGetColumnName)(int column_n);
		ImGuiTableColumnFlags (*TableGetColumnFlags)(int column_n);
		void (*TableSetColumnEnabled)(int column_n, bool v);
		int (*TableGetHoveredColumn)();
		void (*TableSetBgColor)(int target, uint32_t color, int column_n);
		void (*Columns)(int count, const char* id, bool borders);
		void (*NextColumn)();
		int (*GetColumnIndex)();
		float (*GetColumnWidth)(int column_index);
		void (*SetColumnWidth)(int column_index, float width);
		float (*GetColumnOffset)(int column_index);
		void (*SetColumnOffset)(int column_index, float offset_x);
		int (*GetColumnsCount)();

		/* -------------------------------------------------------------------------
		* Content Avail
		* ------------------------------------------------------------------------- */
		ImVec2 (*GetContentRegionMax)(void);
		ImVec2 (*GetWindowContentRegionMin)(void);
		ImVec2 (*GetWindowContentRegionMax)(void);

		/* -------------------------------------------------------------------------
		* Navigation
		* ------------------------------------------------------------------------- */
		void (*SetNavCursorVisible)(bool visible);

		/* -------------------------------------------------------------------------
		* Overlapping
		* ------------------------------------------------------------------------- */
		void (*SetNextItemAllowOverlap)(void);

		/* -------------------------------------------------------------------------
		* Viewports
		* ------------------------------------------------------------------------- */
		ImGuiViewport* (*GetMainViewport)(void);

		/* -------------------------------------------------------------------------
		* DrawLists
		* ------------------------------------------------------------------------- */
		ImDrawList_Opaque* (*GetBackgroundDrawListDirect)(void);
		ImDrawList_Opaque* (*GetForegroundDrawListDirect)(void);

		/* -------------------------------------------------------------------------
		* MultiSelect
		* ------------------------------------------------------------------------- */
		ImGuiMultiSelectIO* (*BeginMultiSelect)(ImGuiMultiSelectFlags flags, int selection_size, int items_count);
		ImGuiMultiSelectIO* (*EndMultiSelect)(void);
		void (*SetNextItemSelectionUserData)(ImGuiSelectionUserData selection_user_data);
		bool (*IsItemToggledSelection)(void);

		/* -----------------------------------------------------------------
		 * Widgets: trees
		 * ----------------------------------------------------------------- */
		bool (*TreeNode)(const char* label);
		bool (*TreeNodeStr)(const char* str_id, const char* fmt, ...);
		bool (*TreeNodePtr)(const void* ptr_id, const char* fmt, ...);
		bool (*TreeNodeEx)(const char* label, ImGuiTreeNodeFlags flags);
		bool (*TreeNodeExStr)(const char* str_id,
			ImGuiTreeNodeFlags flags,
			const char* fmt, ...);
		bool (*TreeNodeExPtr)(const void* ptr_id,
			ImGuiTreeNodeFlags flags,
			const char* fmt, ...);
		void (*TreePush)(const char* str_id);
		void (*TreePushPtr)(const void* ptr_id);
		void (*TreePop)(void);
		float (*GetTreeNodeToLabelSpacing)(void);
		bool (*CollapsingHeader)(const char* label, ImGuiTreeNodeFlags flags);
		bool (*CollapsingHeaderBoolPtr)(const char* label, bool* p_visible,
			ImGuiTreeNodeFlags flags);
		void (*SetNextItemOpen)(bool is_open, ImGuiCond cond);
		bool (*TreeNodeVStr)(const char* str_id, const char* fmt, va_list args);
		bool (*TreeNodeVPtr)(const void* ptr_id, const char* fmt, va_list args);
		bool (*TreeNodeExVStr)(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
		bool (*TreeNodeExVPtr)(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
		void (*SetNextItemStorageID)(ImGuiID storage_id);
		bool (*TreeNodeGetOpen)(ImGuiID storage_id);

		/* -------------------------------------------------------------------------
		* Value
		* ------------------------------------------------------------------------- */
		void (*ValueBool)(const char* prefix, bool b);
		void (*ValueInt)(const char* prefix, int v);
		void (*ValueUint)(const char* prefix, unsigned int v);
		void (*ValueFloat)(const char* prefix, float v, const char* float_format);

		/* -------------------------------------------------------------------------
		* Inputs
		* ------------------------------------------------------------------------- */
		bool (*IsKeyDown)(ImGuiKey key);
		bool (*IsKeyPressed)(ImGuiKey key, bool repeat);
		bool (*IsKeyReleased)(ImGuiKey key);
		bool (*IsKeyChordPressed)(ImGuiKeyChord key_chord);
		int (*GetKeyPressedAmount)(ImGuiKey key, float repeat_delay, float rate);
		const char* (*GetKeyName)(ImGuiKey key);
		void (*SetNextFrameWantCaptureKeyboard)(bool want_capture_keyboard);

		/* -------------------------------------------------------------------------
		* Shortcut
		* ------------------------------------------------------------------------- */
		bool (*Shortcut)(ImGuiKeyChord key_chord, ImGuiInputFlags flags);
		void (*SetNextItemShortcut)(ImGuiKeyChord key_chord, ImGuiInputFlags flags);

		/* -------------------------------------------------------------------------
		* KeyOwnership
		* ------------------------------------------------------------------------- */
		bool (*SetItemKeyOwner)(ImGuiKey key);

		/* -------------------------------------------------------------------------
		* Mouse
		* ------------------------------------------------------------------------- */
		bool (*IsMouseDown)(ImGuiMouseButton button);
		bool (*IsMouseClicked)(ImGuiMouseButton button, bool repeat);
		bool (*IsMouseReleased)(ImGuiMouseButton button);
		bool (*IsMouseDoubleClicked)(ImGuiMouseButton button);
		bool (*IsMouseReleasedWithDelay)(ImGuiMouseButton button, float delay);
		int (*GetMouseClickedCount)(ImGuiMouseButton button);
		bool (*IsMouseHoveringRect)(const ImVec2 r_min, const ImVec2 r_max, bool clip);
		bool (*IsMousePosValid)(const ImVec2* mouse_pos);
		bool (*IsAnyMouseDown)(void);
		ImVec2 (*GetMousePos)(void);
		ImVec2 (*GetMousePosOnOpeningCurrentPopup)(void);
		bool (*IsMouseDragging)(ImGuiMouseButton button, float lock_threshold);
		ImVec2 (*GetMouseDragDelta)(ImGuiMouseButton button, float lock_threshold);
		void (*ResetMouseDragDelta)(ImGuiMouseButton button);
		ImGuiMouseCursor (*GetMouseCursor)(void);
		void (*SetMouseCursor)(ImGuiMouseCursor cursor_type);
		void (*SetNextFrameWantCaptureMouse)(bool want_capture_mouse);

		/* -------------------------------------------------------------------------
		* Clipboard
		* ------------------------------------------------------------------------- */
		const char* (*GetClipboardText)(void);
		void (*SetClipboardText)(const char* text);

		/* -----------------------------------------------------------------
		 * Widgets: selectables
		 * ----------------------------------------------------------------- */
		bool (*Selectable)(const char* label, bool selected,
			ImGuiSelectableFlags flags, ImVec2 size);
		bool (*SelectableBoolPtr)(const char* label, bool* p_selected,
			ImGuiSelectableFlags flags,
			ImVec2 size);

		/* -----------------------------------------------------------------
		 * Widgets: list box
		 * ----------------------------------------------------------------- */
		bool (*BeginListBox)(const char* label, ImVec2 size);
		void (*EndListBox)(void);
		bool (*ListBox)(const char* label, int* current_item,
			const char* const* items, int items_count,
			int height_in_items);
		bool (*ListBoxCallback)(const char* label, int* current_item,
			const char* (*getter)(void* data, int idx),
			void* data, int items_count,
			int height_in_items);

		/* -----------------------------------------------------------------
		 * Widgets: data plotting
		 * ----------------------------------------------------------------- */
		void (*PlotLines)(const char* label,
			const float* values, int values_count,
			int values_offset, const char* overlay_text,
			float scale_min, float scale_max,
			ImVec2 graph_size, int stride);
		void (*PlotLinesCallback)(const char* label,
			float (*values_getter)(void* data, int idx),
			void* data, int values_count, int values_offset,
			const char* overlay_text,
			float scale_min, float scale_max,
			ImVec2 graph_size);
		void (*PlotHistogram)(const char* label,
			const float* values, int values_count,
			int values_offset, const char* overlay_text,
			float scale_min, float scale_max,
			ImVec2 graph_size, int stride);
		void (*PlotHistogramCallback)(const char* label,
			float (*values_getter)(void* data, int idx),
			void* data, int values_count,
			int values_offset,
			const char* overlay_text,
			float scale_min, float scale_max,
			ImVec2 graph_size);

		/* -----------------------------------------------------------------
		 * Menus
		 * ----------------------------------------------------------------- */
		bool (*BeginMenuBar)(void);
		void (*EndMenuBar)(void);
		bool (*BeginMainMenuBar)(void);
		void (*EndMainMenuBar)(void);
		bool (*BeginMenu)(const char* label, bool enabled);
		void (*EndMenu)(void);
		bool (*MenuItem)(const char* label, const char* shortcut,
			bool selected, bool enabled);
		bool (*MenuItemBoolPtr)(const char* label, const char* shortcut,
			bool* p_selected, bool enabled);

		/* -----------------------------------------------------------------
		 * Popups / modals
		 * ----------------------------------------------------------------- */
		bool (*BeginPopup)(const char* str_id, ImGuiWindowFlags flags);
		bool (*BeginPopupModal)(const char* name, bool* p_open,
			ImGuiWindowFlags flags);
		void (*EndPopup)(void);
		void (*OpenPopup)(const char* str_id, ImGuiPopupFlags popup_flags);
		void (*OpenPopupOnItemClick)(const char* str_id,
			ImGuiPopupFlags popup_flags);
		void (*CloseCurrentPopup)(void);
		bool (*BeginPopupContextItem)(const char* str_id,
			ImGuiPopupFlags popup_flags);
		bool (*BeginPopupContextWindow)(const char* str_id,
			ImGuiPopupFlags popup_flags);
		bool (*BeginPopupContextVoid)(const char* str_id,
			ImGuiPopupFlags popup_flags);
		bool (*IsPopupOpen)(const char* str_id, ImGuiPopupFlags flags);
		void (*OpenPopupID)(ImGuiID id, ImGuiPopupFlags popup_flags);

		/* -----------------------------------------------------------------
		 * Tab bars / items
		 * ----------------------------------------------------------------- */
		bool (*BeginTabBar)(const char* str_id, ImGuiTabBarFlags flags);
		void (*EndTabBar)(void);
		bool (*BeginTabItem)(const char* label, bool* p_open,
			ImGuiTabItemFlags flags);
		void (*EndTabItem)(void);
		bool (*TabItemButton)(const char* label, ImGuiTabItemFlags flags);
		void (*SetTabItemClosed)(const char* tab_or_docked_window_label);
		ImGuiItemFlags (*GetItemFlags)(void);

		/* -----------------------------------------------------------------
		 * Docking (stub — host enables if ImGui built with docking branch)
		 * ----------------------------------------------------------------- */
		ImGuiID (*DockSpace)(ImGuiID dockspace_id, ImVec2 size,
			int flags, const void* window_class);
		void (*SetNextWindowDockID)(ImGuiID dock_id, ImGuiCond cond);
		ImGuiID (*GetWindowDockID)(void);
		bool (*IsWindowDocked)(void);

		/* -----------------------------------------------------------------
		 * Logging / capture
		 * ----------------------------------------------------------------- */
		void (*LogToTTY)(int auto_open_depth);
		void (*LogToFile)(int auto_open_depth, const char* filename);
		void (*LogToClipboard)(int auto_open_depth);
		void (*LogFinish)(void);
		void (*LogButtons)(void);
		void (*LogText)(const char* fmt, ...);
		void (*LogTextV)(const char* fmt, va_list args);

		/* -----------------------------------------------------------------
		 * Drag and drop
		 * ----------------------------------------------------------------- */
		bool (*BeginDragDropSource)(ImGuiDragDropFlags flags);
		bool (*SetDragDropPayload)(const char* type,
			const void* data, size_t sz,
			ImGuiCond cond);
		void (*EndDragDropSource)(void);
		bool (*BeginDragDropTarget)(void);
		/* Returned payload is a struct we copy into caller-provided buffer */
		bool (*AcceptDragDropPayload)(const char* type,
			ImGuiDragDropFlags flags,
			void* out_data, size_t* inout_data_size);
		void (*EndDragDropTarget)(void);
		const ImGuiPayload* (*GetDragDropPayload)(void);

		/* -----------------------------------------------------------------
		 * Disabling
		 * ----------------------------------------------------------------- */
		void (*BeginDisabled)(bool disabled);
		void (*EndDisabled)(void);

		/* -----------------------------------------------------------------
		 * Clipping
		 * ----------------------------------------------------------------- */
		void (*PushClipRect)(ImVec2 clip_rect_min,
			ImVec2 clip_rect_max,
			bool intersect_with_current_clip_rect);
		void (*PopClipRect)(void);

		/* -----------------------------------------------------------------
		 * Focus / activation
		 * ----------------------------------------------------------------- */
		void (*SetItemDefaultFocus)(void);
		void (*SetKeyboardFocusHere)(int offset);
		bool (*IsItemHovered)(ImGuiHoveredFlags flags);
		bool (*IsItemActive)(void);
		bool (*IsItemFocused)(void);
		bool (*IsItemClicked)(ImGuiMouseButton mouse_button);
		bool (*IsItemVisible)(void);
		bool (*IsItemEdited)(void);
		bool (*IsItemActivated)(void);
		bool (*IsItemDeactivated)(void);
		bool (*IsItemDeactivatedAfterEdit)(void);
		bool (*IsItemToggledOpen)(void);
		bool (*IsAnyItemHovered)(void);
		bool (*IsAnyItemActive)(void);
		bool (*IsAnyItemFocused)(void);
		ImGuiID (*GetItemID)(void);
		ImVec2 (*GetItemRectMin)(void);
		ImVec2 (*GetItemRectMax)(void);
		ImVec2 (*GetItemRectSize)(void);

		/* -----------------------------------------------------------------
		 * Miscellaneous
		 * ----------------------------------------------------------------- */
		bool (*IsRectVisible)(ImVec2 size);
		bool (*IsRectVisibleVec)(ImVec2 rect_min, ImVec2 rect_max);
		double (*GetTime)(void);
		int (*GetFrameCount)(void);
		const char* (*GetStyleColorName)(ImGuiCol idx);
		ImDrawListSharedData* (*GetDrawListSharedData)(void);
		void (*SetStateStorage)(ImGuiStorage* storage);
		ImGuiStorage* (*GetStateStorage)(void);

		/* -------------------------------------------------------------------------
		* Text Utilities
		* ------------------------------------------------------------------------- */
		ImVec2 (*CalcTextSize)(const char* text, const char* text_end,
			bool hide_text_after_double_hash,
			float wrap_width);

		/* -----------------------------------------------------------------
		 * Draw-list access (for custom rendering)
		 * Plugins receive an opaque ImDrawList_Opaque* and call the draw_list_* helpers.
		 * ----------------------------------------------------------------- */
		ImDrawList_Opaque* (*GetWindowDrawList)(void);

		// Clip rect / texture stack
		void (*DrawList_PushClipRect)(ImDrawList_Opaque* dl,
			ImVec2 clip_rect_min,
			ImVec2 clip_rect_max,
			bool intersect_with_current_clip_rect);
		void (*DrawList_PopClipRect)(ImDrawList_Opaque* dl);
		void (*DrawList_PushClipRectFullScreen)(ImDrawList_Opaque* dl);
		void (*DrawList_PushTexture)(ImDrawList_Opaque* dl, ImTextureRef tex_ref);
		void (*DrawList_PopTexture)(ImDrawList_Opaque* dl);
		ImVec2 (*DrawList_GetClipRectMin)(ImDrawList_Opaque* dl);
		ImVec2 (*DrawList_GetClipRectMax)(ImDrawList_Opaque* dl);

		// Lines
		void (*DrawList_AddLine)(ImDrawList_Opaque* dl,
			ImVec2 p1, ImVec2 p2,
			uint32_t col, float thickness);
		void (*DrawList_AddLineH)(ImDrawList_Opaque* dl,
			float min_x, float max_x, float y,
			uint32_t col, float thickness);
		void (*DrawList_AddLineV)(ImDrawList_Opaque* dl,
			float x, float min_y, float max_y,
			uint32_t col, float thickness);

		// Rectangles
		void (*DrawList_AddRect)(ImDrawList_Opaque* dl,
			ImVec2 p_min, ImVec2 p_max,
			uint32_t col, float rounding,
			int rounding_corners, float thickness);
		void (*DrawList_AddRectFilled)(ImDrawList_Opaque* dl,
			ImVec2 p_min, ImVec2 p_max,
			uint32_t col, float rounding,
			int rounding_corners);
		void (*DrawList_AddRectFilledMultiColor)(ImDrawList_Opaque* dl,
			ImVec2 p_min, ImVec2 p_max,
			uint32_t col_upr_left, uint32_t col_upr_right,
			uint32_t col_bot_right, uint32_t col_bot_left);

		// Circles
		void (*DrawList_AddCircle)(ImDrawList_Opaque* dl,
			ImVec2 center, float radius,
			uint32_t col, int num_segments,
			float thickness);
		void (*DrawList_AddCircleFilled)(ImDrawList_Opaque* dl,
			ImVec2 center, float radius,
			uint32_t col, int num_segments);

		// triangle
		void (*DrawList_AddTriangle)(ImDrawList_Opaque* dl,
			ImVec2 p1, ImVec2 p2,
			ImVec2 p3, uint32_t col,
			float thickness);
		void (*DrawList_AddTriangleFilled)(ImDrawList_Opaque* dl,
			ImVec2 p1, ImVec2 p2,
			ImVec2 p3, uint32_t col);

		// Quads
		void (*DrawList_AddQuad)(ImDrawList_Opaque* dl,
			ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4,
			uint32_t col, float thickness);
		void (*DrawList_AddQuadFilled)(ImDrawList_Opaque* dl,
			ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4,
			uint32_t col);

		// N-gons
		void (*DrawList_AddNgon)(ImDrawList_Opaque* dl,
			ImVec2 center, float radius,
			uint32_t col, int num_segments, float thickness);
		void (*DrawList_AddNgonFilled)(ImDrawList_Opaque* dl,
			ImVec2 center, float radius,
			uint32_t col, int num_segments);

		// Ellipses
		void (*DrawList_AddEllipse)(ImDrawList_Opaque* dl,
			ImVec2 center, ImVec2 radius,
			uint32_t col, float rot,
			int num_segments, float thickness);
		void (*DrawList_AddEllipseFilled)(ImDrawList_Opaque* dl,
			ImVec2 center, ImVec2 radius,
			uint32_t col, float rot, int num_segments);

		// Text
		void (*DrawList_AddText)(ImDrawList_Opaque* dl,
			ImVec2 pos, uint32_t col,
			const char* text_begin,
			const char* text_end);
		void (*DrawList_AddTextFont)(ImDrawList_Opaque* dl,
			ImFont* font, float font_size,
			ImVec2 pos, uint32_t col,
			const char* text_begin, const char* text_end,
			float wrap_width, const ImVec4* cpu_fine_clip_rect);

		// Bezier quadratic
		void (*DrawList_AddBezierQuadratic)(ImDrawList_Opaque* dl,
			ImVec2 p1, ImVec2 p2, ImVec2 p3,
			uint32_t col, float thickness, int num_segments);

		// Polylines / filled polygons
		void (*DrawList_AddPolyline)(ImDrawList_Opaque* dl,
			const ImVec2* points, int num_points,
			uint32_t col, float thickness, ImDrawFlags flags);
		void (*DrawList_AddConvexPolyFilled)(ImDrawList_Opaque* dl,
			const ImVec2* points, int num_points, uint32_t col);
		void (*DrawList_AddConcavePolyFilled)(ImDrawList_Opaque* dl,
			const ImVec2* points, int num_points, uint32_t col);

		// Image quads / rounded images
		void (*DrawList_AddImageQuad)(ImDrawList_Opaque* dl,
			ImTextureRef tex_ref,
			ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4,
			ImVec2 uv1, ImVec2 uv2, ImVec2 uv3, ImVec2 uv4,
			uint32_t col);
		void (*DrawList_AddImageRounded)(ImDrawList_Opaque* dl,
			ImTextureRef tex_ref,
			ImVec2 p_min, ImVec2 p_max,
			ImVec2 uv_min, ImVec2 uv_max,
			uint32_t col, float rounding, ImDrawFlags flags);
		void (*DrawList_AddImage)(ImDrawList_Opaque* dl,
			ImTextureRef tex_ref,
			ImVec2 p_min, ImVec2 p_max,
			ImVec2 uv_min, ImVec2 uv_max,
			uint32_t col);

		// Path API
		void (*DrawList_PathClear)(ImDrawList_Opaque* dl);
		void (*DrawList_PathLineTo)(ImDrawList_Opaque* dl, ImVec2 pos);
		void (*DrawList_PathLineToMergeDuplicate)(ImDrawList_Opaque* dl, ImVec2 pos);
		void (*DrawList_PathFillConvex)(ImDrawList_Opaque* dl, uint32_t col);
		void (*DrawList_PathFillConcave)(ImDrawList_Opaque* dl, uint32_t col);
		void (*DrawList_PathStroke)(ImDrawList_Opaque* dl,
			uint32_t col, float thickness, ImDrawFlags flags);
		void (*DrawList_PathArcTo)(ImDrawList_Opaque* dl,
			ImVec2 center, float radius,
			float a_min, float a_max, int num_segments);
		void (*DrawList_PathArcToFast)(ImDrawList_Opaque* dl,
			ImVec2 center, float radius,
			int a_min_of_12, int a_max_of_12);
		void (*DrawList_PathEllipticalArcTo)(ImDrawList_Opaque* dl,
			ImVec2 center, ImVec2 radius, float rot,
			float a_min, float a_max, int num_segments);
		void (*DrawList_PathBezierCubicCurveTo)(ImDrawList_Opaque* dl,
			ImVec2 p2, ImVec2 p3, ImVec2 p4, int num_segments);
		void (*DrawList_PathBezierQuadraticCurveTo)(ImDrawList_Opaque* dl,
			ImVec2 p2, ImVec2 p3, int num_segments);
		void (*DrawList_PathRect)(ImDrawList_Opaque* dl,
			ImVec2 rect_min, ImVec2 rect_max,
			float rounding, ImDrawFlags flags);

		// Channels
		void (*DrawList_ChannelsSplit)(ImDrawList_Opaque* dl, int count);
		void (*DrawList_ChannelsMerge)(ImDrawList_Opaque* dl);
		void (*DrawList_ChannelsSetCurrent)(ImDrawList_Opaque* dl, int n);

		// Primitive allocation
		void (*DrawList_PrimReserve)(ImDrawList_Opaque* dl,
			int idx_count, int vtx_count);
		void (*DrawList_PrimUnreserve)(ImDrawList_Opaque* dl,
			int idx_count, int vtx_count);
		void (*DrawList_PrimRect)(ImDrawList_Opaque* dl,
			ImVec2 a, ImVec2 b, uint32_t col);
		void (*DrawList_PrimRectUV)(ImDrawList_Opaque* dl,
			ImVec2 a, ImVec2 b,
			ImVec2 uv_a, ImVec2 uv_b, uint32_t col);
		void (*DrawList_PrimQuadUV)(ImDrawList_Opaque* dl,
			ImVec2 a, ImVec2 b, ImVec2 c, ImVec2 d,
			ImVec2 uv_a, ImVec2 uv_b, ImVec2 uv_c, ImVec2 uv_d,
			uint32_t col);
		void (*DrawList_PrimWriteVtx)(ImDrawList_Opaque* dl,
			ImVec2 pos, ImVec2 uv, uint32_t col);
		void (*DrawList_PrimWriteIdx)(ImDrawList_Opaque* dl, ImDrawIdx idx);
		void (*DrawList_PrimVtx)(ImDrawList_Opaque* dl,
			ImVec2 pos, ImVec2 uv, uint32_t col);

		// Callbacks / misc
		void (*DrawList_AddCallback)(ImDrawList_Opaque* dl,
			ImDrawCallback callback, void* userdata, size_t userdata_size);
		void (*DrawList_AddDrawCmd)(ImDrawList_Opaque* dl);
		void (*DrawList_AddBezierCubic)(ImDrawList_Opaque* dl,
			ImVec2 p1, ImVec2 p2,
			ImVec2 p3, ImVec2 p4,
			uint32_t col, float thickness,
			int num_segments);

		/* -------------------------------------------------------------------------
		* Color Utilities
		* ------------------------------------------------------------------------- */
		ImU32 (*ColorConvertFloat4ToU32)(ImVec4 in);
		ImVec4 (*ColorConvertU32ToFloat4)(ImU32 in);
		void (*ColorConvertRGBtoHSV)(float r, float g, float b,
			float* out_h, float* out_s,
			float* out_v);
		void (*ColorConvertHSVtoRGB)(float h, float s, float v,
			float* out_r, float* out_g,
			float* out_b);

		/* -------------------------------------------------------------------------
		* imgui_internal.h
		* ------------------------------------------------------------------------- */
		ImGuiWindow* (*GetCurrentWindow)();
		ImGuiWindow* (*FindWindowByID)(ImGuiID id);
		ImGuiWindow* (*FindWindowByName)(const char* name);
		void (*UpdateWindowParentAndRootLinks)(ImGuiWindow* window, ImGuiWindowFlags flags, ImGuiWindow* parent_window);
		void (*UpdateWindowSkipRefresh)(ImGuiWindow* window);
		ImVec2 (*CalcWindowNextAutoFitSize)(ImGuiWindow* window);
		bool (*IsWindowChildOf)(ImGuiWindow* window, ImGuiWindow* potential_parent, bool popup_hierarchy);
		bool (*IsWindowInBeginStack)(ImGuiWindow* window);
		bool (*IsWindowWithinBeginStackOf)(ImGuiWindow* window, ImGuiWindow* potential_parent);
		bool (*IsWindowAbove)(ImGuiWindow* potential_above, ImGuiWindow* potential_below);
		bool (*IsWindowNavFocusable)(ImGuiWindow* window);
		void (*SetWindowPosGlob)(ImGuiWindow* window, const ImVec2 pos, ImGuiCond cond);
		void (*SetWindowSizeGlob)(ImGuiWindow* window, const ImVec2 size, ImGuiCond cond);
		void (*SetWindowCollapsedGlob)(ImGuiWindow* window, bool collapsed, ImGuiCond cond);
		void (*SetWindowHitTestHole)(ImGuiWindow* window, const ImVec2 pos, const ImVec2 size);
		void (*SetWindowHiddenAndSkipItemsForCurrentFrame)(ImGuiWindow* window);
		void (*FocusWindow)(ImGuiWindow* window, ImGuiFocusRequestFlags flags);
		void (*FocusTopMostWindowUnderOne)(ImGuiWindow* under_this_window, ImGuiWindow* ignore_window, ImGuiViewport* filter_viewport, ImGuiFocusRequestFlags flags);
		void (*BringWindowToFocusFront)(ImGuiWindow* window);
		void (*BringWindowToDisplayFront)(ImGuiWindow* window);
		void (*BringWindowToDisplayBack)(ImGuiWindow* window);
		void (*BringWindowToDisplayBehind)(ImGuiWindow* window, ImGuiWindow* above_window);
		int (*FindWindowDisplayIndex)(ImGuiWindow* window);
		ImGuiWindow* (*FindBottomMostVisibleWindowWithinBeginStack)(ImGuiWindow* window);
		void (*SetNextWindowRefreshPolicy)(ImGuiWindowRefreshFlags flags);
		void (*RegisterUserTexture)(ImTextureData* tex);
		void (*UnregisterUserTexture)(ImTextureData* tex);
		void (*RegisterFontAtlas)(ImFontAtlas* atlas);
		void (*UnregisterFontAtlas)(ImFontAtlas* atlas);
		void (*SetCurrentFont)(ImFont* font, float font_size_before_scaling, float font_size_after_scaling);
		void (*UpdateCurrentFontSize)(float restore_font_size_after_scaling);
		void (*SetFontRasterizerDensity)(float rasterizer_density);
		ImFont* (*GetDefaultFont)();
		void (*PushPasswordFont)();
		void (*PopPasswordFont)();
		ImDrawList_Opaque* (*GetBackgroundDrawList)(ImGuiViewport* viewport);
		ImDrawList_Opaque* (*GetForegroundDrawList)(ImGuiViewport* viewport);
		void (*AddDrawListToDrawDataEx)(ImDrawData* draw_data, ImVector<ImDrawList_Opaque*>* out_list, ImDrawList_Opaque* draw_list);
		void (*Initialize)();
		void (*Shutdown)();
		void (*SetContextName)(ImGuiContext* ctx, const char* name);
		ImGuiID (*AddContextHook)(ImGuiContext* ctx, const ImGuiContextHook* hook);
		void (*RemoveContextHook)(ImGuiContext* ctx, ImGuiID hook_to_remove);
		void (*CallContextHooks)(ImGuiContext* ctx, ImGuiContextHookType type);
		void (*UpdateInputEvents)(bool trickle_fast_inputs);
		void (*UpdateHoveredWindowAndCaptureFlags)(const ImVec2 mouse_pos);
		void (*FindHoveredWindowEx)(const ImVec2 pos, bool find_first_and_in_any_viewport, ImGuiWindow** out_hovered_window, ImGuiWindow** out_hovered_window_under_moving_window);
		void (*StartMouseMovingWindow)(ImGuiWindow* window);
		void (*StopMouseMovingWindow)();
		void (*UpdateMouseMovingWindowNewFrame)();
		void (*UpdateMouseMovingWindowEndFrame)();
		void (*ScaleWindowsInViewport)(ImGuiViewportP* viewport, float scale);
		void (*SetWindowViewport)(ImGuiWindow* window, ImGuiViewportP* viewport);
		void (*MarkIniSettingsDirty)();
		void (*MarkIniSettingsDirtyWindow)(ImGuiWindow* window);
		void (*ClearIniSettings)();
		void (*AddSettingsHandler)(const ImGuiSettingsHandler* handler);
		void (*RemoveSettingsHandler)(const char* type_name);
		ImGuiSettingsHandler* (*FindSettingsHandler)(const char* type_name);
		ImGuiWindowSettings* (*CreateNewWindowSettings)(const char* name);
		ImGuiWindowSettings* (*FindWindowSettingsByID)(ImGuiID id);
		ImGuiWindowSettings* (*FindWindowSettingsByWindow)(ImGuiWindow* window);
		void (*ClearWindowSettings)(const char* name);
		void (*LocalizeRegisterEntries)(const ImGuiLocEntry* entries, int count);
		void (*SetScrollXGlob)(ImGuiWindow* window, float scroll_x);
		void (*SetScrollYGlob)(ImGuiWindow* window, float scroll_y);
		void (*SetScrollFromPosXGlob)(ImGuiWindow* window, float local_x, float center_x_ratio);
		void (*SetScrollFromPosYGlob)(ImGuiWindow* window, float local_y, float center_y_ratio);
		void (*ScrollToItem)(ImGuiScrollFlags flags);
		void (*ScrollToRect)(ImGuiWindow* window, const ImRect rect, ImGuiScrollFlags flags);
		ImVec2 (*ScrollToRectEx)(ImGuiWindow* window, const ImRect rect, ImGuiScrollFlags flags);
		void (*SetActiveID)(ImGuiID id, ImGuiWindow* window);
		void (*SetFocusID)(ImGuiID id, ImGuiWindow* window);
		void (*ClearActiveID)();
		ImGuiID (*GetHoveredID)();
		void (*SetHoveredID)(ImGuiID id);
		void (*KeepAliveID)(ImGuiID id);
		void (*MarkItemEdited)(ImGuiID id);
		void (*PushOverrideID)(ImGuiID id);
		ImGuiID (*GetIDWithSeed)(const char* str_id_begin, const char* str_id_end, ImGuiID seed);
		ImGuiID (*GetIDWithSeedInt)(int n, ImGuiID seed);
		void (*ItemSize)(const ImVec2 size, float text_baseline_y);
		void (*ItemSizeImRect)(const ImRect size, float text_baseline_y);
		bool (*ItemAdd)(const ImRect bb, ImGuiID id, const ImRect* nav_bb, ImGuiItemFlags extra_flags);
		bool (*ItemHoverable)(const ImRect bb, ImGuiID id, ImGuiItemFlags item_flags);
		bool (*IsWindowContentHoverable)(ImGuiWindow* window, ImGuiHoveredFlags flags);
		bool (*IsClippedEx)(const ImRect bb, ImGuiID id);
		void (*SetLastItemData)(ImGuiID item_id, ImGuiItemFlags item_flags, ImGuiItemStatusFlags status_flags, const ImRect item_rect);
		ImVec2 (*CalcItemSize)(ImVec2 size, float default_w, float default_h);
		float (*CalcWrapWidthForPos)(const ImVec2 pos, float wrap_pos_x);
		void (*PushMultiItemsWidths)(int components, float width_full);
		void (*ShrinkWidths)(ImGuiShrinkWidthItem* items, int count, float width_excess, float width_min);
		void (*CalcClipRectVisibleItemsY)(const ImRect clip_rect, const ImVec2 pos, float items_height, int* out_visible_start, int* out_visible_end);
		const ImGuiStyleVarInfo* (*GetStyleVarInfo)(ImGuiStyleVar idx);
		void (*BeginDisabledOverrideReenable)();
		void (*EndDisabledOverrideReenable)();
		void (*LogBegin)(ImGuiLogFlags flags, int auto_open_depth);
		void (*LogToBuffer)(int auto_open_depth);
		void (*LogRenderedText)(const ImVec2* ref_pos, const char* text, const char* text_end);
		void (*LogSetNextTextDecoration)(const char* prefix, const char* suffix);
		bool (*BeginChildEx)(const char* name, ImGuiID id, const ImVec2 size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags);
		ImGuiWindow* (*FindFrontMostVisibleChildWindow)(ImGuiWindow* window);
		bool (*BeginPopupEx)(ImGuiID id, ImGuiWindowFlags extra_window_flags);
		bool (*BeginPopupMenuEx)(ImGuiID id, const char* label, ImGuiWindowFlags extra_window_flags);
		void (*OpenPopupEx)(ImGuiID id, ImGuiPopupFlags popup_flags);
		void (*ClosePopupToLevel)(int remaining, bool restore_focus_to_window_under_popup);
		void (*ClosePopupsOverWindow)(ImGuiWindow* ref_window, bool restore_focus_to_window_under_popup);
		void (*ClosePopupsExceptModals)();
		bool (*IsPopupOpenID)(ImGuiID id, ImGuiPopupFlags popup_flags);
		ImRect (*GetPopupAllowedExtentRect)(ImGuiWindow* window);
		ImGuiWindow* (*GetTopMostPopupModal)();
		ImGuiWindow* (*GetTopMostAndVisiblePopupModal)();
		ImGuiWindow* (*FindBlockingModal)(ImGuiWindow* window);
		ImVec2 (*FindBestWindowPosForPopup)(ImGuiWindow* window);
		ImVec2 (*FindBestWindowPosForPopupEx)(const ImVec2 ref_pos, const ImVec2 size, ImGuiDir* last_dir, const ImRect r_outer, const ImRect r_avoid, ImGuiPopupPositionPolicy policy);
		ImGuiMouseButton (*GetMouseButtonFromPopupFlags)(ImGuiPopupFlags flags);
		bool (*IsPopupOpenRequestForItem)(ImGuiPopupFlags flags, ImGuiID id);
		bool (*IsPopupOpenRequestForWindow)(ImGuiPopupFlags flags);
		bool (*BeginTooltipEx)(ImGuiTooltipFlags tooltip_flags, ImGuiWindowFlags extra_window_flags);
		bool (*BeginTooltipHidden)();
		bool (*BeginViewportSideBar)(const char* name, ImGuiViewport* viewport, ImGuiDir dir, float size, ImGuiWindowFlags window_flags);
		bool (*BeginMenuEx)(const char* label, const char* icon, bool enabled);
		bool (*MenuItemEx)(const char* label, const char* icon, const char* shortcut, bool selected, bool enabled);
		bool (*BeginComboPopup)(ImGuiID popup_id, const ImRect bb, ImGuiComboFlags flags);
		bool (*BeginComboPreview)();
		void (*EndComboPreview)();
		void (*NavInitWindow)(ImGuiWindow* window, bool force_reinit);
		void (*NavInitRequestApplyResult)();
		bool (*NavMoveRequestButNoResultYet)();
		void (*NavMoveRequestSubmit)(ImGuiDir move_dir, ImGuiDir clip_dir, ImGuiNavMoveFlags move_flags, ImGuiScrollFlags scroll_flags);
		void (*NavMoveRequestForward)(ImGuiDir move_dir, ImGuiDir clip_dir, ImGuiNavMoveFlags move_flags, ImGuiScrollFlags scroll_flags);
		void (*NavMoveRequestResolveWithLastItem)(ImGuiNavItemData* result);
		void (*NavMoveRequestResolveWithPastTreeNode)(ImGuiNavItemData* result, const ImGuiTreeNodeStackData* tree_node_data);
		void (*NavMoveRequestCancel)();
		void (*NavMoveRequestApplyResult)();
		void (*NavMoveRequestTryWrapping)(ImGuiWindow* window, ImGuiNavMoveFlags move_flags);
		void (*NavHighlightActivated)(ImGuiID id);
		void (*NavClearPreferredPosForAxis)(ImGuiAxis axis);
		void (*SetNavCursorVisibleAfterMove)();
		void (*NavUpdateCurrentWindowIsScrollPushableX)();
		void (*SetNavWindow)(ImGuiWindow* window);
		void (*SetNavID)(ImGuiID id, ImGuiNavLayer nav_layer, ImGuiID focus_scope_id, const ImRect rect_rel);
		void (*SetNavFocusScope)(ImGuiID focus_scope_id);
		void (*FocusItem)();
		void (*ActivateItemByID)(ImGuiID id);
		ImGuiKeyData* (*GetKeyData)(ImGuiContext* ctx, ImGuiKey key);
		const char* (*GetKeyChordName)(ImGuiKeyChord key_chord);
		bool (*IsMouseDragPastThreshold)(ImGuiMouseButton button, float lock_threshold);
		ImVec2 (*GetKeyMagnitude2d)(ImGuiKey key_left, ImGuiKey key_right, ImGuiKey key_up, ImGuiKey key_down);
		float (*GetNavTweakPressedAmount)(ImGuiAxis axis);
		int (*CalcTypematicRepeatAmount)(float t0, float t1, float repeat_delay, float repeat_rate);
		void (*GetTypematicRepeatRate)(ImGuiInputFlags flags, float* repeat_delay, float* repeat_rate);
		void (*TeleportMousePos)(const ImVec2 pos);
		void (*SetActiveIdUsingAllKeyboardKeys)();
		ImGuiID (*GetKeyOwner)(ImGuiKey key);
		void (*SetKeyOwner)(ImGuiKey key, ImGuiID owner_id, ImGuiInputFlags flags);
		void (*SetKeyOwnersForKeyChord)(ImGuiKeyChord key, ImGuiID owner_id, ImGuiInputFlags flags);
		bool (*SetItemKeyOwnerOwner)(ImGuiKey key, ImGuiInputFlags flags);
		bool (*TestKeyOwner)(ImGuiKey key, ImGuiID owner_id);
		bool (*IsKeyDownOwner)(ImGuiKey key, ImGuiID owner_id);
		bool (*IsKeyPressedOwner)(ImGuiKey key, ImGuiInputFlags flags, ImGuiID owner_id);
		bool (*IsKeyReleasedOwner)(ImGuiKey key, ImGuiID owner_id);
		bool (*IsKeyChordPressedOwner)(ImGuiKeyChord key_chord, ImGuiInputFlags flags, ImGuiID owner_id);
		bool (*IsMouseDownOwner)(ImGuiMouseButton button, ImGuiID owner_id);
		bool (*IsMouseClickedOwner)(ImGuiMouseButton button, ImGuiInputFlags flags, ImGuiID owner_id);
		bool (*IsMouseReleasedOwner)(ImGuiMouseButton button, ImGuiID owner_id);
		bool (*IsMouseDoubleClickedOwner)(ImGuiMouseButton button, ImGuiID owner_id);
		bool (*ShortcutOwner)(ImGuiKeyChord key_chord, ImGuiInputFlags flags, ImGuiID owner_id);
		bool (*SetShortcutRoutingOwner)(ImGuiKeyChord key_chord, ImGuiInputFlags flags, ImGuiID owner_id);
		bool (*TestShortcutRoutingOwner)(ImGuiKeyChord key_chord, ImGuiID owner_id);
		ImGuiKeyRoutingData* (*GetShortcutRoutingData)(ImGuiKeyChord key_chord);
		void (*PushFocusScope)(ImGuiID id);
		void (*PopFocusScope)();
		bool (*IsInNavFocusRoute)(ImGuiID focus_scope_id);
		bool (*IsDragDropActive)();
		bool (*BeginDragDropTargetCustom)(const ImRect bb, ImGuiID id);
		bool (*BeginDragDropTargetViewport)(ImGuiViewport* viewport, const ImRect* p_bb);
		void (*ClearDragDrop)();
		bool (*IsDragDropPayloadBeingAccepted)();
		void (*RenderDragDropTargetRectForItem)(const ImRect bb);
		void (*RenderDragDropTargetRectEx)(ImDrawList_Opaque* draw_list, const ImRect bb, float rounding);
		ImGuiTypingSelectRequest* (*GetTypingSelectRequest)(ImGuiTypingSelectFlags flags);
		int (*TypingSelectFindMatch)(ImGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data, int nav_item_idx);
		int (*TypingSelectFindNextSingleCharMatch)(ImGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data, int nav_item_idx);
		int (*TypingSelectFindBestLeadingMatch)(ImGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data);
		bool (*BeginBoxSelect)(const ImRect scope_rect, ImGuiWindow* window, ImGuiID box_select_id, ImGuiMultiSelectFlags ms_flags);
		void (*EndBoxSelect)(const ImRect scope_rect, ImGuiMultiSelectFlags ms_flags);
		void (*MultiSelectItemHeader)(ImGuiID id, bool* p_selected, ImGuiButtonFlags* p_button_flags);
		void (*MultiSelectItemFooter)(ImGuiID id, bool* p_selected, bool* p_pressed);
		void (*MultiSelectAddSetAll)(ImGuiMultiSelectTempData* ms, bool selected);
		void (*MultiSelectAddSetRange)(ImGuiMultiSelectTempData* ms, bool selected, int range_dir, ImGuiSelectionUserData first_item, ImGuiSelectionUserData last_item);
		void (*SetWindowClipRectBeforeSetChannel)(ImGuiWindow* window, const ImRect clip_rect);
		void (*BeginColumns)(const char* str_id, int count, ImGuiOldColumnFlags flags);
		void (*EndColumns)();
		void (*PushColumnClipRect)(int column_index);
		void (*PushColumnsBackground)();
		void (*PopColumnsBackground)();
		ImGuiID (*GetColumnsID)(const char* str_id, int count);
		ImGuiOldColumns* (*FindOrCreateColumns)(ImGuiWindow* window, ImGuiID id);
		float (*GetColumnOffsetFromNorm)(const ImGuiOldColumns* columns, float offset_norm);
		float (*GetColumnNormFromOffset)(const ImGuiOldColumns* columns, float offset);
		void (*TableOpenContextMenu)(int column_n);
		void (*TableSetColumnWidth)(int column_n, float width);
		void (*TableSetColumnSortDirection)(int column_n, ImGuiSortDirection sort_direction, bool append_to_sort_specs);
		int (*TableGetHoveredRow)();
		float (*TableGetHeaderRowHeight)();
		float (*TableGetHeaderAngledMaxLabelWidth)();
		void (*TablePushBackgroundChannel)();
		void (*TablePopBackgroundChannel)();
		void (*TablePushColumnChannel)(int column_n);
		void (*TablePopColumnChannel)();
		void (*TableAngledHeadersRowEx)(ImGuiID row_id, float angle, float max_label_width, const ImGuiTableHeaderData* data, int data_count);
		ImGuiTable* (*TableFindByID)(ImGuiID id);
		bool (*BeginTableEx)(const char* name, ImGuiID id, int columns_count, ImGuiTableFlags flags, const ImVec2 outer_size, float inner_width);
		void (*TableBeginInitMemory)(ImGuiTable* table, int columns_count);
		void (*TableBeginApplyRequests)(ImGuiTable* table);
		void (*TableSetupDrawChannels)(ImGuiTable* table);
		void (*TableUpdateLayout)(ImGuiTable* table);
		void (*TableUpdateBorders)(ImGuiTable* table);
		void (*TableUpdateColumnsWeightFromWidth)(ImGuiTable* table);
		void (*TableApplyExternalUnclipRect)(ImGuiTable* table, ImRect rect);
		void (*TableDrawBorders)(ImGuiTable* table);
		void (*TableDrawDefaultContextMenu)(ImGuiTable* table, ImGuiTableFlags flags_for_section_to_display);
		bool (*TableBeginContextMenuPopup)(ImGuiTable* table);
		void (*TableMergeDrawChannels)(ImGuiTable* table);
		void (*TableFixDisplayOrder)(ImGuiTable* table);
		void (*TableSortSpecsSanitize)(ImGuiTable* table);
		void (*TableSortSpecsBuild)(ImGuiTable* table);
		ImGuiSortDirection (*TableGetColumnNextSortDirection)(ImGuiTableColumn* column);
		void (*TableFixColumnSortDirection)(ImGuiTable* table, ImGuiTableColumn* column);
		float (*TableGetColumnWidthAuto)(ImGuiTable* table, ImGuiTableColumn* column);
		void (*TableBeginRow)(ImGuiTable* table);
		void (*TableEndRow)(ImGuiTable* table);
		void (*TableBeginCell)(ImGuiTable* table, int column_n);
		void (*TableEndCell)(ImGuiTable* table);
		ImRect (*TableGetCellBgRect)(const ImGuiTable* table, int column_n);
		const char* (*TableGetColumnNameGlob)(const ImGuiTable* table, int column_n);
		ImGuiID (*TableGetColumnResizeID)(ImGuiTable* table, int column_n, int instance_no);
		float (*TableCalcMaxColumnWidth)(const ImGuiTable* table, int column_n);
		void (*TableSetColumnWidthAutoSingle)(ImGuiTable* table, int column_n);
		void (*TableSetColumnWidthAutoAll)(ImGuiTable* table);
		void (*TableSetColumnDisplayOrder)(ImGuiTable* table, int column_n, int dst_order);
		void (*TableQueueSetColumnDisplayOrder)(ImGuiTable* table, int column_n, int dst_order);
		void (*TableRemove)(ImGuiTable* table);
		void (*TableGcCompactTransientBuffers)(ImGuiTable* table);
		void (*TableGcCompactTransientBuffersTemp)(ImGuiTableTempData* table);
		void (*TableGcCompactSettings)();
		void (*TableLoadSettings)(ImGuiTable* table);
		void (*TableSaveSettings)(ImGuiTable* table);
		void (*TableResetSettings)(ImGuiTable* table);
		ImGuiTableSettings* (*TableGetBoundSettings)(ImGuiTable* table);
		void (*TableSettingsAddSettingsHandler)();
		ImGuiTableSettings* (*TableSettingsCreate)(ImGuiID id, int columns_count);
		ImGuiTableSettings* (*TableSettingsFindByID)(ImGuiID id);
		ImGuiTabBar* (*TabBarFindByID)(ImGuiID id);
		void (*TabBarRemove)(ImGuiTabBar* tab_bar);
		bool (*BeginTabBarEx)(ImGuiTabBar* tab_bar, const ImRect bb, ImGuiTabBarFlags flags);
		ImGuiTabItem* (*TabBarFindTabByID)(ImGuiTabBar* tab_bar, ImGuiID tab_id);
		ImGuiTabItem* (*TabBarFindTabByOrder)(ImGuiTabBar* tab_bar, int order);
		ImGuiTabItem* (*TabBarGetCurrentTab)(ImGuiTabBar* tab_bar);
		const char* (*TabBarGetTabName)(ImGuiTabBar* tab_bar, ImGuiTabItem* tab);
		void (*TabBarRemoveTab)(ImGuiTabBar* tab_bar, ImGuiID tab_id);
		void (*TabBarCloseTab)(ImGuiTabBar* tab_bar, ImGuiTabItem* tab);
		void (*TabBarQueueFocus)(ImGuiTabBar* tab_bar, ImGuiTabItem* tab);
		void (*TabBarQueueFocusByName)(ImGuiTabBar* tab_bar, const char* tab_name);
		void (*TabBarQueueReorder)(ImGuiTabBar* tab_bar, ImGuiTabItem* tab, int offset);
		void (*TabBarQueueReorderFromMousePos)(ImGuiTabBar* tab_bar, ImGuiTabItem* tab, ImVec2 mouse_pos);
		bool (*TabBarProcessReorder)(ImGuiTabBar* tab_bar);
		bool (*TabItemEx)(ImGuiTabBar* tab_bar, const char* label, bool* p_open, ImGuiTabItemFlags flags, ImGuiWindow* docked_window);
		void (*TabItemSpacing)(const char* str_id, ImGuiTabItemFlags flags, float width);
		ImVec2 (*TabItemCalcSize)(const char* label, bool has_close_button_or_unsaved_marker);
		ImVec2 (*TabItemCalcSizeWindow)(ImGuiWindow* window);
		void (*TabItemBackground)(ImDrawList_Opaque* draw_list, const ImRect bb, ImGuiTabItemFlags flags, ImU32 col);
		void (*TabItemLabelAndCloseButton)(ImDrawList_Opaque* draw_list, const ImRect bb, ImGuiTabItemFlags flags, ImVec2 frame_padding, const char* label, ImGuiID tab_id, ImGuiID close_button_id, bool is_contents_visible, bool* out_just_closed, bool* out_text_clipped);
		void (*RenderText)(ImVec2 pos, const char* text, const char* text_end, bool hide_text_after_hash);
		void (*RenderTextWrapped)(ImVec2 pos, const char* text, const char* text_end, float wrap_width);
		void (*RenderTextClipped)(const ImVec2 pos_min, const ImVec2 pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2 align, const ImRect* clip_rect);
		void (*RenderTextClippedEx)(ImDrawList_Opaque* draw_list, const ImVec2 pos_min, const ImVec2 pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2 align, const ImRect* clip_rect);
		void (*RenderTextEllipsis)(ImDrawList_Opaque* draw_list, const ImVec2 pos_min, const ImVec2 pos_max, float ellipsis_max_x, const char* text, const char* text_end, const ImVec2* text_size_if_known);
		void (*RenderFrame)(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool borders, float rounding);
		void (*RenderFrameBorder)(ImVec2 p_min, ImVec2 p_max, float rounding);
		void (*RenderColorComponentMarker)(const ImRect bb, ImU32 col, float rounding);
		void (*RenderColorRectWithAlphaCheckerboard)(ImDrawList_Opaque* draw_list, ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float grid_step, ImVec2 grid_off, float rounding, ImDrawFlags flags);
		void (*RenderNavCursor)(const ImRect bb, ImGuiID id, ImGuiNavRenderCursorFlags flags);
		const char* (*FindRenderedTextEnd)(const char* text, const char* text_end);
		void (*RenderMouseCursor)(ImVec2 pos, float scale, ImGuiMouseCursor mouse_cursor, ImU32 col_fill, ImU32 col_border, ImU32 col_shadow);
		void (*RenderArrow)(ImDrawList_Opaque* draw_list, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale);
		void (*RenderBullet)(ImDrawList_Opaque* draw_list, ImVec2 pos, ImU32 col);
		void (*RenderCheckMark)(ImDrawList_Opaque* draw_list, ImVec2 pos, ImU32 col, float sz);
		void (*RenderArrowPointingAt)(ImDrawList_Opaque* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col);
		void (*RenderRectFilledInRangeH)(ImDrawList_Opaque* draw_list, const ImRect rect, ImU32 col, float fill_x0, float fill_x1, float rounding);
		void (*RenderRectFilledWithHole)(ImDrawList_Opaque* draw_list, const ImRect outer, const ImRect inner, ImU32 col, float rounding);
		ImDrawFlags (*CalcRoundingFlagsForRectInRect)(const ImRect r_in, const ImRect r_outer, float threshold);
		void (*TextEx)(const char* text, const char* text_end, ImGuiTextFlags flags);
		bool (*ButtonEx)(const char* label, const ImVec2 size_arg, ImGuiButtonFlags flags);
		bool (*ArrowButtonEx)(const char* str_id, ImGuiDir dir, ImVec2 size_arg, ImGuiButtonFlags flags);
		bool (*ImageButtonEx)(ImGuiID id, ImTextureRef tex_ref, const ImVec2 image_size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 bg_col, const ImVec4 tint_col, ImGuiButtonFlags flags);
		void (*SeparatorEx)(ImGuiSeparatorFlags flags, float thickness);
		void (*SeparatorTextEx)(ImGuiID id, const char* label, const char* label_end, float extra_width);
		bool (*CheckboxFlagsS64)(const char* label, ImS64* flags, ImS64 flags_value);
		bool (*CheckboxFlagsU64)(const char* label, ImU64* flags, ImU64 flags_value);
		bool (*CloseButton)(ImGuiID id, const ImVec2 pos);
		bool (*CollapseButton)(ImGuiID id, const ImVec2 pos);
		void (*Scrollbar)(ImGuiAxis axis);
		bool (*ScrollbarEx)(const ImRect bb, ImGuiID id, ImGuiAxis axis, ImS64* p_scroll_v, ImS64 avail_v, ImS64 contents_v, ImDrawFlags draw_rounding_flags);
		ImRect (*GetWindowScrollbarRect)(ImGuiWindow* window, ImGuiAxis axis);
		ImGuiID (*GetWindowScrollbarID)(ImGuiWindow* window, ImGuiAxis axis);
		ImGuiID (*GetWindowResizeCornerID)(ImGuiWindow* window, int n);
		ImGuiID (*GetWindowResizeBorderID)(ImGuiWindow* window, ImGuiDir dir);
		void (*ExtendHitBoxWhenNearViewportEdge)(ImGuiWindow* window, ImRect* bb, float threshold, ImGuiAxis axis);
		bool (*ButtonBehavior)(const ImRect bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags);
		bool (*DragBehavior)(ImGuiID id, ImGuiDataType data_type, void* p_v, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
		bool (*SliderBehavior)(const ImRect bb, ImGuiID id, ImGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb);
		bool (*SplitterBehavior)(const ImRect bb, ImGuiID id, ImGuiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend, float hover_visibility_delay, ImU32 bg_col);
		bool (*TreeNodeBehavior)(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end);
		void (*TreeNodeDrawLineToChildNode)(const ImVec2 target_pos);
		void (*TreeNodeDrawLineToTreePop)(const ImGuiTreeNodeStackData* data);
		void (*TreePushOverrideID)(ImGuiID id);
		void (*TreeNodeSetOpen)(ImGuiID storage_id, bool open);
		bool (*TreeNodeUpdateNextOpen)(ImGuiID storage_id, ImGuiTreeNodeFlags flags);
		const ImGuiDataTypeInfo* (*DataTypeGetInfo)(ImGuiDataType data_type);
		int (*DataTypeFormatString)(char* buf, int buf_size, ImGuiDataType data_type, const void* p_data, const char* format);
		void (*DataTypeApplyOp)(ImGuiDataType data_type, int op, void* output, const void* arg_1, const void* arg_2);
		bool (*DataTypeApplyFromText)(const char* buf, ImGuiDataType data_type, void* p_data, const char* format, void* p_data_when_empty);
		int (*DataTypeCompare)(ImGuiDataType data_type, const void* arg_1, const void* arg_2);
		bool (*DataTypeClamp)(ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max);
		bool (*DataTypeIsZero)(ImGuiDataType data_type, const void* p_data);
		bool (*InputTextEx)(const char* label, const char* hint, char* buf, int buf_size, const ImVec2 size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
		void (*InputTextDeactivateHook)(ImGuiID id);
		bool (*TempInputText)(const ImRect bb, ImGuiID id, const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
		bool (*TempInputScalar)(const ImRect bb, ImGuiID id, const char* label, ImGuiDataType data_type, void* p_data, const char* format, const void* p_clamp_min, const void* p_clamp_max);
		void (*ColorTooltip)(const char* text, const float* col, ImGuiColorEditFlags flags);
		void (*ColorEditOptionsPopup)(const float* col, ImGuiColorEditFlags flags);
		void (*ColorPickerOptionsPopup)(const float* ref_col, ImGuiColorEditFlags flags);
		int (*PlotEx)(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void*, int), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, const ImVec2 size_arg);
		void (*ShadeVertsLinearColorGradientKeepAlpha)(ImDrawList_Opaque* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1);
		void (*ShadeVertsLinearUV)(ImDrawList_Opaque* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2 a, const ImVec2 b, const ImVec2 uv_a, const ImVec2 uv_b, bool clamp);
		void (*ShadeVertsTransformPos)(ImDrawList_Opaque* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2 pivot_in, float cos_a, float sin_a, const ImVec2 pivot_out);
		void (*GcCompactTransientMiscBuffers)();
		void (*GcCompactTransientWindowBuffers)(ImGuiWindow* window);
		void (*GcAwakeTransientWindowBuffers)(ImGuiWindow* window);
		bool (*ErrorLog)(const char* msg);
		void (*ErrorRecoveryStoreState)(ImGuiErrorRecoveryState* state_out);
		void (*ErrorRecoveryTryToRecoverState)(const ImGuiErrorRecoveryState* state_in);
		void (*ErrorRecoveryTryToRecoverWindowState)(const ImGuiErrorRecoveryState* state_in);
		void (*ErrorCheckUsingSetCursorPosToExtendParentBoundaries)();
		void (*ErrorCheckEndFrameFinalizeErrorTooltip)();
		bool (*BeginErrorTooltip)();
		void (*EndErrorTooltip)();
		void (*DemoMarker)(const char* file, int line, const char* section);
		void (*DebugAllocHook)(ImGuiDebugAllocInfo* info, int frame_count, void* ptr, size_t size);
		void (*DebugDrawCursorPos)(ImU32 col);
		void (*DebugDrawLineExtents)(ImU32 col);
		void (*DebugDrawItemRect)(ImU32 col);
		void (*DebugTextUnformattedWithLocateItem)(const char* line_begin, const char* line_end);
		void (*DebugLocateItem)(ImGuiID target_id);
		void (*DebugLocateItemOnHover)(ImGuiID target_id);
		void (*DebugLocateItemResolveWithLastItem)();
		void (*DebugBreakClearData)();
		bool (*DebugBreakButton)(const char* label, const char* description_of_location);
		void (*DebugBreakButtonTooltip)(bool keyboard_only, const char* description_of_location);
		void (*ShowFontAtlas)(ImFontAtlas* atlas);
		ImU64 (*DebugTextureIDToU64)(ImTextureID tex_id);
		void (*DebugHookIdInfo)(ImGuiID id, ImGuiDataType data_type, const void* data_id, const void* data_id_end);
		void (*DebugNodeColumns)(ImGuiOldColumns* columns);
		void (*DebugNodeDrawList)(ImGuiWindow* window, ImGuiViewportP* viewport, const ImDrawList_Opaque* draw_list, const char* label);
		void (*DebugNodeDrawCmdShowMeshAndBoundingBox)(ImDrawList_Opaque* out_draw_list, const ImDrawList_Opaque* draw_list, const ImDrawCmd* draw_cmd, bool show_mesh, bool show_aabb);
		void (*DebugNodeFont)(ImFont* font);
		void (*DebugNodeFontGlyphsForSrcMask)(ImFont* font, ImFontBaked* baked, int src_mask);
		void (*DebugNodeFontGlyph)(ImFont* font, const ImFontGlyph* glyph);
		void (*DebugNodeTexture)(ImTextureData* tex, int int_id, const ImFontAtlasRect* highlight_rect);
		void (*DebugNodeStorage)(ImGuiStorage* storage, const char* label);
		void (*DebugNodeTabBar)(ImGuiTabBar* tab_bar, const char* label);
		void (*DebugNodeTable)(ImGuiTable* table);
		void (*DebugNodeTableSettings)(ImGuiTableSettings* settings);
		void (*DebugNodeInputTextState)(ImGuiInputTextState* state);
		void (*DebugNodeTypingSelectState)(ImGuiTypingSelectState* state);
		void (*DebugNodeMultiSelectState)(ImGuiMultiSelectState* state);
		void (*DebugNodeWindow)(ImGuiWindow* window, const char* label);
		void (*DebugNodeWindowSettings)(ImGuiWindowSettings* settings);
		void (*DebugNodeWindowsList)(ImVector<ImGuiWindow*>* windows, const char* label);
		void (*DebugNodeWindowsListByBeginStackParent)(ImGuiWindow** windows, int windows_size, ImGuiWindow* parent_in_begin_stack);
		void (*DebugNodeViewport)(ImGuiViewportP* viewport);
		void (*DebugRenderKeyboardPreview)(ImDrawList_Opaque* draw_list);
		void (*DebugRenderViewportThumbnail)(ImDrawList_Opaque* draw_list, ImGuiViewportP* viewport, const ImRect bb);

		// internal Utilities
		Clipper* (*internal_Create_Clipper)();
		void (*internal_Destroy_Clipper)(Clipper* clipper);
		void* (*MemAlloc)(size_t);
		void (*MemFree)(void* ptr);

		ImGuiID (*ImGuiWindow_GetID_Str)(ImGuiWindow* self, const char* str, const char* str_end);
		ImGuiID (*ImGuiWindow_GetID_Ptr)(ImGuiWindow* self, const void* ptr);
		ImGuiID (*ImGuiWindow_GetID_Int)(ImGuiWindow* self, int n);
		ImGuiID (*ImGuiWindow_GetIDFromPos)(ImGuiWindow* self, const ImVec2 p_abs);
		ImGuiID (*ImGuiWindow_GetIDFromRectangle)(ImGuiWindow* self, const ImRect r_abs);

		// ImGuiStorage
		void (*ImGuiStorage_Clear)(ImGuiStorage* self);
		int (*ImGuiStorage_GetInt)(const ImGuiStorage* self, ImGuiID key, int default_val);
		void (*ImGuiStorage_SetInt)(ImGuiStorage* self, ImGuiID key, int val);
		bool (*ImGuiStorage_GetBool)(const ImGuiStorage* self, ImGuiID key, bool default_val);
		void (*ImGuiStorage_SetBool)(ImGuiStorage* self, ImGuiID key, bool val);
		float (*ImGuiStorage_GetFloat)(const ImGuiStorage* self, ImGuiID key, float default_val);
		void (*ImGuiStorage_SetFloat)(ImGuiStorage* self, ImGuiID key, float val);
		void* (*ImGuiStorage_GetVoidPtr)(const ImGuiStorage* self, ImGuiID key);
		void (*ImGuiStorage_SetVoidPtr)(ImGuiStorage* self, ImGuiID key, void* val);
		int* (*ImGuiStorage_GetIntRef)(ImGuiStorage* self, ImGuiID key, int default_val);
		bool* (*ImGuiStorage_GetBoolRef)(ImGuiStorage* self, ImGuiID key, bool default_val);
		float* (*ImGuiStorage_GetFloatRef)(ImGuiStorage* self, ImGuiID key, float default_val);
		void** (*ImGuiStorage_GetVoidPtrRef)(ImGuiStorage* self, ImGuiID key, void* default_val);
		void (*ImGuiStorage_BuildSortByKey)(ImGuiStorage* self);
		void (*ImGuiStorage_SetAllInt)(ImGuiStorage* self, int val);

		uint32_t sentinel; /* must equal 0xDEADBEEF */
	};
}
