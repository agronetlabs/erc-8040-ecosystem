# ERC-8040 Smart Contracts

Solidity smart contracts for the ERC-8040 ESG Compliance Token Standard.

## 📋 Overview

This directory contains the smart contracts, deployment scripts, and tests for the ERC-8040 ecosystem.

### Contracts

- **ERC8040.sol** - Main ESG compliance token with ESG score verification
- **ESGOracle.sol** - Oracle for managing and verifying ESG scores
- **ESGRegistry.sol** - Registry for authorized ESG data providers
- **ERC8040Factory.sol** - Factory for creating new ERC8040 tokens

### Mock Contracts (Testing)

- **MockUSDT.sol** - Mock USDT token with faucet
- **MockUSDC.sol** - Mock USDC token with faucet

## 🚀 Quick Start

### Prerequisites

Install [Foundry](https://book.getfoundry.sh/getting-started/installation):
```bash
curl -L https://foundry.paradigm.xyz | bash
foundryup
```

### Installation

1. Install dependencies:
```bash
make install
```

2. Build contracts:
```bash
make build
```

3. Run tests:
```bash
make test
```

## 📦 Deployment

See [DEPLOYMENT.md](./DEPLOYMENT.md) for comprehensive deployment instructions.

### Quick Deploy to Sepolia

1. Setup environment:
```bash
cp .env.example .env
# Edit .env with your keys and RPC URLs
```

2. Deploy:
```bash
make deploy-sepolia
```

## 🧪 Testing

### Run all tests
```bash
make test
```

### Run with gas report
```bash
make gas
```

### Run with coverage
```bash
make coverage
```

### Test on forked network
```bash
# Start local fork
anvil --fork-url $SEPOLIA_RPC_URL

# Run tests against fork
forge test --fork-url http://localhost:8545
```

## 📁 Directory Structure

```
contracts/
├── src/
│   ├── ERC8040.sol           # Main ESG token
│   ├── ESGOracle.sol         # ESG score oracle
│   ├── ESGRegistry.sol       # Provider registry
│   ├── ERC8040Factory.sol    # Token factory
│   └── mocks/
│       ├── MockUSDT.sol      # Mock USDT
│       └── MockUSDC.sol      # Mock USDC
├── script/
│   ├── DeployTestnet.s.sol   # Testnet deployment
│   ├── Verify.s.sol          # Contract verification
│   └── config/
│       ├── sepolia.json      # Sepolia config
│       └── base-sepolia.json # Base Sepolia config
├── test/
│   └── DeploymentTest.t.sol  # Deployment tests
├── foundry.toml              # Foundry configuration
├── Makefile                  # Build commands
├── DEPLOYMENT.md             # Deployment guide
└── README.md                 # This file
```

## 🛠️ Development

### Format code
```bash
make format
```

### Clean artifacts
```bash
make clean
```

### Update dependencies
```bash
make update
```

### Create gas snapshot
```bash
make snapshot
```

## 🔐 Security

- Never commit private keys
- Use hardware wallets for mainnet
- Get professional audits before production
- Test thoroughly on testnets first

## 📚 Documentation

- [Deployment Guide](./DEPLOYMENT.md) - Comprehensive deployment instructions
- [Foundry Book](https://book.getfoundry.sh/) - Foundry documentation
- [Main README](../README.md) - Project overview

## 🤝 Contributing

1. Write tests for new features
2. Follow existing code style
3. Run `make format` before committing
4. Ensure all tests pass

## 📄 License

MIT License - see [LICENSE](../LICENSE) for details
