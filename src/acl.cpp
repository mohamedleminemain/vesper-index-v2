#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

RoleDefinition* find_role(Store& store, std::string_view role) {
  for (auto& item : store.acl.roles) {
    if (item.name == role) return &item;
  }
  return nullptr;
}

std::uint64_t digest_acl(const AclState& acl) {
  std::uint64_t digest = 0x13198a2e03707344ULL;
  for (const auto& role : acl.roles) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(role.name.data()),
                        role.name.size(), digest);
    for (const auto& permission : role.permissions) {
      digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(
                              permission.data()),
                          permission.size(), digest);
    }
  }
  for (const auto& check : acl.checks) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(check.data()),
                        check.size(), digest);
  }
  return digest;
}

bool valid_name(std::string_view text) {
  if (text.empty() || text.size() > 48) return false;
  return std::all_of(text.begin(), text.end(), [](char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') ||
           ch == '-' || ch == '_';
  });
}

}  // namespace

Result define_role(Store& store, std::string_view role) {
  if (!valid_name(role)) {
    return Result::fail(Status::limit, "role name invalid");
  }
  if (find_role(store, role) != nullptr) {
    return Result::fail(Status::invalid_state, "role already exists");
  }
  store.acl.roles.push_back({std::string(role), {}});
  store.acl.digest = digest_acl(store.acl);
  store.last_digest = store.acl.digest;
  store.events.push_back("acl-role");
  return Result::success();
}

Result grant_role(Store& store, std::string_view role,
                  std::string_view permission) {
  if (!valid_name(permission)) {
    return Result::fail(Status::limit, "permission invalid");
  }
  RoleDefinition* item = find_role(store, role);
  if (item == nullptr) {
    return Result::fail(Status::not_found, "role missing");
  }
  if (std::find(item->permissions.begin(), item->permissions.end(), permission) !=
      item->permissions.end()) {
    return Result::fail(Status::invalid_state, "permission already granted");
  }
  item->permissions.push_back(std::string(permission));
  std::sort(item->permissions.begin(), item->permissions.end());
  store.acl.digest = digest_acl(store.acl);
  store.last_digest = store.acl.digest;
  store.events.push_back("acl-grant");
  return Result::success();
}

Result check_permission(Store& store, std::string_view role,
                        std::string_view permission) {
  RoleDefinition* item = find_role(store, role);
  if (item == nullptr) {
    return Result::fail(Status::not_found, "role missing");
  }
  const bool allowed =
      std::find(item->permissions.begin(), item->permissions.end(), permission) !=
      item->permissions.end();
  store.acl.checks.push_back(std::string(role) + ":" + std::string(permission) +
                             ":" + (allowed ? "allow" : "deny"));
  if (store.acl.checks.size() > 64) {
    store.acl.checks.erase(store.acl.checks.begin(),
                           store.acl.checks.begin() + 16);
  }
  store.acl.digest = digest_acl(store.acl) ^ static_cast<std::uint64_t>(allowed);
  store.last_digest = store.acl.digest;
  store.events.push_back(allowed ? "acl-allow" : "acl-deny");
  return Result::success();
}

}  // namespace vesper
