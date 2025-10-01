local style = require("example.style")

return function(self)
	imgui.text("This is some useful text.")
	local changed, checked = imgui.checkbox("Demo window", self.show_demo_window)
	if changed then
		self.show_demo_window = checked
	end

	imgui.separator()

	if imgui.radio_button("Option 1", self.radio == 1) then
		self.radio  = 1
	end
	if imgui.radio_button("Option 2", self.radio == 2) then
		self.radio  = 2
	end
	if imgui.radio_button("Option 3", self.radio == 3) then
		self.radio  = 3
	end

	imgui.separator()

	if imgui.button("Reset Counter") then
		self.counter = 0
	end
	imgui.same_line()
	if imgui.button_arrow("Up", imgui.DIR_UP) then
		self.counter = self.counter + 1
	end
	imgui.same_line()
	if imgui.button_arrow("Down", imgui.DIR_DOWN) then
		self.counter = self.counter - 1
	end
	imgui.same_line()
	imgui.text(("counter = %d"):format(self.counter))

	imgui.separator()

	local pos = go.get_position("object")
	local changed, x, y, z = imgui.input_float3("pos", pos.x, pos.y, pos.z)
	if changed then
		pos.x = x
		pos.y = y
		pos.z = z
		go.set_position(pos, "object")
	end

	local changed, x, y, z = imgui.input_int4("posi", pos.x, pos.y, pos.z, 0)
	if changed then
		pos.x = x
		pos.y = y
		pos.z = z
		go.set_position(pos, "object")
	end

	local changed, int = imgui.input_int("test int", self.int or 0)
	if changed then
		self.int = int
	end

	if imgui.button("open window with close button") then
		self.show_close_window = true
	end

	if imgui.begin_table("table_example", 3) then
		imgui.table_setup_scroll_freeze(1, 1)
		imgui.table_setup_column("FIRST NAME")
		imgui.table_setup_column("LAST NAME")
		imgui.table_setup_column("AGE")
		imgui.table_headers_row()

		imgui.table_next_row()
		imgui.table_next_column()
		imgui.text("Bilbo")
		imgui.table_next_column()
		imgui.text("Baggins")
		imgui.table_next_column()
		imgui.text("129")

		imgui.table_next_row()
		imgui.table_next_column()
		imgui.text("Frodo")
		imgui.table_next_column()
		imgui.text("Baggins")
		imgui.table_next_column()
		imgui.text("51")

		imgui.end_table()
	end

	imgui.separator()

	-- Manual wrapping
	imgui.text("Manual wrapping:")

	local button_count = 20
	local button_size = 60
	local window_pos_x, window_pos_y = imgui.get_window_pos()
	local region_x, region_y = imgui.get_window_content_region_max()
	local window_visible_x = window_pos_x + region_x
	local spacing_x = style.get().ItemSpacing.x

	for n = 1, button_count do
		imgui.push_id(tostring(n))
		imgui.button("Box", button_size, button_size)
		local last_button_x, last_button_y = imgui.get_item_rect_max()
		local next_button_x = last_button_x + spacing_x + button_size
		if (n  < button_count and next_button_x < window_visible_x) then
			imgui.same_line()
		end
		imgui.pop_id()
	end
end
