/*
 * Copyright (c)2020 ZeroTier, Inc.
 *
 * Use of this software is governed by the Business Source License included
 * in the LICENSE.TXT file in the project's root directory.
 *
 * Change Date: 2023-01-01
 *
 * On the date above, in accordance with the Business Source License, use
 * of this software will be governed by version 2.0 of the Apache License.
 */
/****/

#ifndef ZT_DNS_HPP
#define ZT_DNS_HPP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Buffer.hpp"
#include "InetAddress.hpp"
#include "../include/ZeroTierOne.h"

namespace ZeroTier {

/**
 * DNS data serealization methods
 */
class DNS {
public:
    template<unsigned int C>
    static inline void serializeDNS(Buffer<C> &b, const ZT_VirtualNetworkDNS *dns, unsigned int dnsCount)
    {
        for(unsigned int i = 0; i < dnsCount; ++i) {
            b.append(dns[i].domain, 128);
            for(unsigned int j = 0; j < ZT_MAX_DNS_SERVERS; ++j) {
                InetAddress tmp(dns[i].server_addr[j]);
                tmp.serialize(b);
            }
        }
    }

    template<unsigned int C>
    static inline void deserializeDNS(const Buffer<C> &b, unsigned int &p, ZT_VirtualNetworkDNS *dns, const unsigned int dnsCount)
    {
        memset(dns, 0, sizeof(ZT_VirtualNetworkDNS)*ZT_MAX_NETWORK_DNS);
        for(unsigned int i = 0; i < dnsCount; ++i) {
            char *d = (char*)b.data()+p;
            memcpy(dns[i].domain, d, 128);
            p += 128;
            for (unsigned int j = 0; j < ZT_MAX_DNS_SERVERS; ++j) {
                p += reinterpret_cast<InetAddress *>(&(dns[i].server_addr[j]))->deserialize(b, p);
            }
        }
    }
};

}

#endif // ZT_DNS_HPP
