﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/ec2/EC2_EXPORTS.h>
#include <aws/ec2/EC2Request.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace EC2
{
namespace Model
{

  /**
   */
  class AWS_EC2_API DeprovisionIpamPoolCidrRequest : public EC2Request
  {
  public:
    DeprovisionIpamPoolCidrRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DeprovisionIpamPoolCidrRequest> Clone() const
    {
      return Aws::MakeUnique<DeprovisionIpamPoolCidrRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeprovisionIpamPoolCidr"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>A check for whether you have the required permissions for the action without
     * actually making the request and provides an error response. If you have the
     * required permissions, the error response is <code>DryRunOperation</code>.
     * Otherwise, it is <code>UnauthorizedOperation</code>.</p>
     */
    inline bool GetDryRun() const{ return m_dryRun; }

    /**
     * <p>A check for whether you have the required permissions for the action without
     * actually making the request and provides an error response. If you have the
     * required permissions, the error response is <code>DryRunOperation</code>.
     * Otherwise, it is <code>UnauthorizedOperation</code>.</p>
     */
    inline bool DryRunHasBeenSet() const { return m_dryRunHasBeenSet; }

    /**
     * <p>A check for whether you have the required permissions for the action without
     * actually making the request and provides an error response. If you have the
     * required permissions, the error response is <code>DryRunOperation</code>.
     * Otherwise, it is <code>UnauthorizedOperation</code>.</p>
     */
    inline void SetDryRun(bool value) { m_dryRunHasBeenSet = true; m_dryRun = value; }

    /**
     * <p>A check for whether you have the required permissions for the action without
     * actually making the request and provides an error response. If you have the
     * required permissions, the error response is <code>DryRunOperation</code>.
     * Otherwise, it is <code>UnauthorizedOperation</code>.</p>
     */
    inline DeprovisionIpamPoolCidrRequest& WithDryRun(bool value) { SetDryRun(value); return *this;}


    /**
     * <p>The ID of the pool that has the CIDR you want to deprovision.</p>
     */
    inline const Aws::String& GetIpamPoolId() const{ return m_ipamPoolId; }

    /**
     * <p>The ID of the pool that has the CIDR you want to deprovision.</p>
     */
    inline bool IpamPoolIdHasBeenSet() const { return m_ipamPoolIdHasBeenSet; }

    /**
     * <p>The ID of the pool that has the CIDR you want to deprovision.</p>
     */
    inline void SetIpamPoolId(const Aws::String& value) { m_ipamPoolIdHasBeenSet = true; m_ipamPoolId = value; }

    /**
     * <p>The ID of the pool that has the CIDR you want to deprovision.</p>
     */
    inline void SetIpamPoolId(Aws::String&& value) { m_ipamPoolIdHasBeenSet = true; m_ipamPoolId = std::move(value); }

    /**
     * <p>The ID of the pool that has the CIDR you want to deprovision.</p>
     */
    inline void SetIpamPoolId(const char* value) { m_ipamPoolIdHasBeenSet = true; m_ipamPoolId.assign(value); }

    /**
     * <p>The ID of the pool that has the CIDR you want to deprovision.</p>
     */
    inline DeprovisionIpamPoolCidrRequest& WithIpamPoolId(const Aws::String& value) { SetIpamPoolId(value); return *this;}

    /**
     * <p>The ID of the pool that has the CIDR you want to deprovision.</p>
     */
    inline DeprovisionIpamPoolCidrRequest& WithIpamPoolId(Aws::String&& value) { SetIpamPoolId(std::move(value)); return *this;}

    /**
     * <p>The ID of the pool that has the CIDR you want to deprovision.</p>
     */
    inline DeprovisionIpamPoolCidrRequest& WithIpamPoolId(const char* value) { SetIpamPoolId(value); return *this;}


    /**
     * <p>The CIDR which you want to deprovision from the pool.</p>
     */
    inline const Aws::String& GetCidr() const{ return m_cidr; }

    /**
     * <p>The CIDR which you want to deprovision from the pool.</p>
     */
    inline bool CidrHasBeenSet() const { return m_cidrHasBeenSet; }

    /**
     * <p>The CIDR which you want to deprovision from the pool.</p>
     */
    inline void SetCidr(const Aws::String& value) { m_cidrHasBeenSet = true; m_cidr = value; }

    /**
     * <p>The CIDR which you want to deprovision from the pool.</p>
     */
    inline void SetCidr(Aws::String&& value) { m_cidrHasBeenSet = true; m_cidr = std::move(value); }

    /**
     * <p>The CIDR which you want to deprovision from the pool.</p>
     */
    inline void SetCidr(const char* value) { m_cidrHasBeenSet = true; m_cidr.assign(value); }

    /**
     * <p>The CIDR which you want to deprovision from the pool.</p>
     */
    inline DeprovisionIpamPoolCidrRequest& WithCidr(const Aws::String& value) { SetCidr(value); return *this;}

    /**
     * <p>The CIDR which you want to deprovision from the pool.</p>
     */
    inline DeprovisionIpamPoolCidrRequest& WithCidr(Aws::String&& value) { SetCidr(std::move(value)); return *this;}

    /**
     * <p>The CIDR which you want to deprovision from the pool.</p>
     */
    inline DeprovisionIpamPoolCidrRequest& WithCidr(const char* value) { SetCidr(value); return *this;}

  private:

    bool m_dryRun;
    bool m_dryRunHasBeenSet = false;

    Aws::String m_ipamPoolId;
    bool m_ipamPoolIdHasBeenSet = false;

    Aws::String m_cidr;
    bool m_cidrHasBeenSet = false;
  };

} // namespace Model
} // namespace EC2
} // namespace Aws
