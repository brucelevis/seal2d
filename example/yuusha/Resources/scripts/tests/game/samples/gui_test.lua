local sprite = require "seal.sprite"
local gui_test = class("gui_test", function()
		return sprite.new_container()
	end)

local function attach_next_button(self)
	local button = require "seal.gui.button"

	local function on_click_next(btn)
		print("call on_click_next")
	end

-----------------------------------  test simple mode
	if ENGINE_MODE == 1 then
		local s = sprite.new_attr{
			parent = self,
			atlas='ui.png', texture='smile_middle.png',
			x = WINDOW_WIDTH/2, y = WINDOW_HEIGHT/4,

			moved = function(sender, x, y, sx, sy)
				local cx, cy = sender:get_pos()
				sender:set_pos(cx + sx, cy + sy)
			end,
			
			effect_click = function(sender, x, y)
			
			end,

			{ fnt = 'res/fonts/animated.txt', text = 'click me or drag.' },
		}
	end
------------------------------------

	local btn_next = button.new {
									atlas = "ui.png",
									n = "tan_normal.png",
									s = "tan_selected.png",
									d = "tan_disable.png",
									callback = on_click_next,
									size = {w = 120, h = 74},
									text = 'next',
								}
	btn_next:set_pos(WINDOW_WIDTH/2, WINDOW_HEIGHT/2)
	self:add_child(btn_next)

	local scroll_view = require "seal.gui.scroll_view"
	local new_scroll = scroll_view.new {
    									}
	-- local s = sprite.new_bmfont_label("hello scroll view", "res/fonts/animated.txt")
	-- s:set_bbox_visible(true)
	-- new_scroll:add_content(s)

	local s = sprite.new("ui.png", "smile_middle.png")
    s:set_bbox_visible(true)
	new_scroll:add_content(s)

	local s = sprite.new("ui.png", "smile_middle.png")
    s:set_bbox_visible(true)
	new_scroll:add_content(s)

	local s = sprite.new("ui.png", "smile_middle.png")
    s:set_bbox_visible(true)
	new_scroll:add_content(s)

	local s = sprite.new("ui.png", "smile_middle.png")
    s:set_bbox_visible(true)
	new_scroll:add_content(s)

	local s = sprite.new("ui.png", "smile_middle.png")
    s:set_bbox_visible(true)
	new_scroll:add_content(s)

	new_scroll:set_pos(0, 0)
	--new_scroll:set_bbox_visible(true)
	self:add_child(new_scroll)

end

function gui_test:ctor()
	attach_next_button(self)
end

return gui_test