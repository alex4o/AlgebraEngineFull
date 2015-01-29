local orbit = require "orbit"
local cjson = require "cjson"
-- local gen = require "gen"
local ffi = require "ffi"


ffi.cdef[[
	const char* generate(int Fraction,int Natural,int Irational,int uh,int ul,int dh,int dl);
]]

gen = ffi.load("gen")


module("hello", package.seeall, orbit.new)

function index(web)
	return render_index()
end

function m_gen(web)
	web:content_type("application/json")
	for k,v in pairs(web.GET) do print(k,v) end
	return json_say(tonumber(web.GET.uh),tonumber(web.GET.dh),tonumber(web.GET.ul),tonumber(web.GET.dl),tonumber(web.GET.F),tonumber(web.GET.N),tonumber(web.GET.I))
end

hello:dispatch_get(index, "/", "/index.html")
hello:dispatch_get(m_gen, "/gen/")
		
function render_layout(inner_html)
	return html{
		head{ title "Hello World" },
		body{ inner_html }
	}
end

function render_hello()
	return p.hello"Hello Shit!"
end

function render_index()
	return render_layout(render_hello())
end

function json_say(uh,dh,ul,dl,F,N,I)
	return cjson.encode({equ = ffi.string(gen.generate(F,N,I,uh,ul,dh,dl))})
end

orbit.htmlify(hello, "render_.+")

return _M