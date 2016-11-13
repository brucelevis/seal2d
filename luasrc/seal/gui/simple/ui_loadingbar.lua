
local ui_loadingbar = class("ui_loadingbar", function(w, h)
    local bar = sprite.new_attr{ 
        clip = true, w = w-4, h = h-4, 
        { 
            ui_rect = true, 
            x = w/2-2, y = h/2-2, w = w-4, h = h-4, 
            fc = {233, 80, 90, 255}, 
        }, 
    }
    local border = sprite.new_attr{ 
        w = w, h = h, color = {255, 255, 255, 255}, 
        {
            x = -w/2+2, y = -h/2+2,
            bar,
        }
    }
    border:set_bbox_visible(true)

    local obj 
    obj= sprite.new_attr{
        w = w, h = h,
        border,   
    }

    obj.bar = bar
    obj.w = w - 4
    obj.h = h - 4
    return obj
end)


function ui_loadingbar:ctor(w, h)
    
end

function ui_loadingbar:get_percent()
    return self.per
end

function ui_loadingbar:set_percent(per)
    if per < 0 then per = 0 end   
    if per > 100 then per = 100 end    
    self.per = per

    self:set_visible(per ~= 0)
    --if per ~= 0 then 
        local w = math.floor(self.w * per / 100)
        self.bar:set_size(w, self.h)
    --end    

    return per, w
end

return ui_loadingbar