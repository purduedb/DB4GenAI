#include "duckdb/optimizer/llm_optimizer.hpp"

#include "duckdb/common/limits.hpp"
#include "duckdb/planner/operator/logical_limit.hpp"
#include "duckdb/planner/operator/logical_order.hpp"
#include "duckdb/planner/operator/logical_top_n.hpp"
#include "duckdb/planner/operator/logical_distinct.hpp"
#include "duckdb/planner/operator/logical_llm.hpp"

namespace duckdb {

bool LLM::CanOptimize(LogicalOperator &op) {
	// if (op.type == LogicalOperatorType::LOGICAL_LIMIT &&
	//     op.children[0]->type == LogicalOperatorType::LOGICAL_ORDER_BY) {
	// 	auto &limit = op.Cast<LogicalLimit>();

	// 	if (limit.limit_val.Type() != LimitNodeType::CONSTANT_VALUE) {
	// 		// we need LIMIT to be present AND be a constant value for us to be able to use Top-N
	// 		return false;
	// 	}
	// 	if (limit.offset_val.Type() == LimitNodeType::EXPRESSION_VALUE) {
	// 		// we need offset to be either not set (i.e. limit without offset) OR have offset be
	// 		return false;
	// 	}
	// 	return true;
	// }
	// return false;
	if (op.children.size() > 0 && (*op.children[0]).type == LogicalOperatorType::LOGICAL_LLM) {
		return true;
	}
    return false;
}

unique_ptr<LogicalOperator> LLM::Optimize(unique_ptr<LogicalOperator> op) {
	if (CanOptimize(*op)) {
		auto order = make_uniq<LogicalOrder>();
		auto distinct = make_uniq<LogicalDistinct>(DistinctType::DISTINCT_ON);
		auto llm = (op->children[0])->Cast<LogicalLLM>();
		distinct->AddChild(std::move(llm));
		order->AddChild(std::move(distinct));
		op->AddChild(std::move(order));
		op = std::move(op);
	} else {
		for (auto &child: op->children) {
			child = Optimize(std::move(child));
		}
	}
	return op;
}

} // namespace duckdb
