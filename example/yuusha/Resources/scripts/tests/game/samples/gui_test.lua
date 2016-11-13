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
        local ui_rect = require "seal.gui.simple.ui_rect"
        local ui_button = require "seal.gui.simple.ui_button"

        local menu = nil
        menu = sprite.new_attr{
            parent = self,
            x = WINDOW_WIDTH/2, y = WINDOW_HEIGHT/2,    
            ui_rect = true, w = 150, h = 500, fc = {204, 204, 204, 255},   
            { 
                scale9 = true, w = 150, h = 25, x = 0, y = 237.5, 
                color ={233, 80, 90, 255}, atlas="ui.png", texture="rect-full.png", 
                moved     = function(sender, x, y, sx, sy)
                    local cx, cy = menu:get_pos()
                    menu:set_pos(cx + sx, cy + sy)
                end,
                { fnt = 'res/fonts/animated.txt', text = 'tests:', y = 5, scale = 0.8 },
            },
            {
                scroll = true,
                x = -75, y = -250,
                view_size = { w = 150, h = 475 },
                { ui_button = true, w = 125, h = 25, { y = 4, fnt = 'res/fonts/animated.txt', scale = 0.8, text = 'tests:'} }, 
                { ui_button = true, w = 125, h = 25, { y = 4, fnt = 'res/fonts/animated.txt', scale = 0.8, text = 'tests:'} }, 
                { ui_button = true, w = 125, h = 25, { y = 4, fnt = 'res/fonts/animated.txt', scale = 0.8, text = 'tests:'} }, 
                { ui_button = true, w = 125, h = 25, { y = 4, fnt = 'res/fonts/animated.txt', scale = 0.8, text = 'tests:'} }, 
                { ui_button = true, w = 125, h = 25, { y = 4, fnt = 'res/fonts/animated.txt', scale = 0.8, text = 'tests:'} }, 
            },
            { ui_rect = true, w = 150, h = 500, oc = {0, 0, 0, 255}, }
        } 
        return 
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
    new_scroll:add_child(s)

    local s = sprite.new("ui.png", "smile_middle.png")
    s:set_bbox_visible(true)
    new_scroll:add_child(s)

    local s = sprite.new("ui.png", "smile_middle.png")
    s:set_bbox_visible(true)
    new_scroll:add_child(s)

    local s = sprite.new("ui.png", "smile_middle.png")
    s:set_bbox_visible(true)
    new_scroll:add_child(s)

    local s = sprite.new("ui.png", "smile_middle.png")
    s:set_bbox_visible(true)
    new_scroll:add_child(s)

    new_scroll:set_pos(0, 0)
    --new_scroll:set_bbox_visible(true)
    self:add_child(new_scroll)

end

function gui_test:ctor()
    attach_next_button(self)
end

return gui_test
