#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

std::map<char, std::vector<bool> > bitsFromHex = {
    {'0', {0, 0, 0, 0}},
    {'1', {0, 0, 0, 1}},
    {'2', {0, 0, 1, 0}},
    {'3', {0, 0, 1, 1}},
    {'4', {0, 1, 0, 0}},
    {'5', {0, 1, 0, 1}},
    {'6', {0, 1, 1, 0}},
    {'7', {0, 1, 1, 1}},
    {'8', {1, 0, 0, 0}},
    {'9', {1, 0, 0, 1}},
    {'A', {1, 0, 1, 0}},
    {'B', {1, 0, 1, 1}},
    {'C', {1, 1, 0, 0}},
    {'D', {1, 1, 0, 1}},
    {'E', {1, 1, 1, 0}},
    {'F', {1, 1, 1, 1}},
};

struct Packet {
    int version = 0;
    int typeID = 0;
    int length = 0; // length of packet, including version, typeid, (length typeID), (determination of subpackets length / num subpackets), (literal value / subpackets length)
    int literal_value;  // if typeID is 4
    int lengthTypeId;  // either 0 or 1 if typeID is not 4
    int subpackets_length; // if length type ID is 0
    int num_subpackets; // if length type ID is 1
    std::vector<int> subpacket_indices; // Indices of subpackets.
};

std::vector<bool> ConvertHexToBits(std::string line)
{
    std::vector<bool> bits;
    for (auto &hexdigit : line)
    {
        std::vector<bool> newbits = bitsFromHex[hexdigit];
        bits.insert(bits.end(), newbits.begin(), newbits.end());
    }
    return bits;
}

void PrintBits(std::vector<bool> bits)
{
    for (const auto &digit : bits)
    {
        std::cout << digit;
    }
    std::cout << std::endl;
    return;
}

int ConvertBitsToValue(std::vector<bool> bits, int startidx, int endidx) {
    std::vector<bool> bitgroup = {bits.begin() + startidx, bits.begin() + endidx};
    int value = 0;
    int binarymultiplier = 1;
    int numbits = endidx - startidx;
    for (int i = 0; i < numbits; ++i) {
        int bit = bitgroup[numbits - i - 1];
        value += binarymultiplier * bit;
        binarymultiplier *= 2;
    }
    return value;
}

struct LiteralValue {
    int value = 0;
    int num_groups_of_five = 0;
    int num_padded_zeros = 0;
};

LiteralValue GetLiteralValue(std::vector<bool> bits, int startidx) {
    LiteralValue literal;
    bool found_leading_0_bit = false;
    std::vector<bool> numberbits;
    int number_of_bits_parsed = 0;
    int fiveStartIdx = startidx;
    while (!found_leading_0_bit) {
        // Get the 5 bits to look at.
        std::vector<bool> bitgroup = {bits.begin() + fiveStartIdx, bits.begin() + fiveStartIdx + 5};
        // PrintBits(bitgroup);
        if (bitgroup[0] == 0) {
            found_leading_0_bit = true;
            numberbits.insert(numberbits.end(), bitgroup.begin() + 1, bitgroup.end());
        } else {
            numberbits.insert(numberbits.end(), bitgroup.begin() + 1, bitgroup.end());
            fiveStartIdx += 5;
        }
        ++ literal.num_groups_of_five;
        number_of_bits_parsed += 5;
    }
    // PrintBits(numberbits);
    literal.value = ConvertBitsToValue(numberbits, 0, numberbits.size());
    literal.num_padded_zeros = (number_of_bits_parsed + 6) % 4;
    if (literal.num_padded_zeros > 0) {
        literal.num_padded_zeros = 4 - literal.num_padded_zeros;
    }

    return literal;
}

bool CheckIfAllZeros(std::vector<bool> bits) {
    bool all_zeros = true;
    for (const auto& bit : bits) {
        all_zeros = all_zeros && (bit == false);
    }
    return all_zeros;
}

void DecipherBits(std::vector<bool> bits, std::vector<Packet>& packets) {
    if (bits.size() < 11 && bits.size() > 0) {
        bool all_zeros = CheckIfAllZeros(bits);
        if (all_zeros) {
            std::cout << "Found zero padding of length: " << bits.size() << std::endl;
        } else {
            std::cout << "Something went wrong at the end. See remaining bits." << std::endl;
        }
    }
    int i = 0;
    Packet packet;
    packet.version = ConvertBitsToValue(bits, i, i+3);
    packet.typeID = ConvertBitsToValue(bits, i+3, i+3+3);
    packet.length = 6;

    if (packet.typeID != 4) {
        packet.lengthTypeId = bits[i+6];
        ++packet.length;
        if (packet.lengthTypeId == 0) {
            int parsebits = 15;
            packet.length += parsebits;
            packet.subpackets_length = ConvertBitsToValue(bits, i+7, i+7+parsebits);
            packet.length += packet.subpackets_length;
            std::vector<bool> subpackets_bits = {bits.begin() + i + 7 + parsebits, bits.begin() + i + 7 + parsebits + packet.subpackets_length};
            int starting_number_of_packets = packets.size();
            while (subpackets_bits.size() > 0) {
                DecipherBits(subpackets_bits, packets);
                int ending_number_of_packets = packets.size();
                int offset = 0;
                if (packets[starting_number_of_packets].typeID != 4 && packets[starting_number_of_packets].lengthTypeId == 0) {
                    offset = packets[starting_number_of_packets].length;
                } else {
                    offset = packets.back().length;
                }
                if (offset < subpackets_bits.size()) {
                    subpackets_bits = {subpackets_bits.begin() + offset, subpackets_bits.end()};    
                } else {
                    subpackets_bits.clear();
                }

                // Save the list of subpackets of this packet.
                for (int j = starting_number_of_packets; j < ending_number_of_packets; ++j) {
                    packet.subpacket_indices.push_back(j);
                }
            }
        } else {
            int parsebits = 11;
            packet.length += parsebits;
            packet.num_subpackets = ConvertBitsToValue(bits, i+7, i+7+parsebits); 
            std::vector<bool> subpackets_bits = {bits.begin() + i + 7 + parsebits, bits.end()};
            int starting_number_of_packets = packets.size();
            for (int j = 0; j < packet.num_subpackets; ++j) {
                DecipherBits(subpackets_bits, packets);
                int offset = 0;
                if (packets[starting_number_of_packets].typeID != 4 && packets[starting_number_of_packets].lengthTypeId == 0) {
                    offset = packets[starting_number_of_packets].length;
                } else {
                    offset = packets.back().length;
                }
                subpackets_bits = {subpackets_bits.begin() + offset, subpackets_bits.end()};
                packet.length += offset;
            }
            // Save the list of subpackets of this packet.
            int ending_number_of_packets = packets.size();
            for (int j = starting_number_of_packets; j < ending_number_of_packets; ++j) {
                packet.subpacket_indices.push_back(j);
            }
        }
    } else {
        auto literal = GetLiteralValue(bits, i+6); 
        packet.literal_value = literal.value;
        packet.length += literal.num_groups_of_five * 5; // + literal.num_padded_zeros;
        i += packet.length;
    }

    packets.push_back(packet);

    return;
}

int SumVersions(const std::vector<Packet>& packets) {
    int sum = 0;
    for (auto& packet : packets) {
        sum = sum + packet.version;
    }
    return sum;
}

int ComputeValue(const std::vector<Packet>& packets, int packet_idx) {
    auto packet = packets[packet_idx];
    if (packet.typeID == 0) {
        // Sum all the subpackets.
        std::vector<int> values;
        for (auto& subidx : packet.subpacket_indices) {
            values.push_back(ComputeValue(packets, subidx));
        }
        int value = 0;
        for (auto & subvalue : values) {
            value = value + subvalue;
        }
        return value;
    } else if (packet.typeID == 1) {
        // Multiply all the subpackets.
        std::vector<int> values;
        for (auto& subidx : packet.subpacket_indices) {
            values.push_back(ComputeValue(packets, subidx));
        }
        int value = 1;
        for (auto & subvalue : values) {
            value = value * subvalue;
        }
        return value;
    } else if (packet.typeID == 2) {
        // Minimum of all the subpackets.
        std::vector<int> values;
        for (auto& subidx : packet.subpacket_indices) {
            values.push_back(ComputeValue(packets, subidx));
        }
        auto min_idx = std::min_element(values.begin(), values.end());
        return *min_idx;
    } else if (packet.typeID == 2) {
        // Minimum of all the subpackets.
        std::vector<int> values;
        for (auto& subidx : packet.subpacket_indices) {
            values.push_back(ComputeValue(packets, subidx));
        }
        auto max_idx = std::max_element(values.begin(), values.end());
        return *max_idx;
    } else if (packet.typeID == 4) { 
        return packet.literal_value;
    } else if (packet.typeID == 5) {
        // Minimum of all the subpackets.
        std::vector<int> values;
        for (auto& subidx : packet.subpacket_indices) {
            values.push_back(ComputeValue(packets, subidx));
        }
        if (values.size() != 2) {
            std::cout << "WARNING FOUND NOT 2 BUT " << values.size() << " VALUES " << std::endl;
        }
        return (values[0] > values[1]) ? 1 : 0;
    } else if (packet.typeID == 6) {
        // Minimum of all the subpackets.
        std::vector<int> values;
        for (auto& subidx : packet.subpacket_indices) {
            values.push_back(ComputeValue(packets, subidx));
        }
        if (values.size() != 2) {
            std::cout << "WARNING FOUND NOT 2 BUT " << values.size() << " VALUES " << std::endl;
        }
        return (values[0] < values[1]) ? 1 : 0;
    } else if (packet.typeID == 7) {
        // Minimum of all the subpackets.
        std::vector<int> values;
        for (auto& subidx : packet.subpacket_indices) {
            values.push_back(ComputeValue(packets, subidx));
        }
        if (values.size() != 2) {
            std::cout << "WARNING FOUND NOT 2 BUT " << values.size() << " VALUES " << std::endl;
        }
        return (values[0] == values[1]) ? 1 : 0;
    } 

    return 0;
}

void PrintPackets(const std::vector<Packet>& packets) {
    for (auto & packet : packets) {
        if (packet.typeID == 4) {
            std::cout << "Found literal value: " << packet.literal_value << std::endl;
        } else {
            std::cout << "Found packet of typeID " << packet.typeID << " with subpackets: ";
            for (auto& idx : packet.subpacket_indices) {
                std::cout << idx << ", ";
            }
            std::cout << std::endl;
        }
    }
    return;
}

int main()
{
    std::string line;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> line)
        {
            // std::cout << line << std::endl;
        }
        myfile.close();
    }

    auto bits = ConvertHexToBits(line);
    // PrintBits(bits);

    std::vector<Packet> packets;
    DecipherBits(bits, packets);

    std::cout << "Found " << packets.size() << " packets. " << std::endl;
    PrintPackets(packets);

    int solution = ComputeValue(packets, packets.size() - 1);
    std::cout << "Solution = " << solution << std::endl;

    return 0;
}