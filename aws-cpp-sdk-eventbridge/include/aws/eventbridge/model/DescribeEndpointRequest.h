﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/eventbridge/EventBridge_EXPORTS.h>
#include <aws/eventbridge/EventBridgeRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace EventBridge
{
namespace Model
{

  /**
   */
  class AWS_EVENTBRIDGE_API DescribeEndpointRequest : public EventBridgeRequest
  {
  public:
    DescribeEndpointRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeEndpoint"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The name of the endpoint you want to get information about. For example,
     * <code>"Name":"us-east-2-custom_bus_A-endpoint"</code>.</p>
     */
    inline const Aws::String& GetName() const{ return m_name; }

    /**
     * <p>The name of the endpoint you want to get information about. For example,
     * <code>"Name":"us-east-2-custom_bus_A-endpoint"</code>.</p>
     */
    inline bool NameHasBeenSet() const { return m_nameHasBeenSet; }

    /**
     * <p>The name of the endpoint you want to get information about. For example,
     * <code>"Name":"us-east-2-custom_bus_A-endpoint"</code>.</p>
     */
    inline void SetName(const Aws::String& value) { m_nameHasBeenSet = true; m_name = value; }

    /**
     * <p>The name of the endpoint you want to get information about. For example,
     * <code>"Name":"us-east-2-custom_bus_A-endpoint"</code>.</p>
     */
    inline void SetName(Aws::String&& value) { m_nameHasBeenSet = true; m_name = std::move(value); }

    /**
     * <p>The name of the endpoint you want to get information about. For example,
     * <code>"Name":"us-east-2-custom_bus_A-endpoint"</code>.</p>
     */
    inline void SetName(const char* value) { m_nameHasBeenSet = true; m_name.assign(value); }

    /**
     * <p>The name of the endpoint you want to get information about. For example,
     * <code>"Name":"us-east-2-custom_bus_A-endpoint"</code>.</p>
     */
    inline DescribeEndpointRequest& WithName(const Aws::String& value) { SetName(value); return *this;}

    /**
     * <p>The name of the endpoint you want to get information about. For example,
     * <code>"Name":"us-east-2-custom_bus_A-endpoint"</code>.</p>
     */
    inline DescribeEndpointRequest& WithName(Aws::String&& value) { SetName(std::move(value)); return *this;}

    /**
     * <p>The name of the endpoint you want to get information about. For example,
     * <code>"Name":"us-east-2-custom_bus_A-endpoint"</code>.</p>
     */
    inline DescribeEndpointRequest& WithName(const char* value) { SetName(value); return *this;}


    /**
     * <p>The primary Region of the endpoint you want to get information about. For
     * example <code>"HomeRegion": "us-east-1"</code>.</p>
     */
    inline const Aws::String& GetHomeRegion() const{ return m_homeRegion; }

    /**
     * <p>The primary Region of the endpoint you want to get information about. For
     * example <code>"HomeRegion": "us-east-1"</code>.</p>
     */
    inline bool HomeRegionHasBeenSet() const { return m_homeRegionHasBeenSet; }

    /**
     * <p>The primary Region of the endpoint you want to get information about. For
     * example <code>"HomeRegion": "us-east-1"</code>.</p>
     */
    inline void SetHomeRegion(const Aws::String& value) { m_homeRegionHasBeenSet = true; m_homeRegion = value; }

    /**
     * <p>The primary Region of the endpoint you want to get information about. For
     * example <code>"HomeRegion": "us-east-1"</code>.</p>
     */
    inline void SetHomeRegion(Aws::String&& value) { m_homeRegionHasBeenSet = true; m_homeRegion = std::move(value); }

    /**
     * <p>The primary Region of the endpoint you want to get information about. For
     * example <code>"HomeRegion": "us-east-1"</code>.</p>
     */
    inline void SetHomeRegion(const char* value) { m_homeRegionHasBeenSet = true; m_homeRegion.assign(value); }

    /**
     * <p>The primary Region of the endpoint you want to get information about. For
     * example <code>"HomeRegion": "us-east-1"</code>.</p>
     */
    inline DescribeEndpointRequest& WithHomeRegion(const Aws::String& value) { SetHomeRegion(value); return *this;}

    /**
     * <p>The primary Region of the endpoint you want to get information about. For
     * example <code>"HomeRegion": "us-east-1"</code>.</p>
     */
    inline DescribeEndpointRequest& WithHomeRegion(Aws::String&& value) { SetHomeRegion(std::move(value)); return *this;}

    /**
     * <p>The primary Region of the endpoint you want to get information about. For
     * example <code>"HomeRegion": "us-east-1"</code>.</p>
     */
    inline DescribeEndpointRequest& WithHomeRegion(const char* value) { SetHomeRegion(value); return *this;}

  private:

    Aws::String m_name;
    bool m_nameHasBeenSet = false;

    Aws::String m_homeRegion;
    bool m_homeRegionHasBeenSet = false;
  };

} // namespace Model
} // namespace EventBridge
} // namespace Aws
