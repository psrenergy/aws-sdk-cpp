﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/devicefarm/DeviceFarm_EXPORTS.h>
#include <aws/devicefarm/DeviceFarmRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace DeviceFarm
{
namespace Model
{

  /**
   */
  class AWS_DEVICEFARM_API GetNetworkProfileRequest : public DeviceFarmRequest
  {
  public:
    GetNetworkProfileRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetNetworkProfileRequest> Clone() const
    {
      return Aws::MakeUnique<GetNetworkProfileRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetNetworkProfile"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The ARN of the network profile to return information about.</p>
     */
    inline const Aws::String& GetArn() const{ return m_arn; }

    /**
     * <p>The ARN of the network profile to return information about.</p>
     */
    inline bool ArnHasBeenSet() const { return m_arnHasBeenSet; }

    /**
     * <p>The ARN of the network profile to return information about.</p>
     */
    inline void SetArn(const Aws::String& value) { m_arnHasBeenSet = true; m_arn = value; }

    /**
     * <p>The ARN of the network profile to return information about.</p>
     */
    inline void SetArn(Aws::String&& value) { m_arnHasBeenSet = true; m_arn = std::move(value); }

    /**
     * <p>The ARN of the network profile to return information about.</p>
     */
    inline void SetArn(const char* value) { m_arnHasBeenSet = true; m_arn.assign(value); }

    /**
     * <p>The ARN of the network profile to return information about.</p>
     */
    inline GetNetworkProfileRequest& WithArn(const Aws::String& value) { SetArn(value); return *this;}

    /**
     * <p>The ARN of the network profile to return information about.</p>
     */
    inline GetNetworkProfileRequest& WithArn(Aws::String&& value) { SetArn(std::move(value)); return *this;}

    /**
     * <p>The ARN of the network profile to return information about.</p>
     */
    inline GetNetworkProfileRequest& WithArn(const char* value) { SetArn(value); return *this;}

  private:

    Aws::String m_arn;
    bool m_arnHasBeenSet = false;
  };

} // namespace Model
} // namespace DeviceFarm
} // namespace Aws
