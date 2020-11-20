#include <stdio.h>
#include <stdint.h>

typedef struct {
	uint16_t slot;        // 0-based slot number
    uint16_t localPort;   // 0-based local port number (relative to the slot)

    template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
    {
      ar & slot;
      ar & localPort;
    }
} Interface_s;

int main()
{
	Interface_s obj_ts;
	obj_ts.slot = 10;
	obj_ts.localPort = 101;
	
	uint16_t var_ui = 0xffff;
	
	obj_ts.serialize(var_ui, 0);
	
	printf("0x%x\n", var_ui);
	
	return 0;
}