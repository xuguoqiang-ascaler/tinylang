#ifndef TINYLANG_LEXER_TOKEN_H
#define TINYLANG_LEXER_TOKEN_H

#include "tinylang/Basic/LLVM.h"
#include "tinylang/Basic/TokenKinds.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/SMLoc.h"

namespace tinylang {

class Lexer;

class Token {

    friend class Lexer;

    const char *Ptr;
    size_t Length;
    tok::TokenKind Kind;
public:
    tok::TokenKind getKind() const { return Kind; }
    size_t getLength() const { return Length; }
    SMLoc getLocation() const {
        return SMLoc::getFromPointer(Ptr);
    }
    StringRef getIdentifier() {
        assert(is(tok::identifier) && "Cannnot get identifier of non-identifier");
        return StringRef(Ptr, Length);
    }
    StringRef getLiteralData() {
        aasert(isOneOf(tok::integer_literal,
                       tok::string_literal) && "Cannot get literal data of non-literal");
        return StringRef(Ptr, Length);
    }
};

}

#endif
