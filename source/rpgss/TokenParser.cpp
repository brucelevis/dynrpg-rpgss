/*
    The MIT License (MIT)

    Copyright (c) 2014 Anatoli Steinmark

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include <cstdlib>
#include <cstring>

#define NOT_MAIN_MODULE
#include <DynRPG/DynRPG.h>

#include "TokenParser.hpp"


namespace rpgss {

    //---------------------------------------------------------
    bool
    TokenParser::parse(const char* token)
    {
        ParsedType = ParsedUndefined;
        State = Start;
        int token_len = std::strlen(token);
        int vlu_depth = 0;
        int i = -1;
    next:
        if (++i == token_len) { // either empty or invalid token
            return false;
        }
        switch (State)
        {
            case Start:
            {
                switch (token[i])
                {
                    case 'V':
                    case 'v':
                        State = ParsingVariable;
                        goto next;
                    case 'S':
                    case 's':
                        State = ParsingSwitch;
                        goto next;
                    case 'C':
                    case 'c':
                        State = ParsingColor;
                        goto next;
                    case 'A':
                    case 'a':
                        State = ParsingActorName;
                        goto next;
                    default:
                        return false;
                }
            }
            case ParsingVariable:
            {
                switch (token[i])
                {
                    case 'V':
                    case 'v':
                        vlu_depth++;
                        goto next;
                    case 'S':
                    case 's':
                    case 'C':
                    case 'c':
                    case 'A':
                    case 'a':
                        return false;
                    default:
                    {
                        // convert rest of token to base 10 number, the first lookup index
                        char* endptr = 0;
                        int n = (int)std::strtoul((const char*)(token + i), &endptr, 10);
                        if ((n == 0 && endptr == token) || (endptr != token + token_len)) {
                             // either conversion error or not all characters consumed
                            return false;
                        }
                        // flatten variable lookup nesting, if any
                        while (vlu_depth-- > 0) {
                            if (n < 1 || n > RPG::system->variables.size) {
                                return false; // index out of bounds
                            }
                            n = RPG::variables[n];
                        }
                        // one last obstacle
                        if (n < 1 || n > RPG::system->variables.size) {
                            return false; // index out of bounds
                        }
                        // success!
                        ParsedType = ParsedNumber;
                        Number = RPG::variables[n];
                        return true;
                    }
                }
            }
            case ParsingSwitch:
            {
                switch (token[i])
                {
                    case 'V':
                    case 'v':
                        vlu_depth++;
                        goto next;
                    case 'S':
                    case 's':
                    case 'C':
                    case 'c':
                    case 'A':
                    case 'a':
                        return false;
                    default:
                    {
                        // convert rest of token to base 10 number, the first lookup index
                        char* endptr = 0;
                        int n = (int)std::strtoul((const char*)(token + i), &endptr, 10);
                        if ((n == 0 && endptr == token) || (endptr != token + token_len)) {
                             // either conversion error or not all characters consumed
                            return false;
                        }
                        // flatten variable lookup nesting, if any
                        while (vlu_depth-- > 0) {
                            if (n < 1 || n > RPG::system->variables.size) {
                                return false; // index out of bounds
                            }
                            n = RPG::variables[n];
                        }
                        // one last obstacle
                        if (n < 1 || n > RPG::system->switches.size) {
                            return false; // index out of bounds
                        }
                        // success!
                        ParsedType = ParsedBoolean;
                        Boolean = RPG::switches[n];
                        return true;
                    }
                }
            }
            case ParsingColor:
            {
                switch (token[i])
                {
                    case 'V':
                    case 'v':
                    case 'S':
                    case 's':
                    case 'C':
                    case 'c':
                    case 'A':
                    case 'a':
                        return false;
                    default:
                    {
                        // a valid color literal consists of exactly 8 hex characters
                        if (token_len - i != 8) {
                            return false;
                        }
                        // read rest of token as base 16 number, the color literal in RRGGBBAA format
                        char* endptr = 0;
                        unsigned long n = std::strtoul((const char*)(token + i), &endptr, 16);
                        if ((n == 0 && endptr == token) || (endptr != token + token_len)) {
                             // either conversion error or not all characters consumed
                            return false;
                        }
                        // success!
                        ParsedType = ParsedColor;
                        Color = n;
                        return true;
                    }
                }
            }
            case ParsingActorName:
            {
                switch (token[i])
                {
                    case 'V':
                    case 'v':
                        vlu_depth++;
                        goto next;
                    case 'S':
                    case 's':
                    case 'C':
                    case 'c':
                    case 'A':
                    case 'a':
                        return false;
                    default:
                    {
                        // convert rest of token to base 10 number, the first lookup index
                        char* endptr = 0;
                        int n = (int)std::strtoul((const char*)(token + i), &endptr, 10);
                        if ((n == 0 && endptr == token) || (endptr != token + token_len)) {
                             // either conversion error or not all characters consumed
                            return false;
                        }
                        // flatten variable lookup nesting, if any
                        while (vlu_depth-- > 0) {
                            if (n < 1 || n > RPG::system->variables.size) {
                                return false; // index out of bounds
                            }
                            n = RPG::variables[n];
                        }
                        // one last obstacle
                        if (n < 1 || n > RPG::actors.count()) {
                            return false; // index out of bounds
                        }
                        // success!
                        ParsedType = ParsedString;
                        String = RPG::actors[n]->getName();
                        return true;
                    }
                }
            }
            default:
            {
                break; // shut up compiler
            }
        } // switch (State)

        return false; // shut up compiler
    }

} // namespace rpgss
