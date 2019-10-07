#include <stdio.h>

int main()
{
    const char *funlist[] = {
        "forceOduBdi",
        "getOduBdiForced",
        "forceGenericAis",
        "getGenericAisForced",
        "forceGenericAisInternal",
        "getGenericAisForcedInternal",
        "forceOduIndication",
        "getOduIndicationForced",
        "forceOduInternalIndication",
        "getOduInternalIndicationForced",
        "getOduInternalLck",
        "getOduInternalOci",
        "forceEtherCsfFdiGfp",
        "getEtherCsfRdiGfpForced",
        "forceEtherCsfLosGfp",
        "forceEtherCsfLosyncGfp",
        "getEtherCsfFdiGfpForced",
        "forceEtherCsfRdiGfp",
        "getEtherLfInternalForced",
        "forceEtherLfInternal",
        "getEtherRfForced",
        "forceEtherRf",
        "getEtherRfInternalForced",
        "forceEtherRfInternal",
        "forceEtherLf"
    };

    printf(funlist[0]);
    printf("\n");

    return 0;
}
