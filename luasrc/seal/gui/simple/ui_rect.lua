local sprite = require "seal.sprite"
local core = require "sprite_core"

local OUTLINE_DEFAULT_COLOR = { 0, 0, 0, 255 }
local FILL_DEFAULT_COLOR = { 255, 255, 255, 255 }

local ui_rect = class("ui_rect", function(w, h, fc, oc)
    w = w or 100
    h = h or 100

    local fc_rect = nil
    local oc_rect = nil

    local attr = {}
    if fc then 
        fc_rect = sprite.new_attr { 
            scale9 = true, 
            w = w, h = h, 
            color = fc, 
            atlas="ui.png", texture="rect-full.png" 
        } 
    end
    if oc then 
        oc_rect = sprite.new_attr { 
            scale9 = true, 
            w = w, h = h, 
            color = oc, 
            atlas="ui.png", texture="rect.png" 
        } 
    end

    local obj = sprite.new_attr({
        fc_rect, oc_rect
    })
    obj.fc_rect = fc_rect
    obj.oc_rect = oc_rect
    return obj
end)

function ui_rect:ctor(w, h, fc, oc)

end

function ui_rect:set_size(w, h)
    core.set_size(self, w, h)

    if self.fc_rect then 
        self.fc_rect:set_size(w, h)
    end
    if self.oc_rect then 
        self.oc_rect:set_size(w, h)
    end
end

return ui_rect