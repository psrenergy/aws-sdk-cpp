﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/cloudtrail/CloudTrail_EXPORTS.h>
#include <aws/cloudtrail/CloudTrailRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CloudTrail
{
namespace Model
{

  /**
   */
  class AWS_CLOUDTRAIL_API GetEventDataStoreRequest : public CloudTrailRequest
  {
  public:
    GetEventDataStoreRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetEventDataStoreRequest> Clone() const
    {
      return Aws::MakeUnique<GetEventDataStoreRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetEventDataStore"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The ARN (or ID suffix of the ARN) of the event data store about which you
     * want information.</p>
     */
    inline const Aws::String& GetEventDataStore() const{ return m_eventDataStore; }

    /**
     * <p>The ARN (or ID suffix of the ARN) of the event data store about which you
     * want information.</p>
     */
    inline bool EventDataStoreHasBeenSet() const { return m_eventDataStoreHasBeenSet; }

    /**
     * <p>The ARN (or ID suffix of the ARN) of the event data store about which you
     * want information.</p>
     */
    inline void SetEventDataStore(const Aws::String& value) { m_eventDataStoreHasBeenSet = true; m_eventDataStore = value; }

    /**
     * <p>The ARN (or ID suffix of the ARN) of the event data store about which you
     * want information.</p>
     */
    inline void SetEventDataStore(Aws::String&& value) { m_eventDataStoreHasBeenSet = true; m_eventDataStore = std::move(value); }

    /**
     * <p>The ARN (or ID suffix of the ARN) of the event data store about which you
     * want information.</p>
     */
    inline void SetEventDataStore(const char* value) { m_eventDataStoreHasBeenSet = true; m_eventDataStore.assign(value); }

    /**
     * <p>The ARN (or ID suffix of the ARN) of the event data store about which you
     * want information.</p>
     */
    inline GetEventDataStoreRequest& WithEventDataStore(const Aws::String& value) { SetEventDataStore(value); return *this;}

    /**
     * <p>The ARN (or ID suffix of the ARN) of the event data store about which you
     * want information.</p>
     */
    inline GetEventDataStoreRequest& WithEventDataStore(Aws::String&& value) { SetEventDataStore(std::move(value)); return *this;}

    /**
     * <p>The ARN (or ID suffix of the ARN) of the event data store about which you
     * want information.</p>
     */
    inline GetEventDataStoreRequest& WithEventDataStore(const char* value) { SetEventDataStore(value); return *this;}

  private:

    Aws::String m_eventDataStore;
    bool m_eventDataStoreHasBeenSet = false;
  };

} // namespace Model
} // namespace CloudTrail
} // namespace Aws
