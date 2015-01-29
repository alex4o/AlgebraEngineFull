extern "C" {

	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"

}

using namespace std;

#include <iostream>
#include <sstream>
#include <vector>
#include <random>

#include "Generator.hpp"

std::string gen_string(Polynomial* polynomial){
	//std::string str;
	std::stringstream ss;
	int size = polynomial->monos.size();
	for(int i = 0; i < size; i++)
	{

		if(polynomial->monos[i].coef.isNatural() and polynomial->monos[i].coef.fraction.up==1) continue;
		if(polynomial->monos[i].coef.isNatural()) ss<<polynomial->monos[i].coef.fraction.up;
		else if(polynomial->monos[i].coef.isRational())
		{
			ss<<"\\frac{";
			ss<<polynomial->monos[i].coef.fraction.up;
			ss<<"}{";
			ss<<polynomial->monos[i].coef.fraction.down;
			ss<<"}";
		}

		for(int p = 0; p < polynomial->monos[i].simples.size(); p++)
		{



			if(polynomial->monos[i].simples[p].pow==1) {
				ss<<polynomial->monos[i].simples[p].letter;
			}
			else{
				
				ss<<polynomial->monos[i].simples[p].letter;
				ss<<"^";
				ss<<polynomial->monos[i].simples[p].pow;
			}
		}

		
		//std::cout<<"["<<monos[i].totalPower<<"]";
		if(i!=polynomial->monos.size()-1){
			ss<<" + ";
		}
	}
	return ss.str();
}

static int lua_run( lua_State *L )
{
	RootDescriptor rd;
	rd.pFraction=0;
	rd.pNatural=100;
	rd.pIrational=0;
	rd.upHigh=10;
	rd.upLow=1;
	rd.downLow=1;
	rd.downHigh=1;

	std::random_device r_dev;

	Monomial m(Number(rng(1, 20,&r_dev)));
	Polynomial c = generate(3, rd, 'x',&r_dev);
	c = multByMono(c, m);
	
	
	std::string mon = gen_string(&c);
	  

	lua_pushlstring(L,mon.c_str(),mon.size());
    return 1;
}

static const luaL_Reg foo[] = {
        { "run", lua_run },
        { NULL, NULL }
 };

extern "C" {
const char* generate(int Fraction,int Natural,int Irational,int uh,int ul,int dh,int dl)
{
	RootDescriptor rd;
	rd.pFraction=Fraction;
	rd.pNatural=Natural;
	rd.pIrational=Irational;
	rd.upHigh=uh;
	rd.upLow=ul;
	rd.downHigh=dh;
	rd.downLow=dl;

	std::random_device r_dev;

	Monomial m(Number(rng(1, 20,&r_dev)));
	Polynomial c = generate(3, rd, 'x',&r_dev);
	c = multByMono(c, m);
	
	
	std::string mon = gen_string(&c);
	return mon.c_str();
}

int luaopen_gen(lua_State *L){
	
	luaL_openlib(L, "gen", foo, 0);
	return 1;
} 
}


int main(int argc, char **argv)
{
	
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	lua_newtable(L); 
	luaL_openlib(L, "gen", foo, 0);
	luaL_dostring(L, "print(gen.run())");
	return 0;
}