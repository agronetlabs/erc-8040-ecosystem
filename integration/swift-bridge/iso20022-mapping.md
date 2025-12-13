# ISO 20022 Field Mapping

## Overview

This document provides a comprehensive field-by-field mapping between ERC-8040 ESG data structures and ISO 20022 XML elements for SETR (Securities Trade) messages.

## ERC-8040 to ISO 20022 Mapping

### ESG Score → ESG Classification

| ERC-8040 Field | Type | ISO 20022 Element | XML Path | Description |
|----------------|------|-------------------|----------|-------------|
| environmental | float (0-100) | TaxnmyAlgnmt | /Document/SctiesTradConf/ESGClssfctn/TaxnmyAlgnmt | EU Taxonomy alignment % (calculated) |
| social | float (0-100) | - | - | Used in overall rating calculation |
| governance | float (0-100) | - | - | Used in overall rating calculation |
| total | float (0-100) | - | - | Used to determine rating |
| rating | enum (AAA-D) | ERC8040Rtg | /Document/SctiesTradConf/ESGClssfctn/ERC8040Rtg | Letter rating |
| rating | enum (AAA-D) | SFDRArtcl | /Document/SctiesTradConf/ESGClssfctn/SFDRArtcl | Mapped to 6, 8, or 9 |

### Financial Instrument Mapping

| ERC-8040 Field | Type | ISO 20022 Element | XML Path | Description |
|----------------|------|-------------------|----------|-------------|
| isin | string (12 chars) | ISIN | /Document/SctiesTradConf/FinInstrmId/ISIN | International Securities ID |
| lei | string (20 chars) | LEI | /Document/SctiesTradConf/FinInstrmId/LEI | Legal Entity Identifier |
| name | string | Nm | /Document/SctiesTradConf/FinInstrmId/Nm | Instrument name |

## ESG Score to SFDR Article Mapping

### Python Implementation

```python
def map_sfdr_article(rating: ESGRating) -> int:
    """
    Map ERC-8040 rating to SFDR article classification.
    
    Returns:
        6, 8, or 9 (SFDR Article number)
    """
    if rating in (ESGRating.AAA, ESGRating.AA, ESGRating.A):
        return 9  # Sustainable investment objective
    elif rating in (ESGRating.BBB, ESGRating.BB):
        return 8  # Promotes ESG characteristics
    else:
        return 6  # No sustainability objective
```

### Rust Implementation

```rust
fn determine_sfdr_article(rating: ESGRating) -> u8 {
    match rating {
        ESGRating::AAA | ESGRating::AA | ESGRating::A => 9,
        ESGRating::BBB | ESGRating::BB => 8,
        _ => 6,
    }
}
```

### C++ Implementation

```cpp
uint8_t map_sfdr_article(ESGRating rating) const {
    switch (rating) {
        case ESGRating::AAA:
        case ESGRating::AA:
        case ESGRating::A:
            return 9;
        case ESGRating::BBB:
        case ESGRating::BB:
            return 8;
        default:
            return 6;
    }
}
```

### Mapping Table

| ERC-8040 Rating | Numeric Score Range | SFDR Article | Investment Type |
|----------------|---------------------|--------------|-----------------|
| AAA | 90-100 | 9 | Sustainable investment |
| AA | 85-89 | 9 | Sustainable investment |
| A | 80-84 | 9 | Sustainable investment |
| BBB | 70-79 | 8 | Promotes ESG |
| BB | 60-69 | 8 | Promotes ESG |
| B | 50-59 | 6 | No ESG objective |
| CCC | 40-49 | 6 | No ESG objective |
| CC | 30-39 | 6 | No ESG objective |
| C | 20-29 | 6 | No ESG objective |
| D | 0-19 | 6 | No ESG objective |

## Taxonomy Alignment Calculation

### Formula

```
If environmental_score >= 80:
    taxonomy_alignment = environmental_score
Elif environmental_score >= 60:
    taxonomy_alignment = (environmental_score - 60) * 2
Else:
    taxonomy_alignment = 0
```

### Examples

| Environmental Score | Calculation | Taxonomy Alignment |
|--------------------|--------------|--------------------|
| 95 | 95 | 95% |
| 85 | 85 | 85% |
| 80 | 80 | 80% |
| 75 | (75 - 60) × 2 = 30 | 30% |
| 65 | (65 - 60) × 2 = 10 | 10% |
| 60 | (60 - 60) × 2 = 0 | 0% |
| 50 | 0 | 0% |

### Implementation Comparison

**Python:**
```python
def calculate_taxonomy_alignment(score: ESGScore) -> float:
    env = score.environmental
    if env >= 80:
        return min(env, 100.0)
    elif env >= 60:
        return (env - 60) * 2.0
    else:
        return 0.0
```

**Rust:**
```rust
fn calculate_taxonomy_alignment(score: &ESGScore) -> f64 {
    let env = score.environmental;
    if env >= 80.0 {
        env.min(100.0)
    } else if env >= 60.0 {
        (env - 60.0) * 2.0
    } else {
        0.0
    }
}
```

**C++:**
```cpp
double calculate_taxonomy_alignment(const ESGScore& score) const {
    double env = static_cast<double>(score.environmental);
    if (env >= 80.0) {
        return std::min(env, 100.0);
    } else if (env >= 60.0) {
        return (env - 60.0) * 2.0;
    } else {
        return 0.0;
    }
}
```

## Example Transformations

### Example 1: High ESG Performance

**Input (ERC-8040):**
```json
{
  "environmental": 90.0,
  "social": 85.0,
  "governance": 80.0,
  "total": 85.0,
  "rating": "AA"
}
```

**Output (ISO 20022):**
```xml
<ESGClssfctn>
  <TaxnmyAlgnmt>90.0</TaxnmyAlgnmt>
  <SFDRArtcl>9</SFDRArtcl>
  <ERC8040Rtg>AA</ERC8040Rtg>
</ESGClssfctn>
```

**Calculation:**
- Environmental score 90 → Taxonomy alignment = 90%
- Rating AA → SFDR Article 9

### Example 2: Medium ESG Performance

**Input (ERC-8040):**
```json
{
  "environmental": 75.0,
  "social": 70.0,
  "governance": 68.0,
  "total": 71.0,
  "rating": "BBB"
}
```

**Output (ISO 20022):**
```xml
<ESGClssfctn>
  <TaxnmyAlgnmt>30.0</TaxnmyAlgnmt>
  <SFDRArtcl>8</SFDRArtcl>
  <ERC8040Rtg>BBB</ERC8040Rtg>
</ESGClssfctn>
```

**Calculation:**
- Environmental score 75 → Taxonomy alignment = (75 - 60) × 2 = 30%
- Rating BBB → SFDR Article 8

### Example 3: Low ESG Performance

**Input (ERC-8040):**
```json
{
  "environmental": 45.0,
  "social": 50.0,
  "governance": 48.0,
  "total": 47.7,
  "rating": "CCC"
}
```

**Output (ISO 20022):**
```xml
<ESGClssfctn>
  <TaxnmyAlgnmt>0.0</TaxnmyAlgnmt>
  <SFDRArtcl>6</SFDRArtcl>
  <ERC8040Rtg>CCC</ERC8040Rtg>
</ESGClssfctn>
```

**Calculation:**
- Environmental score 45 → Taxonomy alignment = 0%
- Rating CCC → SFDR Article 6

## Complete Message Transformation

### Input: ERC-8040 Token Data

```json
{
  "instrument": {
    "isin": "US46434G1031",
    "lei": "549300PZDW6EBUUJ8G35",
    "name": "ERC8040 Sustainable Energy Token"
  },
  "esg_score": {
    "environmental": 92.0,
    "social": 88.0,
    "governance": 85.0,
    "total": 88.33,
    "rating": "AA"
  }
}
```

### Output: ISO 20022 SETR Message

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Document xmlns="urn:iso:std:iso:20022:tech:xsd:setr.010.001.04">
  <SctiesTradConf>
    <FinInstrmId>
      <ISIN>US46434G1031</ISIN>
      <LEI>549300PZDW6EBUUJ8G35</LEI>
      <Nm>ERC8040 Sustainable Energy Token</Nm>
    </FinInstrmId>
    <ESGClssfctn>
      <TaxnmyAlgnmt>92.0</TaxnmyAlgnmt>
      <SFDRArtcl>9</SFDRArtcl>
      <ERC8040Rtg>AA</ERC8040Rtg>
    </ESGClssfctn>
  </SctiesTradConf>
</Document>
```

### Transformation Steps

1. **Extract Financial Instrument Data**
   - ISIN: "US46434G1031"
   - LEI: "549300PZDW6EBUUJ8G35"
   - Name: "ERC8040 Sustainable Energy Token"

2. **Calculate Taxonomy Alignment**
   - Environmental score = 92.0 (>= 80)
   - Taxonomy alignment = 92.0%

3. **Map SFDR Article**
   - Rating = AA
   - SFDR Article = 9

4. **Generate ESG Classification**
   - TaxnmyAlgnmt: 92.0
   - SFDRArtcl: 9
   - ERC8040Rtg: "AA"

5. **Build ISO 20022 XML**
   - Construct FinInstrmId element
   - Construct ESGClssfctn element
   - Wrap in SctiesTradConf and Document

## Carbon Intensity Mapping

### Calculation

```python
def estimate_carbon_intensity(environmental_score: float) -> float:
    max_intensity = 500.0  # tCO2e/$M revenue
    return max_intensity * (1.0 - environmental_score / 100.0)
```

### Transformation Table

| Environmental Score | Formula | Carbon Intensity (tCO2e/$M) |
|--------------------|---------|-----------------------------|
| 100 | 500 × (1 - 1.0) = 0 | 0 |
| 90 | 500 × (1 - 0.9) = 50 | 50 |
| 80 | 500 × (1 - 0.8) = 100 | 100 |
| 70 | 500 × (1 - 0.7) = 150 | 150 |
| 60 | 500 × (1 - 0.6) = 200 | 200 |
| 50 | 500 × (1 - 0.5) = 250 | 250 |
| 0 | 500 × (1 - 0.0) = 500 | 500 |

## Data Type Mappings

| ERC-8040 Type | Python Type | Rust Type | C++ Type | ISO 20022 Type |
|---------------|-------------|-----------|----------|----------------|
| ESG Score (0-100) | float | f64 | double | Decimal |
| Rating | enum ESGRating | enum ESGRating | enum ESGRating | String |
| SFDR Article | int | u8 | uint8_t | Integer |
| ISIN | str | String | std::string | ISIN2021Identifier |
| LEI | str | String | std::string | LEIIdentifier |
| Percentage | float | f64 | double | PercentageRate |

## Validation Rules

### ISIN Validation
- Length: Exactly 12 characters
- Format: 2 letter country code + 9 alphanumeric + 1 check digit
- Example: "US46434G1031"

### LEI Validation
- Length: Exactly 20 characters
- Format: ISO 17442 standard
- Example: "549300PZDW6EBUUJ8G35"

### SFDR Article Validation
- Allowed values: 6, 8, 9
- Type: Unsigned 8-bit integer
- Business rule: Must match ESG rating classification

### Taxonomy Alignment Validation
- Range: 0.0 to 100.0
- Type: Float/Double
- Precision: 2 decimal places recommended

### ESG Rating Validation
- Allowed values: "AAA", "AA", "A", "BBB", "BB", "B", "CCC", "CC", "C", "D"
- Type: String
- Must correspond to total ESG score

## Notes

- **Extension Fields**: The ESGClssfctn block is a custom extension to the standard ISO 20022 schema. In production, this should be formalized through the ISO 20022 Registration Authority.
  
- **Backward Compatibility**: Systems that don't recognize the ESG extension can still process the base SETR message using the FinInstrmId fields.

- **Version Control**: All mapping logic is versioned to ensure consistency across SDK implementations (Python, Rust, C++).

- **Precision**: Floating-point calculations should use consistent precision (2 decimal places) to ensure matching results across platforms.
