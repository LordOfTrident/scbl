#include <iostream>
#include <fstream>
#include <cstdio>

#include "../scbl.hh"

bool FileExists(const std::string& p_name) {
	std::ifstream fhnd(p_name);
    
	return fhnd.good();
};

std::string ReadFile(const std::string& p_fname) {
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

void WriteFile(const std::string& p_fname, const std::string& p_txt) {
	std::ofstream fhnd(p_fname);

	fhnd << p_txt;
};

bool running = false;
ui8 exitcode = 0;

int main(int argc, const char* argv[]) {
	SCBL::SCBL_Interpreter SCBLi;

	SCBLi.AddConstant(SCBL::Constant("NUL", 0,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("SOH", 1,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("STX", 2,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("ETX", 3,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("EOT", 4,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("ENQ", 5,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("ACK", 6,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("BEL", 7,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("BS",  8,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("TAB", 9,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("NL",  10, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("VT",  11, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("FF",  12, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("CR",  13, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("SO",  14, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("SI",  15, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("DLE", 16, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("DC1", 17, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("DC2", 18, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("DC3", 19, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("DC4", 20, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("NAK", 21, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("SYN", 22, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("ETB", 23, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("CAN", 24, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("EM",  25, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("SUB", 26, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("ESC", 27, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("FS",  28, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("GS",  29, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("RS",  30, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("US",  31, SCBL_CONSTANT_1BYTE));
	
	SCBLi.AddFunction(SCBL::Function("print", [] (std::vector <ui8> Parameters, const void* _) {
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			std::cout << (char)Parameters[i];
	}));
	
	SCBLi.AddFunction(SCBL::Function("printf", [] (std::vector <ui8> Parameters, const void* _) {
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			std::cout << (char)Parameters[i];
	    
		std::cout << std::endl;
	}));
	
	SCBLi.AddFunction(SCBL::Function("printfr", [] (std::vector <ui8> Parameters, const void* _) {
		SCBL::ParameterHandler phnd(Parameters);
		
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) {
			try {
				ui32 Out = phnd.GetNextParam32();
				
				std::cout << Out;
			} catch (...) {
				break;
			};
		};

		std::cout << std::endl;
	}));
	
	SCBLi.AddFunction(SCBL::Function("printr", [] (std::vector <ui8> Parameters, const void* _) {
		SCBL::ParameterHandler phnd(Parameters);
				
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) {
			try {
				ui32 Out = phnd.GetNextParam32();
				
				std::cout << Out;
			} catch (...) {
				break;
			};
		};
	}));

	SCBLi.AddFunction(SCBL::Function("cprintr", [] (std::vector <ui8> Parameters, const void* _) {
		SCBL::ParameterHandler phnd(Parameters);
						
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) {
			try {
				ui32 Out = phnd.GetNextParam32();
				
				printf("%i", Out);
			} catch (...) {
				break;
			};
		};
	}));
	
	SCBLi.AddFunction(SCBL::Function("cprint", [] (std::vector <ui8> Parameters, const void* _) {
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			printf("%c", Parameters[i]);
	}));

	SCBLi.AddFunction(SCBL::Function("puts", [] (std::vector <ui8> Parameters, const void* _) {
		std::string str = "";
		
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) 
			str += std::string(1, Parameters[i]);

		puts(str.c_str());
	}));
	
	SCBLi.AddFunction(SCBL::Function("putr", [] (std::vector <ui8> Parameters, const void* _) {
		SCBL::ParameterHandler phnd(Parameters);
		std::string str = "";
						
		for (ui32 i = 0; i < (ui32)Parameters.size(); ++ i) {
			try {
				ui32 Out = phnd.GetNextParam32();
				
				str += std::to_string(Out);
			} catch (...) {
				break;
			};
		};
		
		puts(str.c_str());
	}));

	SCBLi.AddFunction(SCBL::Function("stof", [] (std::vector <ui8> Parameters, const void* _) {
		SCBL::ParameterHandler phnd(Parameters);

		try {
			std::string fname = phnd.GetNextParamStr();
			std::string fcontents = phnd.GetNextParamStr();
			
			WriteFile(fname, fcontents);
		} catch (...) {
			std::cout << "Expected 2 string parameters for function 'stof'" << std::endl;
			
			return;	
		};
	}));
	
	SCBLi.AddFunction(SCBL::Function("exit", [] (std::vector <ui8> Parameters, const void* _) {
		SCBL::ParameterHandler phnd(Parameters);
		running = false;

		try {
			exitcode = phnd.GetNextParamInt();
		} catch (...) {};
	}));
	
	SCBLi.AddFunction(SCBL::Function("help", [] (std::vector <ui8> Parameters, const void* _) {
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
			std::cout << (std::string)"E: File '" + argv[1] + "' not found" << std::endl;
		
			return 0;
		};

		try {
			SCBLi.Parse(ReadFile(argv[1]));
			SCBLi.Run();
		} catch (std::exception &Error) {
			std::cout << "\nE: " << Error.what() << std::endl;

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

		try {
			SCBLi.Parse(in);
			SCBLi.Run();
		} catch (std::exception &Error) {
			std::cout << "\nE: " << Error.what() << std::endl;
		};

		std::cout << std::endl;
	};

	return exitcode;	
};

