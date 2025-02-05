﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/gamelift/GameLift_EXPORTS.h>
#include <aws/gamelift/model/GameSessionPlacement.h>
#include <utility>

namespace Aws
{
template<typename RESULT_TYPE>
class AmazonWebServiceResult;

namespace Utils
{
namespace Json
{
  class JsonValue;
} // namespace Json
} // namespace Utils
namespace GameLift
{
namespace Model
{
  class AWS_GAMELIFT_API StopGameSessionPlacementResult
  {
  public:
    StopGameSessionPlacementResult();
    StopGameSessionPlacementResult(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);
    StopGameSessionPlacementResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);


    /**
     * <p>Object that describes the canceled game session placement, with
     * <code>CANCELLED</code> status and an end time stamp. </p>
     */
    inline const GameSessionPlacement& GetGameSessionPlacement() const{ return m_gameSessionPlacement; }

    /**
     * <p>Object that describes the canceled game session placement, with
     * <code>CANCELLED</code> status and an end time stamp. </p>
     */
    inline void SetGameSessionPlacement(const GameSessionPlacement& value) { m_gameSessionPlacement = value; }

    /**
     * <p>Object that describes the canceled game session placement, with
     * <code>CANCELLED</code> status and an end time stamp. </p>
     */
    inline void SetGameSessionPlacement(GameSessionPlacement&& value) { m_gameSessionPlacement = std::move(value); }

    /**
     * <p>Object that describes the canceled game session placement, with
     * <code>CANCELLED</code> status and an end time stamp. </p>
     */
    inline StopGameSessionPlacementResult& WithGameSessionPlacement(const GameSessionPlacement& value) { SetGameSessionPlacement(value); return *this;}

    /**
     * <p>Object that describes the canceled game session placement, with
     * <code>CANCELLED</code> status and an end time stamp. </p>
     */
    inline StopGameSessionPlacementResult& WithGameSessionPlacement(GameSessionPlacement&& value) { SetGameSessionPlacement(std::move(value)); return *this;}

  private:

    GameSessionPlacement m_gameSessionPlacement;
  };

} // namespace Model
} // namespace GameLift
} // namespace Aws
