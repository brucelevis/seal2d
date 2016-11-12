local action = require "action_core"

local touch_event_text = {
    "begin", "move", "end"
}

local nil_func = function() end

local function sprite_init_touch_events(self)
    if not self.__is_init_touch_events then 
        self.__is_init_touch_events = true

        self.__handle_touch_began = self.__handle_touch_began or nil_func
        self.__handle_touch_moved = self.__handle_touch_moved or nil_func
        self.__handle_touch_ended = self.__handle_touch_ended or nil_func

        local last_x, last_y = 0, 0

        local root_event_handler = function(event, id, x, y)
            if event == "touch" then
                if id == 0 then 
                    last_x, last_y = x, y
                    self.__handle_touch_began(self, x, y)
                elseif id == 1 then 
                    local seek_x, seek_y = x - last_x, y - last_y
                    last_x, last_y = x, y
                    self.__handle_touch_moved(self, x, y, seek_x, seek_y)
                elseif id == 2 then 
                    self.__handle_touch_ended(self, x, y)
                end
            end
        end

        self:register_handler(root_event_handler)
    end 
end

function sprite:click(handle)    
    sprite_init_touch_events(self)

    self.__handle_touch_ended = handle

    return self
end

function sprite:touch_began(handle)    
    sprite_init_touch_events(self)

    self.__handle_touch_began = handle

    return self
end

function sprite:touch_moved(handle)    
    sprite_init_touch_events(self)

    self.__handle_touch_moved = handle

    return self
end

function sprite:effect_click(handle)    
    sprite_init_touch_events(self)

    local sx, sy = self:get_scale()
    self.__handle_touch_began = function(sender)
        sender:stop_all_actions()
        local scale = action.scale_to(0.2 , sx * 0.8, sy * 0.8)
        sender:run_action(scale)
    end

    self.__handle_touch_ended = function(sender, x, y)
        sender:stop_all_actions()
        local scale = action.scale_to(0.2 , sx, sy)
        sender:run_action(scale)

        if handle then 
            handle(sender, x, y)
        end
    end

    return self
end


local function sprite_apply_attribute(self, attr)
    if attr.x or attr.y then 
        local x, y = self:get_pos()
        x = attr.x or x
        y = attr.y or y
        self:set_pos(x, y)
    end
    if attr.sx or attr.sy then 
        local sx, sy = self:get_scale()
        sx = attr.sx or sx
        sy = attr.sy or sy
        self:set_scale(sx, sy)
    end
    if attr.w or attr.h then 
        local w, h = self:get_size()
        w = attr.w or w
        h = attr.h or h
        self:set_size(w, h)
    end
    if attr.ax or attr.ay then 
        local ax, ay = self:get_anchor()
        ax = attr.ax or ax
        ay = attr.ay or ay
        self:set_anchor(ax, ay)
    end
    if attr.color then 
        self:set_color(attr.color)
    end
    if attr.rotation then 
        self:set_rotation(attr.rotation)
    end
    if attr.text then 
        self:set_text(attr.text)
    end
    if attr.anim then 
        self:set_spine_anim(attr.anim, 0, attr.loop)
    end
    if attr.moved then 
        self:touch_moved(attr.moved)
    end
    if attr.click then 
        self:click(attr.click)
    end
    if attr.effect_click then 
        self:effect_click(attr.effect_click)
    end
    if attr.parent then 
        attr.parent:add_child(self)
    end

    return self
end

function sprite.new_attr(attr)
    local typename = attr.typename
    local sp = nil

    if attr.atlas and attr.texture then 
        if attr.scale9 then 
            sp = sprite.new_scale9(attr.atlas, attr.texture)
        else 
            sp = sprite.new(attr.atlas, attr.texture)
        end
    elseif attr.fnt then 
        sp = sprite.new_bmfont_label("", attr.fnt)
    elseif attr.json then 
        sp = sprite.new_spine(attr.atlas, attr.json, attr.scale or 1)
    else
        sp = sprite.new_container()
    end

    sprite_apply_attribute(sp, attr)
    
    for idx, attr in ipairs(attr) do 
        local child = sprite.new_attr(attr)
        sp:add_child(child)
    end

    return sp
end

--[[
    style = {
        x = 0, y = 0, sx = 0, sy = 0, ax = 0, ay = 0, rotation = 0, color=0xFFFFFFFF,
        atlas='ui.png', texture='button.png', // sprite
        scale9 = true, atlas='ui.png', texture='button.png', // scale9
        fnt="", text = "", //  bmfont
        json="aaa.json", atlas='ui.png', scale = 1, anim="idle", loop = true // spine
    }

]]
function sprite:apply_style(style)  
    self:remove_all_child()

    sprite_apply_attribute(self, style)

    for idx, attr in ipairs(style) do 
        local child = sprite.new_attr(attr)
        self:add_child(child)
    end

    return self
end