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
  class AWS_EC2_API DetachInternetGatewayRequest : public EC2Request
  {
  public:
    DetachInternetGatewayRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DetachInternetGatewayRequest> Clone() const
    {
      return Aws::MakeUnique<DetachInternetGatewayRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DetachInternetGateway"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>Checks whether you have the required permissions for the action, without
     * actually making the request, and provides an error response. If you have the
     * required permissions, the error response is <code>DryRunOperation</code>.
     * Otherwise, it is <code>UnauthorizedOperation</code>.</p>
     */
    inline bool GetDryRun() const{ return m_dryRun; }

    /**
     * <p>Checks whether you have the required permissions for the action, without
     * actually making the request, and provides an error response. If you have the
     * required permissions, the error response is <code>DryRunOperation</code>.
     * Otherwise, it is <code>UnauthorizedOperation</code>.</p>
     */
    inline bool DryRunHasBeenSet() const { return m_dryRunHasBeenSet; }

    /**
     * <p>Checks whether you have the required permissions for the action, without
     * actually making the request, and provides an error response. If you have the
     * required permissions, the error response is <code>DryRunOperation</code>.
     * Otherwise, it is <code>UnauthorizedOperation</code>.</p>
     */
    inline void SetDryRun(bool value) { m_dryRunHasBeenSet = true; m_dryRun = value; }

    /**
     * <p>Checks whether you have the required permissions for the action, without
     * actually making the request, and provides an error response. If you have the
     * required permissions, the error response is <code>DryRunOperation</code>.
     * Otherwise, it is <code>UnauthorizedOperation</code>.</p>
     */
    inline DetachInternetGatewayRequest& WithDryRun(bool value) { SetDryRun(value); return *this;}


    /**
     * <p>The ID of the internet gateway.</p>
     */
    inline const Aws::String& GetInternetGatewayId() const{ return m_internetGatewayId; }

    /**
     * <p>The ID of the internet gateway.</p>
     */
    inline bool InternetGatewayIdHasBeenSet() const { return m_internetGatewayIdHasBeenSet; }

    /**
     * <p>The ID of the internet gateway.</p>
     */
    inline void SetInternetGatewayId(const Aws::String& value) { m_internetGatewayIdHasBeenSet = true; m_internetGatewayId = value; }

    /**
     * <p>The ID of the internet gateway.</p>
     */
    inline void SetInternetGatewayId(Aws::String&& value) { m_internetGatewayIdHasBeenSet = true; m_internetGatewayId = std::move(value); }

    /**
     * <p>The ID of the internet gateway.</p>
     */
    inline void SetInternetGatewayId(const char* value) { m_internetGatewayIdHasBeenSet = true; m_internetGatewayId.assign(value); }

    /**
     * <p>The ID of the internet gateway.</p>
     */
    inline DetachInternetGatewayRequest& WithInternetGatewayId(const Aws::String& value) { SetInternetGatewayId(value); return *this;}

    /**
     * <p>The ID of the internet gateway.</p>
     */
    inline DetachInternetGatewayRequest& WithInternetGatewayId(Aws::String&& value) { SetInternetGatewayId(std::move(value)); return *this;}

    /**
     * <p>The ID of the internet gateway.</p>
     */
    inline DetachInternetGatewayRequest& WithInternetGatewayId(const char* value) { SetInternetGatewayId(value); return *this;}


    /**
     * <p>The ID of the VPC.</p>
     */
    inline const Aws::String& GetVpcId() const{ return m_vpcId; }

    /**
     * <p>The ID of the VPC.</p>
     */
    inline bool VpcIdHasBeenSet() const { return m_vpcIdHasBeenSet; }

    /**
     * <p>The ID of the VPC.</p>
     */
    inline void SetVpcId(const Aws::String& value) { m_vpcIdHasBeenSet = true; m_vpcId = value; }

    /**
     * <p>The ID of the VPC.</p>
     */
    inline void SetVpcId(Aws::String&& value) { m_vpcIdHasBeenSet = true; m_vpcId = std::move(value); }

    /**
     * <p>The ID of the VPC.</p>
     */
    inline void SetVpcId(const char* value) { m_vpcIdHasBeenSet = true; m_vpcId.assign(value); }

    /**
     * <p>The ID of the VPC.</p>
     */
    inline DetachInternetGatewayRequest& WithVpcId(const Aws::String& value) { SetVpcId(value); return *this;}

    /**
     * <p>The ID of the VPC.</p>
     */
    inline DetachInternetGatewayRequest& WithVpcId(Aws::String&& value) { SetVpcId(std::move(value)); return *this;}

    /**
     * <p>The ID of the VPC.</p>
     */
    inline DetachInternetGatewayRequest& WithVpcId(const char* value) { SetVpcId(value); return *this;}

  private:

    bool m_dryRun;
    bool m_dryRunHasBeenSet = false;

    Aws::String m_internetGatewayId;
    bool m_internetGatewayIdHasBeenSet = false;

    Aws::String m_vpcId;
    bool m_vpcIdHasBeenSet = false;
  };

} // namespace Model
} // namespace EC2
} // namespace Aws
