#pragma once

#include <string>
#include <vector>
#include <cstdint>

#define SCBL_TYPE_INTEGER 0
#define SCBL_TYPE_FUNCTION 1
#define SCBL_CMD_END 2

#define SCBL_RUNTIME_OK 0
#define SCBL_PARSER_OK 0
#define SCBL_RUNTIME_ERROR 1
#define SCBL_PARSER_ERROR 1

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;

typedef uint8_t  ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;

namespace SCBL {
	typedef void (*callback)(std::vector <ui32>);
    
	class Token {
	public: 
		Token(ui8 p_Type, ui32 p_Data):
			Type(p_Type),
			Data(p_Data) 
		{};

		ui8 Type;
		ui32 Data;
	};

	class Function {
	public: 
		Function(std::string p_Name, callback p_Callback):
			Name(p_Name),
			Callback(p_Callback) 
		{};

		std::string Name;
		callback Callback;
	};

	class Constant {
	public: 
		Constant(std::string p_Name, ui32 p_Value):
			Name(p_Name),
			Value(p_Value) 
		{};

		std::string Name;
		ui32 Value;
	};

	class SCBL_Interpreter {
	public: 
		SCBL_Interpreter() {};

		i8 Parse(std::string p_Code) {
			ErrorMsg = "";
			Tokens = {};

			if (p_Code == "") 
				return SCBL_PARSER_OK;

			std::string token;
			for (i32 i = 0; i < (i32)p_Code.length(); ++ i) {
				token = "";

				while (p_Code[i] <= 32 and p_Code[i] >= 0)
					++ i;
        
				if ((p_Code[i] <= 32 and p_Code[i] >= 0) or i > (i32)p_Code.length()) 
					continue;
				else if (p_Code[i] >= '0' and p_Code[i] <= '9') {
					while (p_Code[i] >= '0' and p_Code[i] <= '9') {
						token += p_Code[i];
        
						++ i;
					};

					if (not (p_Code[i] <= 32 and p_Code[i] >= 0) and p_Code[i] != ';' and p_Code[i] != '{') {
						ErrorMsg = (std::string) "Unexpected symbol \"" + p_Code[i] + "\" in number";
        
						return SCBL_PARSER_ERROR;
					};
        
					Tokens.push_back(Token(SCBL_TYPE_INTEGER, std::stoi(token)));
        
					-- i;
				} else if (p_Code[i] == '"') {
					++ i;

					while (p_Code[i] != '"' and not (i > (i32)p_Code.length())) {
						Tokens.push_back(Token(SCBL_TYPE_INTEGER, p_Code[i]));

						++ i;
					};

					if (p_Code[i] != '"') {
						ErrorMsg = "String not closed";

						return SCBL_PARSER_ERROR;
 					};

					++ i;

					if (p_Code[i] != '~')
						Tokens.push_back(Token(SCBL_TYPE_INTEGER, 0));
 					else 
						++ i;
                	
					if (not (p_Code[i] <= 32 and p_Code[i] >= 0) and p_Code[i] != ';' and p_Code[i] != '{') {
						ErrorMsg = "Unexpected symbol after string end";

						return SCBL_PARSER_ERROR;
					};

					-- i;
				} else if (p_Code[i] == '{') {
					++ i;
                	
					while (p_Code[i] != '}' and not (i > (i32)p_Code.length()))
						++ i;

					if (p_Code[i] != '}') {
						ErrorMsg = "Comment not closed";

						return SCBL_PARSER_ERROR;
					};
				} else if (p_Code[i] == '#') {
					++ i;
	               	
					while (p_Code[i] != 10 and not (i > (i32)p_Code.length()))
						++ i;
				} else if (p_Code[i] == ';') 
					Tokens.push_back(Token(SCBL_CMD_END, 0));
				else {
					while (not (p_Code[i] <= 32 and p_Code[i] >= 0) and p_Code[i] != ';' and not (i > (i32)p_Code.length())) {
						token += p_Code[i];
                        
						++ i;
					};

					bool found = false;

					ui32 j;
					for (j = 0; j < Functions.size(); ++ j)
						if (Functions[j].Name == token) {
							found = true;

							break;
						};
        
					if (found) 
                        Tokens.push_back(Token(SCBL_TYPE_FUNCTION, j));
					else {
						found = false;

						for (j = 0; j < Constants.size(); ++ j)
							if (Constants[j].Name == token) {
								found = true;
	
								break;
							};
        
						if (found) 
							Tokens.push_back(Token(SCBL_TYPE_INTEGER, Constants[j].Value));
						else {
							ErrorMsg = "Identifier \"" + token + "\" not found";
        
							return SCBL_PARSER_ERROR;
						};
					};
        
					-- i;
				};
			};
        
			return SCBL_PARSER_OK;
		};
        
		i8 Run()  {
			for (i32 i = 0; i < (i32)Tokens.size(); ++ i) {
				if (Tokens[i].Type != SCBL_TYPE_FUNCTION) {
					ErrorMsg = "[" + std::to_string((int)i) + "] Expected function, got (" + (Tokens[i].Type == SCBL_CMD_END? "CMD_END, " : "INT, ") + ".Data:" + std::to_string(Tokens[i].Data) + ")";

					return SCBL_RUNTIME_ERROR;
				};

				ui32 FuncIdx = Tokens[i].Data;
				std::vector <ui32> Params;

				++ i;

				while (i < (i32)Tokens.size() and Tokens[i].Type != SCBL_CMD_END) {
					if (Tokens[i].Type != SCBL_TYPE_INTEGER) {
						ErrorMsg = "[" + std::to_string(i) + "] Expected integer, got (" + (Tokens[i].Type == SCBL_CMD_END? "CMD_END, " : "FUNC, ") + ".Data:" + std::to_string(Tokens[i].Data) + ")";

						return SCBL_RUNTIME_ERROR;
					};

					Params.push_back(Tokens[i].Data);

					++ i;
				};

				Functions[FuncIdx].Callback(Params);
			};

		    return SCBL_RUNTIME_OK;
		};
		
		void AddFunction(Function p_func) {
			Functions.push_back(p_func);
		};

		void SetFunction(Function p_func) {
			for (ui32 i = 0; i < (ui32)Functions.size(); ++ i) {
				if (Functions[i].Name == p_func.Name) {
					Functions[i] = p_func;

					return;
				};
			};
        	
			Functions.push_back(p_func);
		};
        
		void AddConstant(Constant p_const) {
			Constants.push_back(p_const);
		};

		void SetConstant(Constant p_const) {
			for (ui32 i = 0; i < (ui32)Constants.size(); ++ i) {
				if (Constants[i].Name == p_const.Name) {
					Constants[i] = p_const;

					return;
				};
			};
        	
			Constants.push_back(p_const);
		};
        
		std::string GetErrorMsg() {
			return ErrorMsg;
		};
        
	private:
		std::string ErrorMsg;

		std::vector <Token> Tokens;
		std::vector <Function> Functions;
		std::vector <Constant> Constants;
	};
};
