#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

Key* find_active_key(Store& store, std::uint32_t key_id) {
  const auto match =
      std::find_if(store.active_keys.begin(), store.active_keys.end(),
                   [key_id](const Key* key) {
                     return key != nullptr && key->id == key_id;
                   });
  return match == store.active_keys.end() ? nullptr : *match;
}

KeyLease* find_lease(Store& store, std::uint32_t lease_id) {
  for (auto& lease : store.key_audit.leases) {
    if (lease.lease_id == lease_id) return &lease;
  }
  return nullptr;
}

std::uint32_t active_lease_count(const Store& store, std::uint32_t key_id) {
  std::uint32_t count = 0;
  for (const auto& lease : store.key_audit.leases) {
    if (lease.key_id == key_id && lease.active) ++count;
  }
  return count;
}

std::uint64_t summarize_key_audit(const KeyAuditState& state) {
  std::uint64_t digest = 0x243f6a8885a308d3ULL;
  for (const auto& lease : state.leases) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&lease.lease_id),
                        sizeof(lease.lease_id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&lease.key_id),
                        sizeof(lease.key_id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(lease.scope.data()),
                        lease.scope.size(), digest);
    digest ^= lease.issued_epoch;
    digest ^= lease.released_epoch << 1U;
    digest ^= lease.digest;
    digest ^= static_cast<std::uint64_t>(lease.active) << 7U;
  }
  for (const auto& entry : state.history) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(entry.data()),
                        entry.size(), digest);
  }
  if (state.selected_lease.has_value()) digest ^= *state.selected_lease;
  return digest;
}

std::uint64_t digest_key_material(const Key& key, std::uint64_t seed) {
  return hash_bytes(key.material.data(), key.material_size,
                    seed ^ static_cast<std::uint64_t>(key.id));
}

}  // namespace

Result lease_key(Store& store, std::uint32_t lease_id, std::uint32_t key_id,
                 std::string_view scope) {
  if (lease_id == 0 || scope.empty() || scope.size() > 24) {
    return Result::fail(Status::limit, "key lease invalid");
  }
  if (find_lease(store, lease_id) != nullptr) {
    return Result::fail(Status::invalid_state, "duplicate key lease");
  }
  Key* key = find_active_key(store, key_id);
  if (key == nullptr) {
    return Result::fail(Status::not_found, "leased key missing");
  }
  KeyLease lease;
  lease.lease_id = lease_id;
  lease.key_id = key_id;
  lease.scope.assign(scope);
  lease.issued_epoch = store.epoch;
  lease.active = true;
  lease.digest = digest_key_material(*key, lease_id ^ store.last_digest);
  store.key_audit.leases.push_back(lease);
  store.key_audit.selected_lease = lease_id;
  store.key_audit.history.push_back("lease:" + std::to_string(lease_id) + ":" +
                                    std::to_string(key_id) + ":" +
                                    std::string(scope));
  store.key_audit.digest = summarize_key_audit(store.key_audit);
  store.last_digest = store.key_audit.digest;
  store.events.push_back("key-lease");
  return Result::success();
}

Result release_key(Store& store, std::uint32_t lease_id) {
  KeyLease* lease = find_lease(store, lease_id);
  if (lease == nullptr) {
    return Result::fail(Status::not_found, "key lease missing");
  }
  if (!lease->active) {
    return Result::fail(Status::invalid_state, "key lease inactive");
  }
  lease->active = false;
  lease->released_epoch = store.epoch + store.events.size();
  lease->digest ^= (lease->released_epoch << 9U);
  store.key_audit.selected_lease = lease_id;
  store.key_audit.history.push_back("release:" + std::to_string(lease_id));
  store.key_audit.digest = summarize_key_audit(store.key_audit);
  store.last_digest = store.key_audit.digest;
  store.events.push_back("key-release");
  return Result::success();
}

Result audit_keyring(Store& store, std::string_view view) {
  if (view.empty() || view.size() > 16) {
    return Result::fail(Status::limit, "key audit view invalid");
  }
  store.inspect.lines.clear();
  if (view == "active") {
    for (const Key* key : store.active_keys) {
      if (key == nullptr) continue;
      store.inspect.lines.push_back(
          "active-key=" + std::to_string(key->id) + ":leases=" +
          std::to_string(active_lease_count(store, key->id)));
    }
  } else if (view == "leases") {
    for (const auto& lease : store.key_audit.leases) {
      store.inspect.lines.push_back(
          "lease=" + std::to_string(lease.lease_id) + ":key=" +
          std::to_string(lease.key_id) + ":scope=" + lease.scope + ":state=" +
          (lease.active ? "active" : "released"));
    }
  } else if (view == "history") {
    for (const auto& entry : store.key_audit.history) {
      store.inspect.lines.push_back("history=" + entry);
    }
  } else {
    return Result::fail(Status::unsupported, "unknown key audit view");
  }
  if (store.inspect.lines.empty()) {
    return Result::fail(Status::invalid_state, "key audit empty");
  }
  std::uint64_t digest = 0x13198a2e03707344ULL;
  for (const auto& line : store.inspect.lines) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(line.data()),
                        line.size(), digest);
  }
  store.key_audit.digest = summarize_key_audit(store.key_audit) ^ digest;
  store.last_digest = store.key_audit.digest;
  store.events.push_back("key-audit");
  return Result::success();
}

}  // namespace vesper
