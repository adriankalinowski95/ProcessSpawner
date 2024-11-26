#pragma once

#include <random>
#include <limits>

namespace shared::application::utils {

class RandomValueGenerator {
public:
    RandomValueGenerator() = default;

    [[nodiscard]] int generateRandomValue(int minValue = Min_Number_Value, int maxValue = Max_Number_Value) const {
        std::random_device device;
        std::mt19937 generator(device());
        std::uniform_int_distribution<> distributor(Min_Number_Value, Max_Number_Value);

        return distributor(generator);
    }

private:
    static constexpr std::int32_t Min_Number_Value = 6959333;
    static constexpr std::int32_t Max_Number_Value = std::numeric_limits<std::int32_t>::max();
};

}