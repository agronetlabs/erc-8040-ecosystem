# Migration Notice — erc-8040-ecosystem → ATF-AI

## Status: In Progress (Phase A/D)

## What is happening?

The `erc-8040-ecosystem` repository is being formally repositioned as an
**official ATF-AI adapter** for blockchain-based ESG compliance.

The canonical governance framework, specifications, and documentation are
now maintained in the parent repository:

👉 **[agronetlabs/ATF-AI](https://github.com/agronetlabs/ATF-AI)**

## What stays here?

This repository retains all implementation code:
- `core/` — Rust ESG scoring engine, compliance validator, ISO 20022 bridge, oracle
- `python-sdk/` — Python SDK (Pydantic-based)
- `cpp-sdk/` — C++17 high-performance SDK
- `contracts/` — Solidity smart contracts (ERC8040, ESGOracle, ESGRegistry, ERC8040Factory)
- `integration/` — SWIFT/ISO 20022 integration guides

## What moves to ATF-AI?

| Content | New Location in ATF-AI |
|---|---|
| Integration overview | `docs/integrations/blockchain/overview.md` |
| Architecture docs | `docs/integrations/blockchain/architecture.md` |
| SWIFT bridge docs | `docs/integrations/blockchain/swift-bridge.md` |
| Formal adapter spec | `specs/adapters/erc8040.md` |

## Migration Phases

| Phase | Description | Status |
|---|---|---|
| A | Documentation migrated to ATF-AI | ✅ Complete |
| B | Formal specs migrated to ATF-AI | ✅ Complete |
| C | Code migration decision (monorepo vs. separate repos) | 🔄 Pending |
| D | Archive + redirect (final state) | 🔄 Pending |

## Coordinated by

**Leandro** — AgroNet Labs
Framework: [ATF-AI](https://github.com/agronetlabs/ATF-AI)
