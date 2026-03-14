local values_line = {}
local values_hist = {}

local show_live_data = false

local function make_data()
	values_line = {}
	values_hist = {}
	for i=1, 60 do 
		local data = math.random(1, 30)
		values_hist[data] = (values_hist[data] or 0) + 1
		table.insert(values_line, data)
	end
end

make_data()

return function(self)
	local changed, checked = imgui.checkbox("Live Data", show_live_data)
	if changed then
		show_live_data = checked
	end

	if show_live_data == true then 
		make_data()
	end
	imgui.separator()

	imgui.text_colored(" Data Plot ", 1, 0, 0, 1 )
	imgui.plot_lines( "Data", 0, 150, 40, values_line )

	imgui.separator()

	imgui.text_colored(" Data Histogram ", 0, 1, 0, 1 )
	imgui.plot_histogram( "Histogram", 0, 150, 40, values_hist )

	imgui.separator()
end
