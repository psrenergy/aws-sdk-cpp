﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/gamelift/GameLift_EXPORTS.h>
#include <aws/gamelift/GameLiftRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace GameLift
{
namespace Model
{

  /**
   * <p>Represents the input for a request operation.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/gamelift-2015-10-01/StopGameSessionPlacementInput">AWS
   * API Reference</a></p>
   */
  class AWS_GAMELIFT_API StopGameSessionPlacementRequest : public GameLiftRequest
  {
  public:
    StopGameSessionPlacementRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<StopGameSessionPlacementRequest> Clone() const
    {
      return Aws::MakeUnique<StopGameSessionPlacementRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "StopGameSessionPlacement"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>A unique identifier for a game session placement to cancel.</p>
     */
    inline const Aws::String& GetPlacementId() const{ return m_placementId; }

    /**
     * <p>A unique identifier for a game session placement to cancel.</p>
     */
    inline bool PlacementIdHasBeenSet() const { return m_placementIdHasBeenSet; }

    /**
     * <p>A unique identifier for a game session placement to cancel.</p>
     */
    inline void SetPlacementId(const Aws::String& value) { m_placementIdHasBeenSet = true; m_placementId = value; }

    /**
     * <p>A unique identifier for a game session placement to cancel.</p>
     */
    inline void SetPlacementId(Aws::String&& value) { m_placementIdHasBeenSet = true; m_placementId = std::move(value); }

    /**
     * <p>A unique identifier for a game session placement to cancel.</p>
     */
    inline void SetPlacementId(const char* value) { m_placementIdHasBeenSet = true; m_placementId.assign(value); }

    /**
     * <p>A unique identifier for a game session placement to cancel.</p>
     */
    inline StopGameSessionPlacementRequest& WithPlacementId(const Aws::String& value) { SetPlacementId(value); return *this;}

    /**
     * <p>A unique identifier for a game session placement to cancel.</p>
     */
    inline StopGameSessionPlacementRequest& WithPlacementId(Aws::String&& value) { SetPlacementId(std::move(value)); return *this;}

    /**
     * <p>A unique identifier for a game session placement to cancel.</p>
     */
    inline StopGameSessionPlacementRequest& WithPlacementId(const char* value) { SetPlacementId(value); return *this;}

  private:

    Aws::String m_placementId;
    bool m_placementIdHasBeenSet = false;
  };

} // namespace Model
} // namespace GameLift
} // namespace Aws
