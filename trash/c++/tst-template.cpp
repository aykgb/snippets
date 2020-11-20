#include <iostream>
#include <stdint.h>

using namespace std;

/*
** Common data types that can be used by the HAL
*/
typedef unsigned int    hal_uint_t;
typedef unsigned int    hal_bits_t;
typedef int             hal_int_t;

typedef uint8_t         hal_bool8_t;

typedef uint8_t         hal_bits8_t;
typedef uint16_t        hal_bits16_t;
typedef uint32_t        hal_bits32_t;
typedef uint64_t        hal_bits64_t;

typedef uint8_t         hal_uint8_t;
typedef uint16_t        hal_uint16_t;
typedef uint32_t        hal_uint32_t;
typedef uint64_t        hal_uint64_t;

typedef int8_t          hal_int8_t;
typedef int16_t         hal_int16_t;
typedef int32_t         hal_int32_t;
typedef int64_t         hal_int64_t;

typedef hal_uint8_t     hal_macaddr_t[6];
typedef hal_uint8_t     hal_ipv6addr_t[16];


/*
** Generic Interface Enum and Data Structure for identifying an interface
** type.
*/
typedef enum {
    HAL_InterfaceTypeNotApplicable = 0,
    HAL_InterfaceTypePhysicalPort,
    HAL_InterfaceTypeTrunk,
    HAL_InterfaceTypeTmFlowId,
    HAL_InterfaceTypeLogicalPort
} HAL_INTERFACE_TYPE_e;

typedef struct {
    hal_uint16_t slot;        // 0-based slot number
    hal_uint16_t localPort;   // 0-based local port number (relative to the slot)	//>
#ifdef __cplusplus
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & slot;
        ar & localPort;
    }
#endif
} physicalPortInterface_s;

/* HAL_InterfaceTypeTrunk interfaceType Identifier */
typedef struct {
    hal_int_t trunkNum;    // 0-based Trunk ID
} trunkInterface_s;

/* HAL_InterfaceTypeTmFlowId interfaceType Identifier */
typedef struct {
    hal_int_t tmFlowId;
} tmFlowIdInterface_s;

typedef struct {
    HAL_INTERFACE_TYPE_e interfaceType;

    /*
    ** Use the structure corresponding to the type specified by interfaceType
    */
    union {
        physicalPortInterface_s physicalPortInterface;

        /* HAL_InterfaceTypeLogicalPort interfaceType Identifier */
        struct {
            hal_uint16_t slot;        	// 0-based slot number
            hal_uint16_t logicalPort;   // 0-based local port number (relative to the slot)
        } logicalPortInterface;

        trunkInterface_s trunkInterface;

        tmFlowIdInterface_s tmFlowIdInterface;
    };
#ifdef __cplusplus

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & interfaceType;
        // ar & boost::serialization::make_array(&physicalPortInterface, 1);

    }
#endif
} HAL_InterfaceId_s;

int main()
{
    cout << sizeof(physicalPortInterface_s) << endl;

    return 0;
}