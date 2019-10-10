#include <stdio.h>
#include <stdlib.h>

enum PARTITIONS {
    CONTROL,
    MAN,
    PORT_SUB,
    POWER,
    LED,
    THERM,
    FABRIC,
    TM,
    CARD_SUB,
    TIMING,
    OAM_SUB,
    PKT_SUB,
    PM,
    OTN_SUB,
    MAX_QUEUES
};

class base {
public:
    base()
    {
        printf("base()");
        return;
    }
    ~base()
    {
        printf("~base()");
        return;
    }
};

extern "C" const char *glbMsgQueue[] = {
    "this is a test\n",
    "test\n",
    "tst\n"
};

int main()
{
    // printf(glbMsgQueue[0]);
    base b;

    return 0;
}