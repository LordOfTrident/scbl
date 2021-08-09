#include <iostream>
#include <fstream>
#include <cstdio>

#include "../scbl.hh"

bool FileExists(std::string p_name) {
	std::ifstream fhnd(p_name);
    
	return fhnd.good();
};

std::string ReadFile(std::string p_fname) {
	std::string fstr = "";
	std::ifstream fhnd(p_fname);

	if (fhnd.is_open()) {
		std::string fln = "";

		while (getline(fhnd, fln))
			fstr += fln + '\n';
	};

	fhnd.close();

	return fstr;
};

void WriteFile(std::string p_fname, std::string p_txt) {
	std::ofstream fhnd(p_fname);

	fhnd << p_txt;
};

bool running = false;
ui8 exitcode = 0;

int main(int argc, char* argv[]) {
	SCBL::SCBL_Interpreter SCBLi;

	SCBLi.AddConstant(SCBL::Constant("NUL", 0));
	SCBLi.AddConstant(SCBL::Constant("SOH", 1));
	SCBLi.AddConstant(SCBL::Constant("STX", 2));
	SCBLi.AddConstant(SCBL::Constant("ETX", 3));
	SCBLi.AddConstant(SCBL::Constant("EOT", 4));
	SCBLi.AddConstant(SCBL::Constant("ENQ", 5));
	SCBLi.AddConstant(SCBL::Constant("ACK", 6));
	SCBLi.AddConstant(SCBL::Constant("BEL", 7));
	SCBLi.AddConstant(SCBL::Constant("BS", 8));
	SCBLi.AddConstant(SCBL::Constant("TAB", 9));
	SCBLi.AddConstant(SCBL::Constant("NL", 10));
	SCBLi.AddConstant(SCBL::Constant("VT", 11));
	SCBLi.AddConstant(SCBL::Constant("FF", 12));
	SCBLi.AddConstant(SCBL::Constant("CR", 13));
	SCBLi.AddConstant(SCBL::Constant("SO", 14));
	SCBLi.AddConstant(SCBL::Constant("SI", 15));
	SCBLi.AddConstant(SCBL::Constant("DLE", 16));
	SCBLi.AddConstant(SCBL::Constant("DC1", 17));
	SCBLi.AddConstant(SCBL::Constant("DC2", 18));
	SCBLi.AddConstant(SCBL::Constant("DC3", 19));
	SCBLi.AddConstant(SCBL::Constant("DC4", 20));
	SCBLi.AddConstant(SCBL::Constant("NAK", 21));
	SCBLi.AddConstant(SCBL::Constant("SYN", 22));
	SCBLi.AddConstant(SCBL::Constant("ETB", 23));
	SCBLi.AddConstant(SCBL::Constant("CAN", 24));
	SCBLi.AddConstant(SCBL::Constant("EM", 25));
	SCBLi.AddConstant(SCBL::Constant("SUB", 26));
	SCBLi.AddConstant(SCBL::Constant("ESC", 27));
	SCBLi.AddConstant(SCBL::Constant("FS", 28));
	SCBLi.AddConstant(SCBL::Constant("GS", 29));
	SCBLi.AddConstant(SCBL::Constant("RS", 30));
	SCBLi.AddConstant(SCBL::Constant("US", 31));
	SCBLi.AddConstant(SCBL::Constant("SPACE", 32));

	for (i8 i = 33; i < 127; ++ i)
		SCBLi.AddConstant(SCBL::Constant(std::string(1, i), i));
	
	SCBLi.AddFunction(SCBL::Function("print", [] (std::vector <ui32> Parameters, void* _) {
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			std::cout << (char)Parameters[i];
	}));
	
	SCBLi.AddFunction(SCBL::Function("printf", [] (std::vector <ui32> Parameters, void* _) {
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			std::cout << (char)Parameters[i];
	    
		std::cout << std::endl;
	}));
	
	SCBLi.AddFunction(SCBL::Function("printfr", [] (std::vector <ui32> Parameters, void* _) {
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			std::cout << Parameters[i];

		std::cout << std::endl;
	}));
	
	SCBLi.AddFunction(SCBL::Function("printr", [] (std::vector <ui32> Parameters, void* _) {
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			std::cout << Parameters[i];
	}));

	SCBLi.AddFunction(SCBL::Function("cprintr", [] (std::vector <ui32> Parameters, void* _) {
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			printf("%i", Parameters[i]);
	}));
	
	SCBLi.AddFunction(SCBL::Function("cprint", [] (std::vector <ui32> Parameters, void* _) {
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			printf("%c", Parameters[i]);
	}));

	SCBLi.AddFunction(SCBL::Function("puts", [] (std::vector <ui32> Parameters, void* _) {
		std::string str = "";
		
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			str += std::string(1, Parameters[i]);

		puts(str.c_str());
	}));
	
	SCBLi.AddFunction(SCBL::Function("putr", [] (std::vector <ui32> Parameters, void* _) {
		std::string str = "";
	
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			str += std::to_string(Parameters[i]);

		puts(str.c_str());
	}));

	SCBLi.AddFunction(SCBL::Function("stof", [] (std::vector <ui32> Parameters, void* _) {
		std::string fname = "";
		std::string fcontents = "";

		for (ui32 i = 0; i < (ui32)Parameters.size() and Parameters[i] != 0; ++ i)
			fname += std::string(1, Parameters[i]);
	    
		for (ui32 i = fname.length() + 1; i < (ui32)Parameters.size() and Parameters[i] != 0; ++ i) 
			fcontents += std::string(1, Parameters[i]);
	    
		WriteFile(fname, fcontents);
	}));
	
	SCBLi.AddFunction(SCBL::Function("exit", [] (std::vector <ui32> Parameters, void* _) {
		running = false;
	    
		if (Parameters.size() >= 1)
			exitcode = Parameters[0];
	}));
	
	SCBLi.AddFunction(SCBL::Function("help", [] (std::vector <ui32> Parameters, void* _) {
		std::cout << "Available functions:\n";
		std::cout << " Console functions:\n";
		std::cout << "  print   <...> - Print out encoded characters using std::cout\n";
		std::cout << "  printf  <...> - Print out encoded characters using std::cout and flush the stream\n";
		std::cout << "  cprint  <...> - Print out encoded characters using printf\n";
		std::cout << "  puts    <...> - Print out a string using puts\n";
		std::cout << "  printr  <...> - Print out raw integers using std::cout\n";
		std::cout << "  printfr <...> - Print out raw integers using std::cout and flush the stream\n";
		std::cout << "  cprintr <...> - Print out raw integers using printf\n";
		std::cout << "  putr    <...> - Print out raw integers using puts\n";
		std::cout << "\n File functions:\n";
		std::cout << "  stof <filename> <contents> - Write a file\n";
		std::cout << "\n Other functions:\n";
		std::cout << "  exit <exitcode> - Exit the program\n";
	}));

	if (argc > 1) {
		if (not FileExists(argv[1])) {
			std::cout << (std::string)"File '" + argv[1] + "' not found" << std::endl;
		
			return 0;
		};

		if (SCBLi.Parse(ReadFile(argv[1])) != SCBL_PARSER_OK) {
			std::cout << SCBLi.GetErrorMsg() << std::endl;

			return 0;
		};

		if (SCBLi.Run() != SCBL_RUNTIME_OK) {
			std::cout << SCBLi.GetErrorMsg() << std::endl;

			return 0;
		};

		return exitcode;
	};

	std::cout << "SCBL Shell\nFor help type in 'help'." << std::endl;

	running = true;
	while (running) {
		std::string in;
		
		std::cout << "> ";
		std::getline(std::cin, in);

		if (SCBLi.Parse(in) != SCBL_PARSER_OK) {
			std::cout << SCBLi.GetErrorMsg() << std::endl;
			
			continue;
		};
		
		if (SCBLi.Run() != SCBL_RUNTIME_OK) {
			std::cout << SCBLi.GetErrorMsg() << std::endl;
			
			continue;
		};

		std::cout << std::endl;
	};

	return exitcode;	
};

