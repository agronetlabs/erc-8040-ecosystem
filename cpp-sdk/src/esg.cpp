#include "erc8040/esg.hpp"

namespace erc8040 {

bool ESGScore::is_investment_grade() const {
    return rating == ESGRating::AAA || 
           rating == ESGRating::AA || 
           rating == ESGRating::A || 
           rating == ESGRating::BBB;
}

ESGScoring::ESGScoring(double env_weight, double social_weight, double gov_weight)
    : env_weight_(env_weight), social_weight_(social_weight), gov_weight_(gov_weight) {}

ESGScore ESGScoring::calculate(double environmental, double social, double governance) const {
    double weighted = environmental * env_weight_ + 
                      social * social_weight_ + 
                      governance * gov_weight_;
    uint8_t total = static_cast<uint8_t>(weighted);
    ESGRating rating = rating_from_score(total);
    
    return ESGScore{
        static_cast<uint8_t>(environmental),
        static_cast<uint8_t>(social),
        static_cast<uint8_t>(governance),
        total,
        rating
    };
}

ESGRating ESGScoring::rating_from_score(uint8_t score) {
    if (score >= 90) return ESGRating::AAA;
    if (score >= 80) return ESGRating::AA;
    if (score >= 70) return ESGRating::A;
    if (score >= 60) return ESGRating::BBB;
    if (score >= 50) return ESGRating::BB;
    if (score >= 40) return ESGRating::B;
    if (score >= 30) return ESGRating::CCC;
    if (score >= 20) return ESGRating::CC;
    if (score >= 10) return ESGRating::C;
    return ESGRating::D;
}

std::string ESGScoring::rating_to_string(ESGRating rating) {
    switch (rating) {
        case ESGRating::AAA: return "AAA";
        case ESGRating::AA: return "AA";
        case ESGRating::A: return "A";
        case ESGRating::BBB: return "BBB";
        case ESGRating::BB: return "BB";
        case ESGRating::B: return "B";
        case ESGRating::CCC: return "CCC";
        case ESGRating::CC: return "CC";
        case ESGRating::C: return "C";
        case ESGRating::D: return "D";
        default: return "Unknown";
    }
}

} // namespace erc8040