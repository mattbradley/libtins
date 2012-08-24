#include <gtest/gtest.h>
#include <algorithm>
#include <memory>
#include <stdint.h>
#include "dot11.h"
#include "tests/dot11.h"


using namespace std;
using namespace Tins;

typedef Dot11::address_type address_type;

class Dot11ProbeResponseTest : public testing::Test {
public:
    static const address_type empty_addr, hwaddr;
    static const uint8_t expected_packet[];
};

const uint8_t Dot11ProbeResponseTest::expected_packet[] = { 
    'Q', '\x01', 'O', '#', '\x00', '\x01', '\x02', '\x03', '\x04', 
    '\x05', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x02', 
    '\x03', '\x04', '\x05', '\x06', '\x07', '\x00', '\x00', '\x91', 
    '\x8a', '\x83', '\'', '\xdf', '\x98', '\xa6', '\x17', '\x8d', 
    '\x92', '\x00', '\x00'
};

void test_equals(const Dot11ProbeResponse &dot1, const Dot11ProbeResponse &dot2) {
    EXPECT_EQ(dot1.interval(), dot2.interval());
    EXPECT_EQ(dot1.timestamp(), dot2.timestamp());
    test_equals(
        static_cast<const Dot11ManagementFrame&>(dot1),
        static_cast<const Dot11ManagementFrame&>(dot2)
    );
}

void test_equals_expected(const Dot11ProbeResponse &dot11) {
    test_equals_expected(static_cast<const Dot11ManagementFrame&>(dot11));
    EXPECT_EQ(dot11.timestamp(), 0x17a698df27838a91);
    EXPECT_EQ(dot11.interval(), 0x928d);
    EXPECT_EQ(dot11.subtype(), Dot11::PROBE_RESP);
}

TEST_F(Dot11ProbeResponseTest, Constructor) {
    Dot11ProbeResponse dot11;
    test_equals_empty(static_cast<const Dot11ManagementFrame&>(dot11));
    EXPECT_EQ(dot11.timestamp(), 0);
    EXPECT_EQ(dot11.interval(), 0);
    EXPECT_EQ(dot11.subtype(), Dot11::PROBE_RESP);
}

TEST_F(Dot11ProbeResponseTest, ConstructorFromBuffer) {
    Dot11ProbeResponse dot11(expected_packet, sizeof(expected_packet));
    test_equals_expected(dot11);
}

TEST_F(Dot11ProbeResponseTest, CopyConstructor) {
    Dot11ProbeResponse dot1(expected_packet, sizeof(expected_packet));
    Dot11ProbeResponse dot2(dot1);
    test_equals(dot1, dot2);
}

TEST_F(Dot11ProbeResponseTest, CopyAssignmentOperator) {
    Dot11ProbeResponse dot1(expected_packet, sizeof(expected_packet));
    Dot11ProbeResponse dot2;
    dot2 = dot1;
    test_equals(dot1, dot2);
}

TEST_F(Dot11ProbeResponseTest, ClonePDU) {
    Dot11ProbeResponse dot1(expected_packet, sizeof(expected_packet));
    std::auto_ptr<Dot11ProbeResponse> dot2(dot1.clone_pdu());
    test_equals(dot1, *dot2);
}

TEST_F(Dot11ProbeResponseTest, FromBytes) {
    std::auto_ptr<PDU> dot11(Dot11::from_bytes(expected_packet, sizeof(expected_packet)));
    ASSERT_TRUE(dot11.get());
    const Dot11ProbeResponse *inner = dot11->find_inner_pdu<Dot11ProbeResponse>();
    ASSERT_TRUE(inner);
    test_equals_expected(*inner);
}
