#include "attributes/Health.h"
#include <stdexcept>

Health::Health(int unitHealth) : max_(unitHealth), cur_(unitHealth) {
    if (unitHealth <= 0) {
        throw std::invalid_argument("Health->creator: unitHealth <= 0");
    }
}

void Health::decrease(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Health->decrease: amount < 0");
    }
    if (amount >= cur_) {
        cur_ = 0;
    } else {
        cur_ -= amount;
    }
}
void Health::increase(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Health->increase: amount < 0");
    }
    if (amount >= max_ - cur_) {
        cur_ = max_;
    } else {
        cur_ += amount;
    }
}

bool Health::isZero() const {
    return cur_ == 0;
}
int Health::currentHealth() const {
    return cur_;
}

int Health::maxHealth() const {
    return max_;
}
