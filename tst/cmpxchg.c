#include "cmpxchg.h"
#define CONSENSUS_NUMBER 0xFF
#define CONSENSUS_SETUP cmpxchg_setup()
#define CONSENSUS_METHOD cmpxchg_consensus
#include "tst.h"
