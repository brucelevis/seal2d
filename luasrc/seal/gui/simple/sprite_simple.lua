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
