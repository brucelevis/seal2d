local sprite = require "seal.sprite"
local consts = require "seal.consts"
local action = require "action_core"
local timer = require "seal.timer"

local scroll_view = class("scroll_view", function()
        return sprite.new_container()
    end)

local scroll_dir = {
    DIR_VERTICLE = 1,
    DIR_HORIZONTAL = 2,
    DIR_BOTH = 3,
}
local SIZE = {w = 200, h = 300}
local INTERSPACE = 0
local GAP = 2
local CALLBACK = function() print("no callback in scroll_view.") end

function scroll_view:ctor(conf)

    self.is_bounce = true

    self.d_y = 0
    self.d_x = 0


    self.now_h = 0
    self.now_w = 0

    self.contents = {}

    self.not_first = 0

    self.size = SIZE
    if conf.view_size then
        self.size = conf.view_size
    end
    self:set_size(self.size.w, self.size.h)
    self:set_anchor(0, 0)

    self.direction = scroll_dir.DIR_VERTICLE
    if conf.direction then
        self:set_direction(conf.direction)
    end

    self.interspace = INTERSPACE
    if conf.interspace then
        self.interspace = conf.interspace
    end

    self.gap = GAP
    if conf.gap then
        self.gap = conf.gap
    end

    self.callback = CALLBACK

    local container = sprite.new_clip({x = 0, y = 0, w = self.size.w, h = self.size.h})
    container:set_anchor(0, 0)
    -- container:set_bbox_visible(true)
    self:add_child(container)

    local child_union = sprite.new_container()
    child_union:set_anchor(0, 0)
    container.child_union = child_union
    container:add_child(child_union)

    container.w = self.size.w
    container.h = self.size.h

    self.container = container

    self:register_handler(function(event, ...)
        local function on_touch(event, x, y)
            local w = self.container.w
            local h = self.container.h
            local ox, oy = self.container:get_anchor()

            -- what's this ?
            -- should get world position to do this like touch_cancle event 
            -- or dispatcher touch_cancle event 
            -- if not(x > ox and x < ox + w and
            --        y > oy and y < oy + h) then
            --     self:touch_end(x - ox, y - oy)
            -- end

            if (not self.b_x) or (not self.b_y) then
                self.b_x = x - ox
                self.b_y = y - oy
            end

            if event == consts.TOUCH_BEGIN then
                self:touch_begin(x - ox, y - oy)
            elseif event == consts.TOUCH_MOVE then
                self:touch_move(x - ox, y - oy)
            elseif event == consts.TOUCH_END then
                self:touch_end(x - ox, y - oy)
            end
        end
        if event == "touch" then
            on_touch(...)
        end
    end)

    self.add_child = self.add_content
end

function scroll_view:touch_begin(x, y)

end

function scroll_view:touch_move(x, y)

    local pre_x, pre_y = self.container.child_union:get_pos()

    if self.direction == scroll_dir.DIR_VERTICLE then
        local d_y = y - self.b_y
        self.container.child_union:set_pos(pre_x, pre_y + d_y / 2)
        self.b_y = y

        self.d_y = d_y / 2
        self.d_x = 0

    elseif self.direction == scroll_dir.DIR_HORIZONTAL then
        local d_x = x - self.b_x
        self.container.child_union:set_pos(pre_x + d_x / 2, pre_y)
        self.b_x = x

        self.d_x = d_x / 2
        self.d_y = 0

    else
        print("self.direction error!")
    end

end

function scroll_view:touch_end(touch,event)
    if self.update == nil  then
        self.update = timer.new({interval = 0,
                                 loop = 0,
                                 callback = function() self:after_end() end,
                                })
    end
    self.b_x = nil
    self.b_y = nil
end

function scroll_view:after_end()
    local pre_x, pre_y = self.container.child_union:get_pos()

    local new_Y = pre_y + self.d_y
    local new_X = pre_x + self.d_x
    self.container.child_union:set_pos(new_X, new_Y)

    self.d_y = self.d_y * 0.90
    self.d_x = self.d_x * 0.90

    local Y_max = self.now_h - self.container.h
    if Y_max < 0 then Y_max = 0 end

    local X_max = -(self.now_w - self.container.w)
    if X_max > 0 then X_max = 0 end

    if math.abs(self.d_y) < 1.0 and math.abs(self.d_x) < 1.0 or
       new_Y > Y_max + self.container.h * 0.3 or new_Y < -self.container.h * 0.3 or
       new_X < X_max - self.container.w * 0.3  or new_X > self.container.h * 0.3 then

        self.update:stop()
        self.update = nil
        if new_Y > Y_max then
            local action = action.move_to(0.4, 0, Y_max)
            self.container.child_union:stop_all_actions()
            self.container.child_union:run_action(action)
        elseif new_X < X_max then
            local action = action.move_to(0.4, X_max, 0)
            self.container.child_union:stop_all_actions()
            self.container.child_union:run_action(action)
        elseif new_Y < 0 or new_X > 0 then
            local action = action.move_to(0.4, 0, 0)
            self.container.child_union:stop_all_actions()
            self.container.child_union:run_action(action)
        end
    end
    return 1;
end

function scroll_view:set_bg(spr, rect)
    self.container:add_child(spr)
end

function scroll_view:add_content(content_node)
    self.contents[#self.contents + 1] = content_node

    local my_w, my_h = content_node:get_size()
    local my_anchor_x, my_anchor_y = content_node:get_anchor()

    if self.direction == scroll_dir.DIR_VERTICLE then
        self.now_w = self.container.w
        self.now_h = self.now_h + my_h + self.not_first * self.interspace + self.gap
        local x = self.size.w / 2 + my_w * (my_anchor_x - 0.5)
        local y = self.size.h - self.now_h + my_h * my_anchor_y
        content_node:set_pos(x, y)

    elseif self.direction == scroll_dir.DIR_HORIZONTAL then
        self.now_h = self.container.h
        self.now_w = self.now_w + my_w + self.not_first * self.interspace + self.gap
        local x = self.now_w - (1 - my_anchor_x) * my_w
        local y = self.size.h / 2 + my_h * (my_anchor_y - 0.5)
        content_node:set_pos(x, y)
    --elseif self.direction = scroll_dir.DIR_BOTH ...
    else
        assert(false, "the direction of scroll_view error")
    end


    self.container.child_union:add_child(content_node)
end

function scroll_view:set_direction(dir)
    self.direction = dir
end

function scroll_view:set_interspace(inter)
    self.interspace = inter
end

function scroll_view:set_bounce(bounce)
    if bounce == true then
        self.is_bounce = true
    else
        self.is_bounce = false
    end
end


return scroll_view
