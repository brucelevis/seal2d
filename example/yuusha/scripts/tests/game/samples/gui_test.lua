local sprite = require "seal.sprite"
local gui_test = class("gui_test", function()
		return sprite.new_container()
	end)

local function attach_next_button(self)
	local button = require "seal.gui.button"

	local function on_click_next(btn)
		print("call on_click_next")
	end

	local btn_next = button.new {
									atlas = "ui.png",
									n = "tan_normal.png",
									s = "tan_selected.png",
									d = "tan_disable.png",
									callback = on_click_next,
									size = {w = 120, h = 74},
								}
	btn_next:set_pos(WINDOW_WIDTH/2, WINDOW_HEIGHT/2)
	self:add_child(btn_next)
end

function gui_test:ctor()
	attach_next_button(self)
end

return gui_test