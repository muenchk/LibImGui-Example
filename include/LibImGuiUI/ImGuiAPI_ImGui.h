#pragma once
#include "LibImGuiUI/LibImGuiUI_API.h"

#define API LibImGuiUI::LibImGuiUI_APIv1::imgui

#ifndef H_LibImGuiUIImGui_IMGUI
#	define H_LibImGuiUIImGui_IMGUI 1

namespace LibImGuiUIImGui
{
	// Mock ImGui namespace
	namespace ImGui
	{
		using ImDrawList = ImDrawList_Opaque;
		/*
	// Mock ImDrawList using an opaque proxy cast
	struct ImDrawList
	{
		inline void AddImage(void* user_texture_id, const ImVec2 p_min, const ImVec2 p_max, const ImVec2 uv_min = ImVec2(0, 0), const ImVec2 uv_max = ImVec2(1, 1), uint32_t col = 0xFFFFFFFF)
		{
			API->DrawList_AddImage((ImDrawList_Opaque*)this, user_texture_id, p_min, p_max, uv_min, uv_max, col);
		}
		inline void AddBezierCubic(const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, uint32_t col, float thickness, int num_segments = 0)
		{
			API->DrawList_AddBezierCubic((ImDrawList_Opaque*)this, p1, p2, p3, p4, col, thickness, num_segments);
		}
		inline void PushClipRect(ImVec2 clip_rect_min, ImVec2 clip_rect_max, bool intersect_with_current_clip_rect = false)
		{
			API->DrawList_PushClipRect((ImDrawList_Opaque*)this, clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
		}
		inline void PopClipRect()
		{
			API->DrawList_PopClipRect((ImDrawList_Opaque*)this);
		}
	};*/

		namespace DrawList
		{

			// Clip rect / texture stack
			inline void PushClipRect(ImDrawList* dl, ImVec2 clip_rect_min, ImVec2 clip_rect_max, bool intersect_with_current_clip_rect)
			{
				API->DrawList_PushClipRect((ImDrawList_Opaque*)dl, clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
			}

			inline void PopClipRect(ImDrawList* dl)
			{
				API->DrawList_PopClipRect((ImDrawList_Opaque*)dl);
			}

			inline void PushClipRectFullScreen(ImDrawList* dl)
			{
				API->DrawList_PushClipRectFullScreen((ImDrawList_Opaque*)dl);
			}

			inline void PushTexture(ImDrawList* dl, ImTextureRef tex_ref)
			{
				API->DrawList_PushTexture((ImDrawList_Opaque*)dl, tex_ref);
			}

			inline void PopTexture(ImDrawList* dl)
			{
				API->DrawList_PopTexture((ImDrawList_Opaque*)dl);
			}

			inline ImVec2 GetClipRectMin(ImDrawList* dl)
			{
				return API->DrawList_GetClipRectMin((ImDrawList_Opaque*)dl);
			}

			inline ImVec2 GetClipRectMax(ImDrawList* dl)
			{
				return API->DrawList_GetClipRectMax((ImDrawList_Opaque*)dl);
			}

			// Lines
			inline void AddLine(ImDrawList* dl, const ImVec2 p1, const ImVec2 p2, uint32_t col, float thickness)
			{
				API->DrawList_AddLine((ImDrawList_Opaque*)dl, p1, p2, col, thickness);
			}

			inline void AddLineH(ImDrawList* dl, float min_x, float max_x, float y, uint32_t col, float thickness)
			{
				API->DrawList_AddLineH((ImDrawList_Opaque*)dl, min_x, max_x, y, col, thickness);
			}

			inline void AddLineV(ImDrawList* dl, float x, float min_y, float max_y, uint32_t col, float thickness)
			{
				API->DrawList_AddLineV((ImDrawList_Opaque*)dl, x, min_y, max_y, col, thickness);
			}

			// Rectangles
			inline void AddRect(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, uint32_t col, float rounding, int rounding_corners, float thickness)
			{
				API->DrawList_AddRect((ImDrawList_Opaque*)dl, p_min, p_max, col, rounding, rounding_corners, thickness);
			}

			inline void AddRectFilled(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, uint32_t col, float rounding, int rounding_corners)
			{
				API->DrawList_AddRectFilled((ImDrawList_Opaque*)dl, p_min, p_max, col, rounding, rounding_corners);
			}

			inline void AddRectFilledMultiColor(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, uint32_t col_upr_left, uint32_t col_upr_right, uint32_t col_bot_right, uint32_t col_bot_left)
			{
				API->DrawList_AddRectFilledMultiColor((ImDrawList_Opaque*)dl, p_min, p_max, col_upr_left, col_upr_right, col_bot_right, col_bot_left);
			}

			// Circles
			inline void AddCircle(ImDrawList* dl, ImVec2 center, float radius, uint32_t col, int num_segments, float thickness)
			{
				API->DrawList_AddCircle((ImDrawList_Opaque*)dl, center, radius, col, num_segments, thickness);
			}

			inline void AddCircleFilled(ImDrawList* dl, const ImVec2 center, float radius, uint32_t col, int num_segments)
			{
				API->DrawList_AddCircleFilled((ImDrawList_Opaque*)dl, center, radius, col, num_segments);
			}

			// triangle
			inline void AddTriangle(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, uint32_t col, float thickness)
			{
				API->DrawList_AddTriangle((ImDrawList_Opaque*)dl, p1, p2, p3, col, thickness);
			}

			inline void AddTriangleFilled(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, uint32_t col)
			{
				API->DrawList_AddTriangleFilled((ImDrawList_Opaque*)dl, p1, p2, p3, col);
			}

			// Quads
			inline void AddQuad(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, uint32_t col, float thickness)
			{
				API->DrawList_AddQuad((ImDrawList_Opaque*)dl, p1, p2, p3, p4, col, thickness);
			}

			inline void AddQuadFilled(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, uint32_t col)
			{
				API->DrawList_AddQuadFilled((ImDrawList_Opaque*)dl, p1, p2, p3, p4, col);
			}

			// N-gons
			inline void AddNgon(ImDrawList* dl, ImVec2 center, float radius, uint32_t col, int num_segments, float thickness)
			{
				API->DrawList_AddNgon((ImDrawList_Opaque*)dl, center, radius, col, num_segments, thickness);
			}

			inline void AddNgonFilled(ImDrawList* dl, ImVec2 center, float radius, uint32_t col, int num_segments)
			{
				API->DrawList_AddNgonFilled((ImDrawList_Opaque*)dl, center, radius, col, num_segments);
			}

			// Ellipses
			inline void AddEllipse(ImDrawList* dl, ImVec2 center, ImVec2 radius, uint32_t col, float rot, int num_segments, float thickness)
			{
				API->DrawList_AddEllipse((ImDrawList_Opaque*)dl, center, radius, col, rot, num_segments, thickness);
			}

			inline void AddEllipseFilled(ImDrawList* dl, ImVec2 center, ImVec2 radius, uint32_t col, float rot, int num_segments)
			{
				API->DrawList_AddEllipseFilled((ImDrawList_Opaque*)dl, center, radius, col, rot, num_segments);
			}

			// Text
			inline void AddText(ImDrawList* dl, ImVec2 pos, uint32_t col, const char* text_begin, const char* text_end)
			{
				API->DrawList_AddText((ImDrawList_Opaque*)dl, pos, col, text_begin, text_end);
			}

			inline void AddText(ImDrawList* dl, ImFont* font, float font_size, ImVec2 pos, uint32_t col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
			{
				API->DrawList_AddTextFont((ImDrawList_Opaque*)dl, font, font_size, pos, col, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
			}

			// Bezier
			inline void AddBezierQuadratic(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, uint32_t col, float thickness, int num_segments)
			{
				API->DrawList_AddBezierQuadratic((ImDrawList_Opaque*)dl, p1, p2, p3, col, thickness, num_segments);
			}

			inline void AddBezierCubic(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, uint32_t col, float thickness, int num_segments)
			{
				API->DrawList_AddBezierCubic((ImDrawList_Opaque*)dl, p1, p2, p3, p4, col, thickness, num_segments);
			}

			// Polylines / filled polygons
			inline void AddPolyline(ImDrawList* dl, const ImVec2* points, int num_points, uint32_t col, float thickness, ImDrawFlags flags)
			{
				API->DrawList_AddPolyline((ImDrawList_Opaque*)dl, points, num_points, col, thickness, flags);
			}

			inline void AddConvexPolyFilled(ImDrawList* dl, const ImVec2* points, int num_points, uint32_t col)
			{
				API->DrawList_AddConvexPolyFilled((ImDrawList_Opaque*)dl, points, num_points, col);
			}

			inline void AddConcavePolyFilled(ImDrawList* dl, const ImVec2* points, int num_points, uint32_t col)
			{
				API->DrawList_AddConcavePolyFilled((ImDrawList_Opaque*)dl, points, num_points, col);
			}

			// Image quads / rounded images
			inline void AddImageQuad(ImDrawList* dl, ImTextureRef tex_ref, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, ImVec2 uv1, ImVec2 uv2, ImVec2 uv3, ImVec2 uv4, uint32_t col)
			{
				API->DrawList_AddImageQuad((ImDrawList_Opaque*)dl, tex_ref, p1, p2, p3, p4, uv1, uv2, uv3, uv4, col);
			}

			inline void AddImageRounded(ImDrawList* dl, ImTextureRef tex_ref, ImVec2 p_min, ImVec2 p_max, ImVec2 uv_min, ImVec2 uv_max, uint32_t col, float rounding, ImDrawFlags flags)
			{
				API->DrawList_AddImageRounded((ImDrawList_Opaque*)dl, tex_ref, p_min, p_max, uv_min, uv_max, col, rounding, flags);
			}

			inline void AddImage(ImDrawList* dl, ImTextureRef tex_ref, const ImVec2 p_min, const ImVec2 p_max, const ImVec2 uv_min, const ImVec2 uv_max, uint32_t col)
			{
				API->DrawList_AddImage((ImDrawList_Opaque*)dl, tex_ref, p_min, p_max, uv_min, uv_max, col);
			}

			// Path API
			inline void PathClear(ImDrawList* dl)
			{
				API->DrawList_PathClear((ImDrawList_Opaque*)dl);
			}

			inline void PathLineTo(ImDrawList* dl, ImVec2 pos)
			{
				API->DrawList_PathLineTo((ImDrawList_Opaque*)dl, pos);
			}

			inline void PathLineToMergeDuplicate(ImDrawList* dl, ImVec2 pos)
			{
				API->DrawList_PathLineToMergeDuplicate((ImDrawList_Opaque*)dl, pos);
			}

			inline void PathFillConvex(ImDrawList* dl, uint32_t col)
			{
				API->DrawList_PathFillConvex((ImDrawList_Opaque*)dl, col);
			}

			inline void PathFillConcave(ImDrawList* dl, uint32_t col)
			{
				API->DrawList_PathFillConcave((ImDrawList_Opaque*)dl, col);
			}

			inline void PathStroke(ImDrawList* dl, uint32_t col, float thickness, ImDrawFlags flags)
			{
				API->DrawList_PathStroke((ImDrawList_Opaque*)dl, col, thickness, flags);
			}

			inline void PathArcTo(ImDrawList* dl, ImVec2 center, float radius, float a_min, float a_max, int num_segments)
			{
				API->DrawList_PathArcTo((ImDrawList_Opaque*)dl, center, radius, a_min, a_max, num_segments);
			}

			inline void PathArcToFast(ImDrawList* dl, ImVec2 center, float radius, int a_min_of_12, int a_max_of_12)
			{
				API->DrawList_PathArcToFast((ImDrawList_Opaque*)dl, center, radius, a_min_of_12, a_max_of_12);
			}

			inline void PathEllipticalArcTo(ImDrawList* dl, ImVec2 center, ImVec2 radius, float rot, float a_min, float a_max, int num_segments)
			{
				API->DrawList_PathEllipticalArcTo((ImDrawList_Opaque*)dl, center, radius, rot, a_min, a_max, num_segments);
			}

			inline void PathBezierCubicCurveTo(ImDrawList* dl, ImVec2 p2, ImVec2 p3, ImVec2 p4, int num_segments)
			{
				API->DrawList_PathBezierCubicCurveTo((ImDrawList_Opaque*)dl, p2, p3, p4, num_segments);
			}

			inline void PathBezierQuadraticCurveTo(ImDrawList* dl, ImVec2 p2, ImVec2 p3, int num_segments)
			{
				API->DrawList_PathBezierQuadraticCurveTo((ImDrawList_Opaque*)dl, p2, p3, num_segments);
			}

			inline void PathRect(ImDrawList* dl, ImVec2 rect_min, ImVec2 rect_max, float rounding, ImDrawFlags flags)
			{
				API->DrawList_PathRect((ImDrawList_Opaque*)dl, rect_min, rect_max, rounding, flags);
			}

			// Channels
			inline void ChannelsSplit(ImDrawList* dl, int count)
			{
				API->DrawList_ChannelsSplit((ImDrawList_Opaque*)dl, count);
			}

			inline void ChannelsMerge(ImDrawList* dl)
			{
				API->DrawList_ChannelsMerge((ImDrawList_Opaque*)dl);
			}

			inline void ChannelsSetCurrent(ImDrawList* dl, int n)
			{
				API->DrawList_ChannelsSetCurrent((ImDrawList_Opaque*)dl, n);
			}

			// Primitive allocation
			inline void PrimReserve(ImDrawList* dl, int idx_count, int vtx_count)
			{
				API->DrawList_PrimReserve((ImDrawList_Opaque*)dl, idx_count, vtx_count);
			}

			inline void PrimUnreserve(ImDrawList* dl, int idx_count, int vtx_count)
			{
				API->DrawList_PrimUnreserve((ImDrawList_Opaque*)dl, idx_count, vtx_count);
			}

			inline void PrimRect(ImDrawList* dl, ImVec2 a, ImVec2 b, uint32_t col)
			{
				API->DrawList_PrimRect((ImDrawList_Opaque*)dl, a, b, col);
			}

			inline void PrimRectUV(ImDrawList* dl, ImVec2 a, ImVec2 b, ImVec2 uv_a, ImVec2 uv_b, uint32_t col)
			{
				API->DrawList_PrimRectUV((ImDrawList_Opaque*)dl, a, b, uv_a, uv_b, col);
			}

			inline void PrimQuadUV(ImDrawList* dl, ImVec2 a, ImVec2 b, ImVec2 c, ImVec2 d, ImVec2 uv_a, ImVec2 uv_b, ImVec2 uv_c, ImVec2 uv_d, uint32_t col)
			{
				API->DrawList_PrimQuadUV((ImDrawList_Opaque*)dl, a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);
			}

			inline void PrimWriteVtx(ImDrawList* dl, ImVec2 pos, ImVec2 uv, uint32_t col)
			{
				API->DrawList_PrimWriteVtx((ImDrawList_Opaque*)dl, pos, uv, col);
			}

			inline void PrimWriteIdx(ImDrawList* dl, ImDrawIdx idx)
			{
				API->DrawList_PrimWriteIdx((ImDrawList_Opaque*)dl, idx);
			}

			inline void PrimVtx(ImDrawList* dl, ImVec2 pos, ImVec2 uv, uint32_t col)
			{
				API->DrawList_PrimVtx((ImDrawList_Opaque*)dl, pos, uv, col);
			}

			// Callbacks / misc
			inline void AddCallback(ImDrawList* dl, ImDrawCallback callback, void* userdata, size_t userdata_size)
			{
				API->DrawList_AddCallback((ImDrawList_Opaque*)dl, callback, userdata, userdata_size);
			}

			inline void AddDrawCmd(ImDrawList* dl)
			{
				API->DrawList_AddDrawCmd((ImDrawList_Opaque*)dl);
			}
		}

		namespace ImGuiStorage_
		{
			inline void Clear(ImGuiStorage* self)
			{
				API->ImGuiStorage_Clear((ImGuiStorage*)self);
			}

			inline int GetInt(const ImGuiStorage* self, ImGuiID key, int default_val)
			{
				return API->ImGuiStorage_GetInt((const ImGuiStorage*)self, key, default_val);
			}

			inline void SetInt(ImGuiStorage* self, ImGuiID key, int val)
			{
				API->ImGuiStorage_SetInt((ImGuiStorage*)self, key, val);
			}

			inline bool GetBool(const ImGuiStorage* self, ImGuiID key, bool default_val)
			{
				return API->ImGuiStorage_GetBool((const ImGuiStorage*)self, key, default_val);
			}

			inline void SetBool(ImGuiStorage* self, ImGuiID key, bool val)
			{
				API->ImGuiStorage_SetBool((ImGuiStorage*)self, key, val);
			}

			inline float GetFloat(const ImGuiStorage* self, ImGuiID key, float default_val)
			{
				return API->ImGuiStorage_GetFloat((const ImGuiStorage*)self, key, default_val);
			}

			inline void SetFloat(ImGuiStorage* self, ImGuiID key, float val)
			{
				API->ImGuiStorage_SetFloat((ImGuiStorage*)self, key, val);
			}

			inline void* GetVoidPtr(const ImGuiStorage* self, ImGuiID key)
			{
				return API->ImGuiStorage_GetVoidPtr((const ImGuiStorage*)self, key);
			}

			inline void SetVoidPtr(ImGuiStorage* self, ImGuiID key, void* val)
			{
				API->ImGuiStorage_SetVoidPtr((ImGuiStorage*)self, key, val);
			}

			inline int* GetIntRef(ImGuiStorage* self, ImGuiID key, int default_val)
			{
				return API->ImGuiStorage_GetIntRef((ImGuiStorage*)self, key, default_val);
			}

			inline bool* GetBoolRef(ImGuiStorage* self, ImGuiID key, bool default_val)
			{
				return API->ImGuiStorage_GetBoolRef((ImGuiStorage*)self, key, default_val);
			}

			inline float* GetFloatRef(ImGuiStorage* self, ImGuiID key, float default_val)
			{
				return API->ImGuiStorage_GetFloatRef((ImGuiStorage*)self, key, default_val);
			}

			inline void** GetVoidPtrRef(ImGuiStorage* self, ImGuiID key, void* default_val)
			{
				return API->ImGuiStorage_GetVoidPtrRef((ImGuiStorage*)self, key, default_val);
			}

			inline void BuildSortByKey(ImGuiStorage* self)
			{
				API->ImGuiStorage_BuildSortByKey((ImGuiStorage*)self);
			}

			inline void SetAllInt(ImGuiStorage* self, int val)
			{
				API->ImGuiStorage_SetAllInt((ImGuiStorage*)self, val);
			}
		}

		// style and stuff
		inline ImGuiStyle* GetStyle() { return API->GetStyle(); }
		inline ImGuiIO* GetIO() { return API->GetIO(); }

		// Context
		inline ImGuiContext* GetCurrentContext() { return ImGui::GetCurrentContext(); }
		inline void SetCurrentContext(ImGuiContext* ctx) { ImGui::SetCurrentContext(ctx); }

		// windows
		inline bool Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0) { return API->Begin(name, p_open, flags); }
		inline void End() { API->End(); }

		// Navigation
		inline void SetNavCursorVisible(bool visible) { API->SetNavCursorVisible(visible); }

		// Overlapping
		inline void SetNextItemAllowOverlap() { API->SetNextItemAllowOverlap(); }

		// child windows
		inline bool BeginChild(const char* str_id, const ImVec2 size = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0) { return API->BeginChild(str_id, size, child_flags, window_flags); }
		inline bool BeginChild(ImGuiID id, const ImVec2 size = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0) { return API->BeginChildID(id, size, child_flags, window_flags); }
		inline void EndChild() { API->EndChild(); }

		// windows utilities
		inline bool IsWindowAppearing() { return API->IsWindowAppearing(); }
		inline bool IsWindowCollapsed() { return API->IsWindowCollapsed(); }
		inline bool IsWindowFocused(ImGuiFocusedFlags flags = 0) { return API->IsWindowFocused(flags); }
		inline bool IsWindowHovered(ImGuiHoveredFlags flags = 0) { return API->IsWindowHovered(flags); }
		inline ImDrawList* GetWindowDrawList() { return (ImDrawList*)API->GetWindowDrawList(); }
		inline ImVec2 GetWindowPos() { return API->GetWindowPos(); }
		inline ImVec2 GetWindowSize() { return API->GetWindowSize(); }
		inline float GetWindowWidth() { return API->GetWindowWidth(); }
		inline float GetWindowHeight() { return API->GetWindowHeight(); }

		// window manipulation
		inline void SetNextWindowPos(const ImVec2 pos, ImGuiCond cond = 0, const ImVec2 pivot = ImVec2(0, 0)) { API->SetNextWindowPos(pos, cond, pivot); }
		inline void SetNextWindowSize(const ImVec2 size, ImGuiCond cond = 0) { API->SetNextWindowSize(size, cond); }
		inline void SetNextWindowSizeConstraints(const ImVec2 size_min, const ImVec2 size_max, ImGuiSizeCallback custom_callback = NULL, void* custom_callback_data = NULL) { API->SetNextWindowSizeConstraints(size_min, size_max, custom_callback, custom_callback_data); }
		inline void SetNextWindowContentSize(const ImVec2 size) { API->SetNextWindowContentSize(size); }
		inline void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0) { API->SetNextWindowCollapsed(collapsed, cond); }
		inline void SetNextWindowFocus() { API->SetNextWindowFocus(); }
		inline void SetNextWindowScroll(const ImVec2 scroll) { API->SetNextWindowScroll(scroll); }
		inline void SetNextWindowBgAlpha(float alpha) { API->SetNextWindowBgAlpha(alpha); }
		inline void SetWindowPos(const ImVec2 pos, ImGuiCond cond = 0) { API->SetWindowPos(pos, cond); }
		inline void SetWindowSize(const ImVec2 size, ImGuiCond cond = 0) { API->SetWindowSize(size, cond); }
		inline void SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0) { API->SetWindowCollapsed(collapsed, cond); }
		inline void SetWindowFocus() { API->SetWindowFocus(); }
		inline void SetWindowPos(const char* name, const ImVec2 pos, ImGuiCond cond = 0) { API->SetWindowPosByName(name, pos, cond); }
		inline void SetWindowSize(const char* name, const ImVec2 size, ImGuiCond cond = 0) { API->SetWindowSizeByName(name, size, cond); }
		inline void SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond = 0) { API->SetWindowCollapsedByName(name, collapsed, cond); }
		inline void SetWindowFocus(const char* name) { API->SetWindowFocusByName(name); }

		// window scrolling
		inline float GetScrollX() { return API->GetScrollX(); }
		inline float GetScrollY() { return API->GetScrollY(); }
		inline void SetScrollX(float scroll_x) { API->SetScrollX(scroll_x); }
		inline void SetScrollY(float scroll_y) { API->SetScrollY(scroll_y); }
		inline float GetScrollMaxX() { return API->GetScrollMaxX(); }
		inline float GetScrollMaxY() { return API->GetScrollMaxY(); }
		inline void SetScrollHereX(float center_x_ratio = 0.5f) { API->SetScrollHereX(center_x_ratio); }
		inline void SetScrollHereY(float center_y_ratio = 0.5f) { API->SetScrollHereY(center_y_ratio); }
		inline void SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f) { API->SetScrollFromPosX(local_x, center_x_ratio); }
		inline void SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f) { API->SetScrollFromPosY(local_y, center_y_ratio); }

		// parameter stacks (font)
		inline void PushFont(ImFont* font, float font_size_base_unscaled) { API->PushFont(font, font_size_base_unscaled); }
		inline void PopFont() { API->PopFont(); }
		inline ImFont* GetFont() { return API->GetFont(); }
		inline float GetFontSize() { return API->GetFontSize(); }
		inline ImFontBaked* GetFontBaked() { return API->GetFontBaked(); }

		// Parameters stacks (shared)
		inline void PushStyleColor(ImGuiCol idx, ImU32 col) { API->PushStyleColor_U32(idx, col); }
		inline void PushStyleColor(ImGuiCol idx, const ImVec4 col) { API->PushStyleColor_Vec4(idx, col); }
		inline void PopStyleColor(int count = 1) { API->PopStyleColor(count); }
		inline void PushStyleVar(ImGuiStyleVar idx, float val) { API->PushStyleVar_Float(idx, val); }
		inline void PushStyleVar(ImGuiStyleVar idx, ImVec2 val) { API->PushStyleVar_Vec2(idx, val); }
		inline void PushStyleVarX(ImGuiStyleVar idx, float val_x) { API->PushStyleVarX(idx, val_x); }
		inline void PushStyleVarY(ImGuiStyleVar idx, float val_y) { API->PushStyleVarY(idx, val_y); }
		inline void PopStyleVar(int count = 1) { API->PopStyleVar(count); }
		inline void PushItemFlag(ImGuiItemFlags option, bool enabled) { API->PushItemFlag(option, enabled); }
		inline void PopItemFlag() { API->PopItemFlag(); }
		inline ImGuiItemFlags GetItemFlags() { return API->GetItemFlags(); }

		// Parameters stacks (current window)
		inline void PushItemWidth(float item_width) { API->PushItemWidth(item_width); }
		inline void PopItemWidth() { API->PopItemWidth(); }
		inline void SetNextItemWidth(float item_width) { API->SetNextItemWidth(item_width); }
		inline float CalcItemWidth() { return API->CalcItemWidth(); }
		inline void PushTextWrapPos(float wrap_local_pos_x = 0.0f) { API->PushTextWrapPos(wrap_local_pos_x); }
		inline void PopTextWrapPos() { API->PopTextWrapPos(); }

		// Style read access
		inline ImVec2 GetFontTexUvWhitePixel() { return API->GetFontTexUvWhitePixel(); }
		inline ImU32 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f) { return API->GetColorU32_ColFloat(idx, alpha_mul); }
		inline ImU32 GetColorU32(const ImVec4 col) { return API->GetColorU32_Vec4(col); }
		inline ImU32 GetColorU32(ImU32 col, float alpha_mul = 1.0f) { return API->GetColorU32_U32Float(col, alpha_mul); }
		inline const ImVec4 GetStyleColorVec4(ImGuiCol idx) { return API->GetStyleColorVec4(idx); }

		// Viewports
		inline ImGuiViewport* GetMainViewport() { return API->GetMainViewport(); }

		// DrawLists
		inline ImDrawList* GetBackgroundDrawList() { return (ImDrawList*)API->GetBackgroundDrawListDirect(); }
		inline ImDrawList* GetForegroundDrawList() { return (ImDrawList*)API->GetForegroundDrawListDirect(); }

		// Layout cursor positioning
		inline ImVec2 GetCursorScreenPos() { return API->GetCursorScreenPos(); }
		inline void SetCursorScreenPos(const ImVec2 pos) { API->SetCursorScreenPos(pos); }
		inline ImVec2 GetContentRegionAvail() { return API->GetContentRegionAvail(); }
		inline ImVec2 GetCursorPos() { return API->GetCursorPos(); }
		inline float GetCursorPosX() { return API->GetCursorPosX(); }
		inline float GetCursorPosY() { return API->GetCursorPosY(); }
		inline void SetCursorPos(const ImVec2 local_pos) { API->SetCursorPos(local_pos); }
		inline void SetCursorPosX(float local_x) { API->SetCursorPosX(local_x); }
		inline void SetCursorPosY(float local_y) { API->SetCursorPosY(local_y); }
		inline ImVec2 GetCursorStartPos() { return API->GetCursorStartPos(); }

		// Other layout functions
		inline void Separator() { API->Separator(); }
		inline void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f) { API->SameLine(offset_from_start_x, spacing); }
		inline void NewLine() { API->NewLine(); }
		inline void Spacing() { API->Spacing(); }
		inline void Dummy(const ImVec2 size) { API->Dummy(size); }
		inline void Indent(float indent_w = 0.0f) { API->Indent(indent_w); }
		inline void Unindent(float indent_w = 0.0f) { API->Unindent(indent_w); }
		inline void BeginGroup() { API->BeginGroup(); }
		inline void EndGroup() { API->EndGroup(); }
		inline void AlignTextToFramePadding() { API->AlignTextToFramePadding(); }
		inline float GetTextLineHeight() { return API->GetTextLineHeight(); }
		inline float GetTextLineHeightWithSpacing() { return API->GetTextLineHeightWithSpacing(); }
		inline float GetFrameHeight() { return API->GetFrameHeight(); }
		inline float GetFrameHeightWithSpacing() { return API->GetFrameHeightWithSpacing(); }

		// id stack / scopes
		inline void PushID(const char* str_id) { API->PushIDStr(str_id); }
		inline void PushID(const char* str_id_begin, const char* str_id_end) { API->PushIDStrRange(str_id_begin, str_id_end); }
		inline void PushID(const void* ptr_id) { API->PushIDPtr(ptr_id); }
		inline void PushID(int int_id) { API->PushIDInt(int_id); }
		inline void PopID() { API->PopID(); }
		inline ImGuiID GetID(const char* str_id) { return API->GetIDStr(str_id); }
		inline ImGuiID GetID(const char* str_id_begin, const char* str_id_end) { return API->GetIDStrRange(str_id_begin, str_id_end); }
		inline ImGuiID GetID(const void* ptr_id) { return API->GetIDPtr(ptr_id); }
		inline ImGuiID GetID(int int_id) { return API->GetID(int_id); }

		// Widgets: Text
		inline void TextUnformatted(const char* text, const char* text_end = NULL) { API->TextUnformatted(text, text_end); }
		inline void Text(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			API->Text(fmt, args);
			va_end(args);
		}
		inline void TextV(const char* fmt, va_list args)
		{
			API->TextV(fmt, args);
		}
		inline void TextColored(ImVec4 col, const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			API->TextColored(col, fmt, args);
			va_end(args);
		}
		inline void TextColoredV(const ImVec4 col, const char* fmt, va_list args)
		{
			API->TextColoredV(col, fmt, args);
		}
		inline void TextDisabled(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			API->TextDisabled(fmt, args);
			va_end(args);
		}
		inline void TextDisabledV(const char* fmt, va_list args)
		{
			API->TextDisabledV(fmt, args);
		}
		inline void TextWrapped(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			API->TextWrapped(fmt, args);
			va_end(args);
		}
		inline void TextWrappedV(const char* fmt, va_list args)
		{
			API->TextWrappedV(fmt, args);
		}
		inline void LabelText(const char* label, const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			API->LabelText(label, fmt, args);
			va_end(args);
		}
		inline void LabelTextV(const char* label, const char* fmt, va_list args)
		{
			API->LabelTextV(label, fmt, args);
		}
		inline void BulletText(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			API->BulletText(fmt, args);
			va_end(args);
		}
		inline void BulletTextV(const char* fmt, va_list args)
		{
			API->BulletTextV(fmt, args);
		}
		inline void SeparatorText(const char* label) { API->SeparatorText(label); }

		// Widgets: Main
		inline bool Button(const char* label, const ImVec2 size = ImVec2(0, 0)) { return API->Button(label, size); }
		inline bool SmallButton(const char* label) { return API->SmallButton(label); }
		inline bool InvisibleButton(const char* str_id, const ImVec2 size, ImGuiButtonFlags flags = 0) { return API->InvisibleButton(str_id, size, flags); }
		inline bool ArrowButton(const char* str_id, ImGuiDir dir) { return API->ArrowButton(str_id, dir); }
		inline bool Checkbox(const char* label, bool* v) { return API->Checkbox(label, v); }
		inline bool CheckboxFlags(const char* label, int* flags, int flags_value) { return API->CheckboxFlags_IntPtr(label, flags, flags_value); }
		inline bool CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value) { return API->CheckboxFlags_UintPtr(label, flags, flags_value); }
		inline bool RadioButton(const char* label, bool active) { return API->RadioButton(label, active); }
		inline bool RadioButton(const char* label, int* v, int v_button) { return API->RadioButtonIntPtr(label, v, v_button); }
		inline void ProgressBar(float fraction, const ImVec2 size_arg = ImVec2(-FLT_MIN, 0), const char* overlay = NULL) { API->ProgressBar(fraction, size_arg, overlay); }
		inline void Bullet() { API->Bullet(); }
		inline bool TextLink(const char* label) { return API->TextLink(label); }
		inline bool TextLinkOpenURL(const char* label, const char* url = NULL) { return API->TextLinkOpenURL(label, url); }

		// Widgets: Image
		inline void Image(ImTextureRef tex_ref, const ImVec2 image_size, const ImVec2 uv0 = ImVec2(0, 0), const ImVec2 uv1 = ImVec2(1, 1)) { API->Image(tex_ref, image_size, uv0, uv1); }
		inline void ImageWithBg(ImTextureRef tex_ref, const ImVec2 image_size, const ImVec2 uv0 = ImVec2(0, 0), const ImVec2 uv1 = ImVec2(1, 1), const ImVec4 bg_col = ImVec4(0, 0, 0, 0), const ImVec4 tint_col = ImVec4(1, 1, 1, 1)) { API->ImageWithBg(tex_ref, image_size, uv0, uv1, bg_col, tint_col); }
		inline bool ImageButton(const char* str_id, ImTextureRef tex_ref, const ImVec2 image_size, const ImVec2 uv0 = ImVec2(0, 0), const ImVec2 uv1 = ImVec2(1, 1), const ImVec4 bg_col = ImVec4(0, 0, 0, 0), const ImVec4 tint_col = ImVec4(1, 1, 1, 1)) { return API->ImageButton(str_id, tex_ref, image_size, uv0, uv1, bg_col, tint_col); }

		// Widgets: Combo
		inline bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0) { return API->BeginCombo(label, preview_value, flags); }
		inline void EndCombo() { API->EndCombo(); }
		inline bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1) { return API->Combo(label, current_item, items, items_count, popup_max_height_in_items); }
		inline bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1) { return API->ComboStr(label, current_item, items_separated_by_zeros, popup_max_height_in_items); }
		inline bool Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1) { return API->ComboCallback(label, current_item, getter, user_data, items_count, popup_max_height_in_items); }

		// Widgets: Drag Sliders
		inline bool DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) { return API->DragFloat(label, v, v_speed, v_min, v_max, format, flags); }
		inline bool DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) { return API->DragFloat2(label, v, v_speed, v_min, v_max, format, flags); }
		inline bool DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) { return API->DragFloat3(label, v, v_speed, v_min, v_max, format, flags); }
		inline bool DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) { return API->DragFloat4(label, v, v_speed, v_min, v_max, format, flags); }
		inline bool DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, ImGuiSliderFlags flags = 0) { return API->DragFloatRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags); }
		inline bool DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) { return API->DragInt(label, v, v_speed, v_min, v_max, format, flags); }
		inline bool DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) { return API->DragInt2(label, v, v_speed, v_min, v_max, format, flags); }
		inline bool DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) { return API->DragInt3(label, v, v_speed, v_min, v_max, format, flags); }
		inline bool DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) { return API->DragInt4(label, v, v_speed, v_min, v_max, format, flags); }
		inline bool DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL, ImGuiSliderFlags flags = 0) { return API->DragIntRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags); }
		inline bool DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0) { return API->DragScalar(label, data_type, p_data, v_speed, p_min, p_max, format, flags); }
		inline bool DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0) { return API->DragScalarN(label, data_type, p_data, components, v_speed, p_min, p_max, format, flags); }

		// Widgets: Regular Sliders
		inline bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) { return API->SliderFloat(label, v, v_min, v_max, format, flags); }
		inline bool SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) { return API->SliderFloat2(label, v, v_min, v_max, format, flags); }
		inline bool SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) { return API->SliderFloat3(label, v, v_min, v_max, format, flags); }
		inline bool SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) { return API->SliderFloat4(label, v, v_min, v_max, format, flags); }
		inline bool SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg", ImGuiSliderFlags flags = 0) { return API->SliderAngle(label, v_rad, v_degrees_min, v_degrees_max, format, flags); }
		inline bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) { return API->SliderInt(label, v, v_min, v_max, format, flags); }
		inline bool SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) { return API->SliderInt2(label, v, v_min, v_max, format, flags); }
		inline bool SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) { return API->SliderInt3(label, v, v_min, v_max, format, flags); }
		inline bool SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) { return API->SliderInt4(label, v, v_min, v_max, format, flags); }
		inline bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0) { return API->SliderScalar(label, data_type, p_data, p_min, p_max, format, flags); }
		inline bool SliderScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0) { return API->SliderScalarN(label, data_type, p_data, components, p_min, p_max, format, flags); }
		inline bool VSliderFloat(const char* label, const ImVec2 size, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) { return API->VSliderFloat(label, size, v, v_min, v_max, format, flags); }
		inline bool VSliderInt(const char* label, const ImVec2 size, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) { return API->VSliderInt(label, size, v, v_min, v_max, format, flags); }
		inline bool VSliderScalar(const char* label, const ImVec2 size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0) { return API->VSliderScalar(label, size, data_type, p_data, p_min, p_max, format, flags); }

		// Widgets: Input with Keyboard
		inline bool InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL) { return API->InputText(label, buf, buf_size, flags, callback, user_data); }
		inline bool InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2 size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL) { return API->InputTextMultiline(label, buf, buf_size, size, flags, callback, user_data); }
		inline bool InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL) { return API->InputTextWithHint(label, hint, buf, buf_size, flags, callback, user_data); }
		inline bool InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0) { return API->InputFloat(label, v, step, step_fast, format, flags); }
		inline bool InputFloat2(const char* label, float v[2], const char* format = "%.3f", ImGuiInputTextFlags flags = 0) { return API->InputFloat2(label, v, format, flags); }
		inline bool InputFloat3(const char* label, float v[3], const char* format = "%.3f", ImGuiInputTextFlags flags = 0) { return API->InputFloat3(label, v, format, flags); }
		inline bool InputFloat4(const char* label, float v[4], const char* format = "%.3f", ImGuiInputTextFlags flags = 0) { return API->InputFloat4(label, v, format, flags); }
		inline bool InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0) { return API->InputInt(label, v, step, step_fast, flags); }
		inline bool InputInt2(const char* label, int v[2], ImGuiInputTextFlags flags = 0) { return API->InputInt2(label, v, flags); }
		inline bool InputInt3(const char* label, int v[3], ImGuiInputTextFlags flags = 0) { return API->InputInt3(label, v, flags); }
		inline bool InputInt4(const char* label, int v[4], ImGuiInputTextFlags flags = 0) { return API->InputInt4(label, v, flags); }
		inline bool InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0) { return API->InputDouble(label, v, step, step_fast, format, flags); }
		inline bool InputScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0) { return API->InputScalar(label, data_type, p_data, p_step, p_step_fast, format, flags); }
		inline bool InputScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0) { return API->InputScalarN(label, data_type, p_data, components, p_step, p_step_fast, format, flags); }

		// Widgets: Color Editor/Picker
		inline bool ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0) { return API->ColorEdit3(label, col, flags); }
		inline bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0) { return API->ColorEdit4(label, col, flags); }
		inline bool ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0) { return API->ColorPicker3(label, col, flags); }
		inline bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL) { return API->ColorPicker4(label, col, flags, ref_col); }
		inline bool ColorButton(const char* desc_id, const ImVec4 col, ImGuiColorEditFlags flags = 0, const ImVec2 size = ImVec2(0, 0)) { return API->ColorButton(desc_id, col, flags, size); }
		inline void SetColorEditOptions(ImGuiColorEditFlags flags) { API->SetColorEditOptions(flags); }

		// Widgets: Trees
		inline bool TreeNode(const char* label) { return API->TreeNode(label); }
		inline bool TreeNode(const char* str_id, const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			bool r = API->TreeNodeVStr(str_id, fmt, args);
			va_end(args);
			return r;
		}
		inline bool TreeNode(const void* ptr_id, const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			bool r = API->TreeNodeVPtr(ptr_id, fmt, args);
			va_end(args);
			return r;
		}
		inline bool TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0) { return API->TreeNodeEx(label, flags); }
		inline bool TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			bool r = API->TreeNodeExVStr(str_id, flags, fmt, args);
			va_end(args);
			return r;
		}
		inline bool TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			bool r = API->TreeNodeExVPtr(ptr_id, flags, fmt, args);
			va_end(args);
			return r;
		}
		inline bool TreeNodeV(const char* str_id, const char* fmt, va_list args) { return API->TreeNodeVStr(str_id, fmt, args); }
		inline bool TreeNodeV(const void* ptr_id, const char* fmt, va_list args) { return API->TreeNodeVPtr(ptr_id, fmt, args); }
		inline bool TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) { return API->TreeNodeExVStr(str_id, flags, fmt, args); }
		inline bool TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) { return API->TreeNodeExVPtr(ptr_id, flags, fmt, args); }
		inline void TreePush(const char* str_id) { API->TreePush(str_id); }
		inline void TreePush(const void* ptr_id) { API->TreePushPtr(ptr_id); }
		inline void TreePop() { API->TreePop(); }
		inline float GetTreeNodeToLabelSpacing() { return API->GetTreeNodeToLabelSpacing(); }
		inline bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0) { return API->CollapsingHeader(label, flags); }
		inline bool CollapsingHeader(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags = 0) { return API->CollapsingHeaderBoolPtr(label, p_visible, flags); }
		inline void SetNextItemOpen(bool is_open, ImGuiCond cond = 0) { API->SetNextItemOpen(is_open, cond); }
		inline void SetNextItemStorageID(ImGuiID storage_id) { API->SetNextItemStorageID(storage_id); }
		inline bool TreeNodeGetOpen(ImGuiID storage_id) { return API->TreeNodeGetOpen(storage_id); }

		// Widgets: Selectables
		inline bool Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2 size = ImVec2(0, 0)) { return API->Selectable(label, selected, flags, size); }
		inline bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2 size = ImVec2(0, 0)) { return API->SelectableBoolPtr(label, p_selected, flags, size); }

		// Widgets: List Boxes
		inline bool BeginListBox(const char* label, const ImVec2 size = ImVec2(0, 0)) { return API->BeginListBox(label, size); }
		inline void EndListBox() { API->EndListBox(); }
		inline bool ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1) { return API->ListBox(label, current_item, items, items_count, height_in_items); }
		inline bool ListBox(const char* label, int* current_item, const char* (*getter)(void* data, int idx), void* data, int items_count, int height_in_items = -1) { return API->ListBoxCallback(label, current_item, getter, data, items_count, height_in_items); }

		// Widgets: Data Plotting
		inline void PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float)) { API->PlotLines(label, values, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size, stride); }
		inline void PlotLines(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0)) { API->PlotLinesCallback(label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size); }
		inline void PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float)) { API->PlotHistogram(label, values, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size, stride); }
		inline void PlotHistogram(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0)) { API->PlotHistogramCallback(label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size); }

		// Widgets: Menus
		inline bool BeginMenuBar() { return API->BeginMenuBar(); }
		inline void EndMenuBar() { API->EndMenuBar(); }
		inline bool BeginMainMenuBar() { return API->BeginMainMenuBar(); }
		inline void EndMainMenuBar() { API->EndMainMenuBar(); }
		inline bool BeginMenu(const char* label, bool enabled = true) { return API->BeginMenu(label, enabled); }
		inline void EndMenu() { API->EndMenu(); }
		inline bool MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true) { return API->MenuItem(label, shortcut, selected, enabled); }
		inline bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true) { return API->MenuItemBoolPtr(label, shortcut, p_selected, enabled); }

		// Value
		inline void Value(const char* prefix, bool b) { API->ValueBool(prefix, b); }
		inline void Value(const char* prefix, int v) { API->ValueInt(prefix, v); }
		inline void Value(const char* prefix, unsigned int v) { API->ValueUint(prefix, v); }
		inline void Value(const char* prefix, float v, const char* float_format) { API->ValueFloat(prefix, v, float_format); }

		// Tooltips
		inline bool BeginTooltip() { return API->BeginTooltip(); }
		inline void EndTooltip() { API->EndTooltip(); }
		inline void SetTooltip(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			API->SetTooltip(fmt, args);
			va_end(args);
		}
		inline void SetTooltipV(const char* fmt, va_list args)
		{
			API->SetTooltipV(fmt, args);
		}
		inline bool BeginItemTooltip() { return API->BeginItemTooltip(); }
		inline void SetItemTooltip(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			API->SetItemTooltip(fmt, args);
			va_end(args);
		}
		inline void SetItemTooltipV(const char* fmt, va_list args) { API->SetItemTooltipV(fmt, args); }

		// Popups: open/close functions
		// TODO

		// Popups: Open+Begin
		inline bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0) { return API->BeginPopup(str_id, flags); }
		inline bool BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0) { return API->BeginPopupModal(name, p_open, flags); }
		inline void EndPopup() { API->EndPopup(); }
		inline void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0) { API->OpenPopup(str_id, popup_flags); }
		inline void OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0) { API->OpenPopupID(id, popup_flags); }
		inline void OpenPopupOnItemClick(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) { API->OpenPopupOnItemClick(str_id, popup_flags); }
		inline void CloseCurrentPopup() { API->CloseCurrentPopup(); }
		inline bool BeginPopupContextItem(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) { return API->BeginPopupContextItem(str_id, popup_flags); }
		inline bool BeginPopupContextWindow(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) { return API->BeginPopupContextWindow(str_id, popup_flags); }
		inline bool BeginPopupContextVoid(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1) { return API->BeginPopupContextVoid(str_id, popup_flags); }
		inline bool IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0) { return API->IsPopupOpen(str_id, flags); }

		// Tables
		inline bool BeginTable(const char* str_id, int columns, ImGuiTableFlags flags = 0, const ImVec2 outer_size = ImVec2(0.0f, 0.0f), float inner_width = 0.0f) { return API->BeginTable(str_id, columns, flags, outer_size, inner_width); }
		inline void EndTable() { API->EndTable(); }
		inline void TableNextRow(ImGuiTableRowFlags row_flags = 0, float min_row_height = 0.0f) { API->TableNextRow(row_flags, min_row_height); }
		inline bool TableNextColumn() { return API->TableNextColumn(); }
		inline bool TableSetColumnIndex(int column_n) { return API->TableSetColumnIndex(column_n); }

		// Tables: Headers + Columns declaration
		inline void TableSetupColumn(const char* label, ImGuiTableColumnFlags flags = 0, float init_width_or_weight = 0.0f, ImGuiID user_id = 0) { API->TableSetupColumn(label, flags, init_width_or_weight, user_id); }
		inline void TableSetupScrollFreeze(int cols, int rows) { API->TableSetupScrollFreeze(cols, rows); }
		inline void TableHeader(const char* label) { API->TableHeader(label); }
		inline void TableHeadersRow() { API->TableHeadersRow(); }
		inline void TableAngledHeadersRow() { API->TableAngledHeadersRow(); }

		// Tables: Sorting + Miscellaneous functions
		inline ImGuiTableSortSpecs* TableGetSortSpecs() { return API->TableGetSortSpecs(); }
		inline int TableGetColumnCount() { return API->TableGetColumnCount(); }
		inline int TableGetColumnIndex() { return API->TableGetColumnIndex(); }
		inline int TableGetRowIndex() { return API->TableGetRowIndex(); }
		inline const char* TableGetColumnName(int column_n = -1) { return API->TableGetColumnName(column_n); }
		inline ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1) { return API->TableGetColumnFlags(column_n); }
		inline void TableSetColumnEnabled(int column_n, bool v) { API->TableSetColumnEnabled(column_n, v); }
		inline int TableGetHoveredColumn() { return API->TableGetHoveredColumn(); }
		inline void TableSetBgColor(ImGuiTableBgTarget target, ImU32 color, int column_n = -1) { API->TableSetBgColor(target, color, column_n); }
		inline void Columns(int count = 1, const char* id = NULL, bool borders = true) { API->Columns(count, id, borders); }
		inline void NextColumn() { API->NextColumn(); }
		inline int GetColumnIndex() { return API->GetColumnIndex(); }
		inline float GetColumnWidth(int column_index = -1) { return API->GetColumnWidth(column_index); }
		inline void SetColumnWidth(int column_index, float width) { API->SetColumnWidth(column_index, width); }
		inline float GetColumnOffset(int column_index = -1) { return API->GetColumnOffset(column_index); }
		inline void SetColumnOffset(int column_index, float offset_x) { API->SetColumnOffset(column_index, offset_x); }
		inline int GetColumnsCount() { return API->GetColumnsCount(); }

		// MultiSelect
		inline ImGuiMultiSelectIO* BeginMultiSelect(ImGuiMultiSelectFlags flags, int selection_size, int items_count) { return API->BeginMultiSelect(flags, selection_size, items_count); }
		inline ImGuiMultiSelectIO* EndMultiSelect() { return API->EndMultiSelect(); }
		inline void SetNextItemSelectionUserData(ImGuiSelectionUserData selection_user_data) { API->SetNextItemSelectionUserData(selection_user_data); }
		inline bool IsItemToggledSelection() { return API->IsItemToggledSelection(); }

		// Tab Bars, Tabs
		inline bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0) { return API->BeginTabBar(str_id, flags); }
		inline void EndTabBar() { API->EndTabBar(); }
		inline bool BeginTabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0) { return API->BeginTabItem(label, p_open, flags); }
		inline void EndTabItem() { API->EndTabItem(); }
		inline bool TabItemButton(const char* label, ImGuiTabItemFlags flags = 0) { return API->TabItemButton(label, flags); }
		inline void SetTabItemClosed(const char* tab_or_docked_window_label) { API->SetTabItemClosed(tab_or_docked_window_label); }

		// Docking
		inline ImGuiID DockSpace(ImGuiID dockspace_id, const ImVec2 size = ImVec2(0, 0), int flags = 0, const void* window_class = NULL) { return API->DockSpace(dockspace_id, size, flags, window_class); }
		inline void SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond = 0) { API->SetNextWindowDockID(dock_id, cond); }
		inline ImGuiID GetWindowDockID() { return API->GetWindowDockID(); }
		inline bool IsWindowDocked() { return API->IsWindowDocked(); }

		// Drag and Drop
		inline bool BeginDragDropSource(ImGuiDragDropFlags flags = 0) { return API->BeginDragDropSource(flags); }
		inline bool SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond = 0) { return API->SetDragDropPayload(type, data, sz, cond); }
		inline void EndDragDropSource() { API->EndDragDropSource(); }
		inline bool BeginDragDropTarget() { return API->BeginDragDropTarget(); }
		inline void EndDragDropTarget() { API->EndDragDropTarget(); }
		inline const ImGuiPayload* GetDragDropPayload() { return API->GetDragDropPayload(); }

		// Disabling
		inline void BeginDisabled(bool disabled = true) { API->BeginDisabled(disabled); }
		inline void EndDisabled() { API->EndDisabled(); }

		// Clipping
		inline void PushClipRect(const ImVec2 clip_rect_min, const ImVec2 clip_rect_max, bool intersect_with_current_clip_rect) { API->PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect); }
		inline void PopClipRect() { API->PopClipRect(); }

		// Focus / Activation
		inline void SetItemDefaultFocus() { API->SetItemDefaultFocus(); }
		inline void SetKeyboardFocusHere(int offset = 0) { API->SetKeyboardFocusHere(offset); }

		// Item / Widgets Utilities
		inline bool IsItemHovered(ImGuiHoveredFlags flags = 0) { return API->IsItemHovered(flags); }
		inline bool IsItemActive() { return API->IsItemActive(); }
		inline bool IsItemFocused() { return API->IsItemFocused(); }
		inline bool IsItemClicked(ImGuiMouseButton mouse_button = 0) { return API->IsItemClicked(mouse_button); }
		inline bool IsItemVisible() { return API->IsItemVisible(); }
		inline bool IsItemEdited() { return API->IsItemEdited(); }
		inline bool IsItemActivated() { return API->IsItemActivated(); }
		inline bool IsItemDeactivated() { return API->IsItemDeactivated(); }
		inline bool IsItemDeactivatedAfterEdit() { return API->IsItemDeactivatedAfterEdit(); }
		inline bool IsItemToggledOpen() { return API->IsItemToggledOpen(); }
		inline bool IsAnyItemHovered() { return API->IsAnyItemHovered(); }
		inline bool IsAnyItemActive() { return API->IsAnyItemActive(); }
		inline bool IsAnyItemFocused() { return API->IsAnyItemFocused(); }
		inline ImGuiID GetItemID() { return API->GetItemID(); }
		inline ImVec2 GetItemRectMin() { return API->GetItemRectMin(); }
		inline ImVec2 GetItemRectMax() { return API->GetItemRectMax(); }
		inline ImVec2 GetItemRectSize() { return API->GetItemRectSize(); }

		// Logging/Capture
		inline void LogToTTY(int auto_open_depth = -1) { API->LogToTTY(auto_open_depth); }
		inline void LogToFile(int auto_open_depth = -1, const char* filename = NULL) { API->LogToFile(auto_open_depth, filename); }
		inline void LogToClipboard(int auto_open_depth = -1) { API->LogToClipboard(auto_open_depth); }
		inline void LogFinish() { API->LogFinish(); }
		inline void LogButtons() { API->LogButtons(); }
		inline void LogText(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			API->LogTextV(fmt, args);
			va_end(args);
		}
		inline void LogTextV(const char* fmt, va_list args) { API->LogTextV(fmt, args); }

		// Miscellaneous Utilities
		inline bool IsRectVisible(const ImVec2 size) { return API->IsRectVisible(size); }
		inline bool IsRectVisible(const ImVec2 rect_min, const ImVec2 rect_max) { return API->IsRectVisibleVec(rect_min, rect_max); }
		inline double GetTime() { return API->GetTime(); }
		inline int GetFrameCount() { return API->GetFrameCount(); }
		inline const char* GetStyleColorName(ImGuiCol idx) { return API->GetStyleColorName(idx); }
		inline ImDrawListSharedData* GetDrawListSharedData() { return API->GetDrawListSharedData(); }
		inline void SetStateStorage(ImGuiStorage* storage) { API->SetStateStorage(storage); }
		inline ImGuiStorage* GetStateStorage() { return API->GetStateStorage(); }

		// Mouse
		inline bool IsMouseDown(ImGuiMouseButton button) { return API->IsMouseDown(button); }
		inline bool IsMouseClicked(ImGuiMouseButton button, bool repeat) { return API->IsMouseClicked(button, repeat); }
		inline bool IsMouseReleased(ImGuiMouseButton button) { return API->IsMouseReleased(button); }
		inline bool IsMouseDoubleClicked(ImGuiMouseButton button) { return API->IsMouseDoubleClicked(button); }
		inline bool IsMouseReleasedWithDelay(ImGuiMouseButton button, float delay) { return API->IsMouseReleasedWithDelay(button, delay); }
		inline int GetMouseClickedCount(ImGuiMouseButton button) { return API->GetMouseClickedCount(button); }
		inline bool IsMouseHoveringRect(const ImVec2 r_min, const ImVec2 r_max, bool clip = true) { return API->IsMouseHoveringRect(r_min, r_max, clip); }
		inline bool IsMousePosValid(const ImVec2* mouse_pos) { return API->IsMousePosValid(mouse_pos); }
		inline bool IsAnyMouseDown() { return API->IsAnyMouseDown(); }
		inline ImVec2 GetMousePos() { return API->GetMousePos(); }
		inline ImVec2 GetMousePosOnOpeningCurrentPopup() { return API->GetMousePosOnOpeningCurrentPopup(); }
		inline bool IsMouseDragging(ImGuiMouseButton button, float lock_threshold) { return API->IsMouseDragging(button, lock_threshold); }
		inline ImVec2 GetMouseDragDelta(ImGuiMouseButton button, float lock_threshold) { return API->GetMouseDragDelta(button, lock_threshold); }
		inline void ResetMouseDragDelta(ImGuiMouseButton button) { API->ResetMouseDragDelta(button); }
		inline ImGuiMouseCursor GetMouseCursor() { return API->GetMouseCursor(); }
		inline void SetMouseCursor(ImGuiMouseCursor cursor_type) { API->SetMouseCursor(cursor_type); }
		inline void SetNextFrameWantCaptureMouse(bool want_capture_mouse) { API->SetNextFrameWantCaptureMouse(want_capture_mouse); }

		// Clipboard
		inline const char* GetClipboardText() { return API->GetClipboardText(); }
		inline void SetClipboardText(const char* text) { API->SetClipboardText(text); }

		// KeyOwnership
		inline bool SetItemKeyOwner(ImGuiKey key) { return API->SetItemKeyOwner(key); }

		// Shortcut
		inline bool Shortcut(ImGuiKeyChord key_chord, ImGuiInputFlags flags) { return API->Shortcut(key_chord, flags); }
		inline void SetNextItemShortcut(ImGuiKeyChord key_chord, ImGuiInputFlags flags) { API->SetNextItemShortcut(key_chord, flags); }

		// Inputs
		inline bool IsKeyDown(ImGuiKey key) { return API->IsKeyDown(key); }
		inline bool IsKeyPressed(ImGuiKey key, bool repeat = true) { return API->IsKeyPressed(key, repeat); }
		inline bool IsKeyReleased(ImGuiKey key) { return API->IsKeyReleased(key); }
		inline bool IsKeyChordPressed(ImGuiKeyChord key_chord) { return API->IsKeyChordPressed(key_chord); }
		inline int GetKeyPressedAmount(ImGuiKey key, float repeat_delay, float rate) { return API->GetKeyPressedAmount(key, repeat_delay, rate); }
		inline const char* GetKeyName(ImGuiKey key) { return API->GetKeyName(key); }
		inline void SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard) { API->SetNextFrameWantCaptureKeyboard(want_capture_keyboard); }

		// Text Utilities
		inline ImVec2 CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f)
		{
			return API->CalcTextSize(text, text_end, hide_text_after_double_hash, wrap_width);
		}

		// Color Utilities
		inline ImVec4 ColorConvertU32ToFloat4(ImU32 in) { return API->ColorConvertU32ToFloat4(in); }
		inline ImU32 ColorConvertFloat4ToU32(ImVec4 in) { return API->ColorConvertFloat4ToU32(in); }
		inline void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v) { return API->ColorConvertRGBtoHSV(r, g, b, &out_h, &out_s, &out_v); }
		inline void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b) { return API->ColorConvertHSVtoRGB(h, s, v, &out_r, &out_g, &out_b); }

		// imgui_internal.h
		inline ImGuiWindow* GetCurrentWindow() { return API->GetCurrentWindow(); }
		inline ImGuiWindow* FindWindowByID(ImGuiID id) { return API->FindWindowByID(id); }
		inline ImGuiWindow* FindWindowByName(const char* name) { return API->FindWindowByName(name); }
		inline void UpdateWindowParentAndRootLinks(ImGuiWindow* window, ImGuiWindowFlags flags, ImGuiWindow* parent_window) { API->UpdateWindowParentAndRootLinks(window, flags, parent_window); }
		inline void UpdateWindowSkipRefresh(ImGuiWindow* window) { API->UpdateWindowSkipRefresh(window); }
		inline ImVec2 CalcWindowNextAutoFitSize(ImGuiWindow* window) { return API->CalcWindowNextAutoFitSize(window); }
		inline bool IsWindowChildOf(ImGuiWindow* window, ImGuiWindow* potential_parent, bool popup_hierarchy) { return API->IsWindowChildOf(window, potential_parent, popup_hierarchy); }
		inline bool IsWindowInBeginStack(ImGuiWindow* window) { return API->IsWindowInBeginStack(window); }
		inline bool IsWindowWithinBeginStackOf(ImGuiWindow* window, ImGuiWindow* potential_parent) { return API->IsWindowWithinBeginStackOf(window, potential_parent); }
		inline bool IsWindowAbove(ImGuiWindow* potential_above, ImGuiWindow* potential_below) { return API->IsWindowAbove(potential_above, potential_below); }
		inline bool IsWindowNavFocusable(ImGuiWindow* window) { return API->IsWindowNavFocusable(window); }
		inline void SetWindowPos(ImGuiWindow* window, const ImVec2 pos, ImGuiCond cond) { API->SetWindowPosGlob(window, pos, cond); }
		inline void SetWindowSize(ImGuiWindow* window, const ImVec2 size, ImGuiCond cond) { API->SetWindowSizeGlob(window, size, cond); }
		inline void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond) { API->SetWindowCollapsedGlob(window, collapsed, cond); }
		inline void SetWindowHitTestHole(ImGuiWindow* window, const ImVec2 pos, const ImVec2 size) { API->SetWindowHitTestHole(window, pos, size); }
		inline void SetWindowHiddenAndSkipItemsForCurrentFrame(ImGuiWindow* window) { API->SetWindowHiddenAndSkipItemsForCurrentFrame(window); }
		inline void FocusWindow(ImGuiWindow* window, ImGuiFocusRequestFlags flags) { API->FocusWindow(window, flags); }
		inline void FocusTopMostWindowUnderOne(ImGuiWindow* under_this_window, ImGuiWindow* ignore_window, ImGuiViewport* filter_viewport, ImGuiFocusRequestFlags flags) { API->FocusTopMostWindowUnderOne(under_this_window, ignore_window, filter_viewport, flags); }
		inline void BringWindowToFocusFront(ImGuiWindow* window) { API->BringWindowToFocusFront(window); }
		inline void BringWindowToDisplayFront(ImGuiWindow* window) { API->BringWindowToDisplayFront(window); }
		inline void BringWindowToDisplayBack(ImGuiWindow* window) { API->BringWindowToDisplayBack(window); }
		inline void BringWindowToDisplayBehind(ImGuiWindow* window, ImGuiWindow* above_window) { API->BringWindowToDisplayBehind(window, above_window); }
		inline int FindWindowDisplayIndex(ImGuiWindow* window) { return API->FindWindowDisplayIndex(window); }
		inline ImGuiWindow* FindBottomMostVisibleWindowWithinBeginStack(ImGuiWindow* window) { return API->FindBottomMostVisibleWindowWithinBeginStack(window); }
		inline void SetNextWindowRefreshPolicy(ImGuiWindowRefreshFlags flags) { API->SetNextWindowRefreshPolicy(flags); }
		inline void RegisterUserTexture(ImTextureData* tex) { API->RegisterUserTexture(tex); }
		inline void UnregisterUserTexture(ImTextureData* tex) { API->UnregisterUserTexture(tex); }
		inline void RegisterFontAtlas(ImFontAtlas* atlas) { API->RegisterFontAtlas(atlas); }
		inline void UnregisterFontAtlas(ImFontAtlas* atlas) { API->UnregisterFontAtlas(atlas); }
		inline void SetCurrentFont(ImFont* font, float font_size_before_scaling, float font_size_after_scaling) { API->SetCurrentFont(font, font_size_before_scaling, font_size_after_scaling); }
		inline void UpdateCurrentFontSize(float restore_font_size_after_scaling) { API->UpdateCurrentFontSize(restore_font_size_after_scaling); }
		inline void SetFontRasterizerDensity(float rasterizer_density) { API->SetFontRasterizerDensity(rasterizer_density); }
		inline ImFont* GetDefaultFont() { return API->GetDefaultFont(); }
		inline void PushPasswordFont() { API->PushPasswordFont(); }
		inline void PopPasswordFont() { API->PopPasswordFont(); }
		inline ImDrawList* GetBackgroundDrawList(ImGuiViewport* viewport) { return (ImDrawList*)API->GetBackgroundDrawList(viewport); }
		inline ImDrawList* GetForegroundDrawList(ImGuiViewport* viewport) { return (ImDrawList*)API->GetForegroundDrawList(viewport); }
		//inline void AddDrawListToDrawDataEx(ImDrawData* draw_data, ImVector<ImDrawList*>* out_list, ImDrawList* draw_list) { API->AddDrawListToDrawDataEx(draw_data, (ImDrawList_Opaque*)out_list, (ImDrawList_Opaque*)draw_list); }
		inline void Initialize() { API->Initialize(); }
		inline void Shutdown() { API->Shutdown(); }
		inline void SetContextName(ImGuiContext* ctx, const char* name) { API->SetContextName(ctx, name); }
		inline ImGuiID AddContextHook(ImGuiContext* ctx, const ImGuiContextHook* hook) { return API->AddContextHook(ctx, hook); }
		inline void RemoveContextHook(ImGuiContext* ctx, ImGuiID hook_to_remove) { API->RemoveContextHook(ctx, hook_to_remove); }
		inline void CallContextHooks(ImGuiContext* ctx, ImGuiContextHookType type) { API->CallContextHooks(ctx, type); }
		inline void UpdateInputEvents(bool trickle_fast_inputs) { API->UpdateInputEvents(trickle_fast_inputs); }
		inline void UpdateHoveredWindowAndCaptureFlags(const ImVec2 mouse_pos) { API->UpdateHoveredWindowAndCaptureFlags(mouse_pos); }
		inline void FindHoveredWindowEx(const ImVec2 pos, bool find_first_and_in_any_viewport, ImGuiWindow** out_hovered_window, ImGuiWindow** out_hovered_window_under_moving_window) { API->FindHoveredWindowEx(pos, find_first_and_in_any_viewport, out_hovered_window, out_hovered_window_under_moving_window); }
		inline void StartMouseMovingWindow(ImGuiWindow* window) { API->StartMouseMovingWindow(window); }
		inline void StopMouseMovingWindow() { API->StopMouseMovingWindow(); }
		inline void UpdateMouseMovingWindowNewFrame() { API->UpdateMouseMovingWindowNewFrame(); }
		inline void UpdateMouseMovingWindowEndFrame() { API->UpdateMouseMovingWindowEndFrame(); }
		inline void ScaleWindowsInViewport(ImGuiViewportP* viewport, float scale) { API->ScaleWindowsInViewport(viewport, scale); }
		inline void SetWindowViewport(ImGuiWindow* window, ImGuiViewportP* viewport) { API->SetWindowViewport(window, viewport); }
		inline void MarkIniSettingsDirty() { API->MarkIniSettingsDirty(); }
		inline void MarkIniSettingsDirtyWindow(ImGuiWindow* window) { API->MarkIniSettingsDirtyWindow(window); }
		inline void ClearIniSettings() { API->ClearIniSettings(); }
		inline void AddSettingsHandler(const ImGuiSettingsHandler* handler) { API->AddSettingsHandler(handler); }
		inline void RemoveSettingsHandler(const char* type_name) { API->RemoveSettingsHandler(type_name); }
		inline ImGuiSettingsHandler* FindSettingsHandler(const char* type_name) { return API->FindSettingsHandler(type_name); }
		inline ImGuiWindowSettings* CreateNewWindowSettings(const char* name) { return API->CreateNewWindowSettings(name); }
		inline ImGuiWindowSettings* FindWindowSettingsByID(ImGuiID id) { return API->FindWindowSettingsByID(id); }
		inline ImGuiWindowSettings* FindWindowSettingsByWindow(ImGuiWindow* window) { return API->FindWindowSettingsByWindow(window); }
		inline void ClearWindowSettings(const char* name) { API->ClearWindowSettings(name); }
		inline void LocalizeRegisterEntries(const ImGuiLocEntry* entries, int count) { API->LocalizeRegisterEntries(entries, count); }
		inline void SetScrollX(ImGuiWindow* window, float scroll_x) { API->SetScrollXGlob(window, scroll_x); }
		inline void SetScrollY(ImGuiWindow* window, float scroll_y) { API->SetScrollYGlob(window, scroll_y); }
		inline void SetScrollFromPosX(ImGuiWindow* window, float local_x, float center_x_ratio) { API->SetScrollFromPosXGlob(window, local_x, center_x_ratio); }
		inline void SetScrollFromPosY(ImGuiWindow* window, float local_y, float center_y_ratio) { API->SetScrollFromPosYGlob(window, local_y, center_y_ratio); }
		inline void ScrollToItem(ImGuiScrollFlags flags) { API->ScrollToItem(flags); }
		inline void ScrollToRect(ImGuiWindow* window, const ImRect rect, ImGuiScrollFlags flags) { API->ScrollToRect(window, rect, flags); }
		inline ImVec2 ScrollToRectEx(ImGuiWindow* window, const ImRect rect, ImGuiScrollFlags flags) { return API->ScrollToRectEx(window, rect, flags); }
		inline void SetActiveID(ImGuiID id, ImGuiWindow* window) { API->SetActiveID(id, window); }
		inline void SetFocusID(ImGuiID id, ImGuiWindow* window) { API->SetFocusID(id, window); }
		inline void ClearActiveID() { API->ClearActiveID(); }
		inline ImGuiID GetHoveredID() { return API->GetHoveredID(); }
		inline void SetHoveredID(ImGuiID id) { API->SetHoveredID(id); }
		inline void KeepAliveID(ImGuiID id) { API->KeepAliveID(id); }
		inline void MarkItemEdited(ImGuiID id) { API->MarkItemEdited(id); }
		inline void PushOverrideID(ImGuiID id) { API->PushOverrideID(id); }
		inline ImGuiID GetIDWithSeed(const char* str_id_begin, const char* str_id_end, ImGuiID seed) { return API->GetIDWithSeed(str_id_begin, str_id_end, seed); }
		inline ImGuiID GetIDWithSeedInt(int n, ImGuiID seed) { return API->GetIDWithSeedInt(n, seed); }
		inline void ItemSize(const ImVec2 size, float text_baseline_y = -1.0f) { API->ItemSize(size, text_baseline_y); }
		inline void ItemSize(const ImRect size, float text_baseline_y = -1.0f) { API->ItemSizeImRect(size, text_baseline_y); }
		inline bool ItemAdd(const ImRect bb, ImGuiID id, const ImRect* nav_bb = NULL, ImGuiItemFlags extra_flags = 0) { return API->ItemAdd(bb, id, nav_bb, extra_flags); }
		inline bool ItemHoverable(const ImRect bb, ImGuiID id, ImGuiItemFlags item_flags) { return API->ItemHoverable(bb, id, item_flags); }
		inline bool IsWindowContentHoverable(ImGuiWindow* window, ImGuiHoveredFlags flags) { return API->IsWindowContentHoverable(window, flags); }
		inline bool IsClippedEx(const ImRect bb, ImGuiID id) { return API->IsClippedEx(bb, id); }
		inline void SetLastItemData(ImGuiID item_id, ImGuiItemFlags item_flags, ImGuiItemStatusFlags status_flags, const ImRect item_rect) { API->SetLastItemData(item_id, item_flags, status_flags, item_rect); }
		inline ImVec2 CalcItemSize(ImVec2 size, float default_w, float default_h) { return API->CalcItemSize(size, default_w, default_h); }
		inline float CalcWrapWidthForPos(const ImVec2 pos, float wrap_pos_x) { return API->CalcWrapWidthForPos(pos, wrap_pos_x); }
		inline void PushMultiItemsWidths(int components, float width_full) { API->PushMultiItemsWidths(components, width_full); }
		inline void ShrinkWidths(ImGuiShrinkWidthItem* items, int count, float width_excess, float width_min) { API->ShrinkWidths(items, count, width_excess, width_min); }
		inline void CalcClipRectVisibleItemsY(const ImRect clip_rect, const ImVec2 pos, float items_height, int* out_visible_start, int* out_visible_end) { API->CalcClipRectVisibleItemsY(clip_rect, pos, items_height, out_visible_start, out_visible_end); }
		inline const ImGuiStyleVarInfo* GetStyleVarInfo(ImGuiStyleVar idx) { return API->GetStyleVarInfo(idx); }
		inline void BeginDisabledOverrideReenable() { API->BeginDisabledOverrideReenable(); }
		inline void EndDisabledOverrideReenable() { API->EndDisabledOverrideReenable(); }
		inline void LogBegin(ImGuiLogFlags flags, int auto_open_depth) { API->LogBegin(flags, auto_open_depth); }
		inline void LogToBuffer(int auto_open_depth = -1) { API->LogToBuffer(auto_open_depth); }
		inline void LogRenderedText(const ImVec2* ref_pos, const char* text, const char* text_end = NULL) { API->LogRenderedText(ref_pos, text, text_end); }
		inline void LogSetNextTextDecoration(const char* prefix, const char* suffix) { API->LogSetNextTextDecoration(prefix, suffix); }
		inline bool BeginChildEx(const char* name, ImGuiID id, const ImVec2 size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags) { return API->BeginChildEx(name, id, size_arg, child_flags, window_flags); }
		inline ImGuiWindow* FindFrontMostVisibleChildWindow(ImGuiWindow* window) { return API->FindFrontMostVisibleChildWindow(window); }
		inline bool BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_window_flags) { return API->BeginPopupEx(id, extra_window_flags); }
		inline bool BeginPopupMenuEx(ImGuiID id, const char* label, ImGuiWindowFlags extra_window_flags) { return API->BeginPopupMenuEx(id, label, extra_window_flags); }
		inline void OpenPopupEx(ImGuiID id, ImGuiPopupFlags popup_flags = ImGuiPopupFlags_None) { API->OpenPopupEx(id, popup_flags); }
		inline void ClosePopupToLevel(int remaining, bool restore_focus_to_window_under_popup) { API->ClosePopupToLevel(remaining, restore_focus_to_window_under_popup); }
		inline void ClosePopupsOverWindow(ImGuiWindow* ref_window, bool restore_focus_to_window_under_popup) { API->ClosePopupsOverWindow(ref_window, restore_focus_to_window_under_popup); }
		inline void ClosePopupsExceptModals() { API->ClosePopupsExceptModals(); }
		inline bool IsPopupOpen(ImGuiID id, ImGuiPopupFlags popup_flags) { return API->IsPopupOpenID(id, popup_flags); }
		inline ImRect GetPopupAllowedExtentRect(ImGuiWindow* window) { return API->GetPopupAllowedExtentRect(window); }
		inline ImGuiWindow* GetTopMostPopupModal() { return API->GetTopMostPopupModal(); }
		inline ImGuiWindow* GetTopMostAndVisiblePopupModal() { return API->GetTopMostAndVisiblePopupModal(); }
		inline ImGuiWindow* FindBlockingModal(ImGuiWindow* window) { return API->FindBlockingModal(window); }
		inline ImVec2 FindBestWindowPosForPopup(ImGuiWindow* window) { return API->FindBestWindowPosForPopup(window); }
		inline ImVec2 FindBestWindowPosForPopupEx(const ImVec2 ref_pos, const ImVec2 size, ImGuiDir* last_dir, const ImRect r_outer, const ImRect r_avoid, ImGuiPopupPositionPolicy policy) { return API->FindBestWindowPosForPopupEx(ref_pos, size, last_dir, r_outer, r_avoid, policy); }
		inline ImGuiMouseButton GetMouseButtonFromPopupFlags(ImGuiPopupFlags flags) { return API->GetMouseButtonFromPopupFlags(flags); }
		inline bool IsPopupOpenRequestForItem(ImGuiPopupFlags flags, ImGuiID id) { return API->IsPopupOpenRequestForItem(flags, id); }
		inline bool IsPopupOpenRequestForWindow(ImGuiPopupFlags flags) { return API->IsPopupOpenRequestForWindow(flags); }
		inline bool BeginTooltipEx(ImGuiTooltipFlags tooltip_flags, ImGuiWindowFlags extra_window_flags) { return API->BeginTooltipEx(tooltip_flags, extra_window_flags); }
		inline bool BeginTooltipHidden() { return API->BeginTooltipHidden(); }
		inline bool BeginViewportSideBar(const char* name, ImGuiViewport* viewport, ImGuiDir dir, float size, ImGuiWindowFlags window_flags) { return API->BeginViewportSideBar(name, viewport, dir, size, window_flags); }
		inline bool BeginMenuEx(const char* label, const char* icon, bool enabled = true) { return API->BeginMenuEx(label, icon, enabled); }
		inline bool MenuItemEx(const char* label, const char* icon, const char* shortcut = NULL, bool selected = false, bool enabled = true) { return API->MenuItemEx(label, icon, shortcut, selected, enabled); }
		inline bool BeginComboPopup(ImGuiID popup_id, const ImRect bb, ImGuiComboFlags flags) { return API->BeginComboPopup(popup_id, bb, flags); }
		inline bool BeginComboPreview() { return API->BeginComboPreview(); }
		inline void EndComboPreview() { API->EndComboPreview(); }
		inline void NavInitWindow(ImGuiWindow* window, bool force_reinit) { API->NavInitWindow(window, force_reinit); }
		inline void NavInitRequestApplyResult() { API->NavInitRequestApplyResult(); }
		inline bool NavMoveRequestButNoResultYet() { return API->NavMoveRequestButNoResultYet(); }
		inline void NavMoveRequestSubmit(ImGuiDir move_dir, ImGuiDir clip_dir, ImGuiNavMoveFlags move_flags, ImGuiScrollFlags scroll_flags) { API->NavMoveRequestSubmit(move_dir, clip_dir, move_flags, scroll_flags); }
		inline void NavMoveRequestForward(ImGuiDir move_dir, ImGuiDir clip_dir, ImGuiNavMoveFlags move_flags, ImGuiScrollFlags scroll_flags) { API->NavMoveRequestForward(move_dir, clip_dir, move_flags, scroll_flags); }
		inline void NavMoveRequestResolveWithLastItem(ImGuiNavItemData* result) { API->NavMoveRequestResolveWithLastItem(result); }
		inline void NavMoveRequestResolveWithPastTreeNode(ImGuiNavItemData* result, const ImGuiTreeNodeStackData* tree_node_data) { API->NavMoveRequestResolveWithPastTreeNode(result, tree_node_data); }
		inline void NavMoveRequestCancel() { API->NavMoveRequestCancel(); }
		inline void NavMoveRequestApplyResult() { API->NavMoveRequestApplyResult(); }
		inline void NavMoveRequestTryWrapping(ImGuiWindow* window, ImGuiNavMoveFlags move_flags) { API->NavMoveRequestTryWrapping(window, move_flags); }
		inline void NavHighlightActivated(ImGuiID id) { API->NavHighlightActivated(id); }
		inline void NavClearPreferredPosForAxis(ImGuiAxis axis) { API->NavClearPreferredPosForAxis(axis); }
		inline void SetNavCursorVisibleAfterMove() { API->SetNavCursorVisibleAfterMove(); }
		inline void NavUpdateCurrentWindowIsScrollPushableX() { API->NavUpdateCurrentWindowIsScrollPushableX(); }
		inline void SetNavWindow(ImGuiWindow* window) { API->SetNavWindow(window); }
		inline void SetNavID(ImGuiID id, ImGuiNavLayer nav_layer, ImGuiID focus_scope_id, const ImRect rect_rel) { API->SetNavID(id, nav_layer, focus_scope_id, rect_rel); }
		inline void SetNavFocusScope(ImGuiID focus_scope_id) { API->SetNavFocusScope(focus_scope_id); }
		inline void FocusItem() { API->FocusItem(); }
		inline void ActivateItemByID(ImGuiID id) { API->ActivateItemByID(id); }
		inline ImGuiKeyData* GetKeyData(ImGuiContext* ctx, ImGuiKey key) { return API->GetKeyData(ctx, key); }
		inline const char* GetKeyChordName(ImGuiKeyChord key_chord) { return API->GetKeyChordName(key_chord); }
		inline bool IsMouseDragPastThreshold(ImGuiMouseButton button, float lock_threshold) { return API->IsMouseDragPastThreshold(button, lock_threshold); }
		inline ImVec2 GetKeyMagnitude2d(ImGuiKey key_left, ImGuiKey key_right, ImGuiKey key_up, ImGuiKey key_down) { return API->GetKeyMagnitude2d(key_left, key_right, key_up, key_down); }
		inline float GetNavTweakPressedAmount(ImGuiAxis axis) { return API->GetNavTweakPressedAmount(axis); }
		inline int CalcTypematicRepeatAmount(float t0, float t1, float repeat_delay, float repeat_rate) { return API->CalcTypematicRepeatAmount(t0, t1, repeat_delay, repeat_rate); }
		inline void GetTypematicRepeatRate(ImGuiInputFlags flags, float* repeat_delay, float* repeat_rate) { API->GetTypematicRepeatRate(flags, repeat_delay, repeat_rate); }
		inline void TeleportMousePos(const ImVec2 pos) { API->TeleportMousePos(pos); }
		inline void SetActiveIdUsingAllKeyboardKeys() { API->SetActiveIdUsingAllKeyboardKeys(); }
		inline ImGuiID GetKeyOwner(ImGuiKey key) { return API->GetKeyOwner(key); }
		inline void SetKeyOwner(ImGuiKey key, ImGuiID owner_id, ImGuiInputFlags flags) { API->SetKeyOwner(key, owner_id, flags); }
		inline void SetKeyOwnersForKeyChord(ImGuiKeyChord key, ImGuiID owner_id, ImGuiInputFlags flags) { API->SetKeyOwnersForKeyChord(key, owner_id, flags); }
		inline bool SetItemKeyOwner(ImGuiKey key, ImGuiInputFlags flags) { return API->SetItemKeyOwnerOwner(key, flags); }
		inline bool TestKeyOwner(ImGuiKey key, ImGuiID owner_id) { return API->TestKeyOwner(key, owner_id); }
		inline bool IsKeyDown(ImGuiKey key, ImGuiID owner_id) { return API->IsKeyDownOwner(key, owner_id); }
		inline bool IsKeyPressed(ImGuiKey key, ImGuiInputFlags flags, ImGuiID owner_id) { return API->IsKeyPressedOwner(key, flags, owner_id); }
		inline bool IsKeyReleased(ImGuiKey key, ImGuiID owner_id) { return API->IsKeyReleasedOwner(key, owner_id); }
		inline bool IsKeyChordPressed(ImGuiKeyChord key_chord, ImGuiInputFlags flags, ImGuiID owner_id) { return API->IsKeyChordPressedOwner(key_chord, flags, owner_id); }
		inline bool IsMouseDown(ImGuiMouseButton button, ImGuiID owner_id) { return API->IsMouseDownOwner(button, owner_id); }
		inline bool IsMouseClicked(ImGuiMouseButton button, ImGuiInputFlags flags, ImGuiID owner_id) { return API->IsMouseClickedOwner(button, flags, owner_id); }
		inline bool IsMouseReleased(ImGuiMouseButton button, ImGuiID owner_id) { return API->IsMouseReleasedOwner(button, owner_id); }
		inline bool IsMouseDoubleClicked(ImGuiMouseButton button, ImGuiID owner_id) { return API->IsMouseDoubleClickedOwner(button, owner_id); }
		inline bool Shortcut(ImGuiKeyChord key_chord, ImGuiInputFlags flags, ImGuiID owner_id) { return API->ShortcutOwner(key_chord, flags, owner_id); }
		inline bool SetShortcutRouting(ImGuiKeyChord key_chord, ImGuiInputFlags flags, ImGuiID owner_id) { return API->SetShortcutRoutingOwner(key_chord, flags, owner_id); }
		inline bool TestShortcutRouting(ImGuiKeyChord key_chord, ImGuiID owner_id) { return API->TestShortcutRoutingOwner(key_chord, owner_id); }
		inline ImGuiKeyRoutingData* GetShortcutRoutingData(ImGuiKeyChord key_chord) { return API->GetShortcutRoutingData(key_chord); }
		inline void PushFocusScope(ImGuiID id) { API->PushFocusScope(id); }
		inline void PopFocusScope() { API->PopFocusScope(); }
		inline bool IsInNavFocusRoute(ImGuiID focus_scope_id) { return API->IsInNavFocusRoute(focus_scope_id); }
		inline bool IsDragDropActive() { return API->IsDragDropActive(); }
		inline bool BeginDragDropTargetCustom(const ImRect bb, ImGuiID id) { return API->BeginDragDropTargetCustom(bb, id); }
		inline bool BeginDragDropTargetViewport(ImGuiViewport* viewport, const ImRect* p_bb) { return API->BeginDragDropTargetViewport(viewport, p_bb); }
		inline void ClearDragDrop() { API->ClearDragDrop(); }
		inline bool IsDragDropPayloadBeingAccepted() { return API->IsDragDropPayloadBeingAccepted(); }
		inline void RenderDragDropTargetRectForItem(const ImRect bb) { API->RenderDragDropTargetRectForItem(bb); }
		inline void RenderDragDropTargetRectEx(ImDrawList* draw_list, const ImRect bb, float rounding) { API->RenderDragDropTargetRectEx((ImDrawList_Opaque*)draw_list, bb, rounding); }
		inline ImGuiTypingSelectRequest* GetTypingSelectRequest(ImGuiTypingSelectFlags flags = ImGuiTypingSelectFlags_None) { return API->GetTypingSelectRequest(flags); }
		inline int TypingSelectFindMatch(ImGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data, int nav_item_idx) { return API->TypingSelectFindMatch(req, items_count, get_item_name_func, user_data, nav_item_idx); }
		inline int TypingSelectFindNextSingleCharMatch(ImGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data, int nav_item_idx) { return API->TypingSelectFindNextSingleCharMatch(req, items_count, get_item_name_func, user_data, nav_item_idx); }
		inline int TypingSelectFindBestLeadingMatch(ImGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data) { return API->TypingSelectFindBestLeadingMatch(req, items_count, get_item_name_func, user_data); }
		inline bool BeginBoxSelect(const ImRect scope_rect, ImGuiWindow* window, ImGuiID box_select_id, ImGuiMultiSelectFlags ms_flags) { return API->BeginBoxSelect(scope_rect, window, box_select_id, ms_flags); }
		inline void EndBoxSelect(const ImRect scope_rect, ImGuiMultiSelectFlags ms_flags) { API->EndBoxSelect(scope_rect, ms_flags); }
		inline void MultiSelectItemHeader(ImGuiID id, bool* p_selected, ImGuiButtonFlags* p_button_flags) { API->MultiSelectItemHeader(id, p_selected, p_button_flags); }
		inline void MultiSelectItemFooter(ImGuiID id, bool* p_selected, bool* p_pressed) { API->MultiSelectItemFooter(id, p_selected, p_pressed); }
		inline void MultiSelectAddSetAll(ImGuiMultiSelectTempData* ms, bool selected) { API->MultiSelectAddSetAll(ms, selected); }
		inline void MultiSelectAddSetRange(ImGuiMultiSelectTempData* ms, bool selected, int range_dir, ImGuiSelectionUserData first_item, ImGuiSelectionUserData last_item) { API->MultiSelectAddSetRange(ms, selected, range_dir, first_item, last_item); }
		inline void SetWindowClipRectBeforeSetChannel(ImGuiWindow* window, const ImRect clip_rect) { API->SetWindowClipRectBeforeSetChannel(window, clip_rect); }
		inline void BeginColumns(const char* str_id, int count, ImGuiOldColumnFlags flags = 0) { API->BeginColumns(str_id, count, flags); }
		inline void EndColumns() { API->EndColumns(); }
		inline void PushColumnClipRect(int column_index) { API->PushColumnClipRect(column_index); }
		inline void PushColumnsBackground() { API->PushColumnsBackground(); }
		inline void PopColumnsBackground() { API->PopColumnsBackground(); }
		inline ImGuiID GetColumnsID(const char* str_id, int count) { return API->GetColumnsID(str_id, count); }
		inline ImGuiOldColumns* FindOrCreateColumns(ImGuiWindow* window, ImGuiID id) { return API->FindOrCreateColumns(window, id); }
		inline float GetColumnOffsetFromNorm(const ImGuiOldColumns* columns, float offset_norm) { return API->GetColumnOffsetFromNorm(columns, offset_norm); }
		inline float GetColumnNormFromOffset(const ImGuiOldColumns* columns, float offset) { return API->GetColumnNormFromOffset(columns, offset); }
		inline void TableOpenContextMenu(int column_n) { API->TableOpenContextMenu(column_n = -1); }
		inline void TableSetColumnWidth(int column_n, float width) { API->TableSetColumnWidth(column_n, width); }
		inline void TableSetColumnSortDirection(int column_n, ImGuiSortDirection sort_direction, bool append_to_sort_specs) { API->TableSetColumnSortDirection(column_n, sort_direction, append_to_sort_specs); }
		inline int TableGetHoveredRow() { return API->TableGetHoveredRow(); }
		inline float TableGetHeaderRowHeight() { return API->TableGetHeaderRowHeight(); }
		inline float TableGetHeaderAngledMaxLabelWidth() { return API->TableGetHeaderAngledMaxLabelWidth(); }
		inline void TablePushBackgroundChannel() { API->TablePushBackgroundChannel(); }
		inline void TablePopBackgroundChannel() { API->TablePopBackgroundChannel(); }
		inline void TablePushColumnChannel(int column_n) { API->TablePushColumnChannel(column_n); }
		inline void TablePopColumnChannel() { API->TablePopColumnChannel(); }
		inline void TableAngledHeadersRowEx(ImGuiID row_id, float angle, float max_label_width, const ImGuiTableHeaderData* data, int data_count) { API->TableAngledHeadersRowEx(row_id, angle, max_label_width, data, data_count); }
		inline ImGuiTable* TableFindByID(ImGuiID id) { return API->TableFindByID(id); }
		inline bool BeginTableEx(const char* name, ImGuiID id, int columns_count, ImGuiTableFlags flags = 0, const ImVec2 outer_size = ImVec2(0, 0), float inner_width = 0.0f) { return API->BeginTableEx(name, id, columns_count, flags, outer_size, inner_width); }
		inline void TableBeginInitMemory(ImGuiTable* table, int columns_count) { API->TableBeginInitMemory(table, columns_count); }
		inline void TableBeginApplyRequests(ImGuiTable* table) { API->TableBeginApplyRequests(table); }
		inline void TableSetupDrawChannels(ImGuiTable* table) { API->TableSetupDrawChannels(table); }
		inline void TableUpdateLayout(ImGuiTable* table) { API->TableUpdateLayout(table); }
		inline void TableUpdateBorders(ImGuiTable* table) { API->TableUpdateBorders(table); }
		inline void TableUpdateColumnsWeightFromWidth(ImGuiTable* table) { API->TableUpdateColumnsWeightFromWidth(table); }
		inline void TableApplyExternalUnclipRect(ImGuiTable* table, ImRect rect) { API->TableApplyExternalUnclipRect(table, rect); }
		inline void TableDrawBorders(ImGuiTable* table) { API->TableDrawBorders(table); }
		inline void TableDrawDefaultContextMenu(ImGuiTable* table, ImGuiTableFlags flags_for_section_to_display) { API->TableDrawDefaultContextMenu(table, flags_for_section_to_display); }
		inline bool TableBeginContextMenuPopup(ImGuiTable* table) { return API->TableBeginContextMenuPopup(table); }
		inline void TableMergeDrawChannels(ImGuiTable* table) { API->TableMergeDrawChannels(table); }
		inline void TableFixDisplayOrder(ImGuiTable* table) { API->TableFixDisplayOrder(table); }
		inline void TableSortSpecsSanitize(ImGuiTable* table) { API->TableSortSpecsSanitize(table); }
		inline void TableSortSpecsBuild(ImGuiTable* table) { API->TableSortSpecsBuild(table); }
		inline ImGuiSortDirection TableGetColumnNextSortDirection(ImGuiTableColumn* column) { return API->TableGetColumnNextSortDirection(column); }
		inline void TableFixColumnSortDirection(ImGuiTable* table, ImGuiTableColumn* column) { API->TableFixColumnSortDirection(table, column); }
		inline float TableGetColumnWidthAuto(ImGuiTable* table, ImGuiTableColumn* column) { return API->TableGetColumnWidthAuto(table, column); }
		inline void TableBeginRow(ImGuiTable* table) { API->TableBeginRow(table); }
		inline void TableEndRow(ImGuiTable* table) { API->TableEndRow(table); }
		inline void TableBeginCell(ImGuiTable* table, int column_n) { API->TableBeginCell(table, column_n); }
		inline void TableEndCell(ImGuiTable* table) { API->TableEndCell(table); }
		inline ImRect TableGetCellBgRect(const ImGuiTable* table, int column_n) { return API->TableGetCellBgRect(table, column_n); }
		inline const char* TableGetColumnName(const ImGuiTable* table, int column_n) { return API->TableGetColumnNameGlob(table, column_n); }
		inline ImGuiID TableGetColumnResizeID(ImGuiTable* table, int column_n, int instance_no = 0) { return API->TableGetColumnResizeID(table, column_n, instance_no); }
		inline float TableCalcMaxColumnWidth(const ImGuiTable* table, int column_n) { return API->TableCalcMaxColumnWidth(table, column_n); }
		inline void TableSetColumnWidthAutoSingle(ImGuiTable* table, int column_n) { API->TableSetColumnWidthAutoSingle(table, column_n); }
		inline void TableSetColumnWidthAutoAll(ImGuiTable* table) { API->TableSetColumnWidthAutoAll(table); }
		inline void TableSetColumnDisplayOrder(ImGuiTable* table, int column_n, int dst_order) { API->TableSetColumnDisplayOrder(table, column_n, dst_order); }
		inline void TableQueueSetColumnDisplayOrder(ImGuiTable* table, int column_n, int dst_order) { API->TableQueueSetColumnDisplayOrder(table, column_n, dst_order); }
		inline void TableRemove(ImGuiTable* table) { API->TableRemove(table); }
		inline void TableGcCompactTransientBuffers(ImGuiTable* table) { API->TableGcCompactTransientBuffers(table); }
		inline void TableGcCompactTransientBuffersTemp(ImGuiTableTempData* table) { API->TableGcCompactTransientBuffersTemp(table); }
		inline void TableGcCompactSettings() { API->TableGcCompactSettings(); }
		inline void TableLoadSettings(ImGuiTable* table) { API->TableLoadSettings(table); }
		inline void TableSaveSettings(ImGuiTable* table) { API->TableSaveSettings(table); }
		inline void TableResetSettings(ImGuiTable* table) { API->TableResetSettings(table); }
		inline ImGuiTableSettings* TableGetBoundSettings(ImGuiTable* table) { return API->TableGetBoundSettings(table); }
		inline void TableSettingsAddSettingsHandler() { API->TableSettingsAddSettingsHandler(); }
		inline ImGuiTableSettings* TableSettingsCreate(ImGuiID id, int columns_count) { return API->TableSettingsCreate(id, columns_count); }
		inline ImGuiTableSettings* TableSettingsFindByID(ImGuiID id) { return API->TableSettingsFindByID(id); }
		inline ImGuiTabBar* TabBarFindByID(ImGuiID id) { return API->TabBarFindByID(id); }
		inline void TabBarRemove(ImGuiTabBar* tab_bar) { API->TabBarRemove(tab_bar); }
		inline bool BeginTabBarEx(ImGuiTabBar* tab_bar, const ImRect bb, ImGuiTabBarFlags flags) { return API->BeginTabBarEx(tab_bar, bb, flags); }
		inline ImGuiTabItem* TabBarFindTabByID(ImGuiTabBar* tab_bar, ImGuiID tab_id) { return API->TabBarFindTabByID(tab_bar, tab_id); }
		inline ImGuiTabItem* TabBarFindTabByOrder(ImGuiTabBar* tab_bar, int order) { return API->TabBarFindTabByOrder(tab_bar, order); }
		inline ImGuiTabItem* TabBarGetCurrentTab(ImGuiTabBar* tab_bar) { return API->TabBarGetCurrentTab(tab_bar); }
		inline const char* TabBarGetTabName(ImGuiTabBar* tab_bar, ImGuiTabItem* tab) { return API->TabBarGetTabName(tab_bar, tab); }
		inline void TabBarRemoveTab(ImGuiTabBar* tab_bar, ImGuiID tab_id) { API->TabBarRemoveTab(tab_bar, tab_id); }
		inline void TabBarCloseTab(ImGuiTabBar* tab_bar, ImGuiTabItem* tab) { API->TabBarCloseTab(tab_bar, tab); }
		inline void TabBarQueueFocus(ImGuiTabBar* tab_bar, ImGuiTabItem* tab) { API->TabBarQueueFocus(tab_bar, tab); }
		inline void TabBarQueueFocusByName(ImGuiTabBar* tab_bar, const char* tab_name) { API->TabBarQueueFocusByName(tab_bar, tab_name); }
		inline void TabBarQueueReorder(ImGuiTabBar* tab_bar, ImGuiTabItem* tab, int offset) { API->TabBarQueueReorder(tab_bar, tab, offset); }
		inline void TabBarQueueReorderFromMousePos(ImGuiTabBar* tab_bar, ImGuiTabItem* tab, ImVec2 mouse_pos) { API->TabBarQueueReorderFromMousePos(tab_bar, tab, mouse_pos); }
		inline bool TabBarProcessReorder(ImGuiTabBar* tab_bar) { return API->TabBarProcessReorder(tab_bar); }
		inline bool TabItemEx(ImGuiTabBar* tab_bar, const char* label, bool* p_open, ImGuiTabItemFlags flags, ImGuiWindow* docked_window) { return API->TabItemEx(tab_bar, label, p_open, flags, docked_window); }
		inline void TabItemSpacing(const char* str_id, ImGuiTabItemFlags flags, float width) { API->TabItemSpacing(str_id, flags, width); }
		inline ImVec2 TabItemCalcSize(const char* label, bool has_close_button_or_unsaved_marker) { return API->TabItemCalcSize(label, has_close_button_or_unsaved_marker); }
		inline ImVec2 TabItemCalcSizeWindow(ImGuiWindow* window) { return API->TabItemCalcSizeWindow(window); }
		inline void TabItemBackground(ImDrawList* draw_list, const ImRect bb, ImGuiTabItemFlags flags, ImU32 col) { API->TabItemBackground((ImDrawList_Opaque*)draw_list, bb, flags, col); }
		inline void TabItemLabelAndCloseButton(ImDrawList* draw_list, const ImRect bb, ImGuiTabItemFlags flags, ImVec2 frame_padding, const char* label, ImGuiID tab_id, ImGuiID close_button_id, bool is_contents_visible, bool* out_just_closed, bool* out_text_clipped) { API->TabItemLabelAndCloseButton((ImDrawList_Opaque*)draw_list, bb, flags, frame_padding, label, tab_id, close_button_id, is_contents_visible, out_just_closed, out_text_clipped); }
		inline void RenderText(ImVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true) { API->RenderText(pos, text, text_end, hide_text_after_hash); }
		inline void RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width) { API->RenderTextWrapped(pos, text, text_end, wrap_width); }
		inline void RenderTextClipped(const ImVec2 pos_min, const ImVec2 pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2 align = ImVec2(0, 0), const ImRect* clip_rect = NULL) { API->RenderTextClipped(pos_min, pos_max, text, text_end, text_size_if_known, align, clip_rect); }
		inline void RenderTextClippedEx(ImDrawList* draw_list, const ImVec2 pos_min, const ImVec2 pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2 align = ImVec2(0, 0), const ImRect* clip_rect = NULL) { API->RenderTextClippedEx((ImDrawList_Opaque*)draw_list, pos_min, pos_max, text, text_end, text_size_if_known, align, clip_rect); }
		inline void RenderTextEllipsis(ImDrawList* draw_list, const ImVec2 pos_min, const ImVec2 pos_max, float ellipsis_max_x, const char* text, const char* text_end, const ImVec2* text_size_if_known) { API->RenderTextEllipsis((ImDrawList_Opaque*)draw_list, pos_min, pos_max, ellipsis_max_x, text, text_end, text_size_if_known); }
		inline void RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool borders = true, float rounding = 0.0f) { API->RenderFrame(p_min, p_max, fill_col, borders, rounding); }
		inline void RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding = 0.0f) { API->RenderFrameBorder(p_min, p_max, rounding); }
		inline void RenderColorComponentMarker(const ImRect bb, ImU32 col, float rounding) { API->RenderColorComponentMarker(bb, col, rounding); }
		inline void RenderColorRectWithAlphaCheckerboard(ImDrawList* draw_list, ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float grid_step, ImVec2 grid_off, float rounding = 0.0f, ImDrawFlags flags = 0) { API->RenderColorRectWithAlphaCheckerboard((ImDrawList_Opaque*)draw_list, p_min, p_max, fill_col, grid_step, grid_off, rounding, flags); }
		inline void RenderNavCursor(const ImRect bb, ImGuiID id, ImGuiNavRenderCursorFlags flags = ImGuiNavRenderCursorFlags_None) { API->RenderNavCursor(bb, id, flags); }
		inline const char* FindRenderedTextEnd(const char* text, const char* text_end = NULL) { return API->FindRenderedTextEnd(text, text_end); }
		inline void RenderMouseCursor(ImVec2 pos, float scale, ImGuiMouseCursor mouse_cursor, ImU32 col_fill, ImU32 col_border, ImU32 col_shadow) { API->RenderMouseCursor(pos, scale, mouse_cursor, col_fill, col_border, col_shadow); }
		inline void RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale = 1.0f) { API->RenderArrow((ImDrawList_Opaque*)draw_list, pos, col, dir, scale); }
		inline void RenderBullet(ImDrawList* draw_list, ImVec2 pos, ImU32 col) { API->RenderBullet((ImDrawList_Opaque*)draw_list, pos, col); }
		inline void RenderCheckMark(ImDrawList* draw_list, ImVec2 pos, ImU32 col, float sz) { API->RenderCheckMark((ImDrawList_Opaque*)draw_list, pos, col, sz); }
		inline void RenderArrowPointingAt(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col) { API->RenderArrowPointingAt((ImDrawList_Opaque*)draw_list, pos, half_sz, direction, col); }
		inline void RenderRectFilledInRangeH(ImDrawList* draw_list, const ImRect rect, ImU32 col, float fill_x0, float fill_x1, float rounding) { API->RenderRectFilledInRangeH((ImDrawList_Opaque*)draw_list, rect, col, fill_x0, fill_x1, rounding); }
		inline void RenderRectFilledWithHole(ImDrawList* draw_list, const ImRect outer, const ImRect inner, ImU32 col, float rounding) { API->RenderRectFilledWithHole((ImDrawList_Opaque*)draw_list, outer, inner, col, rounding); }
		inline ImDrawFlags CalcRoundingFlagsForRectInRect(const ImRect r_in, const ImRect r_outer, float threshold) { return API->CalcRoundingFlagsForRectInRect(r_in, r_outer, threshold); }
		inline void TextEx(const char* text, const char* text_end = NULL, ImGuiTextFlags flags = 0) { API->TextEx(text, text_end, flags); }
		inline bool ButtonEx(const char* label, const ImVec2 size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0) { return API->ButtonEx(label, size_arg, flags); }
		inline bool ArrowButtonEx(const char* str_id, ImGuiDir dir, ImVec2 size_arg, ImGuiButtonFlags flags = 0) { return API->ArrowButtonEx(str_id, dir, size_arg, flags); }
		inline bool ImageButtonEx(ImGuiID id, ImTextureRef tex_ref, const ImVec2 image_size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 bg_col, const ImVec4 tint_col, ImGuiButtonFlags flags = 0) { return API->ImageButtonEx(id, tex_ref, image_size, uv0, uv1, bg_col, tint_col, flags); }
		inline void SeparatorEx(ImGuiSeparatorFlags flags, float thickness = 1.0f) { API->SeparatorEx(flags, thickness); }
		inline void SeparatorTextEx(ImGuiID id, const char* label, const char* label_end, float extra_width) { API->SeparatorTextEx(id, label, label_end, extra_width); }
		inline bool CheckboxFlagsS64(const char* label, ImS64* flags, ImS64 flags_value) { return API->CheckboxFlagsS64(label, flags, flags_value); }
		inline bool CheckboxFlagsU64(const char* label, ImU64* flags, ImU64 flags_value) { return API->CheckboxFlagsU64(label, flags, flags_value); }
		inline bool CloseButton(ImGuiID id, const ImVec2 pos) { return API->CloseButton(id, pos); }
		inline bool CollapseButton(ImGuiID id, const ImVec2 pos) { return API->CollapseButton(id, pos); }
		inline void Scrollbar(ImGuiAxis axis) { API->Scrollbar(axis); }
		inline bool ScrollbarEx(const ImRect bb, ImGuiID id, ImGuiAxis axis, ImS64* p_scroll_v, ImS64 avail_v, ImS64 contents_v, ImDrawFlags draw_rounding_flags) { return API->ScrollbarEx(bb, id, axis, p_scroll_v, avail_v, contents_v, draw_rounding_flags); }
		inline ImRect GetWindowScrollbarRect(ImGuiWindow* window, ImGuiAxis axis) { return API->GetWindowScrollbarRect(window, axis); }
		inline ImGuiID GetWindowScrollbarID(ImGuiWindow* window, ImGuiAxis axis) { return API->GetWindowScrollbarID(window, axis); }
		inline ImGuiID GetWindowResizeCornerID(ImGuiWindow* window, int n) { return API->GetWindowResizeCornerID(window, n); }
		inline ImGuiID GetWindowResizeBorderID(ImGuiWindow* window, ImGuiDir dir) { return API->GetWindowResizeBorderID(window, dir); }
		inline void ExtendHitBoxWhenNearViewportEdge(ImGuiWindow* window, ImRect* bb, float threshold, ImGuiAxis axis) { API->ExtendHitBoxWhenNearViewportEdge(window, bb, threshold, axis); }
		inline bool ButtonBehavior(const ImRect bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags = 0) { return API->ButtonBehavior(bb, id, out_hovered, out_held, flags); }
		inline bool DragBehavior(ImGuiID id, ImGuiDataType data_type, void* p_v, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags) { return API->DragBehavior(id, data_type, p_v, v_speed, p_min, p_max, format, flags); }
		inline bool SliderBehavior(const ImRect bb, ImGuiID id, ImGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb) { return API->SliderBehavior(bb, id, data_type, p_v, p_min, p_max, format, flags, out_grab_bb); }
		inline bool SplitterBehavior(const ImRect bb, ImGuiID id, ImGuiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend = 0.0f, float hover_visibility_delay = 0.0f, ImU32 bg_col = 0) { return API->SplitterBehavior(bb, id, axis, size1, size2, min_size1, min_size2, hover_extend, hover_visibility_delay, bg_col); }
		inline bool TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end = NULL) { return API->TreeNodeBehavior(id, flags, label, label_end); }
		inline void TreeNodeDrawLineToChildNode(const ImVec2 target_pos) { API->TreeNodeDrawLineToChildNode(target_pos); }
		inline void TreeNodeDrawLineToTreePop(const ImGuiTreeNodeStackData* data) { API->TreeNodeDrawLineToTreePop(data); }
		inline void TreePushOverrideID(ImGuiID id) { API->TreePushOverrideID(id); }
		inline void TreeNodeSetOpen(ImGuiID storage_id, bool open) { API->TreeNodeSetOpen(storage_id, open); }
		inline bool TreeNodeUpdateNextOpen(ImGuiID storage_id, ImGuiTreeNodeFlags flags) { return API->TreeNodeUpdateNextOpen(storage_id, flags); }
		inline const ImGuiDataTypeInfo* DataTypeGetInfo(ImGuiDataType data_type) { return API->DataTypeGetInfo(data_type); }
		inline int DataTypeFormatString(char* buf, int buf_size, ImGuiDataType data_type, const void* p_data, const char* format) { return API->DataTypeFormatString(buf, buf_size, data_type, p_data, format); }
		inline void DataTypeApplyOp(ImGuiDataType data_type, int op, void* output, const void* arg_1, const void* arg_2) { API->DataTypeApplyOp(data_type, op, output, arg_1, arg_2); }
		inline bool DataTypeApplyFromText(const char* buf, ImGuiDataType data_type, void* p_data, const char* format, void* p_data_when_empty) { return API->DataTypeApplyFromText(buf, data_type, p_data, format, p_data_when_empty); }
		inline int DataTypeCompare(ImGuiDataType data_type, const void* arg_1, const void* arg_2) { return API->DataTypeCompare(data_type, arg_1, arg_2); }
		inline bool DataTypeClamp(ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max) { return API->DataTypeClamp(data_type, p_data, p_min, p_max); }
		inline bool DataTypeIsZero(ImGuiDataType data_type, const void* p_data) { return API->DataTypeIsZero(data_type, p_data); }
		inline bool InputTextEx(const char* label, const char* hint, char* buf, int buf_size, const ImVec2 size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = NULL, void* user_data = NULL) { return API->InputTextEx(label, hint, buf, buf_size, size_arg, flags, callback, user_data); }
		inline void InputTextDeactivateHook(ImGuiID id) { API->InputTextDeactivateHook(id); }
		inline bool TempInputText(const ImRect bb, ImGuiID id, const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL) { return API->TempInputText(bb, id, label, buf, buf_size, flags, callback, user_data); }
		inline bool TempInputScalar(const ImRect bb, ImGuiID id, const char* label, ImGuiDataType data_type, void* p_data, const char* format, const void* p_clamp_min = NULL, const void* p_clamp_max = NULL) { return API->TempInputScalar(bb, id, label, data_type, p_data, format, p_clamp_min, p_clamp_max); }
		inline void ColorTooltip(const char* text, const float* col, ImGuiColorEditFlags flags) { API->ColorTooltip(text, col, flags); }
		inline void ColorEditOptionsPopup(const float* col, ImGuiColorEditFlags flags) { API->ColorEditOptionsPopup(col, flags); }
		inline void ColorPickerOptionsPopup(const float* ref_col, ImGuiColorEditFlags flags) { API->ColorPickerOptionsPopup(ref_col, flags); }
		inline int PlotEx(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void*, int), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, const ImVec2 size_arg) { return API->PlotEx(plot_type, label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, size_arg); }
		inline void ShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1) { API->ShadeVertsLinearColorGradientKeepAlpha((ImDrawList_Opaque*)draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col0, col1); }
		inline void ShadeVertsLinearUV(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2 a, const ImVec2 b, const ImVec2 uv_a, const ImVec2 uv_b, bool clamp) { API->ShadeVertsLinearUV((ImDrawList_Opaque*)draw_list, vert_start_idx, vert_end_idx, a, b, uv_a, uv_b, clamp); }
		inline void ShadeVertsTransformPos(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2 pivot_in, float cos_a, float sin_a, const ImVec2 pivot_out) { API->ShadeVertsTransformPos((ImDrawList_Opaque*)draw_list, vert_start_idx, vert_end_idx, pivot_in, cos_a, sin_a, pivot_out); }
		inline void GcCompactTransientMiscBuffers() { API->GcCompactTransientMiscBuffers(); }
		inline void GcCompactTransientWindowBuffers(ImGuiWindow* window) { API->GcCompactTransientWindowBuffers(window); }
		inline void GcAwakeTransientWindowBuffers(ImGuiWindow* window) { API->GcAwakeTransientWindowBuffers(window); }
		inline bool ErrorLog(const char* msg) { return API->ErrorLog(msg); }
		inline void ErrorRecoveryStoreState(ImGuiErrorRecoveryState* state_out) { API->ErrorRecoveryStoreState(state_out); }
		inline void ErrorRecoveryTryToRecoverState(const ImGuiErrorRecoveryState* state_in) { API->ErrorRecoveryTryToRecoverState(state_in); }
		inline void ErrorRecoveryTryToRecoverWindowState(const ImGuiErrorRecoveryState* state_in) { API->ErrorRecoveryTryToRecoverWindowState(state_in); }
		inline void ErrorCheckUsingSetCursorPosToExtendParentBoundaries() { API->ErrorCheckUsingSetCursorPosToExtendParentBoundaries(); }
		inline void ErrorCheckEndFrameFinalizeErrorTooltip() { API->ErrorCheckEndFrameFinalizeErrorTooltip(); }
		inline bool BeginErrorTooltip() { return API->BeginErrorTooltip(); }
		inline void EndErrorTooltip() { API->EndErrorTooltip(); }
		inline void DemoMarker(const char* file, int line, const char* section) { API->DemoMarker(file, line, section); }
		inline void DebugAllocHook(ImGuiDebugAllocInfo* info, int frame_count, void* ptr, size_t size) { API->DebugAllocHook(info, frame_count, ptr, size); }
		inline void DebugDrawCursorPos(ImU32 col = IM_COL32(255, 0, 0, 255)) { API->DebugDrawCursorPos(col); }
		inline void DebugDrawLineExtents(ImU32 col = IM_COL32(255, 0, 0, 255)) { API->DebugDrawLineExtents(col); }
		inline void DebugDrawItemRect(ImU32 col = IM_COL32(255, 0, 0, 255)) { API->DebugDrawItemRect(col); }
		inline void DebugTextUnformattedWithLocateItem(const char* line_begin, const char* line_end) { API->DebugTextUnformattedWithLocateItem(line_begin, line_end); }
		inline void DebugLocateItem(ImGuiID target_id) { API->DebugLocateItem(target_id); }
		inline void DebugLocateItemOnHover(ImGuiID target_id) { API->DebugLocateItemOnHover(target_id); }
		inline void DebugLocateItemResolveWithLastItem() { API->DebugLocateItemResolveWithLastItem(); }
		inline void DebugBreakClearData() { API->DebugBreakClearData(); }
		inline bool DebugBreakButton(const char* label, const char* description_of_location) { return API->DebugBreakButton(label, description_of_location); }
		inline void DebugBreakButtonTooltip(bool keyboard_only, const char* description_of_location) { API->DebugBreakButtonTooltip(keyboard_only, description_of_location); }
		inline void ShowFontAtlas(ImFontAtlas* atlas) { API->ShowFontAtlas(atlas); }
		inline ImU64 DebugTextureIDToU64(ImTextureID tex_id) { return API->DebugTextureIDToU64(tex_id); }
		inline void DebugHookIdInfo(ImGuiID id, ImGuiDataType data_type, const void* data_id, const void* data_id_end) { API->DebugHookIdInfo(id, data_type, data_id, data_id_end); }
		inline void DebugNodeColumns(ImGuiOldColumns* columns) { API->DebugNodeColumns(columns); }
		inline void DebugNodeDrawList(ImGuiWindow* window, ImGuiViewportP* viewport, const ImDrawList* draw_list, const char* label) { API->DebugNodeDrawList(window, viewport, (ImDrawList_Opaque*)draw_list, label); }
		inline void DebugNodeDrawCmdShowMeshAndBoundingBox(ImDrawList* out_draw_list, const ImDrawList* draw_list, const ImDrawCmd* draw_cmd, bool show_mesh, bool show_aabb) { API->DebugNodeDrawCmdShowMeshAndBoundingBox((ImDrawList_Opaque*)out_draw_list, (ImDrawList_Opaque*)draw_list, draw_cmd, show_mesh, show_aabb); }
		inline void DebugNodeFont(ImFont* font) { API->DebugNodeFont(font); }
		inline void DebugNodeFontGlyphsForSrcMask(ImFont* font, ImFontBaked* baked, int src_mask) { API->DebugNodeFontGlyphsForSrcMask(font, baked, src_mask); }
		inline void DebugNodeFontGlyph(ImFont* font, const ImFontGlyph* glyph) { API->DebugNodeFontGlyph(font, glyph); }
		inline void DebugNodeTexture(ImTextureData* tex, int int_id, const ImFontAtlasRect* highlight_rect = NULL) { API->DebugNodeTexture(tex, int_id, highlight_rect); }
		inline void DebugNodeStorage(ImGuiStorage* storage, const char* label) { API->DebugNodeStorage(storage, label); }
		inline void DebugNodeTabBar(ImGuiTabBar* tab_bar, const char* label) { API->DebugNodeTabBar(tab_bar, label); }
		inline void DebugNodeTable(ImGuiTable* table) { API->DebugNodeTable(table); }
		inline void DebugNodeTableSettings(ImGuiTableSettings* settings) { API->DebugNodeTableSettings(settings); }
		inline void DebugNodeInputTextState(ImGuiInputTextState* state) { API->DebugNodeInputTextState(state); }
		inline void DebugNodeTypingSelectState(ImGuiTypingSelectState* state) { API->DebugNodeTypingSelectState(state); }
		inline void DebugNodeMultiSelectState(ImGuiMultiSelectState* state) { API->DebugNodeMultiSelectState(state); }
		inline void DebugNodeWindow(ImGuiWindow* window, const char* label) { API->DebugNodeWindow(window, label); }
		inline void DebugNodeWindowSettings(ImGuiWindowSettings* settings) { API->DebugNodeWindowSettings(settings); }
		inline void DebugNodeWindowsList(ImVector<ImGuiWindow*>* windows, const char* label) { API->DebugNodeWindowsList(windows, label); }
		inline void DebugNodeWindowsListByBeginStackParent(ImGuiWindow** windows, int windows_size, ImGuiWindow* parent_in_begin_stack) { API->DebugNodeWindowsListByBeginStackParent(windows, windows_size, parent_in_begin_stack); }
		inline void DebugNodeViewport(ImGuiViewportP* viewport) { API->DebugNodeViewport(viewport); }
		inline void DebugRenderKeyboardPreview(ImDrawList* draw_list) { API->DebugRenderKeyboardPreview((ImDrawList_Opaque*)draw_list); }
		inline void DebugRenderViewportThumbnail(ImDrawList* draw_list, ImGuiViewportP* viewport, const ImRect bb) { API->DebugRenderViewportThumbnail((ImDrawList_Opaque*)draw_list, viewport, bb); }

		namespace detail
		{
			inline Clipper* ImGuiListClipper_Create() { return API->internal_Create_Clipper(); }
			inline void ImGuiListClipper_Destroy(Clipper* clipper) { return API->internal_Destroy_Clipper(clipper); }

			// ImGuiWindow
			inline ImGuiID ImGuiWindow_GetID_Str(ImGuiWindow* self, const char* str, const char* str_end) { return API->ImGuiWindow_GetID_Str(self, str, str_end); }
			inline ImGuiID ImGuiWindow_GetID_Ptr(ImGuiWindow* self, const void* ptr) { return API->ImGuiWindow_GetID_Ptr(self, ptr); }
			inline ImGuiID ImGuiWindow_GetID_Int(ImGuiWindow* self, int n) { return API->ImGuiWindow_GetID_Int(self, n); }
			inline ImGuiID ImGuiWindow_GetIDFromPos(ImGuiWindow* self, const ImVec2 p_abs) { return API->ImGuiWindow_GetIDFromPos(self, p_abs); }
			inline ImGuiID ImGuiWindow_GetIDFromRectangle(ImGuiWindow* self, const ImRect r_abs) { return API->ImGuiWindow_GetIDFromRectangle(self, r_abs); }
		}

		// allocation

		inline void* MemAlloc(size_t size) { return API->MemAlloc(size); }
		inline void MemFree(void* ptr) { return API->MemFree(ptr); }
	}
}
#endif
