local BALL_WIDTH = 20

local ui_slider = class("ui_slider", function(w, h)
    local ball = sprite.new_attr{
        ui_rect = true, fc = { 255, 225, 200, 255 },
        w = BALL_WIDTH, h = h*1.2
    }

    local loadingbar = sprite.new_attr{
        loadingbar = true, w = w, h = h,
        ball,
    }

    loadingbar.ball = ball
    loadingbar.__set_percent = loadingbar.set_percent
    return loadingbar
end)

function ui_slider:ctor(w, h)
    self.ball:touch_moved(function(sender, x, y, sx, sy)
        self:on_bar_moved(sx)
    end)

    self:set_percent(0)
end

function ui_slider:on_bar_moved(seek_x)
    local per = seek_x / self.w * 100
    self:set_percent(self:get_percent() + per)
end

function ui_slider:set_percent(per)
    local per = self:__set_percent(per)

    local w = (self.w - BALL_WIDTH) * per / 100

    local x, y = self.ball:get_pos()
    self.ball:set_pos(w-self.w/2 + BALL_WIDTH / 2, y)

    if self.changed_handle then 
        self:changed_handle(per)
    end
end

function ui_slider:on_changed(handle)
    self.changed_handle = handle
end

return ui_slider