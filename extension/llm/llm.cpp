#include "llm.hpp"

#include "duckdb.hpp"
#include "duckdb/main/connection.hpp"
#include "third_party/llama.cpp/common/common.h"

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace duckdb {
static void _llm(DataChunk &args, ExpressionState &state, Vector &result) {

	// total length of the sequence including the prompt
	const int n_len = 32;

	// init LLM
	gpt_params params;
	llama_backend_init();
	llama_numa_init(params.numa);

	// initialize the model

	llama_model_params model_params = llama_model_default_params();

	llama_model *model = llama_load_model_from_file("/home/sun1114/model.gguf", model_params);

	std::vector<pair<std::string, idx_t>> inputs;
	for (idx_t i = 0; i < args.size(); ++i) {
		inputs.push_back(std::make_pair(args.GetValue(0, i).ToString(), i));
	}
	sort(inputs.begin(), inputs.end());
	std::string prev_input("");
	std::string output("");
	for (const auto &input : inputs) {
		std::string prompt = input.first;
		if (prompt.compare(prev_input) == 0) {
			result.SetValue(input.second, Value(output));
			return;
		} else {
			prev_input = std::string(prompt);
			// initialize the context

			llama_context_params ctx_params = llama_context_default_params();

			ctx_params.seed = 1234;
			ctx_params.n_ctx = 2048;
			ctx_params.n_threads = params.n_threads;
			ctx_params.n_threads_batch = params.n_threads_batch == -1 ? params.n_threads : params.n_threads_batch;

			llama_context *ctx = llama_new_context_with_model(model, ctx_params);
			params.prompt = prompt.c_str();
			// tokenize the prompt

			std::vector<llama_token> tokens_list;
			tokens_list = ::llama_tokenize(ctx, params.prompt, true);

			const int n_ctx = llama_n_ctx(ctx);
			const int n_kv_req = tokens_list.size() + (n_len - tokens_list.size());

			// create a llama_batch with size 512
			// we use this object to submit token data for decoding

			llama_batch batch = llama_batch_init(512, 0, 1);

			// evaluate the initial prompt
			for (size_t i = 0; i < tokens_list.size(); i++) {
				llama_batch_add(batch, tokens_list[i], i, {0}, false);
			}

			// llama_decode will output logits only for the last token of the prompt
			batch.logits[batch.n_tokens - 1] = true;
			llama_decode(ctx, batch);

			// main loop

			int n_cur = batch.n_tokens;
			int n_decode = 0;

			while (n_cur <= n_len) {
				// sample the next token
				{
					auto n_vocab = llama_n_vocab(model);
					auto *logits = llama_get_logits_ith(ctx, batch.n_tokens - 1);

					std::vector<llama_token_data> candidates;
					candidates.reserve(n_vocab);

					for (llama_token token_id = 0; token_id < n_vocab; token_id++) {
						candidates.emplace_back(llama_token_data {token_id, logits[token_id], 0.0f});
					}

					llama_token_data_array candidates_p = {candidates.data(), candidates.size(), false};

					// sample the most likely token
					const llama_token new_token_id = llama_sample_token_greedy(ctx, &candidates_p);

					// is it an end of stream?
					if (new_token_id == llama_token_eos(model) || n_cur == n_len) {
						output = output + "\n";
						result.SetValue(input.second, Value(output));
						break;
					}
					// prepare the next batch
					llama_batch_clear(batch);

					// push this new token for next evaluation
					llama_batch_add(batch, new_token_id, n_cur, {0}, true);
					output = output + llama_token_to_piece(ctx, new_token_id).c_str();
					n_decode += 1;
				}

				n_cur += 1;

				// evaluate the current batch with the transformer model
				llama_decode(ctx, batch);
			}
			llama_batch_free(batch);
			llama_free(ctx);
		}
	}

	llama_free_model(model);

	llama_backend_free();
}

static void LoadInternal(DatabaseInstance &instance) {
	scalar_function_t llm_fn = _llm;
	Connection con(instance);
	con.CreateVectorizedFunction<string_t, string_t>("llm", llm_fn);
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
