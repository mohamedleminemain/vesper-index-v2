#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {

Result define_schema(Store& store, std::uint32_t row_size,
                     std::vector<Field> fields) {
  if (row_size == 0 || row_size > (1U << 20U) || fields.empty() ||
      fields.size() > 128) {
    return Result::fail(Status::limit, "schema limits exceeded");
  }
  for (const Field& field : fields) {
    if (field.name.empty() || field.width == 0 ||
        field.offset > row_size) {
      return Result::fail(Status::malformed, "invalid field");
    }
    if (field.offset + field.width > row_size) {
      return Result::fail(Status::malformed, "invalid field");
    }
  }
  std::sort(fields.begin(), fields.end(),
            [](const Field& left, const Field& right) {
              return left.offset < right.offset;
            });
  store.schema.row_size = row_size;
  store.schema.fields = std::move(fields);
  store.schema.verified = true;
  store.events.push_back("schema-defined");
  return Result::success();
}

}  // namespace vesper
