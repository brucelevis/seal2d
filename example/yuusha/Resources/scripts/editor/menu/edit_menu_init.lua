
local edit_menu_top = require("editor.menu.edit_menu_top")

local function _init(stage)
    local top = edit_menu_top.new(stage)   


    top:slot(top.event_type.NEW, function()
        print("OK, we create a new file.")
    end) 

    return {
        top = top,
        left = left,
        center = center,
        prop = prop,
    }
end

return {
    init = _init
}