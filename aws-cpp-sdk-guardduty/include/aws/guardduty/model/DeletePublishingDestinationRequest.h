﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/guardduty/GuardDuty_EXPORTS.h>
#include <aws/guardduty/GuardDutyRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace GuardDuty
{
namespace Model
{

  /**
   */
  class AWS_GUARDDUTY_API DeletePublishingDestinationRequest : public GuardDutyRequest
  {
  public:
    DeletePublishingDestinationRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DeletePublishingDestinationRequest> Clone() const
    {
      return Aws::MakeUnique<DeletePublishingDestinationRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeletePublishingDestination"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The unique ID of the detector associated with the publishing destination to
     * delete.</p>
     */
    inline const Aws::String& GetDetectorId() const{ return m_detectorId; }

    /**
     * <p>The unique ID of the detector associated with the publishing destination to
     * delete.</p>
     */
    inline bool DetectorIdHasBeenSet() const { return m_detectorIdHasBeenSet; }

    /**
     * <p>The unique ID of the detector associated with the publishing destination to
     * delete.</p>
     */
    inline void SetDetectorId(const Aws::String& value) { m_detectorIdHasBeenSet = true; m_detectorId = value; }

    /**
     * <p>The unique ID of the detector associated with the publishing destination to
     * delete.</p>
     */
    inline void SetDetectorId(Aws::String&& value) { m_detectorIdHasBeenSet = true; m_detectorId = std::move(value); }

    /**
     * <p>The unique ID of the detector associated with the publishing destination to
     * delete.</p>
     */
    inline void SetDetectorId(const char* value) { m_detectorIdHasBeenSet = true; m_detectorId.assign(value); }

    /**
     * <p>The unique ID of the detector associated with the publishing destination to
     * delete.</p>
     */
    inline DeletePublishingDestinationRequest& WithDetectorId(const Aws::String& value) { SetDetectorId(value); return *this;}

    /**
     * <p>The unique ID of the detector associated with the publishing destination to
     * delete.</p>
     */
    inline DeletePublishingDestinationRequest& WithDetectorId(Aws::String&& value) { SetDetectorId(std::move(value)); return *this;}

    /**
     * <p>The unique ID of the detector associated with the publishing destination to
     * delete.</p>
     */
    inline DeletePublishingDestinationRequest& WithDetectorId(const char* value) { SetDetectorId(value); return *this;}


    /**
     * <p>The ID of the publishing destination to delete.</p>
     */
    inline const Aws::String& GetDestinationId() const{ return m_destinationId; }

    /**
     * <p>The ID of the publishing destination to delete.</p>
     */
    inline bool DestinationIdHasBeenSet() const { return m_destinationIdHasBeenSet; }

    /**
     * <p>The ID of the publishing destination to delete.</p>
     */
    inline void SetDestinationId(const Aws::String& value) { m_destinationIdHasBeenSet = true; m_destinationId = value; }

    /**
     * <p>The ID of the publishing destination to delete.</p>
     */
    inline void SetDestinationId(Aws::String&& value) { m_destinationIdHasBeenSet = true; m_destinationId = std::move(value); }

    /**
     * <p>The ID of the publishing destination to delete.</p>
     */
    inline void SetDestinationId(const char* value) { m_destinationIdHasBeenSet = true; m_destinationId.assign(value); }

    /**
     * <p>The ID of the publishing destination to delete.</p>
     */
    inline DeletePublishingDestinationRequest& WithDestinationId(const Aws::String& value) { SetDestinationId(value); return *this;}

    /**
     * <p>The ID of the publishing destination to delete.</p>
     */
    inline DeletePublishingDestinationRequest& WithDestinationId(Aws::String&& value) { SetDestinationId(std::move(value)); return *this;}

    /**
     * <p>The ID of the publishing destination to delete.</p>
     */
    inline DeletePublishingDestinationRequest& WithDestinationId(const char* value) { SetDestinationId(value); return *this;}

  private:

    Aws::String m_detectorId;
    bool m_detectorIdHasBeenSet = false;

    Aws::String m_destinationId;
    bool m_destinationIdHasBeenSet = false;
  };

} // namespace Model
} // namespace GuardDuty
} // namespace Aws
