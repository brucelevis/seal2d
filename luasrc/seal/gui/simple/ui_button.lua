local sprite = require "seal.sprite"
local ui_rect = require "seal.gui.simple.ui_rect"

local ui_button = class("ui_button", function(w, h)
    local obj = ui_rect.new(w, h, {233, 80, 90, 255})
    return obj
end)


function ui_button:ctor(w, h)
    
end

return ui_button