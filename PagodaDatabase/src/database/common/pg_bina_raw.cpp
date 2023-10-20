#include "pgpch.h"
#include "pg_bina_raw.h"

namespace Pagoda::Database {
    char SwapFlag(char flag) {
        if (flag == ENDIAN_FLAG_LITTLE) {
            return ENDIAN_FLAG_BIG;
        }
        return ENDIAN_FLAG_LITTLE;
    }

    void SwapBINAHeader(BINAHeader* bh) {
        bh->endianFlag = SwapFlag(bh->endianFlag);
        bh->fileSize = _byteswap_ulong(bh->fileSize);
        bh->nodeCount = _byteswap_ushort(bh->nodeCount);
    }

    void SwapNodeHeader(NodeHeader* nh) {
        nh->length = _byteswap_ulong(nh->length);
        nh->stringTableOffset = _byteswap_ulong(nh->stringTableOffset);
        nh->stringTableLength = _byteswap_ulong(nh->stringTableLength);
        nh->offsetTableLength = _byteswap_ulong(nh->offsetTableLength);
        nh->additionalDataLength = _byteswap_ushort(nh->additionalDataLength);
    }
}