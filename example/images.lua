local M = {}

local images = {}

function M.load()
	-- Resource based Image 
	local img1_data = assert(sys.load_resource("/example/bundle/images/image1.png"))
	images["image1"] = imgui.image_load_data("image1", img1_data, #img1_data)

	-- File based Image can be used on desktop platforms
	-- self.image1 = imgui.image_load("example/images/image1.png")

	local img2_data = assert(sys.load_resource("/example/bundle/images/image2.png"))
	images["image2"] = imgui.image_load_data("image2", img2_data, #img2_data)
end

function M.get(name)
	return images[name]
end

return M