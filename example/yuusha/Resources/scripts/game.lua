local consts = require "seal.consts"

if ENGINE_MODE == 1 then
    require "seal.gui.simple.ui_init"
end

local game = {}
function game.load_shader(shader_index)

end

function game.init()
    local stage = require("tests.game.stage").new()
    game.stage = stage

    return stage.__cobj
end

function game.draw()
end

function game.pause()
    print('call game.pause()')
end

function game.resume()
    print('call game.resume()')
end

function game.reload()
    local loaded = {}
    for k,v in pairs(package.loaded) do
        loaded[#loaded+1] = k
    end

    for k,v in pairs(loaded) do
        package.loaded[v] = nil
    end
end

local EVENT_GAME_START = (0)
local EVENT_GAME_END   = (1)
function game.event(event_type, ...)
    if event_type == EVENT_GAME_START then
        print("Game Started")
    elseif event_type == EVENT_GAME_END then
        print("Game Ended")
    else
        assert(false, "unprocessed game event.")
    end
end

return game
