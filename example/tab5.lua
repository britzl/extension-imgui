local JEDI = { "Luke", "Obi-Wan", "Yoda" }
local SITH = { "Maul", "Vader", "Palpatine" }

return function(self)
	if imgui.tree_node("Jedi vs Sith") then
		imgui.text("Right click for some info")
		for i=1,#JEDI do
			imgui.tree_node(JEDI[i], imgui.TREENODE_LEAF + imgui.TREENODE_NO_TREE_PUSH_ON_OPEN)
			if imgui.begin_dragdrop_source(imgui.DROPFLAGS_SOURCEALLOWNULLID) then
				imgui.set_dragdrop_payload("jedi", JEDI[i])
				imgui.end_dragdrop_source()
			end
			if imgui.begin_popup_context_item("jedipop" .. i) then
				imgui.text("This is a Jedi")
				imgui.end_popup()
			end
		end
		for i=1,#SITH do
			imgui.tree_node(SITH[i], imgui.TREENODE_LEAF + imgui.TREENODE_NO_TREE_PUSH_ON_OPEN)
			if imgui.begin_dragdrop_source(imgui.DROPFLAGS_SOURCEALLOWNULLID) then
				imgui.set_dragdrop_payload("sith", SITH[i])
				imgui.end_dragdrop_source()
			end
			if imgui.begin_popup_context_item("sithpop" .. i) then
				imgui.text("This is a Sith")
				imgui.end_popup()
			end
		end
		imgui.tree_pop()
	end

	imgui.separator()

	imgui.text("Drop Jedi and Sith here")
	if imgui.begin_dragdrop_target() then
		if imgui.accept_dragdrop_payload("jedi") then
			self.counter = self.counter + 1
		elseif imgui.accept_dragdrop_payload("sith") then
			self.counter = self.counter - 1
		end
		imgui.end_dragdrop_target()
	end
	imgui.text("Counter " .. self.counter)

end
