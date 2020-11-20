#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    unsigned long long  counter = 12345678;

    std::cout << "+============== Port Statistics Information ===============+" << std::endl;
    std::cout << "|                        portId:" << 0 << "                          |" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_DROP_EVENTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_DROP_FRAMES" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "IF_IN_DISCARDS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "IF_IN_ERRORS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_IF_UCAST_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_IF_BROADCAST_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "IF_IN_BROADCAST_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_IF_MULTICAST_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "IF_IN_MULTICAST_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_FRAME_CHECK_SEQUENCE_ERRORS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_OVERSIZE_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_JABBER" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_UNDERSIZE_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_FRAGMENTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_PKTS_64_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_PKTS_65_TO_127_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_PKTS_128_TO_255_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_PKTS_256_TO_511_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_PKTS_512_TO_1023_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_PKTS_1024_TO_1518_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_ETHER_STATS_PKTS_1519_TO_MAX_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "TX_ETHER_STATS_DROP_EVENTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "IF_OUT_ERRORS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "TX_IF_UCAST_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "TX_IF_BROADCAST_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "TX_IF_MULTICAST_PKTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "TX_ETHER_STATS_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "TX_PAUSE_MAC_CTRL_FRAMES" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_PAUSE_MAC_CTRL_FRAMES" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|" << setw(42) << right << "RX_IF_OCTS" << "|" << dec << setw(14) << right << counter << setw(2) << right << "|" << std::endl;

}

