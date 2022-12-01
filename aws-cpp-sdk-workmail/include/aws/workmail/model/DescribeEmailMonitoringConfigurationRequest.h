﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/workmail/WorkMail_EXPORTS.h>
#include <aws/workmail/WorkMailRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace WorkMail
{
namespace Model
{

  /**
   */
  class AWS_WORKMAIL_API DescribeEmailMonitoringConfigurationRequest : public WorkMailRequest
  {
  public:
    DescribeEmailMonitoringConfigurationRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DescribeEmailMonitoringConfigurationRequest> Clone() const
    {
      return Aws::MakeUnique<DescribeEmailMonitoringConfigurationRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeEmailMonitoringConfiguration"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The ID of the organization for which the email monitoring configuration is
     * described.</p>
     */
    inline const Aws::String& GetOrganizationId() const{ return m_organizationId; }

    /**
     * <p>The ID of the organization for which the email monitoring configuration is
     * described.</p>
     */
    inline bool OrganizationIdHasBeenSet() const { return m_organizationIdHasBeenSet; }

    /**
     * <p>The ID of the organization for which the email monitoring configuration is
     * described.</p>
     */
    inline void SetOrganizationId(const Aws::String& value) { m_organizationIdHasBeenSet = true; m_organizationId = value; }

    /**
     * <p>The ID of the organization for which the email monitoring configuration is
     * described.</p>
     */
    inline void SetOrganizationId(Aws::String&& value) { m_organizationIdHasBeenSet = true; m_organizationId = std::move(value); }

    /**
     * <p>The ID of the organization for which the email monitoring configuration is
     * described.</p>
     */
    inline void SetOrganizationId(const char* value) { m_organizationIdHasBeenSet = true; m_organizationId.assign(value); }

    /**
     * <p>The ID of the organization for which the email monitoring configuration is
     * described.</p>
     */
    inline DescribeEmailMonitoringConfigurationRequest& WithOrganizationId(const Aws::String& value) { SetOrganizationId(value); return *this;}

    /**
     * <p>The ID of the organization for which the email monitoring configuration is
     * described.</p>
     */
    inline DescribeEmailMonitoringConfigurationRequest& WithOrganizationId(Aws::String&& value) { SetOrganizationId(std::move(value)); return *this;}

    /**
     * <p>The ID of the organization for which the email monitoring configuration is
     * described.</p>
     */
    inline DescribeEmailMonitoringConfigurationRequest& WithOrganizationId(const char* value) { SetOrganizationId(value); return *this;}

  private:

    Aws::String m_organizationId;
    bool m_organizationIdHasBeenSet = false;
  };

} // namespace Model
} // namespace WorkMail
} // namespace Aws
