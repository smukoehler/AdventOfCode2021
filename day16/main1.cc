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
    std::vector<int> literal_values; // if there are subpackets??
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
    PrintBits(bits);
    LiteralValue literal;
    bool found_leading_0_bit = false;
    std::vector<bool> numberbits;
    int number_of_bits_parsed = 0;
    int fiveStartIdx = startidx;
    while (!found_leading_0_bit) {
        // Get the 5 bits to look at.
        std::vector<bool> bitgroup = {bits.begin() + fiveStartIdx, bits.begin() + fiveStartIdx + 5};
        PrintBits(bitgroup);
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
    PrintBits(numberbits);
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
    PrintBits(bits);
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

    std::cout << "version: " << packet.version << std::endl;
    std::cout << "typeID: " <<  packet.typeID << std::endl;

    if (packet.typeID != 4) {
        packet.lengthTypeId = bits[i+6];
        ++packet.length;
        if (packet.lengthTypeId == 0) {
            int parsebits = 15;
            packet.length += parsebits;
            packet.subpackets_length = ConvertBitsToValue(bits, i+7, i+7+parsebits);
            packet.length += packet.subpackets_length;
            std::cout << "length of all subpackets: " << packet.subpackets_length << std::endl;
            std::vector<bool> subpackets_bits = {bits.begin() + i + 7 + parsebits, bits.begin() + i + 7 + parsebits + packet.subpackets_length};
            int starting_number_of_packets = packets.size();
            std::cout << "starting_number_of_packets: " << starting_number_of_packets << std::endl;
            while (subpackets_bits.size() > 0) {
                std::cout << "deciphering subpackets" << std::endl;
                PrintBits(subpackets_bits);
                DecipherBits(subpackets_bits, packets);
                int ending_number_of_packets = packets.size();
                std::cout << "ending_number_of_packets: " << ending_number_of_packets << std::endl;
                int offset = 0;
                if (packets[starting_number_of_packets].typeID != 4 && packets[starting_number_of_packets].lengthTypeId == 0) {
                    offset = packets[starting_number_of_packets].length;
                } else {
                    offset = packets.back().length;
                }
                std::cout << "compute new offset: " << offset << std::endl;
                if (offset < subpackets_bits.size()) {
                    subpackets_bits = {subpackets_bits.begin() + offset, subpackets_bits.end()};    
                } else {
                    subpackets_bits.clear();
                }
            }
            std::cout << "Done with this round of subpackets. " << std::endl;
        } else {
            int parsebits = 11;
            packet.length += parsebits;
            packet.num_subpackets = ConvertBitsToValue(bits, i+7, i+7+parsebits); 
            std::cout << "num subpackets: " << packet.num_subpackets << std::endl;
            std::vector<bool> subpackets_bits = {bits.begin() + i + 7 + parsebits, bits.end()};
            int starting_number_of_packets = packets.size();
            std::cout << "starting_number_of_packets: " << starting_number_of_packets << std::endl;
            for (int j = 0; j < packet.num_subpackets; ++j) {
                std::cout << "deciphering subpacket " << j << std::endl;
                PrintBits(subpackets_bits);
                DecipherBits(subpackets_bits, packets);
                int offset = 0;
                if (packets[starting_number_of_packets].typeID != 4 && packets[starting_number_of_packets].lengthTypeId == 0) {
                    offset = packets[starting_number_of_packets].length;
                } else {
                    offset = packets.back().length;
                }
                std::cout << "compute new offset: " << offset << std::endl;
                subpackets_bits = {subpackets_bits.begin() + offset, subpackets_bits.end()};
                packet.length += offset;
            }
        }
    } else {
        auto literal = GetLiteralValue(bits, i+6); 
        packet.literal_value = literal.value;
        packet.length += literal.num_groups_of_five * 5; // + literal.num_padded_zeros;
        i += packet.length;
        std::cout << "literal value: " << literal.value << std::endl;
        std::cout << "num groups of five: " << literal.num_groups_of_five << std::endl;
        std::cout << "num padded zeros: " << literal.num_padded_zeros << std::endl;
        std::cout << "packet length: " << packet.length << std::endl;
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

    int sum_versions = SumVersions(packets);
    std::cout << "Sum of versions = " << sum_versions << std::endl;

    return 0;
}