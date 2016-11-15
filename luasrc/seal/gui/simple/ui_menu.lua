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

function ui_menu:set_text(text)
    self.text:set_text(text)
end

function ui_menu:ctor(w, h, menus)

    self.button:effect_click(function()
        self:on_button_click()
    end)
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

        local items = {}
        local item_len = #menus
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
        end

        for k, item in ipairs(items) do 
            local x, y = item:get_pos()
            item:set_pos(x, y - height / 2 + self.h / 2)
        end

        local menu = sprite.new_attr{
            parent = self, --self,require("game").stage,
            --ui_rect = true, 
            y = - height / 2 - self.h,
            w = self.w+4, h = height+4, oc = {233, 80, 90, 255},
            table.unpack(items)
        }
        menu:set_bbox_visible(true)

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