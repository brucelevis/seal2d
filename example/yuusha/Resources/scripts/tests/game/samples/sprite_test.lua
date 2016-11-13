local nuk_node = require "seal.nuk_node"
local common = require "tests.game.samples.sample_common"

local sprite_test = class("sprite_test", function()
    return sprite.new_container()
end)

local function load_srt(self)
    local sprites = {
        { frame = "skeleton_1.png" },
        { frame = "skeleton_2.png", scale = 1.5},
        { frame = "skeleton_3.png", rotation = 90 },
        { frame = "skeleton_4.png", rotation = 360, scale = 2 },
    }

    common.load_skeltons(self, "skeleton.png", sprites, 0, WINDOW_HEIGHT/2)
end

local function scene_graph(self)
    local root = sprite.new("ui.png", "tan_normal.png")

    local container = sprite.new_container()
    container:set_pos(0, 0)
    container:set_anchor(0, 0)
    root:add_child(container)

    local s1 = sprite.new("ui.png", "tan_selected.png")
    s1:set_pos(0, 0)
    s1:set_anchor(0, 0)
    container:add_child(s1)

    local ss1 = sprite.new("ui.png", "tan_disable.png")
    ss1:set_pos(48, 48)
    ss1:set_anchor(0.5, 0.5)
    s1:add_child(ss1)

    self:add_child(root)

    root:set_pos(WINDOW_WIDTH/2, WINDOW_HEIGHT/2)

    local action = require "action_core"
    local scale = action.scale_to(1.0 , 2, 2)
    local rotate = action.rotate_to(1.0, 360)
    root:run_action(action.sequence({scale, rotate}))
end

local function load_anchor(self)
    local sprites = {
        {
            frame = "skeleton_1.png",
            anchor = { x = 0, y = 0},
            pos = { x = 0, y = 0},
        },
        {
            frame = "skeleton_1.png",
            anchor = {x = 0.5, y = 0.5},
            pos = {x = WINDOW_WIDTH/2, y = WINDOW_HEIGHT/2}
        },
        {
            frame = "skeleton_1.png",
            anchor = {x = 1, y = 0},
            pos = {x = WINDOW_WIDTH, y = 0}
        },
        {   frame = "skeleton_1.png",
            anchor = {x = 1, y = 1},
            pos = {x = WINDOW_WIDTH, y = WINDOW_HEIGHT}
        },
        {
            frame = "skeleton_1.png",
            anchor = {x = 0, y = 1},
            pos = {x = 0, y = WINDOW_HEIGHT}
        },
    }

    common.load_skeltons(self, "skeleton.png", sprites)
end

local function load_anim(self)
    local frames = {}
    for i = 0, 4 do
        local name = string.format("attack_%d.png", i)
        local f = assert(sprite_frame.get("anim_pirate.png", name))
        frames[#frames+1] = f
    end

    local s = sprite.new("anim_pirate.png", "attack_0.png")

    s:set_pos(WINDOW_WIDTH/2, WINDOW_HEIGHT/2)
    s:set_anim(frames)
    s:set_anim_interval(1)

    self:add_child(s)
end

local function load_clip(self)
    local clip = sprite.new_clip({ x = 100, y = 100, w = 400, h = 200})
    clip:set_anchor(0, 0)
    -- clip:set_bbox_visible(true)
    self:add_child(clip)

    local cases = {
        { x = 0, y = 0 }, --left bottom corner
        { x = 350, y = 0 }, -- see half face on the right
        { x = 0, y = 200}, -- we can't see this
        { x = 200, y = 100}, -- we can see one in the center
        { x = 350, y = 150}
    }
    for i = 1, #cases do
        local c = cases[i]
        local s = sprite.new("ui.png", "smile_middle.png") -- size 100x100
        s:set_pos(c.x, c.y)
        s:set_anchor(0, 0)
        -- s:set_bbox_visible(true)
        clip:add_child(s)
    end
end

local function load_glyph(self)
    local s = sprite.new("ui.png", "smile_middle.png")
    s:set_pos(1, 200)
    s:set_anchor(0, 0.5)
    s:set_bbox_visible(true)
    self:add_child(s)

    local s2 = sprite.new("ui.png", "smile_middle.png")
    s2:set_pos(WINDOW_WIDTH/2, WINDOW_HEIGHT/2)
    s2:set_bbox_visible(true)
    self:add_child(s2)
end

local function load_spine(self)
    local test_cases = {
        {
            atlas = "res/spine_anim/spineboy.atlas",
            json = "res/spine_anim/spineboy.json",
            loop = true,
            anim_name = 'walk',
            x = 30, y = WINDOW_HEIGHT/2,
            scale = 0.2
        },
        {
            atlas = "res/spine_anim/spineboy.atlas",
            json = "res/spine_anim/spineboy.json",
            loop = false,
            anim_name = 'death',
            x = 200, y = WINDOW_HEIGHT/2,
            scale = 0.2
        },
        {
            atlas = "res/spine_anim/spineboy.atlas",
            json = "res/spine_anim/spineboy.json",
            loop = true,
            anim_name = 'run',
            x = WINDOW_WIDTH/2 - 30, y = WINDOW_HEIGHT/2,
            scale = 0.2
        },

        {
            atlas = "res/spine_anim/powerup.atlas",
            json = "res/spine_anim/powerup.json",
            anim_name = 'animation',
            x = WINDOW_WIDTH/2 - 30, y = WINDOW_HEIGHT/2 + 100,
            scale = 0.2
        },

    }
    for i = 1, #test_cases do
        local c = test_cases[i]
        local spine = sprite.new_spine(c.atlas, c.json, c.scale)
        spine:set_pos(c.x, c.y)
        spine:set_spine_anim(c.anim_name, 0, c.loop)
        self:add_child(spine)
    end
end

local function load_zorder(self)
    local zorders = {100, 0, -1, 0, 1}
    local offset_x = 30
    local pos = {
        {x = WINDOW_WIDTH/2, y = WINDOW_HEIGHT/2},
        {x = WINDOW_WIDTH/2 - offset_x, y = WINDOW_HEIGHT/2 + 50},
        {x = WINDOW_WIDTH/2 - offset_x, y = WINDOW_HEIGHT/2 + 100},
        {x = WINDOW_WIDTH/2 + offset_x, y = WINDOW_HEIGHT/2 - 50},
        {x = WINDOW_WIDTH/2 + offset_x, y = WINDOW_HEIGHT/2 + 50},
    }

    for i, v in shuffle_pairs(zorders) do
        local order = zorders[i]
        local p = pos[i]
        local name = string.format("smile_middle_zorder_%d.png", order)
        local s = sprite.new("ui.png", name)
        s:set_pos(p.x, p.y)
        self:add_child(s, order)
    end
end

local function load_scale9(self)
    local s2 = sprite.new_scale9("ui.png", "scale9_example.png")
    s2:set_pos(WINDOW_WIDTH/2, WINDOW_HEIGHT/2)

    s2:set_size(300, 300)

    self:add_child(s2)
end

local function load_touch_test(self)
    local root_label

    local touch_event_text = {
        "begin", "move", "end"
    }
    local root_event_handler = function(event, ...)
        local function on_touch(event, x, y)
            root_label:set_text(string.format("%s: (%d, %d)",
                                 touch_event_text[event+1], x, y))
        end
        if event == "touch" then
            on_touch(...)
        end
    end

    local cw = WINDOW_WIDTH/2
    local ch = WINDOW_HEIGHT/2
    local root = sprite.new_container(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, cw, ch)
    root:set_anchor(0.5, 0.5)
    root:set_bbox_visible(true)
    root:register_handler(root_event_handler)

    root_label = sprite.new_bmfont_label("root:", "res/fonts/animated.txt")
    root_label:set_pos(-cw/2, ch/2)
    root_label:set_anchor(0, 1)
    root:add_child(root_label)

    self:add_child(root)
end

function sprite_test:ctor(parent)
    self:create_menu(parent)

    if device.is_pc() then
        self.bar = nuk_node.new()
    end
end

function sprite_test:on_enter()

end

function sprite_test:on_exit()
    if self.menu then 
        self.menu:remove_from_parent()
    end
end

local test_cases = {
    {name = "srt test", load_func = load_srt},
    {name = "scene graph test", load_func = scene_graph},
    {name = "anchor test", load_func = load_anchor},
    {name = "animation test", load_func = load_anim},
    {name = "clip test", load_func = load_clip},
    -- {name = "spine test", load_func = load_spine },
    {name = "glyph test", load_func = load_glyph },
    {name = "zorder test", load_func = load_zorder},
    {name = "scale9 test", load_func = load_scale9},
    {name = "touch test", load_func = load_touch_test},
}

local BH = 40 --button height
local RH = 100 -- row height
function sprite_test:draw()
    local bar = self.bar

    if (bar:nk_begin("bar",
                     {
                        x = 0,
                        y = WINDOW_HEIGHT - (RH - BH),
                        w = WINDOW_WIDTH,
                        h = RH
                     },
                     nuk_node.NK_WINDOW_MOVABLE )) then

        -- the layout size should never be larger than the panel size.....
        bar.nk_layout_row_dynamic(BH, #test_cases)

        for i = 1, #test_cases do
            local t = test_cases[i]
            if (bar.nk_button_label(t.name, bar.NK_BUTTON_DEFAULT)) then
                self:cleanup()
                t.load_func(self)
            end
        end
    end
    bar.nk_end()
end

function sprite_test:create_menu(parent)
    if ENGINE_MODE == 1 and not self.menu_inited then
        self.menu_inited = true 

        local menues = {}
        for i = 1, #test_cases do
            local t = test_cases[i]
            menues[i] = {  
                ui_button = true, w = 140, h = 25, 
                { 
                    y = 4, fnt = 'res/fonts/animated.txt', scale = 0.8, text = t.name,
                },
                effect_click = function()
                    self:cleanup()
                    t.load_func(self)
                end
            }
        end

        local menu = nil
        menu = sprite.new_attr{
            parent = parent,  
            x =  WINDOW_WIDTH / 2, y = 25,
            ui_rect = true, w = WINDOW_WIDTH, h = 50, fc = {204, 204, 204, 255},   
            {
                scroll = true,
                direction = 2,
                x = -WINDOW_WIDTH / 2, y = -25,
                view_size = { w = WINDOW_WIDTH, h = 50 },
                table.unpack(menues)
            },
            { ui_rect = true, w = WINDOW_WIDTH, h = 50, oc = {0, 0, 0, 255}, }
        } 

        self.menu = menu
        return menu
    end
end

return sprite_test
