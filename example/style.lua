local M = {}

local SCALE = 2

function M.set()
	--[[---@type Style
	local style = {}
	style.ChildBorderSize = 1
	style.PopupRounding = 0
	style.PopupBorderSize = 1
	style.FramePadding = vmath.vector3(4, 3, 0)
	style.FrameRounding = 0
	style.FrameBorderSize = 0
	style.ItemSpacing = vmath.vector3(8, 4, 0)
	style.ItemInnerSpacing = vmath.vector3(4, 4, 0)
	style.CellPadding = vmath.vector3(4, 2, 0)
	style.TouchExtraPadding = vmath.vector3(0, 0, 0)
	style.IndentSpacing = 21
	style.Alpha = 1
	style.ScrollbarSize = 14
	style.ScrollbarRounding = 9
	style.GrabMinSize = 10
	style.GrabRounding = 0
	style.LogSliderDeadzone = 4
	style.TabRounding = 4
	style.TabBorderSize = 0
	style.TabMinWidthForCloseButton = 0
	style.ColorButtonPosition = 1
	style.ButtonTextAlign = vmath.vector3(0.5, 0.5, 0)
	style.SelectableTextAlign = vmath.vector3(0, 0, 0)
	style.DisplayWindowPadding = vmath.vector3(19, 19, 0)
	style.DisplaySafeAreaPadding = vmath.vector3(3, 3, 0)
	style.MouseCursorScale = 1
	style.CircleSegmentMaxError = 1.6
	style.ColumnsMinSpacing = 6
	style.AntiAliasedLines = true
	style.AntiAliasedLinesUseTex = true
	style.AntiAliasedFill = true
	style.CurveTessellationTol = 1.25
	style.WindowPadding = vmath.vector3(8, 8, 0)
	style.WindowRounding = 0
	style.WindowBorderSize = 1
	style.WindowMinSize = vmath.vector3(32, 32, 0)
	style.WindowTitleAlign = vmath.vector3(0, 0.5, 0)
	style.WindowMenuButtonPosition = 0
	style.ChildRounding = 0
	imgui.set_style(style)--]]

	imgui.set_style_color(imgui.ImGuiCol_Text, 0.90, 0.90, 0.90, 0.90)
	imgui.set_style_color(imgui.ImGuiCol_TextDisabled, 0.60, 0.60, 0.60, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_WindowBg, 0.09, 0.09, 0.15, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_PopupBg, 0.05, 0.05, 0.10, 0.85)
	imgui.set_style_color(imgui.ImGuiCol_Border, 0.70, 0.70, 0.70, 0.65)
	imgui.set_style_color(imgui.ImGuiCol_BorderShadow, 0.00, 0.00, 0.00, 0.00)
	imgui.set_style_color(imgui.ImGuiCol_FrameBg, 0.00, 0.00, 0.01, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_FrameBgHovered, 0.90, 0.80, 0.80, 0.40)
	imgui.set_style_color(imgui.ImGuiCol_FrameBgActive, 0.90, 0.65, 0.65, 0.45)
	imgui.set_style_color(imgui.ImGuiCol_TitleBg, 0.00, 0.00, 0.00, 0.83)
	imgui.set_style_color(imgui.ImGuiCol_TitleBgCollapsed, 0.40, 0.40, 0.80, 0.20)
	imgui.set_style_color(imgui.ImGuiCol_TitleBgActive, 0.00, 0.00, 0.00, 0.87)
	imgui.set_style_color(imgui.ImGuiCol_MenuBarBg, 0.01, 0.01, 0.02, 0.80)
	imgui.set_style_color(imgui.ImGuiCol_ScrollbarBg, 0.20, 0.25, 0.30, 0.60)
	imgui.set_style_color(imgui.ImGuiCol_ScrollbarGrab, 0.55, 0.53, 0.55, 0.51)
	imgui.set_style_color(imgui.ImGuiCol_ScrollbarGrabHovered, 0.56, 0.56, 0.56, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_ScrollbarGrabActive, 0.56, 0.56, 0.56, 0.91)
	imgui.set_style_color(imgui.ImGuiCol_CheckMark, 0.90, 0.90, 0.90, 0.83)
	imgui.set_style_color(imgui.ImGuiCol_SliderGrab, 0.70, 0.70, 0.70, 0.62)
	imgui.set_style_color(imgui.ImGuiCol_SliderGrabActive, 0.30, 0.30, 0.30, 0.84)
	imgui.set_style_color(imgui.ImGuiCol_Button, 0.48, 0.72, 0.89, 0.49)
	imgui.set_style_color(imgui.ImGuiCol_ButtonHovered, 0.50, 0.69, 0.99, 0.68)
	imgui.set_style_color(imgui.ImGuiCol_ButtonActive, 0.80, 0.50, 0.50, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_Header, 0.30, 0.69, 1.00, 0.53)
	imgui.set_style_color(imgui.ImGuiCol_HeaderHovered, 0.44, 0.61, 0.86, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_HeaderActive, 0.38, 0.62, 0.83, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_ResizeGrip, 1.00, 1.00, 1.00, 0.85)
	imgui.set_style_color(imgui.ImGuiCol_ResizeGripHovered, 1.00, 1.00, 1.00, 0.60)
	imgui.set_style_color(imgui.ImGuiCol_ResizeGripActive, 1.00, 1.00, 1.00, 0.90)
	imgui.set_style_color(imgui.ImGuiCol_PlotLines, 1.00, 1.00, 1.00, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_PlotLinesHovered, 0.90, 0.70, 0.00, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_PlotHistogram, 0.90, 0.70, 0.00, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_PlotHistogramHovered, 1.00, 0.60, 0.00, 1.00)
	imgui.set_style_color(imgui.ImGuiCol_TextSelectedBg, 0.00, 0.00, 1.00, 0.35)

	imgui.scale_all_sizes(SCALE)
end

function M.get()
	return imgui.get_style()
end

function M.get_scale()
	return SCALE
end

return M