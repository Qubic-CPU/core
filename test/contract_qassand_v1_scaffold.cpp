#define NO_UEFI

#include "gtest/gtest.h"

#include <cstdint>

using uint8 = uint8_t;
using uint64 = uint64_t;

static constexpr uint64 QASSAND_V1_MAX_MARKETS = 65536;

enum QassandV1MarketStatus : uint8
{
	QASSAND_V1_MARKET_EMPTY = 0,
	QASSAND_V1_MARKET_ACTIVE = 1,
	QASSAND_V1_MARKET_MATURED = 2,
	QASSAND_V1_MARKET_ORACLE_PENDING = 3,
	QASSAND_V1_MARKET_DISPUTE_OPEN = 4,
	QASSAND_V1_MARKET_SETTLEMENT_READY = 5,
	QASSAND_V1_MARKET_SETTLED = 6,
	QASSAND_V1_MARKET_INVALID = 7,
	QASSAND_V1_MARKET_FROZEN = 8
};

struct QassandV1MarketState
{
	uint64 marketId;
	uint8 status;
};

static bool qassandV1MarketExists(
	uint64 marketId,
	const bool* marketInitialized,
	const QassandV1MarketState* markets
)
{
	if (marketId == 0)
	{
		return false;
	}

	if (marketId >= QASSAND_V1_MAX_MARKETS)
	{
		return false;
	}

	if (!marketInitialized[marketId])
	{
		return false;
	}

	if (markets[marketId].marketId != marketId)
	{
		return false;
	}

	return true;
}

TEST(QassandV1Scaffold, MarketExistsRejectsZeroMarketId)
{
	bool initialized[4] = {};
	QassandV1MarketState markets[4] = {};

	EXPECT_FALSE(qassandV1MarketExists(0, initialized, markets));
}

TEST(QassandV1Scaffold, MarketExistsRejectsUninitializedMarket)
{
	bool initialized[4] = {};
	QassandV1MarketState markets[4] = {};

	EXPECT_FALSE(qassandV1MarketExists(1, initialized, markets));
}

TEST(QassandV1Scaffold, MarketExistsRejectsMismatchedStoredMarketId)
{
	bool initialized[4] = {};
	QassandV1MarketState markets[4] = {};

	initialized[1] = true;
	markets[1].marketId = 2;

	EXPECT_FALSE(qassandV1MarketExists(1, initialized, markets));
}

TEST(QassandV1Scaffold, MarketExistsAcceptsInitializedMatchingMarket)
{
	bool initialized[4] = {};
	QassandV1MarketState markets[4] = {};

	initialized[1] = true;
	markets[1].marketId = 1;
	markets[1].status = QASSAND_V1_MARKET_EMPTY;

	EXPECT_TRUE(qassandV1MarketExists(1, initialized, markets));
}
