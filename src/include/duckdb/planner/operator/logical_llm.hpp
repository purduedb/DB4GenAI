//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/operator/logical_llm.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/planner/bound_query_node.hpp"
#include "duckdb/planner/logical_operator.hpp"

namespace duckdb {

class LogicalLLM : public LogicalOperator {
public:
	static constexpr const LogicalOperatorType TYPE = LogicalOperatorType::LOGICAL_LLM;

public:
	LogicalLLM(vector<BoundOrderByNode> orders, int64_t limit, int64_t offset)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_TOP_N), orders(std::move(orders)), limit(limit), offset(offset) {
	}

	vector<BoundOrderByNode> orders;
	int64_t limit;
	int64_t offset;

public:
	vector<ColumnBinding> GetColumnBindings() override {
		return children[0]->GetColumnBindings();
	}

	void Serialize(Serializer &serializer) const override;
	static unique_ptr<LogicalOperator> Deserialize(Deserializer &deserializer);

	idx_t EstimateCardinality(ClientContext &context) override;

protected:
	void ResolveTypes() override {
		types = children[0]->types;
	}
};
} // namespace duckdb
