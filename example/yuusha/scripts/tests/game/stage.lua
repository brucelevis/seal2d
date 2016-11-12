local sprite = require "seal.sprite"
local sprite_frame = require "seal.sprite_frame"

local stage = class("stage", function()
		return sprite.new_container()
	end)

function stage:ctor()
	sprite_frame.load_from_json("res/images/ui.json")
    sprite_frame.load_from_json("res/images/anim_pirate.json")
    sprite_frame.load_from_json("res/images/skeleton.json")

    if device.is_pc() then
        local nuk_node = require "seal.nuk_node"
        self.menu = nuk_node.new()
    else
        local hello_world = sprite.new_bmfont_label("hello seal2d.",
                                                    "res/fonts/animated.txt")
        hello_world:set_pos(0, 0)
        self:add_child(hello_world)

    end
end

function stage:switch(new)
    if self.current ~= new then
        if self.current and self.current.on_exit then
            self.current:on_exit()
        end

        if self.current then -- current may not exist for the first time.
            self.current:remove_from_parent()
        end

        self:add_child(new)
        if new.on_enter then
            new:on_enter()
        end
    	self.current = new
    end
end

local tests = {
    { name = "hello world", sameple_name = "hello_world" },
    { name = "sprite test", sameple_name = "sprite_test" },
    { name = "gui test", sameple_name = "gui_test" },
    { name = "dump cmem", sameple_name = function(self)
                                            self.current = nil
                                            self:cleanup()
                                            require("platform_core").__cmem()
                                         end}
}

local function draw_menu(self)
    local nuk_node = require "seal.nuk_node"
    local menu = self.menu

    if (menu:nk_begin("tests",
        {x = WINDOW_WIDTH - 200, y = 50, w = 200, h = WINDOW_HEIGHT - 200/2},
            nuk_node.NK_WINDOW_BORDER      |
            nuk_node.NK_WINDOW_MOVABLE     |
            nuk_node.NK_WINDOW_SCALABLE    |
            nuk_node.NK_WINDOW_MINIMIZABLE |
            nuk_node.NK_WINDOW_TITLE
        )) then

        -- the layout size should never be larger than the panel size.....
        menu.nk_layout_row_static(30, 150, 1)

        for i = 1, #tests do
            local t = tests[i]
            if (menu.nk_button_label(t.name, menu.NK_BUTTON_DEFAULT)) then
                if type(t.sameple_name) == 'string' then
            	   local node = require("tests.game.samples." .. t.sameple_name)
                    self:switch(node.new())
                elseif type(t.sameple_name) == 'function' then
                    t.sameple_name(self)
                end
            end
        end
    end
    menu.nk_end()
end

local function draw_current(self)
	if self.current and self.current.draw then
		self.current:draw()
	end
end

function stage:draw()
	if device.is_pc() then
        local nuk_node = require "seal.nuk_node"
		nuk_node.draw_start()
    	draw_menu(self)
    	draw_current(self)
    	nuk_node.draw_end()
    end

end

return stage