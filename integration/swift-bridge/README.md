# SWIFT ISO 20022 Bridge Documentation

## Overview

The SWIFT ISO 20022 Bridge enables ERC-8040 ESG compliance tokens to be represented and traded using standard financial messaging protocols. This bridge provides seamless interoperability between blockchain-based ESG assets and traditional financial infrastructure.

## Supported Message Types

### Securities Trade (SETR) Messages

The bridge currently supports the following SETR message types:

#### setr.010.001.04 - Securities Trade Confirmation
- **Purpose**: Confirm execution of a securities trade
- **Use Case**: Confirm purchase/sale of ESG tokens with embedded ESG classification
- **ESG Extension**: Full ESG metadata in custom classification block

#### setr.012.001.05 - Subscription Order Confirmation
- **Purpose**: Confirm subscription to investment fund
- **Use Case**: Subscribe to ESG token fund with SFDR article classification
- **ESG Extension**: SFDR and taxonomy alignment data

#### setr.013.001.04 - Subscription Order
- **Purpose**: Instruct subscription to securities
- **Use Case**: Order ESG-compliant tokens with specific sustainability criteria
- **ESG Extension**: Minimum ESG rating requirements

## SFDR Article Mapping

The bridge automatically maps ERC-8040 ratings to SFDR (Sustainable Finance Disclosure Regulation) article classifications:

### Mapping Table

| ERC-8040 Rating | Score Range | SFDR Article | Classification | Description |
|----------------|-------------|--------------|----------------|-------------|
| AAA            | 90-100      | Article 9    | Dark Green     | Sustainable investment with measurable positive impact |
| AA             | 85-89       | Article 9    | Dark Green     | Sustainable investment objective |
| A              | 80-84       | Article 9    | Dark Green     | Strong sustainability objective |
| BBB            | 70-79       | Article 8    | Light Green    | Promotes environmental/social characteristics |
| BB             | 60-69       | Article 8    | Light Green    | Promotes some ESG characteristics |
| B              | 50-59       | Article 6    | Neutral        | No specific sustainability objective |
| CCC            | 40-49       | Article 6    | Neutral        | Below sustainability threshold |
| CC             | 30-39       | Article 6    | Neutral        | Low ESG performance |
| C              | 20-29       | Article 6    | Neutral        | Very low ESG performance |
| D              | 0-19        | Article 6    | Neutral        | Minimal ESG consideration |

### Article Definitions

**Article 9 - Sustainable Investment Products**
- Have a sustainable investment objective
- Measure and report environmental/social impact
- Do no significant harm (DNSH principle)
- Good governance practices

**Article 8 - ESG Promoting Products**  
- Promote environmental or social characteristics
- Investments follow good governance
- May not have explicit sustainability objective
- Transparency on how characteristics are met

**Article 6 - Other Products**
- Do not promote environmental/social characteristics
- No sustainable investment objective
- Standard disclosure requirements
- May still have some ESG integration

## EU Taxonomy Alignment

The bridge calculates EU Taxonomy alignment percentage based on the environmental component of the ESG score.

### Calculation Formula

```python
def calculate_taxonomy_alignment(environmental_score: float) -> float:
    """
    Calculate EU Taxonomy alignment percentage.
    
    Args:
        environmental_score: Environmental score (0-100)
    
    Returns:
        Taxonomy alignment percentage (0-100)
    """
    if environmental_score >= 80:
        # High environmental performance = direct alignment
        return min(environmental_score, 100.0)
    elif environmental_score >= 60:
        # Medium performance = scaled alignment
        return (environmental_score - 60) * 2.0
    else:
        # Low performance = no alignment
        return 0.0
```

### Alignment Thresholds

| Environmental Score | Taxonomy Alignment | Interpretation |
|--------------------|-------------------|----------------|
| 90-100             | 90-100%           | Fully aligned with all 6 objectives |
| 80-89              | 80-89%            | Substantially aligned |
| 70-79              | 20-38%            | Partially aligned |
| 60-69              | 0-18%             | Minimal alignment |
| 0-59               | 0%                | Not aligned |

### EU Taxonomy Objectives

The alignment percentage represents contribution to these 6 environmental objectives:
1. Climate change mitigation
2. Climate change adaptation
3. Sustainable use of water and marine resources
4. Transition to a circular economy
5. Pollution prevention and control
6. Protection and restoration of biodiversity

## ESG Classification Structure

### Full Classification Object

```json
{
  "taxonomy_alignment": 85.0,
  "sfdr_article": 9,
  "erc8040_rating": "AA",
  "carbon_intensity": 75.0
}
```

### Field Descriptions

- **taxonomy_alignment**: EU Taxonomy alignment percentage (0-100)
- **sfdr_article**: SFDR Article classification (6, 8, or 9)
- **erc8040_rating**: Letter rating from AAA (highest) to D (lowest)
- **carbon_intensity**: Estimated carbon intensity in tCO2e per $M revenue

## Carbon Intensity Estimation

Carbon intensity is inversely related to environmental score:

```python
def estimate_carbon_intensity(environmental_score: float) -> float:
    """
    Estimate carbon intensity based on environmental performance.
    
    Args:
        environmental_score: Environmental score (0-100)
    
    Returns:
        Carbon intensity in tCO2e/$M revenue
    """
    max_intensity = 500.0
    return max_intensity * (1.0 - environmental_score / 100.0)
```

### Carbon Intensity Benchmarks

| Environmental Score | Carbon Intensity | Industry Comparison |
|--------------------|------------------|---------------------|
| 90-100             | 0-50 tCO2e/$M    | Renewable energy, tech services |
| 80-89              | 50-100 tCO2e/$M  | Clean tech, sustainable manufacturing |
| 70-79              | 100-150 tCO2e/$M | Modern manufacturing, services |
| 60-69              | 150-200 tCO2e/$M | Traditional manufacturing |
| 0-59               | 200-500 tCO2e/$M | Heavy industry, fossil fuels |

## Message Examples

See [examples/setr010.xml](./examples/setr010.xml) for a complete ISO 20022 setr.010.001.04 message with ESG classification.

## Integration Guide

### Python SDK

```python
from erc8040_sdk import ESGScore, ISO20022Bridge, FinancialInstrument

# Create ESG score
score = ESGScore.create(
    environmental=90.0,
    social=85.0,
    governance=80.0
)

# Initialize bridge
bridge = ISO20022Bridge()

# Map to SFDR article
sfdr = bridge.map_sfdr_article(score.rating)
print(f"SFDR Article: {sfdr}")  # Output: 9

# Calculate taxonomy alignment
alignment = bridge.calculate_taxonomy_alignment(score)
print(f"Taxonomy Alignment: {alignment}%")  # Output: 90.0%

# Generate complete classification
classification = bridge.esg_to_iso(score)

# Create ISO 20022 message
instrument = FinancialInstrument(
    isin="US1234567890",
    lei="123456789012345678XX",
    name="ERC8040 Green Bond"
)

xml_message = bridge.create_setr_message(instrument, classification)
```

### Rust SDK

```rust
use erc8040::esg::ESGScore;
use erc8040::iso20022::{ISO20022Bridge, FinancialInstrument};

// Create ESG score
let score = ESGScore::new(90.0, 85.0, 80.0);

// Initialize bridge
let bridge = ISO20022Bridge::new();

// Convert to ISO classification
let classification = bridge.esg_to_iso(&score);

println!("SFDR Article: {}", classification.sfdr_article);
println!("Taxonomy Alignment: {}%", classification.taxonomy_alignment);
println!("ERC-8040 Rating: {}", classification.erc8040_rating);
```

### C++ SDK

```cpp
#include <erc8040/iso20022.hpp>

using namespace erc8040;

// Create ESG score
ESGScore score{90, 85, 80, 85, ESGRating::AA};

// Initialize bridge
ISO20022Bridge bridge;

// Convert to ISO classification
auto classification = bridge.esg_to_iso(score);

// Map SFDR article
uint8_t article = bridge.map_sfdr_article(score.rating);
std::cout << "SFDR Article: " << (int)article << std::endl;
```

## Validation & Compliance

### Message Validation
- ISO 20022 schema validation
- ISIN format validation (12 characters, correct check digit)
- LEI format validation (20 characters, ISO 17442 compliant)
- SFDR article values restricted to {6, 8, 9}
- Taxonomy alignment range [0, 100]

### Regulatory Compliance
- **MiFID II**: Instrument identification via ISIN/LEI
- **SFDR**: Mandatory ESG disclosure for Article 8/9 products
- **EU Taxonomy**: Alignment percentage reporting
- **CSDR**: Settlement instruction compatibility

## Error Handling

### Common Issues

1. **Invalid ISIN**: Must be 12 characters, alphanumeric, valid check digit
2. **Invalid LEI**: Must be 20 characters, ISO 17442 format
3. **Invalid SFDR Article**: Must be 6, 8, or 9
4. **Score Out of Range**: ESG scores must be 0-100

### Best Practices

- Always validate input scores before conversion
- Use proper identifiers (ISIN/LEI) from official sources
- Maintain audit trail of ESG score changes
- Version control ESG classifications for historical accuracy

## Performance

- **Conversion Time**: < 1ms for ESG to ISO transformation
- **Message Generation**: < 10ms for complete SETR message
- **Validation**: < 5ms for schema and business rule validation
- **Memory Usage**: < 1KB per classification object

## Future Roadmap

- [ ] Support for PAIN (payment initiation) messages with ESG purpose codes
- [ ] CAMT (cash management) integration for green bond proceeds tracking
- [ ] Real-time ESG score updates via blockchain oracles
- [ ] TCFD (Task Force on Climate-related Financial Disclosures) alignment
- [ ] GRI (Global Reporting Initiative) standards mapping
- [ ] SASB (Sustainability Accounting Standards Board) materiality mapping

## Resources

- [ISO 20022 Message Catalogue](https://www.iso20022.org/catalogue-messages)
- [SFDR Technical Standards](https://www.esma.europa.eu/policy-rules/sustainable-finance)
- [EU Taxonomy Compass](https://ec.europa.eu/sustainable-finance-taxonomy/)
- [LEI Registration](https://www.gleif.org/)

## Support

For technical support or questions about the SWIFT bridge:
- GitHub Issues: [erc-8040-ecosystem](https://github.com/agronetlabs/erc-8040-ecosystem/issues)
- Documentation: [integration/](../)
