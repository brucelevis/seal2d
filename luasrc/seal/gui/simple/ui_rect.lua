local sprite = require "seal.sprite"

local OUTLINE_DEFAULT_COLOR = { 0, 0, 0, 255 }
local FILL_DEFAULT_COLOR = { 255, 255, 255, 255 }

local ui_rect = class("ui_rect", function(w, h, fc, oc)
    w = w or 100
    h = h or 100

    local attr = {}
    if fc then 
        attr[#attr+1] = { 
            scale9 = true, 
            w = w, h = h, 
            color = fc, 
            atlas="ui.png", texture="rect-full.png" 
        } 
    end
    if oc then 
        attr[#attr+1] = { 
            scale9 = true, 
            w = w, h = h, 
            color = oc, 
            atlas="ui.png", texture="rect.png" 
        } 
    end

    local obj = sprite.new_attr(attr)
    return obj
end)

function ui_rect:ctor(w, h, fc, oc)
end

return ui_rect