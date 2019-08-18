//
//  ScriptData.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 15/05/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "SystemComponentExtractor.hpp"
#include <clang-c/Index.h>
#include <iostream>
#include <set>
#include <map>
#include <sstream>

using namespace Mini;

void SystemComponentExtractor::Clear() {
    components.clear();
    systems.clear();
}

CXChildVisitResult parseCode(CXCursor cursor, CXCursor parent, CXClientData clientData);

bool SystemComponentExtractor::Extract(const std::string& clangPath, const std::vector<std::string> &cppFiles, const std::vector<std::string> &includePaths, const std::function<bool(const std::string&)>& predicate) {
    std::vector<std::string> arguments;
    arguments.push_back("-std=c++14");
    arguments.push_back("-stdlib=libc++");
    arguments.push_back("-I" + clangPath + "/include/c++/v1" );
    arguments.push_back("-I" + clangPath + "/lib/clang/7.0.0/include" );
    
    for(const auto& includePath : includePaths) {
        std::string extraPath = "-I" + includePath;
        arguments.push_back(extraPath.c_str());
    }
    
    for(auto cppFile : cppFiles) {
        CXIndex index = clang_createIndex(0,1);

        std::vector<const char*> argumentsList;
        for(auto& s : arguments) {
            argumentsList.push_back(s.c_str());
        }

        // create Translation Unit
        CXTranslationUnit tu = clang_parseTranslationUnit(index, cppFile.c_str(), &argumentsList[0], (int)argumentsList.size(), NULL, 0, 0);
        if (tu == NULL) {
            printf("Cannot parse translation unit\n");
            return false;
        }
        
        auto numDiagnostics = clang_getNumDiagnostics(tu);
        
        for (auto i=0; i<numDiagnostics; ++i) {
            CXDiagnostic diagnostic = clang_getDiagnostic(tu, i);
            
            CXString format = clang_formatDiagnostic(diagnostic, CXDiagnostic_DisplaySourceLocation | CXDiagnostic_DisplayColumn);
            
            std::string result      = clang_getCString( format );
            
            std::cout << result << std::endl;

            clang_disposeString( format );
            
            CXString category = clang_getDiagnosticCategoryText(diagnostic);
            
            std::string categoryString = clang_getCString( category );
            
            if (categoryString.find("Issue") != std::string::npos) {
                clang_disposeString(category);
            
                clang_disposeDiagnostic(diagnostic);
                return false;
            }
            
            std::cout << categoryString << std::endl;
            
            clang_disposeString(category);
            
            clang_disposeDiagnostic(diagnostic);
        }
        
        

        CXCursor startCursor = clang_getTranslationUnitCursor(tu);
        clang_visitChildren(startCursor, parseCode, this);
        
        clang_disposeTranslationUnit(tu);
        clang_disposeIndex(index);
    }
    
    std::map<std::string, System> uniqueSystems;
    
    for(auto& s : systems) {
        uniqueSystems[s.name] = s;
    }
    
    systems.clear();
    
    for(auto& s : uniqueSystems) {
        systems.push_back(s.second);
    }
    
    std::set<std::string> uniqueComponentNames;
    
    for(auto& s : systems) {
        for(auto& c : s.components) {
            auto found = c.find("struct ");
            if (found!=std::string::npos) {
                c = c.substr(0, found) + c.substr(found + 7, c.size() - found - 7);
            } else {
                found = c.find("class ");
                if (found!=std::string::npos) {
                    c = c.substr(0, found) + c.substr(found + 6, c.size() - found - 6);
                }
            }
            uniqueComponentNames.insert(c);
        }
    }
    
    Components systemComponents;
    
    for(auto& componentName : uniqueComponentNames) {
        Component* component = FindComponent(componentName);
        if (!component) continue;
        if (!predicate(componentName)) continue;
        systemComponents.push_back(*component);
    }
    
    components = systemComponents;
    
    return true;
}

SystemComponentExtractor::Component* SystemComponentExtractor::FindComponent(const std::string &name) {
    for(auto& c : components) {
        if (c.name == name) return &c;
    }
    return nullptr;
}

std::string getCursorSpelling2( CXCursor cursor )
{
  CXString cursorSpelling = clang_getCursorSpelling( cursor );
  std::string result      = clang_getCString( cursorSpelling );

  clang_disposeString( cursorSpelling );
  return result;
}

std::string getCursorType( CXCursor cursor )
{
  CXType type = clang_getCursorType( cursor );
  CXString cursorSpelling = clang_getTypeSpelling(type);

  std::string result      = clang_getCString( cursorSpelling );

  clang_disposeString( cursorSpelling );
  return result;
}

void PrintCursorLocation(CXCursor cursor) {

    CXSourceLocation location = clang_getCursorLocation(cursor);

    CXFile file;
    unsigned int line;
    unsigned int column;
    unsigned int offset;
    clang_getSpellingLocation(location, &file, &line, &column, &offset);

    CXString filePath = clang_getFileName(file);

    std::string filePathString = clang_getCString( filePath );



    clang_disposeString( filePath );
    
    std::cout << "file:"<< filePathString << ", line:"<<line<<", column:"<< column << std::endl;
}

CXChildVisitResult parseCode(CXCursor cursor, CXCursor parent, CXClientData clientData) {
    CXSourceLocation location = clang_getCursorLocation( cursor );
    if (clang_Location_isInSystemHeader(location) != 0) {
        return CXChildVisit_Continue;
    }
    
    SystemComponentExtractor& scriptData = *static_cast<SystemComponentExtractor*>(clientData);
    
    std::string cursorSpelling = getCursorSpelling2(cursor);
    
    static bool isSystem = false;
    static std::string nameSpace = "";
    
    if (parent.kind == CXCursor_TranslationUnit) {
        nameSpace = "";
        isSystem = false;
    }
    
    if (cursor.kind == CXCursor_Namespace) {
        nameSpace = cursorSpelling;
        isSystem = false;
    }
    else if (cursor.kind == CXCursor_StructDecl || cursor.kind == CXCursor_ClassDecl) {
        
        SystemComponentExtractor::Component component;
        if (nameSpace!="") {
            component.name = nameSpace +"::" + cursorSpelling;
        } else {
            component.name = cursorSpelling;
        }
        
        scriptData.components.push_back(component);
        
        isSystem = false;
    } else if (cursor.kind == CXCursor_TemplateRef && cursorSpelling == "System" &&
               (parent.kind != CXCursor_CXXMethod && parent.kind != CXCursor_MemberRefExpr)) {
        scriptData.systems.push_back({scriptData.components.back().name});
        isSystem = true;
    } else if (cursor.kind == CXCursor_TypeRef && isSystem && parent.kind == CXCursor_CXXBaseSpecifier) {
        scriptData.systems.back().components.push_back(cursorSpelling);
    } else if (cursor.kind == CXCursor_FieldDecl && !scriptData.components.empty()) {
        isSystem = false;
    
        //std::cout << scriptData.components.back().name<<":" << cursorSpelling << " : " << getCursorType(cursor) << std::endl;
        scriptData.components.back().fields.push_back({cursorSpelling, getCursorType(cursor)});
    }
    
    return CXChildVisit_Recurse;
}
