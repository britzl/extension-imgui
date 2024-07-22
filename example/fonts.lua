local style = require("example.style")

local M = {}

local BASE_SIZE = 12

local fonts = {}

function M.load()
	fonts 	= {}

	local size = BASE_SIZE * style.get_scale()
	local fontsize = size
	local fontsizebase = size
	local regular_data = assert(sys.load_resource("/example/bundle/fonts/Montserrat-Regular.ttf"))
	fonts["Regular"] = imgui.font_add_ttf_data(regular_data, #regular_data, fontsize, fontsizebase, imgui.GLYPH_RANGES_CYRILLIC)
	local bold_data = assert(sys.load_resource("/example/bundle/fonts/Montserrat-Bold.ttf"))
	fonts["Bold"] = imgui.font_add_ttf_data(bold_data, #bold_data, fontsize, fontsizebase)
	local italic_data = assert(sys.load_resource("/example/bundle/fonts/Montserrat-Italic.ttf"))
	fonts["Italic"] = imgui.font_add_ttf_data(italic_data, #italic_data, fontsize, fontsizebase)
	local bolditalic_data = assert(sys.load_resource("/example/bundle/fonts/Montserrat-BoldItalic.ttf"))
	fonts["BoldItalic"] = imgui.font_add_ttf_data(bolditalic_data, #bolditalic_data, fontsize, fontsizebase)
end

function M.get(name)
	return fonts[name]
end

return M