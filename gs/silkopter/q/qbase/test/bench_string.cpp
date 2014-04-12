#include "qbase.h"
#include <boost/test/unit_test.hpp>
#include <unordered_map>

#include <sstream>
#include <ctime>
#include <iostream>


std::string textSource = 
"A battery is a device that converts chemical energy directly to electrical energy.[22] It consists of a number of voltaic cells; "\
"each voltaic cell consists of two half-cells connected in series by a conductive electrolyte containing anions and cations. "\
"One half-cell includes electrolyte and the electrode to which anions (negatively charged ions) migrate, i.e., the anode or negative "\
"electrode; the other half-cell includes electrolyte and the electrode to which cations (positively charged ions) migrate, i.e., the "\
"cathode or positive electrode. In the redox reaction that powers the battery, cations are reduced (electrons are added) at the cathode, "\
"while anions are oxidized (electrons are removed) at the anode.[23] The electrodes do not touch each other but are electrically connected "\
"by the electrolyte. Some cells use two half-cells with different electrolytes. A separator between half-cells allows ions to flow, "\
"but prevents mixing of the electrolytes."\
"	Each half-cell has an electromotive force (or emf), determined by its ability to drive electric current from the interior to the exterior "\
"	of the cell. The net emf of the cell is the difference between the emfs of its half-cells, as first recognized by Volta.[12] Therefore, if "\
"	the electrodes have emfs  and , then the net emf is ; in other words, the net emf is the difference between the reduction potentials of the "\
"	half-reactions.[24]"\
"The electrical driving force or  across the terminals of a cell is known as the terminal voltage (difference) and is measured in volts.[25] "\
"The terminal voltage of a cell that is neither charging nor discharging is called the open-circuit voltage and equals the emf of the cell. "\
"Because of internal resistance,[26] the terminal voltage of a cell that is discharging is smaller in magnitude than the open-circuit voltage "\
"and the terminal voltage of a cell that is charging exceeds the open-circuit voltage.[27] An ideal cell has negligible internal resistance, "\
"so it would maintain a constant terminal voltage of  until exhausted, then dropping to zero. If such a cell maintained 1.5 volts and stored a "\
"charge of one coulomb then on complete discharge it would perform 1.5 joule of work.[25] In actual cells, the internal resistance increases "\
"under discharge,[26] and the open circuit voltage also decreases under discharge. If the voltage and resistance are plotted against time, the "\
"resulting graphs typically are a curve; the shape of the curve varies according to the chemistry and internal arrangement employed.[28]"\
"As stated above, the voltage developed across a cell's terminals depends on the energy release of the chemical reactions of its electrodes and "\
"electrolyte. Alkaline and zinc–carbon cells have different chemistries but approximately the same emf of 1.5 volts; likewise NiCd and NiMH "\
"cells have different chemistries, but approximately the same emf of 1.2 volts.[29] On the other hand the high electrochemical potential "\
"changes in the reactions of lithium compounds give lithium cells emfs of 3 volts or more.[30]"\
"A battery is a device that converts chemical energy directly to electrical energy.[22] It consists of a number of voltaic cells; "\
"EACH VOLTAIC CELL CONSISTS OF TWO HALF-CELLS CONNECTED IN SERIES BY A CONDUCTIVE ELECTROLYTE CONTAINING ANIONS AND CATIONS. "\
"ONE HALF-CELL INCLUDES ELECTROLYTE AND THE ELECTRODE TO WHICH ANIONS (NEGATIVELY CHARGED IONS) MIGRATE, I.E., THE ANODE OR NEGATIVE "\
"ELECTRODE; THE OTHER HALF-CELL INCLUDES ELECTROLYTE AND THE ELECTRODE TO WHICH CATIONS (POSITIVELY CHARGED IONS) MIGRATE, I.E., THE "\
"CATHODE OR POSITIVE ELECTRODE. IN THE REDOX REACTION THAT POWERS THE BATTERY, CATIONS ARE REDUCED (ELECTRONS ARE ADDED) AT THE CATHODE, "\
"WHILE ANIONS ARE OXIDIZED (ELECTRONS ARE REMOVED) AT THE ANODE.[23] THE ELECTRODES DO NOT TOUCH EACH OTHER BUT ARE ELECTRICALLY CONNECTED "\
"BY THE ELECTROLYTE. SOME CELLS USE TWO HALF-CELLS WITH DIFFERENT ELECTROLYTES. A SEPARATOR BETWEEN HALF-CELLS ALLOWS IONS TO FLOW, "\
"BUT PREVENTS MIXING OF THE ELECTROLYTES."\
"	EACH HALF-CELL HAS AN ELECTROMOTIVE FORCE (OR EMF), DETERMINED BY ITS ABILITY TO DRIVE ELECTRIC CURRENT FROM THE INTERIOR TO THE EXTERIOR "\
"	OF THE CELL. THE NET EMF OF THE CELL IS THE DIFFERENCE BETWEEN THE EMFS OF ITS HALF-CELLS, AS FIRST RECOGNIZED BY VOLTA.[12] THEREFORE, IF "\
"	THE ELECTRODES HAVE EMFS  AND , THEN THE NET EMF IS ; IN OTHER WORDS, THE NET EMF IS THE DIFFERENCE BETWEEN THE REDUCTION POTENTIALS OF THE "\
"	HALF-REACTIONS.[24]"\
"THE ELECTRICAL DRIVING FORCE OR  ACROSS THE TERMINALS OF A CELL IS KNOWN AS THE TERMINAL VOLTAGE (DIFFERENCE) AND IS MEASURED IN VOLTS.[25] "\
"THE TERMINAL VOLTAGE OF A CELL THAT IS NEITHER CHARGING NOR DISCHARGING IS CALLED THE OPEN-CIRCUIT VOLTAGE AND EQUALS THE EMF OF THE CELL. "\
"BECAUSE OF INTERNAL RESISTANCE,[26] THE TERMINAL VOLTAGE OF A CELL THAT IS DISCHARGING IS SMALLER IN MAGNITUDE THAN THE OPEN-CIRCUIT VOLTAGE "\
"AND THE TERMINAL VOLTAGE OF A CELL THAT IS CHARGING EXCEEDS THE OPEN-CIRCUIT VOLTAGE.[27] AN IDEAL CELL HAS NEGLIGIBLE INTERNAL RESISTANCE, "\
"SO IT WOULD MAINTAIN A CONSTANT TERMINAL VOLTAGE OF  UNTIL EXHAUSTED, THEN DROPPING TO ZERO. IF SUCH A CELL MAINTAINED 1.5 VOLTS AND STORED A "\
"CHARGE OF ONE COULOMB THEN ON COMPLETE DISCHARGE IT WOULD PERFORM 1.5 JOULE OF WORK.[25] IN ACTUAL CELLS, THE INTERNAL RESISTANCE INCREASES "\
"UNDER DISCHARGE,[26] AND THE OPEN CIRCUIT VOLTAGE ALSO DECREASES UNDER DISCHARGE. IF THE VOLTAGE AND RESISTANCE ARE PLOTTED AGAINST TIME, THE "\
"RESULTING GRAPHS TYPICALLY ARE A CURVE; THE SHAPE OF THE CURVE VARIES ACCORDING TO THE CHEMISTRY AND INTERNAL ARRANGEMENT EMPLOYED.[28]"\
"AS STATED ABOVE, THE VOLTAGE DEVELOPED ACROSS A CELL'S TERMINALS DEPENDS ON THE ENERGY RELEASE OF THE CHEMICAL REACTIONS OF ITS ELECTRODES AND "\
"ELECTROLYTE. ALKALINE AND ZINC–CARBON CELLS HAVE DIFFERENT CHEMISTRIES BUT APPROXIMATELY THE SAME EMF OF 1.5 VOLTS; LIKEWISE NICD AND NIMH "\
"CELLS HAVE DIFFERENT CHEMISTRIES, BUT APPROXIMATELY THE SAME EMF OF 1.2 VOLTS.[29] ON THE OTHER HAND THE HIGH ELECTROCHEMICAL POTENTIAL "\
"CHANGES IN THE REACTIONS OF LITHIUM COMPOUNDS GIVE LITHIUM CELLS EMFS OF 3 VOLTS OR MORE.[30]";

#define COUNT 0xFFFF

void printOut(std::map<std::string, std::vector<std::clock_t>> const& times)
{
    auto i = times.begin();
    auto e = times.end();
    std::cout << "Benchmark	std::string	q::String" << std::endl;
    for(; i != e; ++i)
	{
	   std::string current = i->first;
	   while (current.length() < 24)
	   { 
		   current += ' '; 
	   }
	   
	   {
		  std::stringstream out;
		  for (auto t : i->second)
		  {				
			  out << t << "\t\t";
		  }
		  out << i->second[0]*100 / i->second[1] << "%";
		  current += out.str();
	   }
	   
	   std::cout << current;
	   std::cout << std::endl;
    }
}


#define BENCH(x)\
	{\
		clock_t diff = clock() - start;\
		if (diff == 0) diff = 1;\
    	times[x].push_back(diff);\
		std::cout << "  " << x << std::endl;\
	}


template<class T>
void benchmark(std::string const& name, std::map<std::string, std::vector<clock_t>>& times)
{
    std::cout << "Benchmarking " << name << std::endl;
    T meh("hello, I am a short string, that will be used in this test.  It is long enough to allow good copies.");
    std::clock_t start = std::clock();
    for (unsigned int x = 0; x < COUNT; ++x)
	{
	   T m("hello, I am a short string, that will be used in this test.  It is long enough to allow good copies.");
    }
    BENCH("Ctor const char*");
    
    start = clock();
    for (unsigned int x = 0; x < COUNT*100; ++x)
	{
	   T m;
    }
    BENCH("Ctor empty");
    
    start = clock();
    for (unsigned int x = 0; x < COUNT; ++x)
	{
	   T m(meh);
    }
    BENCH("Copy ctor");
    
    start = clock();
    for (unsigned int x = 0; x < COUNT; ++x)
	{
	   T m(meh);
	   m += meh;
    }
    BENCH("Copy ctor + append");
    
    start = clock();
    for (unsigned int x = 0; x < COUNT; ++x)
	{
	   T m(meh, 3, 18);
    }
    BENCH("Copy ctor with num");
    
    start = clock();
    for (unsigned int x = 0; x < COUNT; ++x)
	{
	   T m("hello, I am a short string, that will be used in this test.  It is long enough to allow good copies.", 20);
    }
    BENCH("Ctor const char* n");
    
    start = clock();
    for (unsigned int x = 0; x < COUNT; ++x)
	{
	   T m(meh.begin() + 6, meh.end());
	   T k(m.begin() + 2, m.begin() + 8);
    }
    BENCH("Substr via ctor");
    
    start = clock();
    for (unsigned int x = 0; x < COUNT; ++x)
	{
	   T m = meh.substr(6, 15);
	   T k = m.substr(2, 7);
    }
    BENCH("Substr");

    {
	   start = clock();
	   T mm;
	   for (unsigned int x = 0; x < COUNT*10; ++x)
	   {
		  mm = meh;
	   }
	   BENCH("= operator string");
    }
    
    {
	   start = clock();
	   T mm;
	   for (unsigned int x = 0; x < COUNT*4; ++x)
	   {
		  mm = "hello";
	   }
	   BENCH("= operator char*");
    }
    
    start = clock();
    for (unsigned int x = 0; x < COUNT; ++x)
	{
	   T m;
	   for(unsigned i = 0; i < 8; ++i)
	   {
		  m += 'c';
	   }
    }
    BENCH("append char");
    
    start = clock();
    for (unsigned int x = 0; x < COUNT; ++x)
	{
	   T m;
	   for(unsigned i = 0; i < 8; ++i)
	   {
		  m += "cs";
	   }
    }
    BENCH("append char*");
    
    start = clock();
    {
	   T mmm("hello");
	   for (unsigned int x = 0; x < COUNT; ++x)
	   {
		  T m;
		  for(unsigned i = 0; i < 8; ++i)
		  {
			 m += mmm;
		  }
	   }
	   BENCH("append string");
    }
    
    {
	   start = clock();
	   T mmm("hello");
	   T bbb("hello world");
	   for (unsigned int x = 0; x < COUNT*10; ++x)
	   {
		  mmm.swap(bbb);
	   }
	   BENCH("swap");
    }

	T text(textSource.c_str());
	std::vector<T> tokens;
	start = clock();
	{
		for (unsigned int x = 0; x < (COUNT >> 5); ++x)
		{
			tokens = q::util::tokenize(text, T(" .,!?"));
		}
		BENCH("tokenize");
	}

	std::vector<T> manyTokens;
	manyTokens.reserve(100000);
	manyTokens = tokens;
	for (int i = 0; i < 100; i++)
	{
		if ((i & 3) == 0)
		{
			for (auto const& t: tokens)
			{
				manyTokens.push_back(t);
			}
		}
		else
		{
			for (auto const& t: tokens)
			{
				char buf[32];
				sprintf(buf, "_ count %d", i);
				manyTokens.push_back(t + buf);
			}
		}
	}
	std::map<T, int> testMap;
	start = clock();
	{
		for (unsigned int x = 0; x < 10; ++x)
		{
			for (auto const& t: manyTokens)
			{
				testMap.insert(std::make_pair(t, 1));
			}
		}
		BENCH("map insert");
	}
	T xx("xx");

 	std::unordered_map<T, int> testUMap;
	testUMap.reserve(100000);
	start = clock();
	{
		for (unsigned int x = 0; x < 5; ++x)
		{
			for (auto const& t: manyTokens)
			{
				testUMap.insert(std::make_pair(t, 1));
			}
		}
		BENCH("unordered_map insert");
	}

	start = clock();
	{
		for (unsigned int x = 0; x < 100; ++x)
		{
			for (auto const& t: manyTokens)
			{
				testMap.find(t);
			}
		}
		BENCH("map find");
	}
	start = clock();
	{
		for (unsigned int x = 0; x < 100; ++x)
		{
			for (auto const& t: manyTokens)
			{
				testUMap.find(t);
			}
		}
		BENCH("unordered_map find");
	}

    std::cout << "done" << std::endl;
}

//extern int qMallocCount;
//extern int qFreeCount;

struct ci_char_traits : public std::char_traits<char>
	// just inherit all the other functions
	//  that we don't need to override
{
	static bool eq(char c1, char c2)
	{
		return toupper(c1) == toupper(c2);
	}

	static bool lt(char c1, char c2)
	{ 
		return toupper(c1) <  toupper(c2); 
	}

	static int compare(const char* s1, const char* s2, size_t n)
	{
		return _memicmp(s1, s2, n); 
	}

	static const char* find(const char* s, int n, char a)
	{
		while (n-- > 0 && toupper(*s) != toupper(a))
		{
			++s;
		}
		return n >= 0 ? s : 0;
	}
};

BOOST_AUTO_TEST_CASE(test_q_String_Benchmark)
{
	std::cout << sizeof(std::string) << " " << sizeof(q::String) << std::endl;

	std::map<std::string, std::vector<std::clock_t>> times;

	typedef std::basic_string<char, ci_char_traits> stdstring;
	//typedef std::string stdstring;

#ifndef _DEBUG
// 	int mc = qMallocCount;
// 	int fc = qFreeCount;
	benchmark<stdstring>("std::string", times);
//	std::cout << "Malloc Count: " << qMallocCount - mc << "\tFree Count:" << qFreeCount - fc << std::endl;

//	mc = qMallocCount;
//	fc = qFreeCount;
	benchmark<q::String>("q::String", times);
//	std::cout << "Malloc Count: " << qMallocCount - mc << "\tFree Count:" << qFreeCount - fc << std::endl;

	printOut(times);
#endif
}

