#include <_ctype.h>

#include <cctype>
#include <fstream>
#include <iostream>

using namespace std;

enum TokenType {
    Unknown,
    Identifier,
    Mul,
    OpenParen,
    Comma,
    CloseParen,
    Number,
};

struct Token {
    TokenType type;
    string lexeme;
};

optional<Token> get_next_token(ifstream &input) {
    char c;
    string lexeme;

    while ((c = input.get()) != EOF) {
        // one character tokens
        if (c == '(') {
            return Token{.type = TokenType::OpenParen, .lexeme = string("(")};
        } else if (c == ')') {
            return Token{.type = TokenType::CloseParen, .lexeme = string(")")};
        } else if (c == ',') {
            return Token{.type = TokenType::Comma, .lexeme = string(",")};
            // numbers
        } else if (isnumber(c)) {
            lexeme.push_back(c);
            while ((c = input.get()) != EOF && isnumber(c)) {
                lexeme.push_back(c);
            }
            input.unget();

            return Token{.type = TokenType::Number, .lexeme = lexeme};
            // ascii identifiers
        } else if (isalpha(c)) {
            lexeme.push_back(c);
            while ((c = input.get()) != EOF && isalpha(c)) {
                lexeme.push_back(c);

                if (lexeme == "mul") {
                    return Token{.type = TokenType::Mul, .lexeme = lexeme};
                }

                // kind of hacky but we have to do lookback
                if (lexeme.ends_with("mul")) {
                    input.unget();
                    input.unget();
                    input.unget();
                    return Token{.type = TokenType::Identifier,
                                 .lexeme = lexeme.substr(0, lexeme.size() - 3)};
                }
            }

            return Token{.type = TokenType::Identifier, .lexeme = lexeme};
        } else {
            lexeme.push_back(c);
            while ((c = input.get()) != EOF && !isnumber(c) && c != '(' &&
                   c != ')' && !isalpha(c)) {
                lexeme.push_back(c);
            }
            input.unget();

            return Token{.type = TokenType::Unknown, .lexeme = lexeme};
        }
    }

    return nullopt;
};

optional<Token> peek_next_token(ifstream &input) {
    auto token = get_next_token(input);

    if (token.has_value()) {
        for (auto c : token.value().lexeme) {
            input.unget();
        }

        return token;
    }

    return nullopt;
}

optional<Token> expect(ifstream &input, TokenType type) {
    optional<Token> t = get_next_token(input);
    if (!t.has_value()) {
        return nullopt;
    }

    if (t.value().type != type) {
        return nullopt;
    }

    return t;
}

void try_parse_mul(ifstream &input, vector<pair<int, int>> &stack) {
    auto mul = expect(input, TokenType::Mul);
    if (!mul.has_value()) {
        return;
    }

    auto open = expect(input, TokenType::OpenParen);
    if (!open.has_value()) {
        return;
    }

    auto first = expect(input, TokenType::Number);
    if (!first.has_value()) {
        return;
    }

    auto comma = expect(input, TokenType::Comma);
    if (!comma.has_value()) {
        return;
    }

    auto second = expect(input, TokenType::Number);
    if (!second.has_value()) {
        return;
    }

    auto close = expect(input, TokenType::CloseParen);
    if (!close.has_value()) {
        return;
    }

    stack.emplace_back(pair{std::atoi(first.value().lexeme.c_str()),
                            std::atoi(second.value().lexeme.c_str())});
}

int main() {
    ifstream in("./input/day3", ios::in);
    // ifstream in("./input/day3_example", ios::in);

    if (!in) {
        cerr << "Error reading file: " << strerror(errno) << std::endl;
        return 1;
    }

    vector<pair<int, int>> stack;

    while (true) {
        auto token = peek_next_token(in);
        if (token.has_value()) {
            if (token.value().type == TokenType::Mul) {
                try_parse_mul(in, stack);
            } else {
                // otherwise consume and move on
                get_next_token(in);
            }
        } else {
            break;
        }
    }

    long long result = 0;
    for (auto p : stack) {
        cout << "(" << p.first << ", " << p.second << ")" << endl;
        result += p.first * p.second;
    }

    cout << "PART 1 RESULT: " << result << endl;
}
