local engine_core = require "engine_core"
local timer = require "seal.timer"
local engine = {}

function engine.start(game)
    engine_core.inject({
            update = assert(engine.update),

            draw   = assert(game.draw),
            event  = assert(game.event),
            init   = assert(game.init),
            pause  = assert(game.pause),
            resume = assert(game.resume),
        })
end

function engine.update(dt)
    timer.update(dt)
end

return engine