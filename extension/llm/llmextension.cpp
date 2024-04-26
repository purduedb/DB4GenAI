#include "llmextension.hpp"
#include "third_party/llama.cpp/common/common.h"
#include "duckdb.hpp"
#include "duckdb/catalog/catalog.hpp"
#include "duckdb/main/extension.hpp"
#include "duckdb/main/extension_entries.hpp"
#include "duckdb/main/extension_helper.hpp"
#include "duckdb/main/extension_util.hpp"
#include "duckdb/optimizer/optimizer.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/optimizer/optimizer_extension.hpp"
#include "duckdb/common/sort/sort.hpp"


namespace duckdb {
void _llm(DataChunk &args, ExpressionState &state, Vector &result) {
    



    // Yunxin: First, let's try to concat the string based on their column orders
    result.SetVectorType(VectorType::CONSTANT_VECTOR);
	// iterate over the vectors to count how large the final string will be
	idx_t constant_lengths = 0;
	vector<idx_t> result_lengths(args.size(), 0);
	for (idx_t col_idx = 0; col_idx < args.ColumnCount(); col_idx++) {
		auto &input = args.data[col_idx];
		D_ASSERT(input.GetType().id() == LogicalTypeId::VARCHAR);
		if (input.GetVectorType() == VectorType::CONSTANT_VECTOR) {
			if (ConstantVector::IsNull(input)) {
				// constant null, skip
				continue;
			}
			auto input_data = ConstantVector::GetData<string_t>(input);
			constant_lengths += input_data->GetSize();
		} else {
			// non-constant vector: set the result type to a flat vector
			result.SetVectorType(VectorType::FLAT_VECTOR);
			// now get the lengths of each of the input elements
			UnifiedVectorFormat vdata;
			input.ToUnifiedFormat(args.size(), vdata);

			auto input_data = UnifiedVectorFormat::GetData<string_t>(vdata);
			// now add the length of each vector to the result length
			for (idx_t i = 0; i < args.size(); i++) {
				auto idx = vdata.sel->get_index(i);
				if (!vdata.validity.RowIsValid(idx)) {
					continue;
				}
				result_lengths[i] += input_data[idx].GetSize();
			}
		}
	}

	// first we allocate the empty strings for each of the values
	auto result_data = FlatVector::GetData<string_t>(result);
	for (idx_t i = 0; i < args.size(); i++) {
		// allocate an empty string of the required size
		idx_t str_length = constant_lengths + result_lengths[i];
		result_data[i] = StringVector::EmptyString(result, str_length);
		// we reuse the result_lengths vector to store the currently appended size
		result_lengths[i] = 0;
	}

	// now that the empty space for the strings has been allocated, perform the concatenation
	for (idx_t col_idx = 0; col_idx < args.ColumnCount(); col_idx++) {
		auto &input = args.data[col_idx];

		// loop over the vector and concat to all results
		if (input.GetVectorType() == VectorType::CONSTANT_VECTOR) {
			// constant vector
			if (ConstantVector::IsNull(input)) {
				// constant null, skip
				continue;
			}
			// append the constant vector to each of the strings
			auto input_data = ConstantVector::GetData<string_t>(input);
			auto input_ptr = input_data->GetData();
			auto input_len = input_data->GetSize();
			for (idx_t i = 0; i < args.size(); i++) {
				memcpy(result_data[i].GetDataWriteable() + result_lengths[i], input_ptr, input_len);
				result_lengths[i] += input_len;
			}
		} else {
			// standard vector
			UnifiedVectorFormat idata;
			input.ToUnifiedFormat(args.size(), idata);

			auto input_data = UnifiedVectorFormat::GetData<string_t>(idata);
			for (idx_t i = 0; i < args.size(); i++) {
				auto idx = idata.sel->get_index(i);
				if (!idata.validity.RowIsValid(idx)) {
					continue;
				}
				auto input_ptr = input_data[idx].GetData();
				auto input_len = input_data[idx].GetSize();
				memcpy(result_data[i].GetDataWriteable() + result_lengths[i], input_ptr, input_len);
				result_lengths[i] += input_len;
			}
		}
	}
	for (idx_t i = 0; i < args.size(); i++) {
		result_data[i].Finalize();
	}



    // LLM Part
    auto llm_input = std::string(result_data[0].GetData());
    gpt_params params;
    llama_backend_init();
    llama_numa_init(params.numa);
    params.prompt = "<|im_start|>system\n" + params.prompt + "<|im_end|>";
    params.prompt = params.prompt + llm_input;
    llama_model * model;
    llama_context * ctx;
    llama_context * ctx_guidance = NULL;
    llama_sampling_context *ctx_sampling = 0;
    std::vector<llama_token> embd_inp;
    embd_inp = ::llama_tokenize(ctx, params.prompt, true, true);
    const auto inp_pfx = ::llama_tokenize(ctx, "\n\n### Instruction:\n\n", true,  true);
    const auto inp_sfx = ::llama_tokenize(ctx, "\n\n### Response:\n\n",    false, true);
    // chatml prefix & suffix
    const auto cml_pfx = ::llama_tokenize(ctx, "\n<|im_start|>user\n", true, true);
    const auto cml_sfx = ::llama_tokenize(ctx, "<|im_end|>\n<|im_start|>assistant\n", false, true);
    std::vector<llama_token>  input_tokens, output_tokens; 
    std::string output;
    const llama_token id = llama_sampling_sample(ctx_sampling, ctx, ctx);
    int id;
    std::string output = "";
    for (auto id : embd_inp) {
        const std::string token_str = llama_token_to_piece(ctx, id);
        printf("%s", token_str.c_str());
        if (embd_inp.size() > 1) {
            input_tokens.push_back(id);
        } else {
            output_tokens.push_back(id);
            output += token_str;
        }
    }
    result_data[0] = duckdb::string_t(output);

}
static void LoadInternal(DatabaseInstance &instance) {
	auto llm_fn = ScalarFunction("llm", {LogicalType::ANY}, LogicalType::VARCHAR, _llm);
	ExtensionUtil::RegisterFunction(instance, llm_fn);
}
void LLMExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string LLMExtension::Name() {
    return "llm";
}
} // namespace duckdb
extern "C" {

DUCKDB_EXTENSION_API void llm_init(duckdb::DatabaseInstance &db) {
    duckdb::DuckDB db_wrapper(db);
    db_wrapper.LoadExtension<duckdb::LLMExtension>();
}

DUCKDB_EXTENSION_API const char *llm_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}
