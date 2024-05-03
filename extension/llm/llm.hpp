#pragma once

#include "duckdb.hpp"
#include "llama.cpp/llama.h"

namespace duckdb{
    class LLMExtension: public Extension {
        void Load(DuckDB &db) override;
        std::string Name() override;
    };
}
