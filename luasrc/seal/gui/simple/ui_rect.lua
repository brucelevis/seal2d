local sprite = require "seal.sprite"
local consts = require "seal.consts"
local ui_rect = class("ui_rect", function()
    local obj = sprite.new_container()
    return obj
end)


function ui_rect:ctor(w, h)
    self:set_size(w, h)
    self:init_rect()
end

function ui_rect:init_rect()
    if not self.__bbox_lines then

        local ax, ay = self:get_anchor()
        local w, h = self:get_size()
        local xoffset = -w * (ax)
        local yoffset = -h * (ay)

        --[[
           p3-----p2
            |     |
            |     |
           p0_____p1
        ]]
        local x0, y0 = 0 + xoffset, 0 + yoffset
        local x1, y1 = w + xoffset, 0 + yoffset
        local x2, y2 = w + xoffset, h + yoffset
        local x3, y3 = 0 + xoffset, h + yoffset

        local lines = {
            {x0, y0, x1, y1},
            {x1, y1, x2, y2},
            {x2, y2, x3, y3},
            {x3, y3, x0, y0}
        }

        local bbox_lines = {}
        for i = 1, #lines do
            local line = sprite.new_primitive("L", {
                                            vertex = lines[i],
                                            width = 2.0,
                                            color = {255, 0, 0, 255}} )
            self:add_child(line)
            bbox_lines[#bbox_lines+1] = line
        end
        self.__bbox_lines = bbox_lines
    end
end

return ui_rect