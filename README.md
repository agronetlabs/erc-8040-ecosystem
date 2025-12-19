[![ISO 20022 Compatible](https://img.shields.io/badge/ISO%2020022-Compatible-00a651?style=for-the-badge&logo=swift&logoColor=white)](https://www.iso20022.org/)
[![SWIFT Ready](https://img.shields.io/badge/SWIFT-Ready-ff6600?style=for-the-badge&logo=swift&logoColor=white)](https://www.swift.com/)
[![ATF-AI Verified](https://img.shields.io/badge/ATF--AI-VERIFIED-2ea44f?style=for-the-badge&logo=vercel)](https://github.com/agronetlabs/atf-ai)
[![Provenance Traceable](https://img.shields.io/badge/PROVENANCE-SIGNED-0f9d58?style=for-the-badge&logo=oci)](https://github.com/agronetlabs/atf-ai)
[![Copilot](https://img.shields.io/badge/GitHub%20Copilot-Active-0066ff?style=for-the-badge&logo=githubcopilot)](https://github.com/features/copilot)
[![OpenAI Codex](https://img.shields.io/badge/OpenAI%20Codex-Active-ff6600?style=for-the-badge&logo=openai&logoColor=white)](https://github.com/features/copilot)

[![License: MIT OR Apache-2.0](https://img.shields.io/crates/l/esg-tokenization-protocol)](https://opensource.org/licenses)
![Build](https://img.shields.io/badge/build-passing-brightgreen)
![Status](https://img.shields.io/badge/project-Verified%20Blockchain%20Infra-orange)
![Deployed](https://img.shields.io/badge/deployed-Cloudflare-orange)
![Deployed](https://img.shields.io/badge/deployed-OpenAI-black)

# erc-8040-ecosystem

ERC-8040 Compliance Token Standard - ESG & DeFi

## 🏦 ISO 20022 SWIFT Integration

**ERC-8040 is the FIRST ESG token standard with native SWIFT ISO 20022 support**, enabling seamless integration between blockchain-based ESG compliance tokens and traditional financial infrastructure.

### Key Features

- ✅ **Native ISO 20022 support** for securities trading messages (SETR)
- ✅ **SFDR compliance mapping** (Articles 6, 8, 9) for EU regulatory requirements
- ✅ **EU Taxonomy alignment** calculation and reporting
- ✅ **Carbon intensity estimation** based on environmental scores
- ✅ **Multi-language SDK support**: Python, Rust, C++

### Architecture Overview

```
┌─────────────────┐      ┌──────────────────┐      ┌─────────────────┐
│  ERC-8040 Token │─────►│  ISO 20022       │─────►│  SWIFT Network  │
│  ESG Scores     │      │  Bridge          │      │  Financial Inst │
└─────────────────┘      └──────────────────┘      └─────────────────┘
     Blockchain              Transformation           Traditional Finance
```

The ISO 20022 bridge automatically:
1. **Converts ESG scores** to ISO 20022 classifications
2. **Maps ratings to SFDR articles** (AAA/AA/A → 9, BBB/BB → 8, others → 6)
3. **Calculates EU Taxonomy alignment** from environmental scores
4. **Generates compliant SETR messages** with embedded ESG metadata

### Quick Example

```python
from erc8040_sdk import ESGScore, ISO20022Bridge, FinancialInstrument

# Create ESG score
score = ESGScore.create(environmental=90.0, social=85.0, governance=80.0)

# Convert to ISO 20022
bridge = ISO20022Bridge()
classification = bridge.esg_to_iso(score)

# Generate SWIFT message
instrument = FinancialInstrument(
    isin="US46434G1031",
    lei="549300PZDW6EBUUJ8G35",
    name="ERC8040 Green Bond"
)
xml_message = bridge.create_setr_message(instrument, classification)
```

### Documentation

- 📖 [Integration Overview](./integration/README.md)
- 🏗️ [Architecture Details](./integration/architecture.md)
- 🌉 [SWIFT Bridge Guide](./integration/swift-bridge/README.md)
- 🗺️ [ISO 20022 Field Mapping](./integration/swift-bridge/iso20022-mapping.md)
- 📝 [Example SETR Message](./integration/swift-bridge/examples/setr010.xml)

## Overview

This repository provides a core Rust library and SDKs for ESG scoring, regulatory
compliance validation, and ISO 20022 integration.

## Structure

- `core/`: Rust core library (ESG, compliance, ISO 20022, oracle)
- `python-sdk/`: Python SDK built on Pydantic
- `cpp-sdk/`: C++17 SDK for high-performance environments

## Notes

- ESG weights are validated: non-negative and sum > 0.
