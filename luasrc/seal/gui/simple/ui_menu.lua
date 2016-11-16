local sprite = require "seal.sprite"
local ui_rect = require "seal.gui.simple.ui_rect"

local ui_menu = class("ui_menu", function(w, h, menus)
    local font_scale = h / 35

    local text = sprite.new_attr{ 
        fnt = 'res/fonts/animated.txt', 
        text = '', 
        scale = font_scale,
        y = 5
    }

    local t_w, t_h = text:get_size()

    w = math.max(w, t_w)

    local button = sprite.new_attr{
        ui_rect = true, 
        w = w, h = h, fc = {233, 80, 90, 255},
        text,
    }

    local obj = sprite.new_attr{
        w = w, h = h,
        button,
    }
    
    --button:set_bbox_visible(true)

    obj.text = text
    obj.button = button
    obj.w = w
    obj.h = h
    obj.menus = menus
    return obj
end)

function ui_menu:ctor(w, h, menus)

    self.button:effect_click(function()
        self:on_button_click()
    end)
end

function ui_menu:set_text(text)
    self.text:set_text(text)

    local t_w, t_h = self.text:get_size()
    self.w = math.max(self.w, t_w)

    self:adjust_size(self.w, self.h)
end

function ui_menu:adjust_size(w, h)
    self:set_size(w, h)
    self.button:set_size(w, h)

    if self.__menu then 
        local items = self.__menu.items
        for item in ipairs(items) do 
            item:set_size(w, h)
        end
    end
end

function ui_menu:on_button_click()
    print("on_button_click")

    self.is_show = not self.is_show 

    if self.is_show then 
        self:show()
    else 
        self:hide()
    end
end

function ui_menu:create_menu()
    local menus = self.menus

    if not menus then 
        self.__menu = sprite.new_attr{  }
    else 
        local height = 0--self.h
        local gap = 3

        local wpos_x, wpos_y = self:get_world_pos()

        local items = {}
        local item_len = #menus
        local max_w = 0
        for idx = item_len, 1, -1 do 
            local info = menus[idx]
            local item = sprite.new_attr{
                menu = true,
                w = self.w, h = self.h, 
                x = 0, y = height,
                text = info.name or "Null",
                effect_click = info.click,
            }

            items[#items + 1] = item

            height = height + self.h + ( idx ~= 1 and gap or 0)

            local w = item:get_size()
            max_w = math.max(max_w, w)
        end

        for k, item in ipairs(items) do 
            local x, y = item:get_pos()
            item:set_pos(x + max_w / 2 + 2, y - height / 2 + self.h / 2)

            item:adjust_size(max_w, self.h)
        end

        local menu = sprite.new_attr{
            parent = require("game").stage,
            ax = 0,
            x = wpos_x - self.w / 2, y = wpos_y - height / 2 - self.h,
            w = max_w+4, h = height+4, oc = {233, 80, 90, 255},
            table.unpack(items)
        }
        menu:set_bbox_visible(true)

        menu.items = items

        self.__menu = menu
    end

    return self.__menu
end

function ui_menu:show()
    self:hide()

    self.is_show = true
    self:create_menu()
end

function ui_menu:hide()
    self.is_show = false
    if self.__menu then 
        self.__menu:remove_from_parent()
        self.__menu = nil
    end
end

return ui_menu