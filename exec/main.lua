local orbit = require("orbit")
local cjson = require("cjson")
-- local gen = require "gen"
local ffi = require("ffi")
local jwt = require("luajwt")
local base64 = require("base64")
local pg = require('pgproc')


ffi.cdef[[
	const char* generate(int Fraction,int Natural,int Irational,int uh,int ul,int dh,int dl,int count);
	

	typedef struct
	{
		char* problem;
		char* solution;
	} Result;

	typedef struct
	{
	    int pNatural;
	    int pRational;
	    int pIrational;

	    int pNegative;

	    int upLow, upHigh, downLow, downHigh;
	} CoefDescriptor;

	typedef struct 
	{
	    int maxPow; //Maximum power of expression

	    int maxLetters; //Minimal and maximum number of letters in subterm
	    int minLetters;

	    int maxTerms;
	    int minTerms;

	    int minSubTerm, maxSubTerm;

	    bool factored;

	    CoefDescriptor cf;//Descriptor for the coef of letters, for example the 3 in 3a
	    CoefDescriptor transformCF;
	    
	    char letters[8];
	    int cLetters;

	} ExpressionDescriptor;

	Result oprosti(ExpressionDescriptor ed);

]]



pg.connect("host=postgres  user='postgres' password=illuminati dbname=postgres")
pg.bind("public")


local secret = "itanimulli"

local alg = "HS256"

local gen = ffi.load("gen")
local ae = ffi.load("AlgebraEngine")


module("hello", package.seeall, orbit.new)

function index(web)
	return render_index()
end

function post_gen(web)
	web:content_type("text/plain")
	local data = cjson.decode(web.POST.post_data)
	tprint(data)
	return generate(data.down, data.up, data.type[1], data.type[2])
end

function login(web)
	web:content_type("application/jwt")
	local data = cjson.decode(web.POST.post_data)
	tprint(data)
	id = public.get_user(data.pass,data.name)["get_user"]
	if id == "" then
		payload = {
			user_id = -1
		}
	else
		payload = {
			user_id = id
		}
	end
	print(type(id))

	local token = jwt.encode(payload, secret, alg)
	return token

end

function signup(web)
	web:content_type("application/jwt")
	local data = cjson.decode(web.POST.post_data)
	tprint(data)
	res = public.create_user(data.username,data.password)

	print(res)
end

function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. ": "
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    elseif type(v) == 'boolean' then
      print(formatting .. tostring(v))      
    else
      print(formatting .. v)
    end
  end
end

hello:dispatch_get(index, "/", "/index.html")
hello:dispatch_post(post_gen, "/gen/")
hello:dispatch_post(login,"/login/")
hello:dispatch_post(signup,"/signup/")



function render_layout(inner_html)
	return html{
		head{ title "Hello World" },
		body{ inner_html }
	}
end

function render_hello()
	return p.hello"Hello Shit!" .. a
end

function render_index()
	return render_layout(render_hello())
end

function generate(down,up,F,N)
	

	local c_res = gen.generate(F,N,0,up.high,up.low,down.high,down.low,3)
	local math = ffi.string(c_res)
	print(math)
	return math
end

orbit.htmlify(hello, "render_.+")

ed = ffi.new("ExpressionDescriptor")
 -- res = ffi.new("Result")

ed.minTerms=1;
ed.maxTerms=3;

ed.factored=false;
ffi.copy(ed.letters,"abc")
ed.cLetters=3;

ed.minLetters=2;
ed.maxLetters=3;

ed.cf.pNatural=100;
ed.cf.pNegative=50;
ed.cf.pIrational=0;
ed.cf.pRational=0;
ed.cf.upHigh=7;
ed.cf.upLow=1;

ed.transformCF.pNatural=100;
ed.transformCF.pIrational=0;
ed.transformCF.pRational=0;
ed.transformCF.upHigh=10;
ed.transformCF.upLow=1;

ed.minSubTerm=1;
ed.maxSubTerm=2;

ed.maxPow=4;

print(ed.maxTerms)
print(ed.maxSubTerm)

res = ae.oprosti(ed)

print(ffi.string(res.problem))
print(ffi.string(res.solution))

return _M