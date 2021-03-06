local sprite = require "seal.sprite"
local sprite_frame = require "seal.sprite_frame"
local profiler = require "profiler_core"

local stage = class("stage", function()
        return sprite.new_container()
    end)

local function add_profile_status(self)
    local fps_label = sprite.new_bmfont_label("fps", "res/fonts/animated.txt")
    fps_label:set_pos(0, 0)
    fps_label:set_anchor(0, 0)
    self:add_child(fps_label, 10000)

    local draw_call_label = sprite.new_bmfont_label("drawcall", "res/fonts/animated.txt")
    draw_call_label:set_pos(0, 70)
    draw_call_label:set_anchor(0, 0)
    self:add_child(draw_call_label, 10000)

    local update_timer = require("seal.timer").new {
        loop = -1,
        callback = function(dt)
            fps_label:set_text(string.format("fps: %.2f", profiler.get_fps()))
            draw_call_label:set_text(string.format("drawcall: %d", profiler.get_drawcall()))
        end,
        interval = 3/60,
    }

end

function stage:ctor()
    sprite_frame.load_from_json("res/images/ui.json")
    sprite_frame.load_from_json("res/images/anim_pirate.json")
    sprite_frame.load_from_json("res/images/skeleton.json")

    local edit = require("editor.menu.edit_menu_init").init(self)
    self:init_events(edit)

    add_profile_status(self)
end

function stage:init_events(edit)
    edit.top:slot("hello_world", function()
        self:on_test_menu_click("hello_world")
    end)
    edit.top:slot("sprite_test", function()
        self:on_test_menu_click("sprite_test")
    end)
    edit.top:slot("gui_test", function()
        self:on_test_menu_click("gui_test")
    end)
    edit.top:slot("bunny_test", function()
        self:on_test_menu_click("bunny_test")
    end)
    edit.top:slot("dump_cmem", function()
        self:switch(nil)
        self.current = nil
        collectgarbage()
        require("platform_core").__cmem()
    end)
end

function stage:on_test_menu_click(name)
    if "dump_cmem" == name then
        self:switch(nil)
        self.current = nil
        collectgarbage()
        require("platform_core").__cmem()
        return
    end

    if type(name) == 'string' then
        local path = "tests.game.samples." .. name
        package.loaded[path] = nil
        local node = require(path)
        self:switch(node.new(self))
    elseif type(name) == 'function' then
        name(self)
    end
    --self:create_menu()
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
    { name = "bunny test", sample_name = "bunny_test"},
    { name = "dump cmem", sample_name = function(self)
                                            self:switch(nil)
                                            self.current = nil
                                            collectgarbage()
                                            require("platform_core").__cmem()
                                         end}
}

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


return stage
