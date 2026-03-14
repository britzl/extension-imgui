return function(self)
	if not self.tab6_f then self.tab6_f = 0 end

	imgui.text("SetNextItemWidth(200)")
	imgui.same_line()
	imgui.set_next_item_width(200)
	local changed, f = imgui.input_float("float##1", self.tab6_f)
	if changed then
		self.tab6_f = f
	end

	imgui.text("SetNextItemWidth(GetWindowWidth() * 0.5f)")
	imgui.same_line()
	local w,h = imgui.get_window_size()
	imgui.set_next_item_width(w * 0.5)
	local changed, f = imgui.input_float("float##2", self.tab6_f)
	if changed then
		self.tab6_f = f
	end

	imgui.text("SetNextItemWidth(GetContentRegionAvail().x * 0.5f)")
	imgui.same_line()
	local aw, ah = imgui.get_content_region_avail()
	imgui.set_next_item_width(aw * 0.5)
	local changed, f = imgui.input_float("float##3", self.tab6_f)
	if changed then
		self.tab6_f = f
	end

	imgui.text("SetNextItemWidth(-200)")
	imgui.same_line()
	imgui.set_next_item_width(-200)
	local changed, f = imgui.input_float("float##4", self.tab6_f)
	if changed then
		self.tab6_f = f
	end
end
