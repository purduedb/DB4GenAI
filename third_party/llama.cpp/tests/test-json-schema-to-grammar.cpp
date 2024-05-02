#ifdef NDEBUG
#undef NDEBUG
#endif

#include <fstream>
#include <sstream>
#include <regex>

#include "json-schema-to-grammar.h"
#include "grammar-parser.h"

static std::string trim(const std::string & source) {
    std::string s(source);
    s.erase(0,s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\r\t")+1);
    return std::regex_replace(s, std::regex("(^|\n)[ \t]+"), "$1");
}

enum TestCaseStatus {
    SUCCESS,
    FAILURE
};

struct TestCase {
    TestCaseStatus expected_status;
    std::string name;
    std::string schema;
    std::string expected_grammar;

    void _print_failure_header() const {
        fprintf(stderr, "#\n# Test '%s' failed.\n#\n%s\n", name.c_str(), schema.c_str());
    }
    void verify(const std::string & actual_grammar) const {
        if (trim(actual_grammar) != trim(expected_grammar)) {
        _print_failure_header();
        fprintf(stderr, "# EXPECTED:\n%s\n# ACTUAL:\n%s\n", expected_grammar.c_str(), actual_grammar.c_str());
        assert(false);
        }
    }
    void verify_expectation_parseable() const {
        try {
            auto state = grammar_parser::parse(expected_grammar.c_str());
            if (state.symbol_ids.find("root") == state.symbol_ids.end()) {
                throw std::runtime_error("Grammar failed to parse:\n" + expected_grammar);
            }
        } catch (const std::runtime_error & ex) {
            _print_failure_header();
            fprintf(stderr, "# GRAMMAR ERROR: %s\n", ex.what());
            assert(false);
        }
    }
    void verify_status(TestCaseStatus status) const {
        if (status != expected_status) {
            _print_failure_header();
            fprintf(stderr, "# EXPECTED STATUS: %s\n", expected_status == SUCCESS ? "SUCCESS" : "FAILURE");
            fprintf(stderr, "# ACTUAL STATUS: %s\n", status == SUCCESS ? "SUCCESS" : "FAILURE");
            assert(false);
        }
    }
};

static void write(const std::string & file, const std::string & content) {
    std::ofstream f;
    f.open(file.c_str());
    f << content.c_str();
    f.close();
}

static std::string read(const std::string & file) {
    std::ostringstream actuals;
    actuals << std::ifstream(file.c_str()).rdbuf();
    return actuals.str();
}

static void test_all(const std::string & lang, std::function<void(const TestCase &)> runner) {
    fprintf(stderr, "#\n# Testing JSON schema conversion (%s)\n#\n", lang.c_str());
    auto test = [&](const TestCase & tc) {
        fprintf(stderr, "- %s%s\n", tc.name.c_str(), tc.expected_status == FAILURE ? " (failure expected)" : "");
        runner(tc);
    };

    test({
        FAILURE,
        "unknown type",
        R"""({
            "type": "kaboom"
        })""",
        ""
    });

    test({
        FAILURE,
        "invalid type",
        R"""({
            "type": 123
        })""",
        ""
    });

    test({
        SUCCESS,
        "empty schema (object)",
        "{}",
        R"""(
            array ::= "[" space ( value ("," space value)* )? "]" space
            boolean ::= ("true" | "false") space
            null ::= "null" space
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            object ::= "{" space ( string ":" space value ("," space string ":" space value)* )? "}" space
            root ::= object
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
            value ::= object | array | string | number | boolean
        )"""
    });

    test({
        SUCCESS,
        "exotic formats",
        R"""({
            "items": [
                { "format": "date" },
                { "format": "uuid" },
                { "format": "time" },
                { "format": "date-time" }
            ]
        })""",
        R"""(
            date ::= [0-9] [0-9] [0-9] [0-9] "-" ( "0" [1-9] | "1" [0-2] ) "-" ( "0" [1-9] | [1-2] [0-9] | "3" [0-1] )
            date-string ::= "\"" date "\"" space
            date-time ::= date "T" time
            date-time-string ::= "\"" date-time "\"" space
            root ::= "[" space date-string "," space uuid "," space time-string "," space date-time-string "]" space
            space ::= " "?
            time ::= ([01] [0-9] | "2" [0-3]) ":" [0-5] [0-9] ":" [0-5] [0-9] ( "." [0-9] [0-9] [0-9] )? ( "Z" | ( "+" | "-" ) ( [01] [0-9] | "2" [0-3] ) ":" [0-5] [0-9] )
            time-string ::= "\"" time "\"" space
            uuid ::= "\"" [0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F] "-" [0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F] "-" [0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F] "-" [0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F] "-" [0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F] "\"" space
        )"""
    });

    test({
        SUCCESS,
        "string",
        R"""({
            "type": "string"
        })""",
        R"""(
            root ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "boolean",
        R"""({
            "type": "boolean"
        })""",
        R"""(
            root ::= ("true" | "false") space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "integer",
        R"""({
            "type": "integer"
        })""",
        R"""(
            root ::= ("-"? ([0-9] | [1-9] [0-9]*)) space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "string const",
        R"""({
            "const": "foo"
        })""",
        R"""(
            root ::= "\"foo\""
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "non-string const",
        R"""({
            "const": 123
        })""",
        R"""(
            root ::= "123"
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "non-string enum",
        R"""({
            "enum": ["red", "amber", "green", null, 42, ["foo"]]
        })""",
        R"""(
            root ::= "\"red\"" | "\"amber\"" | "\"green\"" | "null" | "42" | "[\"foo\"]"
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "tuple1",
        R"""({
            "prefixItems": [{ "type": "string" }]
        })""",
        R"""(
            root ::= "[" space string "]" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "tuple2",
        R"""({
            "prefixItems": [{ "type": "string" }, { "type": "number" }]
        })""",
        R"""(
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            root ::= "[" space string "," space number "]" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "number",
        R"""({
            "type": "number"
        })""",
        R"""(
            root ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "minItems",
        R"""({
            "items": {
                "type": "boolean"
            },
            "minItems": 2
        })""",
        R"""(
            boolean ::= ("true" | "false") space
            root ::= "[" space boolean ( "," space boolean )( "," space boolean )* "]" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "maxItems 1",
        R"""({
            "items": {
                "type": "boolean"
            },
            "maxItems": 1
        })""",
        R"""(
            boolean ::= ("true" | "false") space
            root ::= "[" space ( boolean  )? "]" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "maxItems 2",
        R"""({
            "items": {
                "type": "boolean"
            },
            "maxItems": 2
        })""",
        R"""(
            boolean ::= ("true" | "false") space
            root ::= "[" space ( boolean ( "," space boolean )? )? "]" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "min + maxItems",
        R"""({
            "items": {
                "type": ["number", "integer"]
            },
            "minItems": 3,
            "maxItems": 5
        })""",
        R"""(
            integer ::= ("-"? ([0-9] | [1-9] [0-9]*)) space
            item ::= number | integer
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            root ::= "[" space item ( "," space item )( "," space item )( "," space item )?( "," space item )? "]" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "simple regexp",
        R"""({
            "type": "string",
            "pattern": "^abc?d*efg+(hij)?kl$"
        })""",
        R"""(
            root ::= "\"" "ab" "c"? "d"* "ef" "g"+ ("hij")? "kl" "\"" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "regexp escapes",
        R"""({
            "type": "string",
            "pattern": "^\\[\\]\\{\\}\\(\\)\\|\\+\\*\\?$"
        })""",
        R"""(
            root ::= "\"" "[]{}()|+*?" "\"" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "regexp quote",
        R"""({
            "type": "string",
            "pattern": "^\"$"
        })""",
        R"""(
            root ::= "\"" "\"" "\"" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "regexp",
        R"""({
            "type": "string",
            "pattern": "^(\\([0-9]{1,3}\\))?[0-9]{3}-[0-9]{4} and...$"
        })""",
        R"""(
            dot ::= [\U00000000-\x09\x0B\x0C\x0E-\U0010FFFF]
            root ::= "\"" ("(" root-1 root-1? root-1? ")")? root-1 root-1 root-1 "-" root-1 root-1 root-1 root-1 " and" dot dot dot "\"" space
            root-1 ::= [0-9]
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "required props in original order",
        R"""({
            "type": "object",
            "properties": {
                "b": {"type": "string"},
                "c": {"type": "string"},
                "a": {"type": "string"}
            },
            "required": [
                "a",
                "b",
                "c"
            ],
            "additionalProperties": false,
            "definitions": {}
        })""",
        R"""(
            a-kv ::= "\"a\"" space ":" space string
            b-kv ::= "\"b\"" space ":" space string
            c-kv ::= "\"c\"" space ":" space string
            root ::= "{" space b-kv "," space c-kv "," space a-kv "}" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "1 optional prop",
        R"""({
            "properties": {
                "a": {
                "type": "string"
                }
            },
            "additionalProperties": false
        })""",
        R"""(
            a-kv ::= "\"a\"" space ":" space string
            root ::= "{" space  (a-kv )? "}" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "N optional props",
        R"""({
            "properties": {
                "a": {"type": "string"},
                "b": {"type": "string"},
                "c": {"type": "string"}
            },
            "additionalProperties": false
        })""",
        R"""(
            a-kv ::= "\"a\"" space ":" space string
            a-rest ::= ( "," space b-kv )? b-rest
            b-kv ::= "\"b\"" space ":" space string
            b-rest ::= ( "," space c-kv )?
            c-kv ::= "\"c\"" space ":" space string
            root ::= "{" space  (a-kv a-rest | b-kv b-rest | c-kv )? "}" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "required + optional props each in original order",
        R"""({
            "properties": {
                "b": {"type": "string"},
                "a": {"type": "string"},
                "d": {"type": "string"},
                "c": {"type": "string"}
            },
            "required": ["a", "b"],
            "additionalProperties": false
        })""",
        R"""(
            a-kv ::= "\"a\"" space ":" space string
            b-kv ::= "\"b\"" space ":" space string
            c-kv ::= "\"c\"" space ":" space string
            d-kv ::= "\"d\"" space ":" space string
            d-rest ::= ( "," space c-kv )?
            root ::= "{" space b-kv "," space a-kv ( "," space ( d-kv d-rest | c-kv ) )? "}" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "additional props",
        R"""({
            "type": "object",
            "additionalProperties": {"type": "array", "items": {"type": "number"}}
        })""",
        R"""(
            additional-kv ::= string ":" space additional-value
            additional-kvs ::= additional-kv ( "," space additional-kv )*
            additional-value ::= "[" space ( number ( "," space number )* )? "]" space
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            root ::= "{" space  (additional-kvs )? "}" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "additional props (true)",
        R"""({
            "type": "object",
            "additionalProperties": true
        })""",
        R"""(
            array ::= "[" space ( value ("," space value)* )? "]" space
            boolean ::= ("true" | "false") space
            null ::= "null" space
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            object ::= "{" space ( string ":" space value ("," space string ":" space value)* )? "}" space
            root ::= object
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
            value ::= object | array | string | number | boolean
        )"""
    });

    test({
        SUCCESS,
        "additional props (implicit)",
        R"""({
            "type": "object"
        })""",
        R"""(
            array ::= "[" space ( value ("," space value)* )? "]" space
            boolean ::= ("true" | "false") space
            null ::= "null" space
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            object ::= "{" space ( string ":" space value ("," space string ":" space value)* )? "}" space
            root ::= object
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
            value ::= object | array | string | number | boolean
        )"""
    });

    test({
        SUCCESS,
        "empty w/o additional props",
        R"""({
            "type": "object",
            "additionalProperties": false
        })""",
        R"""(
            root ::= "{" space  "}" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "required + additional props",
        R"""({
            "type": "object",
            "properties": {
                "a": {"type": "number"}
            },
            "required": ["a"],
            "additionalProperties": {"type": "string"}
        })""",
        R"""(
            a-kv ::= "\"a\"" space ":" space number
            additional-kv ::= string ":" space string
            additional-kvs ::= additional-kv ( "," space additional-kv )*
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            root ::= "{" space a-kv ( "," space ( additional-kvs ) )? "}" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "optional + additional props",
        R"""({
            "type": "object",
            "properties": {
                "a": {"type": "number"}
            },
            "additionalProperties": {"type": "number"}
        })""",
        R"""(
            a-kv ::= "\"a\"" space ":" space number
            a-rest ::= additional-kvs
            additional-kv ::= string ":" space number
            additional-kvs ::= additional-kv ( "," space additional-kv )*
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            root ::= "{" space  (a-kv a-rest | additional-kvs )? "}" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "required + optional + additional props",
        R"""({
            "type": "object",
            "properties": {
                "a": {"type": "number"},
                "b": {"type": "number"}
            },
            "required": ["a"],
            "additionalProperties": {"type": "number"}
        })""",
        R"""(
            a-kv ::= "\"a\"" space ":" space number
            additional-kv ::= string ":" space number
            additional-kvs ::= additional-kv ( "," space additional-kv )*
            b-kv ::= "\"b\"" space ":" space number
            b-rest ::= additional-kvs
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            root ::= "{" space a-kv ( "," space ( b-kv b-rest | additional-kvs ) )? "}" space
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "top-level $ref",
        R"""({
            "$ref": "#/definitions/MyType",
            "definitions": {
                "MyType": {
                    "type": "object",
                    "properties": {
                        "a": {
                            "type": "string"
                        }
                    },
                    "required": [
                        "a"
                    ],
                    "additionalProperties": false
                }
            }
        })""",
        R"""(
            MyType ::= "{" space MyType-a-kv "}" space
            MyType-a-kv ::= "\"a\"" space ":" space string
            root ::= MyType
            space ::= " "?
            string ::=  "\"" (
                    [^"\\] |
                    "\\" (["\\/bfnrt] | "u" [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F] [0-9a-fA-F])
                    )* "\"" space
        )"""
    });

    test({
        SUCCESS,
        "anyOf",
        R"""({
            "anyOf": [
                {"$ref": "#/definitions/foo"},
                {"$ref": "#/definitions/bar"}
            ],
            "definitions": {
                "foo": {
                    "properties": {"a": {"type": "number"}}
                },
                "bar": {
                    "properties": {"b": {"type": "number"}}
                }
            },
            "type": "object"
        })""",
        R"""(
            alternative-0 ::= foo
            alternative-1 ::= bar
            bar ::= "{" space  (bar-b-kv )? "}" space
            bar-b-kv ::= "\"b\"" space ":" space number
            foo ::= "{" space  (foo-a-kv )? "}" space
            foo-a-kv ::= "\"a\"" space ":" space number
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            root ::= alternative-0 | alternative-1
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "mix of allOf, anyOf and $ref (similar to https://json.schemastore.org/tsconfig.json)",
        R"""({
            "allOf": [
                {"$ref": "#/definitions/foo"},
                {"$ref": "#/definitions/bar"},
                {
                "anyOf": [
                    {"$ref": "#/definitions/baz"},
                    {"$ref": "#/definitions/bam"}
                ]
                }
            ],
            "definitions": {
                "foo": {
                    "properties": {"a": {"type": "number"}}
                },
                "bar": {
                    "properties": {"b": {"type": "number"}}
                },
                "bam": {
                    "properties": {"c": {"type": "number"}}
                },
                "baz": {
                    "properties": {"d": {"type": "number"}}
                }
            },
            "type": "object"
        })""",
        R"""(
            a-kv ::= "\"a\"" space ":" space number
            b-kv ::= "\"b\"" space ":" space number
            c-kv ::= "\"c\"" space ":" space number
            d-kv ::= "\"d\"" space ":" space number
            d-rest ::= ( "," space c-kv )?
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            root ::= "{" space a-kv "," space b-kv ( "," space ( d-kv d-rest | c-kv ) )? "}" space
            space ::= " "?
        )"""
    });

    test({
        SUCCESS,
        "conflicting names",
        R"""({
            "type": "object",
            "properties": {
                "number": {
                "type": "object",
                "properties": {
                    "number": {
                    "type": "object",
                        "properties": {
                            "root": {
                                "type": "number"
                            }
                        },
                        "required": [
                            "root"
                        ],
                        "additionalProperties": false
                    }
                },
                "required": [
                    "number"
                ],
                "additionalProperties": false
                }
            },
            "required": [
                "number"
            ],
            "additionalProperties": false,
            "definitions": {}
        })""",
        R"""(
            number ::= ("-"? ([0-9] | [1-9] [0-9]*)) ("." [0-9]+)? ([eE] [-+]? [0-9]+)? space
            number- ::= "{" space number-number-kv "}" space
            number-kv ::= "\"number\"" space ":" space number-
            number-number ::= "{" space number-number-root-kv "}" space
            number-number-kv ::= "\"number\"" space ":" space number-number
            number-number-root-kv ::= "\"root\"" space ":" space number
            root ::= "{" space number-kv "}" space
            space ::= " "?
        )"""
    });
}

int main() {
    fprintf(stderr, "LLAMA_NODE_AVAILABLE = %s\n", getenv("LLAMA_NODE_AVAILABLE") ? "true" : "false");
    fprintf(stderr, "LLAMA_PYTHON_AVAILABLE = %s\n", getenv("LLAMA_PYTHON_AVAILABLE") ? "true" : "false");

    test_all("C++", [](const TestCase & tc) {
        try {
            tc.verify(json_schema_to_grammar(nlohmann::ordered_json::parse(tc.schema)));
            tc.verify_status(SUCCESS);
        } catch (const std::runtime_error & ex) {
            fprintf(stderr, "Error: %s\n", ex.what());
            tc.verify_status(FAILURE);
        }
    });

    if (getenv("LLAMA_PYTHON_AVAILABLE") || (std::system("python --version") == 0)) {
        test_all("Python", [](const TestCase & tc) {
            write("test-json-schema-input.tmp", tc.schema);
            tc.verify_status(std::system(
                "python ./examples/json-schema-to-grammar.py test-json-schema-input.tmp > test-grammar-output.tmp") == 0 ? SUCCESS : FAILURE);
            tc.verify(read("test-grammar-output.tmp"));
        });
    } else {
        fprintf(stderr, "\033[33mWARNING: Python not found, skipping Python JSON schema -> grammar tests.\n\033[0m");
    }

    if (getenv("LLAMA_NODE_AVAILABLE") || (std::system("node --version") == 0)) {
        test_all("JavaScript", [](const TestCase & tc) {
            write("test-json-schema-input.tmp", tc.schema);
            tc.verify_status(std::system(
                "node ./tests/run-json-schema-to-grammar.mjs test-json-schema-input.tmp > test-grammar-output.tmp") == 0 ? SUCCESS : FAILURE);
            tc.verify(read("test-grammar-output.tmp"));
        });
    } else {
        fprintf(stderr, "\033[33mWARNING: Node not found, skipping JavaScript JSON schema -> grammar tests.\n\033[0m");
    }

    test_all("Check Expectations Validity", [](const TestCase & tc) {
        if (tc.expected_status == SUCCESS) {
            tc.verify_expectation_parseable();
        }
    });
}
