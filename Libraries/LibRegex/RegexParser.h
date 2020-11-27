/*
 * Copyright (c) 2020, Emanuel Sprung <emanuel.sprung@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "RegexByteCode.h"
#include "RegexError.h"
#include "RegexLexer.h"
#include "RegexOptions.h"

#include <AK/Forward.h>
#include <AK/StringBuilder.h>
#include <AK/Types.h>
#include <AK/Vector.h>

namespace regex {

class PosixExtendedParser;

template<typename T>
struct GenericParserTraits {
    using OptionsType = T;
};

template<typename T>
struct ParserTraits : public GenericParserTraits<T> {
};

template<>
struct ParserTraits<PosixExtendedParser> : public GenericParserTraits<PosixOptions> {
};

class Parser {
public:
    struct Result {
        ByteCode bytecode;
        size_t capture_groups_count;
        size_t named_capture_groups_count;
        size_t match_length_minimum;
        Error error;
        Token error_token;
    };

    explicit Parser(Lexer& lexer)
        : m_parser_state(lexer)
    {
    }

    Parser(Lexer& lexer, AllOptions regex_options)
        : m_parser_state(lexer, regex_options)
    {
    }

    virtual ~Parser() = default;

    Result parse(Optional<AllOptions> regex_options = {});
    bool has_error() const { return m_parser_state.error != Error::NoError; }
    Error error() const { return m_parser_state.error; }

protected:
    virtual bool parse_internal(ByteCode&, size_t& match_length_minimum) = 0;

    ALWAYS_INLINE bool match(TokenType type) const;
    ALWAYS_INLINE bool match(char ch) const;
    ALWAYS_INLINE Token consume();
    ALWAYS_INLINE Token consume(TokenType type, Error error);
    ALWAYS_INLINE bool consume(const String&);
    ALWAYS_INLINE void reset();
    ALWAYS_INLINE bool done() const;
    ALWAYS_INLINE bool set_error(Error error);

    struct ParserState {
        Lexer& lexer;
        Token current_token;
        Error error = Error::NoError;
        Token error_token { TokenType::Eof, 0, StringView(nullptr) };
        ByteCode bytecode;
        size_t capture_groups_count { 0 };
        size_t named_capture_groups_count { 0 };
        size_t match_length_minimum { 0 };
        AllOptions regex_options;
        explicit ParserState(Lexer& lexer)
            : lexer(lexer)
            , current_token(lexer.next())
        {
        }
        explicit ParserState(Lexer& lexer, AllOptions regex_options)
            : lexer(lexer)
            , current_token(lexer.next())
            , regex_options(regex_options)
        {
        }
    };

    ParserState m_parser_state;
};

class PosixExtendedParser final : public Parser {
public:
    explicit PosixExtendedParser(Lexer& lexer)
        : Parser(lexer) {};
    PosixExtendedParser(Lexer& lexer, Optional<typename ParserTraits<PosixExtendedParser>::OptionsType> regex_options)
        : Parser(lexer, regex_options.value_or({})) {};
    ~PosixExtendedParser() = default;

private:
    ALWAYS_INLINE bool match_repetition_symbol();
    ALWAYS_INLINE bool match_ordinary_characters();

    bool parse_internal(ByteCode&, size_t&) override;

    bool parse_root(ByteCode&, size_t&);
    ALWAYS_INLINE bool parse_sub_expression(ByteCode&, size_t&);
    ALWAYS_INLINE bool parse_bracket_expression(ByteCode&, size_t&);
    ALWAYS_INLINE bool parse_repetition_symbol(ByteCode&, size_t&);
};

using PosixExtended = PosixExtendedParser;

}

using regex::PosixExtended;