local hello_world = class("hello_world", function()
		return sprite.new_container()
	end)

function hello_world:ctor()
	local label = sprite.new_bmfont_label("hello seal2d :)",
										  "res/fonts/animated.txt")

	label:set_pos(WINDOW_WIDTH/2 - label:get_size()/2, WINDOW_HEIGHT/2)
	self:add_child(label)

	print("construct hello world finished, self = ", self)
	require("platform_core").__cmem()
end

return hello_world