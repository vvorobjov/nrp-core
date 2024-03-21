#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <clang-c/Index.h>

//// Constants and templates

// Constant string containing the representation of 
const std::string constQualifierStr = "const ";

// Template for a registerInput statement
std::string registerInputTemplate = R"(
          fn->registerInput<<<N>>, <<ARG_TYPE_BASE>>, <<ARG_TYPE_BASE>>>("<<ARG_NAME>>");)";

// Template for a registerOutput statement
std::string registerOutputTemplate = R"(
          fn->registerOutput<<<N>>, <<ARG_TYPE_BASE>>>("<<ARG_NAME>>");)";

// Template for a emplace statement in the createFNMap object of the FN Factory module
std::string mapEmplaceStatementTemplate = R"(
  createFNMap.emplace("<<FUNCTION_NAME>>", [&] (const std::string& fn_name, FunctionalNodePolicies::ExecutionPolicy policy)  {
          std::function< <<FUNCTION_SIGNATURE>> > f = std::bind(&<<FUNCTION_NAME>>, <<STD_PLACEHOLDERS>>);
          auto fn = FunctionalNodeFactory::create< <<N_I>>, <<N_O>>, <<ARG_TYPES>>>(fn_name, f, policy);
          <<REGISTER_INPUTS>>
          <<REGISTER_OUTPUTS>>

          return fn;
      });
)";

//// Helper functions

// Looks for instances of 'toReplace' in 's' and replace them with 'replaceWith'
void findAndReplaceStr(std::string &s, std::string const &toReplace, std::string const &replaceWith) {
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos)
        return;

    s.replace(pos, toReplace.length(), replaceWith);
    findAndReplaceStr(s, toReplace, replaceWith);
}

// Converts a CXString to std::string, dispose the former, return the latter
std::string CXStringToStd(const CXString &s) {
    std::string result = clang_getCString(s);
    clang_disposeString(s);
    return result;
}

// Check if a CXType fulfills the constraints fo be an FN input parameter, must have the form: const T*
bool isArgFNInput(CXType argType) {
    if (argType.kind != CXType_Pointer)
        return false;

    argType = clang_getPointeeType(argType);

    if (!clang_isConstQualifiedType(argType))
        return false;

    // TODO: the former checks that the type is const T*, but T can still be complex
    //  clang_equalTypes and clang_getCanonicalType could be used if a way is found to remove the const qualifier from argType
    //  in later versions of clang there is clang_getUnqualifiedType
    return true;
}

// Returns the string representation of the unqualified version of argType assuming that it has the structure of an FN input parameter
std::string getInputArgBaseStr(CXType argType) {
    if (!isArgFNInput(argType))
        return "";

    // TODO: would be better to remove the const part from argType using clang, but can't find a way to do it. clang_getUnqualifiedType in more recent versions would help
    auto s = CXStringToStd(clang_getTypeSpelling(clang_getPointeeType(argType)));
    return s.substr(constQualifierStr.length());
}

// Check if a CXType fulfills the constraints fo be an FN output parameter, must have the form: T&
bool isArgFNOutput(CXType argType) {
    if (argType.kind != CXType_LValueReference)
        return false;

    argType = clang_getPointeeType(argType);

    if (clang_isConstQualifiedType(argType))
        return false;

    // TODO: the former checks that the type is (not const) T&, but T can still be complex
    //  clang_equalTypes and clang_getCanonicalType could be used if a way is found to remove the const qualifier from argType
    //  in later versions of clang there is clang_getUnqualifiedType
    return true;
}


// Returns the string representation of the unqualified version of argType assuming that it has the structure of an FN output parameter
std::string getOutputArgBaseStr(CXType argType) {
    if (!isArgFNOutput(argType))
        return "";
    else
        return CXStringToStd(clang_getTypeSpelling(clang_getPointeeType(argType)));
}

//// Main processing functions

// From a function prototype cursor, compiles the statements necessary to create a FNode from that function and adds them as a functor to the createFNMap map
std::string processFunctionPrototype(CXCursor cursor) {
    // Retrieve some function information from cursor
    auto type = clang_getCursorType(cursor);
    auto return_type = clang_getResultType(type);
    auto function_name = CXStringToStd(clang_getCursorSpelling(cursor));
    auto return_type_str = CXStringToStd(clang_getTypeSpelling(return_type));

    // Check return type is bool
    if (return_type.kind != CXType_Bool)
        throw std::invalid_argument("Can't process function: \"" + function_name + "\". Return type must be \"bool\"");

    // Parse parameters
    size_t n_inputs = 0;
    std::vector<std::string> arg_names;
    std::vector<std::string> arg_types_str;
    std::string registerInputsStatement = "";
    std::string registerOutputsStatement = "";

    int n_args = clang_Cursor_getNumArguments(cursor);
    for (int i = 0; i < n_args; ++i) {
        auto arg_cursor = clang_Cursor_getArgument(cursor, i);
        auto arg_name = CXStringToStd(clang_getCursorSpelling(arg_cursor));
        if (arg_name.empty())
            throw std::invalid_argument(
                    "Can't process function: \"" + function_name + "\". All function parameters must be named");

        auto arg_type = clang_getArgType(type, i);
        auto arg_type_str = CXStringToStd(clang_getTypeSpelling(arg_type));

        // Check function signature constraints
        auto isArgInput = isArgFNInput(arg_type);
        if ((isArgInput && i != n_inputs++) || (!isArgInput && !isArgFNOutput(arg_type)))
            throw std::invalid_argument("Can't process function: \"" + function_name +
                                        "\". Function parameters must be 0 or more input parameters (const T*) followed by 0 or more output parameters (T&)");

        // store arg info
        arg_names.push_back(arg_name);
        arg_types_str.push_back(arg_type_str);

        if (isArgInput) {
            auto regSt = registerInputTemplate;
            findAndReplaceStr(regSt, "<<N>>", std::to_string(i));
            findAndReplaceStr(regSt, "<<ARG_TYPE_BASE>>", getInputArgBaseStr(arg_type));
            findAndReplaceStr(regSt, "<<ARG_NAME>>", arg_name);
            registerInputsStatement.append(regSt);
        } else {
            auto regSt = registerOutputTemplate;
            findAndReplaceStr(regSt, "<<N>>", std::to_string(i - n_inputs));
            findAndReplaceStr(regSt, "<<ARG_TYPE_BASE>>", getOutputArgBaseStr(arg_type));
            findAndReplaceStr(regSt, "<<ARG_NAME>>", arg_name);
            registerOutputsStatement.append(regSt);
        }
    }

    // Build emplace statement
    std::string args_str;
    for (const auto &a_type: arg_types_str)
        args_str += a_type + ",";
    args_str.pop_back();

    std::string function_signature = return_type_str + " (" + args_str + ")";
    std::string placeholdersStr = "";
    for (unsigned int i = 1; i <= n_args; ++i)
        placeholdersStr += "std::placeholders::_" + std::to_string(i) + ",";
    placeholdersStr.pop_back();

    auto mapEmplaceStatement = mapEmplaceStatementTemplate;
    findAndReplaceStr(mapEmplaceStatement, "<<FUNCTION_NAME>>", function_name);
    findAndReplaceStr(mapEmplaceStatement, "<<FUNCTION_SIGNATURE>>", function_signature);
    findAndReplaceStr(mapEmplaceStatement, "<<N_I>>", std::to_string(n_inputs));
    findAndReplaceStr(mapEmplaceStatement, "<<N_O>>", std::to_string(n_args - n_inputs));
    findAndReplaceStr(mapEmplaceStatement, "<<ARG_TYPES>>", args_str);
    findAndReplaceStr(mapEmplaceStatement, "<<STD_PLACEHOLDERS>>", placeholdersStr);
    findAndReplaceStr(mapEmplaceStatement, "<<REGISTER_INPUTS>>", registerInputsStatement);
    findAndReplaceStr(mapEmplaceStatement, "<<REGISTER_OUTPUTS>>", registerOutputsStatement);

    return mapEmplaceStatement;
}


// Visitor function used to process a translation unit
CXChildVisitResult functionVisitor(CXCursor cursor, CXCursor /* parent */, CXClientData mapEmplaceStatements) {
    // Check if cursor refers to source file
    if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) == 0)
        return CXChildVisit_Continue;

    // Process function declarations
    if (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_FunctionDecl) {
        std::string *mapEmplaceStatementsStr = reinterpret_cast<std::string *>(mapEmplaceStatements);
        mapEmplaceStatementsStr->append(processFunctionPrototype(cursor));
    }

    return CXChildVisit_Recurse;
}

// Checks for generated diagnostics after parsing a translation unit. Exists with error if there are error diagnostics
void checkDiagnostics(CXTranslationUnit translationUnit) {
    int nbDiag = clang_getNumDiagnostics(translationUnit);

    bool foundError = false;
    for (unsigned int currentDiag = 0; currentDiag < nbDiag; ++currentDiag) {
        CXDiagnostic diagnotic = clang_getDiagnostic(translationUnit, currentDiag);

        // TODO: Ideally, only errors in the main source file would be considered, but, it seems like diagnostics are stopped after a fatal error, 
        // independently of where it happens. Therefore all errors must be considered or otherwise errors in the main source file could go undetected
        // if(clang_Location_isFromMainFile( clang_getDiagnosticLocation( diagnotic ) ) == 0)
        //   continue;

        CXString errorString = clang_formatDiagnostic(diagnotic, clang_defaultDiagnosticDisplayOptions());
        std::string tmp = CXStringToStd(errorString);

        if (clang_getDiagnosticSeverity(diagnotic) >= CXDiagnostic_Error)
            foundError = true;

        std::cerr << tmp << std::endl;
    }
    if (foundError) {
        std::cerr << "Please resolve these issues and try again." << std::endl;
        exit(-1);
    }
}

//// Main
int main(int argc, char *argv[]) {
    // First parameter should be the source file path, second fn_factory_module_template_dir, other parameters should be clang parameters
    std::string usageMsg = "USAGE: generateFnFactoryModule header_filepath fn_factory_module_template_dir [clang_args]";

    if (argc < 2) {
        std::cerr
                << "First positional argument missing, should be the path to a header file containing the function prototypes from which FN nodes will be generated."
                << std::endl << usageMsg << std::endl;
        exit(-1);
    }

    if (argc < 3) {
        std::cerr
                << "Second positional argument missing, should be the path to the directory containing fn_factory_module.cpp.template."
                << std::endl << usageMsg << std::endl;
        exit(-1);
    }

    // Read fn_factory_module.cpp skeleton
    std::filesystem::path templateDir(argv[2]);
    std::filesystem::path templateFile("fn_factory_module.cpp.template");
    std::filesystem::path templatePath = templateDir / templateFile;
    if (!std::filesystem::exists(templatePath)) {
        std::cerr << "[" << templatePath << "] not found." << std::endl;
        exit(-1);
    }

    std::ifstream t(templatePath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string moduleStr = buffer.str();

    // Parse and process translational unit
    std::cout << "Generating module from file: " << argv[1] << std::endl;
    char **clangArgs = nullptr;
    if (argc > 3) {
        clangArgs = &argv[3];
        std::cout << "Clang arguments: ";
        for (int i = 3; i < argc; ++i)
            std::cout << argv[i] << " ";
        std::cout << std::endl;
    }

    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
            index,
            argv[1], clangArgs, argc - 3,
            nullptr, 0,
            CXTranslationUnit_None);

    if (unit == nullptr) {
        std::cerr << "It was not possible to parse the source file: " << argv[1]
                  << ". Please review and correct this file." << std::endl;
        exit(-1);
    }

    checkDiagnostics(unit);

    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    std::string mapEmplaceStatements = "";

    clang_visitChildren(
            cursor,
            functionVisitor,
            &mapEmplaceStatements);

    findAndReplaceStr(moduleStr, "<<FUNCTION_PROTOTYPES.H>>", argv[1]);
    findAndReplaceStr(moduleStr, "<<MAP_EMPLACE_STATEMENTS>>", mapEmplaceStatements);

    std::ofstream out("fn_factory_module.cpp");
    out << moduleStr;
    out.close();

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return 0;
}