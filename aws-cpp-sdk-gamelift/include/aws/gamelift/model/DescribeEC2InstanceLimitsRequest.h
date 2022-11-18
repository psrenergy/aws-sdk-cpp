﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/gamelift/GameLift_EXPORTS.h>
#include <aws/gamelift/GameLiftRequest.h>
#include <aws/gamelift/model/EC2InstanceType.h>
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
   * href="http://docs.aws.amazon.com/goto/WebAPI/gamelift-2015-10-01/DescribeEC2InstanceLimitsInput">AWS
   * API Reference</a></p>
   */
  class AWS_GAMELIFT_API DescribeEC2InstanceLimitsRequest : public GameLiftRequest
  {
  public:
    DescribeEC2InstanceLimitsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DescribeEC2InstanceLimitsRequest> Clone() const
    {
      return Aws::MakeUnique<DescribeEC2InstanceLimitsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeEC2InstanceLimits"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>Name of an Amazon EC2 instance type that is supported in GameLift. A fleet
     * instance type determines the computing resources of each instance in the fleet,
     * including CPU, memory, storage, and networking capacity. Do not specify a value
     * for this parameter to retrieve limits for all instance types.</p>
     */
    inline const EC2InstanceType& GetEC2InstanceType() const{ return m_eC2InstanceType; }

    /**
     * <p>Name of an Amazon EC2 instance type that is supported in GameLift. A fleet
     * instance type determines the computing resources of each instance in the fleet,
     * including CPU, memory, storage, and networking capacity. Do not specify a value
     * for this parameter to retrieve limits for all instance types.</p>
     */
    inline bool EC2InstanceTypeHasBeenSet() const { return m_eC2InstanceTypeHasBeenSet; }

    /**
     * <p>Name of an Amazon EC2 instance type that is supported in GameLift. A fleet
     * instance type determines the computing resources of each instance in the fleet,
     * including CPU, memory, storage, and networking capacity. Do not specify a value
     * for this parameter to retrieve limits for all instance types.</p>
     */
    inline void SetEC2InstanceType(const EC2InstanceType& value) { m_eC2InstanceTypeHasBeenSet = true; m_eC2InstanceType = value; }

    /**
     * <p>Name of an Amazon EC2 instance type that is supported in GameLift. A fleet
     * instance type determines the computing resources of each instance in the fleet,
     * including CPU, memory, storage, and networking capacity. Do not specify a value
     * for this parameter to retrieve limits for all instance types.</p>
     */
    inline void SetEC2InstanceType(EC2InstanceType&& value) { m_eC2InstanceTypeHasBeenSet = true; m_eC2InstanceType = std::move(value); }

    /**
     * <p>Name of an Amazon EC2 instance type that is supported in GameLift. A fleet
     * instance type determines the computing resources of each instance in the fleet,
     * including CPU, memory, storage, and networking capacity. Do not specify a value
     * for this parameter to retrieve limits for all instance types.</p>
     */
    inline DescribeEC2InstanceLimitsRequest& WithEC2InstanceType(const EC2InstanceType& value) { SetEC2InstanceType(value); return *this;}

    /**
     * <p>Name of an Amazon EC2 instance type that is supported in GameLift. A fleet
     * instance type determines the computing resources of each instance in the fleet,
     * including CPU, memory, storage, and networking capacity. Do not specify a value
     * for this parameter to retrieve limits for all instance types.</p>
     */
    inline DescribeEC2InstanceLimitsRequest& WithEC2InstanceType(EC2InstanceType&& value) { SetEC2InstanceType(std::move(value)); return *this;}


    /**
     * <p>The name of a remote location to request instance limits for, in the form of
     * an Amazon Web Services Region code such as <code>us-west-2</code>.</p>
     */
    inline const Aws::String& GetLocation() const{ return m_location; }

    /**
     * <p>The name of a remote location to request instance limits for, in the form of
     * an Amazon Web Services Region code such as <code>us-west-2</code>.</p>
     */
    inline bool LocationHasBeenSet() const { return m_locationHasBeenSet; }

    /**
     * <p>The name of a remote location to request instance limits for, in the form of
     * an Amazon Web Services Region code such as <code>us-west-2</code>.</p>
     */
    inline void SetLocation(const Aws::String& value) { m_locationHasBeenSet = true; m_location = value; }

    /**
     * <p>The name of a remote location to request instance limits for, in the form of
     * an Amazon Web Services Region code such as <code>us-west-2</code>.</p>
     */
    inline void SetLocation(Aws::String&& value) { m_locationHasBeenSet = true; m_location = std::move(value); }

    /**
     * <p>The name of a remote location to request instance limits for, in the form of
     * an Amazon Web Services Region code such as <code>us-west-2</code>.</p>
     */
    inline void SetLocation(const char* value) { m_locationHasBeenSet = true; m_location.assign(value); }

    /**
     * <p>The name of a remote location to request instance limits for, in the form of
     * an Amazon Web Services Region code such as <code>us-west-2</code>.</p>
     */
    inline DescribeEC2InstanceLimitsRequest& WithLocation(const Aws::String& value) { SetLocation(value); return *this;}

    /**
     * <p>The name of a remote location to request instance limits for, in the form of
     * an Amazon Web Services Region code such as <code>us-west-2</code>.</p>
     */
    inline DescribeEC2InstanceLimitsRequest& WithLocation(Aws::String&& value) { SetLocation(std::move(value)); return *this;}

    /**
     * <p>The name of a remote location to request instance limits for, in the form of
     * an Amazon Web Services Region code such as <code>us-west-2</code>.</p>
     */
    inline DescribeEC2InstanceLimitsRequest& WithLocation(const char* value) { SetLocation(value); return *this;}

  private:

    EC2InstanceType m_eC2InstanceType;
    bool m_eC2InstanceTypeHasBeenSet = false;

    Aws::String m_location;
    bool m_locationHasBeenSet = false;
  };

} // namespace Model
} // namespace GameLift
} // namespace Aws
