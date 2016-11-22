local core = require "sprite_core"
local sprite_frame = require "seal.sprite_frame"
local util = require "seal.util"

local sprite = {
    get_frame_from_cache = core.get_frame_from_cache,
    load_sprite_frame = core.load_sprite_frame,
    unload_sprite_frame = core.unload_sprite_frame,
    register_handler = core.register_handler,
    clean_handler = core.clean_handler,
    run_action = core.run_action,
    stop_all_actions = core.stop_all_actions,

    set_anim = core.set_anim,
    set_spine_anim = core.set_spine_anim,
    set_anim_interval = core.set_anim_interval,
    set_visible = core.set_visible,
    set_pos = function(self, x, y) core.set_pos(self.__cobj, x or 0, y or 0) end,
    set_anchor = core.set_anchor,
    set_rotation = core.set_rotation,
    set_scale = core.set_scale,
    set_color = core.set_color,
    set_size = core.set_size,
    set_glyph = core.set_glyph,

    set_frame_texture_id = core.set_frame_texture_id,
    set_text = core.set_text,
    set_swallow = core.set_swallow,

    get_swallow = core.get_swallow,
    get_rotation = core.get_rotation,
    get_scale = core.get_scale,
    get_pos = core.get_pos,
    get_world_pos = core.get_world_pos,
    get_size = core.get_size,
    get_anchor = core.get_anchor,
    get_glyph = core.get_glyph,
    get_color = core.get_color,

    remove_from_parent = core.remove_from_parent,
    remove_all_child = core.remove_all_child,
    
    slot= util.event_mt.slot,
    emit= util.event_mt.emit,
}

local meta = {__index = sprite}

function sprite.new(...)
    local self = {}
    setmetatable(self, meta)
    local __frame = sprite_frame.get(...)
    self.__cobj = core.new(__frame)
    return self
end

function sprite.new_container(...)
    local self = {}
    setmetatable(self, meta)
    self.__cobj = core.new_container(...)
    return self
end

function sprite.new_bmfont_label(label, path, line_width)
    local self = {}
    setmetatable(self, meta)
    self.__cobj = core.new_bmfont_label(label, path, line_width or 0)
    return self
end

function sprite.new_spine(atlas_file, spine_data_file, scale)
    local self = {}
    setmetatable(self, meta)
    self.__cobj = core.new_spine(atlas_file, spine_data_file, scale or 1.0)
    return self
end

function sprite.new_primitive(...)
    local self = {}
    setmetatable(self, meta)
    self.__cobj = core.new_primitive(...)
    return self
end

function sprite.new_clip(...)
    local self = {}
    setmetatable(self, meta)
    self.__cobj = core.new_clip(...)
    return self
end

function sprite.new_scale9(texture_name, frame_name, rect)
    local self = {}
    setmetatable(self, meta)
    local frame = sprite_frame.get(texture_name, frame_name)
    local r = rect
    if not r then -- estimate the rect if not provied.
        local w, h = core.get_sprite_frame_size(frame)
        r = {x = w/3, y = h/3, w = w/3, h = h/3}
    end
    self.__cobj = core.new_scale9(frame, r)
    return self
end

function sprite:add_child(child, ...)
    assert(child.parent == nil, "can not add again")
    child.parent = self
    core.add_child(self, child, ...)
end

function sprite:cleanup(...)
    self:remove_all_child(...)
    self:clean_handler()
end

function sprite:set_bbox_visible(visible, color)
    if not self.__bbox_lines then
        color = color or {255, 255, 255, 255}

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
                                            color = color} )
            self:add_child(line)
            bbox_lines[#bbox_lines+1] = line
        end
        self.__bbox_lines = bbox_lines
    end

    if not visible and self.__bbox_lines then 
        for _, line in pairs(self.__bbox_lines) do
            line:remove_from_parent()
        end
        self.__bbox_lines = nil
    end
end

function sprite:set_tex_rect(x, y, w, h)
    -- bl br tr tl 
    local glyph = { self:get_glyph() }

    -- todo: set texture rect 
    -- now, uv will back to 0 - 1 

end

return sprite