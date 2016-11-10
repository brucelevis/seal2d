local sprite = require "seal.sprite"
local consts = require "seal.consts"
local button = class("button", function()
		return sprite.new_container()
	end)

local state_visible = {
	['normal']   = {true,  false, false},
	['selected'] = {false, true,  false},
	['disable']  = {false, false, true},
}

local function default_callback(self)
	print("no callback set for button", self)
end

local function validate(self)
	local v = state_visible[self.state]
	self.ns:set_visible(v[1])
	self.ss:set_visible(v[2])
	self.ds:set_visible(v[3])
end

local function init(self, text, atlas, size, nor, sel, dis)
	local ns = sprite.new_scale9(atlas, nor)
	local ss = sprite.new_scale9(atlas, sel)
	local ds = sprite.new_scale9(atlas, dis)

	local w, h = ns:get_size()
	print(string.format("w, h = %s, %s", w, h))
	if size then
		w, h = size.w, size.h
	end

	ns:set_size(w, h)
	ss:set_size(w, h)
	ds:set_size(w, h)

	ns:set_anchor(0.5, 0.5)
	ns:set_anchor(0.5, 0.5)
	ns:set_anchor(0.5, 0.5)

	-- ns:set_pos(w/2, h/2)
	-- ss:set_pos(w/2, h/2)/
	-- ds:set_pos(w/2, h/2)

	self:add_child(ns)
	self:add_child(ss)
	self:add_child(ds)

	self.ns, self.ss, self.ds = ns, ss, ds

	print("size = ", w, h)
	self:set_size(w, h)
	validate(self)
end

function button:ctor(config)
	local atlas = assert(config.atlas)
	local n = assert(config.n)
	local s = config.s or n
	local d = config.d or n
	local text = config.text or " " -- render an empty space as placeholder
	local size = config.size

	self.callback = config.callback or default_callback
	self.state = 'normal'

	self:register_handler(function(event, ...)
        local function on_touch(event, x, y)
            print(string.format("Touch (%s) (%d, %d)", event, x, y))
            if event == consts.TOUCH_END then
            	self.callback(self)
            end
        end
        if event == "touch" then
            on_touch(...)
        end
    end)

	init(self, text, atlas, size, n, s, d)
	self:set_bbox_visible(true)
end

function button:set_enable(enable)

end

function button:set_cb(callback)
	assert(callback and type(callback) == 'function')
	self.callback = callback
end

return button