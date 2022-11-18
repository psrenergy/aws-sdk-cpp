﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/cloudfront/CloudFront_EXPORTS.h>
#include <aws/cloudfront/CloudFrontRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CloudFront
{
namespace Model
{

  /**
   */
  class AWS_CLOUDFRONT_API GetOriginRequestPolicy2020_05_31Request : public CloudFrontRequest
  {
  public:
    GetOriginRequestPolicy2020_05_31Request();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetOriginRequestPolicy2020_05_31Request> Clone() const
    {
      return Aws::MakeUnique<GetOriginRequestPolicy2020_05_31Request>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetOriginRequestPolicy"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The unique identifier for the origin request policy. If the origin request
     * policy is attached to a distribution’s cache behavior, you can get the policy’s
     * identifier using <code>ListDistributions</code> or <code>GetDistribution</code>.
     * If the origin request policy is not attached to a cache behavior, you can get
     * the identifier using <code>ListOriginRequestPolicies</code>.</p>
     */
    inline const Aws::String& GetId() const{ return m_id; }

    /**
     * <p>The unique identifier for the origin request policy. If the origin request
     * policy is attached to a distribution’s cache behavior, you can get the policy’s
     * identifier using <code>ListDistributions</code> or <code>GetDistribution</code>.
     * If the origin request policy is not attached to a cache behavior, you can get
     * the identifier using <code>ListOriginRequestPolicies</code>.</p>
     */
    inline bool IdHasBeenSet() const { return m_idHasBeenSet; }

    /**
     * <p>The unique identifier for the origin request policy. If the origin request
     * policy is attached to a distribution’s cache behavior, you can get the policy’s
     * identifier using <code>ListDistributions</code> or <code>GetDistribution</code>.
     * If the origin request policy is not attached to a cache behavior, you can get
     * the identifier using <code>ListOriginRequestPolicies</code>.</p>
     */
    inline void SetId(const Aws::String& value) { m_idHasBeenSet = true; m_id = value; }

    /**
     * <p>The unique identifier for the origin request policy. If the origin request
     * policy is attached to a distribution’s cache behavior, you can get the policy’s
     * identifier using <code>ListDistributions</code> or <code>GetDistribution</code>.
     * If the origin request policy is not attached to a cache behavior, you can get
     * the identifier using <code>ListOriginRequestPolicies</code>.</p>
     */
    inline void SetId(Aws::String&& value) { m_idHasBeenSet = true; m_id = std::move(value); }

    /**
     * <p>The unique identifier for the origin request policy. If the origin request
     * policy is attached to a distribution’s cache behavior, you can get the policy’s
     * identifier using <code>ListDistributions</code> or <code>GetDistribution</code>.
     * If the origin request policy is not attached to a cache behavior, you can get
     * the identifier using <code>ListOriginRequestPolicies</code>.</p>
     */
    inline void SetId(const char* value) { m_idHasBeenSet = true; m_id.assign(value); }

    /**
     * <p>The unique identifier for the origin request policy. If the origin request
     * policy is attached to a distribution’s cache behavior, you can get the policy’s
     * identifier using <code>ListDistributions</code> or <code>GetDistribution</code>.
     * If the origin request policy is not attached to a cache behavior, you can get
     * the identifier using <code>ListOriginRequestPolicies</code>.</p>
     */
    inline GetOriginRequestPolicy2020_05_31Request& WithId(const Aws::String& value) { SetId(value); return *this;}

    /**
     * <p>The unique identifier for the origin request policy. If the origin request
     * policy is attached to a distribution’s cache behavior, you can get the policy’s
     * identifier using <code>ListDistributions</code> or <code>GetDistribution</code>.
     * If the origin request policy is not attached to a cache behavior, you can get
     * the identifier using <code>ListOriginRequestPolicies</code>.</p>
     */
    inline GetOriginRequestPolicy2020_05_31Request& WithId(Aws::String&& value) { SetId(std::move(value)); return *this;}

    /**
     * <p>The unique identifier for the origin request policy. If the origin request
     * policy is attached to a distribution’s cache behavior, you can get the policy’s
     * identifier using <code>ListDistributions</code> or <code>GetDistribution</code>.
     * If the origin request policy is not attached to a cache behavior, you can get
     * the identifier using <code>ListOriginRequestPolicies</code>.</p>
     */
    inline GetOriginRequestPolicy2020_05_31Request& WithId(const char* value) { SetId(value); return *this;}

  private:

    Aws::String m_id;
    bool m_idHasBeenSet = false;
  };

} // namespace Model
} // namespace CloudFront
} // namespace Aws
