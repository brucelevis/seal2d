local sprite = require "seal.sprite"
local sprite_frame = require "seal.sprite_frame"

local stage = class("stage", function()
        return sprite.new_container()
    end)

function stage:ctor()
    sprite_frame.load_from_json("res/images/ui.json")
    sprite_frame.load_from_json("res/images/anim_pirate.json")
    sprite_frame.load_from_json("res/images/skeleton.json")

    self:create_menu()
end

function stage:switch(new)
    if self.current ~= new then
        if self.current and self.current.on_exit then
            self.current:on_exit()
        end

        if self.current then -- current may not exist for the first time.
            self.current:remove_from_parent()
        end

        if new then
            self:add_child(new)
            if new.on_enter then
                new:on_enter()
            end
        end
        self.current = new
    end
end

local tests = {
    { name = "hello world", sample_name = "hello_world" },
    { name = "sprite test", sample_name = "sprite_test" },
    { name = "gui test", sample_name = "gui_test" },
    { name = "dump cmem", sample_name = function(self)
                                            self:switch(nil)
                                            self.current = nil
                                            collectgarbage()
                                            require("platform_core").__cmem()
                                         end}
}

local function draw_menu(self)
    local nuk_node = require "seal.nuk_node"
    local menu = self.menu

    menu.nk_set_button_border_color(0, 255, 0, 255)

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
                if type(t.sample_name) == 'string' then
                   local node = require("tests.game.samples." .. t.sample_name)
                    self:switch(node.new())
                elseif type(t.sample_name) == 'function' then
                    t.sample_name(self)
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

function stage:create_menu()
    if ENGINE_MODE == 1 and not self.menu_inited then
        self.menu_inited = true

        local ui_rect = require "seal.gui.simple.ui_rect"
        local ui_button = require "seal.gui.simple.ui_button"

        local menues = {}
        for i = 1, #tests do
            local t = tests[i]
            menues[i] = {
                ui_button = true, w = 140, h = 25,
                {
                    y = 4, fnt = 'res/fonts/animated.txt', scale = 0.8, text = t.name,
                },
                effect_click = function()
                    if type(t.sample_name) == 'string' then
                        local path = "tests.game.samples." .. t.sample_name
                        package.loaded[path] = nil
                        local node = require(path)
                        self:switch(node.new(self))
                    elseif type(t.sample_name) == 'function' then
                        t.sample_name(self)
                    end
                end
            }
        end

        local menu = nil
        menu = sprite.new_attr{
            parent = self,
            x = WINDOW_WIDTH - 80, y = WINDOW_HEIGHT/2 + 100,
            ui_rect = true, w = 150, h = 500, fc = {204, 204, 204, 255},
            {
                scale9 = true, w = 150, h = 25, x = 0, y = 237.5,
                color  = {233, 80, 90, 255}, atlas="ui.png", texture="rect-full.png",
                moved  = function(sender, x, y, sx, sy)
                    local cx, cy = menu:get_pos()
                    menu:set_pos(cx + sx, cy + sy)
                end,
                { fnt = 'res/fonts/animated.txt', text = 'tests:', x = -40, y = 5, scale = 0.8 },
            },
            {
                scroll = true,
                x = -75, y = -250,
                view_size = { w = 150, h = 475 },
                table.unpack(menues)
            },
            { ui_rect = true, w = 150, h = 500, oc = {0, 0, 0, 255}, }
        }
        return
    end
end

function stage:draw()

end

return stage
