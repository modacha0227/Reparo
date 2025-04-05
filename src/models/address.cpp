#include "address.h"
#include "../../src/text_fields.h"
#include <string>
#include <vector>
#include <algorithm> // For std::equal

class Address::Data {
public:
    Data(const Address& address) : data_(address) {}

    const int& ID() const {
        return data_.id;
    }

    const std::vector<std::string>& Lines() const {
        return data_.address;
    }

private:
    Address data_;
};

Address& Address::SetID(int id_) {
    id = id_;
    return *this;
}

Address& Address::SetLines(std::vector<TextField>& _address) {
    address.clear();
    for (const auto& field : _address) {
        address.push_back(field.Get());
    }
    return *this;
}

Address& Address::SetLines(std::vector<std::string>& _address) {
    address = _address; // More efficient to assign directly
    return *this;
}

bool Address::Equals(const Address& _other, bool skip_id) const {
    if (!skip_id && id != _other.id) {
        return false;
    }
    return address == _other.address; // Use direct vector comparison
}

std::string Address::ToString(const std::string& delimiter, const std::string& align) const {
    std::string str;
    for (size_t i = 0; i < address.size(); ++i) {
        if (!address[i].empty()) {
            str += address[i];
            if (i < address.size() - 1) {
                str += delimiter;
            }
        }
    }
    return str;
}

void Address::Clear() {
    id = -1;
    address.clear();
}
