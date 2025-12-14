# ERC-8040 Testnet Deployment Guide

This guide covers deploying the ERC-8040 ecosystem to Sepolia and Base Sepolia testnets.

## Prerequisites

Before deploying, ensure you have:

- **Foundry** installed ([Installation Guide](https://book.getfoundry.sh/getting-started/installation))
- **Testnet ETH** for gas fees:
  - Sepolia: [Sepolia Faucet](https://sepoliafaucet.com/)
  - Base Sepolia: [Base Sepolia Faucet](https://www.coinbase.com/faucets/base-ethereum-sepolia-faucet)
- **RPC URL** from a provider like:
  - [Alchemy](https://www.alchemy.com/)
  - [Infura](https://www.infura.io/)
  - Public RPCs (slower, less reliable)
- **Block Explorer API Keys** (for contract verification):
  - Etherscan: [Get API Key](https://etherscan.io/myapikey)
  - Basescan: [Get API Key](https://basescan.org/myapikey)

## Quick Start

### 1. Setup Environment

Navigate to the contracts directory:
```bash
cd contracts
```

Copy the environment template:
```bash
cp .env.example .env
```

Edit `.env` and fill in your values:
```bash
# Your wallet private key (must have testnet ETH)
PRIVATE_KEY=0x...

# RPC endpoints
SEPOLIA_RPC_URL=https://eth-sepolia.g.alchemy.com/v2/YOUR_KEY
BASE_SEPOLIA_RPC_URL=https://sepolia.base.org

# API keys for verification
ETHERSCAN_API_KEY=your_key_here
BASESCAN_API_KEY=your_key_here
```

⚠️ **Security Warning**: Never commit your actual private key! The `.env` file is gitignored by default.

### 2. Install Dependencies

Install OpenZeppelin contracts:
```bash
make install
```

Or manually:
```bash
forge install OpenZeppelin/openzeppelin-contracts --no-commit
```

### 3. Build Contracts

Compile the smart contracts:
```bash
make build
```

### 4. Run Tests (Optional)

Verify everything works locally:
```bash
make test
```

### 5. Deploy to Testnet

#### Deploy to Sepolia:
```bash
make deploy-sepolia
```

#### Deploy to Base Sepolia:
```bash
make deploy-base-sepolia
```

The deployment script will:
1. Deploy ESGRegistry
2. Deploy ESGOracle (with Registry)
3. Deploy ERC8040 Token (AgroNet ESG Token - AGRO)
4. Deploy ERC8040Factory
5. Register the deployer as an ESG provider
6. Set sample ESG scores (75, 80, 85)
7. Automatically verify contracts on block explorer

### 6. Save Deployment Addresses

After deployment, save the contract addresses displayed in the console to your `.env` file:

```bash
SEPOLIA_REGISTRY=0x...
SEPOLIA_ORACLE=0x...
SEPOLIA_ERC8040=0x...
SEPOLIA_FACTORY=0x...
```

## Deployed Addresses

### Sepolia Testnet (Chain ID: 11155111)

| Contract | Address | Explorer |
|----------|---------|----------|
| ESGRegistry | `0x...` | [View](https://sepolia.etherscan.io/address/0x...) |
| ESGOracle | `0x...` | [View](https://sepolia.etherscan.io/address/0x...) |
| ERC8040 (AGRO) | `0x...` | [View](https://sepolia.etherscan.io/address/0x...) |
| ERC8040Factory | `0x...` | [View](https://sepolia.etherscan.io/address/0x...) |

### Base Sepolia Testnet (Chain ID: 84532)

| Contract | Address | Explorer |
|----------|---------|----------|
| ESGRegistry | `0x...` | [View](https://sepolia.basescan.org/address/0x...) |
| ESGOracle | `0x...` | [View](https://sepolia.basescan.org/address/0x...) |
| ERC8040 (AGRO) | `0x...` | [View](https://sepolia.basescan.org/address/0x...) |
| ERC8040Factory | `0x...` | [View](https://sepolia.basescan.org/address/0x...) |

## Manual Contract Verification

If automatic verification fails during deployment, you can verify manually:

### Verify on Sepolia:
```bash
# Set the contract address and path
make verify-sepolia CONTRACT_ADDRESS=0x... CONTRACT_PATH=src/ERC8040.sol:ERC8040
```

### Verify on Base Sepolia:
```bash
make verify-base-sepolia CONTRACT_ADDRESS=0x... CONTRACT_PATH=src/ERC8040.sol:ERC8040
```

### Individual Contract Verification with Constructor Args:

```bash
# ESGRegistry (no constructor args)
forge verify-contract $REGISTRY_ADDRESS src/ESGRegistry.sol:ESGRegistry \
  --chain sepolia \
  --etherscan-api-key $ETHERSCAN_API_KEY

# ESGOracle (requires registry address)
forge verify-contract $ORACLE_ADDRESS src/ESGOracle.sol:ESGOracle \
  --chain sepolia \
  --etherscan-api-key $ETHERSCAN_API_KEY \
  --constructor-args $(cast abi-encode "constructor(address)" $REGISTRY_ADDRESS)

# ERC8040 Token (requires name, symbol, oracle)
forge verify-contract $TOKEN_ADDRESS src/ERC8040.sol:ERC8040 \
  --chain sepolia \
  --etherscan-api-key $ETHERSCAN_API_KEY \
  --constructor-args $(cast abi-encode "constructor(string,string,address)" "AgroNet ESG Token" "AGRO" $ORACLE_ADDRESS)

# ERC8040Factory (requires oracle address)
forge verify-contract $FACTORY_ADDRESS src/ERC8040Factory.sol:ERC8040Factory \
  --chain sepolia \
  --etherscan-api-key $ETHERSCAN_API_KEY \
  --constructor-args $(cast abi-encode "constructor(address)" $ORACLE_ADDRESS)
```

### Using the Verify Script:
```bash
export REGISTRY_ADDRESS=0x...
export ORACLE_ADDRESS=0x...
export TOKEN_ADDRESS=0x...
export FACTORY_ADDRESS=0x...

forge script script/Verify.s.sol:Verify --rpc-url $SEPOLIA_RPC_URL -vvv
```

## Testing on Testnet

### Using Cast (Foundry CLI)

#### 1. Check ESG Score
```bash
cast call $SEPOLIA_ORACLE "getScore(address)(uint8,uint8,uint8,uint256)" $YOUR_ADDRESS --rpc-url $SEPOLIA_RPC_URL
```

#### 2. Update ESG Score (as provider)
```bash
cast send $SEPOLIA_ORACLE \
  "updateScore(address,uint8,uint8,uint8)" \
  $TARGET_ADDRESS 80 85 90 \
  --private-key $PRIVATE_KEY \
  --rpc-url $SEPOLIA_RPC_URL
```

#### 3. Mint ESG Tokens
```bash
cast send $SEPOLIA_ERC8040 \
  "mintWithESG(address,uint256,uint256,string)" \
  $RECIPIENT 1000000000000000000 1 "ipfs://QmTest..." \
  --private-key $PRIVATE_KEY \
  --rpc-url $SEPOLIA_RPC_URL
```

#### 4. Check Token Balance
```bash
cast call $SEPOLIA_ERC8040 "balanceOf(address)(uint256)" $YOUR_ADDRESS --rpc-url $SEPOLIA_RPC_URL
```

#### 5. Create New Token via Factory
```bash
cast send $SEPOLIA_FACTORY \
  "createToken(string,string)(address)" \
  "My ESG Token" "MESG" \
  --private-key $PRIVATE_KEY \
  --rpc-url $SEPOLIA_RPC_URL
```

### Using Mock Tokens

Deploy mock USDT/USDC for testing (if needed):
```bash
forge create src/mocks/MockUSDT.sol:MockUSDT \
  --rpc-url $SEPOLIA_RPC_URL \
  --private-key $PRIVATE_KEY \
  --verify
```

Mint from faucet:
```bash
cast send $MOCK_USDT \
  "faucet(address,uint256)" \
  $YOUR_ADDRESS 1000000000 \
  --rpc-url $SEPOLIA_RPC_URL \
  --private-key $PRIVATE_KEY
```

## Integration with Backend

### Environment Variables for Backend

After deployment, provide these addresses to your backend:

```bash
# Sepolia
SEPOLIA_CHAIN_ID=11155111
SEPOLIA_RPC_URL=https://eth-sepolia.g.alchemy.com/v2/...
SEPOLIA_REGISTRY_ADDRESS=0x...
SEPOLIA_ORACLE_ADDRESS=0x...
SEPOLIA_ERC8040_ADDRESS=0x...
SEPOLIA_FACTORY_ADDRESS=0x...

# Base Sepolia
BASE_SEPOLIA_CHAIN_ID=84532
BASE_SEPOLIA_RPC_URL=https://sepolia.base.org
BASE_SEPOLIA_REGISTRY_ADDRESS=0x...
BASE_SEPOLIA_ORACLE_ADDRESS=0x...
BASE_SEPOLIA_ERC8040_ADDRESS=0x...
BASE_SEPOLIA_FACTORY_ADDRESS=0x...
```

### Contract ABIs

ABIs are located in `out/` directory after building:
- `out/ERC8040.sol/ERC8040.json`
- `out/ESGOracle.sol/ESGOracle.json`
- `out/ESGRegistry.sol/ESGRegistry.json`
- `out/ERC8040Factory.sol/ERC8040Factory.json`

## Troubleshooting

### Deployment Fails with "Insufficient Funds"
- Ensure your wallet has enough testnet ETH for gas
- Get more from faucets (links in Prerequisites section)

### Contract Verification Fails
- Check your API key is correct
- Verify you're using the right network (sepolia vs base-sepolia)
- Try manual verification after a few minutes
- Check [status.etherscan.io](https://status.etherscan.io) for API issues

### "Provider not active in registry" Error
- Ensure the provider was registered: `registerProvider(address, name)`
- Ensure the provider is authorized: `setProvider(address, true)`

### Transaction Reverts with "No ESG score found"
- The address must have an ESG score before minting
- Call `updateScore()` on the oracle first

### Gas Price Too High
- Wait for lower network congestion
- Use a lower priority fee in your transaction

## Advanced Usage

### Custom Deployment Script

Create your own deployment script in `script/`:

```solidity
// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

import "forge-std/Script.sol";
import "../src/ERC8040.sol";

contract CustomDeploy is Script {
    function run() external {
        // Your custom deployment logic
    }
}
```

Run with:
```bash
forge script script/CustomDeploy.s.sol:CustomDeploy \
  --rpc-url $RPC_URL \
  --broadcast \
  --verify
```

### Gas Optimization

Generate gas report:
```bash
make gas
```

Optimize contract size:
```bash
# Edit foundry.toml
optimizer = true
optimizer_runs = 1000000  # Optimize for runtime
# or
optimizer_runs = 1        # Optimize for deployment size
```

### Testing with Forked Network

Test against a fork of the testnet:
```bash
# Start local fork
anvil --fork-url $SEPOLIA_RPC_URL

# Deploy to local fork (in another terminal)
forge script script/DeployTestnet.s.sol:DeployTestnet \
  --fork-url http://localhost:8545 \
  --broadcast
```

## Makefile Commands Reference

| Command | Description |
|---------|-------------|
| `make build` | Compile contracts |
| `make test` | Run tests |
| `make deploy-sepolia` | Deploy to Sepolia |
| `make deploy-base-sepolia` | Deploy to Base Sepolia |
| `make verify-sepolia` | Verify contract on Sepolia |
| `make verify-base-sepolia` | Verify contract on Base Sepolia |
| `make gas` | Generate gas report |
| `make coverage` | Generate coverage report |
| `make format` | Format Solidity code |
| `make clean` | Clean build artifacts |
| `make install` | Install dependencies |
| `make update` | Update dependencies |
| `make snapshot` | Snapshot gas usage |

## Security Considerations

1. **Private Keys**: Never commit private keys to git
2. **Testnet Only**: These scripts are for testnet deployment only
3. **Ownership**: Transfer ownership to a multisig for production
4. **Upgrades**: These contracts are not upgradeable by default
5. **Audits**: Get professional audits before mainnet deployment

## Support

For issues or questions:
- GitHub Issues: [erc-8040-ecosystem/issues](https://github.com/agronetlabs/erc-8040-ecosystem/issues)
- Documentation: [Project README](../README.md)

## License

MIT License - see [LICENSE](../LICENSE) for details
