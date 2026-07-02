#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

bool valid_subject(std::string_view subject) {
  return !subject.empty() && subject.size() <= 48 &&
         std::all_of(subject.begin(), subject.end(), [](char ch) {
           return (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') ||
                  ch == '-' || ch == '_';
         });
}

std::uint64_t summarize_tickets(const TicketState& tickets) {
  std::uint64_t digest = 0x510e527fade682d1ULL ^ tickets.epoch;
  for (const auto& ticket : tickets.tickets) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(ticket.subject.data()),
                        ticket.subject.size(), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&ticket.issued),
                        sizeof(ticket.issued), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&ticket.expires),
                        sizeof(ticket.expires), digest);
    digest ^= ticket.token;
    digest ^= static_cast<std::uint64_t>(ticket.revoked) << 9U;
  }
  return digest;
}

}  // namespace

Result issue_ticket(Store& store, std::string_view subject, std::uint32_t ttl) {
  if (!valid_subject(subject) || ttl == 0 || ttl > 4096) {
    return Result::fail(Status::limit, "ticket parameters invalid");
  }
  Ticket ticket;
  ticket.subject = std::string(subject);
  ticket.issued = ++store.tickets.epoch;
  ticket.expires = ticket.issued + ttl;
  ticket.token = hash_bytes(reinterpret_cast<const std::uint8_t*>(subject.data()),
                            subject.size(), ticket.expires ^ store.last_digest);
  store.tickets.tickets.push_back(std::move(ticket));
  store.tickets.digest = summarize_tickets(store.tickets);
  store.last_digest = store.tickets.digest;
  store.events.push_back("ticket-issue");
  return Result::success();
}

Result renew_ticket(Store& store, std::uint32_t index, std::uint32_t ttl) {
  if (index >= store.tickets.tickets.size()) {
    return Result::fail(Status::limit, "ticket index invalid");
  }
  if (ttl == 0 || ttl > 4096) {
    return Result::fail(Status::limit, "ticket ttl invalid");
  }
  Ticket& ticket = store.tickets.tickets[index];
  if (ticket.revoked) {
    return Result::fail(Status::invalid_state, "ticket revoked");
  }
  ticket.issued = ++store.tickets.epoch;
  ticket.expires = ticket.issued + ttl;
  ticket.token = hash_bytes(reinterpret_cast<const std::uint8_t*>(ticket.subject.data()),
                            ticket.subject.size(), ticket.expires ^ ticket.token);
  store.tickets.digest = summarize_tickets(store.tickets);
  store.last_digest = store.tickets.digest;
  store.events.push_back("ticket-renew");
  return Result::success();
}

Result check_ticket(Store& store, std::uint32_t index) {
  if (index >= store.tickets.tickets.size()) {
    return Result::fail(Status::limit, "ticket index invalid");
  }
  Ticket& ticket = store.tickets.tickets[index];
  if (ticket.revoked) {
    return Result::fail(Status::invalid_state, "ticket revoked");
  }
  const bool alive = ticket.expires >= store.tickets.epoch;
  if (!alive) {
    ticket.revoked = true;
  }
  store.tickets.digest = summarize_tickets(store.tickets) ^
                         static_cast<std::uint64_t>(alive);
  store.last_digest = store.tickets.digest;
  store.events.push_back(alive ? "ticket-alive" : "ticket-expired");
  return Result::success();
}

}  // namespace vesper
