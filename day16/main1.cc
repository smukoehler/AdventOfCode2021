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
    PrintBits(bitgroup);
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

void DecipherBits(std::vector<bool> bits, std::vector<Packet>& packets) {
    PrintBits(bits);
    int i = 0;
    Packet packet;
    packet.version = ConvertBitsToValue(bits, i, i+3);
    packet.typeID = ConvertBitsToValue(bits, i+3, i+3+3);
    packet.length = 6;

    std::cout << "version: " << packet.version << std::endl;
    std::cout << "typeID: " <<  packet.typeID << std::endl;

    if (packet.typeID != 4) {
        int lengthTypeID = bits[i+6];
        ++packet.length;
        if (lengthTypeID == 0) {
            int parsebits = 15;
            packet.length += parsebits;
            packet.subpackets_length = ConvertBitsToValue(bits, i+7, i+7+parsebits);
            packet.length += packet.subpackets_length;
            std::cout << "length of all subpackets: " << packet.subpackets_length << std::endl;
            std::vector<bool> subpackets_bits = {bits.begin() + i+7+parsebits, bits.begin() + i+7+parsebits + packet.subpackets_length};
            while (subpackets_bits.size() > 0) {
                std::cout << "deciphering subpackets" << std::endl;
                PrintBits(subpackets_bits);
                DecipherBits(subpackets_bits, packets);
                subpackets_bits = {subpackets_bits.begin() + packets.back().length, subpackets_bits.end()};
            }
        } else {
            int parsebits = 11;
            packet.length += parsebits;
            packet.num_subpackets = ConvertBitsToValue(bits, i+7, i+7+parsebits); 
            std::cout << "num subpackets: " << packet.num_subpackets << std::endl;
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
    PrintBits(bits);

    std::vector<Packet> packets;
    DecipherBits(bits, packets);

    std::cout << "Found " << packets.size() << " packets. " << std::endl;

    return 0;
}