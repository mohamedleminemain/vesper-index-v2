#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

bool enabled(Profile profile, Profile family) {
  return profile == Profile::all || profile == family;
}

Result wrong_arity(const Command& command) {
  return Result::fail(Status::malformed,
                      command.verb + " has the wrong number of arguments");
}

Result number_error(const Command& command) {
  return Result::fail(Status::malformed,
                      command.verb + " contains an invalid integer");
}

Result hex_error(const Command& command) {
  return Result::fail(Status::malformed,
                      command.verb + " contains invalid hexadecimal data");
}

Result dispatch_archive(Store& store, const Command& command) {
  std::uint32_t a = 0;
  std::uint32_t b = 0;
  bool valid = false;
  if (command.verb == "DOC") {
    if (command.args.size() != 3) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    auto body = decode_hex(command.args[2], &valid);
    if (!valid) return hex_error(command);
    return add_document(store, a, command.args[1], std::move(body));
  }
  if (command.verb == "VIEW") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return create_title_view(store, a);
  }
  if (command.verb == "RENAME") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return rename_document(store, a, command.args[1]);
  }
  if (command.verb == "MANIFEST") {
    if (!command.args.empty()) return wrong_arity(command);
    return emit_manifest(store);
  }
  if (command.verb == "KEY") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    auto material = decode_hex(command.args[1], &valid);
    if (!valid) return hex_error(command);
    return add_key(store, a, std::move(material));
  }
  if (command.verb == "ROTATE") {
    if (command.args.size() != 3) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) ||
        !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    auto material = decode_hex(command.args[2], &valid);
    if (!valid) return hex_error(command);
    return rotate_key(store, a, b, std::move(material));
  }
  if (command.verb == "KEYLEASE") {
    if (command.args.size() != 3) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) || !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return lease_key(store, a, b, command.args[2]);
  }
  if (command.verb == "KEYRELEASE") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return release_key(store, a);
  }
  if (command.verb == "KEYAUDIT") {
    if (command.args.size() != 1) return wrong_arity(command);
    return audit_keyring(store, command.args[0]);
  }
  if (command.verb == "PAGE") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    auto bytes = decode_hex(command.args[1], &valid);
    if (!valid) return hex_error(command);
    return add_page(store, a, std::move(bytes));
  }
  if (command.verb == "CURSOR") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) ||
        !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return create_cursor(store, a, b);
  }
  if (command.verb == "COMPACT") {
    if (!command.args.empty()) return wrong_arity(command);
    return compact_pages(store);
  }
  if (command.verb == "SEGMENT") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) || !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return begin_segment(store, a, b);
  }
  if (command.verb == "ENTRY") {
    if (command.args.size() != 2) return wrong_arity(command);
    auto payload = decode_hex(command.args[1], &valid);
    if (!valid) return hex_error(command);
    return append_segment(store, command.args[0], std::move(payload));
  }
  if (command.verb == "SEALSEG") {
    if (!command.args.empty()) return wrong_arity(command);
    return seal_segment(store);
  }
  if (command.verb == "REPLAY") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return replay_segment(store, a);
  }
  if (command.verb == "SNAPSHOT") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return capture_snapshot(store, a);
  }
  if (command.verb == "RESTORE") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return restore_snapshot(store, a);
  }
  if (command.verb == "SNAPINFO") {
    if (!command.args.empty()) return wrong_arity(command);
    return inspect_snapshots(store);
  }
  if (command.verb == "CACHE") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) || !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return cache_page_window(store, a, b);
  }
  if (command.verb == "TOUCH") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return touch_cache_window(store, a);
  }
  if (command.verb == "EVICT") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return evict_cache_windows(store, a);
  }
  return Result::fail(Status::unsupported, "not an archive command");
}

Result dispatch_query(Store& store, const Command& command) {
  std::uint32_t a = 0;
  std::uint64_t wide = 0;
  std::uint32_t b = 0;
  bool valid = false;
  if (command.verb == "SCAN") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return scan_cursor(store, a);
  }
  if (command.verb == "NODE") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) ||
        !parse_u64(command.args[1], wide)) {
      return number_error(command);
    }
    return add_node(store, a, wide);
  }
  if (command.verb == "WATCH") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return watch_node(store, a);
  }
  if (command.verb == "DROP") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return drop_node(store, a);
  }
  if (command.verb == "AUDIT") {
    if (!command.args.empty()) return wrong_arity(command);
    return audit_watches(store);
  }
  if (command.verb == "WATCHSNAP") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return snapshot_watches(store, a);
  }
  if (command.verb == "WATCHREVIEW") {
    if (command.args.size() != 1) return wrong_arity(command);
    return review_watch_state(store, command.args[0]);
  }
  if (command.verb == "SCHEMA") {
    if (command.args.size() < 4 || command.args.size() % 3 != 1) {
      return wrong_arity(command);
    }
    if (!parse_u32(command.args[0], a)) return number_error(command);
    std::vector<Field> fields;
    for (std::size_t i = 1; i < command.args.size(); i += 3) {
      std::uint32_t offset = 0;
      std::uint32_t width = 0;
      if (!parse_u32(command.args[i + 1], offset) ||
          !parse_u32(command.args[i + 2], width)) {
        return number_error(command);
      }
      fields.push_back({command.args[i], offset, width});
    }
    return define_schema(store, a, std::move(fields));
  }
  if (command.verb == "ROW") {
    if (command.args.size() != 1) return wrong_arity(command);
    auto bytes = decode_hex(command.args[0], &valid);
    if (!valid) return hex_error(command);
    return install_row(store, std::move(bytes));
  }
  if (command.verb == "PROJECT") {
    if (!command.args.empty()) return wrong_arity(command);
    return project_row(store);
  }
  if (command.verb == "LINKS") {
    if (command.args.empty()) return wrong_arity(command);
    std::vector<std::uint32_t> links;
    for (const std::string& text : command.args) {
      if (!parse_u32(text, a)) return number_error(command);
      links.push_back(a);
    }
    return define_links(store, std::move(links));
  }
  if (command.verb == "FOLLOW") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return select_link(store, a);
  }
  if (command.verb == "EXPORT") {
    if (!command.args.empty()) return wrong_arity(command);
    return export_selected_link(store);
  }
  if (command.verb == "BLOOM") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) || !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return build_bloom(store, a, b);
  }
  if (command.verb == "PROBE") {
    if (command.args.size() != 1) return wrong_arity(command);
    return probe_bloom(store, command.args[0]);
  }
  if (command.verb == "INDEX") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) || !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return build_skip_index(store, a, b);
  }
  if (command.verb == "SEEK") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return seek_skip_index(store, a);
  }
  if (command.verb == "PLAN") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[1], a)) return number_error(command);
    return build_query_plan(store, command.args[0], a);
  }
  if (command.verb == "EVAL") {
    if (!command.args.empty()) return wrong_arity(command);
    return evaluate_query_plan(store);
  }
  if (command.verb == "METRICS") {
    if (command.args.size() != 1) return wrong_arity(command);
    return collect_metrics(store, command.args[0]);
  }
  if (command.verb == "REDUCE") {
    if (!command.args.empty()) return wrong_arity(command);
    return reduce_metrics(store);
  }
  if (command.verb == "INSPECT") {
    if (command.args.size() != 1) return wrong_arity(command);
    return inspect_store(store, command.args[0]);
  }
  if (command.verb == "DIGEST") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[1], a)) return number_error(command);
    return capture_digest(store, command.args[0], a);
  }
  if (command.verb == "VERIFY") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u64(command.args[0], wide)) return number_error(command);
    return verify_digest(store, wide);
  }
  if (command.verb == "MERGE") {
    if (command.args.size() != 1) return wrong_arity(command);
    return prepare_merge(store, command.args[0]);
  }
  if (command.verb == "NEXT") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return next_merge(store, a);
  }
  if (command.verb == "SAMPLE") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[1], a)) return number_error(command);
    return sample_scope(store, command.args[0], a);
  }
  if (command.verb == "FOLD") {
    if (!command.args.empty()) return wrong_arity(command);
    return fold_sample(store);
  }
  if (command.verb == "DIFFSNAP") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) || !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return diff_snapshots(store, a, b);
  }
  if (command.verb == "LEDGER") {
    if (command.args.size() != 1) return wrong_arity(command);
    return build_ledger(store, command.args[0]);
  }
  if (command.verb == "FOLDLEDGER") {
    if (!command.args.empty()) return wrong_arity(command);
    return fold_ledger(store);
  }
  if (command.verb == "QUOTA") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return prepare_quota(store, a);
  }
  if (command.verb == "ENFORCE") {
    if (command.args.size() != 1) return wrong_arity(command);
    return enforce_quota(store, command.args[0]);
  }
  if (command.verb == "RELATE") {
    if (command.args.size() != 1) return wrong_arity(command);
    return build_relations(store, command.args[0]);
  }
  if (command.verb == "WALKREL") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) || !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return walk_relations(store, a, b);
  }
  if (command.verb == "CHECKPOINT") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return capture_checkpoint(store, a);
  }
  if (command.verb == "CHECKVIEW") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return inspect_checkpoint(store, a);
  }
  if (command.verb == "POLICY") {
    if (command.args.size() != 3) return wrong_arity(command);
    if (!parse_u32(command.args[2], a)) return number_error(command);
    return define_policy(store, command.args[0], command.args[1], a);
  }
  if (command.verb == "POLICYCHECK") {
    if (command.args.size() != 2) return wrong_arity(command);
    return evaluate_policy(store, command.args[0], command.args[1]);
  }
  if (command.verb == "SHARD") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return assign_shards(store, a);
  }
  if (command.verb == "REBALANCE") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return rebalance_shards(store, a);
  }
  if (command.verb == "SHARDINFO") {
    if (!command.args.empty()) return wrong_arity(command);
    return inspect_shards(store);
  }
  return Result::fail(Status::unsupported, "not a query command");
}

Result dispatch_codec(Store& store, const Command& command) {
  std::uint32_t a = 0;
  std::uint32_t b = 0;
  bool valid = false;
  if (command.verb == "VECTOR") {
    if (command.args.size() != 3) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) ||
        !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    auto source = decode_hex(command.args[2], &valid);
    if (!valid) return hex_error(command);
    return configure_vector(store, a, b, std::move(source));
  }
  if (command.verb == "DECODE") {
    if (!command.args.empty()) return wrong_arity(command);
    return decode_vector(store);
  }
  if (command.verb == "LABEL") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return configure_label(store, a, command.args[1]);
  }
  if (command.verb == "RENDER") {
    if (!command.args.empty()) return wrong_arity(command);
    return render_label(store);
  }
  if (command.verb == "PALETTE") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return choose_palette(store, a);
  }
  if (command.verb == "COLOR") {
    if (!command.args.empty()) return wrong_arity(command);
    return emit_palette_color(store);
  }
  if (command.verb == "NORMALIZE") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return normalize_document(store, a);
  }
  if (command.verb == "TOKENS") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return emit_normalized_tokens(store, a);
  }
  if (command.verb == "PACK") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return pack_blocks(store, a);
  }
  if (command.verb == "UNPACK") {
    if (!command.args.empty()) return wrong_arity(command);
    return unpack_blocks(store);
  }
  return Result::fail(Status::unsupported, "not a codec command");
}

Result dispatch_session(Store& store, const Command& command) {
  std::uint32_t a = 0;
  std::uint32_t b = 0;
  std::uint64_t wide = 0;
  if (command.verb == "SIGPLAN") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) ||
        !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return configure_signatures(store, a, b);
  }
  if (command.verb == "FINALIZE") {
    if (!command.args.empty()) return wrong_arity(command);
    return finalize_session(store);
  }
  if (command.verb == "SIGN") {
    if (!command.args.empty()) return wrong_arity(command);
    return write_signatures(store);
  }
  if (command.verb == "ROLE") {
    if (command.args.size() != 1) return wrong_arity(command);
    return define_role(store, command.args[0]);
  }
  if (command.verb == "GRANT") {
    if (command.args.size() != 2) return wrong_arity(command);
    return grant_role(store, command.args[0], command.args[1]);
  }
  if (command.verb == "CHECK") {
    if (command.args.size() != 2) return wrong_arity(command);
    return check_permission(store, command.args[0], command.args[1]);
  }
  if (command.verb == "TIME") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) || !parse_u64(command.args[1], wide)) {
      return number_error(command);
    }
    return record_timeline(store, a, wide);
  }
  if (command.verb == "WINDOW") {
    if (command.args.size() != 2) return wrong_arity(command);
    std::uint64_t start = 0;
    std::uint64_t end = 0;
    if (!parse_u64(command.args[0], start) || !parse_u64(command.args[1], end)) {
      return number_error(command);
    }
    return window_timeline(store, start, end);
  }
  if (command.verb == "RETAIN") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return retain_timeline(store, a);
  }
  if (command.verb == "ISSUE") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[1], a)) return number_error(command);
    return issue_ticket(store, command.args[0], a);
  }
  if (command.verb == "RENEWTICKET") {
    if (command.args.size() != 2) return wrong_arity(command);
    if (!parse_u32(command.args[0], a) || !parse_u32(command.args[1], b)) {
      return number_error(command);
    }
    return renew_ticket(store, a, b);
  }
  if (command.verb == "CHECKTICKET") {
    if (command.args.size() != 1) return wrong_arity(command);
    if (!parse_u32(command.args[0], a)) return number_error(command);
    return check_ticket(store, a);
  }
  return Result::fail(Status::unsupported, "not a session command");
}

}  // namespace

Result execute_commands(Store& store, const std::vector<Command>& commands,
                        Profile profile) {
  for (const Command& command : commands) {
    Result result = Result::fail(Status::unsupported, "unknown command");
    if (enabled(profile, Profile::archive)) {
      result = dispatch_archive(store, command);
    }
    if (result.status == Status::unsupported &&
        enabled(profile, Profile::query)) {
      result = dispatch_query(store, command);
    }
    if (result.status == Status::unsupported &&
        enabled(profile, Profile::codec)) {
      result = dispatch_codec(store, command);
    }
    if (result.status == Status::unsupported &&
        enabled(profile, Profile::session)) {
      result = dispatch_session(store, command);
    }
    if (!result) {
      if (result.status == Status::unsupported) continue;
      result.message += " at line " + std::to_string(command.line);
      return result;
    }
  }
  return Result::success();
}

Result execute_script(const std::uint8_t* data, std::size_t size,
                      Profile profile, std::uint64_t* digest) {
  std::vector<Command> commands;
  Result result = parse_script(data, size, commands);
  if (!result) return result;
  Store store;
  result = execute_commands(store, commands, profile);
  if (digest) *digest = store.last_digest;
  return result;
}

}  // namespace vesper
