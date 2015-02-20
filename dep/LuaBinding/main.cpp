using namespace std;

#include <iostream>
#include <sstream>
#include <vector>
#include "SPolynomial.hpp"
#include "Generator.hpp"
#include "Interface.cpp"




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

std::string gen_string(SPolynomial* poly){
	std::stringstream ss;

		for(int i = poly->power; i>=0; i--)
		{
			if(poly->coef[i]==0) continue;

				if(poly->coef[i].isNatural()) ss<<poly->coef[i].fraction.up;
				else if(poly->coef[i].isRational())
				{
				ss<<"\\frac{"<<poly->coef[i].fraction.up<<"}{"<<poly->coef[i].fraction.down<<"}";
				}

			ss<<poly->letter<<"^"<<i<<" + ";
		}
	return ss.str();
}

extern "C" {

const char* generate(int Fraction,int Natural,int Irational,int uh,int ul,int dh,int dl,int count)
{
	Generator gen;
	gen.descriptor.pFraction=Fraction;
	gen.descriptor.pNatural=Natural;
	gen.descriptor.pIrational=Irational;
	gen.descriptor.upHigh=uh;
	gen.descriptor.upLow=ul;
	gen.descriptor.downHigh=dh;
	gen.descriptor.downLow=dl;

	/*
	SPolynomial a, b;
	a.letter='x';
	b.letter='x';
	a.power=1;
	a.coef[0]=-2;
	a.coef[1]=1;
//a.coef[2]=2;
	b.power=1;
	b.coef[0]=0;
	b.coef[1]=2;
	a.print();
	cout<<endl;
	b.print();
	cout<<endl;
	SPolynomial c= a*b;
	c.letter='x';
	//std::random_device r_dev;
	//Monomial m(Number(rng(1, 20,&r_dev)));
	*/
	Polynomial c = gen.generatePoly(count,'x');
	//c = multByMono(c, m);
	
	
	std::string mon = gen_string(&c);
	return mon.c_str();
}
/*
int luaopen_gen(lua_State *L){
	
	luaL_openlib(L, "gen", foo, 0);
	return 1;
} 
*/
}


int main(int argc, char **argv)
{
	return 0;

}