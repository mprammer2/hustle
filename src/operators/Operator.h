#ifndef HUSTLE_OPERATOR_H
#define HUSTLE_OPERATOR_H

#include <string>
#include <table/block.h>
#include <table/table.h>
#include <arrow/compute/api.h>

namespace hustle {
namespace operators {

enum FilterOperator {
    AND,
    OR,
    NONE
};

//TODO(nicholas): Better name?
struct JoinResult {
    // TODO(nicholas): Combine into a ColumnReference?
    std::shared_ptr<Table> table;
    std::string join_col_name;
    // column AFTER a filter from a select predicate was applied.
    std::shared_ptr<arrow::ChunkedArray> join_col;
    arrow::compute::Datum filter; // filters are ChunkedArrays
    arrow::compute::Datum selection; // selections are Arrays
};

struct ColumnReference {
    std::shared_ptr<Table> table;
    std::string col_name;
};

class Operator {
public:

};
} // namespace operators
} // namespace hustle

#endif //HUSTLE_OPERATOR_H
