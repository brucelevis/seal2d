
local event_type = {
    NEW     = "event_new_file",
    OPEN    = "event_open_file",
    SAVE    = "event_save_file",
    CLOSE   = "event_close_file",
}

local edit_menu_top = class("edit_menu_top", function(stage)
    local w = WINDOW_WIDTH
    local h = 30
    local obj = nil

    -- file menu
    local file_menu = sprite.new_attr{
        id = 1,
        menu = true,
        w = 70, h = 25,
        x = 35 + 2,
        text = 'Menu',
        menus = {
            { name = "New",     click = function() obj:emit(event_type.NEW    ) end },
            { name = "Open",    click = function() obj:emit(event_type.OPEN   ) end },
            { name = "Save",    click = function() obj:emit(event_type.SAVE   ) end },
            { name = "Cloese",  click = function() obj:emit(event_type.CLOSE  ) end },
        },
    }

    -- test menu
    local test_menu = sprite.new_attr{
        id = 1,
        menu = true,
        w = 70, h = 25,
        x = 35 + 2,
        text = 'Test',
        menus = {
            { name = "hello world",     click = function() obj:emit("hello_world"   ) end },
            { name = "sprite test",     click = function() obj:emit("sprite_test"   ) end },
            { name = "gui test",        click = function() obj:emit("gui_test"      ) end },
            { name = "bunny test",      click = function() obj:emit("bunny_test"    ) end },
            { name = "dump cmem",       click = function() obj:emit("dump_cmem"     ) end },
        },
    }

    obj = sprite.new_attr{
        w = w-2, h = h-2, parent = stage,
        x = 0, y = WINDOW_HEIGHT - h / 2,
        ax = 0,
        {
            scroll = true, direction = 2,
            view_size = { w = w, h = h },
            y = -h/2,
            file_menu,
            test_menu
        }

    }

    obj:set_bbox_visible(true, { 0x0, 0x7A, 0xCC, 255 })

    return obj
end)

function edit_menu_top:ctor(stage)
    print("stage:", stage)
end

edit_menu_top.event_type = event_type

return edit_menu_top