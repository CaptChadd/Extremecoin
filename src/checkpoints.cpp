// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2012 Litecoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of // Yo dawg, this is the secret. Checkpoint 0 hash == Genesis block hash.
        (         0, uint256("0xc6f87e3d194e61bd61bb13d2d50ff712eb3ce180467b50806b10989fac4ed561"))
		(         100, uint256("0x57960bc8ab65dc552fd1dd0b3cf771041155b35c8ceeb087b764730eea6040ff"))
		(         500, uint256("0xf271b8a943dffab218f73bddd281ebce03e9b7cfc1688b39659b4a7b2490b8aa"))
		(         1000, uint256("0x52ff7b14be280b1a4cc5c12d0c0a18bd95ed5964d015c11b373401ccf6b1e953"))
		(         2000, uint256("0x9d06c49cde83f002131c8212a05debd8f03b0d4b80d77ea59c311ceea75308c8"))
        ;

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints

        MapCheckpoints::const_iterator i = mapCheckpoints.find(nHeight);
        if (i == mapCheckpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0;
        return mapCheckpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (fTestNet) return NULL;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, mapCheckpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
