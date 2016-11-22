local sprite = require "seal.sprite"

local bunny_test = class("bunny_test", function()
        return sprite.new_container()
    end)

function bunny_test:ctor()
    local counter_label = sprite.new_bmfont_label("bunny:0", "res/fonts/animated.txt")
    counter_label:set_pos(0, 100)
    self:add_child(counter_label)
    local n_bunny = 0

    self:set_anchor(0, 0)
    local t = 50
    local bunnies = {}
    self:click(function(_, x, y)
            n_bunny = n_bunny + 50
            counter_label:set_text(string.format("bunny: %d", n_bunny))
            print("n_bunny = ", n_bunny)
            for i = 1, t do
                local s = sprite.new("ui.png", "bunny.png")
                s:set_pos(x, y)
                self:add_child(s)
                bunnies[#bunnies+1] = s
                s.speed_x = math.random(1, 100)
                s.speed_y = math.random(1, 100)
            end
        end)

    local update_timer = require("seal.timer").new {
        loop = -1,
        callback = function(dt)
            for k,s in pairs(bunnies) do
                local x, y = s:get_pos()
                local dx, dy = s.speed_x * dt, s.speed_y * dt
                x = x + dx
                y = y + dy
                if x > WINDOW_WIDTH or x < 0 then
                    s.speed_x = -s.speed_x
                end

                if y > WINDOW_HEIGHT or y < 0 then
                    s.speed_y = -s.speed_y
                end
                s:set_pos(x, y)
            end
        end,
        interval = 0,
    }
end


return bunny_test